#!/usr/bin/env bash

set -euo pipefail

now () {
    date +%s.%N
}

flags=(
    "-ferror-limit=1"
    -fshort-enums
    "-march=native"
    -O1
    "-std=c99"
    -Werror
    -Weverything
    -Wno-declaration-after-statement
    -Wno-disabled-macro-expansion
    -Wno-extra-semi-stmt
    -Wno-padded
    -Wno-reserved-id-macro
)
bins=(
    test_heap
    test_dijkstra
    main
)

(
    clang-format -i -verbose "$WD/src/"*
    start=$(now)
    for x in "${bins[@]}"; do
        mold -run clang "${flags[@]}" -o "$WD/bin/$x" "$WD/src/$x.c"
    done
    end=$(now)
    python3 -c "print(\"Compiled! ({:.3f}s)\".format(${end} - ${start}))"
)

"$WD/bin/test_heap"
"$WD/bin/test_dijkstra"
"$WD/bin/main" | dot -Tpng > "$WD/out/main.png"
feh "$WD/out/main.png"
