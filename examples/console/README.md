# One Pascal console program, three targets

`demo.p` reads a bounded name and a signed integer, then prints them using
Pascal console I/O. Explicit `flush` makes prompts visible before reads.

## Host

From the repository root:

```sh
make bootstrap
make -C examples/console
printf 'Ada\n-42\n' | examples/console/console-demo
```

Or run `examples/console/console-demo` interactively. The name must be one word
of at most 12 bytes. Character arrays are space-padded; brackets in the greeting
show their fixed length.

## cc65 / Oric Atmos

```sh
make -C examples/console cc65
```

This produces `console.tap`. It uses the same Pascal source, native-integer
console helpers, and the Oric adapter compiled by cc65. No hand-written assembly
or Pascal awareness of fastcall is required.

## OSDK

With the SDK data checkout described in `examples/oric/lores/README.md`:

```sh
make -C examples/console osdk \
    OSDK="$PWD/.build/osdk-source/osdk/main/Osdk/_final_"
```

This produces `console-osdk.tap` at `$0600`. `OSDKADDR` and `OSDKMACROS` can
override the load address and macro-file path.

Only `console_adapter.pre.c` uses the OSDK system include directory. Translated
Pascal, input helpers, and formatting use just the project interface. No FILE
structures, printf, scanf, or filesystem functions are involved. Host system
headers are excluded with `-nostdinc`.

Retro targets use `ptc -c -i16`; the host uses `ptc -c`. `-c` implies minimal
support. Formatting handles native signed/unsigned integers, including the most
negative signed value, without wide arithmetic.

The Oric adapter echoes input and waits for keys. It has no line editor or EOF
key. Errors display a code and stop; the host adapter instead reports to stderr
and exits. See `runtime/README.md` for the interface and supported semantics.
