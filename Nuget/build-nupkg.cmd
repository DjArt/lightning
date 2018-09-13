@echo off
setlocal enableextensions disabledelayedexpansion
set NUSPEC="Microsoft.IoT.Lightning.nuspec"
set NUSPEC_WIN32="Microsoft.IoT.Lightning.Win32.nuspec"

:: Parse options
:GETOPTS
 if /I "%~1" == "/?" goto USAGE
 if /I "%~1" == "/Help" goto USAGE
 if /I "%~1" == "/clean" set CLEAN=1
 if /I "%~1" == "/nopack" set NOPACK=1
 if /I "%~1" == "/debug" set NUSPEC="Microsoft.IoT.Lightning.Debug.nuspec" && NUSPEC_WIN32="Microsoft.IoT.Lightning.Win32.Debug.nuspec" 
 if /I "%~1" == "/public" set NUSPEC="Microsoft.IoT.Lightning.Public.nuspec" && NUSPEC_WIN32="Microsoft.IoT.Lightning.Win32.Public.nuspec"
 shift
if not (%1)==() goto GETOPTS

echo Cleaning outputs
del Microsoft.IoT.Lightning*.nupkg 2> NUL

:: if a clean was requested, exit here
if "%CLEAN%"=="1" goto end

:: skip packaging step if requested
if "%NOPACK%"=="1" goto end

echo Creating NuGet Package
nuget help > NUL
IF ERRORLEVEL 1 (
    echo Please install nuget.exe from http://nuget.org
    goto err
)
nuget pack %NUSPEC% -NoPackageAnalysis || goto err
nuget pack %NUSPEC_WIN32% -NoPackageAnalysis || goto err
:end

echo Success
exit /b 0

:err
  echo Script failed!
  exit /b 1