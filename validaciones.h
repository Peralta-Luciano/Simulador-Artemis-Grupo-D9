#ifndef VALIDACIONES_H
#define VALIDACIONES_H

void limpiar_buffer(void);
double pedir_double_seguro(const char *mensaje, double min, double max);
int pedir_entero_seguro(const char *mensaje, int min, int max);

#endif
