#!/bin/sh

array=( 2 3 5 10 20 30 40 50 70 120 160 )

echo $2 > $1/compiler_flags
FILE=$1/config
while read line; do
    initial="$(echo $line | head -c 1)"
    if [ "$initial" = "P" ] ;
    then
        tag=`echo $line | sed 's/ .*//'`
        echo $tag
        gcc ./rbm_main.c -I./$1 -I. $2 -o $1/measure -D$tag
        c=5

        for i in "${array[@]}"
        do
            echo $i
            ./$1/measure $i >> $1/more_$tag
        done
    fi
done < $FILE

cat $1/$tag >> $1/more_$tag
printf "" > $1/$tag
cat $1/more_$tag >> $1/$tag
rm $1/more*
Rscript plot_benchmarks.R $1