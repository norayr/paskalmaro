program consoleseparate(input, output);
var ch: char;
procedure fromlibrary; external;
begin
  if not eof then
  begin
    fromlibrary;
    read(ch);
    writeln(ch)
  end
end.
