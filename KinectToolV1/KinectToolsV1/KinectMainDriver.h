#ifndef KINECTGAMEDRIVER_H
#define KINECTGAMEDRIVER_H

#include <string>

#include <Windows.h>

#include "KinectDevice.h"
#include "KinectOutput.h"


//-----------------------------------------------------------------
//---- Main class for the c-game
//---- Author: Andrey G.
//-----------------------------------------------------------------

class KinectMainDriver
{
private:
	//----------------------------------------
	static KinectMainDriver * instance;

	KinectMainDriver();
	//----------------------------------------

	KinectDevice * kinectDevice;
	KinectOutput * kinectOutput;

	//---- These events are needed to check status of captured frames
	HANDLE hevents[4];

	//---- Skeleton data
	SkeletonBody skeletonPool[KinectDevice::SKELETON_MAX_COUNT];

	//---- Depth data captured as image
	byte dataDepth[KinectDevice::DEFAULT_WIDTH*KinectDevice::DEFAULT_HEIGHT * 4];

	//---- Color data captured as image
	byte dataColor[KinectDevice::DEFAULT_WIDTH*KinectDevice::DEFAULT_HEIGHT * 4];

	byte dataBodyMask[KinectDevice::DEFAULT_WIDTH*KinectDevice::DEFAULT_HEIGHT];

	//---- Range data 
	unsigned short dataRange[KinectDevice::DEFAULT_WIDTH*KinectDevice::DEFAULT_HEIGHT];

	//----------------------------------------
	string PATH_DIRECTORY_RGB = "";
	string PATH_DIRECTORY_DPT = "";
	string PATH_DIRECTORY_DPTI = "";
	string PATH_DIRECTORY_SKLI = "";
	string PATH_DIRECTORY_SKL = "";

	//---- Flage - true if this class was initialized
	bool isInit;

	//---- Flags - true if capturing 
	//---- RGB - color images
	//---- DPT - depth binary files
	//---- SKL - skeleton data as text file
	//---- DPTI - depth as image
	//---- SKLI - skeleton as image
	bool isCaptureRGB;
	bool isCaptureDPT;
	bool isCaptureSKL;
	bool isCaptureDPTI;
	bool isCaptureSKLI;

	//----------------------------------------

	void captureFrameRGB(double timestamp = 0);
	void captureFrameDPT(double timestamp = 0);
	void captureFrameSKL(double timestamp = 0);
	void captureFrameDPTI(double timestamp = 0);

public:
	//----------------------------------------

	static KinectMainDriver * getInstance();
	
	//----------------------------------------
	//---- Initialize the main driver
	//---- In [pathDirRGB] ----- path to the DIRECTORY that will contain colored images
	//---- In [pathDirDPT] ----- path to the DIRECTORY that will conatin depth binary files
	//---- <In (optional) [pathDirDPTI] -- DEBUG!!! path to the DIRECTORY that will contain depth image files
	//---- <In (optional) [pathDirSKLI] -- DEBUG!!! path to the DIRECTORY that will contain skeleton image files
	//---- In [pathFileSKL] ---- path to the FILE that will contain skeleton data
	//---- In [pathLoggerRGB] -- path to the FILE that will contain LOG info about saved color files
	//---- In [pathLoggerDPT] -- path to the FILE that will contain LOG info about saved depth (range) files
	//---- In [isRGB] - flag, set to true if want to capture color
	//---- In [isDPT] - flag, set to true if want to capture depth binary
	//---- <In (optional) [isDPTI] -- flag, set to true if want to capture depth image
	//---- <In (optional) [isSKLI] -- flag, set to true if want to capture skeleton as image (set SKL to true!)
	//---- In [isSKL] - flag, set to true if capture skeleton

	bool init(string pathDirRGB, string pathDirDPT, string pathFileSKL, string pathLoggerRGB, string pathLoggerDPT, bool isRGB, bool isDPT, bool isSKL);
	bool init(string pathDirRGB, string pathDirDPT, string pathDirDPTI, string pathFileSKL, string pathLoggerRGB, string pathLoggerDPT, bool isRGB, bool isDPT, bool isDPTI, bool isSKL);
	bool init(string pathDirRGB, string pathDirDPT, string pathDirDPTI, string pathDirSKLI, string pathFileSKL, string pathLoggerRGB, string pathLoggerDPT, bool isRGB, bool isDPT, bool isDPTI, bool isSKLI, bool isSKL);
	void update(double timestamp = 0);
	void finalize();

	//----------------------------------------

	//---- Get kinect RGB data, size (640*480*4) 
	byte * getDataColor();

	//---- Get kinect DEPTH data as an RGB image, size (640*480*4)
	byte * getDataDepth();

	//---- Get a body silhouette (color), size (640*480*4)
	byte * getDataChromakey(bool isColor = true);

	//---- Get kinect DEPTH data, (in millimeters), size (640*480)
	unsigned short * getDataRange();

	//----- Get skeleton data (size of the array is 6)
	SkeletonBody * getDataSkeletonPool();

};


#endif 

