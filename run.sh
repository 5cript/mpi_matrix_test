#!/bin/bash

cd ./build
mpirun ./MpiMatrixTest "$@"
