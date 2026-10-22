# Funcionamiento del simulador de máquinas de Turing

## 1. Objetivo del programa

Este proyecto implementa un simulador de máquinas de Turing deterministas,
configurable mediante un fichero de texto. El programa no contiene las reglas
de una máquina concreta dentro de `main`: las reglas se leen desde el fichero
indicado mediante `-config`. De esta forma, el mismo ejecutable puede simular
máquinas diferentes.

La implementación admite máquinas de una o varias cintas. Cada cinta se
representa de forma dispersa: solo se almacenan las posiciones que han recibido
un símbolo explícito. Cualquier posición no almacenada contiene el símbolo
blanco.

## 2. Modelo formal

Una máquina se describe mediante:

- `Q`: conjunto de estados.
- `Sigma`: alfabeto de entrada.
- `Gamma`: alfabeto de cinta.
- `q0`: estado inicial.
- `B`: símbolo blanco.
- `F`: conjunto de estados de aceptación.
- `k`: número de cintas.
- `delta`: conjunto de transiciones deterministas.

Una transición tiene la forma:

```text
estado_actual simbolos_leidos estado_siguiente simbolos_escritos movimientos
```

Para una máquina de una cinta, por ejemplo:

```text
q0 0 q1 0 R
```

significa:

1. Si la máquina está en `q0`.
2. Si el cabezal lee `0`.
3. Escribe `0`.
4. Mueve el cabezal a la derecha (`R`).
5. Cambia al estado `q1`.

Los movimientos disponibles son:

- `L`: izquierda.
- `R`: derecha.
- `S`: permanece en la misma posición.

En una máquina multicinta se indica un símbolo y un movimiento para cada cinta.
Por ejemplo:

```text
q0 a,_ q1 a,a R,R
```

## 3. Organización del código

### `Transition`

`Transition` es un objeto inmutable desde el punto de vista de la simulación
que almacena una regla de `delta`:

- Estado de origen.
- Símbolos que se deben leer.
- Estado de destino.
- Símbolos que se deben escribir.
- Movimientos de los cabezales.

La transición no ejecuta nada por sí misma. Solo contiene las instrucciones;
`TuringMachine` se encarga de aplicarlas.

### `Tape`

Cada cinta mantiene:

- Un `std::map<long long, char>` con las celdas escritas.
- La posición actual del cabezal.
- El símbolo blanco.
- La posibilidad de moverse en ambas direcciones.

La cinta es conceptualmente infinita en ambas direcciones. Si se consulta una
posición que no existe en el mapa, `read()` devuelve el blanco sin crear una
célula nueva.

`getVisibleContents()` calcula el intervalo mínimo que contiene las celdas
escritas y el cabezal. Ese intervalo es el que se muestra al usuario.

### `TuringMachine`

Es el núcleo del simulador. Guarda la definición de la máquina, sus transiciones
y las cintas de la ejecución actual. Sus operaciones principales son:

1. `validate()`: comprueba que la definición sea formalmente correcta.
2. `initializeTapes(input)`: coloca la entrada en la primera cinta y deja las
   restantes vacías.
3. `findApplicableTransition()`: busca una transición cuyo estado de origen
   coincida y cuyos símbolos leídos sean exactamente los que hay bajo los
   cabezales.
4. `executeTransition()`: escribe primero en todas las cintas, mueve después
   todos los cabezales y cambia finalmente de estado.
5. `run(input)`: repite la búsqueda y ejecución hasta que no haya transición.

La máquina no se detiene automáticamente al entrar en un estado de aceptación.
Solo se detiene cuando no existe una transición aplicable. Después comprueba si
el estado en el que se ha detenido pertenece a `F`.

### `ConfigurationParser`

Lee el fichero de configuración. Se soportan dos formatos:

1. El formato posicional de la práctica.
2. El formato de propiedades `clave=valor` usado inicialmente en el proyecto.

El formato posicional utilizado por los ejemplos actuales es:

```text
q0 q1 q2
0 1
0 1 .
q0
.
q2
1
q0 0 q1 0 R
...
```

Las primeras siete líneas significan, en orden:

1. Estados.
2. Alfabeto de entrada.
3. Alfabeto de cinta.
4. Estado inicial.
5. Símbolo blanco.
6. Estados de aceptación.
7. Número de cintas.

Las líneas restantes son transiciones.

Después de construir la máquina, el parser llama a `validate()`.

### `CommandLineParser`

Procesa los argumentos:

