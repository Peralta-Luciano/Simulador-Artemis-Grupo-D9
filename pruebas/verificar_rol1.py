"""Ejecutar con Python desde cualquier carpeta, luego de compilar test_rol1."""
from pathlib import Path
import subprocess
import os
import sys

ROOT = Path(__file__).resolve().parents[1]
EXE = Path(sys.argv[1]).resolve() if len(sys.argv) > 1 else ROOT / ('test_rol1.exe' if os.name == 'nt' else 'test_rol1')
OFICIAL = ['10400', '120000', '11000', '6.2', '400', '2500', '25', '8', '1.5']
FIELDS = ['masa', 'h_inicial', 'v_inicial', 'angulo_reingreso', 'tiempo_frenado',
          'h_paracaidas', 'v_paracaidas', 'v_impacto', 'hundimiento']
count = 0


def probar(nombre, datos, errores, decimal='55.50', entero='3', mision=OFICIAL, eof=False):
    global count
    result = subprocess.run([str(EXE)], input=datos, text=True,
                            capture_output=True, timeout=5)
    out = result.stdout
    assert (result.returncode != 0) == eof, (nombre, result.returncode, out)
    if eof:
        assert 'El programa termina.' in out, (nombre, out)
    else:
        assert out.count('[ERROR]') == errores, (nombre, out)
        assert f'Valor aceptado: {decimal}\n' in out, (nombre, out)
        assert f'Valor aceptado: {entero}\n' in out, (nombre, out)
        parsed = {}
        blocks = []
        for line in out.splitlines():
            if '=' in line:
                key, _, value = line.partition('=')
                if key.strip() in FIELDS:
                    parsed[key.strip()] = float(value.strip())
                    if key.strip() == 'hundimiento':
                        blocks.append(parsed)
                        parsed = {}
        assert len(blocks) == 2, (nombre, blocks)
        assert blocks[0] == dict(zip(FIELDS, map(float, OFICIAL))), (nombre, blocks)
        assert blocks[1] == dict(zip(FIELDS, map(float, mision))), (nombre, blocks)
    count += 1
    print('OK:', nombre)


def entrada(*prefijos, mision=OFICIAL):
    return '\n'.join([*prefijos, *mision]) + '\n'


probar('datos oficiales', entrada('55.5', '3'), 0)
for invalid in ['hola', '150', '-1', '12abc', '5 6', 'nan', 'inf', '-inf',
                '1e9999', '1e-9999', '1,5', '', '12 ', '9' * 300]:
    probar('decimal invalido: ' + repr(invalid[:25]), entrada(invalid, '55.5', '3'), 1)
for invalid in ['abc', '9', '-1', '2.5', '1abc', '2147483649', '9' * 100, '9' * 300]:
    probar('entero invalido: ' + repr(invalid[:25]), entrada('55.5', invalid, '3'), 1)
for decimal, entero in [('0', '1'), ('100', '4')]:
    probar('limites incluidos', entrada(decimal, entero), 0,
           decimal=f'{float(decimal):.2f}', entero=entero)

cross = ['10400', '20000', '1000', '5', '400', '30000', '20000', '15000',
         '1000', '25', '30', '25', '8', '1.5']
expected = ['10400', '20000', '1000', '5', '400', '15000', '25', '8', '1.5']
probar('tres reglas fisicas, incluida igualdad; angulo 5 permitido',
       entrada('55.5', '3', mision=cross), 5, mision=expected)
with_letters = [item for value in OFICIAL for item in ['hola', value]]
probar('letras en los nueve campos', entrada('55.5', '3', mision=with_letters), 9)
for angle in ['0.1', '10', '89.9']:
    mission = OFICIAL.copy()
    mission[3] = angle
    probar('angulo admitido: ' + angle, entrada('55.5', '3', mision=mission), 0, mision=mission)
probar('ultima linea sin Enter', entrada('55.5', '3').rstrip('\n'), 0)
probar('EOF al inicio', '', 0, eof=True)
probar('EOF despues de error', 'hola\n', 0, eof=True)
probar('EOF al pedir entero', '55.5\n', 0, eof=True)
for i in range(9):
    probar(f'EOF en campo {FIELDS[i]}', '\n'.join(['55.5', '3', *OFICIAL[:i]]) + '\n',
           0, eof=True)
print(f'{count} casos aprobados.')
