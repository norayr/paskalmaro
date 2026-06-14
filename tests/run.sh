#!/bin/sh
set -eu

PTC=${PTC:-../ptc}
failed=0

for source in ./*.p; do
    output=${source%.p}.c
    diagnostics=${source%.p}.err
    if "$PTC" < "$source" > "$output" 2> "$diagnostics"; then
        echo "FAIL: $source was accepted"
        failed=1
    else
        printf 'PASS: %s: ' "$source"
        sed -n '1p' "$diagnostics"
    fi
    rm -f "$output" "$diagnostics"
done

exit "$failed"
