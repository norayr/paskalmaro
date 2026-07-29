program array_bounds_hi(output);
var
  a : array [1..5] of integer;
  i : integer;
begin
  for i := 1 to 5 do
    a[i] := i * 10;
  i := 6;
  writeln(a[i])
end.
