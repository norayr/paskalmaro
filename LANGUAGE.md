# PTC Pascal Dialect and Implementation Notes

This document describes the Pascal dialect currently accepted by the maintained
version of **ptc**, its important differences from ISO Standard Pascal and Turbo
Pascal, its C representation, and the current foreign-function interface.

## 1. Basic use

PTC reads Pascal source from standard input and writes generated C to standard
output:

```sh
./ptc < program.p > program.c
cc -std=c89 -o program program.c
./program
```

Compiler diagnostics are written to standard error.

A normal executable source begins with a `program` heading:

```pascal
program Hello(output);

begin
    writeln('Hello from Pascal!')
end.
```

Programs using standard input or output should name them in the program
parameter list:

```pascal
program Filter(input, output);
```

The maintained compiler can also parse a headerless declaration/body fragment.
This historical parser path is internally called a module, but it is **not a
real module system**: it has no module name, imports, exports, separate symbol
file, or namespace.

## 2. General character of the dialect

The implemented language is best described as:

> A substantial ISO-7185-style Pascal dialect, including conformant-array
> parameters and several implementation extensions, translated to C89.

It supports the traditional Pascal programming model:

- programs;
- nested procedures and functions;
- lexical scopes;
- value and `var` parameters;
- procedure and function parameters;
- labels and `goto`;
- constants, types and variables;
- enumerated and subrange types;
- arrays and conformant-array parameters;
- records and variant records;
- sets;
- pointers;
- files and text files;
- `if`, `case`, `while`, `repeat`, `for`, and `with`;
- `forward` and the non-standard `external` directive.

The maintained version performs semantic checking and stops at the first
detected error.

## 3. Lexical syntax

### 3.1 Case and identifiers

Pascal identifiers are case-insensitive:

```pascal
Count
count
COUNT
```

all denote the same identifier.

Identifiers begin with a letter and may continue with letters, digits, and
underscores. The implementation stores identifiers in a fixed token buffer, so
identifier length is limited rather than unlimited.

Unlike ISO 7185 Pascal, underscore is accepted:

```pascal
line_count
```

### 3.2 Comments

The supported comment forms are:

```pascal
{ comment }
```

and:

```pascal
(* comment *)
```

C++, Delphi, and modern Free Pascal line comments are not accepted:

```pascal
// not supported
```

### 3.3 Numbers

Decimal integer and real literals are supported:

```pascal
123
-42
3.14159
1.5e6
2E-3
```

Turbo Pascal forms such as hexadecimal `$ff` are not supported.

### 3.4 Character and string literals

Character and string literals use apostrophes:

```pascal
'A'
'hello'
'Don''t'
```

A one-character literal has type `char`. A longer literal is represented by the
translator's fixed-string machinery.

The implementation uses an ASCII-oriented character model. The maintained
compiler currently assumes a Pascal character range of `0..127`.

## 4. Program and declaration structure

A complete program has this general shape:

```pascal
program Name(input, output);

label
    100;

const
    Limit = 10;

type
    Index = 1..Limit;

var
    I: Index;

procedure PrintOne(N: integer);
begin
    writeln(N)
end;

begin
    for I := 1 to Limit do
        PrintOne(I)
end.
```

The declaration sections occur in the traditional order:

1. `label`
2. `const`
3. `type`
4. `var`
5. procedures and functions
6. the compound statement beginning with `begin`

The compiler does not implement the Extended Pascal rule that allows
declaration parts to occur repeatedly in arbitrary order.

## 5. Types

### 5.1 Predefined types

The primary predefined types are:

```pascal
boolean
char
integer
real
text
```

The values `false`, `true`, `nil`, and `maxint` are predefined.

### 5.2 Enumerated types

```pascal
type
    Colour = (red, green, blue);
```

### 5.3 Subranges

```pascal
type
    ByteValue = 0..255;
    SmallSigned = -128..127;
    Month = 1..12;
```

Subranges are important because PTC selects a C integer representation from
their declared bounds.

Programmer can create custom Turbo Pascal style new types:

```pascal
    byte     = 0..255;
    shortint = -128..127;
    word     = 0..65535;
    smallint = -32768..32767;
    longint  = integer;
```

### 5.4 Arrays

```pascal
type
    Vector = array [1..10] of real;
    Matrix = array [1..10, 1..10] of real;
```

Multidimensional arrays are internally translated as nested arrays.

### 5.5 Records

```pascal
type
    Point = record
        X, Y: integer
    end;
```

