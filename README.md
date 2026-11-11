# Simulador de Misiones Artemis — Situación 4

Simulador en lenguaje C (C99) del **reingreso atmosférico, frenado aerodinámico y amerizaje**
de una nave Artemis. Trabajo práctico de Algoritmos y Estructuras de Datos (UTN FRRe).

**Grupo D9**

| Integrante | Rol |
|---|---|
| Ramírez Pablo Nicolás (legajo 28856) | Rol 1 — Validaciones y carga |
| Peralta Luciano (legajo: completar) | Rol 2/4 (completar) |
| Rojas Gonzalo (legajo: completar) | Rol (completar) |
| Vaernet Bianca Estefanía (legajo: completar) | Rol (completar) |

Comisión: *(completar)*

## Archivos del proyecto

| Archivo | Qué contiene |
|---|---|
| `main.c` | Menú principal y flujo del programa |
| `simulador.c` / `simulador.h` | Cálculos físicos, simulación y telemetría |
| `validaciones.c` / `validaciones.h` | Lectura y validación segura de datos por teclado |
| `Makefile` | Receta de compilación (`make`) |
| `compilar.bat` / `compilar.sh` | Scripts de compilación con un solo clic / comando |

---

## Cómo compilar y ejecutar (paso a paso, sin conocimientos previos)

Para ejecutar el programa hace falta **un compilador de C** (se llama *GCC*). Es un programa
gratuito que convierte el código en un programa que se puede abrir. Elegí tu sistema operativo.

### Paso 0: Descargar el proyecto

1. Entrá a la página del repositorio en GitHub.
2. Apretá el botón verde **Code** y luego **Download ZIP**.
3. Buscá el archivo `.zip` descargado (carpeta *Descargas*), hacé clic derecho y elegí
   **Extraer todo…** (Windows) o hacé doble clic (Mac).
4. Se crea una carpeta con los archivos `main.c`, `simulador.c`, etc. **No muevas ni borres nada.**

### Windows

**Paso 1 — Instalar el compilador (solo la primera vez)**

1. Abrí el menú Inicio, escribí `cmd` y apretá Enter. Se abre una ventana negra.
2. Escribí `gcc --version` y apretá Enter.
   - Si aparece un número de versión, **ya lo tenés**: andá al Paso 2.
   - Si dice *"no se reconoce como un comando"*, seguí con estos puntos:
