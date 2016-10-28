#!/bin/bash

LINES=`cat $1`

for LINE in $LINES
do
	x=`echo $LINE | cut -d';' -f1`
	y=`echo $LINE | cut -d';' -f2`
	reste=`echo $LINE | cut -d';' -f3-4`
	y=$(($y + $2))
	echo "$x;$y;$reste"
done
