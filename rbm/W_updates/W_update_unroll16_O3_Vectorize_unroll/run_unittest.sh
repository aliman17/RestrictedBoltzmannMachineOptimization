#! /bin/bash

gcc -o unittest.o unittest.c -mfma
./unittest.o
