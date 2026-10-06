program loresdemo;
var x, y, oldx, oldy, colour, pressedkey: integer;

procedure loresinit; external;
procedure loresplot(x, y, colour: integer); external;
function orickey: integer; external;

begin
  loresinit;
  x := 5; y := 13; colour := 2;
  loresplot(x, y, colour);
  repeat
    oldx := x; oldy := y;
    pressedkey := orickey;
    case pressedkey of
      65, 97: if x > 0 then x := x - 1;
      68, 100: if x < 11 then x := x + 1;
      87, 119: if y > 0 then y := y - 1;
      83, 115: if y < 26 then y := y + 1;
      67, 99: colour := colour mod 7 + 1;
      otherwise: begin end
    end;
    loresplot(oldx, oldy, 0);
    loresplot(x, y, colour)
  until (pressedkey = 81) or (pressedkey = 113)
end.
