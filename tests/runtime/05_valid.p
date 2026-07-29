program valid(output);
type
  node = record
    val : integer
  end;
var
  a : array [1..3] of integer;
  p : ^node;
  i : integer;
begin
  for i := 1 to 3 do
    a[i] := i * 100;
  writeln(a[2]);
  new(p);
  p^.val := 42;
  writeln(p^.val);
  writeln('ok')
end.
