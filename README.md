#Simulador de Misiones Artemis — Situación 4
### Reingreso Atmosférico, Frenado Aerodinámico y Amerizaje (*Splashdown*)

Trabajo Práctico Integrador de **Algoritmos y Estructuras de Datos**  
**Universidad Tecnológica Nacional — Facultad Regional Resistencia (UTN FRRe)**  
Lenguaje: **C**

---

## a) Integrantes y Comisión

* **Grupo:** D9
* **Comisión:** 4

| Integrante | Legajo |
|---|---|
| **Peralta, Luciano Iván** | 29393 |
| **Ramirez, Pablo Nicolas** | 28856 |
| **Rojas, Gonzalo Damian** | 28838 |
| **Vaernet, Bianca Estefania** | 28930 |

---

## b) Descripción Detallada del Escenario Artemis y Reglas del Juego

### 1. El Escenario Físico: Situación 4
El programa modela la etapa final y más crítica de la misión lunar **Artemis**: el regreso de la cápsula tripulada **Orion** a la Tierra.

1. **Reingreso a la Alta Atmósfera (120 km):**  
   La cápsula (masa nominal de $10.400\text{ kg}$) ingresa a las capas superiores de la atmósfera terrestre a una velocidad hipersónica extrema de $11.000\text{ m/s}$ ($\approx 39.600\text{ km/h}$).
2. **Frenado Aerodinámico y Escudo Térmico:**  
   Durante un período de frenado intenso ($\Delta t \approx 400\text{ s}$), el escudo térmico disipa la colosal energía cinética de la nave mediante rozamiento con las partículas de aire, reduciendo la velocidad a $25\text{ m/s}$ al alcanzar los $2.500\text{ m}$ de altitud.
3. **Despliegue de Paracaídas:**  
   A los $2.500\text{ m}$ sobre el nivel del mar, se activan los paracaídas principales, estabilizando y frenando la nave hasta una velocidad de impacto controlada de $8\text{ m/s}$.
4. **Amerizaje (*Splashdown*) en el Océano Pacífico:**  
   La cápsula golpea la superficie del agua a $8\text{ m/s}$ y penetra aproximadamente $1,5\text{ m}$ en el mar hasta detenerse por completo y quedar flotando para el rescate de la tripulación.

---

### 2. Parámetros de Entrada del Simulador

El usuario puede ejecutar la simulación con los datos oficiales de la NASA o ingresar sus propios parámetros personalizados:

| Parámetro | Variable | Unidad | Mínimo | Máximo | Valor Oficial Artemis |
|---|---|---|---:|---:|---:|
| Masa de la cápsula | `masa` | kg | 1.000 | 50.000 | 10.400 |
| Altitud inicial de reingreso | `h_inicial` | m | 10.000 | 150.000 | 120.000 |
| Velocidad inicial | `v_inicial` | m/s | 1.000 | 20.000 | 11.000 |
| Ángulo de reingreso ($\gamma$) | `angulo_reingreso` | grados | 0,1 | 89,9 | 6,2 |
| Tiempo de frenado aerodinámico | `tiempo_frenado` | s | 10 | 2.000 | 400 |
| Altitud de apertura de paracaídas | `h_paracaidas` | m | 100 | 50.000 | 2.500 |
| Velocidad al abrir paracaídas | `v_paracaidas` | m/s | 5 | 1.000 | 25 |
| Velocidad de impacto en el agua | `v_impacto` | m/s | 0,1 | 500 | 8 |
| Profundidad de hundimiento | `hundimiento` | m | 0,1 | 20 | 1,5 |

---

### 3. Reglas del Simulador y Condiciones de Vuelo

Para que una misión sea considerada **Éxito Nominal**, deben cumplirse las leyes físicas y tolerancias biológicas de la tripulación:

* **Ventana de Reingreso Angular ($\gamma$):**
  * **$5,5^\circ \le \gamma \le 7,0^\circ$ (Corredor Seguro):** La nave desacelera adecuadamente y ameriza con éxito (el valor histórico oficial es de **$6,2^\circ$**).
  * **$\gamma < 5,5^\circ$ (Rebote Atmosférico):** La trayectoria es demasiado rasante. La atmósfera actúa como un espejo reflector; la nave "rebota" contra las capas densas y es eyectada nuevamente hacia el espacio sin combustible para retornar.
  * **$\gamma > 7,0^\circ$ (Peligro Térmico y Desintegración):** La trayectoria es demasiado empinada. La tasa de compresión atmosférica genera temperaturas extremas que superan la resistencia del escudo térmico, sumado a una desaceleración violenta que supera los límites estructurales de la nave.
