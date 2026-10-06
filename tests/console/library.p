(* Console state is shared across separate translations. *)
procedure fromlibrary;
var ch: char;
begin
  if not eof then
  begin
    read(ch);
    write(ch)
  end
end;
