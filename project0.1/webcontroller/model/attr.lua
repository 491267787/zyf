require("luci.sys")
require("luci.sys.zoneinfo")
require("luci.tools.webadmin")
require("nixio.fs")
require("luci.config")

local m, s, o

m = Map("attr", translate("attribution"), translate("This is design by ZYF, in order to control  board."))
m:chain("luci")

s = m:section(TypedSection, "controlboard", translate("Control Board"))
s.anonymous = true
s.addremove = false


s:tab("mode", translate("WIFI MODE"))
s:tab("ap", translate("AP CONFIG"))
s:tab("sta", translate("STA CONFIG"))
s:tab("pppoe", translate("PPPOE"))
s:tab("usr", translate("USR"))
s:tab("led", translate("LED"))

--
-- mode
--
o = s:taboption("mode", ListValue, "mode", translate("WIFI MODE:"))
o.default = 1
o.datatype = "uinteger"
o:value(0, translate("DISABLE"))
o:value(1, translate("AP"))
o:value(2, translate("STA"))


--
-- ap config
--
o = s:taboption("ap", Value, "apaccount", translate("AP ACCOUNT:"))
o = s:taboption("ap", Value, "appassword", translate("AP PASSWORD:"))
o.password=true;

--
-- sta config
--
o = s:taboption("sta", Value, "staaccount", translate("STA ACCOUNT:"))
o = s:taboption("sta", Value, "stapassword", translate("STA PASSWORD:"))
o.password=true;

--
-- pppoe config
--
o = s:taboption("pppoe", Value, "pppoeaccount", translate("PPPOE ACCOUNT:"))
o = s:taboption("pppoe", Value, "pppoepassword", translate("PPPOE PASSWORD:"))
o.password=true;

--
-- usr config
--
o = s:taboption("usr", Value, "usrname", translate("USR NAME:"))
o = s:taboption("usr", Value, "usrpassword", translate("USR PASSWORD:"))
o.password=true;


--
-- led
--
o = s:taboption("led", ListValue, "ledname", translate("LED NUM:"))
--o.datatype = "char"
o:value("1", translate("LED1"))
o:value("2", translate("LED2"))
o:value("3", translate("LED3"))

o = s:taboption("led", ListValue, "ledstatus", translate("LED STATUS:"))
o:value("on", translate("LED ON"))
o:value("off", translate("LED OFF"))


local apply = luci.http.formvalue("cbi.apply")
if apply then
    io.popen("/etc/init.d/attr restart")
end


return m