* **Límites de Aceleración y Carga Fisiológica (Fuerzas G):**  
  La desaceleración media durante el frenado no debe exceder las tolerancias humanas de la tripulación ($\approx 10\text{ a }12\text{ g}$).
* **Coherencias Físicas Obligatorias:**
  * La altitud de paracaídas debe ser estrictamente menor a la inicial: $h_{\text{paracaídas}} < h_{\text{inicial}}$.
  * La velocidad con paracaídas debe ser menor a la inicial: $v_{\text{paracaídas}} < v_{\text{inicial}}$.
  * La velocidad final de impacto debe ser menor a la de paracaídas: $v_{\text{impacto}} < v_{\text{paracaídas}}$.

---

## c) Breve Detalle de Funcionamiento

### 1. Arquitectura de Software Modular (C99)
El proyecto respeta rigurosamente las buenas prácticas de la cátedra:
* **Separación de responsabilidades:** Interfaz y flujo (`main.c`), motor físico y telemetría (`simulador.c` / `simulador.h`), entrada segura (`validaciones.c` / `validaciones.h`).
* **Memoria centralizada en `main`:** Todas las variables de estado (`mision_actual`, `reporte_actual` y el arreglo de 1000 registros `caja_negra`) residen en la pila de `main`. Los submódulos reciben punteros (`entradas *`, `resultados *`, `instante_vuelo *`), garantizando transparencia y previniendo efectos colaterales o uso de variables globales ocultas.
* **Tolerancia a fallos por teclado:** Ningún módulo de cálculo lee directamente de `stdin`. Todo ingreso es procesado por `validaciones.c` mediante `fgets` y conversión segura con `strtod`/`strtol`, descartando desbordamientos de búfer, caracteres basura (ej: `12abc`), números imaginarios o valores fuera de rango físico.

```text
Simulador-Artemis-Grupo-D9/
├── main.c              # Menú interactivo y orquestación general del flujo
├── simulador.h         # Cabecera con estructuras, constantes y prototipos
├── simulador.c         # Cálculos analíticos, simulación temporal y telemetría
├── validaciones.h      # Prototipos de funciones de entrada segura
├── validaciones.c      # Implementación de lectura y saneamiento de buffer
├── Makefile            # Automatización estándar de compilación (make)
├── compilar.bat        # Script de compilación automática para Windows (1-clic)
├── compilar.sh         # Script de compilación para Linux / macOS
└── .gitignore          # Filtro de archivos binarios y temporales
```

---

### 2. Módulos y Flujo del Programa

#### A. Menú Principal (`main.c`)
Ofrece 4 opciones al usuario:
1. **Simular misión Artemis histórica:** Carga instantáneamente los valores oficiales de la misión Orion, ejecuta los cálculos y genera la telemetría.
2. **Simular misión con parámetros personalizados:** Solicita los 9 parámetros por teclado con validación interactiva campo por campo.
3. **Ver último reporte y caja negra:** Muestra el informe analítico y la tabla de telemetría de la última corrida en memoria sin necesidad de recalcular.
4. **Salir:** Finaliza el programa de manera limpia liberando la terminal.

#### B. Cálculos Físicos Analíticos (`simulador.c`)
* **Punto A (Trabajo de Rozamiento Aerodinámico):**  
  Calcula la energía mecánica inicial y final ($E = \frac{1}{2} m v^2 + m g h$) aplicando el teorema del trabajo y la energía mecánica para fuerzas no conservativas:  
  $$W_{\text{roz}} = \Delta E_{\text{mec}} = E_{\text{mec final}} - E_{\text{mec inicial}}$$  
  Se presenta en Joules y en Gigajoules (GJ) mediante notación científica.
* **Punto B (Desaceleración Media):**  
  Aceleración promedio sufrida por la cápsula durante el frenado:  
  $$a_{\text{media}} = \frac{v_{\text{inicial}} - v_{\text{paracaídas}}}{\Delta t}$$  
  Se reporta tanto en $\text{m/s}^2$ como en múltiplos de la aceleración de la gravedad estándar ($g_0 = 9,80665\text{ m/s}^2$).
