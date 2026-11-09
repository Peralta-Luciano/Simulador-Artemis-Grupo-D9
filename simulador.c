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
    double rad = mision->angulo_reingreso * PI / 180.0;  /* angulo en radianes */
    double t = 0.0;                                      /* tiempo [s] */
    double dt = 1.0;                                     /* paso de integracion [s] */
    double h = mision->h_inicial;                        /* altitud actual [m] */
    double v = mision->v_inicial;                        /* velocidad actual [m/s] */
    double rho;                                          /* densidad del aire */
    double a_inst;                                       /* desaceleracion por arrastre */
    int pasos = 0;                                       /* proxima fila libre */
    int rebotando = 0;                                   /* 1 = la capsula ya sube por rebote */

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

    /* --- Ciclo de integracion paso a paso (Euler, dt = 1 s) --- */
    while (h > mision->h_paracaidas && v > mision->v_paracaidas && pasos < LIMITE_ARREGLO) {

        /* en un rebote se registran solo PASOS_TOTAL_REBOTE filas */
        if (res->estado_mision == ESTADO_REBOTE && pasos >= PASOS_TOTAL_REBOTE) {
            break;
        }

        rho = DENSIDAD_BASE * exp(-h / CONSTANTE_ALTURA);
        a_inst = (0.5 * rho * v * v * COEF_ARRASTRE_AREA) / mision->masa;

        /* se guarda el estado ANTES de actualizarlo */
        caja_negra[pasos].t_actual = t;
        caja_negra[pasos].h_actual = h;
        caja_negra[pasos].v_actual = v;
        caja_negra[pasos].fuerzas_g = a_inst / GRAVEDAD;

        /* a partir de PASOS_ANTES_REBOTE la capsula empieza a subir */
        if (res->estado_mision == ESTADO_REBOTE && pasos >= PASOS_ANTES_REBOTE) {
            rebotando = 1;
        }

        v = v - a_inst * dt;
        if (v < mision->v_paracaidas) {
            v = mision->v_paracaidas;
        }

        if (rebotando) {
            h = h + v * sin(rad) * dt;
        } else {
            h = h - v * sin(rad) * dt;
        }

        t = t + dt;
        pasos++;
    }

    return pasos;
}

/* Imprime el resultado del Punto D, la tabla de la caja negra
   y el pico maximo de fuerzas G. No devuelve nada. */
void mostrar_telemetria(const resultados *res, const instante_vuelo *caja_negra, int cantidad_pasos)
{
    int i;
    int indice_pico = 0;   /* fila con mas fuerzas G */
    int salto;             /* cada cuantas filas se imprime */

    printf("\n--- PUNTO D: Analisis del angulo de reingreso ---\n");
    printf("  %s\n", res->mensaje_falla);

    if (cantidad_pasos <= 0) {
        printf("\n  (No hay datos de telemetria registrados)\n");
        return;
    }

    salto = cantidad_pasos / FILAS_TABLA;
    if (salto < 1) {
        salto = 1;
    }

    printf("\n--- CAJA NEGRA: TELEMETRIA ---\n");
    printf("  %10s | %14s | %15s | %10s\n", "Tiempo (s)", "Altitud (m)", "Velocidad (m/s)", "Fuerzas G");
    printf("  -----------+----------------+-----------------+-----------\n");

    /* recorre todas las filas validas: busca el pico e imprime ~FILAS_TABLA filas */
    for (i = 0; i < cantidad_pasos; i++) {
        if (caja_negra[i].fuerzas_g > caja_negra[indice_pico].fuerzas_g) {
            indice_pico = i;
        }
        if (i % salto == 0 || i == cantidad_pasos - 1) {
            printf("  %10.1f | %14.1f | %15.2f | %10.2f\n",
                   caja_negra[i].t_actual, caja_negra[i].h_actual,
                   caja_negra[i].v_actual, caja_negra[i].fuerzas_g);
        }
    }

    printf("\n  Pico maximo de fuerzas G: %.2f g a %.1f m de altitud (t = %.1f s)\n",
           caja_negra[indice_pico].fuerzas_g, caja_negra[indice_pico].h_actual,
           caja_negra[indice_pico].t_actual);
}

/* Arma el reporte completo de la mision: Puntos A, B, C (Rol 2)
   y Punto D con la telemetria. No devuelve nada. */
void mostrar_reporte_completo(const entradas *mision, const resultados *res, const instante_vuelo *caja_negra, int cantidad_pasos)
{
    printf("\n==============================================\n");
    printf("   REPORTE DE MISION ARTEMIS - REINGRESO\n");
    printf("==============================================\n");
    mostrar_resultados_analiticos(mision, res);          /* Puntos A, B y C (Rol 2) */
    mostrar_telemetria(res, caja_negra, cantidad_pasos); /* Punto D, tabla y pico G */
    printf("\n==============================================\n");
}