Variant records are also supported:

```pascal
type
    Value = record
        case Kind: boolean of
            false: (I: integer);
            true:  (R: real)
    end;
```

### 5.6 Pointers

```pascal
type
    NodePtr = ^Node;
    Node = record
        Value: integer;
        Next: NodePtr
    end;
```

Pointer dereference uses `^`:

```pascal
P^.Value
```

The alternative pointer token `@` is recognized in type and dereference
positions, but Turbo Pascal's address-of operator semantics should not be
assumed.

### 5.7 Sets

```pascal
type
    Digit = 0..9;
    Digits = set of Digit;
```

Set constructors and operations are supported:

```pascal
S := [1, 2, 4..7];

if 4 in S then
    S := S - [4];
```

The implementation has restrictions on set bases and set ranges because sets
are represented by a small C runtime.

### 5.8 Files

Pascal files are sequential streams whose elements all have one component
type. For example:

```pascal
type
    NumberFile = file of integer;
```

A value of type `NumberFile` contains `integer` elements, not formatted text.
It is read and written with `read`, `write`, `get`, and `put` according to the
file operation being used.

The predefined type `text` is a **text-file type**, not a string type. A
variable of type `text` represents a sequential character stream with line
boundaries. The predefined variables `input` and `output` have type `text` and
normally correspond to standard input and standard output.

PTC can also open named files. Unlike Turbo Pascal, it does not use a separate
`Assign` call. The filename is passed directly to `reset` or `rewrite`:

```pascal
reset(F, 'input.txt');       { open an existing file for reading }
rewrite(F, 'output.txt');    { create or truncate a file for writing }
```

Close a named file with:

```pascal
close(F)
```

#### Writing a text file

```pascal
program WriteFile;

var
    F: text;

begin
    rewrite(F, 'output.txt');
    writeln(F, 'first line');
    writeln(F, 'second line');
    close(F)
end.
```

This creates or truncates `output.txt` and writes two lines.

#### Reading a text file

```pascal
program ReadFile(output);

var
    F: text;
    Ch: char;

begin
    reset(F, 'input.txt');

    while not eof(F) do
    begin
        while not eoln(F) do
        begin
            read(F, Ch);
            write(Ch)
        end;

        readln(F);
        writeln
    end;

    close(F)
end.
```

This opens `input.txt`, reads it one character at a time, and copies it to
standard output.

An end of line is a state of the text stream:

- `eoln(F)` is true when `F` is positioned at the end of the current line;
- `readln(F)` consumes the remainder of the current line and advances to the
  next line;
- `writeln(F, ...)` writes its arguments and then terminates the output line;
- `eof(F)` is true when no more file contents remain.

In the generated C runtime, a text file keeps its buffered character and its
end-of-line state separately. A physical newline from the underlying C stream
sets the `eoln` state instead of being returned as an ordinary character by
`read(F, Ch)`.

The corresponding Turbo Pascal sequence:

```pascal
Assign(F, 'input.txt');
Reset(F);
```

is therefore written in this dialect as:

```pascal
reset(F, 'input.txt')
```

Similarly:

```pascal
Assign(F, 'output.txt');
Rewrite(F);
```

becomes:

```pascal
rewrite(F, 'output.txt')
```

### 5.9 `packed`

The keyword `packed` is accepted before arrays, records, sets, and files, but is
currently ignored. It does not request a distinct packed C representation.

### 5.10 Conformant-array parameters

One-dimensional conformant-array parameters are supported, but only as `var`
parameters — value-parameter conformant arrays are not implemented:

```pascal
procedure Fill(var A: array [Lo..Hi: integer] of integer; V: integer);
var I: integer;
begin
    for I := 0 to Hi - 1 do
        A[I] := V
end;
```

**Important implementation differences from ISO 7185:**

- The bounds identifiers `Lo` and `Hi` are **not** accessible as ordinary Pascal
  variables inside the procedure body. They exist only as implementation details
  used to generate correct C parameter declarations and array size information.
- Internally, ptc maps every conformant array to a C array indexed from 0.
  The lower bound is always set to 0 inside the called procedure. The upper
  bound parameter receives the element count (= `actual_hi − actual_lo + 1`),
  not the actual upper bound of the passed array.
- Therefore, valid C indices inside the procedure are `0` to `Hi − 1` (where
  `Hi` is the element count), not `Lo` to `Hi` in the ISO sense.
