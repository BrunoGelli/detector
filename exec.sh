#!/bin/bash



rm FiltroOutPut.txt

for ((i=25;i<=20000;i=i+25)); do

    echo "Running for $(printf '%d' $i) SiPMs"
    ./filtro filtro.in $(printf '%d' $i) >/dev/null

done

rm *.root 

