#! /bin/bash
CDIR=`pwd`
PG=`dirname $CDIR`
NAME=`basename $PG`
rm $NAME.results
for results in `ls ../results/`
do 
	awk '$0="'$results'\t"$0' ../results/$results/${results#*fault_}.cvs >> $NAME.results
done

