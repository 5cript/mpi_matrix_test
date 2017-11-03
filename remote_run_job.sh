#!/bin/bash -login

#PBS -N ebbeke_matrix
#PBS -j oe
#PBS -l nodes=4:ppn=2
#PBS -l walltime=00:10:00
#PBS -l mem=4gb

module load GCC/5.4.0-2.26
module load OpenMPI/1.10.3

cd $BIGWORK/mpi_matrix_test

bash run.sh