- Nested and multidimensional conformant arrays are currently a restriction.
- The `pack` and `unpack` built-ins use conformant arrays internally.

## 6. Constants and variables

Constant definitions use traditional Pascal syntax:

```pascal
const
    Maximum = 100;
    Newline = 'N';
```

The current constant grammar is substantially more restrictive than Extended
Pascal constant expressions. Do not assume that arbitrary expressions,
structured constants, or Turbo Pascal typed constants are accepted.

Variables are declared conventionally:

```pascal
var
    Count: integer;
    Buffer: array [0..255] of char;
```

There are no initializers in variable declarations.

## 7. Procedures and functions

### 7.1 Ordinary parameters

```pascal
procedure Swap(var A, B: integer);
var
    T: integer;
begin
    T := A;
    A := B;
    B := T
end;
```

Parameters without `var` are value parameters.

### 7.2 Function results

A function returns a value by assigning to its own identifier:

```pascal
function Square(X: integer): integer;
begin
    Square := X * X
end;
```

The semantic checker verifies that a function result is assigned somewhere in
the function. It does not yet prove that every possible control-flow path
assigns the result.

### 7.3 Nested routines

Procedures and functions may be nested. The translator converts references to
captured variables into generated C support structures.

### 7.4 Procedural and functional parameters

Traditional Pascal routine parameters are supported:

```pascal
procedure Apply(
    function F(X: integer): integer;
    X: integer
);
begin
    writeln(F(X))
end;
```

### 7.5 Forward declarations

```pascal
procedure Walk(P: NodePtr); forward;
```

A later declaration supplies the body.

### 7.6 External declarations

The extension:

```pascal
function getppid: integer; external;
```

declares a routine implemented outside the Pascal program. See the FFI section
below.

## 8. Statements

The following statement forms are supported:

```pascal
begin ... end
if ... then ... else ...
case ... of ... end
while ... do ...
repeat ... until ...
for ... := ... to ... do ...
for ... := ... downto ... do ...
with ... do ...
goto ...
```

An empty statement is accepted where standard Pascal permits one.

The non-standard `otherwise` keyword supplies a default `case` arm:

```pascal
case N of
    0: writeln('zero');
    1: writeln('one');
    otherwise:
        writeln('other')
end
```

This differs from common Turbo Pascal source, which normally uses `else` in a
`case` statement.

## 9. Expressions and operators

### Arithmetic

```text
+  -  *  /  div  mod
```

`/` produces a real result. `div` and `mod` are integer operations.

### Boolean

```text
not  and  or
```

Short-circuit Extended Pascal operators `and then` and `or else` are not
implemented.

### Comparison

```text
=  <>  <  <=  >  >=
```

### Sets

```text
in  +  -  *
```

For sets, these operators are used for membership, union, difference, and
intersection as appropriate.

### Designators

```text
A[I]     array indexing
R.Field  record field selection
P^       pointer or file-buffer dereference
F(...)   call
```

## 10. Predefined identifiers

The maintained compiler recognizes these principal predefined identifiers.

### Types and values

```text
boolean  char  integer  real  text
false    true  nil      maxint
input    output
```

### Numeric and ordinal functions

```text
abs  arctan  chr  cos  exp  ln  odd  ord
pred  round  sin  sqr  sqrt  succ  tan  trunc
```

### Allocation

```text
new  dispose
```

### File and text operations

```text
eof  eoln  get  put  read  readln
reset  rewrite  write  writeln  page
pack  unpack
```

### Implementation and operating-system extensions

```text
argc  argv  close  exit  flush  halt  message
```

`message(...)` behaves like a diagnostic `writeln`: it writes its arguments to
standard error rather than to Pascal `output`.

## 11. Semantic checking

The maintained version checks, among other rules:

- assignment compatibility;
- operand types;
- procedure and function argument counts;
- argument type and `var`-parameter compatibility;
- function result types;
- array index types;
- record field selection;
- pointer/file dereference;
- `for` control variables and bounds;
- Boolean conditions;
- `case` selector and label compatibility;
- duplicate case labels;
- functions that never assign their result;
- procedure calls used as expressions;
- function calls used as statements;
- identifier use in the wrong context;
- predefined routine argument rules.

The compiler stops at the first detected error.

**Compile-time checks not yet performed** (violations pass through silently):

- uninitialized variables;
- forward declarations whose body is never supplied;
- subrange overflow on assignment (e.g. assigning `10` to a `1..5` variable);
- integer overflow and division by zero.

