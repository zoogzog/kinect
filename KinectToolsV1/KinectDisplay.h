#ifndef KINECTDISPLAY_H
#define KINECTDISPLAY_H

#include <Windows.h>
#include <Ole2.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glut.h>

#include "KinectDevice.h"
#include "SkeletonBody.h"

//-----------------------------------------------------------------
//---- Helper class to display Kinect data.
//---- Mainly use it for debug purpose.
//---- Author: Andrey G.
//-----------------------------------------------------------------

class KinectDisplay
{

private:

	//---- Textures to display kinect color & depth
	GLuint textureID;
 
	GLubyte * dataDepth;  
	GLubyte * dataColor; 



public:
	int width;
	int height;

	KinectDisplay();

	//---- Initialize displaying data
	void init(int width, int height);

	//---- Draws a particular bone of the skeleton
	void drawSkeletonBone (SkeletonBody skeleton, NUI_SKELETON_POSITION_INDEX joint1, NUI_SKELETON_POSITION_INDEX joint2);

	//---- Draw all tracked skeletons
	void drawKinectSkeleton (SkeletonBody *skeletonPool);

	//---- Draw depth data
	void drawKinectData (int startX, int startY, byte *data);
};



#endif
