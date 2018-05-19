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

	tPixel pixel0 = {500,500,255,255,255,0};
    tPixel pixel1 = {1, 5, 255, 255, 255, 0};
    tPixel pixel2 = {1, 3, 255, 255, 255, 0};
	tPixel pixel3 = {0, 512, 255, 255, 255, 0};
    tPixel pixel4 = {512, 0, 255, 255, 255, 0};
	
	//putPixel (pixel0);
	DLine (pixel4, pixel3);
	//HVLine(pixel1, pixel2);

    //drawTriangleInterpolado(pixel3, pixel2, pixel1);

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

