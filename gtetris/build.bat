@echo off

rem >nul 2>nul suppresses terminal output
where gcc >nul 2>nul

rem the "where command" sets the errorlevel to 0 if found, 1 if not found
if %errorlevel% equ 1 (
    echo ERROR: gcc not found in system PATH
    echo Please install MSYS2 or MinGW
    exit /b 1
)

rem check project path first
if exist "%~dp0include\raylib.h" (
    set "FOUND_INCLUDE=%~dp0include"
    set "FOUND_LIB=%~dp0lib"
    goto :found
)

rem check msys64 ucrt64 path
if exist "C:\msys64\ucrt64\include\raylib.h" (
    set "FOUND_INCLUDE=C:\msys64\ucrt64\include"
    set "FOUND_LIB=C:\msys64\ucrt64\lib"
    goto :found
)

rem check msys64 mingw64 path
if exist "C:\msys64\mingw64\include\raylib.h" (
    set "FOUND_INCLUDE=C:\msys64\mingw64\include"
    set "FOUND_LIB=C:\msys64\mingw64\lib"
    goto :found
)

rem check mingw path
if exist "C:\MinGW\include\raylib.h" (
    set "FOUND_INCLUDE=C:\MinGW\include"
    set "FOUND_LIB=C:\MinGW\lib"
    goto :found
)

rem exit if raylib not found
echo ERROR: raylib was not found in any normal locations
echo Please install raylib
exit /b 1

:found

rem build using gcc
gcc main.c game.c pieces.c render.c -g -Wall -Wextra -I"%FOUND_INCLUDE%" -L"%FOUND_LIB%" -static -lraylib -lopengl32 -lgdi32 -lwinmm -o tetris.exe

if %errorlevel% equ 1 (
    echo compilation failed
) else (
    echo tetris.exe compiled successfully
)