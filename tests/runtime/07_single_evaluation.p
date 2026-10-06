program singleevaluation(output);
type
  node = record value: integer end;
  nodeptr = ^node;
var
  a: array [1..3] of integer;
  p: nodeptr;
  pointers: array [1..3] of nodeptr;
  calls: integer;

function nextindex: integer;
begin
  calls := calls + 1;
  nextindex := calls
end;

begin
  calls := 0;
  a[nextindex] := 42;
  writeln(calls:1, ' ', a[1]:1);
  new(p);
  pointers[1] := p; pointers[2] := p; pointers[3] := p;
  calls := 0;
  pointers[nextindex]^.value := 17;
  writeln(calls:1, ' ', p^.value:1);
  dispose(p)
end.
