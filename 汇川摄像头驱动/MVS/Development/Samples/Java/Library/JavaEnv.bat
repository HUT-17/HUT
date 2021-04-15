:: 检查java安装环境

@echo off

:: 设置全局错误码
set /A errorcode=0

:: 开启扩展功能
setlocal ENABLEEXTENSIONS
setlocal ENABLEDELAYEDEXPANSION

::通过向临时文件写内容，判断是否有文件读写权限
echo > tmp
if exist tmp (
    del /F/Q tmp >nul 2>&1
) else (
    echo Please switch to an administrator account to run this batch!!!
    set /A errorcode=1
    goto:END
)

::判断JAVA_HOME是否被定义
if "%JAVA_HOME%"=="" (
    echo JAVA_HOME not set. Please make sure that java is correctly installed.
    set /A errorcode=2
    goto:END
)

:: 判断CLASSPATH是否被定义
if "%CLASSPATH%"=="" (
    echo CLASSPATH not set. Please make sure that java is correctly installed.
    set /A errorcode=3
    goto:END
)

:: 判断java版本是否比 1.7.0高，需先将版本信息重定向到文件中，再对文件内容进行分析
java -version >nul 2> JavaVer.tmp
for /F "tokens=1,2,3*" %%i in (JavaVer.tmp) do (
    if "%%j" == "version" (
        if %%k LSS "1.7.0" (
            echo Java version is less than "1.7.0", warnings or errors may occur.
        )
    )
)
del /F/Q JavaVer.tmp >nul 2>&1

:END

::返回错误码
exit /B %errorcode%
