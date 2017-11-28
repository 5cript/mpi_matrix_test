#!/bin/bash

export CC=gcc
export CXX=g++

mkdir -p build
cd build
if [[ -v MSYSTEM ]]; then
	cmake -G"MSYS Makefiles" ..
else
	cmake ..
fi
make -j4
