#!/bin/sh


uci set /etc/config/wireless.@wifi-iface[0].disabled=1
uci set /etc/config/wireless.@wifi-iface[1].disabled=1
uci commit /etc/config/wireless


chmod 755 /etc/init.d/network
/etc/init.d/network restart
sleep 5
/app/clientOP0.1 restart
