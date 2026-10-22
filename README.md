# Práctica 2: simulador de máquinas de Turing

Simulador de máquinas de Turing deterministas multicinta escrito en C++17. La
definición de la máquina se carga desde un fichero, por lo que el simulador no
contiene lógica específica de ninguna máquina concreta.

## Compilar

```text
make
```

También se puede compilar directamente con un compilador C++17:

```text
g++ -std=c++17 -Iinclude src/*.cc -o turing_simulator
```

O utilizando el `Makefile`:

```text
make
make run-machine1
make run-machine2
make clean
```

## Ejecutar

```text
./turing_simulator -config config/machine1.txt -in examples/machine1.txt
./turing_simulator -config config/machine2.txt
```

Si no se proporciona `-in`, las entradas se leen de la consola, una por línea.
Una línea vacía termina la lectura interactiva.

## Formato de configuración

El fichero usa una propiedad por línea. Se ignoran líneas vacías y comentarios
que comienzan por `#`.

```text
states=q0,q1
input_alphabet=a,b
tape_alphabet=a,b,X,_
initial_state=q0
blank=_
accepting_states=q_accept
tapes=1
transition=q0|a|q1|X|R
```

Una transición contiene `estado_origen|símbolos_leídos|estado_destino|
símbolos_escritos|movimientos`. En una máquina de `n` cintas, los tres campos
de símbolos y movimientos contienen `n` valores separados por comas. Los
movimientos permitidos son `L`, `R` y `S` (sin movimiento). La escritura y el
movimiento de todas las cintas se realizan de forma simultánea.

También se acepta el formato posicional utilizado en los ficheros de la
práctica:

```text
q0 q1 q2
0 1
0 1 .
q0
.
q2
1
q0 0 q1 0 R
q0 1 q0 1 R
```

En este formato, las primeras siete líneas contienen, respectivamente, los
estados, el alfabeto de entrada, el alfabeto de cinta, el estado inicial, el
blanco, los estados de aceptación y el número de cintas. Las líneas restantes
son transiciones con los cinco campos separados por espacios:
`estado_actual símbolo_leído estado_siguiente símbolo_escrito movimiento`.

## Máquinas incluidas

* `config/machine1.txt` reconoce cadenas binarias con un número impar de `0`.
* `config/machine2.txt` calcula el doble de un número representado como `1^n`.

La salida muestra el estado final, si la entrada fue aceptada, el número de
pasos y cada cinta con la posición del cabezal marcada entre corchetes.

La explicación detallada de la arquitectura, el funcionamiento de la máquina y
un ejemplo completo del pipeline está en
[docs/Funcionamiento.md](docs/Funcionamiento.md).