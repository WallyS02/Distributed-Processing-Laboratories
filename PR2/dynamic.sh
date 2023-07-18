#!/bin/bash
gcc -Wall -c main.c -o main.o
gcc -Wall -fPIC -c list.c
gcc list.o -shared -o lib_list.so
gcc main.o -L. -l_list -o program_dynamic
export LD_LIBRARY_PATH=.:"$LD_LIBRARY_PATH"
./program_dynamic
