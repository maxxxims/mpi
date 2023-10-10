#!/bin/bash

#PBS -l walltime=00:5:00,nodes=1:ppn=2
#PBS -N send_info
#PBS -q batch

cd $PBS_O_WORKDIR
mpirun --hostfile $PBS_NODEFILE -np 2 ./send_info
