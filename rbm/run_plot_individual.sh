#!/bin/sh

echo $2 > $1/compiler_flags
rm $1/more*
FILE=$1/config
while read line; do
    initial="$(echo $line | head -c 1)"
    if [ "$initial" = "P" ] ;
    then
        tag=`echo $line | sed 's/ .*//'`
        echo $tag
        gcc ./rbm_main.c -I./$1 -I. $2 -o $1/measure -D$tag
        c=5
        printf "" > $1/$tag
        while ((c<=200))
        do
            echo $c
            ./$1/measure $c >> $1/more_$tag
            c=$(($c + 30))
            done
    fi
done < $FILE
