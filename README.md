# Simulador-Artemis-Grupo-D9

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

## Pruebas de Rol 1

Desde la raiz del repositorio, con GCC:

```sh
gcc -Wall -Wextra -std=c99 -pedantic -I. pruebas/test_rol1.c pruebas/stub_rol2.c simulador.c validaciones.c -o test_rol1 -lm
./test_rol1
```

En PowerShell, ejecutar `.\test_rol1.exe`. El programa prueba decimales, enteros,
los nueve valores oficiales y la carga personalizada. Se pueden probar entradas
incorrectas, valores fuera de rango y datos incoherentes desde la consola.

El archivo `pruebas/stub_rol2.c` permite enlazar las pruebas mientras falta
`mostrar_resultados_analiticos` del Rol 2; no debe incluirse al enlazar con su
implementacion real.

Esta entrega implementa exclusivamente Rol 1. El menu, los calculos fisicos y la
telemetria se incorporan mediante los aportes de los otros integrantes.

## aporte Rol 1

- Integrante: Ramirez Pablo Nicolas, 28856.
- Consultas: lectura de los cinco documentos de roles y desarrollo de Rol 1;
  revision de la consigna original y del repositorio para aportar individualmente.
- Asistencia: implementacion inicial, adaptacion de lectura segura, pruebas y documentacion.
- Adaptacion: lectura con fgets y conversion con strtod/strtol para detectar
  desbordamientos; firmas publicas y estructura entradas conservadas.
- Verificacion: MSVC con /W4 /WX sin advertencias y 43 casos automaticos aprobados.
  Queda pendiente verificar con GCC, revisar el codigo con el alumno e integrar
  los otros roles. El uso de IA no reemplaza la comprension para el coloquio.
