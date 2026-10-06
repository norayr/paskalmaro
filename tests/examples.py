"""End-to-end modern filters and an Oric memory/keyboard simulation."""
from pathlib import Path
import os
import shlex
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
cc = shlex.split(os.environ.get("CC", "cc"))


def run(args, **kwargs):
    return subprocess.run(args, check=True, timeout=10, **kwargs)


for example in ("textstats", "fletcher16"):
    run(["make", "-C", str(root / "examples/modern" / example)])

stats = root / "examples/modern/textstats/textstats"
for data, expected in ((b"", b"lines=0 words=0 bytes=0\n"),
                       (b"hello world\n", b"lines=1 words=2 bytes=12\n"),
                       (b"one\ttwo\nthree", b"lines=2 words=3 bytes=13\n"),
                       (b"\n\n", b"lines=2 words=0 bytes=2\n")):
    result = run([str(stats)], input=data, capture_output=True)
    assert result.stdout == expected, (data, result.stdout)

checksum = root / "examples/modern/fletcher16/fletcher16"
for data in (b"", b"abcde", bytes(range(256)), bytes(range(256)) * 10):
    sum1 = sum2 = 0
    for value in data:
        sum1 = (sum1 + value) % 255
        sum2 = (sum2 + sum1) % 255
    result = run([str(checksum)], input=data, capture_output=True)
    assert int(result.stdout) == sum2 * 256 + sum1
print("PASS: modern text and binary filters")

run(["make", "-C", str(root / "examples/console")])
result = run([str(root / "examples/console/console-demo")],
             input=b"Ada\n-42\n", capture_output=True)
assert result.stdout == (b"Pascal console backend\n"
                         b"Name (one word, max 12): Signed integer: "
                         b"Hello, [Ada         ]\nNumber:    -42\nPositive: false\n"), result.stdout
print("PASS: hosted interactive-console example with redirected input")

with tempfile.TemporaryDirectory(dir=root / "tests") as temporary:
    build = Path(temporary)
    example = root / "examples/oric/lores"
    for name in ("demo", "lores"):
        with (example / (name + ".p")).open() as source, (build / (name + ".c")).open("w") as out:
            run([str(root / "ptc"), "-m", "-r"], stdin=source, stdout=out)
    bridge = build / "bridge.c"
    bridge.write_text('''#include <assert.h>
#include <stdlib.h>
static unsigned char memory[1120];
static char keys[] = "cawdsq";
static int nextkey;
void oricpoke(unsigned int address, int value)
{
    assert(address >= 48000 && address <= 49119);
    assert(value >= 0 && value <= 255);
    memory[address - 48000] = (unsigned char)value;
}
int orickey(void)
{
    if (keys[nextkey] == 'q') {
        assert(memory[1119] == 26);
        assert(memory[13 * 40 + 2 + 5 * 3] == 19);
        assert(memory[13 * 40 + 2 + 5 * 3 + 1] == 32);
        assert(memory[13 * 40 + 2 + 5 * 3 + 2] == 16);
        assert(memory[13 * 40 + 2 + 4 * 3] == 16);
        assert(memory[12 * 40 + 2 + 5 * 3] == 16);
    }
    assert(nextkey < 6);
    return keys[nextkey++];
}
void PtcFail(int code) { exit(100 + code); }
''')
    binary = build / "demo"
    run([*cc, "-std=c89", "-DPTC_CUSTOM_FAIL", str(build / "demo.c"),
         str(build / "lores.c"), str(bridge), "-o", str(binary)])
    run([str(binary)])
print("PASS: separate Pascal LORES library, movement, colours, and screen bounds")
