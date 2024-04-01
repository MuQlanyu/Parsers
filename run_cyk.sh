#!/usr/local/bin/bash

mkdir build
cd build
cmake ..
make
./CYK
cd ..
rm -r build