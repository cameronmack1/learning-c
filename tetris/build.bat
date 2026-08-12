@echo off

rem >nul 2>nul suppresses terminal output
where gcc >nul 2>nul

rem the "where command" sets the errorlevel to 0 if found, 1 if not found
if %errorlevel% equ 1 (
    echo ERROR: gcc not found in system PATH
    echo Please install MSYS2 or MinGW
    exit /b 1
)
rem check msys64 ucrt64 path
if exist "C:\msys64\ucrt64\include\pdcurses\curses.h" (
    set "FOUND_INCLUDE=C:\msys64\ucrt64\include"
    set "FOUND_LIB=C:\msys64\ucrt64\lib"
    goto :found
)

rem check msys64 mingw64 path
if exist "C:\msys64\mingw64\include\pdcurses\curses.h" (
    set "FOUND_INCLUDE=C:\msys64\mingw64\include"
    set "FOUND_LIB=C:\msys64\mingw64\lib"
    goto :found
)

rem check mingw path
if exist "C:\MinGW\include\curses.h" (
    set "FOUND_INCLUDE=C:\MinGW\include"
    set "FOUND_LIB=C:\MinGW\lib"
    goto :found
)

rem check project path
if exist "%~dp0include\curses.h" (
    set "FOUND_INCLUDE=%~dp0include"
    set "FOUND_LIB=%~dp0lib"
    goto :found
)

rem exit if pdcurses not found
echo ERROR: PDCurses was not found in any normal locations
echo Please install pdcurses
exit /b 1

:found

rem build using gcc
gcc main.c game.c pieces.c -g -Wall -Wextra -I"%FOUND_INCLUDE%" -L"%FOUND_LIB%" -lpdcurses -o tetris.exe

if %errorlevel% equ 1 (
    echo compilation failed
) else (
    echo tetris.exe compiled successfully
)