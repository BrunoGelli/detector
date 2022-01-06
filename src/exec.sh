#!/bin/bash



rm FiltroOutPut.txt

for ((i=10;i<=4000;i=i+5)); do

    echo "Running for $(printf '%d' $i) SiPMs"
    ./filtro filtro.in $(printf '%d' $i) >/dev/null

done
