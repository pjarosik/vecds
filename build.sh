#!/bin/bash

set -e

echo "BUILDING FEMApp"

./cleanallreleaseFEMApp 
./makeallreleaseFEMApp

echo "BUILDING vecds"

set -e
rm -rf build 
mkdir -p build 
cmake -S . -B build
cmake --build build --config Release -j8
cmake --install build --prefix bin
