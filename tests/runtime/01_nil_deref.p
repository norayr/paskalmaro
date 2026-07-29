program nil_deref(output);
type
  node = record
    val : integer
  end;
var
  p : ^node;
begin
  p := nil;
  writeln(p^.val)
end.
