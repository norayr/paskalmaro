program consoledemo(input, output);
var
  namevalue: array [1..12] of char;
  numbervalue: integer;
begin
  writeln('Pascal console backend');
  write('Name (one word, max 12): ');
  flush(output);
  readln(namevalue);
  write('Signed integer: ');
  flush(output);
  readln(numbervalue);
  writeln('Hello, [', namevalue, ']');
  writeln('Number: ', numbervalue:6);
  writeln('Positive: ', numbervalue >= 0)
end.
