# Semantic error tests

From the repository root, `make check` runs self-hosting verification, these
semantic rejection tests, `runtime/` tests, and hosted example checks.

`console/run.py` checks field widths, native signed/unsigned limits, bounded
words/subranges, lazy CR/LF lookahead, sticky EOF, errors, separate translations
sharing input state, and helper objects with no libc dependencies.

`runtime/run.py` checks single evaluation, conformant-array forwarding, custom
failure codes, inline/linked check implementations, minimal object dependencies,
and target options. `examples.py` checks text/binary filters and runs the separate
Pascal LORES library with a simulated Oric keyboard and screen.

Every `.p` file in this directory is intentionally invalid. The compiler must
reject each program and stop at the first diagnostic.

Run from this directory with:

```sh
PTC=../ptc ./run.sh
```
