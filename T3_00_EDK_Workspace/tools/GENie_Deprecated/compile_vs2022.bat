@ECHO off
cls
.\GENie\genie.exe vs2022

xcopy /s /d /Y .\..\assets .\..\bin\debug\x32
xcopy /s /d /Y .\..\assets .\..\bin\release\x32

xcopy /s /d /Y .\..\assets .\..\build\PR03_00_Template\vs2022
xcopy /s /d /Y .\..\assets .\..\build\PR03_01_Demo\vs2022
xcopy /s /d /Y .\..\assets .\..\build\PR03_02_FirstScene\vs2022
xcopy /s /d /Y .\..\assets .\..\build\PR03_03_SolarSystem\vs2022
xcopy /s /d /Y .\..\assets .\..\build\PR03_05_BasicMaterial\vs2022
xcopy /s /d /Y .\..\assets .\..\build\PR03_06_CustomMaterial\vs2022
pause
