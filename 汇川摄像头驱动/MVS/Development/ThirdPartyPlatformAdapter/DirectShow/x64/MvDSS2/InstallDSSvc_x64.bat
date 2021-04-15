@echo off
set base_dir=%~dp0
%base_dir:~0,2%
pushd %base_dir%
echo %cd%

echo install DSServer x64...
"%ProgramFiles(x86)%\\Common Files\\MVS\\Service\\DirectShow\\x64\\MvDSServer_x64.exe" install

pushd %systemroot%\system32
echo %cd%

echo start DSServer x64...
sc start MvDSServer_x64

@ping 127.0.0.1 -n 3 >nul
