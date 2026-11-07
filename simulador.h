#ifndef SIMULADOR_H
#define SIMULADOR_H
//CONSTANTES//
#define LIMITE_ARREGLO 1000
#define GRAVEDAD 9.8
#define DENSIDAD_BASE 1.225
#define CONSTANTE_ALTURA 8500.0

//ESTRUCTURA DE ENTRADAS/
typedef struct{
    double masa;
    double h_inicial;
    double v_inicial;
    double angulo_reingreso;
    double tiempo_frenado;
    double h_paracaidas;
    double v_paracaidas;
    double v_impacto;
    double hundimiento;
} entradas;

//REGISTRO DE TELEMETRIA//
typedef struct{
    double t_actual;
    double h_actual;
    double v_actual;
    double fuerzas_g;
} instante_vuelo;

//REGISTRO DE RESULTADOS//
typedef struct{
    double trabajo_rozamiento;
    double desaceleracion_media;
    double f_impacto;
    int estado_mision;
    char mensaje_falla[100];
} resultados;

#endif