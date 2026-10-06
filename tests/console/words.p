program consolewords(input, output);
var
  tokenvalue: array [1..6] of char;
  separator, last: char;
begin
  read(input, tokenvalue, separator, last);
  writeln('[', tokenvalue, ']', separator, last)
end.
