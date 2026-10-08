# Oric character-cell colour demo

`demo.p` implements keyboard movement. `lores.p` is a headerless, separately
translated Pascal library. `oric.c` supplies just a memory write and a blocking
keyboard read; it is compiled with the same C toolchain as the Pascal output.

Controls: **W/A/S/D** move, **C** cycles the seven visible colours, **Q** quits.
Uppercase works too. A keypress moves one cell; this version uses blocking
keyboard input rather than a frame-timed game loop.

Oric colours are serial attributes. For independently coloured cells, this
demo uses three physical columns: a PAPER attribute, a space (the coloured
cell), and a black PAPER reset. The resulting grid is **12 × 27**. This is a
text-mode colour-cell library, not ROM `LORES 1` semigraphics or HIRES. The last
physical row is reserved for ROM use. It selects 50 Hz text mode; quitting
leaves the screen visible.

## cc65

From the repository root:

```sh
make bootstrap
make -C examples/oric/lores
```

Load `examples/oric/lores/demo.tap` in an Oric Atmos or emulator. No hand-written
fastcall assembly wrapper is needed. The C compiler handles calls to `oricpoke`
and `orickey`, and the C bridge includes the proper declaration for `cgetc`.

## OSDK on Linux

The installed `lcc65` and `rcc16` are aliases of `osdk-compiler` on the inspected
Gentoo installation, not two different backends. Its ordinary C calls use
stack arguments. The no-argument `get` keyboard function needs no argument-ABI
adapter. Some OSDK library versions have fastcall routines too; a C bridge with
the appropriate header can handle them without exposing that detail to Pascal.

To generate the compiler's intermediate assembly using just the installed
tools:

```sh
make -C examples/oric/lores frontend
```

The three `.c2` files still contain OSDK instruction macros such as `ENTER` and
`CALLV_C`. They cannot be passed directly to `xa`. A runnable program also needs
OSDK's macro definitions, linker library index, startup, and helper routines.
The executable-only package listed by `qlist osdk` does not install these.

Obtain the SDK data from a complete OSDK distribution, or check out the upstream
data directories:

```sh
mkdir -p .build
git clone --depth 1 --filter=blob:none --sparse \
    https://github.com/Oric-Software-Development-Kit/osdk.git .build/osdk-source
git -C .build/osdk-source sparse-checkout set \
    osdk/main/Osdk/_final_/lib osdk/main/Osdk/_final_/macro \
    osdk/main/Osdk/_final_/include
make -C examples/oric/lores osdk \
    OSDK="$PWD/.build/osdk-source/osdk/main/Osdk/_final_"
```

This produces `demo-osdk.tap`, loaded at `$0600`. `OSDKADDR` can override that
address. The SDK may spell its macro file `MACROS.H` or `macros.h`; the Makefile
accepts both (or an explicit `OSDKMACROS` override). The tested SDK revision was
`ad23abcee52d9fc0bcdfb3f90d828290589cc8eb`, with the locally installed compiler
reporting `16-bit code V2.0`.

The pipeline is:

```text
Pascal → ptc -m -i16 → C → host cpp → lcc65
       → macrosplitter → link65 + OSDK libraries → xa → header → TAP
```

Host preprocessing uses `-nostdinc`: Linux's C headers describe the wrong target
and must not be passed to the 6502 compiler. The bridge needs no OSDK headers
because it uses only a no-argument function and memory access.

### Loading without autorun and starting with BASIC CALL

The Makefile creates `demo-osdk.tap` with automatic execution enabled (`-a1`).
To load the machine code and return to the BASIC prompt instead, use the OSDK
`header` tool's `-a0` option. After building the OSDK demo, run this from the
repository root:

```sh
header -nLORES -a0 -b1 \
    examples/oric/lores/demo-osdk.bin \
    examples/oric/lores/demo-osdk-manual.tap \
    0x600
```

Load `demo-osdk-manual.tap` on the Oric with:

```basic
CLOAD "LORES"
```

Wait for loading to finish and BASIC to return to its prompt, then start the
program manually:

```basic
CALL 1536
```

The entry/load address is `$0600`, or **1536 decimal**. The tape header stores
the start and end addresses, which `CLOAD` uses to place the program in memory.
`-b1` identifies the file as machine code; `-a0` disables autorun. The manual
variant has the same program payload and load address as the autorun variant.

If you build with a different `OSDKADDR`, supply that same address to `header`
and use its decimal equivalent in `CALL`. Changing only the tape header does
not relocate the assembled program.

In the standard OSDK batch build, the equivalent configuration is:

```bat
SET OSDKADDR=$600
SET OSDKHEAD=-a0
```

This example's Linux Makefile explicitly passes `-a1`; it does not read
`OSDKHEAD`. Use the standalone `header` command above for the manual-start tape.

## Representation and separate compilation

Use the same translator options for both Pascal files. `-i16` selects native
16-bit integers and maps `0..65535` to `unsigned int`, matching `oricpoke`.
OSDK's `short` is 8 bits, so a host-style `unsigned short` would truncate these
addresses. `-m` avoids stdio, heap allocation, `exit`, and libc string copying.
There is still a C startup and compiler helper code; those are part of the C
toolchain, not a large Pascal runtime.

Declarations must be repeated in callers because PTC has no import/interface
system. Global C symbols share one linker namespace: avoid names used by the
SDK, such as `key`. All files must be built with the same target compiler;
cc65 and OSDK object/assembly formats and ABIs cannot be mixed.

Both tape builds have been checked. `make check` also runs movement and colour
logic against a simulated screen and keyboard on the host. Actual display,
ROM keyboard behavior, and cursor interactions still need an Atmos/emulator
run; building a tape alone does not validate those hardware details.
