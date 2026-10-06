(* Fletcher-16 of binary standard input. No Pascal text-file conversion.
   Useful for asset pipelines, file comparisons, and testing C interfaces. *)
program checksum(output);
var value, sum1, sum2: integer;

function readbyte: integer; external;

begin
  sum1 := 0; sum2 := 0;
  value := readbyte;
  while value >= 0 do
  begin
    sum1 := (sum1 + value) mod 255;
    sum2 := (sum2 + sum1) mod 255;
    value := readbyte
  end;
  writeln(sum2 * 256 + sum1:1)
end.
