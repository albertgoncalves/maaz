#!/usr/bin/env bash

set -euo pipefail

now () {
    date +%s.%N
}

flags=(
    "-fno-strict-aliasing"
    "-fshort-enums"
    "-fsingle-precision-constant"
    "-g"
    "-march=native"
    "-O1"
    "-static"
    "-std=c11"
    "-Wall"
    "-Wcast-align"
    "-Wcast-qual"
    "-Wconversion"
    "-Wdate-time"
    "-Wdouble-promotion"
    "-Wduplicated-branches"
    "-Wduplicated-cond"
    "-Werror"
    "-Wextra"
    "-Wfatal-errors"
    "-Wfloat-equal"
    "-Wformat-signedness"
    "-Wformat=2"
    "-Winline"
    "-Wlogical-op"
    "-Wmissing-declarations"
    "-Wmissing-include-dirs"
    "-Wmissing-prototypes"
    "-Wnull-dereference"
    "-Wpacked"
    "-Wpedantic"
    "-Wpointer-arith"
    "-Wredundant-decls"
    "-Wshadow"
    "-Wstack-protector"
    "-Wstrict-prototypes"
    "-Wswitch-enum"
    "-Wtrampolines"
    "-Wundef"
    "-Wunused"
    "-Wunused-macros"
    "-Wwrite-strings"
)

(
    cppcheck --enable=all --suppress=missingIncludeSystem "$WD/src"
    clang-format -i -verbose "$WD/src/"*
    start=$(now)
    gcc "${flags[@]}" -o "$WD/bin/main" "$WD/src/main.c"
    end=$(now)
    python3 -c "print(\"Compiled! ({:.3f}s)\".format(${end} - ${start}))"
)

"$WD/bin/main" | dot -Tpng > "$WD/out/main.png"
feh "$WD/out/main.png"
