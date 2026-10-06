"""Behavioral checks for generated, linked, and library-free runtime modes."""
import os
from pathlib import Path
import shlex
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
ptc = Path(os.environ.get("PTC", str(root / "ptc"))).resolve()
cc = shlex.split(os.environ.get("CC", "cc"))
sources = Path(__file__).resolve().parent


def run(args, **kwargs):
    return subprocess.run(args, check=True, **kwargs)


with tempfile.TemporaryDirectory(dir=sources) as temporary:
    build = Path(temporary)
    for mode in ([], ["-m"], ["-r"], ["-e"], ["-m", "-r"], ["-e", "-m"]):
        extra = [str(root / "runtime/ptc_checks.c")] if "-e" in mode else []
        custom = build / "custom.c"
        custom.write_text(
            "#include <stdlib.h>\n"
            "void PtcFail(int code) { exit(100 + code); }\n"
            "void verify(int value) { if (value != 102) exit(9); }\n"
        )
        for name, expected in (("07_single_evaluation", "1 42\n1 17\n"),
                               ("08_conformant_forward", "42\n"),
                               ("09_minimal", "")):
            generated = build / "program.c"
            with (sources / (name + ".p")).open() as source, generated.open("w") as out:
                run([str(ptc), *mode], stdin=source, stdout=out)
            binary = build / "program"
            run([*cc, "-std=c89", "-DPTC_CUSTOM_FAIL", "-I" + str(root / "runtime"),
                 str(generated), str(custom), *extra, "-o", str(binary)])
            result = run([str(binary)], capture_output=True, text=True)
            assert result.stdout == expected, (mode, name, result.stdout)
            if name == "09_minimal" and "-m" in mode:
                obj = build / "minimal.o"
                run([*cc, "-std=c89", "-ffreestanding", "-fno-builtin",
                     "-DPTC_CUSTOM_FAIL", "-I" + str(root / "runtime"),
                     "-c", str(generated), "-o", str(obj)])
                symbols = run(["nm", "-u", str(obj)], capture_output=True, text=True).stdout
                assert not any(lib in symbols for lib in
                               ("exit", "printf", "strncpy", "malloc", "signal")), symbols
        if "-r" in mode or "-e" in mode:
            for name, code in (("01_nil_deref", 1), ("02_nil_arrow", 1),
                               ("03_array_bounds_hi", 2), ("04_array_bounds_lo", 2),
                               ("06_confarr_bounds", 2)):
                with (sources / (name + ".p")).open() as source, generated.open("w") as out:
                    run([str(ptc), *mode], stdin=source, stdout=out)
                run([*cc, "-std=c89", "-DPTC_CUSTOM_FAIL", "-I" + str(root / "runtime"),
                     str(generated), str(custom), *extra, "-o", str(binary)])
                result = subprocess.run([str(binary)], capture_output=True)
                assert result.returncode == 100 + code, (mode, name, result.returncode)
        print("PASS:", " ".join(mode) or "unchecked", "behavior and runtime dependencies")

    for flag in ("-rubbish", "-unknown"):
        result = subprocess.run([str(ptc), flag], input="", capture_output=True, text=True)
        assert result.returncode != 0 and "Usage:" in result.stderr
    result = run([str(ptc), "-i16", "-m"], input="program width; var w: 0..65535; "
                 "begin w := maxint end.\n", capture_output=True, text=True)
    assert "32767" in result.stdout and "unsigned int" in result.stdout
    print("PASS: option validation and 16-bit target constants")

    for mode in (["-m"], ["-m", "-r"], ["-m", "-e"]):
        result = run([str(ptc), *mode], input="program missingcase; var n: integer; "
                     "begin n := 2; case n of 1: n := 3 end end.\n",
                     capture_output=True, text=True)
        generated.write_text(result.stdout)
        extra = [str(root / "runtime/ptc_checks.c")] if "-e" in mode else []
        run([*cc, "-std=c89", "-DPTC_CUSTOM_FAIL", "-I" + str(root / "runtime"),
             str(generated), str(custom), *extra, "-o", str(binary)])
        result = subprocess.run([str(binary)], capture_output=True)
        assert result.returncode == 103, (mode, result.returncode)
    obj = build / "runtime.o"
    run([*cc, "-std=c89", "-DPTC_MINIMAL", "-ffreestanding", "-fno-builtin",
         "-c", str(root / "runtime/ptc_checks.c"), "-o", str(obj)])
    symbols = run(["nm", "-u", str(obj)], capture_output=True, text=True).stdout
    assert not symbols.strip(), symbols
    print("PASS: missing-case hook and library-free linked runtime")
