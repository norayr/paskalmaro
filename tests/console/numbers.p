program consolenumbers(input, output);
type
  wordvalue = 0..65535;
  bytevalue = 0..255;
  smallvalue = -10..10;
var
  n: integer;
  w: wordvalue;
  b: bytevalue;
  s: smallvalue;
begin
  readln(input, n, w, b, s);
  writeln(n:0, ',', w:0, ',', b:0, ',', s:0)
end.
