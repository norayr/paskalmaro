program confarr_bounds(output);

var
  arr : array [1..4] of integer;
  i   : integer;

procedure show(var a : array [lo..hi : integer] of integer);
var j : integer;
begin
  j := 5;  (* one past the end of a 4-element array *)
  writeln(a[j])
end;

begin
  for i := 1 to 4 do
    arr[i] := i * 10;
  show(arr)
end.
