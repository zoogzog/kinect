#pragma once

//--------------------------------------------------------------------------------
//---- Basic structures: vectors 2D, 3D, 4D
//---- Author: Andrey G.
//--------------------------------------------------------------------------------

struct vector2f
{
	float x;
	float y;

	vector2f();
	vector2f(float ix, float iy);
};

struct vector2d
{
	int x;
	int y;

	vector2d();
	vector2d(int ix, int iy);
};

struct vector3f
{
	float x;
	float y;
	float z;

	vector3f();
	vector3f(float ix, float iy, float iz);
};

struct vector3d
{
	int x;
	int y;
	int z;

	vector3d();
	vector3d(int ix, int iy, int iz);
};

struct vector4d
{
	int x;
	int y;
	int z;
	int w;

	vector4d();
	vector4d(int ix, int iy, int iz, int iw);
};

struct vector4f
{
	float x;
	float y; 
	float z;
	float w;

	vector4f();
	vector4f(float ix, float iy, float iz, float iw);
};

