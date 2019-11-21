#!/bin/bash
flag=0
count=0
#echo $1
#参数$1表示低第几本

#if [ "$1" != "" ]
#then
	> caserun.sh
#else
#	echo “请输入版本参数”
#	exit 1
#fi

while read line
do
#   echo $line     #这里可根据实际用途变化
	if [[ "$line" =~ ">>>>>>>>running test" &&  "$flag" == 0 ]]
		then flag=1
#		count=${line//[A-Za-z\>]/}
		count=`expr $count + 1`
		echo $line >> caserun.sh
	elif [ "$flag" = 1 ]
		then flag=0
		echo $line >> caserun.sh
		echo 'gcov ../source/tcas.c >/dev/null 2>&1'>> caserun.sh		#生成gcov文件
		echo 'mv tcas.c.gcov ../traces/'$count.gcov >> caserun.sh		#重命名gcov
		echo 'rm tcas.gcda'  >> caserun.sh		#删除gcda、gcov
		echo  >> caserun.sh		#打印空行
#	else
#		echo $line >> caserun.sh
	fi
done < runall.sh
chmod +x caserun.sh
