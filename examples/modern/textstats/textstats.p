(* A Unix-style byte-stream filter. ASCII whitespace separates words.
   An unterminated final line is included in the line count. *)
program textstats(output);
var
  lines, words, bytes, value: integer;
  inword, newlinestart: boolean;

function readbyte: integer; external;

begin
  lines := 0; words := 0; bytes := 0;
  inword := false; newlinestart := true;
  value := readbyte;
  while value >= 0 do
  begin
    bytes := bytes + 1;
    if newlinestart then lines := lines + 1;
    newlinestart := value = 10;
    if (value = 32) or ((value >= 9) and (value <= 13)) then
      inword := false
    else if not inword then
    begin
      words := words + 1;
      inword := true
    end;
    value := readbyte
  end;
  writeln('lines=', lines:1, ' words=', words:1, ' bytes=', bytes:1)
end.
