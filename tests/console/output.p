program consoleoutput(output);
type wordvalue = 0..65535;
var
  textvalue: array [1..4] of char;
  calls: integer;
  wordnumber: wordvalue;

function tick: integer;
begin
  calls := calls + 1;
  tick := -7
end;

function fieldwidth: integer;
begin
  calls := calls + 1;
  fieldwidth := 4
end;

begin
  calls := 0;
  textvalue := 'A%B!';
  wordnumber := 65535;
  write('literal%:', true, false:7, 'Z':3);
  writeln('|', tick:fieldwidth, '|', tick:0, '|', textvalue:6,
          '|', wordnumber:0);
  writeln('calls=', calls:1);
  writeln;
  page(output);
  flush(output);
  message('done', 1:1)
end.
