(* strlib.p - another module without a program heading.
   Provides a simple string-length counter for packed arrays.                 *)

function countch(var s : array [lo..hi : integer] of char;
                 count : integer; c : char) : integer;
var i, n : integer;
begin
  n := 0;
  { Bound names are not Pascal variables in the current PTC dialect. }
  for i := 0 to count - 1 do
    if s[i] = c then
      n := n + 1;
  countch := n
end;
