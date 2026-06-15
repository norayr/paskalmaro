program ParentPid(output);

function getppid: integer; external;

begin
    writeln('Parent process ID: ', getppid)
end.
