CC := mpicxx.mpich2
CXXFLAG := -Wall -Wextra -O2

all:	Sieves.out

Sieves.out:	Sieves.o
	$(CC) $< -o $@ -g $(CXXFLAG)

Sieves.o: Sieves.cpp
	$(CC) $< -o $@ -g -c $(CXXFLAG)

clean:
	rm -rf *.out *.o

rebuild:	clean all

.PHONY: all rebuild