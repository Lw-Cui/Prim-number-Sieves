#!/bin/bash
NUM=100000000
export DEBUG=true
mpirun.mpich -np 4 ./Sieves.out $NUM
unset DEBUG
sort Prime.* -n -o Prime
rm -rf Prime.*
./Verification.out $NUM > Standard
diff Prime Standard
[ $? -eq 0 ] && echo "Yeah"
