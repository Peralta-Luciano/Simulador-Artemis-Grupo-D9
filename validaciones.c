#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "validaciones.h"

/* Descarta el resto de la linea de entrada.
   No recibe parametros ni retorna valores; tambien se detiene ante EOF. */
void limpiar_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* Descarta caracteres. */
    }
}

/* Lee una linea acotada en el arreglo recibido. Devuelve 0 si es muy larga.
   Ante fin de entrada termina, ya que las firmas no permiten retornar error. */
static int leer_linea(const char *mensaje, char *linea, int capacidad)
{
    char *salto;
    int c;

    printf("%s", mensaje);
    fflush(stdout);
    if (fgets(linea, capacidad, stdin) == NULL) {
        printf("\nSe cerro la entrada de datos o no se pudo leer. El programa termina.\n");
        exit(EXIT_FAILURE);
    }
    salto = strchr(linea, '\n');
    if (salto != NULL) {
        *salto = '\0';
        return 1;
    }
    c = getchar();
    if (c == '\n' || c == EOF) {
        return 1;
    }
    limpiar_buffer();
    printf("  [ERROR] La entrada es demasiado larga.\n");
    return 0;
}

/* Pide un decimal dentro de [min, max] y lo devuelve al validarlo.
   Rechaza texto sobrante, valores no finitos y desbordamientos numericos. */
double pedir_double_seguro(const char *mensaje, double min, double max)
{
    char linea[256];
    char *fin;
    double valor;

    do {
        if (!leer_linea(mensaje, linea, sizeof(linea))) {
            continue;
        }
        errno = 0;
        valor = strtod(linea, &fin);
        if (fin == linea) {
            printf("  [ERROR] Debe ingresar un numero (no letras ni simbolos).\n");
        } else if (*fin != '\0') {
            printf("  [ERROR] Ingreso caracteres extra despues del numero.\n");
        } else if (errno == ERANGE || !isfinite(valor) ||
                   !(valor >= min && valor <= max)) {
            printf("  [ERROR] El valor debe estar entre %.2f y %.2f.\n", min, max);
        } else {
            return valor;
        }
    } while (1);
}

/* Pide un entero dentro de [min, max] y lo devuelve al validarlo.
   Convierte primero a long para comprobar el rango antes de pasar a int. */
int pedir_entero_seguro(const char *mensaje, int min, int max)
{
    char linea[256];
    char *fin;
    long valor;

    do {
        if (!leer_linea(mensaje, linea, sizeof(linea))) {
            continue;
        }
        errno = 0;
        valor = strtol(linea, &fin, 10);
        if (fin == linea) {
            printf("  [ERROR] Debe ingresar un numero entero (no letras ni simbolos).\n");
        } else if (*fin != '\0') {
            printf("  [ERROR] Ingreso caracteres extra despues del numero.\n");
        } else if (errno == ERANGE || valor < min || valor > max) {
            printf("  [ERROR] El valor debe estar entre %d y %d.\n", min, max);
        } else {
            return (int)valor;
        }
    } while (1);
}
