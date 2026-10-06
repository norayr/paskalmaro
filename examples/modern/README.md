# Contemporary command-line examples

These programs put portable processing logic in Pascal and use a small C bridge
for binary input. `readbyte` returns a byte in `0..255` or `-1` for EOF, and
reports stream errors. This avoids Pascal text-file newline conversion and
works with ordinary shell redirection, pipes, and arbitrary binary bytes.

## Text statistics

```sh
make -C examples/modern/textstats
printf 'hello world\n' | examples/modern/textstats/textstats
# lines=1 words=2 bytes=12
examples/modern/textstats/textstats < readme.md
```

Words are separated by ASCII whitespace; this is byte-oriented, not a Unicode
word parser. An unterminated final line counts as a line, while an empty stream
has zero lines. Consequently the line count differs from `wc -l` for a file
without a final newline. It is useful for log/pipeline summaries and for trying
out streaming algorithms in Pascal.

## Fletcher-16 checksum

```sh
make -C examples/modern/fletcher16
printf 'abcde' | examples/modern/fletcher16/fletcher16
# 51440 (hexadecimal 0xc8f0)
examples/modern/fletcher16/fletcher16 < examples/oric/lores/demo-osdk.tap
```

The checksum is printed in decimal. Empty input produces 0; NUL and bytes
128..255 are included. This is useful in asset-generation pipelines and for
accidental-corruption checks, not cryptographic verification.

Both Makefiles enable `-r` by default and use a modern host's native `integer`.
The counters and final checksum expression are intended for a 32-bit C `int`;
these examples are not built with the retro `-i16` profile. Counter overflow
on exceptionally large streams is not detected.

## Other useful directions

- Put a small game/simulation engine in Pascal and use C only for SDL/window
  events and rendering.
- Translate file-format parsers and asset converters, using C wrappers for
  byte streams and filesystem APIs.
- Build numerical or validation routines as headerless Pascal fragments and
  link them into an existing C command-line program.

The current FFI is easiest with integers, doubles, and `var` parameters.
Wrappers are preferable for C strings, structs, macros, or API-specific types.
`make check` verifies empty/final-line text cases, binary checksum fixtures, and
the independent Pascal drawing library.
