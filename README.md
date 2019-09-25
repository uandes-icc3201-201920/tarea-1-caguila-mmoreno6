# Informe tarea 1.2

1. Carlos Aguila
2. Martin Moreno



### *Introducción* 

En el siguente informe veremos los puntos mas relevantes de la tarea que acabamos de realizar.
La idea principal de esta tarea es la _**concurrencia para nuestra base de datos no relacional**_, usando _**unix domain socket**_ , como medio de comunicación entre los distintos procesos.

1. cliente(s)
2. servidor



### *Dificultades*
    
Después de discutir sobre el código llegamos al acuerdo de que las dificultades principales fueron 2
 1. concurrencia de procesos
 2. contador global compartido

**concurrencia de procesos** al ser primera vez que programabamos un servidor con acceso a multiples clientes al mismo tiempo, tuvimos que hacer un estudio profundo de esto en internet antes de empezar nuestra tarea.

**contador globar compartido** fue un desafio ya que al ser una variable compartida por distintos threads, se podían generar ciertos problemas que fueron arreglados.



 ### *Funciones no realizadas*

Manejo de errores de algunos tipos inputs



 ### *Problemas conocidos*
          
Algunos tipos de inputs mal ingresados pueden generar problemas con el programa.
Por ejemplo: 
```
insert(45,
update(dfg
get(344,6)
```
