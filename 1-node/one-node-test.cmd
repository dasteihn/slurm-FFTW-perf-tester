#!/bin/bash

#SBATCH -J fftw_1_node_perftest
#SBATCH -D ./
#SBATCH -o ../logfiles/one-node/%x.%j.%N.out
#SBATCH --clusters=mpp3
#SBATCH --nodes=1
#SBATCH --get-user-env
#SBATCH --mail-type=end
#SBATCH --mem=2000mb
#SBATCH --mail-user=stanner@posteo.de
#SBATCH --export=NONE
#SBATCH --time=03:30:00
#SBATCH --cpus-per-task=256

module load fftw/3.3.8-intel-impi
mpiexec ./measprog
