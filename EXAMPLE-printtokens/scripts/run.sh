#!/bin/bash
#/scripts/
#自动运行所有测试操作的脚本
#输入两个参数：$1-测试版本  

#./../../mts/bin/bsh/mts .. ../source/grep.exe ../testplans.alt/v1/v0_2.tsl.universe R 1.sh  NULL NULL
rm ../source/* ../outputs/* ../outputs.alt/* ../traces/* ../traces.alt/* >/dev/null 2>&1
#./scripts/mts.sh  自行修改mts并运行
#./gcov.sh $1
#echo 生成测试用例

echo $#
versionnum=`echo _$#`

cp ../versions.alt/versions.seeds/* ../source/
echo 复制错误版本v"$1"
version_out_document=""
for arg in "$@"
do
	version_out_document=$version_out_document'_'$arg
	echo $arg
	flag1=$arg
	flag2="s/^\/\*//"
	flag3="s/\*\/$//"
	flag=$flag1$flag2
	flag0=$flag1$flag3
	echo $flag
	sed -i -e $flag ../source/FaultSeeds.h
	sed -i -e $flag0 ../source/FaultSeeds.h

done
#echo $version_out_document
cd ../source
make clear
make all
#gcc -fprofile-arcs -ftest-coverage -I ../source/ -o ../source/totinfo.exe ../source/schedule.c >/dev/null 2>&1
echo 编译错误版本
#cd ../scripts/
#cd scripts/
echo 正在运行测试用例...
../scripts/caserun.sh  #>/dev/null 2>&1
mv ../outputs/* ../outputs.alt/
mv ../traces/* ../traces.alt/

cd ../scripts
echo 错误版本完成

echo #************************************
rm ../source/*
cp ../versions.alt/versions.seeds/* ../source/
echo 复制正确版本v"$1"
cd ../source/
make clear
make all

#gcc -fprofile-arcs -ftest-coverage -I ../source/ -o ../source/totinfo.exe ../source/schedule.c >/dev/null 2>&1
echo 编译正确版本
echo 正在运行测试用例...
../scripts/caserun.sh   #>/dev/null 2>&1
echo 正确版本完成
#rm printtokens.gcno
cd ../scripts
echo 
echo #***********************************
./cmp.sh
echo 比较完成！out.list

./clocate_thread.sh  $version_out_document
#./clocate_thread.sh $versionnum

#rm ../source/* ../outputs/* ../outputs.alt/* ../traces/* ../traces.alt/* >/dev/null 2>&1
