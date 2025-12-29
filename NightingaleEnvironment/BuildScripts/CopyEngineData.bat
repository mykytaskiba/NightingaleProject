@echo off

REM arg1 = SolutionDir (from visual studio)
REM arg2 = ProjectName (from visual studio)

set SolutionDir=%1
set ProjectName=%2

set EngineDataPath=%SolutionDir%\NightingaleEngine\data
set DestinationPath=%SolutionDir%\%ProjectName%\data

echo Solution Dir: %SolutionDir%
echo Project Name: %ProjectName%
echo Copying From: %EngineDataPath%
echo Copying Into: %DestinationPath%

xcopy %EngineDataPath% %DestinationPath% /E /I /Y 
pause