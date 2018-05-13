# Trabalho 1 - Rasterização de Pontos e Linhas

<p> O trabalho apresentado consiste na implementação de algoritmos para rasterizar pontos e linhas e, em seguida, construir triângulos usando a rasterização das linhas que compõem suas arestas. O objetivo desse exercício, como colocado na sua descrição, é fazer com que nós, alunos da disciplina, tenhamos uma maior familiarização com os algoritmos de rasterização utilizados dentro da computação gráfica.
Vale salientar que para o seu desenvolvimento, será utilizado um framework disponibilizado pelo professor. Ele tem o objetivo de proporcionar a simulação do acesso à memória de vídeo, uma vez que os sistemas operacionais atuais tem um mecanismo de proteção a memória, impedindo que seja realizado qualquer tipo de acesso direto a mesma.</p>


<p><b>a)putPixel: Desenha um pixel na tela </b> </p>
<p><b>b)drawLine: Desenha uma linha na tela</b> </p>
<p><b>c)drawTriangle: Desenha um triângulo na tela</b> </p>


<p> Para entender o trabalho que aqui se delineia, é importante que tenhamos em mente alguns conceitos importantes. Um deles é o de rasterização, que pode ser definido como a conversão de uma primitiva matemática (retas, pontos polígonos ou outros objetos do plano cartesiano) em pixels e desenhá-los na dela.
O pixel corresponde a menor unidade de uma imagem digital, representado por 4 bytes em uma posição de memória em que os três primeiros bytes correspondem as cores primárias vermelho, verde e azul respectivamente e o quarto byte corresponde a um canal (alpha) que pode ser utilizado para transparência. </p>

