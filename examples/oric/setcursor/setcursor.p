(* setcursor.p - position the cursor on Oric Atmos then print.
 *
 * setcursor(col, row) writes column to $0269 and row to $0268,
 * then calls $DA0C to recalculate the screen row address.
 * col: 0..39, row: 0..27.
 *
 * Build:
 *   ptc -m -i16 < setcursor.p > setcursor.c
 *   cl65 -t atmos -O -o setcursor.tap setcursor.c rom.s     *)

program setcursor_demo;

var
    msg : packed array [1..13] of char;
    i   : integer;

procedure clrscr;                        external;
procedure setcursor(col, row : integer); external;
procedure putch(c : char);               external;
procedure newline;                       external;

begin
    clrscr;

    setcursor(10, 5);
    msg := 'Row 5, col 10';
    for i := 1 to 13 do
        putch(msg[i]);

    setcursor(0, 12);
    msg := 'Row 12, col 0';
    for i := 1 to 13 do
        putch(msg[i]);

    newline
end.
