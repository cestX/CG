#include "main.h"

//-----------------------------------------------------------------------------
void MyGlDraw(void)
{
	//*************************************************************************
	// Chame aqui as funções do mygl.h
	//*************************************************************************

	//linha();
	//DesenhaLinha();
    //DesenhaPixels();

    tPixel pixel1 = {256, 256, 255, 0, 0, 255};
    tPixel pixel2 = {384, 128, 0, 255, 255, 255};
   	tPixel pixel3 = {128, 128, 0, 0, 255, 255};
   	tPixel pixel4 = {50, 100, 0, 0, 255, 255};
    drawTriangleInterpolado(pixel3, pixel2, pixel1);

}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	// Inicializações.
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();


	// Ajusta a função que chama as funções do mygl.h
	DrawFunc = MyGlDraw;
	// Framebuffer scan loop.
 
	glutMainLoop();


	return 0;
}

