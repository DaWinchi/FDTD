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
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 10);

	glTranslated(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotated(*angleX, 1, 0, 0);
	glRotated(*angleY, 0, 1, 0);
	glScaled(*zoom, *zoom, *zoom);
	glPolygonMode(GL_FRONT_AND_BACK, GL_QUADS);//
	glBegin(GL_QUADS);
	////��������
	//glColor3d(255, 0, 0);
	//glVertex3d(-5.0, -5.0, 5.0); //1
	//glVertex3d(5.0, -5.0, 5.0);  //2
	//glVertex3d(5.0, 5.0, 5.0);//3
	//glVertex3d(-5.0, 5.0, 5.0);//4

	//												   //����
	//glColor3d(255, 255, 0);
	//glVertex3d(-5.0, 5.0, 5.0); //1
	//glVertex3d(5.0, 5.0, 5.0); //2
	//glVertex3d(5.0, 5.0, -5.0); //3
	//glVertex3d(-5.0, 5.0, -5.0); //4
	//												 //������
	//glColor3d(255, 0, 0);
	//glVertex3d(5.0, -5.0, -5.0); //1
	//glVertex3d(-5.0, -5.0, -5.0); //2
	// glVertex3d(-5.0, 5.0, -5.0); //3
	// glVertex3d(5.0, 5.0, -5.0); //4

	//												//���
	//glColor3d(0, 0, 255);
	//glVertex3d(5.0, -5.0, 5.0); //1
	//glVertex3d(-5.0, -5.0, 5.0); //2
	// glVertex3d(-5.0, -5.0, -5.0); //3
	// glVertex3d(5.0, -5.0, -5.0); //4
	//												 //�����
	//glColor3d(0, 255, 0);
	//glVertex3d(5.0, -5.0, 5.0); //1
	// glVertex3d(5.0, -5.0, -5.0); //2
	//glVertex3d(5.0, 5.0, -5.0); //3
	//glVertex3d(5.0, 5.0, 5.0); //4
	//													 //����
	//glColor3d(255, 0, 255); 
	//glVertex3d(-5.0, -5.0, -5.0); //1
	//glVertex3d(-5.0, -5.0, 5.0); //2
	//glVertex3d(-5.0, 5.0, 5.0); //3
	//glVertex3d(-5.0, 5.0, -5.0); //4
	for (int i = 0; i < points[0].size(); i++)
	{
		for (int j = 0; j < points[0][i].size(); j++)
		{
			glColor3d(1.0f, 1.0f, 1.0f);
			int iright = i + 1;
			int jup = j + 1;
			if (iright == points[0].size())
			{
				iright -=2 ;
			}
			if (jup == points[0][i].size())
			{
				jup -= 2;
			}
			if (points[0][i][j].z<0) glColor3d(0, 0, 255); else glColor3d(255, 0, 0);
			glVertex3d(points[0][i][j].x , points[0][i][j].y,points[0][i][j].z);
			if (points[0][iright][j].z<0) glColor3d(0, 0, 255); else glColor3d(255, 0, 0);
			glVertex3d(points[0][iright][j].x, points[0][iright][j].y, points[0][iright][j].z);
			if (points[0][iright][jup].z<0) glColor3d(0, 0, 255); else glColor3d(255, 0, 0);
			glVertex3d(points[0][iright][jup].x, points[0][iright][jup].y, points[0][iright][jup].z);
			if (points[0][i][jup].z<0) glColor3d(0, 0, 255); else glColor3d(255, 0, 0);
			glVertex3d(points[0][i][jup].x, points[0][i][jup].y, points[0][i][jup].z);
		}
	}
	glEnd();
	glFinish();
	SwapBuffers(wglGetCurrentDC());
}