@echo off

if exist %Systemroot%\SysWOW64 (
    regsvr32 /s "%ProgramFiles(x86)%\\Common Files\\MVS\\Runtime\\Win32_i86\\MvDSS.ax"
) else (
    regsvr32 /s "%ProgramFiles%\\Common Files\\MVS\\Runtime\\Win32_i86\\MvDSS.ax"
)

echo MvDSS register ok
@ping 127.0.0.1 -n 3 >nul
