#!/bin/sh

glslc assets/shaders/triangle.vert -o assets/shaders/triangle.vert.spv
glslc assets/shaders/triangle.frag -o assets/shaders/triangle.frag.spv
cd build

cmake --build . 

./Voxely
