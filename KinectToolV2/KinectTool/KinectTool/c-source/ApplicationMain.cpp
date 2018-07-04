#include "ofMain.h"

#include "DataDriver.h"
#include "ApplicationMain.h"

//--------------------------------------------------------------
ApplicationMain::ApplicationMain()
{

}


void ApplicationMain::setup()
{

	DataDriver::getInstance().getObjectSkeletonTrackerBodyCollection()->setMaxCordinates(ofGetWidth(), ofGetHeight(), 9000);

	//---- Launch the kinect tracker
	DataDriver::getInstance().getObjectThreadSkeletonTracker()->setup(
		DataDriver::getInstance().getObjectSkeletonTrackerDevice(),
		DataDriver::getInstance().getObjectSkeletonTrackerDataRGB(),
		DataDriver::getInstance().getObjectSkeletonTrackerDataDPT(),
		DataDriver::getInstance().getObjectSkeletonTrackerDataBODY(),
		DataDriver::getInstance().getObjectSkeletonTrackerBodyCollection()
	);

	DataDriver::getInstance().getObjectThreadSkeletonTracker()->startThread();

	//---- Initialize the visual driver
	vdriver.setup(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
//---- LOOP: UPDATE
//--------------------------------------------------------------

void ApplicationMain::update()
{
	vdriver.update();
}

//--------------------------------------------------------------
//---- LOOP: DRAW
//--------------------------------------------------------------
void ApplicationMain::draw()
{
	vdriver.draw();
}


//--------------------------------------------------------------

void ApplicationMain::keyPressed(int key) { }
void ApplicationMain::keyReleased(int key) { }
void ApplicationMain::mouseMoved(int x, int y) {}
void ApplicationMain::mouseDragged(int x, int y, int button) {}
void ApplicationMain::mousePressed(int x, int y, int button) {}
void ApplicationMain::mouseReleased(int x, int y, int button) {}
void ApplicationMain::windowResized(int w, int h) { }
void ApplicationMain::gotMessage(ofMessage msg) {}
void ApplicationMain::dragEvent(ofDragInfo dragInfo) {}

void ApplicationMain::exit()
{

}