* **Punto C (Fuerza Promedio de Impacto en el Agua):**  
  Fuerza media ejercida por el agua durante la desaceleración del impacto a lo largo de la distancia de hundimiento $d$, considerando la energía cinética y el trabajo del peso:  
  $$F_{\text{agua}} = \frac{\frac{1}{2} m v_{\text{impacto}}^2}{d} + m g_0$$  
  Se informa en Newtons (N), kiloNewtons (kN) y como múltiplo del peso estático de la cápsula ($P = m \cdot g_0$).

#### C. Simulación Dinámica y Telemetría (`simulador.c`)
* Integra temporalmente el descenso con un paso de tiempo de $\Delta t = 1,0\text{ s}$.
* Modela la variación de densidad del aire en función de la altitud según la ley barométrica exponencial:  
  $$\rho(h) = \rho_0 \cdot e^{-h / H}$$  
  *(donde $\rho_0 = 1,225\text{ kg/m}^3$ y la altura de escala $H \approx 7.200\text{ a }8.500\text{ m}$)*.
* Registra segundo a segundo en el arreglo `caja_negra`: tiempo transcurrido, altitud actual, velocidad instantánea, desaceleración y fuerzas G.
* Emite alertas dinámicas en la consola: *"Rebote atmosférico"*, *"Peligro térmico"*, *"Apertura de paracaídas"* y *"Amerizaje exitoso"*, computando el **pico máximo de fuerzas G** experimentado durante toda la trayectoria.

---

## d) Instrucciones Claras de Compilación y Ejecución

El código fue escrito en **C99 estándar** y es 100% multiplataforma. No requiere librerías externas más allá de la biblioteca matemática estándar (`-lm`).

### Paso 0: Descarga del Proyecto
1. Descargá el repositorio desde GitHub haciendo clic en el botón verde **`Code`** $\to$ **`Download ZIP`**.
2. Descomprimí el archivo `.zip` en tu computadora.

---

### En Windows (Para usuarios con o sin conocimientos de C)

#### Opción A: Compilación Automática (La más sencilla)
1. Abrí la carpeta del proyecto.
2. Hacé doble clic en el archivo **`compilar.bat`**.
3. El script detectará tu compilador GCC, compilará el programa con todas las directivas de seguridad (`-Wall -Wextra -std=c99 -pedantic`) y te consultará si deseás ejecutar el simulador en el instante (`s/n`).

#### Opción B: Por Consola de Comandos (CMD / PowerShell)
1. Abrí la carpeta donde descomprimiste el proyecto.
2. En la barra de direcciones superior del Explorador de Windows, escribí `cmd` y presioná **Enter**.
3. Compilá con el siguiente comando:
   ```cmd
   gcc -Wall -Wextra -std=c99 -pedantic main.c simulador.c validaciones.c -o simulador.exe -lm
   ```
4. Ejecutá el programa:
   ```cmd
   simulador.exe
   ```

