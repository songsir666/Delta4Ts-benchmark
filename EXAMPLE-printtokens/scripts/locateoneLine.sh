#!/bin/bash
# $1 fault list
# $2 size of fault list
# $3 formula name
# $4 the file of the fault line
declare min=99999999;
firstfault=""
faultstring=`echo $1 | tr '_' ' '`
for faultnum in `echo $faultstring`
do
	for formula in `ls ../results/fault_"$2"/$3formula.fault.list`
	do
		faultname=`grep -w ^$faultnum $4 | cut -d " " -f 2 `
		for faultnameline in $faultname
		do
			faultnameline=`expr $faultnameline + 4 `
			susp=`grep -n -w ^$faultnameline  $formula |head -1 | cut -d ":" -f 2|cut -f 3`
			min_E=`cat $formula | cut -f 3| grep -n -w ^$susp$| head -1 | cut -d ":" -f -1`
			MAX_E=`cat $formula | cut -f 3| grep -n -w ^$susp$| tail -1 | cut -d ":" -f -1`
			#echo ^$faultnameline  $formula
			Atline=$min_E
			#Atline=`grep -n -w ^$faultnameline  $formula |head -1 | cut -d ":" -f 1`
			#Atline=`grep -n -w ^$faultnameline  $formula|head -1 | cut -d ":" -f 1 `  #错误在rank.list行号
			if [ -n $min_E ];then
				Atline=`echo  $((($min_E+($MAX_E-$min_E)/2)))`
				if [ $Atline -lt $min ];then
					min=$Atline
					firstfault=$faultnum
				fi
			fi
			#echo $Atline $min $firstfault
		done	
			
	done
done
if [ -n $Atline ];then
	echo $firstfault:$min
else
	echo $firstfault:99999999
fi
