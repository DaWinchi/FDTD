#pragma once
#include <gl/GLU.h>
#include <gl/GL.h>
#include "glut.h"

class CDrawer
{
public:
	CDrawer();
	~CDrawer();

	double *angleX, *angleY;
	void Draw();
};

