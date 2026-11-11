@echo off
title Compilador Simulador Artemis
cd /d "%~dp0"
echo ==============================================
echo   Compilador automatico - Simulador Artemis
echo ==============================================
echo.
where gcc >nul 2>nul
if errorlevel 1 goto sin_gcc
echo Compilando...
gcc -Wall -Wextra -std=c99 -pedantic main.c simulador.c validaciones.c -o simulador.exe -lm
if errorlevel 1 goto error
echo.
echo [EXITO] Se genero simulador.exe
echo.
set /p RUN=Queres ejecutar el simulador ahora? (s/n): 
if /i "%RUN%"=="s" goto ejecutar
goto fin
:ejecutar
echo.
simulador.exe
goto fin
:sin_gcc
echo [ERROR] No se encontro el compilador GCC.
echo Segui el "Paso 1" de la seccion Windows del README.md para instalarlo.
goto fin
:error
echo [ERROR] Fallo la compilacion. Revisa los mensajes de arriba.
:fin
echo.
pause
