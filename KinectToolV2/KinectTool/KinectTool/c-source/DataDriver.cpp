#include "DataDriver.h"

#include "SkeletonTrackerFactory.h"

//--------------------------------------------------------------

DataDriver::DataDriver()
{
	sktrackerDevice = SkeletonTrackerFactory::getDevice(TrackerDeviceType::DEVICE_KINECT_V2);
	sktrackerBodyCollection = new SkeletonBodyCollection(TrackerDeviceType::DEVICE_KINECT_V2, sktrackerDevice->getSettingsSkeletonCount());

	sktrackerRGB = new ofPixels();
	sktrackerRGB->allocate(sktrackerRGB->getWidth(), sktrackerRGB->getHeight(), ofImageType::OF_IMAGE_COLOR_ALPHA);

	sktrackerDPT = new ofPixels();
	sktrackerDPT->allocate(sktrackerDPT->getWidth(), sktrackerDPT->getHeight(), ofImageType::OF_IMAGE_GRAYSCALE);

	sktrackerBODY = new ofPixels();
	sktrackerBODY->allocate(sktrackerBODY->getWidth(), sktrackerBODY->getHeight(), ofImageType::OF_IMAGE_GRAYSCALE);

	threadSkeletonTracker = new ThreadSkeletonTracker();
}

DataDriver::~DataDriver()
{
	if (sktrackerDevice != NULL) { sktrackerDevice->close(); delete sktrackerDevice; sktrackerDevice = NULL; }
	if (sktrackerBodyCollection != NULL) { sktrackerBodyCollection->clear(); delete sktrackerBodyCollection; sktrackerBodyCollection = NULL; }
	if (sktrackerRGB != NULL) { delete sktrackerRGB; sktrackerRGB = NULL; }
	if (sktrackerDPT != NULL) { delete sktrackerDPT; sktrackerDPT = NULL; }
	if (sktrackerBODY != NULL) { delete sktrackerBODY; sktrackerBODY = NULL; }

	if (threadSkeletonTracker != NULL) { threadSkeletonTracker->stopThread(); threadSkeletonTracker = NULL; }
}

DataDriver & DataDriver::getInstance()
{
	static DataDriver instance;
	return instance;
}

//--------------------------------------------------------------

SkeletonTracker * DataDriver::getObjectSkeletonTrackerDevice()
{
	return sktrackerDevice;
}

SkeletonBodyCollection * DataDriver::getObjectSkeletonTrackerBodyCollection()
{
	return sktrackerBodyCollection;
}

ofPixels * DataDriver::getObjectSkeletonTrackerDataRGB()
{
	return sktrackerRGB;
}

ofPixels * DataDriver::getObjectSkeletonTrackerDataDPT()
{
	return sktrackerDPT;
}

ofPixels * DataDriver::getObjectSkeletonTrackerDataBODY()
{
	return sktrackerBODY;
}

ThreadSkeletonTracker * DataDriver::getObjectThreadSkeletonTracker()
{
	return threadSkeletonTracker;
}