> **¿No tenés GCC instalado en Windows?**  
> 1. Descargá el entorno liviano y portable **w64devkit**: [Descargar última versión en GitHub](https://github.com/skeeto/w64devkit/releases/latest) (elegir el archivo `w64devkit-x64-X.X.X.zip` o `.exe`).  
> 2. Descomprimilo en `C:\w64devkit`.  
> 3. Agregá `C:\w64devkit\bin` a las *Variables de Entorno* de tu Windows (Path), o simplemente abrí el ejecutable `w64devkit.exe` y compilá desde la consola que se abre.

---

### En macOS

1. Abrí la aplicación **Terminal** (`Cmd + Espacio`, escribí `Terminal` y presioná Enter).
2. Si nunca instalaste las herramientas de desarrollo de Apple, ejecutá una única vez:
   ```bash
   xcode-select --install
   ```
3. Navegá hasta la carpeta del proyecto escribiendo `cd ` (con espacio) y arrastrando la carpeta desde el Finder hacia la Terminal.
4. Compilá con `make`:
   ```bash
   make
   ```
5. Ejecutá el simulador:
   ```bash
   ./simulador
   ```
   *(También podés compilar y ejecutar en un solo paso escribiendo `make run`)*.

---

### En Linux (Ubuntu / Debian / Fedora / Mint)

1. Abrí una terminal (`Ctrl + Alt + T`).
2. Asegurate de contar con el paquete de desarrollo básico:
   ```bash
   sudo apt update && sudo apt install build-essential
   ```
3. Posicionate en el directorio del proyecto y compilá con:
   ```bash
   make
   ```
4. Ejecutá el simulador:
   ```bash
   ./simulador
   ```

---

### Limpieza de Binarios
Para eliminar los archivos compilados (`.o`, ejecutables) y dejar limpio el repositorio:
```bash
make clean
```

---

## e) Registro del Uso de Inteligencia Artificial

Para el desarrollo de este proyecto utilizamos herramientas de Inteligencia Artificial (principalmente **Google Gemini**, **ChatGPT** y **Claude**) como soporte técnico y guía. La IA nos asistió en la generación de estructuras iniciales de código, la formulación de los cálculos físicos, la resolución de dudas sobre el lenguaje C y la corrección de errores de compilación.

A partir de las respuestas y fragmentos generados por las herramientas, el grupo se encargó de analizar las propuestas, adaptar e integrar las funciones en el proyecto, realizar las pruebas de ejecución y estudiar el código para comprender su funcionamiento.

A continuación detallamos las áreas principales donde recurrimos a la IA y cómo trabajamos las soluciones:

| Área de Consulta | ¿Para qué consultamos a la IA? | ¿Cómo lo trabajamos y resolvimos nosotros? |
|---|---|---|
| **Sintaxis y conceptos de C** | Le pedimos que nos explique con ejemplos sencillos cómo pasar datos entre funciones usando punteros, cómo conectar los archivos `.c` con sus `.h`, y cómo evitar que el programa se tilde o entre en bucles infinitos si alguien escribe una letra en vez de un número. | Entendimos la diferencia entre pasar datos por valor y por referencia. Para la entrada de datos, decidimos leer líneas enteras con `fgets` y convertirlas con funciones estándar (`strtod` y `strtol`), probando nosotros mismos desde la consola casos raros como escribir letras, números negativos o espacios hasta que no fallara |
| **Física y fórmulas del problema** | Teníamos dudas sobre cómo plantear correctamente el trabajo de rozamiento con la energía mecánica (inicial vs final) y cómo convertir los valores a las unidades que pedía la consigna (como pasar los Joules a Gigajoules en notación científica o calcular los múltiplos de $g$). | Analizamos las explicaciones de las fórmulas, las pasamos a código C en `simulador.c` y verificamos los resultados|
| **Simulación temporal y telemetría** | Le consultamos cómo pensar un bucle que simule el paso del tiempo segundo a segundo y cómo ir guardando los datos en un arreglo (la caja negra) sin que se pase del tamaño máximo de memoria. | Diseñamos la lógica del ciclo de vuelo, agregamos las condiciones para avisar si la nave rebota en la atmósfera o si corre peligro térmico según el ángulo de entrada, y calculamos el pico máximo de desaceleración que experimenta la cápsula. |
| **Depuración de errores (Debugging)** | Cuando el compilador nos tiraba advertencias o errores que no entendíamos, le pegábamos el mensaje a la IA para que nos explique qué significaba. | Aprendimos que funciones matemáticas como `sqrt` y `pow` necesitan la bandera `-lm` para compilar en Linux/Mac. Corregimos las advertencias una por una hasta que el código compiló de forma completamente limpia |
| **Depuración de errores (Debugging)** | Cuando el compilador nos tiraba advertencias o errores que no entendíamos, le pegábamos el mensaje a la IA para que nos explique qué significaba. | Aprendimos que funciones matemáticas como `sqrt` y `pow` necesitan la bandera `-lm` para compilar en Linux/Mac. Corregimos las advertencias una por una hasta que el código compiló de forma completamente limpia. |
| **Automatización y Scripts** | Le pedimos ayuda para entender la sintaxis de un `Makefile` básico y cómo armar un archivo `.bat` para Windows, ya que varios compañeros usamos Windows y queríamos una forma rápida de compilar sin memorizar comandos largos. | Probamos el archivo `compilar.bat` haciendo doble clic en Windows y el `Makefile` con el comando `make` en Mac y Linux, dejando instrucciones claras en este README para que cualquier persona o docente pueda probarlo sin complicaciones. |

