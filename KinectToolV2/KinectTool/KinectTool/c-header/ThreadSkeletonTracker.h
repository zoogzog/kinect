#pragma once

#include "ofMain.h"

#include "SkeletonTracker.h"
#include "SkeletonBodyCollection.h"

class ThreadSkeletonTracker : public ofThread
{
private:
	//---- Device that will captured the data
	SkeletonTracker * pointerSktrackerDevice;

	//---- Where the captured data will be stored
	ofPixels * pointerSktrackerRGB;
	ofPixels * pointerSktrackerDPT;
	ofPixels * pointerSktrackerBODY;

	SkeletonBodyCollection * pointerSktrackerBodyCollection;

public:
	ThreadSkeletonTracker();
	~ThreadSkeletonTracker();

	void setup(SkeletonTracker * device, ofPixels * dataRGB, ofPixels * dataDPT, ofPixels * dataBODY, SkeletonBodyCollection * dataSKL);

	void threadedFunction();
	

};
