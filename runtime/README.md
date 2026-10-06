# Optional Pascal runtime components

## Console I/O (`ptc -c`)

`-c` selects console I/O and implies `-m`. Ordinary Pascal `read`, `readln`,
`write`, and `writeln` call small, typed C helpers instead of stdio formatters.
There are no FILE objects, file bitfields, formatted-input buffers, or runtime
initialization calls. Target-specific system headers belong to the adapter.

```pascal
program Greeting(input, output);
var Number: integer;
begin
    write('Number: ');
    flush(output);
    readln(Number);
    writeln('You entered ', Number:6)
end.
```

Build on a contemporary host:

```sh
./ptc -c < greeting.p > greeting.c
cc -std=c89 -Iruntime -o greeting greeting.c \
    runtime/ptc_console_input.c runtime/ptc_console_output.c \
    runtime/ptc_console_host.c
```

Input and output helpers are separate files. Omit the input helper for an
output-only program, or the output helper for a program with no formatted
output. Link exactly one adapter, and one copy of each needed helper, even when
linking several translated Pascal files. The adapter provides all four device
functions; an output-only adapter can supply an EOF-returning `PtcGetChar` stub.

### Device interface

`ptc_console.h` includes no system headers. The adapter implements:

```c
void PtcPutChar(int character);
int PtcGetChar(void);
void PtcFlush(void);
void PtcConsoleFail(int code); /* MUST NOT return */
```

- `PtcPutChar` receives a byte in `0..255`; LF is the logical newline. Translate
  it to the device's convention here. Output errors call the failure hook from
  the adapter.
- `PtcGetChar` returns a byte in `0..255`, `PTC_CONSOLE_EOF` (-1), or
  `PTC_CONSOLE_IO_ERROR` (-2). It must not skip whitespace or convert EOF to an
  ordinary character.
- `PtcFlush` makes pending output visible. An unbuffered display can implement
  it as an empty function. Pascal `flush` calls it, and a generated main calls
  it before returning. Use an explicit `flush` after an interactive prompt;
  there is no implicit flush before every read.
- `PtcConsoleFail` reports/stops on malformed input or device errors. The host
  adapter prints to stderr and exits with the failure code. The Oric adapter
  displays an error code and stops in a loop.

Console error codes are independent of the optional nil/index-check `PtcFail`:

| Code | Meaning |
|---|---|
| 1 | Input requested at EOF |
| 2 | Invalid integer syntax |
| 3 | Integer/subrange overflow or invalid field width |
| 4 | Device I/O error or invalid input-adapter return value |
| 5 | Word does not fit the destination character array |

Compile translations, helpers, and the adapter using the same target compiler
and ABI. Numeric helpers use native C int/unsigned int, with two's-complement
signed integers. For a 16-bit C compiler, translate with `-c -i16`.

### Implemented operations

| Pascal operation | Console behavior |
|---|---|
| `write` / `writeln` | Characters, literals, character arrays, booleans, native signed/unsigned integers |
| `value:width` | Nonnegative minimum width, right-aligned with spaces; no truncation |
| Integer output without a width | Existing PTC default of 10 columns |
| `read` / `readln` | Characters, native integers/subranges, whitespace-delimited character-array words |
| `eof` / `eoln` | Lazy lookahead, without consuming input |
| `flush` | Adapter flush |
| `page` | Form-feed byte |
| `message` | Diagnostic line on the same console |

Only predefined input/output streams are supported, explicitly or implicitly.
Console mode has one output device: `message` uses it even on the host.
Unsupported operations are diagnosed before C is emitted: named/typed files,
user-defined text objects, `reset`/`rewrite`/`close`, `get`/`put`, file-buffer
dereference, real I/O, boolean input, and input field formats are not part of
this first backend. With `-i16`, I/O of subranges wider than native signed or
unsigned int is also rejected.

Other requested features keep their dependencies: `new` still needs allocation,
and string comparisons still use `strncmp`. The input and formatter helpers
themselves need no C library. Freestanding host builds can use
`-ffreestanding -fno-builtin` to avoid optimizer-generated libc calls. Target
arithmetic/startup helpers remain the C toolchain's responsibility.

### Input semantics

- CRLF and bare CR normalize to LF. A CR does not immediately fetch the next
  character: `readln` can return after Enter without waiting for another key.
- `eof` and `eoln` share one lazy lookahead across translations. EOF is sticky;
  repeated tests and `readln` at EOF do not read the adapter again.
- `eoln` is true at a line boundary or EOF. `readln` discards the remainder of
  the line and its boundary; an unterminated final line ends normally.
- Character reads do not skip whitespace. A consumed line boundary becomes a
  space, matching Pascal text's logical character representation.
- Integer reads skip ASCII whitespace, accept an optional sign, and consume
  decimal digits. Signed input accepts the native minimum/maximum; unsigned
  subranges accept an optional `+`. A following non-digit remains buffered.
  Overflow and declared integer subrange violations fail before assignment.
- Array input reads one ASCII-whitespace-delimited word, space-pads unused
  cells, and fails if the word is too long. It does not NUL-terminate the array.
- Array output writes every declared cell, including embedded NULs. String
  literals use NUL-terminated C literals.

### Supplied adapters

- `ptc_console_host.c`: C character I/O and stream-error reporting. It includes
  stdio/stdlib headers but does not use printf or scanf.
- `ptc_console_oric.c`: define `PTC_OSDK` for OSDK, otherwise build for cc65
  Atmos. OSDK's stdio header declares its output ABI; cc65 uses conio. Keyboard
  input blocks, echoes keys, and has no natural EOF. This is not a line editor:
  backspace editing and keyboard hotkeys are not implemented by the helpers.

See `examples/console/` for one source built for host, cc65, and OSDK. `make
check` includes behavior, bounded-input failures, shared state, and freestanding
dependency tests. Tape builds check compilation/linking; actual ROM/display
behavior requires an Oric or emulator run.

## Optional nil and array checks

`ptc -r` embeds checks; `ptc -e` includes `ptc_checks.h` and links `ptc_checks.c`.
Both work with `-c`. Compile linked checks with `PTC_MINIMAL` for library-free
defaults, or `PTC_CUSTOM_FAIL` and your non-returning `PtcFail(int)` hook. See
`LANGUAGE.md` for their policy and limitations.
