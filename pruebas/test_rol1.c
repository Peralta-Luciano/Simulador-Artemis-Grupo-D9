#include <stdio.h>
#include "simulador.h"
#include "validaciones.h"

static void imprimir_entradas(const entradas *m)
{
    printf("  masa             = %.2f\n", m->masa);
    printf("  h_inicial        = %.2f\n", m->h_inicial);
    printf("  v_inicial        = %.2f\n", m->v_inicial);
    printf("  angulo_reingreso = %.2f\n", m->angulo_reingreso);
    printf("  tiempo_frenado   = %.2f\n", m->tiempo_frenado);
    printf("  h_paracaidas     = %.2f\n", m->h_paracaidas);
    printf("  v_paracaidas     = %.2f\n", m->v_paracaidas);
    printf("  v_impacto        = %.2f\n", m->v_impacto);
    printf("  hundimiento      = %.2f\n", m->hundimiento);
}

int main(void)
{
    entradas m;
    double x;
    int n;

    printf("--- PRUEBA 1: pedir_double_seguro ---\n");
    x = pedir_double_seguro("Ingrese un decimal entre 0 y 100: ", 0.0, 100.0);
    printf("Valor aceptado: %.2f\n", x);

    printf("\n--- PRUEBA 2: pedir_entero_seguro ---\n");
    n = pedir_entero_seguro("Ingrese un entero entre 1 y 4: ", 1, 4);
    printf("Valor aceptado: %d\n", n);

    printf("\n--- PRUEBA 3: cargar_mision_historica ---\n");
    cargar_mision_historica(&m);
    imprimir_entradas(&m);

    printf("\n--- PRUEBA 4: configurar_mision_personalizada ---\n");
    configurar_mision_personalizada(&m);
    imprimir_entradas(&m);

    return 0;
}
