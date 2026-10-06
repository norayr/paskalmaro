# paskalmaro: ptc — historical Pascal-to-C translator

This repository preserves and maintains **ptc**, the Pascal-to-C translator written by Per Bergsten in 1987.

## Repository contents

The repository contains:

* `original/` — the original ptc source, reproduced unaltered;
* `modified/` — the original source with those patches applied;
* `generated/` — C source generated from the modified translator, where applicable.

The original source is kept separately so that it remains possible to inspect, compile and redistribute the historical program exactly as it was released. For details see below in this file.

## Building

PTC is self-hosting. The repository includes a C translation of the maintained compiler in `generated/ptc.c`.

To build a compiler directly from the generated C source:

```sh
make bootstrap
```

This produces `ptc`.

To perform the full self-hosting build and comparison:

```sh
make
```

The build performs these steps:

```text
generated/ptc.c  --C compiler-->  ptc
ptc.p            --ptc--------->  ptc-new.c
ptc-new.c        --C compiler-->  ptc-new
```

It then prints checksums for the generated C files and compiler executables.

The maintained compiler source of truth is `modified/ptc.p`. After editing it,
run `make update-bootstrap`: this bootstraps through three translations and
compares the last two before updating `generated/ptc.c`. `make verify` checks
that the checked-in C regenerates exactly and builds identical executables.

The generated compiler source uses C89-style C and is built with:

```sh
cc -std=c89 -o ptc generated/ptc.c
```

## Using PTC

PTC reads Pascal source from standard input and writes C source to standard output:

```sh
./ptc < program.pas > program.c
```

Compile the generated C with a C compiler:

```sh
cc -std=c89 -o program program.c
```

Then run the resulting program:

```sh
./program
```

Compiler diagnostics are written to standard error, so they remain separate from the generated C output.

Programs that use the predefined standard files must declare them in the program heading. For example:

```pascal
program Example(output);
```

or, when both standard input and standard output are used:

```pascal
program Example(input, output);
```

The generated C includes the runtime definitions it needs. The old `ptc_runtime.h` compatibility header is no longer required.

### Optional checks and small targets

Options can be combined in any order:

| Option | Purpose |
|---|---|
| `-r` | Emit nil-pointer and array-index checks into the generated C. |
| `-e` | Enable those checks, but link their implementation from `runtime/ptc_checks.c`. |
| `-m` | Minimize implicit C-library requirements; return from `main`, copy string literals locally, and use a library-free failure handler. |
| `-c` | Use the optional character-console backend for Pascal I/O; implies `-m`. |
| `-i16` | Target a 16-bit C `int`; set `maxint` to 32767 and use native `int` for 16-bit subranges. |

For a hosted program with generated checks:

```sh
./ptc -r < program.p > program.c
cc -std=c89 -o program program.c
```

To link one shared check implementation for several translated files:

```sh
./ptc -e < program.p > program.c
cc -std=c89 -Iruntime -o program program.c runtime/ptc_checks.c
```

For a small-target program using hardware helpers instead of Pascal file I/O:

```sh
./ptc -m -i16 < program.p > program.c
```

`-m` does not silently replace language features: `writeln` still requires
stdio, `new` still requires allocation, string comparison still uses `strncmp`,
and Pascal files and sets still use their existing support. Scalar logic,
arrays, records, pointer access, ordinary routines, and hardware calls can use
no C library at all. The target C compiler still supplies its startup, stack,
and any arithmetic helper routines.

For console I/O without the full stdio/file runtime, select `-c`:

```sh
./ptc -c < program.p > program.c
cc -std=c89 -Iruntime -o program program.c \
    runtime/ptc_console_input.c runtime/ptc_console_output.c \
    runtime/ptc_console_host.c
```

Characters, native integers, booleans, strings/character arrays, field widths,
and buffered `read`/`readln` work through small helpers. The target adapter alone
includes system headers. Omit unused input/output helpers, and use `-c -i16`
for OSDK or cc65. See [the runtime interface](runtime/README.md) and
[the three-target console demo](examples/console/README.md).

