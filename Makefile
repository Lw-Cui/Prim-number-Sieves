CC := mpicxx.mpich2
CXXFLAG := -Wall -Wextra -O2

all:	Sieves.out Verification.out

Verification.out: Verification.o
	$(CC) $< -o $@ -g $(CXXFLAG)
	
Sieves.out:	Sieves.o
	$(CC) $< -o $@ -g $(CXXFLAG)

Sieves.o: Sieves.cpp
	$(CC) $< -o $@ -g -c $(CXXFLAG)

Verification.o:	Verification.cpp
	$(CC) $< -o $@ -g -c $(CXXFLAG)

clean:
	rm -rf *.out *.o

rebuild:	clean all

.PHONY: all rebuild