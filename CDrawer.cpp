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
	//��������
	glColor3d(132, 112, 255); glVertex3d(-5.0, -5.0, 5.0); //1
	glColor3d(255, 255, 0); glVertex3d(5.0, -5.0, 5.0);  //2
	glColor3d(238, 233, 233); glVertex3d(5.0, 5.0, 5.0);//3
	glColor3d(0, 245, 255); glVertex3d(-5.0, 5.0, 5.0);//4

													   //����
	glColor3d(0, 245, 255); glVertex3d(-5.0, 5.0, 5.0); //1
	glColor3d(238, 233, 233); glVertex3d(5.0, 5.0, 5.0); //2
	glColor3d(0, 0, 1); glVertex3d(5.0, 5.0, -5.0); //3
	glColor3d(0, 0, 1); glVertex3d(-5.0, 5.0, -5.0); //4
													 //������
	glColor3d(0, 0, 1); glVertex3d(5.0, -5.0, -5.0); //1
	glColor3d(0, 0, 1); glVertex3d(-5.0, -5.0, -5.0); //2
	glColor3d(1, 1, 0); glVertex3d(-5.0, 5.0, -5.0); //3
	glColor3d(0, 0, 1); glVertex3d(5.0, 5.0, -5.0); //4

													//���
	glColor3d(1, 0, 0); glVertex3d(5.0, -5.0, 5.0); //1
	glColor3d(0, 0, 1); glVertex3d(-5.0, -5.0, 5.0); //2
	glColor3d(0, 0, 1); glVertex3d(-5.0, -5.0, -5.0); //3
	glColor3d(0, 0, 1); glVertex3d(5.0, -5.0, -5.0); //4
													 //�����
	glColor3d(255, 255, 0); glVertex3d(5.0, -5.0, 5.0); //1
	glColor3d(0, 0, 1); glVertex3d(5.0, -5.0, -5.0); //2
	glColor3d(0, 0, 1); glVertex3d(5.0, 5.0, -5.0); //3
	glColor3d(238, 233, 233); glVertex3d(5.0, 5.0, 5.0); //4
														 //����
	glColor3d(0, 0, 1); glVertex3d(-5.0, -5.0, -5.0); //1
	glColor3d(132, 112, 255); glVertex3d(-5.0, -5.0, 5.0); //2
	glColor3d(0, 245, 255); glVertex3d(-5.0, 5.0, 5.0); //3
	glColor3d(0, 0, 1); glVertex3d(-5.0, 5.0, -5.0); //4
	glEnd();

	SwapBuffers(wglGetCurrentDC());
}