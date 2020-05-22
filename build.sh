#!/bin/bash
current_dir=$(pwd)
script_dir=$(dirname $0)

if [ $script_dir = '.' ]; then
    script_dir="$current_dir"
fi

cd "${current_dir}"
mkdir -p ./build
touch ./build/.gitkeep

cd "${current_dir}/build"
cmake ../

cmake --build . --config Debug --target all
