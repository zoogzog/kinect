#include <Windows.h>
#include <Ole2.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glut.h>

#include "KinectDisplay.h"
#include "KinectDevice.h"
#include "SkeletonBody.h"

//-----------------------------------------------------------------

KinectDisplay::KinectDisplay ()
{
}


void KinectDisplay::init(int width, int height)
{
	this->width = width;
	this->height = height;
	
	//---- Allocate memory
	dataDepth = new GLubyte[width * height * 4];
	dataColor = new GLubyte[width * height * 4];

	//---- Bind textures to display depth and color
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*) dataDepth);
    glBindTexture(GL_TEXTURE_2D, 0);
}

//-----------------------------------------------------------------

void KinectDisplay::drawSkeletonBone (SkeletonBody skeleton, int joint1, int joint2)
{
	float px1 = skeleton.skeletonPosition[joint1].x;
	float py1 = skeleton.skeletonPosition[joint1].y;

	float px2 = skeleton.skeletonPosition[joint2].x;
	float py2 = skeleton.skeletonPosition[joint2].y;
	

	//---- Draw bone (COLOR = RED)
	glPushAttrib( GL_CURRENT_BIT );
	
	glLineWidth(3); 
	glColor4f(1.0, 0.0, 0.0, 1);

	glBegin(GL_LINES);
		glVertex3f(px1, py1, 0.0);
		glVertex3f(px2, py2, 0);
	glEnd();

	glColor4f(0.0, 1.0, 0.0, 1);

	//---- Draw joint (COLOR = GREEN)
	glPointSize(4);   
	glBegin(GL_POINTS);
		glVertex3f(px1, py1, 0.0);
		glVertex3f(px2, py2, 0);
	glEnd();

	glPopAttrib();
}

void KinectDisplay::drawKinectSkeleton (SkeletonBody *skeletonPool)
{
	glPushMatrix();
	for (int i = 0; i < 6; i++)
	{
		//---- Check if this particular skeleton is tracked
		if (skeletonPool[i].isTracked)
		{
			//----- Transform all coordinates of the skeleton
			SkeletonBody transformed = KinectDevice::convertSkeletonCoordinates(skeletonPool[i], width, height);

			for (int boneID = 0; boneID < JOINT_ID_KINECTV1::ATLAS_SIZE; boneID++)
			{
				int joint1 = JOINT_ID_KINECTV1::ATLAS[boneID].j1;
				int joint2 = JOINT_ID_KINECTV1::ATLAS[boneID].j2;

				drawSkeletonBone(transformed, joint1, joint2);

			}
		}
	}
	glPopMatrix();
}

void KinectDisplay::drawKinectData (int startX, int startY, byte *data)
{
	//---- This clears previously used part of screen
	glScissor(startX,startY,width,height);
	glEnable(GL_SCISSOR_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glDisable(GL_SCISSOR_TEST);

	//---- Display texture (depth or color image) here
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glBindTexture(GL_TEXTURE_2D, textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*)data);
 
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(startX, startY, 0);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(startX + width, startY, 0);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(startX + width, startY + height, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(startX, startY + height, 0.0f);
    glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
}

