#!/bin/bash

MYPATH=$PWD/`dirname $0`
CURPATH=$PWD
cd $MYPATH

ARGUMENTS=
index="0";

while [ "$#" != "0" ]; do
	ARG=$1
	if (( $index % 2 == 1 )); then
		ARG=$CURPATH/"$1"
	fi
	index=$(( $index + 1 ))
	ARGUMENTS=$ARGUMENTS" $ARG"
	shift
done

export LD_LIBRARY_PATH=LD_LIBRARY_PATH:$MYPATH/../GLFrontEnd/lib/:$MYPATH/../Serialization/lib
exec $MYPATH/./landscape $ARGUMENTS
