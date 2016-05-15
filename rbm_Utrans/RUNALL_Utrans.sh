#!/bin/bash
./run_plot_Utrans.sh "$@"
Rscript ./plot_benchmarks.R $1 2> /dev/null
