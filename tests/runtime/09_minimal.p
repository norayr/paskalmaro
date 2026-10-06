program minimaldemo;
var
  msg: array [1..3] of char;
  a: array [-1..1] of integer;
  i: integer;

procedure verify(value: integer); external;

begin
  msg := 'abc';
  for i := -1 to 1 do a[i] := i + 2;
  case a[0] of
    2: verify(ord(msg[2]) + a[-1] + a[1])
  end
end.
