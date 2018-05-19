#ifndef _MYGL_H_
#define _MYGL_H_
#include <stdio.h>
#include <math.h>

#include "definitions.h"

//******************************************************************************
//estrutura das posicao do pixel
typedef struct
{
    unsigned int X;
    unsigned int Y;
} tPosicao;
//estrutura do pixel e suas respectivas características
typedef struct
{
    tPosicao posicao;
    unsigned int R;
    unsigned int G;
    unsigned int B;
    unsigned int A;
} tPixel;

//******************************************************************************

//*****************************************************************************
// Defina aqui as suas funções gráficas

void putPixel(tPixel pixel)
{
    int offset;
    if (pixel.posicao.X >= 0 && pixel.posicao.X <= IMAGE_WIDTH && pixel.posicao.Y >= 0 && pixel.posicao.Y <= IMAGE_HEIGHT)
    {

        offset = (IMAGE_WIDTH * pixel.posicao.Y + pixel.posicao.X) * 4;
        FBptr[offset] = pixel.R;
        FBptr[offset + 1] = pixel.B;
        FBptr[offset + 2] = pixel.G;
        FBptr[offset + 3] = pixel.A;
    }
    else
    {
        printf("Tente um valor entre %d e %d", IMAGE_WIDTH, IMAGE_HEIGHT);
    }
}

void HVLine(tPixel _inicial, tPixel _final)
{

    int dx = _final.posicao.X - _inicial.posicao.X;
    int dy = _final.posicao.Y - _inicial.posicao.Y;

    if (dx == 0)
    {
        while (_inicial.posicao.Y != _final.posicao.Y)
        {

            putPixel(_inicial);
            _inicial.posicao.Y++;
        }
    }
    if (dy == 0)
    {

        while (_inicial.posicao.X != _final.posicao.X)
        {

            putPixel(_inicial);
            _inicial.posicao.X++;
        }
    }
}

void DLine (tPixel _inicial, tPixel _final) {

     int dx = _final.posicao.X - _inicial.posicao.X;
    int dy = _final.posicao.Y - _inicial.posicao.Y;
    int m = dy/dx; 


   if (abs(dy) == abs(dx)){
    // para m = 1   (quadrante 1 e 3)
    if (m == 1)
    {
        while ((_inicial.posicao.X != _final.posicao.X) && (_inicial.posicao.Y  != _final.posicao.Y)) {

            putPixel (_inicial); 
            _inicial.posicao.X++;
            _inicial.posicao.Y++;

        }
    }

        if (m == - 1)
    {
        while ((_inicial.posicao.X != _final.posicao.X) && (_inicial.posicao.Y  != _final.posicao.Y)) {

            putPixel (_final); 
            _final.posicao.X++;
            _final.posicao.Y--;

        }

    //para m = -1 (quadrante 2 e 4 )
    }

   } 


}

void drawLine(tPixel primeiro, tPixel ultimo)
{
    int d;
    int incrementaLeste;
    int incrementaNordeste;
    int incrementaX = 0;
    int incrementaY = 0;
    int deltaX = ultimo.posicao.X - primeiro.posicao.X;
    int deltaY = ultimo.posicao.Y - primeiro.posicao.Y;

    //se o delta for negativo decrementa
    // o valor de incrementaX é 1, caso contrário é -1
    if (deltaX > 0)
    {
        incrementaX = 1;
    }
    else
    {
        incrementaX = -1;
        //módulo de deltaX
        deltaX = abs(deltaX);
    }
    if (deltaY > 0)
    {
        incrementaY = 1;
    }
    else
    {
        incrementaY = -1;
        //módulo de deltaY
        deltaY = abs(deltaY);
    }

    //posicao e características do pixel
    tPixel atual;
    atual.posicao.X = primeiro.posicao.X;
    atual.posicao.Y = primeiro.posicao.Y;
    atual.R = ultimo.R;
    atual.G = ultimo.G;
    atual.B = ultimo.B;
    atual.A = ultimo.A;

    //se delta X for igual a zero, temos uma coluna
    //se delta X for diferente de zero e
    //delta Y for igual a zero temos uma linha
    if (deltaX == 0)
    {
        for (atual.posicao.Y; atual.posicao.Y != ultimo.posicao.Y; atual.posicao.Y += incrementaY)
        {
            putPixel(atual);
        }
    }
    else
    {
        if (deltaY == 0)
        { //linha horizontal
            for (atual.posicao.X; atual.posicao.X != ultimo.posicao.X; atual.posicao.X += incrementaX)
            {
                putPixel(atual);
            }
        }
        else
        {
            //caso de não ser uma reta horizontal/vertical
            if (abs(deltaX) >= abs(deltaY))
            {
                d = 2 * deltaY - deltaX;
                incrementaLeste = 2 * deltaY;
                incrementaNordeste = 2 * (deltaY - deltaX);
                while (atual.posicao.X != ultimo.posicao.X)
                {
                    if (d <= 0)
                    {
                        d += incrementaLeste;
                        atual.posicao.X += incrementaX;
                    }
                    else
                    {
                        d += incrementaNordeste;
                        atual.posicao.X += incrementaX;
                        atual.posicao.Y += incrementaY;
                    }
                    putPixel(atual);
                }
            }
            else
            {
                d = 2 * deltaX - deltaY;
                incrementaLeste = 2 * deltaX;
                incrementaNordeste = 2 * (deltaX - deltaY);
                while (atual.posicao.Y != ultimo.posicao.Y)
                {
                    if (d <= 0)
                    {
                        d += incrementaLeste;
                        atual.posicao.Y += incrementaY;
                    }
                    else
                    {
                        d += incrementaNordeste;
                        atual.posicao.Y += incrementaY;
                        atual.posicao.X += incrementaX;
                    }
                    putPixel(atual);
                }
            }
        }
    }
}

