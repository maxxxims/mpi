#!/bin/bash

#PBS -l walltime=00:5:00,nodes=2:ppn=1
#PBS -N send_nothing2
#PBS -q batch

cd $PBS_O_WORKDIR
mpirun --hostfile $PBS_NODEFILE -np 2 ./send_nothing2
