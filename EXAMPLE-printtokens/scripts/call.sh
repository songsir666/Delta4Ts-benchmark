

for((i=$1;i<=$2;i++))
do
	./run.sh $i > /dev/null
	echo running $i--
done
