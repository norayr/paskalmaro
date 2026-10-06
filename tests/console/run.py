"""Behavioral and dependency checks for the optional Pascal console backend."""
import ctypes
import os
from pathlib import Path
import shlex
import subprocess
import tempfile

sources = Path(__file__).resolve().parent
root = sources.parents[1]
runtime = root / "runtime"
ptc = root / "ptc"
cc = shlex.split(os.environ.get("CC", "cc"))


def run(args, **kwargs):
    return subprocess.run(args, check=True, timeout=10, **kwargs)


with tempfile.TemporaryDirectory(dir=sources) as temporary:
    build = Path(temporary)

    def translate(name, flags=("-c",), text=None):
        result = run([str(ptc), *flags], input=text or (sources / (name + ".p")).read_text(),
                     capture_output=True, text=True)
        generated = build / (name + ".c")
        generated.write_text(result.stdout)
        assert "<stdio.h>" not in result.stdout and "FILE" not in result.stdout
        assert "fprintf(" not in result.stdout and "fscanf(" not in result.stdout
        assert "<stdlib.h>" not in result.stdout
        return generated

    def compile_source(name, flags=("-c",), text=None, extra=()):
        generated = translate(name, flags, text)
        binary = build / name
        run([*cc, "-std=c89", "-O2", "-I" + str(runtime), str(generated),
             str(runtime / "ptc_console_input.c"), str(runtime / "ptc_console_output.c"),
             str(runtime / "ptc_console_host.c"), *map(str, extra), "-o", str(binary)])
        return binary

    def check(binary, data, expected=None, code=0):
        result = subprocess.run([str(binary)], input=data, capture_output=True, timeout=5)
        assert result.returncode == code, (binary.name, data, result.returncode, result.stderr)
        if expected is not None:
            assert result.stdout == expected, (binary.name, data, result.stdout, expected)
        if code:
            assert b"Console I/O:" in result.stderr

    output = compile_source("output")
    check(output, b"", b"literal%:true  false  Z|  -7|-7|  A%B!|65535\ncalls=3\n\n\fdone1\n")
    # The same native-width formatting path is emitted for 16-bit targets.
    output16 = compile_source("output", ("-c", "-i16", "-r"))
    check(output16, b"", b"literal%:true  false  Z|  -7|-7|  A%B!|65535\ncalls=3\n\n\fdone1\n")
    lines = compile_source("lines")
    for data, prefix in ((b"", b""), (b"\n", b"|\n"), (b"abc", b"abc|\n"),
                         (b"abc\nlast", b"abc|\nlast|\n"),
                         (b"a\r\nb\rc\n", b"a|\nb|\nc|\n"),
                         (b"a\r\n\r\nb", b"a|\n|\nb|\n")):
        check(lines, data, prefix + b"true true true\nend\n")

    numbers = compile_source("numbers")
    int_max = 2 ** (ctypes.sizeof(ctypes.c_int) * 8 - 1) - 1
    int_min = -int_max - 1
    for n in (0, -1, int_min, int_max):
        data = f"{n} +65535 255 -10 trailing".encode()
        check(numbers, data, f"{n},65535,255,-10\n".encode())
    for data, code in ((b"", 1), (b"- 1 2 3", 2), (b"abc", 2),
                       (f"{int_max + 1} 0 0 0".encode(), 3),
                       (f"{int_min - 1} 0 0 0".encode(), 3),
                       (b"0 65536 0 0", 3), (b"0 0 256 0", 3),
                       (b"0 0 0 -11", 3)):
        check(numbers, data, code=code)
    words = compile_source("words")
    check(words, b"abc X", b"[abc   ] X\n")
    check(words, b"abcdef X", b"[abcdef] X\n")
    check(words, b"abcdefg X", code=5)
    check(words, b"\t\n", code=1)
    arrays = compile_source("arrays")
    check(arrays, b"abc", b"[  abc ]\n")
    library = translate("library")
    separate = compile_source("separate", extra=(library,))
    check(separate, b"AB", b"AB\n")
    check(separate, b"", b"")

    fixed_bytes = compile_source("fixed_bytes", text=
        "program fixedbytes(output); var a: array [1..3] of char; "
        "begin a[1] := 'A'; a[2] := chr(0); a[3] := 'B'; writeln(a:5) end.\n")
    check(fixed_bytes, b"", b"  A\0B\n")
    character_bytes = compile_source("character_bytes", text=
        "program characterbytes(input, output); var c: char; "
        "begin while not eof do begin read(c); write(c) end end.\n")
    check(character_bytes, b"\x80\xff\0", b"\x80\xff\0")
    check(character_bytes, b"\n", b" ")

    negative_width = compile_source("negative_width", text=
        "program negativewidth(output); var w: integer; "
        "begin w := -1; writeln(1:w) end.\n")
    check(negative_width, b"", code=3)

    # An instrumented adapter verifies no next-line prefetch and sticky EOF.
    probe = build / "probe_adapter.c"
    probe.write_text('''#include <assert.h>
#include <stdlib.h>
#include "ptc_console.h"
static char input[] = "ab\\r\\nZ";
static int calls, offset;
int PtcGetChar(void) {
#ifdef FAIL_GETCHAR
    return -2;
#else
    calls++; return input[offset] ? input[offset++] : -1;
#endif
}
void PtcPutChar(int c) { (void)c; }
void PtcFlush(void) { }
void PtcConsoleFail(int c) { exit(c); }
void probe(int expected) { assert(calls == expected); }
''')
    generated = translate("probe", text="program lazyinput(input); var ch: char; "
        "procedure probe(n: integer); external; begin readln; probe(3); "
        "read(ch); probe(5); if not eof then probe(99); probe(6); "
        "if not eof then probe(99); readln; probe(6) end.\n")
    binary = build / "probe"
    run([*cc, "-std=c89", "-I" + str(runtime), str(generated), str(probe),
         str(runtime / "ptc_console_input.c"), "-o", str(binary)])
    check(binary, b"")
    run([*cc, "-std=c89", "-DFAIL_GETCHAR", "-I" + str(runtime), str(generated),
         str(probe), str(runtime / "ptc_console_input.c"), "-o", str(binary)])
    result = subprocess.run([str(binary)], capture_output=True, timeout=5)
    assert result.returncode == 4, result

    # Only adapter symbols may be undefined in the freestanding helper objects.
    for part in ("input", "output"):
        obj = build / (part + ".o")
        run([*cc, "-std=c89", "-ffreestanding", "-fno-builtin", "-O2", "-c",
             str(runtime / ("ptc_console_" + part + ".c")), "-o", str(obj)])
        symbols = run(["nm", "-u", str(obj)], capture_output=True, text=True).stdout
        allowed = {"PtcGetChar", "PtcPutChar", "PtcConsoleFail"}
        assert all(line.split()[-1] in allowed for line in symbols.splitlines()), symbols

    for source, flags in (("program p; var f: text; begin end.\n", ("-c",)),
                          ("program p; var f: file of integer; begin end.\n", ("-c",)),
                          ("program p(input); begin get(input) end.\n", ("-c",)),
                          ("program p(output); begin writeln(1.5) end.\n", ("-c",)),
                          ("program p(input); var b: boolean; begin read(b) end.\n", ("-c",)),
                          ("program p(output); begin write(input, 'x') end.\n", ("-c",)),
                          ("program p(output); begin write(48000) end.\n", ("-c", "-i16")),
                          ("program p(output); var n: -40000..40000; begin write(n) end.\n",
                           ("-c", "-i16")),
                          ("program p(output); var w: 0..65536; begin w := 65536; write(1:w) end.\n",
                           ("-c", "-i16"))):
        result = subprocess.run([str(ptc), *flags], input=source, capture_output=True,
                                text=True, timeout=10)
        assert result.returncode != 0 and "console" in result.stderr.lower(), result
    print("PASS: console formatting, native limits, buffered input, errors, separate files, and dependencies")
