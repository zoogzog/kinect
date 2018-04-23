#include <iostream>
#include <string>
#include <chrono>

#include <Windows.h>
#include <Ole2.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glut.h>

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

#include "KinectDisplay.h"
#include "KinectMainDriver.h"

using namespace std;

//-----------------------------------------------------------------
//---- This is an example, that shows how to use KinectMainDriver class
//---- First
//-----------------------------------------------------------------
#define width 640
#define height 480

//-----------------------------------------------------------------

KinectDisplay kinectDisplay;
KinectMainDriver * kinectGameDriver;

double timestamp = 0;

//-----------------------------------------------------------------

void testDrawScene() 
{


	kinectGameDriver->update();



	kinectDisplay.drawKinectData(0, 0, kinectGameDriver->getDataDepth());
	kinectDisplay.drawKinectData(width, 0, kinectGameDriver->getDataColor());
	kinectDisplay.drawKinectSkeleton(kinectGameDriver->getDataSkeletonPool());

	glutSwapBuffers();
}

bool testInitGLUT(int argc, char* argv[]) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(2 * width,height);
    glutCreateWindow("Kinect SDK Tutorial");
    glutDisplayFunc(testDrawScene);
    glutIdleFunc(testDrawScene);
    return true;
}

void testInitGL ()
{
	// OpenGL setup
    glClearColor(0,0,0,0);
    glClearDepth(1.0f);
    glEnable(GL_TEXTURE_2D);

    // Camera setup
    glViewport(0, 0, 2 * width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 2 * width, height, 0, 1, -1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void testMainLoop() 
{
    glutMainLoop();
}

void testLaunch(int argc, char* argv[])
{
	//----- DEFINE PARAMETERS
	string DEFAULT_DIR_RGB = "test/RGB/";
	string DEFAULT_DIR_DPT = "test/D/";
	string DEFAULT_DIR_DPTI = "test/DEBUG-DEPTH/";
	string DEFAULT_DIR_SKLI = "test/DEBUG-SKELETON/";
	string DEFAULT_FILE_SKL = "test/body.txt";
	string DEFAULT_FILE_LOG_RGB = "test/color.txt";
	string DEFAULT_FILE_LOG_DPT = "test/depth.txt";

	if (!testInitGLUT(argc, argv)) { return; }

	kinectGameDriver = KinectMainDriver::getInstance();
	bool isOK = kinectGameDriver->init(DEFAULT_DIR_RGB, DEFAULT_DIR_DPT, DEFAULT_DIR_DPTI, DEFAULT_DIR_SKLI, DEFAULT_FILE_SKL, DEFAULT_FILE_LOG_RGB, DEFAULT_FILE_LOG_DPT, true, true, true, true, true);

	kinectDisplay.init(width, height);


	testInitGL();
	testMainLoop();
}

//-----------------------------------------------------------------
int main(int argc, char* argv[]) 
{
	testLaunch(argc, argv);
	return 0;
}
