#!/bin/bash
# Copyright (c) 2022 Seth Galasso
# SPDX-License-Identifier: MIT

name="GTE"
cc="gcc"
flags="-Wall -Wextra -Wpedantic -Werror -g -Og"
compile_flags="-iquote include"
link_flags=""
max_threads=4

build_dir=.build
src_dir=src.conf

set -e
shopt -s nullglob
cd "$(dirname "$0")"

step () (
    echo "$@"
    "$@"
)

compile () {
    local in="$1"; shift

    if [[ -d $in ]]; then
        for file in "$in"/*; do
            if [[ ! -f $file.conf ]]; then
                compile "$file"
            fi
        done
    elif [[ $in == *.conf ]]; then
        . "$in"
    else
        local out="$build_dir/$in.o"
        step mkdir -p "$(dirname "$out")"
        submit step $cc $flags -c $compile_flags -o "$out" "$in"
        objs[${#objs[@]}]="$out"
    fi
}

threads=0
submit () {
    if [[ $threads -lt $max_threads ]]; then
        threads=$(($threads + 1))
    else
        wait -n
    fi
    "$@" &
}

waitall () {
    # plain `wait` would not check all return values
    while [[ $threads -ne 0 ]]; do
        wait -n
        threads=$(($threads - 1))
    done
}

main () {
    step rm -rf $build_dir
    compile $src_dir
    waitall
    step $cc $flags -o $build_dir/"$name" "${objs[@]}" $link_flags
}

main