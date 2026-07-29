program nil_arrow(output);
type
  node = record
    val : integer;
    next : ^node
  end;
var
  p : ^node;
begin
  p := nil;
  writeln(p^.next^.val)
end.