![alt text](https://i.imgur.com/nAdNyN4.png "Representaçao da Memória de Vídeo")
<div align="center"> <p> Representação da Memória de Vídeo <p> </div>

Foram definidas duas estruturas principais a fim de facilitar as manipulações:

```c++
    typedef struct{
        unsigned int X;
        unsigned int Y;
    }tPosicao;
    //estrutura do pixel e suas respectivas características
    typedef struct{
        tPosicao posicao;
        unsigned int R;
        unsigned int G;
        unsigned int B;
        unsigned int A;
    }tPixel;

```

1 - putPixel()
====

<p> Rasterizar um ponto no monitor significa simplesmente escolher um determinado ponto no mesmo para podermos atribuir uma cor. A função responsável por desenhar na tela um pixel, indo na memória de vídeo escrevendo o RGBA de cor do pixel é a putPixel (descrita abaixo). Ela recebe como parâmetro pixel, que contém as informações referentes a sua cor e as suas coordenadas. O offset de cada pixel (retornando o endereço do 1º byte do mesmo) é calculado para garantir que ele ocupe um único e distinto espaço em memória. Utilizamos o ponteiro FBprt (Frame Buffer Pointer) existente dentro do Framework definido para o trabalho para que pudéssemos acessar a posição de memória referente ao pixel para escrever o RGBA de cor referente ao pixel, como mostrado abaixo: </p>

```c++
     offset = (IMAGE_WIDTH*pixel.coordenadas.Y + pixel.coordenadas.X)*4;
     FBptr[offset]   = pixel.R;
     FBptr[offset+1] = pixel.B;
     FBptr[offset+2] = pixel.G;
     FBptr[offset+3] = pixel.A;

```
![alt text](https://i.imgur.com/SNwdThi.png "putPixel() em funcionamento")



2 - drawLine()
====

<p> A rasterização de uma reta dentro do mundo computacional ocorre da seguinte maneira: dados dois pixels extremos em um dispositivo quadriculado, e queremos traçar uma reta entre eles, devemos escolher os pixels localizados entre eles de modo que formem o segmento desejado. O que acontece é que nem sempre isso é simples. Em determinados momentos, reta não intersecciona apenas um pixel, e a interpretação para qual deve ser pintado é outra. Uma solução para esse problema é, dado dois pixels candidatos, escolhermos o mais próximo do segmento de reta que pode ser encontrado usando o coeficiente angular e a <b>equação</b> de tal reta. A imagem a seguir ilustra essa solução: </p>

![alt text](https://i.imgur.com/kn3ozB1.png "Rasterização")

<p>O problema é o custo da operação, que trabalha com resultados float e arredondamentos. Para isso utilizaremos como implementação da função drawLine o <b>Algorítmo de Bresenham </b>que utiliza operações aritméticas apenas em inteiros, procurando diminuir o custo de implementação padrão</p>


<p> O algoritmo de Bresenham utiliza como base a equação geral da reta</p>

```c++
T = AX+B

```
![alt text](https://i.imgur.com/mqj4qnp.png "Ponto Médio")

<p> Partindo do píxel C, temos duas possibilidades para qual o próximo pixel a ser pintado. (Xc+1, Yc) que corresponde ao pixel (E) e (Xc+1, Yc + 1) que corresponde ao pixel (NE). Logicamente o píxel a ser pintado é
se encontra mais próximo do ponto <u>m</u> que pode ser calculado da seguinte maneira. </p>

```c++
m  = (Xc + 1, Yc + ½)

```
<p> Mas não basta só encontrar o valor da coordenada <b>m</b>. É preciso determinar se ela está acima ou abaixo da reta para então acender o pixel mais próximo (E) ou (NE).

Para encontrarmos o ponto médio, é preciso antes encontrar o primeiro valor de <b>d</b> que é dado por </p>

```c++

d0 = 2(Δy-Δx)

```
<p> E assim os demais valores de <b>d</b> serão calculados baseados no algoritmo abaixo</p>

```c++
Φ(x,y) = Δy * x - Δx * y + b * Δx = 0

Se Φ(m) ≥ 0
	proxPixel(leste)
	dnovo = dantigo + 2*Δy
Se Φ(m) < 0 
	proxPixel(nordeste)
	dnovo = dantigo + 2(Δy-Δx)

```
<p> É importante lembrar que a lógica demontrada acima funciona apenas para um octante. O que está entre 0º e 45º. Quanto aos demais octantes, devemos generalizar o algoritmo baseado na imagem abaixo. </p>


![alt text](https://i.imgur.com/WRMBVfx.png "Demais Octantes")


<p>Para generalizar o algoritmo para todos os octantes é preciso fazer algumas motificações dependendo dos valores de <b>∆x</b> e <b>∆y</b>. </p>

<p> Estabelecer o valor +1 , -1 ou 0 para valores de <b>∆x</b> e <b>∆y</b>:</p>

```c++
	incrementaX =  0 se ∆x == 0
	incrementaX = +1 se ∆x >  0
	incrementaX = -1 se ∆x <  0
	incrementaY =  0 se ∆y == 0
	incrementaY = +1 se ∆y >  0
	incrementaY = -1 se ∆x <  0
```
E então qual dos dois módulos é maior, o de <b>∆x</b> ou o de <b>∆y</b>: </p>
<p> Se <b>∆x</b> for maior que <b>∆y</b> </p>

```c++
  if (abs(deltaX)>=abs(deltaY)){
        d=2*deltaY-deltaX;
        incrementaLeste=2*deltaY;
        incrementaNordeste=2*(deltaY-deltaX);
        while(atual.posicao.X!=ultimo.posicao.X){
                        if(d<=0){
                d+=incrementaLeste;
                atual.posicao.X+=incrementaX;
            }else{
                d+=incrementaNordeste;
                atual.posicao.X+=incrementaX;
                atual.posicao.Y+=incrementaY;
            }
            putPixel(atual);
        }
```
<p> Caso contrário </p>

```c++
	else {
	    d=2*deltaX-deltaY;
	    incrementaLeste=2*deltaX;
	    incrementaNordeste=2*(deltaX-deltaY);
	    while(atual.posicao.Y!=ultimo.posicao.Y){
	        if(d<=0){
	            d+=incrementaLeste;
	            atual.posicao.Y+=incrementaY;
	        }else{
	            d+=incrementaNordeste;
	            atual.posicao.Y+=incrementaY;
	            atual.posicao.X+=incrementaX;
	        }
	        putPixel(atual);
	    }
```

3 - drawTriangle()
====

<p>Baseado o método <b>drawLine()</b> o método <b>drawTriangle()</b> chama o método anterior três vezes. Obtemos como resultado a imagem abaixo.</p>

![alt text](https://i.imgur.com/gl2keqZ.png "Triângulo1")

<p></p>

![alt text](https://i.imgur.com/X1xF9fy.png "Triângulo2")

<h0>Note que não há suavização na reta verde. Uma possível melhoria não implementada seria aplicar um Anti-aliasing para dar a ilusão de suavidade. </h0>


Interpolação
====

<p>A interpolação consiste no processo de alternar as cores entre o ponto inicial e o ponto final de uma linha de modo que ela mude gradualmente de cor entre esses dois pontos.</p>
<p> A lógica do algoritmo consiste em determinar qual módulo da variação é maior <b>∆x</b> ou <b>∆y</b> para sabermos a quantidade de pixel da reta, logo após devemos determinar a variação para os canais <b>R</b>, <b>G</b>, <b>B</b> e <b>A</b>, para então criarmos variáveis que vão conter os valores de incremento de cada byte de cor.

```c++
Se abs(∆x) > abs(∆y)
	quantidadePixels = abs(∆x)
Se abs(∆y) > (∆x)
	quantidadePixels = abs(∆y)

//determinar a variação dos canais

deltaR = Rfinal - Rinicial
deltaG = Gfinal - Ginicial
deltaB = Bfinal - Ginicial

// optei por não utilizaremos o pixel Alfa por não 
// ainda entender seu funcionamento, mas a lógica 
//segue o mesmo princípio.

// ∆A = Afinal - Ainicial

incrementaR = deltaR/quantidadePixels
incrementaG = deltaG/quantidadePixels
incrementaB = deltaB/quantidadePixels

```
<p> E ao utilizarmos o drawLine() implementando a lógica acima, obtemos os seguintes resultados:</p>

![alt text](https://i.imgur.com/dQzUKlu.png "Linha Interpolada")

<p> E ao utilizarmos o drawTriangle() chamando o drawLine() com a interpolação, temos:</p>


![alt text](https://i.imgur.com/eQnonYq.png "Triangulo Interpolado")


<p>Os códigos e materiais utilizados para implementação encontram-se neste repositório.</p>


∞ - Extras
====


<h3> Comentários e Dificuldades </h3>
<p> Nossa maior dificuldade na implementação foi a generalização do desenho das retas nos octantes, mais especificamente às variações presentes nos valores de |∆x| e |∆y|. Apenas após analisarmos <a target="_blank" href="https://camo.githubusercontent.com/b02ab4b60031f0352751efe3d70355de302b049c/68747470733a2f2f692e696d6775722e636f6d2f57524d425666782e706e67"> esta imagem</a> pudemos compreender melhor as variações. Outro ponto foi a interpolação das linhas, encontrar a quantidade de pixels para adicionar na variável incremento de cada canal. Mas foi sanada no mesmo blog.</p>

<h3>Fontes</h3> 

<p> 1 - <a target="_blank" href="http://angelinacg.blogspot.com.br/"> Blog Angelina Sales</a> 
<p> 2 - <a target="_blank" href="https://johannesca.github.io/cg_t1/"> Página Johannes Cavalcante</a> 
<p> 3 - Slides Professor <a target = "_blank" href = "https://sites.google.com/a/ci.ufpb.br/capagot/"> Cristhian Pagot</a></p>
<p> 4- FOLEY, Computer Graphics: Principles and Practice </p>



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













