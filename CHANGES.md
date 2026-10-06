# Maintained-version changes

`original/` contains the unaltered historical distribution. `modified/ptc.p`
is the maintained Pascal compiler; `generated/ptc.c` is regenerated from it.
The Git history records earlier maintenance, including semantic checking,
separate translation, C89 compatibility, and the initial `-r` support.

## Optional support and practical targets

- `-c` selects console I/O and implies `-m`. Pascal I/O lowers to typed
  character/native-integer helpers rather than printf/scanf. Input/output
  helpers need no system headers or C library and use a replaceable host/Oric
  adapter. One lazy input state is shared across translations; CR/LF, sticky
  EOF, unterminated lines, bounded words, and integer/subrange overflow are
  handled. Unsupported file/type/format operations are diagnosed during
  translation. Added a three-target example and tests to `make check`.

- `-r` uses single-evaluation functions for nil and array-bound checks, including
  conformant arrays. Generated pointer casts preserve their original types.
  Signal handling is no longer part of the generated checks.
- `-e` links checks through `runtime/ptc_checks.h` and `runtime/ptc_checks.c`.
- `-m` removes implicit hosted support: main returns, string-literal copying
  uses a generated loop, and check/case failure handling can be library-free.
  Used file, heap, math, and set features retain their dependencies.
- `PTC_CUSTOM_FAIL` permits a target-defined, non-returning failure hook.
  `PTC_MINIMAL` selects a library-free default for separately compiled checks.
- `-i16` selects 16-bit native integers and maxint, maps 16-bit subranges to
  native int rather than short, and casts scalar call arguments to their
  parameter type. This accommodates OSDK's 8-bit short and wide C literals.
- Options are parsed exactly and can be combined in any order.
- Conformant-array C parameters use element pointers plus counts instead of
  invalid C89 flexible-array wrappers. Counts are passed before subsequent
  parameter groups, and conformant parameters can be forwarded.
- Boolean output-table declarations have a complete array size for OSDK.
- Bootstrap updates allow one output-format transition before verifying a
  fixed point; `make verify` still compares regenerated C and executables.
- `make check` combines self-hosting, semantic, runtime, and example checks.
- Added a separately translated Oric colour-cell library and keyboard demo,
  with cc65 and OSDK tape builds, plus hosted text and binary filter examples.
- Fixed the separate-compilation example's translator path and inaccessible
  bound-name use. Updated retro examples to use the 16-bit minimal profile,
  unsigned addresses, target API declarations, and accurate build comments.

## Known practical follow-up

The existing Pascal text-input buffering still needs an audit: repeated `eof`
tests before a first `read` can consume a character, and `readln` can loop on an
unterminated final line. The hosted filter examples use an explicit byte-reader
C bridge, including EOF and stream-error handling. Pascal source supplied to
the translator should currently end with a newline. These are existing input
limitations, not part of the new optional-check runtime.

The `-c` application backend has independent buffering without those EOF/line
defects. It does not repair the historical file runtime or support direct
file-buffer designators used by the translator's scanner; the compiler's
self-hosting build continues to use its normal profile.
