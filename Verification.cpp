#include <stdio.h>
#include <string.h>
#include <assert.h>

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
		if (array[i]) {
			ans++;
			printf("%d\n", i);
			fflush(stdout);
		}
	return ans;
}

int main(int argc, char *argv[]) {
	int origin;
	assert(argc >= 2 && sscanf(argv[1], "%d", &origin) >= 0);
	int ans;
	ans = simple_sieves(origin);
	//printf("Number of prime is %d.\n", ans);
	return 0;
}