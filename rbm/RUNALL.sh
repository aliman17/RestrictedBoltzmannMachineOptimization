#!/bin/bash
./run_plot.sh "$@"
Rscript ./plot_benchmarks.R $1 2> /dev/null
