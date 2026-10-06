(* hello.p - print a string on Oric Atmos using ROM character output.
 *
 * putch sends one character to $CCD9 (ROM char output, char in A).
 * newline calls $CBF0 (ROM NEWLINE: CR then LF).
 *
 * Build:
 *   ptc -m -i16 < hello.p > hello.c
 *   cl65 -t atmos -O -o hello.tap hello.c rom.s            *)

program hello;

var
    msg : packed array [1..12] of char;
    i   : integer;

procedure putch(c : char); external;
procedure newline;         external;

begin
    msg := 'Hello, Oric!';
    for i := 1 to 12 do
        putch(msg[i]);
    newline
end.
