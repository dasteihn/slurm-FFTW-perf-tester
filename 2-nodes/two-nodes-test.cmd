#!/bin/bash

#SBATCH -J fftw_2_node_perftest
#SBATCH -D ./
#SBATCH -o ../logfiles/two-nodes/%x.%j.%N.out
#SBATCH --clusters=mpp3
#SBATCH --nodes=2
#SBATCH --get-user-env
#SBATCH --mail-type=end
#SBATCH --mem=1000mb
#SBATCH --mail-user=stanner@posteo.de
#SBATCH --export=NONE
#SBATCH --time=00:15:00
#SBATCH --cpus-per-task=256

module load fftw/3.3.8-intel-impi
mpiexec ./measprog
