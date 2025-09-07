# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

Fausto Manrique: Trabajo conjunto en desarrollo del modelo físico. Desarrollo del sistema de
generación de texto para los cuerpos de la simulación. Reformateo y estilo de los archivos de texto
Documentación y redacción del archivo README.

Alex Petersen: Trabajo conjunto en desarrollo del modelo físico. Desarrollo de procesamiento de 
gráficos dependiendo de la distancia. Integración de asteroides en la simulación. 
Debugging del modelo físico. Implementación de sistema de pausado y avance rápido de la simulación.

## Verificación del timestep

Implementando el timestep predeterminado por la cátedra (100 días por segundo de simulación), el 
comportamiento de la simulación es consistente con lo esperable según el modelo físico 
preestablecido. Experimentando con distintos valores, vemos que cuando el timestep supera el valor
de 500000 (aproximadamente 600 días por segundo), la simulación pasa a mostrar resultados erráticos,
como por ejemplo, asteroides saliendo de órbita y alejándose hacia el infinito. Esto se debe a que,
al aumentar el timestep, la precisión de la simulación disminuye por utilizar la misma aceleración
durante un intervalo de tiempo mayor, lo cual genera que los cuerpos se muevan en la misma dirección por 
más tiempo. 

## Verificación del tipo de datos float

Para los cálculos realizados en la simulación, se optó por mantener el tipo de dato float, dado que la
precisión que este ofrece es suficiente para el correcto funcionamiento de la simulación. Sin embargo,
hubo inconvenientes al intentar calcular las fuerzas entre cuerpos, dado que la multiplicación entre 2
masas generaba un overflow en la variable (10^24 * 10^24 > 10^38). Para evitar este problema, se evitó multiplicar las masas
omitiendo la división por la masa del cuerpo que recibe la fuerza calculada, obteniendo el mismo resultado
sin tener que utilizar variables de tipo double. Además de optimizar el uso de memoria, esto facilitó el
uso de la estructura Vector3 de Raylib, que utiliza el tipo float para realizar operaciones, evitando 
posibles pérdidas de información al castear double a float.

## Complejidad computacional con asteroides

Los principales inconvenientes al momento de implementar asteroides fueron la cantidad de fuerzas a calcular
en cada timestep, y la cantidad de esferas que debían ser dibujadas en pantalla.

Si calculamos las fuerzas de cada cuerpo con todos los otros, incluyendo los asteroides, la complejidad
computacional del programa es de O(n*(n-1)), ya que no se toma la interacción de un cuerpo consigo mismo.
Esta complejidod se puede aproximar a O(n^2) para valores altos de n. Esta complejidad es demasiado alta
para poder realizar una simulación con más de 1000 asteroides, ya que se tendrían que realizar más de 
1.000.000 de cálculos de fuerza por timestep. En el caso de la aceleración, velocidad y posición, 
estas se calculan en O(n) para n cuerpos, por lo que no representan un problema significativo en el
rendimiento de la simulación.

En relación a los gráficos, el programa se vuelve muy lento al tener que dibujar más de 500 esferas en
pantalla, ya que cada esfera en 3D requiere de varios polígonos para ser dibujada correctamente. Este 
problema ralentizaba la simulación incluso más que el cálculo de fuerzas, por lo que era necesario 
realizar cambios para mejorar el rendimiento al agregar asteroides. 

## Mejora de la complejidad computacional

Como se planteó en el apartado anterior, se abordaron dos problemas principales para mejorar el
rendimiento: el cálculo de fuerzas y el renderizado de los asteroides.

