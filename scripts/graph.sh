#!/usr/bin/env bash

set -euo pipefail

handle=$(echo "$1" | sed 's/^[a-z\/]*\/\(.*\)\.[a-z]*$/\1/')
output="$WD/out/$handle.png"
dot -Tpng "$1" -o"$output"
feh "$output"
