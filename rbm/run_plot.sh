#!/bin/sh

echo $2 > compiler_flags

FILE=$1/config
while read line; do
    initial="$(echo $line | head -c 1)"
    if [ "$initial" = "P" ] ;
    then
        tag=`echo $line | sed 's/ .*//'`
        echo $tag
        gcc ./rbm_main.c -I./$1 -I. $2 -o $1/measure -D$tag
        c=100
        while ((c<=800))
        do
            echo $c
            ./$1/measure $c >> $1/$tag
            c=$(($c + 100))
            done
    fi
done < $FILE
