#!/bin/bash

LINES=`cat $1`

for LINE in $LINES
do
	x=`echo $LINE | cut -d';' -f1`
	reste=`echo $LINE | cut -d';' -f2-4`
	x=$(($x + $2))
	echo "$x;$reste"
done
