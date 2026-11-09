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
    int pasos = 0;

    (void)caja_negra;

    /* --- Punto D: evaluar la ventana del angulo de reingreso --- */
    if (mision->angulo_reingreso < VENTANA_ANGULO_MIN) {
        res->estado_mision = ESTADO_REBOTE;
        snprintf(res->mensaje_falla, sizeof(res->mensaje_falla),
                 "REBOTE ATMOSFERICO: angulo menor a 5.5 grados, la capsula rebota al espacio.");
    } else if (mision->angulo_reingreso > VENTANA_ANGULO_MAX) {
        res->estado_mision = ESTADO_DESINTEGRACION;
        snprintf(res->mensaje_falla, sizeof(res->mensaje_falla),
                 "DESINTEGRACION: angulo mayor a 7.0 grados, G excesivas y sobrecarga termica.");
    } else {
        res->estado_mision = ESTADO_NOMINAL;
        snprintf(res->mensaje_falla, sizeof(res->mensaje_falla),
                 "MISION EXITOSA: reingreso dentro de la ventana segura (5.5 a 7.0 grados).");
    }

    return pasos;
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
