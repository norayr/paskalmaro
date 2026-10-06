(* clrscr.p - clear the Oric Atmos screen then print a message.
 *
 * clrscr calls ROM entry $CCCE which sends Ctrl-L ($0C) to the
 * character output routine, clearing the screen.
 * Equivalent: putch(chr(12)).
 *
 * Build:
 *   ptc -m -i16 < clrscr.p > clrscr.c
 *   cl65 -t atmos -O -o clrscr.tap clrscr.c rom.s          *)

program clrscr_demo;

var
    msg : packed array [1..15] of char;
    i   : integer;

procedure clrscr;          external;
procedure putch(c : char); external;
procedure newline;         external;

begin
    clrscr;
    msg := 'Screen cleared!';
    for i := 1 to 15 do
        putch(msg[i]);
    newline
end.
