#!/bin/sh


uci set /etc/config/loogson.@controlboard[0].lednum=$1
uci set /etc/config/loogson.@controlboard[0].ledstatus=$2
uci commit /etc/config/loogson
chmod 777 etc/init.d/loogson 
/etc/init.d/loogson restart

