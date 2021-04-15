@echo off
set target=mv_digicap.dav
echo 局域网内支持Gige的设备IP。
gigetool -l
set /p ip=请输入要升级的设备ip: 
hik_upg_cli %ip% %target%
echo 按回车退出程序。
pause>nul