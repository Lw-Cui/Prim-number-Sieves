#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>

int simple_sieves(int origin) {
	bool *array = new bool[origin + 1];
	memset(array, true, sizeof(bool) * (origin + 1));
	array[0] = array[1] = false;
	int prime = 0;
	while (prime * prime <= origin) {
		do 
			prime++;
		while (!array[prime]);
		for (int i = prime * prime; i <= origin; i++)
			if (array[i] && i % prime == 0)
				array[i] = !array[i];
	}
	int ans = 0;
	for (int i = 0; i <= origin; i++)
		if (array[i]) ans++;
	return ans;
}

int sieves(int origin, int p, int id) {
	// index range from 0 to origin (origin + 1 elements)
	int num = (origin + 1) / p + ((origin + 1) % p? 1: 0);
	assert(num);
	bool *array = new bool[num];
	int base = id * num;
	// The range of process n is [id * num, 2 * id * num - 1]
	memset(array, true, sizeof(bool) * num);
	// Zero and one isn't prime
	if (!id) array[0] = array[1] = false;
	int prime = 2;
	do {
		// The first element of sub-array is (id * num)
		int i = 0;
		if (!(id && base % prime)) i = 0;
		else i = prime - base % prime;
		while (i < num) {
			// array[i] isn't prime if 1)array[i] % prime == 0 2)array[i] != prime
			if (array[i] && base + i != prime)
				array[i] = !array[i];
			i += prime;
		}
		do 
			prime++;
		while (prime < num && !array[prime]);
		MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
	} while (prime * prime < origin);
	int cnt = 0;
	const size_t len = 10;
	char str[len];
	snprintf(str, len, "Prime.%d", id);
	FILE *pFile;
	if (getenv("DEBUG")) pFile = fopen(str, "w");
	for (int i = 0; i < num; i++)
		if (array[i] && base + i <= origin) {
			cnt++;
			if (getenv("DEBUG"))
			  fprintf(pFile, "%d\n", base + i);
		}
	if (getenv("DEBUG")) fclose(pFile);
	delete[] array;
	int ans;
	MPI_Reduce(&cnt, &ans, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	return ans;
}

int main(int argc, char *argv[]) {
	int origin;
	assert(argc >= 2 && sscanf(argv[1], "%d", &origin) >= 0);
	MPI_Init(&argc, &argv);
	int id;
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	int p;
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	// num: the integer number of each process
	// n / p > sqrt(n)(low floor) -> n > p ^ 2
	int ans;
	if (p * p > origin)
		ans = simple_sieves(origin);
	else
		ans = sieves(origin, p, id);
	if (!id) printf("Number of prime is %d.\n", ans);
	MPI_Finalize();
	return 0;
}
