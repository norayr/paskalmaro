program consolearrays(input, output);
var chars: array [-1..2] of char;

procedure fill(var value: array [lo..hi: integer] of char);
begin
  read(value)
end;

procedure display(var value: array [lo..hi: integer] of char);
begin
  writeln('[', value:6, ']')
end;

begin
  fill(chars);
  display(chars)
end.
