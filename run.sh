#!/bin/bash

cd ./build

# Windows or Linux?
if [[ -v MSYSTEM ]]; then
	export PATH="$MSMPI_BIN":$PATH
	mpiexec -n 4 ./MpiMatrixTest "$@"
else
	mpirun ./MpiMatrixTest "$@"
fi