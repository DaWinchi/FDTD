#pragma once
#include <gl/GLU.h>
#include <gl/GL.h>
#include "glut.h"
#include <vector>

struct Point3D
{
	double x, y, z;
};

class CDrawer
{
public:
	CDrawer();
	~CDrawer();

	double *angleX, *angleY, *zoom;
	void Draw();
	std::vector<std::vector<Point3D>> *points;
};

