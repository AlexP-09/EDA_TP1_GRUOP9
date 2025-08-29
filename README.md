# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

* [Nombre]: [contribución]

Fausto Manrique: Trabajo conjunto en desarrollo del modelo físico. Desarrollo del sistema de
generación de texto para los cuerpos de la simulación. Reformateo y estilo de los archivos de texto
Documentación y redacción del archivo README.

Alex Petersen: Trabajo conjunto en desarrollo del modelo físico. Desarrollo de procesamiento de 
gráficos. Integración de asteroides en la simulación. Debugging de 

## Verificación del timestep

[completar]

## Verificación del tipo de datos float

Los datos tipo float tienen una precisión de hasta 7 dígitos significativos, lo cual puede generar 
imprecisiones. De modificarse los datos a double, la precisión aumenta a aproximadamente 16 dígitos
significativos. Cuando se agregan asteroides a la simulación, los resultados 

## Complejidad computacional con asteroides

La complejidad computacional del algoritmo cuando se calcula la interacción entre todos los 
cuerpos, sean asteroides o cuerpos de mayor masa es de O(n^n), lo cual resulta imposible para una
simulación de 1000 cuerpos. Nuestra medida inicial para resolver este problema es implementar una
restricción que impida a los cuerpos computar la fuerza ejercida en ellos por un asteroide (dado 
que la masa de los asteroides es al menos 8 órdenes de magnitud menor a la de los demás cuerpos), 
previo a cualquier prueba con asteroides ya configurados. Esto nos permite reducir la complejidad
a O(n^n-k), siendo n la cantidad de cuerpos de la simulación y k el número de asteroides que son 
parte de dicho conjunto n. En el caso del sistema solar, dado que se computan 9 cuerpos celestes de
masa significativa, esto reduce la complejidad del algoritmo a O(n^9), que es una mejora 
significativa a comparación del O(n^n) original.

## Mejora de la complejidad computacional

[completar]

## Bonus points

Easter egg: En el archivo de orbitalSim.cpp, aparece un comentario que indica que phi = 0. Cuando 
se quita el comentario, la simulación se rompe, haciendo que los asteroides se generen en una línea
recta y luego salgan disparados de la simulación.

Adicionalmente, se agregó una funcionalidad que muestra el nombre de los cuerpos celestes nominados sobre su imagen 
en la simulación, utilizando como parámetros de computación la posición de la cámara, su orientación y la posición de
cada cuerpo respecto de ella. 
