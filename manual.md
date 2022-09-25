- [Bash](#bash)
  - [Help](#help)
  - [Fibonacci](#fibonacci)
  - [Prime](#prime)
  - [Inforeg](#inforeg)
  - [Divide by Zero](#divide-by-zero)
  - [Invalid Opcode](#invalid-opcode)
  - [Printmem](#printmem)
  - [Time](#time)
  - [Pipe](#pipe)
- [Syscalls](#syscalls)
  - [Read](#read)
  - [Write](#write)
  - [getRegisters](#getregisters)
  - [Exec](#exec)
  - [Exit](#exit)
  - [Time](#time-1)
  - [Copy memory](#copy-memory)

## Bash

### Help

Imprime un mensaje indicando los comandos y lo que realizan.

### Fibonacci

Imprime los numberos de la serie de fibonacci, hasta que ocurre overflow.

### Prime

Calcula e imprime los numeros primos, hasta que ocurre overflow.

### Inforeg

Al apretar las teclas Ctrl+C, se guardan los registros en el momento, y al llamar a printreg, estos se imprimen. Si no se apretaron antes, da un aviso de que todavia no fueron guardados.

### Divide by Zero

Un programa que hace una division por cero para testear las excepciones.

### Invalid Opcode

Trata de procesar una instrucción erronea para testear las excepciones.

### Printmem

Recibe un address escrito en formato "printmem0x------------------", y lee de esa ubicación de memoria e imprime los 32 bytes siguientes.

### Time

Imprime el tiempo local en pantalla

### Pipe

Sirve para paralelizar dos funciones. Mediante el uso de Ctrl+L se puede parar la función de la ventana izquierda, y usando Ctrl+R se para la de la ventana derecha.

Con Ctrl+E se terminan ambas funciones y vuelve al bash.

## Syscalls

|Numero|Nombre|`RAX`|`RDI`|`RSI`|`RDX`|
|-|-|-|-|-|-|
|0|read|0x00|`unsigned int` fd|`char *` buffer|`uint64_t` count|
|1|write|0x01|`unsigned int` fd|`const char * buffer`|`uint64_t` count|
|2|getRegisters|0x02|`uint64_t*`|-|-|
|3|exec|0x03|`int(*)()` f1|`int(*)()` f2|-|
|4|exit|0x04|`int` ret|-|-|
|5|time|0x05|`time_t *` time|-|-|
|6|copymem|0x06|`uint64_t` address|`uint8_t *` pointer| `uint64_t` length|

### Read

La syscall de read recive un file descriptor, que debe ser 0, pues es el unico input activado por el momento. Este se encarga de leer del teclado hasta que se llena el buffer con `count` elementos.

### Write

Hay dos posibles file descriptors:

|fd|Nombre|Función|
|-|-|-|
|1|STDOUT|Escribe a la pantalla en letras grises con fondo negro|
|2|STDERR|Escribe a la pantalla en letras rojas con fondo negro|

Luego, recibe un vector de `char`s, del que imprime los primeros `count` elementos.

### getRegisters

Esta syscall recibe en `RDI` un puntero a un vector que debe tener suficiente espacio para guardar los 18 registros de 64 bits. Si copio los registros a ese vector, devuelve 1.

Antes de llamarlo, en algún momento se deben apretar las letras Ctrl+C, así se guardan los registros del momento. En caso contrario, va a devolver 0.

### Exec

Mediante esta syscall se pueden paralelizar dos funciones, partiendo la pantalla en dos. Para que funcionen como es esperado, su stack debe ser menor a 1MB. No pueden recibir argumentos, pero es posible evitar esta restricción mediante el uso de una variable global.

Para terminar correctamente, las funciones deberian realizar una syscall exit, aunque a todas se les asigna una dirección de retorno que se encarga de realizarlo automaticamente.

Mediante la combinación de teclas Ctrl+L se pausa y resume la pantalla izquierda, mientras que utilizando Ctrl+R se pausa y resume el de la pantalla derecha.

En caso de que una de las funciones tire excepción, y la otra termine, el programa se quedara en ese estado hasta que el usuario apriete Ctrl+E, donde se terminara la ejecuión de ambas y se volvera a la función que lo llamo originalmente.

### Exit

Exit es exclusivamente para uso junto a `Exec`, en caso de ser usada desde cuando no hay funciones siendo paralelizadas no hara nada.

Cuando se usa correctamente, simplemente recive un valor de retorno que imprime a pantalla sobre la que esta corriendo esa función en especifico.

### Time

Imprime el tiempo en zona horaria ART (UTC-3) en pantalla, imprimiendo primero la hora en formato HH:MM:SS, y luego la fecha en DD/MM/AAAA.

Para obtenerlo, se le debe pasar un puntero a una estructura en especifico:

```C
typedef struct {
    uint8_t day, month, year;
    uint8_t hours, minutes, seconds;
} time_t;
```

### Copy memory

Copia la cantidad de bytes pedidos desde la dirección de memoria al pointer, que debe tener el tamaño apropiado para guardar esa cantidad de bytes.
