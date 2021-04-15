@echo off
echo stop DSServer...
sc stop MvDSServer
@ping 127.0.0.1 -n 3 >nul

echo delete DSServer...
sc delete MvDSServer

@ping 127.0.0.1 -n 3 >nul

set base_dir=%~dp0
%base_dir:~0,2%
pushd %base_dir%
echo %cd%
