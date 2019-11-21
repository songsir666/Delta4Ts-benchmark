#!/bin/bash

#mv ../outputs/* ../outputs.alt/
#mv ../traces/* ../traces.alt/
>out.list
failed=0;
passed=0;
sum=4130;  #测试用例总数

for ((i=1;i<=$sum;i++))
do
	f=t"$i"
	diff ../outputs/"$f" ../outputs.alt/"$f" >/dev/null 2>&1
	if [ $? == 1 ]
	then echo ${f#*t}	1 >> out.list
		failed=`expr $failed + 1`
	else 
		echo ${f#*t}	0 >> out.list
		passed=`expr $passed + 1`
	fi
done

#约定在out.list末尾两行为失败成功统计值
echo $failed >> out.list
echo $passed >> out.list