**Runtime-dependent violations** can optionally be caught with `ptc -r` (see
§ 16). Without that flag, out-of-bounds indexing, nil-pointer dereference, and
wild memory access produce undefined C behavior.

## 12. C type and representation mapping

The exact size of a C type is defined by the target C implementation. The
maintained PTC configuration is intended for ordinary modern systems with
8-bit `char`, 16-bit `short`, and 32-bit `int`.

### Predefined scalar types

| Pascal | Generated C | Typical Linux size |
|---|---|---:|
| `boolean` | `char` | 1 byte |
| `char` | `char` | 1 byte |
| `integer` | `int` | 4 bytes |
| `real` | `double` | 8 bytes |

`maxint` is currently 2147483647.

### Subrange selection

The maintained machine table selects approximately:

| Pascal bounds | Generated C |
|---|---|
| `0..255` | `unsigned char` |
| `-128..127` | `signed char` |
| `0..65535` | `unsigned short` |
| `-32768..32767` | `short` |
| 32-bit signed range | `int` |

Consequently, aliases similar to some Turbo Pascal scalar types can be declared
manually:

```pascal
type
    byte = 0..255;
    shortint = -128..127;
    word = 0..65535;
    smallint = -32768..32767;
    longint = integer;
```

### Structured values

- arrays become C arrays, with generated index adjustment when the Pascal lower
  bound is not zero;
- records become C structures, with unions used for variants;
- pointers become C pointers;
- sets use generated runtime functions and arrays of set words;
- Pascal files use a generated wrapper around `FILE *`;
- longer Pascal character strings use fixed-size generated structures and are
  not C `char *` strings.

## 13. Differences from ISO 7185 Standard Pascal

PTC is close to ISO 7185 in its basic declarations, types, expressions,
statements, nesting, files, sets, and routine model.

No complete ISO 7185 conformance audit has been performed.

Notable extensions include:

- underscores in identifiers;
- `external`;
- `message`;
- `otherwise`;
- operating-system predefined identifiers such as `argc` and `argv`;
- acceptance of a headerless source form;
- C-oriented implementation rules.

Notable restrictions or divergences include:

- `packed` is ignored;
- implementation limits on strings, sets, identifiers, and conformant arrays;
- ASCII-oriented `char`;
- some standard errors or dynamic violations may remain unimplemented;
- C representation and host ABI affect program behavior.

**Conformant-array parameters (ISO §6.6.3.7):**

- Only `var` parameters may be conformant — value-parameter conformant arrays
  are not implemented.
- The bound identifiers (e.g. `Lo` and `Hi` in `array [Lo..Hi: integer]`) are
  **not accessible as Pascal variables** inside the procedure body. ISO 7185
  requires them to be in scope as integer variables carrying the actual bounds
  of the passed array.
- Internally, ptc maps all conformant arrays to 0-based C arrays. The lower
  bound is always 0 inside the called procedure; the upper-bound parameter
  carries the element count (`actual_hi − actual_lo + 1`), not the ISO upper
  bound. Valid indices inside the procedure are `0` to `count − 1`, not
  `Lo` to `Hi` in the ISO sense.

ISO 7185 is useful as a formal reference for the core language, but a Pascal
user manual or tutorial is more practical for learning to write programs.

## 14. Differences from Turbo Pascal

Turbo Pascal and later Borland Pascal are related languages, but many Turbo
features are not part of this dialect.

### Not supported

- `unit`, `uses`, `interface`, and `implementation`;
- Borland unit initialization/finalization;
- predefined `byte`, `shortint`, `word`, `longint`, and similar aliases;
- the built-in Turbo `string`/`ShortString` type;
- typed constants and variable initializers;
- `const` parameters;
- open-array parameters in Borland syntax;
- objects, classes, constructors, destructors, and methods;
- method pointers;
- routine overloading;
- default parameters;
- `cdecl`, `pascal`, `stdcall`, `register`, `far`, and `near` directives;
- inline assembler;
- `absolute`;
- compiler directives such as `{$I ...}`;
- `//` comments;
- hexadecimal `$` literals;
- Turbo-specific library routines such as `Inc`, `Dec`, `Assign`,
  `ParamStr`, and `ParamCount`;
- `break`, `continue`, and `exit` with modern Turbo/Free Pascal meanings.

### Important behavioral differences

1. `input` and `output` should be listed in the `program` heading when used.

2. There is no Borland unit system. Source files do not automatically form
   namespaces.

