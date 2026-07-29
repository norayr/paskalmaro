program array_bounds_lo(output);
var
  a : array [1..5] of integer;
  i : integer;
begin
  for i := 1 to 5 do
    a[i] := i * 10;
  i := 0;
  writeln(a[i])
end.
