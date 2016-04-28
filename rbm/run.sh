#!/bin/bash
c=100

while (( c <= 1 ))
do
    echo "Training Phase"
    python3 captionIt.py --train
    echo "development Phase"
    python3 captionIt.py --develop	
done
