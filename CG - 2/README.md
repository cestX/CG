

# Trabalho 2 - Implementação do Pipeline Gráfico

<p>O presente trabalho tem como objetivo realizar a implementação de todos os passos constituintes de um pipeline gráfico,
desde a descrição dos vértices no espaço do objeto, passando pelo espaço do universo, da câmera, pelo espaço de projeção,
de recorte até chegar na sua rasterização no espaço de tela. Para isso, será utilizado um objloader fornecido pelo professor
da disciplina, Christian Azambuja Pagot, e o algoritmo de rasterização de primitivas implementado no exercício anterior.</p>

<h3>TRANSFORMAÇÕES AO LONGO DO PIPELINE GRÁFICO </h3> 


<p>Para criar uma representação raster 2D de uma cena 3D é necessário que seja seguida uma sequência
de passos que caracterizam o pipeline gráfico, levando o objeto de um espaço para outro até que ele
seja rasterizado na tela. A sequência desses espaços é exibida na figura 1 e descrita logo em seguida.</p>

![alt text](https://i.imgur.com/YGpIZrW.jpg "Espaços")


Espaço do Objeto → Espaço do Universo
====

<p>Compreende o espaço onde são descritas as coordenadas do objeto bem como todas as transformações geométricas realizados sob o mesmo. Essas transformações são:</p>

<h3>ESCALA</h3>

![alt text](https://i.imgur.com/Kbf8Cts.png "Escala")

<h3>SHEAR</h3> 

![alt text](https://i.imgur.com/7XbAu3X.png "Shear")

<h3>TRANSLAÇÃO </h3>

![alt text](https://i.imgur.com/Lzs1EqI.png "Translação")

<h3>ROTAÇÃO</h3>

![alt text](https://i.imgur.com/9jZdRO0.png "Rotação")


<p>Como visto acima, cada transformação possui sua matriz e o produto de todas as matrizes das transformações aplicadas no objeto gera a Matriz Model, que leva o objeto do Espaço do Objeto para o Espaço do Universo. Se o objeto em questão não passar por nenhuma transformação, essa matriz é a própria identidade. Vale salientar que, como a matriz de translação é uma transformação afim e não pode ser escrita em forma de matriz, é preciso criar um espaço homogêneo com valor 1 e para passar o espaço euclidiano para o espaço homogêneo multiplicamos as coordenadas do vetor por w, onde w é a coordenada homogênea.</p>




Espaço do Universo → Espaço de Câmera
====

<p>No espaço do Universo, se encontram todos os objetos após aplicadas as suas transformações. Nele, são definidas as coordenadas da câmera (posição da câmera, vetor de direção e direção do topo da câmera) para indicar de onde serão visualizados os objetos. A construção da Matriz View, que leva a o objeto do espaço do universo para o espaço da câmera é composta pelos valores da posição da câmera (Xc, Yc, Zc) e podem ser encontrados da seguinte forma: </p>

<p>Para encontrarmos o Zc, é preciso calcular a diferença existente entre a posição da câmera e a direção para onde a câmera está olhando e dividir pela norma dessa diferença, garantindo assim que Zc é um vetor unitário.</p>

```
Zc =(camera_posição – camera_lookat) / norm(camera_posição – camera_lookat);
```

<p>Para calcularmos o Xc calculamos o produto vetorial entre a direção do topo da câmera o up da câmera e a direção para onde a câmera está olhando. Depois, assim como fizemos com o Zc, dividimos pela norma desse produto  para garantir que seja um vetor unitário.</p>

```
Xc= cross(camera_up, z_camera) / norm(cross(camera_up, z_camera));
```


<p>E para encontrar o Yc, calculamos o produto vetorial entre o Zc e o Xc seguindo a logica anterior, dividimos pela norma desse produto. </p>
 
```
Yc = cross(z_camera, x_camera) / norm(cross(z_camera, x_camera));
```

<p>Encontrados esses valores, podemos construir a Matriz que leve os vértices do espaço universo para o espaço da câmera. Mas antes, temos que lembrar que a matriz deve ser transposta. Além disso, nem sempre o sistema de coordenadas do universo é o mesmo da câmera. Por isso é necessário transladar a câmera para a origem do sistema de coordenadas para alinhar a câmera com o universo, e essa translação equivale a posição da câmera. Logo, nossa Matriz View constituída uma rotação e uma translação.</p>



Espaço câmera → Espaço de Recorte
====

<p>Nesse espaço, os pontos no espaço da camera serão transformados para o espaço projetivo através da multiplicação dos
vértices descritos no espaço da câmera pela  Matriz Projection. Como a câmera está apontada para um ponto específico
da cena, há uma distorção perspectiva onde os objetos próximos se tornam maiores que os mais distantes. Ao posicionarmos
uma câmera, penas os objetos contidos entre esse ângulo poderão ser renderizados posteriormente. Vale lembrar que nessa
transformação, a coordenada w é alterada.</p>

<p>Para a construção dessa Matriz de Projeção, utilizamos a distância entre a câmera e o view plane.</p>


Espaço de Recorte → Espaço Canônico
====

<p>Para causar uma distorção de perspectiva na cena e a sensação de profundidade, é preciso que realizemos a homogeneização
dos vértices, dividindo todos eles pela coordenada homogênea w, mudando assim a geometria na cena e fazendo com que os
objetos próximos da câmera ficam maiores, e os mais afastados ficam menores.</p>



Espaço Canônico → Espaço de Tela
====
<p>Agora, os objetos poderão ser representados em pixels utilizando, para isso, o código da atividade anterior de rasterização
de primitivas. Como no espaço de tela o início do sistema de coordenadas está localizado no canto superior esquerdo, algumas
transformações são necessárias: uma escala anisotrópica do objeto para inverter o eixo y de modo que o objeto apareça no local
correto, uma translação de 1 nos eixos x e y para garantir que os valores das coordenadas sejam maiores ou iguais a zero e por
últimos fazemos uma escala em x com [(largura da tela -1/)2] e no  y[(altura da tela-1)/2].</p>













