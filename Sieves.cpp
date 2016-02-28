#include <stdio.h>
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
		printf("%d\n", prime);
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
	// The range of process n is [n * num, 2n * num - 1]
	memset(array, true, sizeof(bool) * num);
	if (!id) array[0] = array[1] = false;
	int prime = 0;
	while (prime * prime <= origin) {
		do 
			prime++;
		while (!array[prime]);
		MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
		for (int i = 0; i < num; i++)
			//array[i] isn't prime if 1)array[i] % prime == 0 2)array[i] != prime
			if (array[i] && (id * num + i) % prime == 0 && id * num + i != prime)
				array[i] = !array[i];
	}
	int cnt = 0;
	for (int i = 0; i < num; i++)
		if (array[i] && id * num + i <= origin) {
		 cnt++;
		}
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
