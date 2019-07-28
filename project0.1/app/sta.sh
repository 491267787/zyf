#!/bin/sh


uci set /etc/config/wireless.@wifi-iface[0].disabled=0
uci commit /etc/config/wireless
/etc/init.d/network restart
sleep 5

iw dev wlan0 scan | grep -B 8 $1 > tmp.txt
#grep -B 8 $1 ./a > tmp.txt
if [ -s "./tmp.txt" ];then
	echo "ok"
	bssid=$(cut -b 5- ./tmp.txt | cut -b -17 | head -n 1)

#	uci set /etc/config/wireless.@wifi-iface[0].disabled=0
	uci set /etc/config/wireless.@wifi-iface[1].disabled=1

	uci set /etc/config/wireless.@wifi-iface[0].ssid=$1
	uci set /etc/config/wireless.@wifi-iface[0].key=$2
	uci set /etc/config/wireless.@wifi-iface[0].bssid=${bssid}

	uci commit /etc/config/wireless


	chmod 755 /etc/init.d/network
	/etc/init.d/network restart
	sleep 5 
	./clientOP0.1 restart
else
	echo "fail"
	./clientOP0.1 restart

fi


