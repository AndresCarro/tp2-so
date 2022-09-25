
- [Diseño](#diseño)
  - [Funciones Kernel](#funciones-kernel)
  - [Funciones Userland](#funciones-userland)
  - [Userland](#userland)
  - [Interrupciones](#interrupciones)
    - [Handler](#handler)
    - [Timer Tick](#timer-tick)
    - [Keyboard](#keyboard)
  - [Syscalls](#syscalls)
    - [Handler](#handler-1)
    - [0 - Read](#0---read)
    - [1 - Write](#1---write)
    - [2 - Get registers](#2---get-registers)
    - [3 - Exec](#3---exec)
    - [4 - Exit](#4---exit)
    - [5 - Time](#5---time)
    - [6 - Copy memory](#6---copy-memory)
  - [Excepciones](#excepciones)
  - [Scheduler](#scheduler)
    - [Estructura](#estructura)
    - [Cargado de Tareas](#cargado-de-tareas)
- [Dificultades Encontradas](#dificultades-encontradas)
  - [Scheduler](#scheduler-1)
    - [Interrupciones](#interrupciones-1)
    - [Pausado de un Programa](#pausado-de-un-programa)
    - [Terminado de un Programa](#terminado-de-un-programa)
  - [Naive Console](#naive-console)
  - [Newline](#newline)
  - [Scroll Up](#scroll-up)
- [Partición del Trabajo](#partición-del-trabajo)
- [Herramientas Utilizadas](#herramientas-utilizadas)
- [Limitaciones del Trabajo](#limitaciones-del-trabajo)

## Diseño

### Funciones Kernel

Decidimos que el Kernel se encargue de poder paralelizar las funciones para el uso del pipe mediante un Scheduler, dado que esto nos permitia un mejor control mediante el uso de interrupciones, y tambien nos permite abstraer de los programas donde deben escribir, pues el Scheduler decide en que lado de la pantalla escriben, las funciones solo saben que estan haciendo un write a `STDOUT`.

Tambien modificamos el naiveConsole original para que los caracters `'\b'` y `'\n'` los interprete como comandos especificos para borrar un caracter y hacer un salto de linea, asi el usuario puede eliminar o saltar con el mismo syscall de write. Aparte de esto, lo modificamos para que pueda dividirse en dos pantallas mediante las funciones `ncWindows`, y que se pueda elegir la pantalla a utilizar mediante `ncCurrentWindow`.

Para poder implementar inforeg, decidimos utilizar la combinación de teclas Ctrl+C, asi mediante la interrupción de teclado ya teniamos los registros lindos para copiar en la memoria, y luego mediante una syscall, el usuario le pasa un vector de `uint64_t` de dieciocho posiciones, para que el Kernel copie los registros a este. Decidimos hacer que el usuario pase un registro para evitar que este tenga un puntero a algún punto de la memoria en el kernel.

El kernel tambien se encarga, de setear la IDT, manejar las interrupciones y las excepciones, y aparte tiene una interrupción para las syscalls, que decidimos fuera la `0x80` para copiar a Linux.

### Funciones Userland

Bash decidimos hacerlo de Userland porque decidimos que no necesitaba acceso a nada más que lo que las syscalls le permiten.

### Userland

Decidimos dejar la dirección de Userland en la dirección en la que vino, es decir, `0x400000`, pero `RSP` lo seteamos a `0x900000`, asi dejamos 1MB para el stack de Userland, respecto de las funciones del Scheduler.

### Interrupciones

Apenas comienza el main del kernel, se setea una IDT de 256 entradas en la posición 0, hasta la posición 4096, con las siguientes entradas:

Primero se desactivan los interrupts, y despues en la posición `0x00`, que es la llamada a la excepción por dividir con 0, se pone su handler, y lo mismo con `0x06`, que es la excepción por invalid opcode. En `0x20` y `0x21`, las interrupciones de timer tick y de teclado, se setean sus handlers. Y por ultimo decidimos utilizar el numero `0x80` para las syscalls, imitando a Linux, por lo que allí pusimos el puntero al handler de las syscalls.

Despues de setear la IDT, cambiamos las mascaras del Master PIC para que atienda las interrupciones por timer tick y keyboard, y despues habilitamos las interrupciones en el procesador.

#### Handler

El handler es el mismo para todas las interrupciones: Pushea todos los registros generales (`RAX` hasta `R15`), y pone en `RDI` el valor de la interrupción, en `RSI` un backup a los registros, y luego llama a un dispatcher que se encarga de llamar a sus respectivos handlers.

Cuando el dispatcher termina su trabajo, el handler le indica al PIC el end of interrupt, restaura los registros y vuelve al lugar donde fue llamado.

#### Timer Tick

El handler de timer tick recive solo el puntero a los backups de los registros, y simplemente realiza dos tareas: Aumenta en uno el contador de ticks, y llama a `nextTask` y le pasa el pointer para que este pueda modificarlos en caso de ser necesario.

#### Keyboard

Cuando se realiza una interrupción de keyboard, se llama a la función `keyboard_handler`, que se encarga de fijarse si se apreto la tecla, si es una letra con un caracter ASCII la agrega al buffer, y en caso contrario no hace nada.

El buffer lo hicimos de 32 posiciones, por lo que solo puede guardar 32 keystrokes antes de limpiarlo. Aún asi se pueden hacer reads de más de 32 por la implementación de readBuffer, que se queda esperando hasta llegar a la cantidad de caracteres pedidos.

***

### Syscalls

Decidimos realizar syscalls de solo cuatro argumentos por simplicidad, dado que las que implementamos no necesitaban nada más.

#### Handler

Al realizar una syscall, el `syscallHandler` pushea al stack todos los registros generales, y ya al momento de llamar a una interrupt con `int` quedan puestos los registros `RIP`, `CS`, `RFLAGS` y `RSP` en ese orden. Luego, movemos `RAX`, el registro donde esta la syscall a desarrollar, a `RCX` y seteamos `R8` por el lugar en espacio donde quedo pusheado el ultimo registro.

Esto lo hacemos especialmente para la syscall `exec` que utiliza un scheduler para "paralelizar" dos procesos, y la syscall `exit`, que es usada por el scheduler para saber cuando terminan los procesos que esta corriendo, y tambien directamente pasa a la proxima tarea, por lo que necesita modificar el contexto de la tarea actual.

Por simplicidad a la hora de modificar los registros en memoria, tambien pusheamos `RAX`, pese a ser un registro que no podemos restaurar pues al salir de una syscall toma el valor de retorno de esta.

En caso de que el numero de syscall no sea valido, simplemente retorna.

#### 0 - Read

La syscall `read` decidimos hacerla igual a la de Linux, por lo que recibe tres parametros, un file descriptor, un pointer a un buffer donde va a guardar las cosas, y un limite de cuanto debe leer.

En nuestro caso, lo unico que puede ser leido es el buffer de teclado, mediante el file descriptor 0, el uso de cualquier otro hace que la syscall inmediatamente termine.

#### 1 - Write

Recibe los mismos argumentos que la de Linux, es decir, un file descriptor, un pointer al buffer, y la cantidad a leer. En este caso, solo se aceptan dos file descriptors, que lo unico que cambian es el color:

1. **STDOUT**: Imprime a la pantalla en color blanco con fondo negro.
2. **STDERR**: Imprime a la pantalla en color rojo con fondo negro.

Al momento de hacer el driver de video, que permite que la consola se divida en dos, debatimos darles file descriptors separados a cada ventana, pero como hicimos un scheduler en el kernel, decidimos que tenia mas sentido que el kernel tomara la decisión de cuando se debe dividir la ventana, y donde tiene que escribir cada programa, pues de esta forma no pueden escribir a la ventana de otro.

#### 2 - Get registers

Esta es la que decidimos hacer para obtener un screenshot de los registros en determinado momento. Decidimos que resiva como argumento un puntero a una posición donde deben entrar los 18 registros de 64 bits que se copian, asi el usuario no tiene acceso al kernel space en memoria, y que retorne 1 si se copiaron los registros y 0 si no habia registros para copiar.

Para hacer posible que se tome una imagen en cualquier momento, decidimos utilizar la combinacion de teclas Ctrl + C. Si no se apretaron en algun momento antes de llamar a get registers, el return va a ser 0.

#### 3 - Exec

La idea de esta syscall es poder simular la paralelización de dos funciones al mismo tiempo, de forma que el usuario no note el hecho de que realmente no estan corriendo al mismo tiempo.

De esta forma, `exec` recibe tres parametros, dos que son seteados por el llamador de la interrupción y otro que agrega el syscall handler que es el pointer a los registros en memoria.

Las funciones que recibe deben ser del tipo `int (*)(void)`, pues decidimos que la complejidad de implementar argumenos era innecesaria teniendo en cuenta que se puede utilizar una variable global para simularlo.

#### 4 - Exit

Esta syscall realmente solo sirve para el Scheduler, pues es usada para que cuando termine uno de los programas en el scheduler, este pueda saber que termino y dejar de alocarle tiempo. Recive el valor de retorno de la función, lo imprime en pantalla, y setea un flag para que el scheduler sepa que termino.

Si se la llama cuando el scheduler no esta funcionando, no hace nada, y como cuando esta funcionando la unica forma en que puede ser llamada es que una de las funciones halla terminado, no se hacen más checkeos.

#### 5 - Time

Recibe como argumento una estructura que debe tener el siguiente formato:

```C
typedef struct {
    uint8_t day, month, year;
    uint8_t hours, minutes, seconds;
} time_t;
```

Luego, devuelve el tiempo en formato ART(UTC-3), pero para el año solo retorna los ultimos dos digitos.

#### 6 - Copy memory

Para poder realizar el printmem del trabajo, decidimos implementar una syscall que recibe la dirección de memoria, un pointer a `uint8_t`, y la cantidad de bytes a copiar.

De esta forma, el kernel copia la cantidad de bytes pedidos de la dirección de memoria a donde apunta el pointer.

***

### Excepciones

Para las excepciones, para poder imprimir los registros decidimos desde assembler pasarle un puntero a donde se pushearon todos los registros apenas empezo la excepción, y un puntero a los valores de RIP y RSP que fueron guardados en el stack cuando se realizo la interrupción, luego se imprime el tipo de error y todos los registros en color rojo.

Luego de imprimirse los registros, para evitar problemas con el Scheduler se encarga de pausear la task que tiro la excepción, dado que si no esta corriendo el Scheduler no hace nada, y despues entra en un loop llamado `haltcpu` que setea los interrupts y luego la instrucción `hlt`. De esta forma, nos aseguramos que cuando una excepción ocurre durante el scheduler, la otra función pueda seguir funcionando.

***

### Scheduler

Luego de mirar al mapa de memoria de Pure64, decidimos que las variables "paralelizadas" empiecen su stack en la posición `0x700000`, con un stepping de `0x100000`. De esta forma, la función en la pantalla izquierda tiene espacio hasta encontrarse con el SampleDataModule, cargado en la dirección `0x500000`, y asumiendo que este va a ocupar menos de 1MB, entonces tiene al menos 1MB para crecer el stack. Lo mismo con la función de la pantalla derecha, solo que esta si tiene 1MB justo, pues si sigue creciendo se encuentra con el stack de la función de la pantalla derecha.

Tambien tenemos un valor inicial default para el registro `RFLAGS` de cada tarea, `0x202`, ya que necesitamos que las interrupciones esten activadas, e Intel requiere que el bit uno este activado.

`Current` indica el indice de la tarea corriendo en el momento, y cuando no esta corriendo el Scheduler vale -1 para indicar que este recien comienza y asi se guarda el contexto del origen en vez de en una de las funciones.

`Amount` indica la cantidad de tareas corriendo, asi el Scheduler sabe cuando debe volver al contexto original. Si vale -1, entonces recien se terminaron las dos tareas mediante el uso de Ctrl+E.

#### Estructura

En el caso del scheduler decidimos que solo se use para simular la paralelización de dos funciones, mediante el uso de una estructura que utilizamos para guardar el contexto de la función que lo llamo en `origin` y el de las funciones corriendo en el vector `tasks`.

Tiene no solo el espacio para los registros, si no que tambien guarda unas variables utilizadas para el control de las tasks:

- `buRIP`: Es un backup del registro `RIP` usado cuando se pausa una de las funciones.
- `window`: Lleva control de en que ventana debe escribir cada tarea.
- `active`: Si esta seteada en 1, entonces la tarea esta activa, si no, esta pausada.
- `finished`: Cuando una tarea termina mediante la syscall exit, se setea en 1 para indicarle al Scheduler que la función termino.

#### Cargado de Tareas

En este caso, dado que el Scheduler se maneja por interrupciones, y tiene que terminar la interrupción antes de poder correr la siguiente tarea, decidimos que cuando se realizan interrupciones tambien se pase la dirección de memoria donde pusheamos todos los registros, entonces la memoria queda:

| Stack  | Posicion |
|--------|----------|
| R15    | RSP      |
| RAX    | RSP+14*8 |
| RIP    | RSP+15*8 |
| CS     | RSP+16*8 |
| RFLAGS | RSP+17*8 |
| RSP    | RSP+18*8 |

De esta forma, al pasar a la proxima tarea solo tenemos que acceder al puntero a R15 (que es el ultimo pusheado) para tratar a todos los registros como un array. Ponemos todos los valores de registros, RIP, RSP y RFLAGS de la tarea actual en una estructura auxiliar, y luego modificamos esas direcciones de memoria por los valores de los registros de la nueva tarea, asi cuando termina la interrupción se vuelven a cargar y vuelve al lugar donde se quedo en la nueva tarea a la que paso.

***

## Dificultades Encontradas

Tuvimos problemas con la implementacion del schelduler, problemas con el manejo de registros que hacian que este falle.

En el keboard driver tuvimos muchos problemas con el buffer resultando que se copiase doble en el buffer o que no se copie nada. Luego de debuggear nos dimos cuenta que el problema estaba em la funcion readBuffer y que podia ser arreglada usando un segundo ciclo (el while(i<count))


### Scheduler

#### Interrupciones

Pese a que el Scheduler funcionaba a la hora de cargar los programas, estuvimos un tiempo largo con el problema de que las interrupciones no funcionaban al entrar en este, y dado que usamos el timer tick para cambiar de tareas, se quedaba siempre en una hasta terminar con la otra.

Despues de recrear el scheduler desde cero, con el mismo problema, fue que nos enteramos de que el problema venia de que como estabamos resguardando el registro `RFLAGS` entre funciones, y el valor que le seteamos por default era cero, las interrupciones no andaban por que el bit de interrupciones quedaba en cero. Despues de ver la estructura del registro `RFLAGS`, decidimos ponerles por valor default `0x202`, pues el bit nueve es el bit de interrupciones, y el bit uno es "Intel reserved" y debe quedar encendido.

#### Pausado de un Programa

Decidimos que cuando pausaramos un programa, decidimos tomar un backup del registro RIP, y despues cambiar el de la función a pausar por uno de una función `haltcpu`, que se encarga de setear los interrupts (por si las dudas), y luego ejecuta el comando `hlt`, asi se queda en su lugar.

El problema con el que nos encontrabamos es que al volver de la otra función a la pausada, se salteaba el hlt y se seguia corriendo codigo sin control, por lo que terminaba tirando excepción de Pure64. Despues nos dimos cuenta que en vez de un solo comando de halt, teniamos que envolverlo en un ciclo incondicional para que se siga realizando.

#### Terminado de un Programa

Al inicio, nos olvidamos de ponerle un valor de retorno en el stack a las funciones que llamabamos, pese a que cambiabamos el valor de `RSP`. Esto, claramente, llevo a errores, asi que finalmente decidimos hacer una función de exit default.

Tambien, como decidimos utilizar los registros guardados en el stack a la hora de hacer una syscall o interrupción para modificar el contexto, en vez de hacer un loop similar al `haltcpu` que usamos para pausar las funciones, decidimos simplemente hacer un syscall exit, que es especificamente para el Scheduler, para que termine en el momento.

### Naive Console

### Newline

Tomo un rato conseguir una función de newline que funcionara en el modo de dos ventanas, pues teniamos que asegurarnos que estuviera limitada a la pantalla correcta.

### Scroll Up

Despues de un rato de intentar crear cosas nuevas, nos dimos cuenta que podiamos reutilizar el codigo que usamos para el newline de pantallas separadas y modificarlo para que en vez de reemplazar caracteres por `' '`, los reemplazara por los de la proxima fila.

## Partición del Trabajo

Keyboard Driver: Juan Castro y Andres Carro Wetzel

Naive Console: Juan Castro y Matias Wodtke

Syscalls: Entre todos

Interupciones: Juan Castro

Schelduler: Juan Castro

Bash: Juan Segundo Arnaude, Matias Wodtke y Andres Carro Wetzel

Syslib: Juan Segundo Arnaude, Matias Wodtke y Andres Carro Wetzel

Las otras partes del trabajo fueron hechas entre todos.

## Herramientas Utilizadas

Para poder trabajar de forma grupal asincronicamente, usamos Git para llevar control de lo que ibamos cambiando, y GitHub para sincronizar lo que teniamos instantaneamente.

Tambien hicimos uso de Discord para juntarnos de forma remota y debatir sobre las deciciones a tomar.

## Limitaciones del Trabajo

El Scheduler no es realmente util más que para completar con lo pedido por el enunciado, ni siquiera es capaz de recibir argumentos para las funciones que corre, a parte de que no corre programas completos, si no que el usuario le debe mandar un lugar donde ya este cargada en la memoria.
