#!/bin/sh 

grep -B liba ./a > /dev/null
if [ $? -eq 0 ];then
	echo "Found"
else
	echo "Not found"
fi 
