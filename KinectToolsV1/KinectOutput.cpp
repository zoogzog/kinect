#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "IOFileManager.h"
#include "KinectOutput.h"
#include "KinectDevice.h"

//-----------------------------------------------------------------

KinectOutput * KinectOutput::instance = 0;

KinectOutput::KinectOutput()
{
	logFileColorIndex = 0;
	logFileDepthIndex = 0;
	logFileRangeIndex = 0;
}

KinectOutput * KinectOutput::getInstance()
{
	if (instance == 0)
	{
		instance = new KinectOutput();
	}

	return instance;
}

//-----------------------------------------------------------------

void KinectOutput::init(string pathFileColor, string pathFileDepth, string pathFileRange)
{
	loggerFileColor = ofstream(pathFileColor);
	loggerFileDepth = ofstream(pathFileDepth);
	loggerFileRange = ofstream(pathFileRange);

	cout << pathFileRange << endl;

	logFileColorIndex = 0;
	logFileDepthIndex = 0;
	logFileRangeIndex = 0;
}

//-----------------------------------------------------------------

void KinectOutput::saveKinectColor(byte * data, int width, int height, string path, bool isLog, double timestamp)
{
	IOFileManager::saveImageBMP(data, width, height, path, IOFileManager::METHOD_WINDOWS);

	string fileName = path.substr(path.find_last_of('/') + 1) + ".bmp";
	if (isLog) { loggerFileColor << logFileColorIndex << "," << timestamp << "," << fileName << endl; }

	logFileColorIndex++;

}

void KinectOutput::saveKinectDepth(byte * data, int width, int height, string path, bool isLog, double timestamp)
{
	IOFileManager::saveImageBMP(data, width, height, path, IOFileManager::METHOD_WINDOWS);

	string fileName = path.substr(path.find_last_of('/') + 1) + ".bmp";
	if (isLog) { loggerFileDepth << logFileDepthIndex << "," << timestamp << "," << fileName << endl; }

	logFileDepthIndex++;
}

void KinectOutput::saveKinectRange(unsigned short * data, int width, int height, string path, bool isLog, double timestamp)
{
	IOFileManager::saveImageBIN(data, width, height, path, IOFileManager::METHOD_LZ4);

	string fileName = path.substr(path.find_last_of('/') + 1) + ".lz4"; 
	if (isLog) { loggerFileRange << logFileRangeIndex << "," << timestamp << "," << fileName << endl; }

	logFileRangeIndex++;
}

void KinectOutput::saveKinectSkeleton(SkeletonBody skeleton)
{
	for (int i = 0; i < SkeletonBody::MAX_SKELETON_JOINT; i++)
	{
		loggerSkeletonJoint << 
			skeleton.skeletonPosition[i].w << ", " << skeleton.skeletonPosition[i].x << ", " <<
			skeleton.skeletonPosition[i].y << ", " << skeleton.skeletonPosition[i].z << ", " << endl;
	}

	//---- Here we put joints that are not existed in Kinect v1 but present in Kinect v2

	//---- JOINT = SPINE-SHOULDE (v2) -> SHOULDER_CENTER (v1) (index = 2)
	loggerSkeletonJoint <<
		skeleton.skeletonPosition[2].w << ", " << skeleton.skeletonPosition[2].x << ", " <<
		skeleton.skeletonPosition[2].y << ", " << skeleton.skeletonPosition[2].z << endl;

	//---- JOINT = LEFT-TIP-HAND
	loggerSkeletonJoint << 0 << ", " << 0 << ", " << 0 << ", " << 0 << endl;

	//---- JOINT = LEFT-THUMB
	loggerSkeletonJoint << 0 << ", " << 0 << ", " << 0 << ", " << 0 << endl;

	//---- JOINT = RIGHT-TIP-HAND
	loggerSkeletonJoint << 0 << ", " << 0 << ", " << 0 << ", " << 0 << endl;

	//---- JOINT = RIGHT-THUMB
	loggerSkeletonJoint << 0 << ", " << 0 << ", " << 0 << ", " << 0 << endl;

	//DEBUG
	//loggerSkeletonJoint.flush();
}

void KinectOutput::saveKinectSkeleton(SkeletonBody  * skeletonPool, int timestamp)
{
	if (!loggerSkeletonJoint.is_open()) { return; }

	//---- Count detected skeletons
	int countSkeletonDetected = 0;
	for (int i = 0; i < KinectDevice::SKELETON_MAX_COUNT; i++) { if (skeletonPool[i].isTracked) { countSkeletonDetected++; } }

	loggerSkeletonJoint << timestamp << ", " << countSkeletonDetected << endl;

	for (int i = 0; i < KinectDevice::SKELETON_MAX_COUNT; i++)
	{
		if (skeletonPool[i].isTracked)
		{
			loggerSkeletonJoint << i << endl;

			saveKinectSkeleton(skeletonPool[i]);
		}
	}
}

//-----------------------------------------------------------------

void KinectOutput::saveKinectSkeletonImage(SkeletonBody * skeletonPool, int width, int height, string path)
{
	const int acsize = SkeletonBody::MAX_SKELETON_JOINT * 2;
	const int adsize = SkeletonBody::SKELETON_ATLAS_SIZE;

	for (int i = 0; i < KinectDevice::SKELETON_MAX_COUNT; i++)
	{
		if (skeletonPool[i].isTracked)
		{
			SkeletonBody skeleton = KinectDevice::convertSkeletonCoordinates(skeletonPool[i], width, height);

			//---- Double size of joints to put both X,Y
			float jointAtlasCoordinates[acsize];
			int jointAtlasDescription[adsize];

			skeleton.getSkeletonAtlasCoordinates(jointAtlasCoordinates);
			skeleton.getSkeletonAtlasDescription(jointAtlasDescription);

			IOFileManager::saveSkeletonImage(jointAtlasCoordinates, acsize, jointAtlasDescription, adsize, width, height, path);
			
			return;
		}
	}
}

//-----------------------------------------------------------------

void KinectOutput::loggerSkeletonJointOPEN(string path)
{
	loggerSkeletonJoint.open(path);
}

void KinectOutput::loggerSkeletonJointCLOSE()
{
	loggerSkeletonJoint.close();
}

//-----------------------------------------------------------------


int KinectOutput::getCountSavedFileColor()
{
	return logFileColorIndex;
}

int KinectOutput::getCountSavedFileDepth()
{
	return logFileDepthIndex;
}

int KinectOutput::getCountSavedFileRange()
{
	return logFileRangeIndex;
}