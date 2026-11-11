#include <stdio.h>
#include <string.h>
#include <math.h>
#include "simulador.h"
#include "validaciones.h"

/* ROL 1 - Carga de mision. Integrar solo esta seccion. */

/* Recibe la direccion de una mision y carga los nueve valores oficiales.
   Modifica la estructura original; no devuelve un valor. */
void cargar_mision_historica(entradas *mision)
{
    mision->masa = 10400.0;
    mision->h_inicial = 120000.0;
    mision->v_inicial = 11000.0;
    mision->angulo_reingreso = 6.2;
    mision->tiempo_frenado = 400.0;
    mision->h_paracaidas = 2500.0;
    mision->v_paracaidas = 25.0;
    mision->v_impacto = 8.0;
    mision->hundimiento = 1.50;

    printf("\n>> Se cargaron los datos oficiales de la mision Artemis.\n");
}

/* Recibe una mision y pide sus parametros mediante validaciones.c.
   Repite cada campo incoherente y modifica la estructura original. */
void configurar_mision_personalizada(entradas *mision)
{
    printf("\n=== CONFIGURACION DE MISION PERSONALIZADA ===\n");

    mision->masa = pedir_double_seguro("Masa de la capsula [1000 - 50000] kg: ", 1000.0, 50000.0);
    mision->h_inicial = pedir_double_seguro("Altitud inicial de reingreso [10000 - 150000] m: ", 10000.0, 150000.0);
    mision->v_inicial = pedir_double_seguro("Velocidad inicial [1000 - 20000] m/s: ", 1000.0, 20000.0);
    mision->angulo_reingreso = pedir_double_seguro("Angulo de reingreso [0.1 - 89.9] grados: ", 0.1, 89.9);
    mision->tiempo_frenado = pedir_double_seguro("Duracion del frenado [10 - 2000] s: ", 10.0, 2000.0);

    do {
        mision->h_paracaidas = pedir_double_seguro("Altitud de apertura de paracaidas [100 - 50000] m: ", 100.0, 50000.0);
        if (mision->h_paracaidas >= mision->h_inicial) {
            printf("  [ERROR] Incoherencia fisica: la altitud de paracaidas debe ser MENOR que la altitud inicial (%.2f m).\n", mision->h_inicial);
        }
    } while (mision->h_paracaidas >= mision->h_inicial);

    do {
        mision->v_paracaidas = pedir_double_seguro("Velocidad al abrir paracaidas [5 - 1000] m/s: ", 5.0, 1000.0);
        if (mision->v_paracaidas >= mision->v_inicial) {
            printf("  [ERROR] Incoherencia fisica: la velocidad de paracaidas debe ser MENOR que la velocidad inicial (%.2f m/s).\n", mision->v_inicial);
        }
    } while (mision->v_paracaidas >= mision->v_inicial);

    do {
        mision->v_impacto = pedir_double_seguro("Velocidad de impacto en el agua [0.1 - 500] m/s: ", 0.1, 500.0);
        if (mision->v_impacto >= mision->v_paracaidas) {
            printf("  [ERROR] Incoherencia fisica: la velocidad de impacto debe ser MENOR que la de paracaidas (%.2f m/s).\n", mision->v_paracaidas);
        }
    } while (mision->v_impacto >= mision->v_paracaidas);

    mision->hundimiento = pedir_double_seguro("Hundimiento en el agua [0.1 - 20] m: ", 0.1, 20.0);

    printf("\n>> Parametros cargados correctamente.\n");
}

/* ============================================================
   ROL 2 - Fisica Analitica
   Puntos A, B y C: Energia, Desaceleracion e Impacto
   ============================================================ */
double calcular_trabajo_rozamiento(const entradas *mision)
{   
    double energia_inicial = 0.5 * mision->masa *
                        mision->v_inicial * mision->v_inicial +
                        mision->masa * GRAVEDAD * mision->h_inicial;

    double energia_final = 0.5 * mision->masa *
                       mision->v_paracaidas * mision->v_paracaidas +
                       mision->masa * GRAVEDAD * mision->h_paracaidas;

    return energia_final - energia_inicial;
}

double calcular_desaceleracion_media(const entradas *mision)
{
    return (mision->v_inicial - mision->v_paracaidas)
       / mision->tiempo_frenado;
}

double calcular_fuerza_amerizaje(const entradas *mision)
{
    double fuerza_frenado = (0.5 * mision->masa *
                        mision->v_impacto * mision->v_impacto)
                        / mision->hundimiento;
    
    return fuerza_frenado + mision->masa * GRAVEDAD;
}

void procesar_calculos_analiticos(const entradas *mision, resultados *res)
{
    res->trabajo_rozamiento = calcular_trabajo_rozamiento(mision);
    res->desaceleracion_media = calcular_desaceleracion_media(mision);
    res->f_impacto = calcular_fuerza_amerizaje(mision);
}

void mostrar_resultados_analiticos(const entradas *mision, const resultados *res)
{
    (void)mision;

    printf("\n=== RESULTADOS DE FISICA ANALITICA ===\n");

    printf("A) Trabajo de rozamiento: %.2f J\n",
           res->trabajo_rozamiento);

    printf("B) Desaceleracion media: %.4f m/s^2\n",
           res->desaceleracion_media);

    printf("C) Fuerza de amerizaje: %.2f N\n",
           res->f_impacto);
}

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
