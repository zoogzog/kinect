#include "Debugger.h"

#include "ofMain.h"

#include "DataDriver.h"

#include "VisualizerDriver.h"

void VisualizerDriver::draw()
{
	ofClear(0);

	driverKinect.draw();
}

void VisualizerDriver::setup(int width, int height)
{
	resolutionWidth = width;
	resolutionHeight = height;

	driverKinect.setup(
		DataDriver::getInstance().getObjectSkeletonTrackerDataRGB(), 0, 0, 256, 192,
		DataDriver::getInstance().getObjectSkeletonTrackerDataDPT(), 256, 0, 256, 192,
		DataDriver::getInstance().getObjectSkeletonTrackerDataBODY(), 0, 192, 256, 192,
		DataDriver::getInstance().getObjectSkeletonTrackerBodyCollection(), 256, 192, 256, 192);
}

void VisualizerDriver::update()
{
	driverKinect.update();
}

void VisualizerDriver::resize(int width, int height)
{
	resolutionWidth = width;
	resolutionHeight = height;
}
