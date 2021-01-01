#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

clear_build_dir () {
    rm -rf $DIR/../build
}

invoke_cmake () {
    mkdir -p $DIR/../build
    cd $DIR/../build
    cmake -D$1=ON ..
    cmake --build .
}

if [ "$#" -eq 0 ]; then
    echo "Illegal number of parameters"
    exit 1
fi

if [ "$#" -eq 2 ] && [ $2 == "clean" ]; then clear_build_dir; fi
if [ $1 == "client" ]; then invoke_cmake "BuildClient"; fi
if [ $1 == "server" ]; then invoke_cmake "BuildServer"; fi
