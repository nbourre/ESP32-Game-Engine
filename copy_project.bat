@echo off
echo Copying ESP32 Game Engine to Arduino libraries...

:: Define source and destination paths
set SRC=%cd%
set DEST=%userprofile%\Documents\Arduino\libraries\ESP32 Game Engine

:: Ensure destination exists
if not exist "%DEST%" mkdir "%DEST%"

:: Copy all files while preserving structure
xcopy "%SRC%\*" "%DEST%\" /E /H /Y

echo Done!
exit
