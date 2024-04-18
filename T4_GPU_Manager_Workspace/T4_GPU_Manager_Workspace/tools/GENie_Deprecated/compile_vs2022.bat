@ECHO off
cls
.\GENie\genie.exe vs2022

xcopy /s /d /Y .\..\assets .\..\bin\debug\x32
xcopy /s /d /Y .\..\assets .\..\bin\release\x32

xcopy /s /d /Y .\..\assets .\..\build\PR04_00_GPUManager_Template\vs2022

pause