```text
-config fichero_mt.txt
-in entradas.txt
```

`-config` es obligatorio. `-in` es opcional; si falta, las entradas se leen
desde la consola.

### `InputReader`

Lee una entrada por línea desde un fichero o desde `stdin`. Antes de ejecutar
cada entrada, verifica que todos sus símbolos pertenezcan al alfabeto de
entrada `Sigma`.

### `OutputFormatter`

Transforma el resultado en una salida legible. Muestra:

- `ACCEPTED` o `REJECTED`.
- Estado final.
- Número de pasos.
- Contenido visible de cada cinta.
- Posición del cabezal entre corchetes cuando es representable en el intervalo
  mostrado.

## 4. Validación de la máquina

Antes de ejecutar una entrada se comprueba que:

- Exista al menos un estado.
- El estado inicial pertenezca a `Q`.
- El blanco pertenezca a `Gamma`.
- Todo símbolo de `Sigma` pertenezca a `Gamma`.
- Todo estado de aceptación pertenezca a `Q`.
- Los estados usados en transiciones estén declarados.
- Los símbolos leídos y escritos pertenezcan a `Gamma`.
- Cada transición tenga exactamente un símbolo leído, escrito y movimiento por
  cinta.
- Los movimientos sean `L`, `R` o `S`.
- No haya dos transiciones con el mismo estado y la misma combinación de
  símbolos leídos.

La última comprobación garantiza el determinismo: para una configuración dada
no pueden existir dos reglas diferentes aplicables.

## 5. Pipeline completo

La ejecución normal sigue este recorrido:

```text
argumentos
    |
    v
CommandLineParser
    |
    v
ConfigurationParser
    |
    v
TuringMachine validada
    |
    +--> InputReader
    |       |
    |       v
    |   entradas validas
    |
    v
TuringMachine::run
    |
    +--> initializeTapes
    +--> findApplicableTransition
    +--> executeTransition
    +--> repetir hasta detenerse
    |
    v
SimulationResult
    |
    v
OutputFormatter
```

## 6. Ejemplo de ejecución

Se puede ejecutar la primera máquina con:

```bash
./turing_simulator -config config/machine1.txt -in examples/machine1.txt
```

La máquina de `config/machine1.txt` reconoce cadenas binarias con un número
impar de ceros. Sus estados relevantes son:

- `q0`: se han leído cero o un número par de ceros.
- `q1`: se ha leído un número impar de ceros.
- `q2`: estado de aceptación.

Consideremos la entrada `101`.

### Inicialización

La primera cinta queda así:

```text
posición:  0 1 2 3
cinta:     1 0 1 .
cabezal:   ^
estado:    q0
```

El punto `.` es el blanco y el cabezal comienza en la posición `0`.

### Paso 1

La máquina está en `q0` y lee `1`. Se aplica:

```text
q0 1 q0 1 R
```

No cambia el símbolo, avanza a la derecha y permanece en `q0`.

```text
cinta:     1 0 1 .
              ^
estado:    q0
```

### Paso 2

En `q0` se lee `0`. Se aplica:

```text
q0 0 q1 0 R
```

El número de ceros leídos pasa a ser impar, por lo que la máquina cambia a
`q1`.

```text
cinta:     1 0 1 .
                ^
estado:    q1
```

### Paso 3

En `q1` se lee `1`. Se aplica:

```text
q1 1 q1 1 R
```

La máquina avanza hasta el blanco sin modificar el estado.

```text
cinta:     1 0 1 .
                  ^
estado:    q1
```

### Paso 4

En `q1` se lee el blanco `.`. Se aplica:

```text
q1 . q2 . R
```

La máquina entra en `q2` y avanza una posición.

### Detención y aceptación

En `q2` no hay ninguna transición definida para el siguiente símbolo. Por
tanto, `findApplicableTransition()` devuelve `nullopt` y `run()` termina.
Como `q2` pertenece al conjunto de aceptación, el resultado es:

```text
ACCEPTED
```

El programa también informa del estado final, el número de pasos y el contenido
visible de la cinta.

Si se ejecutase la entrada `010`, se alcanzarían dos ceros y la máquina
terminaría en `q0`, que no es de aceptación. El resultado sería `REJECTED`.

## 7. Compilación y ejecución

Con `Makefile`:

```bash
make
make run-machine1
make run-machine2
make clean
```

El programa se compila con C++17 y los fuentes se encuentran en `src/` con
extensión `.cc`.
