@echo off

IF (%1)==() GOTO error
dir /b /ad %1 >nul 2>nul && GOTO indentDir
IF NOT EXIST %1 GOTO error
goto indentFile

:indentDir
set searchdir=%1

IF (%2)==() GOTO assignDefaultSuffix
set filesuffix=%2

GOTO run

:assignDefaultSuffix
::echo !!!!DEFAULT SUFFIX!!!
set filesuffix=*

:run
FOR /F "tokens=*" %%G IN ('DIR /B /S %searchdir%\*.h%') DO (
echo Indenting file "%%G"
"uncrustify.exe" -f "%%G" -c "uncrustify.cfg" -o indentoutput.tmp
move /Y indentoutput.tmp "%%G"

)
FOR /F "tokens=*" %%G IN ('DIR /B /S %searchdir%\*.c%') DO (
echo Indenting file "%%G"
"uncrustify.exe" -f "%%G" -c "uncrustify.cfg" -o indentoutput.tmp
move /Y indentoutput.tmp "%%G"

)
FOR /F "tokens=*" %%G IN ('DIR /B /S %searchdir%\*.cpp%') DO (
echo Indenting file "%%G"
"uncrustify.exe" -f "%%G" -c "uncrustify.cfg" -o indentoutput.tmp
move /Y indentoutput.tmp "%%G"

)
GOTO ende

:indentFile
echo Indenting one file %1
"uncrustify.exe" -f "%1" -c "uncrustify.cfg" -o indentoutput.tmp
move /Y indentoutput.tmp "%1"


GOTO ende

:error
echo .
echo ERROR: As parameter given directory or file does not exist!
echo Syntax is: teste.bat dirname filesuffix
echo Syntax is: teste.bat filename
echo Example: teste.bat temp cpp
echo .

:ende
