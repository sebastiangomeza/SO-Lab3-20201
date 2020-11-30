/**
 * @defgroup   SAXPY saxpy
 *
 * @brief      This file implements an iterative saxpy operation
 * 
 * @param[in] <-p> {vector size} 
 * @param[in] <-s> {seed}
 * @param[in] <-n> {number of threads to create} 
 * @param[in] <-i> {maximum itertions} 
 *
 * @author     Danny Munera
 * @date       2020
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <pthread.h>

// Variables to obtain command line parameters
unsigned int seed = 1;
int p = 10000000;
int n_threads = 8;
int max_iters = 1000;
// Variables to perform SAXPY operation
double *X;
double a;
double *Y;
double *Y_avgs;
int i, it;
// Variables to get execution time
struct timeval t_start, t_end;
double exec_time;

void *calcular(void *arg)
{
	char *is;
	is = (char *)arg;
	int its = is[0] - 48;
	
	for (its = its; its < max_iters; its = its + n_threads)
	{
		for (i = 0; i < p; i++)
		{
			Y[i] = Y[i] + a * X[i];
			Y_avgs[its] += Y[i];
		}
		Y_avgs[its] = Y_avgs[its] / p;
	}
	return NULL;
}
void *oneThreads(void *arg)
{
	char *var1 = "0";
	pthread_t h1;
	pthread_create(&h1, NULL, calcular, (void *)var1);
	pthread_join(h1, NULL);
	return NULL;
}

void *twoThreads(void *arg)
{
	char *var1 = "0";
	char *var2 = "1";
	pthread_t h1;
	pthread_t h2;
	pthread_create(&h1, NULL, calcular, (void *)var1);
	pthread_create(&h2, NULL, calcular, (void *)var2);
	pthread_join(h1, NULL);
	pthread_join(h2, NULL);
	return NULL;
}

void *fourThreads(void *arg)
{
	char *var1 = "0";
	char *var2 = "1";
	char *var3 = "2";
	char *var4 = "3";
	pthread_t h1;
	pthread_t h2;
	pthread_t h3;
	pthread_t h4;
	pthread_create(&h1, NULL, calcular, (void *)var1);
	pthread_create(&h2, NULL, calcular, (void *)var2);
	pthread_create(&h3, NULL, calcular, (void *)var3);
	pthread_create(&h4, NULL, calcular, (void *)var4);
	pthread_join(h1, NULL);
	pthread_join(h2, NULL);
	pthread_join(h3, NULL);
	pthread_join(h4, NULL);
	return NULL;
}

void *eightThreads(void *arg)
{
	char *var1 = "0";
	char *var2 = "1";
	char *var3 = "2";
	char *var4 = "3";
	char *var5 = "4";
	char *var6 = "5";
	char *var7 = "6";
	char *var8 = "7";
	pthread_t h1;
	pthread_t h2;
	pthread_t h3;
	pthread_t h4;
	pthread_t h5;
	pthread_t h6;
	pthread_t h7;
	pthread_t h8;
	pthread_create(&h1, NULL, calcular, (void *)var1);
	pthread_create(&h2, NULL, calcular, (void *)var2);
	pthread_create(&h3, NULL, calcular, (void *)var3);
	pthread_create(&h4, NULL, calcular, (void *)var4);
	pthread_create(&h5, NULL, calcular, (void *)var5);
	pthread_create(&h6, NULL, calcular, (void *)var6);
	pthread_create(&h7, NULL, calcular, (void *)var7);
	pthread_create(&h8, NULL, calcular, (void *)var8);
	pthread_join(h1, NULL);
	pthread_join(h2, NULL);
	pthread_join(h3, NULL);
	pthread_join(h4, NULL);
	pthread_join(h5, NULL);
	pthread_join(h6, NULL);
	pthread_join(h7, NULL);
	pthread_join(h8, NULL);
	return NULL;
}

int main(int argc, char *argv[])
{

	// Getting input values
	int opt;
	while ((opt = getopt(argc, argv, ":p:s:n:i:")) != -1)
	{
		switch (opt)
		{
		case 'p':
			printf("vector size: %s\n", optarg);
			p = strtol(optarg, NULL, 10);
			assert(p > 0 && p <= 2147483647);
			break;
		case 's':
			printf("seed: %s\n", optarg);
			seed = strtol(optarg, NULL, 10);
			break;
		case 'n':
			printf("threads number: %s\n", optarg);
			n_threads = strtol(optarg, NULL, 10);
			break;
		case 'i':
			printf("max. iterations: %s\n", optarg);
			max_iters = strtol(optarg, NULL, 10);
			break;
		case ':':
			printf("option -%c needs a value\n", optopt);
			break;
		case '?':
			fprintf(stderr, "Usage: %s [-p <vector size>] [-s <seed>] [-n <threads number>]\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	srand(seed);

	printf("p = %d, seed = %d, n_threads = %d, max_iters = %d\n",
		   p, seed, n_threads, max_iters);

	// initializing data
	X = (double *)malloc(sizeof(double) * p);
	Y = (double *)malloc(sizeof(double) * p);
	Y_avgs = (double *)malloc(sizeof(double) * max_iters);

	for (i = 0; i < p; i++)
	{
		X[i] = (double)rand() / RAND_MAX;
		Y[i] = (double)rand() / RAND_MAX;
	}
	for (i = 0; i < max_iters; i++)
	{
		Y_avgs[i] = 0.0;
	}
	a = (double)rand() / RAND_MAX;

#ifdef DEBUG
	printf("vector X= [ ");
	for (i = 0; i < p - 1; i++)
	{
		printf("%f, ", X[i]);
	}
	printf("%f ]\n", X[p - 1]);

	printf("vector Y= [ ");
	for (i = 0; i < p - 1; i++)
	{
		printf("%f, ", Y[i]);
	}
	printf("%f ]\n", Y[p - 1]);

	printf("a= %f \n", a);
#endif

	/*
	 *	Function to parallelize 
	 */
	gettimeofday(&t_start, NULL);
	//SAXPY iterative SAXPY mfunction

	//hilos aca
	switch (n_threads)
	{
	case 1:
		oneThreads(NULL);
		/* code */
		break;
	case 2:
		twoThreads(NULL);
		/* code */
		break;
	case 4:
		fourThreads(NULL);
		/* code */
		break;
	case 8:
		eightThreads(NULL);
		/* code */
		break;
	default:
		break;
	}

	// hilos finaliza

	gettimeofday(&t_end, NULL);

#ifdef DEBUG
	printf("RES: final vector Y= [ ");
	for (i = 0; i < p - 1; i++)
	{
		printf("%f, ", Y[i]);
	}
	printf("%f ]\n", Y[p - 1]);
#endif

	// Computing execution time
	exec_time = (t_end.tv_sec - t_start.tv_sec) * 1000.0;	 // sec to ms
	exec_time += (t_end.tv_usec - t_start.tv_usec) / 1000.0; // us to ms
	printf("Execution time: %f ms \n", exec_time);
	printf("Last 3 values of Y: %f, %f, %f \n", Y[p - 3], Y[p - 2], Y[p - 1]);
	printf("Last 3 values of Y_avgs: %f, %f, %f \n", Y_avgs[max_iters - 3], Y_avgs[max_iters - 2], Y_avgs[max_iters - 1]);
	return 0;
}
