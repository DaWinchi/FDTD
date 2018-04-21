#include "stdafx.h"
#include "CDrawer.h"


CDrawer::CDrawer()
{
}


CDrawer::~CDrawer()
{
}

void CDrawer::Draw()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 10);

	glTranslated(0.0f, 0.0f, -1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotated(*angleX, 1, 0, 0);
	glRotated(*angleY, 0, 1, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);//
	glBegin(GL_QUADS);
	//передн€€
	glColor3d(255, 0, 0);
	glVertex3d(-5.0, -5.0, 5.0); //1
	glVertex3d(5.0, -5.0, 5.0);  //2
	glVertex3d(5.0, 5.0, 5.0);//3
	glVertex3d(-5.0, 5.0, 5.0);//4

													   //верх
	glColor3d(255, 255, 0);
	glVertex3d(-5.0, 5.0, 5.0); //1
	glVertex3d(5.0, 5.0, 5.0); //2
	glVertex3d(5.0, 5.0, -5.0); //3
	glVertex3d(-5.0, 5.0, -5.0); //4
													 //задн€€
	glColor3d(255, 0, 0);
	glVertex3d(5.0, -5.0, -5.0); //1
	glVertex3d(-5.0, -5.0, -5.0); //2
	 glVertex3d(-5.0, 5.0, -5.0); //3
	 glVertex3d(5.0, 5.0, -5.0); //4

													//низ
	glColor3d(0, 0, 255);
	glVertex3d(5.0, -5.0, 5.0); //1
	glVertex3d(-5.0, -5.0, 5.0); //2
	 glVertex3d(-5.0, -5.0, -5.0); //3
	 glVertex3d(5.0, -5.0, -5.0); //4
													 //право
	glColor3d(0, 255, 0);
	glVertex3d(5.0, -5.0, 5.0); //1
	 glVertex3d(5.0, -5.0, -5.0); //2
	glVertex3d(5.0, 5.0, -5.0); //3
	glVertex3d(5.0, 5.0, 5.0); //4
														 //лево
	glColor3d(255, 0, 255); 
	glVertex3d(-5.0, -5.0, -5.0); //1
	glVertex3d(-5.0, -5.0, 5.0); //2
	glVertex3d(-5.0, 5.0, 5.0); //3
	glVertex3d(-5.0, 5.0, -5.0); //4
	glEnd();

	SwapBuffers(wglGetCurrentDC());
}