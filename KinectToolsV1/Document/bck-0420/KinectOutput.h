#ifndef OUTPUTDRIVER_H
#define OUTPUTDRIVER_H

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "SkeletonBody.h"

using namespace std;

//-----------------------------------------------------------------
//----- This class saves data captured by Kinect v1 into files.
//----- This is a singleton class.
//----- Author: Andrey G.
//-----------------------------------------------------------------

class KinectOutput
{
private:
	//---- Instance to our class (SINGLETON!)
	static KinectOutput * instance;

	//---- Private constructor for our singleton class.
	KinectOutput();

	//---- Output stream - save here information about files that contain color data
	//---- captured with the kinect device.
	//---- Index below represents the number of previosly stored files.
	ofstream loggerFileColor;
	int logFileColorIndex;

	//---- Output stream - save here information about files that contain depth  data
	//---- captured with the kinect device.
	ofstream loggerFileDepth; 
	int logFileDepthIndex;

	//---- Output stream - save here information about files that contain range data
	//---- captured with the kinect device
	ofstream loggerFileRange;
	int logFileRangeIndex;

	//---- Output stream - save joint information, skeleton. 
	//---- MAIN OUTPUT STREAM FOR SKELETON JOINTS!!!
	ofstream loggerSkeletonJoint;

	//---- Save one skeleton, this is a part of public function
	void saveKinectSkeleton(SkeletonBody skeleton);
public:

	static KinectOutput * getInstance();

	//---- Initialize the output
	//---- In [pathFileColor] - path to the log file, that will have info about saved color files
	//---- In [pathFileDpeth] - path to the log file, that will have info about saved depth (image) files
	//---- In [pathFileRange] - path to the log file, that will have info about saved range (depth binary) files
	void init(string pathFileColor, string pathFileDepth, string pathFileRange);

	void loggerSkeletonJointOPEN (string path);
	void loggerSkeletonJointCLOSE ();

	//--------------------------------------------------------------------------------------

	//---- Save color data captured with Kinect as an image
	//---- In [data] - color bytes to save
	//---- In [width] - width of the image 
	//---- In [height] - height of the image
	//---- In [path] - path of the files
	void saveKinectColor(byte * data, int width, int height, string path, bool isLog = false, double timestamp = 0);

	//---- Save depth data captured with Kinect as an image
	//---- In [data] - depth bytes to save
	//---- In [width] - width of the image 
	//---- In [height] - height of the image
	//---- In [path] - path of the files
	void saveKinectDepth(byte * data, int width, int height, string path, bool isLog = false, double timestamp = 0);

	//---- Save depth data captured with Kinect as a binary file
	//---- In [data] - range data (in millimeters) to save
	//---- In [width] - width of the image 
	//---- In [height] - height of the image
	//---- In [path] - path of the files
	void saveKinectRange(unsigned short * data, int width, int height, string path, bool isLog = false, double timestamp = 0);

	//---- Save skeleton data captured with Kinect (one skeleton, or all)
	void saveKinectSkeleton(SkeletonBody * skeletonPool, int timestamp);

	//---- Save skeleton data as an image
	void saveKinectSkeletonImage(SkeletonBody * skeletonPool, int width, int height, string path);

	//--------------------------------------------------------------------------------------

	int getCountSavedFileColor();
	int getCountSavedFileDepth();
	int getCountSavedFileRange();

};


#endif