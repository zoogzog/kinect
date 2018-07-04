#pragma once

#include "ofMain.h"

#include "ThreadSkeletonTracker.h"

#include "SkeletonBodyCollection.h"
#include "SkeletonTracker.h"

class DataDriver
{
private:
	//---------------------------------------------
	//----- SKELETON TRACKER
	//---------------------------------------------
	SkeletonTracker * sktrackerDevice;
	SkeletonBodyCollection * sktrackerBodyCollection;

	ofPixels * sktrackerRGB;
	ofPixels * sktrackerDPT;
	ofPixels * sktrackerBODY;

	//---------------------------------------------
	//----- THREAD
	//---------------------------------------------

	ThreadSkeletonTracker * threadSkeletonTracker;



	//---------------------------------------------
	DataDriver();

public:

	//---------------------------------------------
	~DataDriver();
	static DataDriver & getInstance();
	//---------------------------------------------

	SkeletonTracker * getObjectSkeletonTrackerDevice();
	SkeletonBodyCollection * getObjectSkeletonTrackerBodyCollection();
	ofPixels * getObjectSkeletonTrackerDataRGB();
	ofPixels * getObjectSkeletonTrackerDataDPT();
	ofPixels * getObjectSkeletonTrackerDataBODY();

	ThreadSkeletonTracker * getObjectThreadSkeletonTracker();
};