# Trabalho 2 - Implementação do Pipeline Gráfico

<p>O presente trabalho tem como objetivo realizar a implementação de todos os passos constituintes de um pipeline gráfico,
desde a descrição dos vértices no espaço do objeto, passando pelo espaço do universo, da câmera, pelo espaço de projeção,
de recorte até chegar na sua rasterização no espaço de tela. Para isso, será utilizado um objloader fornecido pelo professor
da disciplina, Christian Azambuja Pagot, e o algoritmo de rasterização de primitivas implementado no exercício anterior.</p>

<h3>TRANSFORMAÇÕES AO LONGO DO PIPELINE GRÁFICO </h3> 


<p>Para criar uma representação raster 2D de uma cena 3D é necessário que seja seguida uma sequência
de passos que caracterizam o pipeline gráfico, levando o objeto de um espaço para outro até que ele
seja rasterizado na tela. A sequência desses espaços é exibida na figura 1 e descrita logo em seguida.</p>

![alt text](https://i.imgur.com/AKk1hg7.jpg "Pipeline")


Espaço do Objeto → Espaço do Universo
====

<p>Compreende o espaço onde são descritas as coordenadas do objeto bem como todas as transformações geométricas realizados sob o mesmo. Essas transformações são:</p>

<h3><b>ESCALA</b></h3>
<p> Consiste em mudar a dimenção de uma imagem, fazendo com que ela mude de tamanho. Para isso, os valores de suas coordenadas são multiplicados por um fator escala. Essa transformação pode ser escrita através da seguinte matriz:</p>

![alt text](https://i.imgur.com/cPR0Y6G.jpg "Escala")

<h3><b>SHEAR</b></h3>
<p> Transformação que desloca cada ponto em uma direção fixada. </p>

![alt text](https://i.imgur.com/cPR0Y6G.jpg "Shear")

<h3><b>TRASLAÇÃO</b></h3>
<p> Transformação que altera a posição dos pontos através da soma do vetor de deslocamento às suas coordenadas. Pode ser descrito pela seguinte matriz: </p>

![alt text](https://i.imgur.com/BtcaEyI.jpg "Translação")

<h3><b>ROTAÇÃO</b></h3>

<p> Consiste em um giro de um determinado ângulo em torno de um ponto de modo que a distância entre eles não é alterada. Pode ser representada através da seguinte matriz:</p>

![alt text](https://i.imgur.com/WNdacIA.jpg "Rotação")

Em 3D, as transformações tem suas matrizes alteradas com o acréscimo da coordenada Z. No caso da rotação, a matriz varia de acordo com o eixo a ser rotacionado. 

<p>Como visto acima, cada transformação possui sua matriz e o produto de todas as matrizes das transformações aplicadas no objeto gera a Matriz Model, que leva o objeto do Espaço do Objeto para o Espaço do Universo. Se o objeto em questão não passar por nenhuma transformação, essa matriz é a própria identidade. Vale salientar que, como a matriz de translação é uma transformação afim e não pode ser escrita em forma de matriz, é preciso criar um espaço homogêneo com valor 1 e para passar o espaço euclidiano para o espaço homogêneo multiplicamos as coordenadas do vetor por w, onde w é a coordenada homogênea.</p>

![alt text](https://i.imgur.com/ie7KKob.jpg "Matriz View")


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

![alt text](https://i.imgur.com/HXd4OHA.jpg "Espaço Universo --> Espaço Camera")

<p>Encontrados esses valores, podemos construir a Matriz que leve os vértices do espaço universo para o espaço da câmera. Mas antes, temos que lembrar que a matriz deve ser transposta. Além disso, nem sempre o sistema de coordenadas do universo é o mesmo da câmera. Por isso é necessário transladar a câmera para a origem do sistema de coordenadas para alinhar a câmera com o universo, e essa translação equivale a posição da câmera. Logo, nossa Matriz View constituída uma rotação e uma translação. É nesse momento em que todos os vértices passarão por ela de modo a serem representados no espaço da câmera, primeiro definimos a posição da câmera e para onde ela está olhando usando o sistema de coordenadas do universo. No código isso é representado pela função abaixo: </p>

```c++
    myglLookAt (
        0.0f. 0.0f, 5.0f, //Posição da Câmera
        0.0f, 0.0f, 0.0f,  //LookAt
        0.0f, 1.0f, 0.0f   //Vetor Up
    );

```

Espaço câmera → Espaço de Recorte
====

<p>Nesse espaço, os pontos no espaço da camera serão transformados para o espaço projetivo através da multiplicação dos
vértices descritos no espaço da câmera pela  Matriz de Projeção. Como a câmera está apontada para um ponto específico
da cena, há uma distorção perspectiva onde os objetos próximos se tornam maiores que os mais distantes. Ao posicionarmos
uma câmera, penas os objetos contidos entre esse ângulo poderão ser renderizados posteriormente. Vale lembrar que nessa
transformação, a coordenada w é alterada.</p>

![alt text](https://i.imgur.com/wY5OfyV.jpg "Espaço Camera --> Espaço Recorte")

<p>Para a construção dessa Matriz de Projeção, utilizamos a distância entre a câmera e o view plane.</p>

![alt text](https://i.imgur.com/Wldfz1Z.jpg "Matriz de Projeção")


Espaço de Recorte → Espaço Canônico
====

<p>Para causar uma distorção de perspectiva na cena e a sensação de profundidade, é preciso que realizemos a homogeneização
dos vértices, dividindo todos eles pela coordenada homogênea w, mudando assim a geometria na cena e fazendo com que os
objetos próximos da câmera fiquem maiores, e os mais afastados fiquem menores.</p>



Espaço Canônico → Espaço de Tela
====

![alt text](https://i.imgur.com/EDHmHy2.jpg "Tela")


<p>Agora, os objetos poderão ser representados em pixels utilizando, para isso, o código da atividade anterior de rasterização
de primitivas. Como no espaço de tela o início do sistema de coordenadas está localizado no canto superior esquerdo, algumas
transformações são necessárias: uma escala anisotrópica do objeto para inverter o eixo y de modo que o objeto apareça no local
correto, uma translação de 1 nos eixos x e y para garantir que os valores das coordenadas sejam maiores ou iguais a zero e por
últimos fazemos uma escala em x com [(largura da tela -1/)2] e no  y[(altura da tela-1)/2].</p>


![alt text](https://i.imgur.com/LL8VUnb.jpg "Matriz para Espaço de Tela")

<h3> Comparação entre OBJLoader e My OpenGL </h3>

![alt text](https://i.imgur.com/UtlRqFZ.png "Comparação entre MyOpenGL e OBJLoader")


<h3> Pipeline Rodando </h3>

![Monkey](https://i.imgur.com/yrdBnM7.gif)


<p> 1 - O link do video referente ao gif <a target="_blank" href="https://youtu.be/rbGPExuxl9I"> Monkey OpenGL</a> 


<h3> Comentários e Dificuldades </h3>
<p> Nossa maior dificuldade na implementação a prinipio o foi no processamento do arquivo .obj. Quanto às Matrizes de transformação, a dificuldade foi entender a parte algébrica para levá-las de um espaço a outro, mas depois de ver os padrões nestas transformações, não houveram mais problemas. Outro problema no início foi usar o blender para modelar. Tentamos modelar nossos próprios modelos para carregar no pipeline mas não obtivemos êxito.</p>


<h3>Fontes</h3> 
<p> 1 - Slides Professor <a target = "_blank" href = "https://sites.google.com/a/ci.ufpb.br/capagot/"> Cristhian Pagot</a></p>
<p> 2 - Script Octave disponibilizado em aula </p>
<p> 3 - FOLEY, Computer Graphics: Principles and Practice </p>
<p> 4 - <a target = "_blank" href = "http://www.assimp.org"> Loader Assimp</a></p>















