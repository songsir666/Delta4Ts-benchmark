#!/bin/bash

echo $*
historyversion=$1
shift
echo $*
faultlist="$*"
Result=${faultlist// /_}
rm $Result.cvs
rm -r  ../results/fault_$Result 
mkdir ../results/fault_$Result  


	./run.sh $faultlist 									# run test to get coverage information
 	./clocate.sh $historyversion "$Result"						        # use formula to generate rank list

 


