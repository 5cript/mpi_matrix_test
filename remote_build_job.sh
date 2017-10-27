#!/bin/bash -login

#PBS -N ebbeke_matrix
#PBS -j oe
#PBS -l nodes=1:ppn=1
#PBS -l walltime=00:05:00
#PBS -l mem=2gb

module load foss/2016b
module load CMake/3.7.1
module load Boost/1.61.0

cd $BIGWORK/mpi_matrix_test

bash build.sh
