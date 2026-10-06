program consolelines(input, output);
var ch: char;
begin
  while not eof(input) do
  begin
    if eof(input) then writeln('unexpected EOF');
    while not eoln(input) do
    begin
      if eoln(input) then writeln('unexpected EOL');
      read(input, ch);
      write(ch)
    end;
    readln(input);
    writeln('|')
  end;
  writeln(eof(input), ' ', eof, ' ', eoln(input));
  readln(input);
  writeln('end')
end.
