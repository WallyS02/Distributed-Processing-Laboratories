#!/bin/bash
gcc -Wall -c list.c -o list.o
gcc -Wall -c main.c -o main.o
ar r lib_list.a list.o
gcc main.o lib_list.a -o program_stat
./program_stat
