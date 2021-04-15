:: 运行java程序
@echo off
title ConvertPixelType

:: 确保当前目录为脚本所在目录，若不添加，则在系统盘会有错误
set base_dir=%~dp0
%base_dir:~0,2%
pushd %base_dir% 1>nul 2>&1

:: 检查 java 环境
call ..\Library\JavaEnv.bat || goto:END

set OsArch=x64

java -classpath "%CLASSPATH%.\bin;..\Library\%OsArch%\MvCameraControlWrapper.jar;" ConvertPixelType

:END

popd
pause
