@echo off

if exist %Systemroot%\SysWOW64 (
    regsvr32 /u /s "%ProgramFiles(x86)%\\Common Files\\MVS\\Runtime\\Win64_x64\\MvDSS.ax"
) else (
    echo System not support...
)

echo MvDSS unregister ok
@ping 127.0.0.1 -n 3 >nul
