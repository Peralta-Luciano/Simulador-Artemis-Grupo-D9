#!/bin/sh
# Compila y (opcionalmente) ejecuta el Simulador Artemis en Linux / macOS.
cd "$(dirname "$0")" || exit 1
if ! command -v gcc >/dev/null 2>&1; then
    echo "[ERROR] No se encontro gcc. Segui el README.md para instalarlo."
    exit 1
fi
echo "Compilando..."
if gcc -Wall -Wextra -std=c99 -pedantic main.c simulador.c validaciones.c -o simulador -lm; then
    echo "[EXITO] Se genero ./simulador"
    printf "Queres ejecutarlo ahora? (s/n): "
    read r
    [ "$r" = "s" ] && ./simulador
else
    echo "[ERROR] Fallo la compilacion."
    exit 1
fi