Checks do not require signals, allocation, or a global initialization call.
In minimal mode, a failed check stops in an infinite loop. Define
`PTC_CUSTOM_FAIL` when compiling C and supply a non-returning `void PtcFail(int)`
to display a message, return to a monitor, or halt differently. For linked
checks, also compile the runtime with `PTC_MINIMAL` or `PTC_CUSTOM_FAIL`;
translator options cannot configure a separately compiled C file. See
[LANGUAGE.md](LANGUAGE.md#16-runtime-safety-checks) for the exact contract.

### Useful examples

- [Console demo](examples/console/README.md): ordinary Pascal `readln`/`writeln`
  built for a host, cc65 Atmos, or OSDK with a replaceable adapter.

- [Oric LORES demo](examples/oric/lores/README.md): a separately translated
  Pascal drawing library, a moving coloured cell, and a small C hardware bridge.
  Builds using either cc65 or OSDK.
- [Modern examples](examples/modern/README.md): a text-statistics filter and a
  binary Fletcher-16 checksum tool, usable in ordinary shell pipelines.
- `examples/separate_compilation/`: link several Pascal translations with C's
  linker; a real Pascal module system is not required for this pattern.

The OSDK compiler tools alone are insufficient to build a runnable tape: its
macro definitions, startup code, and library index are also needed. The LORES
instructions show both a compiler-only stage and a complete tape build.

## Testing

Run the self-hosting check, semantic rejection tests, runtime-mode tests, and
hosted example checks together:

```sh
make check
```

To translate and compile the included test program:

```sh
make test
```

The test target uses the bootstrapped compiler to translate `test.pas`, compiles the resulting `test.c`, and runs the `test` executable.

Run it with:

```sh
./test
```

## Copyright and original distribution terms

The original program is:

```
Copyright (C) 1987 by Per Bergsten, Gothenburg, Sweden
```

The complete original copyright and distribution notice is preserved in the source and in `COPYING.PTC`.

The original notice permits free redistribution of the unaltered program, provided that:

1. the original program and notice are reproduced unaltered;
2. no charge other than nominal media cost is demanded;
3. a package containing the program is distributed only at media cost.

It also prohibits selling, hiring or otherwise commercially exploiting the program or material derived from it without the author’s written consent.

## Modified and derived versions

The modified Pascal source and generated C source are provided for historical preservation, study and continued non-commercial use.

I understand the intention of the original notice to be that ptc should remain freely available, that the original source and attribution must always be preserved, and that neither the original program nor derived versions may be commercially distributed.

On that good-faith interpretation, my modifications are distributed under the same non-commercial conditions as the original program:

* the original copyright and notice must be retained;
* the origin of the program must not be misrepresented;
* modified files must be clearly identified as modified;
* the original unmodified source must remain available;
* neither the original nor modified versions may be sold, hired, included in a commercially distributed package, or otherwise commercially exploited without permission from the original copyright holder.

This statement does not replace, alter or broaden Per Bergsten’s original notice. It records the conditions under which I am making my own changes available.

Copyright in the original program remains with Per Bergsten. To the extent that my changes contain independently copyrightable material, I make those changes available under the same conditions described above.

## No commercial use

This repository and its contents are provided without charge.

No permission is claimed or granted for commercial distribution, paid licensing, sale, rental or inclusion in a commercially distributed software collection.

## Provenance and changes

The unmodified historical version can be found in `original/`.

All intentional differences from that version are documented in `CHANGES.md` and, where possible, are also available as patches in `patches/`.

The maintained source must not be represented as the unmodified 1987 version.

## Copyright-holder contact

I have attempted to locate the original author or current copyright holder but have not been able to establish contact.

If Per Bergsten or another person who can demonstrate ownership of the relevant rights contacts the repository maintainer and objects to the distribution of the modified or derived files, the request will be considered promptly and the repository adjusted where appropriate.

Electronic mail contact: `username: norayr; domain: arnet.am`

## No warranty

The maintained files are provided for historical and educational purposes, without any promise that they are correct, safe or suitable for a particular purpose.
