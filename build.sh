#!/bin/bash

export CC=gcc
export CXX=g++

mkdir -p build
cd build
cmake ..
make -j4
