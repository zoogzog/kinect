#pragma once

#include <map>

#include <windows.h>

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

#include <ofMain.h>

#include "SkeletonBodyCollection.h"
#include "SkeletonBody.h";
#include "SkeletonTracker.h";

//--------------------------------------------------------------------------------
//---- Implementation of the skeleton tracker for the kinect V1
//---- The implementation uses NUI windows library
//---- Author: Andrey G.
//--------------------------------------------------------------------------------

class TrackerKinectV1 : public SkeletonTracker
{
private:
	const int DEPTH_WIDTH = 640;
	const int DEPTH_HEIGHT = 480;
	const int DEPTH_SIZE = DEPTH_WIDTH * DEPTH_HEIGHT;

	const int COLOR_WIDTH = 640;
	const int COLOR_HEIGHT = 480;

	const int MAX_BODY_COUNT = 6;

	//---- Main link to the Kinect Sensor (v.1)
	INuiSensor * sensor;

	//---- Kinect streams
	HANDLE streamRGB;		//<---- Stream of color images
	HANDLE streamDEPTH;		//<---- Stream of depth (range) data
	HANDLE streamSKELETON;	//<---- Stream of tracking skeleton joints

	HANDLE eventStreamRGB;
	HANDLE eventStreamDEPTH;
	HANDLE eventStreamSKELETON;

	bool isInit;
	bool isDeviceClosed;

	//---- Because in kinect v1 depth and body mask are captured together
	//---- Have to save temporary result somewhere
	ofPixels tempMaskData;
	//---- Switcher (if mask was updated set to true, read it, then set to false)
	bool isMaskUpdated = false; 

public:
	int getSettingsSkeletonCount();
	
	bool init();

	bool update();

	//---- Get resolution of the RGB or depth streams
	int getWidthRGB();
	int getWidthDPT();
	int getHeightRGB();
	int getHeightDPT();

	//---- Get data, if can't obtain data return false, otherwise true.
	bool getframeRGB(ofPixels & data);
	bool getframeDPT(ofShortPixels & data);
	bool getframeSKL(SkeletonBodyCollection & skeletonCollection);
	bool getframeBODY(ofPixels & data);

	bool generateImageChromakey(ofPixels  & dataChromakey, ofPixels & dataRGB, ofShortPixels & dataDPT, ofPixels & dataBODY);

	virtual bool close();
};
