#!/bin/bash

# argument checking

test -f $1 || { echo "$1 is invalid" && exit; }
test -f $2 || { echo "$2 is invalid" && exit; }
[ $3 -gt 0 ] || { echo "$3 is invalid" && exit; }
[ $4 -eq 0 ] || [ $4 -eq 1 ] || { echo "$4 is invalid" && exit; }

# virus file
vF=$1

# countries file
cF=$2

# number of output lines
numLines=$3

# duplicates allowed
duplicates=$4

#touch inputFile.txt

i=0
while [ $i -lt $numLines ]; do
	citizenID=$((RANDOM%10000))
	
	fnameLength=$((RANDOM%12+3))

	fname=$(tr -dc A-Z < /dev/urandom | head -c ${fnameLength})

	lnameLength=$((RANDOM%12+3))

	lname=$(tr -dc A-Z < /dev/urandom | head -c ${lnameLength})

	echo "$citizenID $fname $lname"

	let i+=1;	
done

