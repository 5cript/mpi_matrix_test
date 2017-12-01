#!/bin/bash

export CC=gcc
export CXX=g++

mkdir -p build
cd build
if [[ -n ${!MSYSTEM} ]]; then
	cmake -G"MSYS Makefiles" ..
else
	cmake ..
fi
make -j4
