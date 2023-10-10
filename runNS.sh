#! /usr/bin/bash

inputfiles=()
outputfile="a.out"

args=()

for var in "$@"
do
	args+=($var)
done

count=0
for var in "$@"
do 
	count=$((count+1))
	if [[ "$var" == "-o" ]]; then
		break;
	fi
	inputfiles+="$var "
done

outputfile=${args[$count]}

dir=$(dirname -- "$0")

arm-linux-gnueabi-gcc ${inputfiles[@]} -o $outputfile -nostartfiles -nostdlib -nolibc -nodefaultlibs -mcpu=arm7tdmi -T ${dir}/linkS.lds -L${dir}/Libs

