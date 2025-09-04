# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

Fausto Manrique: Trabajo conjunto en desarrollo del modelo físico. Desarrollo del sistema de
generación de texto para los cuerpos de la simulación. Reformateo y estilo de los archivos de texto
Documentación y redacción del archivo README.

Alex Petersen: Trabajo conjunto en desarrollo del modelo físico. Desarrollo de procesamiento de 
gráficos. Integración de asteroides en la simulación. Debugging del modelo físico. Implementación 
de sistema de pausado y acelerado de la simulación.

## Verificación del timestep

Implementando el timestep predeterminado por la cátedra (100 días por segundo de simulación), el 
comportamiento de la simulación es consistente con lo esperable según el modelo físico 
preestablecido. Experimentando con distintos valores, vemos que cuando el timestep supera el valor
de 500000 (aproximadamente 600 días por segundo), la simulación pasa a mostrar resultados erráticos,
como por ejemplo, asteroides saliendo de órbita y alejándose hacia el infinito. 

## Verificación del tipo de datos float

Los datos tipo float tienen una precisión de hasta 7 dígitos significativos, lo cual puede generar 
imprecisiones. De modificarse los datos a double, la precisión aumenta a aproximadamente 16 dígitos
significativos. Al correr el programa, no se comporta de manera adecuada, dado que en el cálculo de 
aceleración, donde se hace un producto entre las masas de los cuerpos, la operación genera un 
overflow de la variable. Para mantener la consistencia y optimizar memoria, se optó por modificar el 
orden en el que se procesa la información de la cuenta de cálculo de aceleración para evitar 
multiplicar masas entre sí y así evitar el error, y por lo tanto evitar realizar el cambio a tipo de 
dato double.

## Complejidad computacional con asteroides

La complejidad computacional del algoritmo cuando se calcula la interacción entre todos los 
cuerpos, sean asteroides o cuerpos de mayor masa es de O(n^2), lo cual resulta imposible para una
simulación de 1000 cuerpos. Nuestra medida inicial para resolver este problema es implementar una
restricción que impida a los cuerpos computar la fuerza ejercida en ellos por un asteroide (dado 
que la masa de los asteroides es al menos 8 órdenes de magnitud menor a la de los demás cuerpos), 
previo a cualquier prueba con asteroides ya configurados. Esto nos permite reducir la complejidad
a O(n), siendo n la cantidad de cuerpos de la simulación. En el caso del sistema solar, dado que se
computan 9 cuerpos celestes de masa significativa, esto reduce la complejidad del algoritmo a 
O(9+[número de asteroides]), que es una mejora significativa a comparación del O(n^2) original.

## Mejora de la complejidad computacional

Luego de implementar los asteroides, la simulación sigue siendo demasiado lenta, por lo que se 
decidió modificar el procesamiento de los asteroides, haciendo que estos solamente procesen su
aceleración en función del objeto más masivo, que es el sol. Esto redujo la complejidad aún más,
y dado que el número de cuerpos de masa significativa es más de 2 órdenes de magnitud menor al de 
los asteroides, la complejidad se aproxima a ser de O(9). Además, se cambió la forma de dibujar
los asteroides, usando la función DrawSphereWires en lugar de DrawSphere en los casos donde estos 
se encuentran a una distancia media de la cámara, y dibujándolos como puntos cuando se encuentran
muy lejos de esta, marcando otra optimización considerable en el uso de recursos gráficos. Habiendo
implementado esto, la simulación alcanza los 60 FPS con más de 1000 asteroides al mismo tiempo, lo 
cual cumple con el requisito planteado por la consigna. 

## Bonus points

Easter egg: En el archivo de orbitalSim.cpp, aparece un comentario que indica que phi = 0. Cuando 
se quita el comentario, la simulación se rompe, haciendo que los asteroides se generen en una línea
recta y luego salgan disparados de la simulación.

Adicionalmente, se agregó una funcionalidad que muestra el nombre de los cuerpos celestes nominados
sobre su imagen en la simulación, utilizando como parámetros de computación la posición de la 
cámara, su orientación y la posición de cada cuerpo respecto de ella.

Otra funcionalidad agregada por nosotros fue la posibilidad de pausar la simulación usando la tecla
'p' y acelerarla por un factor de 10 presionando la tecla 'f'.
