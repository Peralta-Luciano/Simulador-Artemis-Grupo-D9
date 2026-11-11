#include <stdio.h>
#include "simulador.h"

int main(void)
{
    entradas mision = {0};
    resultados res = {0};

    /* Datos oficiales de la mision Artemis */
    mision.masa = 10400.0;
    mision.v_inicial = 11000.0;
    mision.h_inicial = 120000.0;
    mision.v_paracaidas = 25.0;
    mision.h_paracaidas = 2500.0;
    mision.tiempo_frenado = 400.0;
    mision.v_impacto = 8.0;
    mision.hundimiento = 1.5;

    /* Ejecutar los calculos del Rol 2 */
    procesar_calculos_analiticos(&mision, &res);

    /* Mostrar los resultados */
    mostrar_resultados_analiticos(&mision, &res);

    mision.v_inicial = 1000.0;
    mision.h_inicial = 10000.0;
    mision.v_paracaidas = 5.0;
    mision.h_paracaidas = 500.0;
    mision.tiempo_frenado = 10.0;
    mision.v_impacto = 10.0;
    mision.hundimiento = 1.0;

    printf("\n=== SEGUNDA PRUEBA ===\n");

    procesar_calculos_analiticos(&mision, &res);
    mostrar_resultados_analiticos(&mision, &res);

    return 0;
}