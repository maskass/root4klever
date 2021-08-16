#!/bin/bash
#if [ $# -ne 2 ];then
#    echo "ERROR: use 2 arguments (ascii dir [ascii_dir], root dir [root_dir])"
#    exit 1
#fi

ascii_dir="./ascii"
root_dir="./root"

if [ ! -d $ascii_dir ];then
    echo "ERROR: directory "$ascii_dir" not present."
    exit 1
fi
if [ ! -d $root_dir ];then
    echo "ERROR: directory "$root_dir" not present."
    exit 1
fi

sleepseconds=20
while [ 1 ] ; do

    last_run=$(ls -1t $ascii_dir/ | head -n 1 |  awk '{c=split($1,a,"/");split(a[c],b,".");split(b[1],d,"_");split(d[1],e,"run");printf("%d",e[2])}')
    echo "updating run "$last_run
    ./update_single_run.sh $last_run
    echo "sleeping "$sleepseconds" seconds"
    sleep $sleepseconds
done


    
