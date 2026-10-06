(* A separately translated character-cell colour library.
   Oric colours are serial attributes, not an attribute plane: each logical
   pixel occupies PAPER attribute, one space, and a black PAPER reset.
   Logical resolution is 12 x 27, using columns 2..37 of the text screen. *)

type address = 0..65535;
var screen: address;

procedure oricpoke(addr: address; value: integer); external;

procedure loresinit;
var row, col: integer;
begin
  screen := 48000;
  { Select 50 Hz text mode; leave the last physical row for ROM use. }
  oricpoke(49119, 26);
  for row := 0 to 26 do
  begin
    for col := 0 to 39 do oricpoke(screen + row * 40 + col, 32);
    oricpoke(screen + row * 40, 16);
    oricpoke(screen + row * 40 + 1, 7)
  end
end;

procedure loresplot(x, y, colour: integer);
var addr: address;
begin
  if (x >= 0) and (x < 12) and (y >= 0) and (y < 27) and
     (colour >= 0) and (colour <= 7) then
  begin
    addr := screen + 2 + y * 40 + x * 3;
    oricpoke(addr, 16 + colour);
    oricpoke(addr + 1, 32);
    oricpoke(addr + 2, 16)
  end
end;
