:: 示例程序编译脚本
@echo off
title SaveImage

:: 确保当前目录为脚本所在目录，若不添加，则在系统盘会有错误
set base_dir=%~dp0
%base_dir:~0,2%
pushd %base_dir% 1>nul 2>&1

:: 检查 java 环境
call ..\Library\JavaEnv.bat || goto:END

set OsArch=x64

java -classpath ".\bin;..\Library\%OsArch%\MvCameraControlWrapper.jar;%CLASSPATH%" SaveImage

:END

popd
pause
