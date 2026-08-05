@echo off

rem >nul 2>nul suppresses terminal output
where gcc >nul 2>nul

rem the "where command" sets the errorlevel to 0 if found, 1 if not found
if %errorlevel% equ 1 (
    echo ERROR: gcc not found in system PATH
    echo Please install MSYS2 or MinGW
    exit /b 1
)

rem check for pdcurses install in compiler include path
for %%G in (
    "C:\msys64\ucrt64\include\pdcurses\curses.h"
    "C:\msys64\mingw64\include\pdcurses\curses.h"
    "C:\MinGW\include\curses.h"
    "%~dp0include\curses.h"
) do (
    if exist "%%~G" (
        rem %%~dpG gets the folder of the file
        set "FOUND_PATH=%%~dpG"
        goto :found
    )
)

rem exit if pdcurses not found
echo ERROR: PDCurses was not found in any normal locations
echo Please install pdcurses
exit /b 1

:found

rem build snake.c using gcc
gcc snake.c -g -Wall -Wextra -I%FOUND_PATH% -lpdcurses -o snake.exe

if %errorlevel% equ 1 (
    echo compilation failed
) else (
    echo snake.exe compiled successfully
)