En relación al cálculo de fuerzas, se decidió que para todos los cuerpos se calculen solo las fuerzas
ejercidas por los cuerpos de masa significativa del sistema (>1E20F), ya que la fuerza ejercida por un 
asteroide sobre otros cuerpos se puede considerar despreciable, debido a la gran diferencia de masa entre 
ambos. Debido a esto, las fuerzas calculadas en cada timestep no sufren una variación significativa.
Además, para los asteroides se optó por que solo se calcule la fuerza ejercida por el objeto más masivo 
del sistema, que se define al inicializar la simulación (el sol en caso de no alterar nignuna masa). 
Esta última medida ciertamente reduce la precisión de la simulación para los asteroides, pero dada 
la cantidad de asteroides que se deben procesar, es una pérdida aceptable. Si consideramos los 2 cambios 
principales mencionados, para los asteroides se calcula solo una fuerza, mientras que para los 
cuerpos masivos se calculan fuerzas según la cantidad de cuerpos masivos que haya. Esto disminuye 
la complejidad computacional a O((n-k)*(n-k-1)+k), donde n es la cantidad total de cuerpos y k la 
cantidad de asteroides. Para casos donde k es mucho mayor que n-k, esto se puede aproximar a O(n).

Por otro lado, para mejorar el procesamiento gráfico de la simulación se utilizó principalmente la función
DrawSphereWires() de Raylib, que renderiza una esfera con la cantidad de polígonos que se le introduzca como
parámetro a la función. Esto se combinó con una función lineal que calcula la cantidad de wires a utilizar
según la distancia a la que se encuentre cada cuerpo de la cámara. Cuanto más lejos está un cuerpo de la 
cámara, menor será la calidad de la esfera. Esto implicó una mejora sustancial en el rendimiento del programa.
Además, se escribió la función IsEntityInView(), que determina si un objeto está en frente de la cámara y 
dentro de su ángulo de FOV para renderizar los objetos. Esto se hizo utilizando el producto escalar de los 
vectores definidos por el objetivo de la cámara y la posición del objeto respecto de la cámara. Estos dos 
cambios redujeron significativamente la cantidad de cuerpos a procesar, sobre todo cuando se posiciona la
cámara cerca del centro del sistema solar, reduciendo la cantidad de cuerpos a casi un tercio del total.

Estas mejoras lograron un aumento significativo en el rendimiento de la simulación. Al principio la simulación
funcionaba a menos de 10 FPS a partir de 200 asteroides, y luego se logró mantener entre 50 y 60 FPS 
con más de 1000 asteroides en pantalla, con una precisión aceptable para el modelo físico.

## Bonus points

Ajustes de masa: La masa de un agujero negro puede variar por múltiples órdenes de magnitud. De 
añadir uno "pequeño" de 8 masas solares, todo el sistema solar tarda menos de un fotograma en 
desaparecer de la vista, mientras que de alterarse la masa de Júpiter, las órbitas de los cuerpos
que interactúan con él(todos menos los asteroides) se deforman considerablemente y pasa a formarse
un sistema complejo de 2 cuerpos entre el planeta(cuya masa ahora se asemeja más a la de una 
estrella) y el sol. Al aumentar mucho la masa de un cuerpo, se puede apreciar el "horizonte de sucesos"
que genera ese cuerpo a su alrededor como una esfera donde no hay cuerpos, donde si otro cuerpo entra 
sale disparado instantaneamente hacia el infinito (ya que no hay colisiones, sino se uniria al 
cuerpo de mayor masa).

Easter egg: En el archivo de orbitalSim.cpp, aparece un comentario que indica que phi = 0. Cuando 
se quita el comentario, los asteroides se generan en un mismo plano respecto del sol y comienzan 
a orbitar el cuerpo más masivo del sistema.

Adicionalmente, se agregó una funcio que muestra el nombre de los cuerpos celestes nominados
sobre su imagen en la simulación, utilizando como parámetros de computación la posición de la 
cámara, su orientación y la posición de cada cuerpo respecto de ella.

Otra funcion adicional agregada al programa fue la posibilidad de pausar la simulación usando la tecla
'p' y acelerarla por un factor de 10 presionando la tecla 'f'.
