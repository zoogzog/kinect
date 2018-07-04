#include "ThreadSkeletonTracker.h"

#include "SkeletonTrackerUtils.h"


ThreadSkeletonTracker::ThreadSkeletonTracker()
{
	pointerSktrackerRGB = NULL;
	pointerSktrackerDPT = NULL;
	pointerSktrackerBODY = NULL;
	pointerSktrackerDevice = NULL;
	pointerSktrackerBodyCollection = NULL;
}

ThreadSkeletonTracker::~ThreadSkeletonTracker()
{
	pointerSktrackerRGB = NULL;
	pointerSktrackerDPT = NULL;
	pointerSktrackerBODY = NULL;
	pointerSktrackerDevice = NULL;
	pointerSktrackerBodyCollection = NULL;

}

void ThreadSkeletonTracker::setup(SkeletonTracker * device, ofPixels * dataRGB, ofPixels * dataDPT, ofPixels * dataBODY, SkeletonBodyCollection * dataSKL)
{
	pointerSktrackerDevice = device;
	pointerSktrackerBodyCollection = dataSKL;
	pointerSktrackerRGB = dataRGB;
	pointerSktrackerDPT = dataDPT;
	pointerSktrackerBODY = dataBODY;
}

void ThreadSkeletonTracker::threadedFunction()
{
	if (pointerSktrackerRGB == NULL || pointerSktrackerDPT == NULL || pointerSktrackerBodyCollection == NULL || pointerSktrackerDevice == NULL) { return;  }

	bool isOpen = pointerSktrackerDevice->init();

	while (isThreadRunning())
	{
		pointerSktrackerDevice->update();

		ofPixels bufferRGB;
		ofShortPixels bufferDPT;
		ofPixels bufferBODY;

		bool isCapturedRGB = pointerSktrackerDevice->getframeRGB(bufferRGB);
		bool isCapturedDPT = pointerSktrackerDevice->getframeDPT(bufferDPT);
		bool isCapturedBODY = pointerSktrackerDevice->getframeBODY(bufferBODY);
		bool isCapturedSKL = pointerSktrackerDevice->getframeSKL(*pointerSktrackerBodyCollection);

		if (isCapturedRGB) { pointerSktrackerRGB->setFromPixels(bufferRGB.getPixels(), bufferRGB.getWidth(), bufferRGB.getHeight(), ofImageType::OF_IMAGE_COLOR_ALPHA); }
		//---- Here we want to conver RANGE data into grayscale
		if (isCapturedDPT) 
		{ 
			ofPixels imgdpt = SkeletonTrackerUtils::convertRangeToGrayscale(bufferDPT);
			pointerSktrackerDPT->setFromPixels(imgdpt.getPixels(), imgdpt.getWidth(), imgdpt.getHeight(), ofImageType::OF_IMAGE_GRAYSCALE); 
		}
		if (isCapturedBODY) { pointerSktrackerBODY->setFromPixels(bufferBODY.getPixels(), bufferBODY.getWidth(), bufferBODY.getHeight(), ofImageType::OF_IMAGE_GRAYSCALE); }
		
	}
}