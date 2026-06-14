# Semantic error tests

Every `.p` file in this directory is intentionally invalid. The compiler must
reject each program and stop at the first diagnostic.

Run from this directory with:

```sh
PTC=../ptc ./run.sh
```
