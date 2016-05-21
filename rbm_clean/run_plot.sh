#!/bin/sh

echo $2 > $1/compiler_flags
rm $1/PERF*
FILE=$1/config
while read line; do
    initial="$(echo $line | head -c 1)"
    if [ "$initial" = "P" ] ;
    then
        tag=`echo $line | sed 's/ .*//'`
        echo $tag
        gcc-5 ./rbm_main.c -I./$1 -I. $2 -o $1/measure -D$tag
        c=200
        printf "" > $1/$tag
        while ((c<=2000))
        do
            echo $c
            ./$1/measure $c >> $1/$tag
            c=$(($c + 100))
            done
    fi
done < $FILE
