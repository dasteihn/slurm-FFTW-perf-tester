# slurm-FFTW-perf-tester

This test measures the FFTWs performance on Linux Clusters using SLURM

It was run on the **Leibniz Rechenzentrum Clusters** in Germany, but should also
work on other clusters supporting FFTW+MPI

## What does this test do?

This test was created to simulate the behavior of a fiber-simulation tool at my
university, so it does rather specific stuff, but might be useful for others as
well.

Basically, it does:

- Allocate an amount of memory corresponding to a power of 2
- Set each byte to 0x00
- Use these bytes to calculate the FFTWs and reverse FFTWs for a certain number
  of times
- Measure the total program runtime.
- Log the time into logfiles. Wohoo.

## Configurating

### The Config File

For each node-amount-folder-thingy there's a separate config file.

The config file contains three parameters:

1. The exponent for the power of 2 (amount of RAM)
2. The number of threads to use per node
3. How often the FFTW-invFFTW-pairs should be done

The order of these parameters is important, because I was too lazy to write a
proper parser.

### The SLURM Script

> Note:
Don't forget to add your e-mail-adress in your scripts! Otherwise the cluster
won't inform you that your job has failed and you will be too proud of yourself.

There's also a slurm script for each node configuration in the subfolder. You
are responsible for adjusting:

- the runtime
- the minimum number of threads so
- the approx. needed amount of RAM

The number of threads must always be at least as large as the number specified
in the config file.

## Building

For LRZ systems:

```
module load fftw/3.3.8-intel-impi
make
```

## Running

- If you run for the first time: `make prepare`
- switch to the directory indicating how many nodes you want to use
- edit the config file
- edit the slurm script (`.cmd`)
- Run it: `sbatch SLURMSKRIPT.cmd`

