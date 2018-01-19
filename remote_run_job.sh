#!/bin/bash -login

#PBS -N ebbeke_matrix
#PBS -j oe
#PBS -l nodes=4:ppn=16
#PBS -l walltime=200:00:00
#PBS -l mem=42gb

module load GCC/5.4.0-2.26 OpenMPI/1.10.3 CMake/3.7.1 Boost/1.61.0

cd $BIGWORK/mpi_matrix_test

bash run.sh -d1000
