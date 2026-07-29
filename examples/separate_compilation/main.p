(* main.p - the only file with a program heading.
   Uses square() and cube() from mathlib.p.
   Declare them external so ptc knows they are defined in another object file. *)

program demo(output);

var i : integer;

function square(n : integer) : integer; external;
function cube(n : integer)   : integer; external;

begin
  for i := 1 to 5 do
    begin
      write(i:3);
      write(square(i):6);
      writeln(cube(i):8)
    end
end.
