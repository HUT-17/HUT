@echo off

if exist %Systemroot%\SysWOW64 (
    regsvr32 /u /s "%ProgramFiles(x86)%\\Common Files\\MVS\\Runtime\\Win32_i86\\MvDSS.ax
) else (
    regsvr32 /u /s "%ProgramFiles%\\Common Files\\MVS\\Runtime\\Win32_i86\\MvDSS.ax
)

echo MvDSS unregister ok
@ping 127.0.0.1 -n 3 >nul
