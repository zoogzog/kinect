#ifndef KINECTDEVICE_H
#define KINECTDEVICE_H

#include <windows.h>


#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

#include "SkeletonBody.h"

//-----------------------------------------------------------------
//----- This class handles all queries to the Kinect Device v.1
//----- This is a singleton class (only one can exist in the code.
//----- Author: Andrey G.
//-----------------------------------------------------------------

class KinectDevice
{

private: 
	//---- Instance declaration for the singleton
	static KinectDevice * instance;

	//---- Private constructor for the singleton
	KinectDevice();

	//---- Main link to the Kinect Sensor (v.1)
	INuiSensor * sensor;  

	//---- Kinect streams
	HANDLE streamRGB; 
	HANDLE streamDEPTH;
	HANDLE streamSKELETON;

	HANDLE eventStreamRGB;
	HANDLE eventStreamDEPTH;
	HANDLE eventStreamSKELETON;

	//---- Kinect status 
	int STATUS_FLAG;


	//---- Flag, shows if the device was inited
	bool isInit;

public: 

	//---- Default resolutions used to transform Kinect coordinates
	//---- Into coordinates to display on a depth image. DON"T TOUCH THIS!!!
	static const int DEFAULT_TRANSFORM_WIDTH = 320;
	static const int DEFAULT_TRANSFORM_HEIGHT = 240;

	//----------------------------------------
	//---- Max number of skeleton the sensor can track 
	static const int SKELETON_MAX_COUNT = NUI_SKELETON_COUNT;

	//---- Default resolution of image streams.
	//---- Kinect v1 color resolution 640*480
	//---- Kinect v1 depth resolution 640*480
	static const int DEFAULT_WIDTH = 640;
	static const int DEFAULT_HEIGHT = 480;

	//---- Status flags of the Kinect sensor
	static const int STATUS_OK = 0;
	static const int STATUS_NOSENSORFOUND = 1;
	static const int STATUS_FAILCREATE = 2;
	static const int STATUS_FAILSTATUS = 3;
	static const int STATUS_FAILINIT = 4;
	static const int STATUS_FAILFRAMERGB = 5;
	static const int STATUS_FAILFRAMEDPTH = 6;
	static const int STATUS_FAILFRAMESKLT = 7;

	static const int STATUS_OSTREAM_COLOR = 10;
	static const int STATUS_OSTREAM_DEPTH = 11;
	static const int STATUS_OSTREAM_SKELETON = 12;

	static const int STATUS_WAIT_COLOR = 101;
	static const int STATUS_WAIT_DEPTH = 102;
	static const int STATUS_WAIT_SKELETON = 103;
	
	//---- Code error, call init first, before using other methods!
	static const int STATUS_NOT_INIT = 500;
	//----------------------------------------

    static KinectDevice* getInstance();

	//---- Initialize the Kinect sensor. If can't return false.
	//---- Check the status flag (STATUS_FLAG) to understand what went wrong.
	bool init();

	//---- Returns the status of the Kinect sensor. 
	int getKinectStatus ();

	//---- Grabs color frame of the size [width*height*4]. 
	//---- Returns false, if fails, sets status flag.
	//---- Out [dest] - color data, array size of DEFAULT_WIDTH * DEFAULT_HEIGHT
	bool getKinectFrameColor (byte * dest);

	//---- Grabs depth frame converted to intensities %256. Size is [width*height*4].
	//---- Returns false if fails, sets status flag.
	//---- Out [dest] - depth data, array size of DEFAULT_WIDTH * DEFAULT_HEIGHT
	bool getKinectFrameDepth (byte * dest);

	//---- Grabs depth frame without converstion. Each element of the 'dest' array
	//---- [width*height] is the distance to object in millimeters. 
	//---- Returns false if fails, sets status flag.
	//---- Out [dest] - range data, array size of DEFAULT_WIDTH * DEFAULT_HEIGHT
	bool getKinectFrameRange (unsigned short * dest);
	
	//---- Grab the all possible skeletons. The size of the skeleton pool should 
	//---- be of KinectDevice::SKELETON_MAX_COUNT (by default this value is 6)
	//---- Returns false if fails, sets status flag.
	//---- Out [skeletonPool] - skeleton data, array, size of SKELETON_MAX_COUNT
	bool getKinectSkeleton (SkeletonBody * skeletonPool);

	//---- Call this function before destroying the kinect instance.
	//---- Singleton - normal destructor will spoil evertyhing.
	//---- Returns true, if the kinect was previously inited (and released)
	//---- Returns false, if the kinect was not inited before
	bool release ();

	//----------------------------------------

	SkeletonBody static convertSkeletonCoordinates(SkeletonBody skeletonIN, int width, int height);


	//----------------------------------------

	HANDLE & getEventStreamRGB ();
	HANDLE & getEventStreamDEPTH ();
	HANDLE & getEventStreamSKELETON ();
	

};


#endif 



