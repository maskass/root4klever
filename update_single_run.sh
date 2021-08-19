#!/bin/bash
if [ $# -ne 1 ];then
    echo "ERROR: use 1 argument (run number)"
    exit 1
fi

nrun=$( printf "%6.6d" "$1")

ascii_dir="$HOME/LOCAL_ASCII"
root_dir="$HOME/LOCAL_ROOT"
root_merged_dir="/eos/user/m/mmoulson/KLEVER/TestBeamData/Root2021/merged"

# for every spill of a given run, check the root dir and
# create the root file if it does not exist

new_file_count=0;

for file in $(ls $ascii_dir"/run"$nrun"_wave_"*.dat); do
    nspill=$(echo $file | awk '{c=split($1,a,"/");split(a[c],b,".");dd=split(b[1],d,"_");print d[dd]+0}')

    root_file=$(awk -v run=$nrun -v spill=$nspill 'BEGIN{printf("run%6.6d_%6.6d.root",run,spill)}')

    
    # check file existence
    if [ ! -e $root_dir/$root_file ];then
	new_file_count=$(echo $new_file_count + 1 | bc)
	root -l -b -q 'ascii2root.cc+('\"$file\"','\"$root_dir/$root_file\"')'
    fi
    
done
echo $new_file_count
root_merged_file=$(awk -v run=$nrun 'BEGIN{printf("run%6.6d.root",run)}')

if [ $new_file_count -gt 0 ];then
    echo "creating new merged file..."
    hadd -f $root_merged_dir/$root_merged_file $root_dir/run$nrun*.root
    echo " "
    echo "... done."
fi
