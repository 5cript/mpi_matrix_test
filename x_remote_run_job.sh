#!/bin/bash -login

#PBS -N ebbeke_matrix
#PBS -j oe
#PBS -l nodes=16:ppn=4
#PBS -l walltime=200:00:00
#PBS -l mem=42gb

module load GCC/5.4.0-2.26
module load OpenMPI/1.10.3

cd $BIGWORK/mpi_matrix_test

bash run.sh
