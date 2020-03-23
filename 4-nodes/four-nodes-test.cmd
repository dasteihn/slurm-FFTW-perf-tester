#!/bin/bash

#SBATCH -J fftw_4_node_perftest
#SBATCH -D ./
#SBATCH -o ../logfiles/four-nodes/%x.%j.%N.out
#SBATCH --clusters=mpp3
#SBATCH --nodes=4
#SBATCH --get-user-env
#SBATCH --mail-type=end
#SBATCH --mem=1000mb
#SBATCH --mail-user=stanner@posteo.de
#SBATCH --export=NONE
#SBATCH --time=06:00:00
#SBATCH --cpus-per-task=256

module load fftw/3.3.8-intel-impi
mpiexec ./measprog
