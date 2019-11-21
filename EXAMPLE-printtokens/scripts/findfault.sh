#cat v1/fault.list | grep -n  e107:

a=(
0
50
124
63
46
58
27
97
)

for((i=1;i<=7;i++))
do
	echo `cat ../versions.alt/versions.orig/v"$i"/fault.list | grep -n  e${a[$i]}:`
done
