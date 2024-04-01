#!/usr/local/bin/bash

mkdir build
cd build
cmake ..
make
./Earley
cd ..
rm -r build