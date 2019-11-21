#!/bin/bash
#利用得出的out.list找出failed用例分析对应的.gcov文件
#利用thread.c,运行clocate.sh调用c程序并传入参数，分别是源代码总行数、失败用例数、成功测试用例数


#初始化两数组
linenum=`grep -c "" ../traces.alt/1.gcov`

#获得成功失败统计值
passnum=`grep -c "" out.list`
failnum=`expr $passnum - 1`
passnum=`sed -n "$passnum"p out.list`
failnum=`sed -n "$failnum"p out.list`

echo $passnum $failnum $linenum
./thread.exe $linenum $failnum $passnum > temp
echo 定位已完成！
rm -rf ../results/fault"$1"
mkdir ../results/fault"$1"
for name in `ls -a| grep .list`
do
	
	mv $name ../results/fault"$1"/$name
done
mv out.list ../results/fault"$1"/out.list
mv temp ../results/fault"$1"/temp

