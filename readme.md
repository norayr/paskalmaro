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

This produces `ptc-new`.

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

## Testing

To translate and compile the included test program:

```sh
make test
```

The test target uses the self-hosted compiler to translate `test.pas`, compiles the resulting `test.c`, and creates the `test` executable.

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

