#include "DataVectors.h"

vector2d::vector2d()
{
	x = 0;
	y = 0;
}

vector2d::vector2d(int ix, int iy)
{
	x = ix;
	y = iy;
}

vector2f::vector2f()
{
	x = 0;
	y = 0;
}

vector2f::vector2f(float ix, float iy)
{
	x = ix; 
	y = iy;
}

vector3d::vector3d()
{
	x = 0; 
	y = 0;
	z = 0;
}

vector3d::vector3d(int ix, int iy, int iz)
{
	x = ix;
	y = iy;
	z = iz;
}

vector3f::vector3f()
{
	x = 0;
	y = 0; 
	z = 0;
}

vector3f::vector3f(float ix, float iy, float iz)
{
	x = ix; 
	y = iy;
	z = iz;
}

vector4d::vector4d()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

vector4d::vector4d(int ix, int iy, int iz, int iw)
{
	x = ix;
	y = iy;
	z = iz;
	w = iw;
}

vector4f::vector4f()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

vector4f::vector4f(float ix, float iy, float iz, float iw)
{
	x = ix;
	y = iy;
	z = iz;
	w = iw;
}
