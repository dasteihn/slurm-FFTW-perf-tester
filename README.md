# slurm-FFTW-perf-tester

This repo measures how fast the FFTW combined with MPI+threads
calculates stuff on linux clusters using SLURM

It was run on the **Leibniz Rechenzentrum Clusters** in Germany, but should also
work on other clusters supporting FFTW+MPI

## What does this test do?

This test was created to simulate the behavior of a fiber-simulation tool at my
university, so it does rather specific stuff, but might be useful for others as
well.

Basically, it does:

- Allocate an amount of memory corresponding to a power of 2
- Use the random data in this allocated RAM to calculate the FFTWs and reverse FFTWs for a certain number of times
- Measure the time needed for the calculations
- Log the time into logfiles. Wohoo.

## Configurating

### The Config File

For each node-amount-folder-thingy there's a separate config file.

The config file contains three parameters:

1. The exponent for the power of 2 (amount of RAM)
2. The number of threads to use per node
3. How often the FFTW-invFFTW-pairs should be done

The order of these parameters is important, because I was too lazy to write a proper parser.

### The SLURM Script

There's also a slurm script for each node configuration in the subfolder.
You are responsible for adjusting the runtime and the minimum number of threads so it fits your `config.txt`

## Building

For LRZ systems:

```
module load fftw/3.3.8-intel-impi
make
```

## Running

- switch to the directory indicating how many nodes you want to use
- edit the config file
- edit the slurm script (`.cmd`)
- Run it: `sbatch 1-node-test.cmd`

