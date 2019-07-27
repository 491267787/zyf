#!/bin/sh



grep pppoe /etc/config/network > tmp.txt
if [ -s "./tmp.txt" ];then
	echo "ok"

	uci set /etc/config/network.wan.username=$1
	uci set /etc/config/network.wan.password=$2

	uci commit /etc/config/network


	chmod 755 /etc/init.d/network
	/etc/init.d/network restart
	sleep 5 
	./clientOP0.1 restart
else
	echo "no pppoe"
#	./clientOP0.1 restart

fi