3. Entrá a https://github.com/skeeto/w64devkit/releases/latest
4. Bajá hasta *Assets* y descargá el archivo que termina en `.exe` (por ejemplo
   `w64devkit-x64-X.X.X.exe`). Al abrirlo, extrae la carpeta `w64devkit`: elegí `C:\`.
5. Agregalo al PATH para que Windows lo encuentre:
   1. Menú Inicio → escribí **variables de entorno** → *Editar las variables de entorno del sistema*.
   2. Botón **Variables de entorno…**.
   3. En *Variables de usuario*, seleccioná **Path** → **Editar…** → **Nuevo**.
   4. Escribí `C:\w64devkit\bin` y aceptá todas las ventanas.
6. **Cerrá y volvé a abrir** la ventana `cmd`, y repetí `gcc --version` para comprobar.

**Paso 2 — Compilar y ejecutar**

*Forma fácil:* hacé **doble clic en `compilar.bat`**. Si Windows muestra una advertencia azul,
elegí *Más información → Ejecutar de todas formas*. Cuando termine, escribí `s` y Enter para
iniciar el simulador.

*Forma manual:*

1. Abrí la carpeta del proyecto en el Explorador de archivos.
2. Hacé clic en la barra de direcciones (arriba), escribí `cmd` y apretá Enter.
3. Escribí (todo en una línea) y apretá Enter:
   ```
   gcc -Wall -Wextra -std=c99 -pedantic main.c simulador.c validaciones.c -o simulador.exe -lm
   ```
4. Para ejecutar, escribí `simulador.exe` y apretá Enter.

### macOS

1. Abrí **Terminal** (`Cmd + Espacio`, escribí `Terminal`, Enter).
2. Instalá las herramientas del compilador (solo la primera vez): `xcode-select --install`
   y aceptá la instalación. Puede tardar unos minutos.
3. Entrá a la carpeta del proyecto: escribí `cd ` (con un espacio), **arrastrá la carpeta**
   desde el Finder a la Terminal y apretá Enter.
4. Compilá: `make`
5. Ejecutá: `./simulador`

### Linux (Ubuntu / Debian / Mint)

1. Abrí una terminal (`Ctrl + Alt + T`).
2. Instalá el compilador (solo la primera vez): `sudo apt update && sudo apt install build-essential`
3. Entrá a la carpeta: `cd ruta/de/la/carpeta`
4. Compilá: `make`
5. Ejecutá: `./simulador`

### Problemas frecuentes

| Mensaje | Solución |
|---|---|
| `gcc no se reconoce como un comando` | El compilador no está instalado o falta el PATH. Repetí el Paso 1 y reiniciá `cmd`. |
| `make: command not found` | Usá `./compilar.sh`, o el comando `gcc …` manual indicado arriba. |
| `Permission denied` al ejecutar | Escribí `chmod +x simulador` y reintentá. |
| `undefined reference to sqrt/pow` | Falta `-lm` al final del comando de compilación. |

Para borrar los archivos generados: `make clean`.

---

## Uso del simulador

Al ejecutar `simulador` aparece un menú numérico. Escribí el número de la opción y apretá Enter.
Para ingresar datos usá **punto decimal** (`1.5`, no `1,5`). Si el valor es inválido, el programa lo
vuelve a pedir. Para cerrar el programa elegí la opción de salida del menú.

---

## Validaciones y reglas de entrada

Se rechazan letras sin número, líneas vacías, texto adicional (`12abc`, `5 6`),
decimales donde se exige un entero (`2.5`), números fuera de rango, `nan`, infinitos,
desbordamientos y líneas de más de 255 caracteres. Se repite el campo hasta validarlo.
Se usa punto decimal: `1.5`, no `1,5`. Los espacios posteriores al número se rechazan,
como en el ejemplo de los documentos; los iniciales se permiten.

| Campo | Unidad | Mínimo | Máximo | Valor oficial |
|---|---|---:|---:|---:|
| masa | kg | 1000 | 50000 | 10400 |
| h_inicial | m | 10000 | 150000 | 120000 |
| v_inicial | m/s | 1000 | 20000 | 11000 |
| angulo_reingreso | grados | 0.1 | 89.9 | 6.2 |
| tiempo_frenado | s | 10 | 2000 | 400 |
| h_paracaidas | m | 100 | 50000 | 2500 |
| v_paracaidas | m/s | 5 | 1000 | 25 |
| v_impacto | m/s | 0.1 | 500 | 8 |
| hundimiento | m | 0.1 | 20 | 1.5 |

Los límites de la tabla se incluyen. Además se exige:

- `h_paracaidas < h_inicial`.
- `v_paracaidas < v_inicial`.
- `v_impacto < v_paracaidas`.

La igualdad también se rechaza. Ante incoherencia se pide de nuevo solo ese campo.
Los ángulos 5 y 10 grados son entradas válidas: Rol 3 debe poder simular rebote y
desintegración. No hay que restringir la carga a la ventana segura 5.5–7 grados.

`limpiar_buffer` descarta caracteres hasta Enter o EOF. Aquí se usa cuando una línea
excede la capacidad del arreglo, para que sus restos no contaminen la siguiente lectura.
Al cerrarse la entrada se muestra un mensaje y se termina con `EXIT_FAILURE`, sin bucles.
En una consola Windows, EOF normalmente se ingresa con Ctrl+Z seguido de Enter.

### Adaptación respecto del ejemplo del documento

El documento enseña `scanf` numérico más `getchar`. Esta versión usa `fgets` para leer
la línea completa y `strtod`/`strtol` para convertirla. Así se comprueba `errno == ERANGE`
cuando el número no es representable, antes de convertirlo a `int`. `fin` señala el primer
carácter que no se convirtió: si no es el final de la cadena, hay texto sobrante.
`isfinite` rechaza NaN e infinito. Una función auxiliar `static`, `leer_linea`, comparte
la lectura entre ambos validadores sin cambiar los encabezados públicos.

Esta adaptación agrega conceptos a los explicados en el documento. Si la cátedra exige
específicamente usar `scanf`, habrá que ajustar esa implementación a la restricción.
Todo el ingreso por teclado sigue concentrado en `validaciones.c`.

## aporte Rol 1

- Integrante: Ramirez Pablo Nicolas, 28856.
- Consultas: lectura de los cinco documentos de roles y desarrollo de Rol 1;
  revision de la consigna original y del repositorio para aportar individualmente.
- Asistencia: implementacion inicial, adaptacion de lectura segura, pruebas y documentacion.
- Adaptacion: lectura con fgets y conversion con strtod/strtol para detectar
  desbordamientos; firmas publicas y estructura entradas conservadas.
- Verificacion: MSVC con /W4 /WX y GCC 16.2 con -Wall -Wextra -std=c99 -pedantic,
  sin advertencias; 43 casos automaticos aprobados.
