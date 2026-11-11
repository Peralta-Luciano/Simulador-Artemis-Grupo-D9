#include <stdio.h>
#include "simulador.h"
#include "validaciones.h"

static void mostrar_menu(void)
{

    printf("        SIMULADOR ARTEMIS - SITUACION 4\n");
    printf("\n");

    printf(" 1. Simular mision Artemis historica (datos oficiales)\n");
    printf(" 2. Simular mision con parametros personalizados\n");
    printf(" 3. Ver ultimo reporte y caja negra\n");
    printf(" 4. Salir\n");
    printf("\n");
}

//Calcula A, B y C, y despues corre la simulacion del punto D, devuelve la cantidad de filas cargadas en caja_negra
static int correr_simulacion(const entradas *mision, resultados *reporte, instante_vuelo *caja_negra)
{
    procesar_calculos_analiticos(mision, reporte);
    return ejecutar_simulacion_dinamica(mision, caja_negra, reporte);
}

int main(void)
{
    entradas mision_actual;                       //Molde 1: los 9 parametros de la mision
    instante_vuelo caja_negra[LIMITE_ARREGLO];    //Molde 2: arreglo de telemetria
    resultados reporte_actual;                    //Molde 3: resultados de A, B, C y D
    int pasos_registrados = 0;                    //filas ocupadas de caja_negra
    int mision_simulada = 0;                      //bandera: 0 = no hay datos todavia, 1 = ya se simulo
    int opcion;

    do {
        mostrar_menu();
        opcion = pedir_entero_seguro("Seleccione una opcion: ", 1, 4);

        switch (opcion) {
            case 1:
                cargar_mision_historica(&mision_actual);
                pasos_registrados = correr_simulacion(&mision_actual, &reporte_actual, caja_negra);
                mision_simulada = 1;
                mostrar_reporte_completo(&mision_actual, &reporte_actual, caja_negra, pasos_registrados);
                break;
            case 2:
                configurar_mision_personalizada(&mision_actual);
                pasos_registrados = correr_simulacion(&mision_actual, &reporte_actual, caja_negra);
                mision_simulada = 1;
                mostrar_reporte_completo(&mision_actual, &reporte_actual, caja_negra, pasos_registrados);
                break;
            case 3:
                if (mision_simulada == 0) {
                    printf("\n[AVISO] Todavia no hay datos. Ejecute primero la opcion 1 o 2.\n");
                } else {
                    mostrar_reporte_completo(&mision_actual, &reporte_actual, caja_negra, pasos_registrados);
                }
                break;
            case 4:
                break;
        }
    } while (opcion != 4);

    return 0;
}  

