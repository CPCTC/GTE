#!/bin/bash
# Copyright (c) 2022 Seth Galasso
# SPDX-License-Identifier: MIT

name="GTE"
cc="gcc"
flags="-Wall -Wextra -Wpedantic -Werror -g -Og"
compile_flags="-iquote include"\
" -DAPPNAME=\"$name\""\
" -DAPPVMAJOR=0"\
" -DAPPVMINOR=0"\
" -DAPPVPATCH=0"\
" -DDEBUG"\

link_flags="$(pkg-config --libs glfw3 vulkan libxxhash)"
max_threads=4

build_dir=.build
src_dir=src
shader_dir=shader

cmds[${#cmds[@]}]="gen_size;"
gen_size () {
    (
        unset objs
        name=cache-line-size
        link_flags+=" -lcpuid"
        build_dir="cache/$build_dir"
        src_dir=cache
        unset 'cmds'
        main

        $build_dir/cache-line-size cache/.build/size
    )
    compile_flags+=" -DBUFF_DATA_CAP=$(cat cache/.build/size)"
}
cmds[${#cmds[@]}]="gen_shader;"
gen_shader () {
    if [[ -f "$shader_dir/conf" ]]; then
        compile "$shader_dir/conf"
    else
        compile "$shader_dir"
    fi
}

#
# End Config
#

set -e
shopt -s nullglob
cd "$(dirname "$0")"

step=pretty_step
pretty_step () {
    if [[ -v in ]]; then
        echo "CC $in"
    else
        echo "LD"
    fi
    "$@"
}
dbg_step () {
    echo "$@"
    "$@"
}

compiler=default_compiler
default_compiler () {
    submit $step $cc $flags -c $compile_flags -o "$out.o" "$in"
    objs[${#objs[@]}]="$out.o"
}

compile () {
    local in="$1"; shift

    if [[ -d $in ]]; then
        for file in "$in"/*; do
            if [[ $file != */conf && ! -f $file.conf ]]; then
                if [[ -f $file/conf ]]; then
                    compile "$file/conf"
                else
                    compile "$file"
                fi
            fi
        done
    else
        local out="$build_dir/$in"
        mkdir -p "$(dirname "$out")"
        if [[ $in =~ [./]conf$ ]]; then
            in="$(echo "$in" | sed 's|.conf$||')" \
                out="$build_dir/$in" \
                . "$in"
        else
            $compiler
        fi
    fi
}

threads=0
submit () {
    if [[ $threads -lt $max_threads ]]; then
        threads=$(($threads + 1))
    else
        wait -n || {
            threads=$(($threads - 1))
            waitall
            return 1
        }
    fi
    "$@" &
}

waitall () {
    local r=0
    while [[ $threads -ne 0 ]]; do
        wait -n || r=1
        threads=$(($threads - 1))
    done
    return $r
}

main () {
    rm -rf "$build_dir"
    eval "$@"
    eval "${cmds[@]}"
    if [[ -f "$src_dir/conf" ]]; then
        compile "$src_dir/conf"
    else
        compile "$src_dir"
    fi
    waitall
    $step $cc $flags -o "$build_dir/$name" "${objs[@]}" $link_flags
}

main "$@"
