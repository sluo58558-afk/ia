@echo off
echo Compiling Flyff Bot Loader...

if not exist "bin\Win32\Release" mkdir "bin\Win32\Release"

cl /EHsc /W4 /DUNICODE /D_UNICODE /MT /link /SUBSYSTEM:WINDOWS /ENTRY:WinMain /OUT:bin\Win32\Release\RIMJOB.exe ^
    src\main.cpp ^
    src\res\Resource.rc ^
    comctl32.lib

if %ERRORLEVEL% EQU 0 (
    echo Build successful!
) else (
    echo Build failed!
    pause
)
