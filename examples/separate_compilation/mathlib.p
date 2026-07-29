(* mathlib.p - utility routines, no program heading so it becomes a module.
   Compile with:  ptc < mathlib.p > mathlib.c  then  cc -c mathlib.c        *)

function square(n : integer) : integer;
begin
  square := n * n
end;

function cube(n : integer) : integer;
begin
  cube := n * n * n
end;
