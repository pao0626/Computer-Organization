#!/usr/bin/bash

Policy=self
>output"$Policy".txt
make $Policy
for block in $(seq 3 6)
do
    for set in $(seq 0 $((6-$block)))
    do
        way=$((6-$set-$block))
        
        #change config and print config
        printf "[cache]\nSet = $((2**$set))\nWay = $((2**$way))\nBlockSize = $((2**$block))\nPolicy = \"$Policy\"" > config.conf
#        echo $set $way $block        
        #make score
        make test 2>&1 | grep -A 3 ==== >> output"$Policy".txt
    done
done