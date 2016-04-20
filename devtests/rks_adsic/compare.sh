#!/bin/bash

refen=-1.181951039128841
lsden=`grep 'ENERGY| ' lsd/test.out | awk '{print $9}'`
rksen=`grep 'ENERGY| ' rks/test.out | awk '{print $9}'`

result=`echo "scale=5; $lsden - $refen" | bc`

if [[ "$result" > "0.00001" ]]; then
    exit 1;
else
    echo "reference lsd is fine"

    result=`echo "scale=5; $lsden - $rksen" | bc`

    if [[ "$result" > "0.00001" ]]; then
        exit 1;
    else
        echo "everything is fine"
        exit 0;
    fi
fi