3. `external` is currently only a simple directive. It has no library name,
   symbol-name override, header, or calling-convention clause.

4. A long Pascal string is not represented like a Turbo Pascal short string or
   a C string.

5. `otherwise` is the implemented default `case` spelling.

6. `message(...)` writes diagnostics to standard error.

7. Subrange declarations influence the selected C integer type.

## 15. Current foreign-function interface

### 15.1 Simple libc example

On Linux and other Unix-like systems, a simple no-argument libc function can be
declared directly:

```pascal
program ParentPid(output);

function getppid: integer; external;

begin
    writeln(getppid)
end.
```

Build normally:

```sh
./ptc < parentpid.p > parentpid.c
cc -std=c89 -o parentpid parentpid.c
```

The C linker driver automatically links the normal C library.

### 15.2 Separate C source example

C:

```c
int twice(int value)
{
    return value * 2;
}
```

Pascal:

```pascal
program TwiceDemo(output);

function twice(value: integer): integer; external;

begin
    writeln(twice(21))
end.
```

Build:

```sh
./ptc < twice.p > twice-pascal.c
cc -std=c89 -o twice-demo twice-pascal.c twice.c
```

### 15.3 Extra library example

```pascal
program CeilingDemo(output);

function ceil(value: real): real; external;

begin
    writeln(ceil(3.14):0:1)
end.
```

Build with the mathematics library:

```sh
./ptc < ceiling.p > ceiling.c
cc -std=c89 -o ceiling ceiling.c -lm
```

### 15.4 What currently works reasonably

The current FFI is suitable for simple routines where:

- the Pascal identifier can be used unchanged as the C linker symbol;
- Pascal `integer` corresponds to C `int`;
- Pascal `real` corresponds to C `double`;
- a `var` parameter corresponds to a pointer to a compatible C object;
- no special calling convention is required;
- the linker options are supplied manually.

The semantic checker verifies calls against the Pascal declaration.

### 15.5 Current limitations

The current FFI does not provide:

- an explicit C symbol name;
- an explicit header name;
- an explicit library name or linker option;
- C prototypes in generated declarations;
- portable C fixed-width integer types;
- `size_t`, `ptrdiff_t`, or other ABI types;
- `const` pointers;
- a general `void *` or opaque-pointer type;
- C string conversion;
- external global variables;
- variadic C functions;
- C structures and unions with guaranteed ABI layout;
- callback calling-convention declarations;
- control over symbol visibility;
- C macros or inline functions.

The external symbol is derived from the Pascal identifier. This can fail when
PTC renames an identifier to avoid a C keyword or runtime-name collision.

## 16. Runtime safety checks (`ptc -r`)

Passing the `-r` flag causes ptc to emit additional safety-checking code in the
generated C. These checks are compiled into the executable and abort with a
diagnostic when a violation is detected.

### Usage

```sh
./ptc -r < program.p > program.c
cc -std=c89 -o program program.c
./program
```

The `-r` flag must be the first command-line argument.

### What is checked

**Nil-pointer dereference.** Every `P^` and `P^.field` dereference is wrapped
in a `Chknil` macro. If the pointer is nil at runtime, the program aborts:

```
Fatal: nil pointer dereference
```

**Static array bounds.** Every index into a statically declared array is
wrapped in `Chkidx` with the declared compile-time bounds. Violation aborts:

```
Fatal: array index out of bounds
```

**Conformant-array bounds.** Index operations on conformant-array parameters
are checked at runtime against the actual element count supplied by the caller.

**Wild memory access.** A SIGSEGV handler (`Pasjmp`) is registered in `main`
so that access violations not caught by the above checks produce a diagnostic
message and a clean exit instead of a bare OS fault.

### What is not reported

- **Source location**: ptc discards source positions during translation, so no
  file name or line number can be reported at the error site.
- **Module attribution**: no module system exists, so per-file attribution is
  not feasible.
- **Uninitialized variables**: no initialization tracking is performed.
- **Integer overflow**: not detected.

### Emitted C constructs

```c
#define Chknil(p) \
    ((p)?(p):(fprintf(stderr,"Fatal: nil pointer dereference\n"),exit(1),(p)))
#define Chkidx(i,l,h) \
    ((i)>=(l)&&(i)<=(h)?(i):(fprintf(stderr,"Fatal: array index out of bounds\n"),exit(1),(i)))
```

A SIGSEGV handler function `Pasjmp` is emitted, and `signal(SIGSEGV, Pasjmp)`
is called at the start of the generated `main`.

