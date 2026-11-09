#ifndef SIMULADOR_H
#define SIMULADOR_H
 
//CONSTANTES FISICAS
#define LIMITE_ARREGLO 1000        // Capacidad maxima de telemetria (filas)
#define GRAVEDAD 9.8               // Aceleracion gravitatoria terrestre [m/s^2]
#define DENSIDAD_BASE 1.225        // Densidad del aire al nivel del mar [kg/m^3]
#define CONSTANTE_ALTURA 8500.0    // Escala de altura de la atmosfera [m]
#define VENTANA_ANGULO_MIN 5.5     // Angulo minimo seguro para no rebotar [grados]
#define VENTANA_ANGULO_MAX 7.0     // Angulo maximo seguro para no desintegrarse [grados]
#define PI 3.14159265358979323846  // Constante Pi para conversion a radianes
#define COEF_ARRASTRE_AREA 15.0    // Coeficiente de arrastre por area (Cd*A) [m^2]
 
//CODIGOS DE ESTADO DE LA MISION
#define ESTADO_NOMINAL 1           // Mision exitosa (nominal)
#define ESTADO_DESINTEGRACION 2    // Falla por calor y fuerzas G excesivas (>7.0 grados)
#define ESTADO_REBOTE 3            // Falla por rebote atmosferico (<5.5 grados)

//PARAMETROS DE SIMULACION Y DEL REPORTE
#define PASOS_ANTES_REBOTE 30      // Segundos de descenso antes de rebotar [s]
#define PASOS_TOTAL_REBOTE 60      // Segundos totales simulados en un rebote [s]
#define FILAS_TABLA 20             // Cantidad aproximada de filas a mostrar en consola
 
//ESTRUCTURA DE ENTRADAS
typedef struct{
    double masa;                   // Masa de la capsula Orion [kg]
    double h_inicial;              // Altitud inicial de reingreso [m]
    double v_inicial;              // Velocidad inicial de reingreso [m/s]
    double angulo_reingreso;       // Angulo de reingreso respecto al horizonte [grados]
    double tiempo_frenado;         // Duracion de la fase de frenado [s]
    double h_paracaidas;           // Altitud al abrir paracaidas [m]
    double v_paracaidas;           // Velocidad al abrir paracaidas [m/s]
    double v_impacto;              // Velocidad al tocar el agua [m/s]
    double hundimiento;            // Profundidad de hundimiento en el agua [m]
} entradas;
 
//REGISTRO DE TELEMETRIA(un registro en la caja negra)
typedef struct{
    double t_actual;               // Tiempo transcurrido [s]
    double h_actual;               // Altitud en el instante [m]
    double v_actual;               // Velocidad en el instante [m/s]
    double fuerzas_g;              // Desaceleracion instantanea [multiplos de g]
} instante_vuelo;
 
//REGISTRO DE RESULTADOS
typedef struct{
    double trabajo_rozamiento;     // Trabajo total de rozamiento [Joules, J]
    double desaceleracion_media;   // Desaceleracion media del frenado [m/s^2]
    double f_impacto;              // Fuerza promedio del agua en amerizaje [N]
    int estado_mision;             // Codigo de estado (1: nominal, 2: desintegracion, 3: rebote)
    char mensaje_falla[100];       // Descripcion textual del resultado
} resultados;
 
//PROTOTIPOS    
void cargar_mision_historica(entradas *mision); //carga los 9 valores por defecto de la mision original
void configurar_mision_personalizada(entradas *mision); //pide al usuario los 9 parametros usando validaciones
 

double calcular_trabajo_rozamiento(const entradas *mision); //punto A, calcula el trabajo de rozamiento atmosferico en joules(retorna valor negativo)
double calcular_desaceleracion_media(const entradas *mision); //Punto B, calcula la desaceleracion media durante el frenado en m/s2
double calcular_fuerza_amerizaje(const entradas *mision); // Punto C, calcula la fuerza promedio del agua en amerizaje en newtons
void procesar_calculos_analiticos(const entradas *mision, resultados *res);// Ejecuta las 3 formulas anteriores y guarda los resultados en la estructura "res"
void mostrar_resultados_analiticos(const entradas *mision, const resultados *res); //muestra en consola los resultados de los puntos A, B y C
 

int ejecutar_simulacion_dinamica(const entradas *mision, instante_vuelo *caja_negra, resultados *res); //Punto D, evalua la ventana de reingreso simulando segundo a segundo la caida, llena el arreglo "caja_negra" y devuelve la cantidad de pasos simulador
void mostrar_telemetria(const resultados *res, const instante_vuelo *caja_negra, int cantidad_pasos);//Muestra el estado del punto D, la tabla de telemetria y el pico de fuerzas G
void mostrar_reporte_completo(const entradas *mision, const resultados *res, const instante_vuelo *caja_negra, int cantidad_pasos); // llama a mostrar_resultados_analiticos y a mostrar_telemetria
 
#endif
