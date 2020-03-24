/*
 *  (C) 2020, Philipp Stanner, <stanner@posteo.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fftw3-mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#define S_TO_US_FACTOR (1ULL * 1000 * 1000)

#define POTENZ 13u
#define THREADS_NUM 256u
#define NR_OF_FFTWS (8ULL * 1000)
#define NR_OF_CFG_ENTRIES 3

int threads_ok;

struct config {
	unsigned exponent, nr_of_threads, nr_of_fftws;
};


struct pretty_time {
	unsigned h, m, s, ms;
};

struct pretty_time
pretty_timediff(struct timeval tv_start, struct timeval tv_end)
{
	struct pretty_time result;
	long int diff_s = 0, diff_us = 0;
	time_t start, end;
	suseconds_t sustart, suend;

	start = tv_start.tv_sec;
	end = tv_end.tv_sec;
	sustart = tv_start.tv_usec;
	suend = tv_end.tv_usec;

	diff_s = end - start;
	diff_us = suend - sustart;

	if (diff_s <= 0 || diff_us <= 0)
		fprintf(stderr, "Severe problem with timestamps.\n");

	result.h = diff_s / (60 * 60);
	result.m = diff_s / 60 - result.h * 60;
	result.s = diff_s - result.h * 60 * 60 - result.m * 60;
	result.ms = diff_us / 1000;

	return result;
}


time_t sec_timediff(struct timeval tv_start, struct timeval tv_end)
{
	return tv_end.tv_sec - tv_start.tv_sec;
}


void pretty_printer(struct pretty_time t)
{
	printf("%02u:%02u:%02u.%03u\n", t.h, t.m, t.s, t.ms);
}

void get_config(struct config *cfg)
{
	char trash[256];
	unsigned scanned[NR_OF_CFG_ENTRIES], i = 0;
	FILE *fp = fopen("config.txt", "rt");
	if (!fp)
		goto err;

	while (fscanf(fp, "%s%u", trash, &scanned[i % NR_OF_CFG_ENTRIES]) == 2)
		i++;
		
	if (i != NR_OF_CFG_ENTRIES)
		goto err;
	
	cfg->exponent = scanned[0];
	cfg->nr_of_threads = scanned[1];
	cfg->nr_of_fftws = scanned[2];

	fclose(fp);
	return;

err:
	if (fp) fclose(fp);
	perror("get_config");
	exit(EXIT_FAILURE);
}


int main(int argc, char **argv)
{
	fftw_plan plan_fwd, plan_bwd;
	fftw_complex *data;
	int provided, i;
	long long ram_amount = 0, nr_of_values = 0;
	struct timeval tv_start, tv_end;
	struct config cfg;

	gettimeofday(&tv_start, NULL);
	get_config(&cfg);
	nr_of_values = (long long)exp2(cfg.exponent);
	ram_amount = nr_of_values * sizeof(fftw_complex);

	MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
	threads_ok = provided >= MPI_THREAD_FUNNELED;
	if (threads_ok) {
		/* this function should be called _before_ fftw_mpi_init */
		threads_ok = fftw_init_threads();
	}

	if (!threads_ok) {
		fprintf(stderr, "Could not initialize threads. Abort.\n");
		return EXIT_FAILURE;
	}

	/* At this point, we're ready to initialize MPI and plan with threads */
	fftw_mpi_init();
	fftw_plan_with_nthreads(cfg.nr_of_threads);

	/* prepare data for fourier transform.
	 * in this example, we will just calculate the transform
	 * of random ram_amount values. */
	printf("Trying to allocate %lli byte...\n", ram_amount);
	data = fftw_malloc(ram_amount);
	if (!data)
		return EXIT_FAILURE;

	puts("allocated!");
	/* create plan for in-place DFTs */
	plan_fwd = fftw_mpi_plan_dft_1d(nr_of_values, data, data,
			MPI_COMM_WORLD, FFTW_FORWARD, FFTW_ESTIMATE);    
	plan_bwd = fftw_mpi_plan_dft_1d(nr_of_values, data, data,
			MPI_COMM_WORLD, FFTW_BACKWARD, FFTW_ESTIMATE);    

	puts("Start executing plans...");
	for (i = 0; i < cfg.nr_of_fftws; i++) {
		fftw_execute(plan_fwd);
		fftw_execute(plan_bwd);
	}

	puts("Executed. Finalizing...");
	fftw_destroy_plan(plan_fwd);
	fftw_destroy_plan(plan_bwd);

	MPI_Finalize();
	fftw_free(data);

	gettimeofday(&tv_end, NULL);
	printf("Executed MPI-FFTW successfully %llu times.\n"
		"\tUsed %u Threads per Node.\n"
		"\tArray exponent was: %u.\n",
		cfg.nr_of_fftws, cfg.nr_of_threads, cfg.exponent);
	printf("\tTook (hh:mm:ss.ms): ");
	pretty_printer(pretty_timediff(tv_start, tv_end));
	printf("\tin seconds: %ld\n", sec_timediff(tv_start, tv_end));

	return EXIT_SUCCESS;
}
