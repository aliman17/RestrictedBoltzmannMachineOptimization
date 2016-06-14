#!/bin/sh

array=( 2 3 5 10 20 30 40 50 70 120 160 200 300 400 500 600 700 800 1100 1500 2000)

echo $2 > $1/compiler_flags
rm $1/PERF*
FILE=$1/config
while read line; do
    initial="$(echo $line | head -c 1)"
    if [ "$initial" = "P" ] ;
    then
        tag=`echo $line | sed 's/ .*//'`
        echo $tag
        gcc ./rbm_main.c -I./$1 -I. $2 -o $1/measure -D$tag
        printf "" > $1/$tag

        for i in "${array[@]}"
        do
            echo $i
            ./$1/measure $i >> $1/$tag
            Rscript ./plot_benchmarks.R $1 2> /dev/null
        done
    fi
done < $FILE