void interpolar(tPixel primeiro, tPixel ultimo, tPixel *atual)
{

    int R, G, B, A;
    float deltaX = (float)(ultimo.posicao.X - primeiro.posicao.X);
    float deltaY = (float)(ultimo.posicao.Y - primeiro.posicao.Y);

    if (abs(deltaX) >= abs(deltaY))
    {
        R = (ultimo.R - primeiro.R) / deltaX;
        G = (ultimo.G - primeiro.G) / deltaX;
        B = (ultimo.B - primeiro.B) / deltaX;
        A = (ultimo.A - primeiro.A) / deltaX;
    }
    else
    {
        R = (ultimo.R - primeiro.R) / deltaY;
        G = (ultimo.G - primeiro.G) / deltaY;
        B = (ultimo.B - primeiro.B) / deltaY;
        A = (ultimo.A - primeiro.A) / deltaY;
    }

    atual->R += R;
    atual->G += G;
    atual->B += B;
    atual->A += A;

    putPixel(*atual);
}

void drawLineInterpolado(tPixel primeiro, tPixel ultimo)
{

    int deltaX = ultimo.posicao.X - primeiro.posicao.X;
    int deltaY = ultimo.posicao.Y - primeiro.posicao.Y;
    int d;
    int incrementaLeste;
    int incrementaNordeste;
    int incrementaX = 0;
    int incrementaY = 0;

    if (deltaX > 0)
    {
        incrementaX = 1;
    }
    else if (deltaX < 0)
    {
        incrementaX = -1;
        deltaX = abs(deltaX);
    }
    if (deltaY > 0)
    {
        incrementaY = 1;
    }
    else if (deltaY < 0)
    {
        incrementaY = -1;
        deltaY = abs(deltaY);

        tPixel atual;
        atual.posicao.X = primeiro.posicao.X;
        atual.posicao.Y = primeiro.posicao.Y;
        atual.R = primeiro.R;
        atual.G = primeiro.G;
        atual.B = primeiro.B;
        atual.A = primeiro.A;

        if (deltaX == 0)
        {
            for (atual.posicao.Y; atual.posicao.Y != ultimo.posicao.Y; atual.posicao.Y += incrementaY)
            {
                interpolar(primeiro, ultimo, &atual);
            }
        }
        else
        {
            if (deltaY == 0)
            {
                for (atual.posicao.X; atual.posicao.X != ultimo.posicao.X; atual.posicao.X += incrementaX)
                {
                    interpolar(primeiro, ultimo, &atual);
                }
            }
            else
            {
                if (abs(deltaX) >= abs(deltaY))
                {
                    d = 2 * deltaY - deltaX;
                    incrementaLeste = 2 * deltaY;
                    incrementaNordeste = 2 * (deltaY - deltaX);
                    while (atual.posicao.X != ultimo.posicao.X)
                    {
                        if (d <= 0)
                        {
                            d += incrementaLeste;
                            atual.posicao.X += incrementaX;
                        }
                        else
                        {
                            d += incrementaNordeste;
                            atual.posicao.X += incrementaX;
                            atual.posicao.Y += incrementaY;
                        }
                        interpolar(primeiro, ultimo, &atual);
                    }
                }
                else
                {
                    d = 2 * deltaX - deltaY;
                    incrementaLeste = 2 * deltaX;
                    incrementaNordeste = 2 * (deltaX - deltaY);
                    while (atual.posicao.Y != ultimo.posicao.Y)
                    {
                        if (d <= 0)
                        {
                            d += incrementaLeste;
                            atual.posicao.Y += incrementaY;
                        }
                        else
                        {
                            d += incrementaNordeste;
                            atual.posicao.Y += incrementaY;
                            atual.posicao.X += incrementaX;
                        }
                        interpolar(primeiro, ultimo, &atual);
                    }
                }
            }
        }
    }
}

