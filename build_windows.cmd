@echo off

set GCC_BIN=
where /q g++.exe
if %ERRORLEVEL%==0 (
	set GCC_BIN=g++
)
if "%GCC_BIN%"=="" (
    echo "Cannot find g++ binary."
    goto END
)
set CFLAGS="-std=c++17"

set BUILD_FLAGS="-O3"

%GCC_BIN% %CFLAGS% %BUILD_FLAGS% -o pikachu.exe src/main.h src/main.cpp
if %ERRORLEVEL%==0 (
	echo "Done!"
)
:ENDs
pause