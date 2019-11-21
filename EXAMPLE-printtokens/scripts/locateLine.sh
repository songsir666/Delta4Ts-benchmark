#!/bin/bash
#echo "hello world!!"
dir=`ls -l ../results/ | awk '/^d/{print $NF}' `
#echo "dir $dir"
for subdir in $dir
do
#	echo "subdir$subdir"
	faultstring=`echo ${subdir#*fault_}|tr '_' ' '|tr -s ' '`
#	echo "faultstring$faultstring"
	for faultnum in `echo $faultstring`
	do
#		echo "faultnum$faultnum"
		for formula in `ls ../results/$subdir/*_*formula.fault.list`
		do
#			echo "for_formula"
			faultname=`grep -w ^$faultnum $1 | cut -d " " -f 2 `
#			echo "after for_formula"
#			#faultname=` sed -ne ${faultnum}p $2 ` #获得错误在faultseeds.h里面行号
			#Atline=`grep -n -w $faultname  $formula|head -1 | cut -d ":" -f 1 `  #错误在rank.list行号
			declare min=99999999;
			for faultnameline in $faultname
			do
				susp=`grep -n -w ^$faultnameline  $formula |head -1 | cut -d ":" -f 2|cut -f 3`
				#min_E=`cat $formula | cut -f 3| grep -n -w ^$susp$| head -1 | cut -d ":" -f -1`
				#Num=`cat $formula | cut -f 3| grep -c ^$susp$`
				#Atline=`echo  $((($Num+$min_E-1)/2))`
				#echo $Atline
				Atline=`cat $formula | cut -f 3| grep -n -w ^$susp$| head -1 | cut -d ":" -f -1`
				#Atline=`grep -n -w ^$faultnameline  $formula|head -1 | cut -d ":" -f 1 `  #错误在rank.list行号
				if [ $Atline -lt $min ];then
					min=$Atline
				fi
			
			done	
			subname=` basename $formula|sed -ne "s/formula.fault.list//p" `
#			echo $subname
			fault_list=`echo $subname|tr '_' ' '`
#			echo $fault_list
			echo "$subdir $fault_list $faultnum $min"
			
		done
	done
done