void drawTriangle(tPixel p1, tPixel p2, tPixel p3)
{
    drawLine(p2, p1);
    drawLine(p3, p2);
    drawLine(p1, p3);
}

void pintaTrianguloInterpolado(tPixel primeiro, tPixel ultimo, tPixel p3)
{
    int deltaX = ultimo.posicao.X - primeiro.posicao.X;
    int deltaY = ultimo.posicao.Y - primeiro.posicao.Y;
    int d;
    int incrementaLeste;
    int incrementaNordeste;
    int incrementaX = 0;
    int incrementaY = 0;

    if (deltaX > 0)
    {
        incrementaX = 1;
    }
    else
    {
        incrementaX = -1;
        deltaX = abs(deltaX);
    }
    if (deltaY > 0)
    {
        incrementaY = 1;
    }
    else
    {
        incrementaY = -1;
        deltaY = abs(deltaY);
    }

    tPixel atual;
    atual.posicao.X = primeiro.posicao.X;
    atual.posicao.Y = primeiro.posicao.Y;
    atual.R = primeiro.R;
    atual.G = primeiro.G;
    atual.B = primeiro.B;
    atual.A = primeiro.A;

    int comprimento = sqrt(deltaX * deltaX + deltaY * deltaY);
    int incR = (ultimo.R - primeiro.R) / comprimento;
    int incG = (ultimo.G - primeiro.G) / comprimento;
    int incB = (ultimo.B - primeiro.B) / comprimento;
    int incA = (ultimo.A - primeiro.A) / comprimento;

    if (deltaX == 0)
    {
        for (atual.posicao.Y; atual.posicao.Y != ultimo.posicao.Y; atual.posicao.Y += incrementaY)
        {

            interpolar(primeiro, ultimo, &atual);
            drawLineInterpolado(atual, p3);
        }
    }
    else
    {
        if (deltaY == 0)
        {
            for (atual.posicao.X; atual.posicao.X != ultimo.posicao.X; atual.posicao.X += incrementaX)
            {

                interpolar(primeiro, ultimo, &atual);
                drawLineInterpolado(atual, p3);
            }
        }
        else
        {
            if (abs(deltaX) >= abs(deltaY))
            {
                d = 2 * deltaY - deltaX;
                incrementaLeste = 2 * deltaY;
                incrementaNordeste = 2 * (deltaY - deltaX);
                while (atual.posicao.X != ultimo.posicao.X)
                {
                    if (d <= 0)
                    {
                        d += incrementaLeste;
                        atual.posicao.X += incrementaX;
                    }
                    else
                    {
                        d += incrementaNordeste;
                        atual.posicao.X += incrementaX;
                        atual.posicao.Y += incrementaY;
                    }
                    interpolar(primeiro, ultimo, &atual);
                    drawLineInterpolado(atual, p3);
                }
            }
            else
            {
                d = 2 * deltaX - deltaY;
                incrementaLeste = 2 * deltaX;
                incrementaNordeste = 2 * (deltaX - deltaY);
                while (atual.posicao.Y != ultimo.posicao.Y)
                {
                    if (d <= 0)
                    {
                        d += incrementaLeste;
                        atual.posicao.Y += incrementaY;
                    }
                    else
                    {
                        d += incrementaNordeste;
                        atual.posicao.Y += incrementaY;
                        atual.posicao.X += incrementaX;
                    }
                    interpolar(primeiro, ultimo, &atual);
                    drawLineInterpolado(atual, p3);
                }
            }
        }
    }
}

void drawTriangleInterpolado(tPixel p1, tPixel p2, tPixel p3)
{
    drawLineInterpolado(p2, p1);
    drawLineInterpolado(p3, p2);
    drawLineInterpolado(p1, p3);
}

//*****************************************************************************

#endif // _MYGL_H_
