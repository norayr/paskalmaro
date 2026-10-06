program conformantforward(output);
var a: array [-2..0] of integer;

procedure putvalue(var b: array [lo..hi: integer] of integer;
                   value: integer);
begin
  b[2] := value
end;

procedure forwardvalue(var b: array [lo..hi: integer] of integer);
begin
  putvalue(b, 42)
end;

begin
  forwardvalue(a);
  writeln(a[0]:1)
end.
