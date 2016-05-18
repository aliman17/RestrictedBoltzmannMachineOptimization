#!/bin/bash
./run_plot_Utrans_S.sh "$@"
Rscript ./plot_benchmarks.R $1 2> /dev/null
