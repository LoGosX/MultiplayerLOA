#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
target=$1
clear_build_dir () {
    rm -rf $DIR/../build/$target
}

invoke_cmake () {
    mkdir -p $DIR/../build/$target
    cd $DIR/../build/$target
    cmake -D$1=ON ../../
    cmake --build .
}

if [ "$#" -eq 0 ]; then
    echo "Illegal number of parameters"
    exit 1
fi

# if [ "$#" -eq 2 ] && [ $2 == "clean" ]; then clear_build_dir; fi
if [ $target == "client" ]; then invoke_cmake "BuildClient"; fi
if [ $target == "server" ]; then invoke_cmake "BuildServer"; fi
