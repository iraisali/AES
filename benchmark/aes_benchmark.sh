#!/bin/bash

MODE=$1
KEY=$2
FILE=$3
N=$4

for (( i=0; i<=${N}; i++  ))
do
    ./AES -m $1 -k $2 $3
    #cat /proc/sys/kernel/random/entropy_avail
done

