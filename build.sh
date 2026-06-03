#!/bin/sh

make
cd build

cmake --build . 

./Voxely
