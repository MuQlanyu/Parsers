#!/usr/local/bin/bash

mkdir build
cd build
cmake ..
make
./tests/Tests
cd ..
rm -r build