#include <stdio.h>
#include <string.h>
#include <math.h>
#include "simulador.h"
#include "validaciones.h"

/* ============================================================
   ROL 3 - Simulacion dinamica y telemetria (Punto D)
   ============================================================ */

/* Simula la caida segundo a segundo, evalua el angulo de reingreso
   y guarda cada instante en la caja negra.
   Devuelve la cantidad de filas cargadas en caja_negra. */
int ejecutar_simulacion_dinamica(const entradas *mision, instante_vuelo *caja_negra, resultados *res)
{
    (void)mision; (void)caja_negra; (void)res;
    return 0;
}

/* Imprime el resultado del Punto D, la tabla de la caja negra
   y el pico maximo de fuerzas G. No devuelve nada. */
void mostrar_telemetria(const resultados *res, const instante_vuelo *caja_negra, int cantidad_pasos)
{
    (void)res; (void)caja_negra; (void)cantidad_pasos;
}

/* Arma el reporte completo de la mision: Puntos A, B, C (Rol 2)
   y Punto D con la telemetria. No devuelve nada. */
void mostrar_reporte_completo(const entradas *mision, const resultados *res, const instante_vuelo *caja_negra, int cantidad_pasos)
{
    (void)mision; (void)res; (void)caja_negra; (void)cantidad_pasos;
}
