module("luci.controller.admin.yanyitong",package.seeall)

function index()
--	entry({"admin","yanyitong"},alias("admin","yanyitong","yanyitong"),_("YYT"),30).index = true

	entry({"admin","yanyitong"},cbi("admin_yanyitong/attr"),_("YYT"),70)

end
