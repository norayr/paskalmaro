program externaltest(output);

function twice(x: integer): integer; external;

begin
    writeln(twice(21))
end.
