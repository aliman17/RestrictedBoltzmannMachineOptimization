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
        gcc ./rbm_main.c -I./$1 -I. $2 -o $1/measure -D$tag
<<<<<<< HEAD
=======
        #gcc -std=c99 -Wa,-q ./rbm_main.c -I./$1 -I. $2 -o $1/measure -D$tag
>>>>>>> d67faa253c49f7fd9f4a2ccfb01c829b5750e37c
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
