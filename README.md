# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

* [Nombre]: [contribución]

[completar]

## Verificación del timestep

[completar]

## Verificación del tipo de datos float

[completar]

## Complejidad computacional con asteroides

La complejidad computacional del algoritmo cuando se calcula la interacción entre todos los cuerpos, sean asteroides
o cuerpos de mayor masa es de O(n^n), lo cual resulta imposible para una simulación de 1000 cuerpos. Nuestra medida 
inicial para resolver este problema es implementar una restricción que impida a los cuerpos computar la fuerza 
ejercida en ellos por un asteroide (dado que la masa de los asteroides es al menos 8 órdenes de magnitud menor a la 
de los demás cuerpos), previo a cualquier prueba con cuerpos ya configurados. Esto nos permite reducir la complejidad
a O(n^n-k), siendo n la cantidad de cuerpos de la simulación y k el número de asteroides que son parte de dicho 
conjunto n. En el caso del sistema solar, dado que se computan 9 cuerpos celestes de masa significativa, esto reduce
la complejidad del algoritmo a O(n^9), que es una mejora significativa a comparación del O(n^n) original

## Mejora de la complejidad computacional

[completar]

## Bonus points

Easter egg: En el archivo de orbitalSim.cpp, aparece un comentario que indica que phi = 0
