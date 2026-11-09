/* Programa de prueba del Rol 3: ejecuta la simulacion dinamica con los
   datos oficiales y tres angulos distintos (nominal, rebote y desintegracion).
   Compilar desde la raiz del proyecto:
   gcc -Wall -Wextra -std=c99 -pedantic -I. pruebas/test_rol3.c simulador.c validaciones.c -o test_rol3 -lm */
#include <stdio.h>
#include "simulador.h"

/* Corre un caso con el angulo indicado e imprime pasos, estado,
   primera y ultima fila de la caja negra y la telemetria. */
static void probar(const char *titulo, double angulo)
{
    entradas m = {10400.0, 120000.0, 11000.0, 0.0, 400.0, 2500.0, 25.0, 8.0, 1.50};
    instante_vuelo caja[LIMITE_ARREGLO];
    resultados res;
    int pasos;

    m.angulo_reingreso = angulo;
    pasos = ejecutar_simulacion_dinamica(&m, caja, &res);

    printf("=== %s (angulo %.1f) ===\n", titulo, angulo);
    printf("  pasos  = %d\n", pasos);
    printf("  estado = %d\n", res.estado_mision);
    printf("  msg    = %s\n", res.mensaje_falla);
    if (pasos > 0) {
        printf("  fila 0 : t=%.1f h=%.1f v=%.2f g=%.4f\n",
               caja[0].t_actual, caja[0].h_actual, caja[0].v_actual, caja[0].fuerzas_g);
        printf("  ultima : t=%.1f h=%.1f v=%.2f g=%.4f\n",
               caja[pasos - 1].t_actual, caja[pasos - 1].h_actual,
               caja[pasos - 1].v_actual, caja[pasos - 1].fuerzas_g);
    }
    mostrar_telemetria(&res, caja, pasos);
    printf("\n");
}

int main(void)
{
    probar("NOMINAL", 6.2);          /* esperado: 739 pasos, estado 1 */
    probar("REBOTE", 5.0);           /* esperado:  60 pasos, estado 3 */
    probar("DESINTEGRACION", 10.0);  /* esperado: 467 pasos, estado 2 */
    return 0;
}
