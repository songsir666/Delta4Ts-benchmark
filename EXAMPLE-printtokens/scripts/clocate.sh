#!/bin/bash
#利用得出的out.list找出failed用例分析对应的.gcov文件
#利用thread.c,运行clocate.sh调用c程序并传入参数，分别是源代码总行数、失败用例数、成功测试用例数


#初始化两数组
#linenum=`grep -c "" ../traces.alt/1.gcov`
linenum=615
version=$1
#获得成功失败统计值
passnum=`grep -c "" ../results/fault_"$1"/out.list`
failnum=`expr $passnum - 1`
passnum=`sed -n "$passnum"p ../results/fault_"$1"/out.list`
failnum=`sed -n "$failnum"p ../results/fault_"$1"/out.list`

#echo $linenum  $failnum $passnum $version
./caculate $linenum $failnum $passnum $version
echo 定位已完成！
#rm -r ../results/fault_"$1"/FaulTamer_*formula.fault.list
rm -r ../results/fault_"$1"/peer_*formula.fault.list
#mkdir ../results/fault"$2"
for name in `ls -a| grep .list`
do
	
	mv $name ../results/fault_"$1"/$name
done
#mv out.list ../results/fault_"$2"/out.list
#mv temp ../results/fault_"$2"/temp

