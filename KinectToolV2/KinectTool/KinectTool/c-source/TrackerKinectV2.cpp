#include "ofMain.h"

#include "SkeletonBody.h"
#include "SkeletonBodyCollection.h"
#include "TrackerKinectV2.h"

using namespace std;

//--------------------------------------------------------------


int TrackerKinectV2::getSettingsSkeletonCount()
{
	return MAX_BODY_COUNT;
}

//--------------------------------------------------------------

bool TrackerKinectV2::init()
{
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initBodySource();
	kinect.initBodyIndexSource();

	if (kinect.getSensor()->get_CoordinateMapper(&kinectCoordinateMapper) < 0)
	{
		isKinectOK = false;
		return false;
	}

	kinectSourceColor = kinect.getColorSource();
	kinectSourceDepth = kinect.getDepthSource();
	kinectSourceBody = kinect.getBodySource();
	kinectSourceBodyIndex = kinect.getBodyIndexSource();

	colorCoords.resize(DEPTH_WIDTH * DEPTH_HEIGHT);;

	if (kinect.isOpen()) { isKinectOK = true; }
	else { isKinectOK = false; }

	isDeviceClosed = false;

	return true;
}


bool TrackerKinectV2::update()
{
	if (!kinect.isOpen()) { return false; }
	if (isDeviceClosed) { return false; }

	kinect.update();
	
	return true;
}

//--------------------------------------------------------------

int TrackerKinectV2::getWidthRGB()
{
	return COLOR_WIDTH;
}

int TrackerKinectV2::getWidthDPT()
{
	return DEPTH_WIDTH;
}

int TrackerKinectV2::getHeightRGB()
{
	return COLOR_HEIGHT;
}

int TrackerKinectV2::getHeightDPT()
{
	return DEPTH_HEIGHT;
}

//--------------------------------------------------------------

bool TrackerKinectV2::getframeRGB(ofPixels & data)
{
	if (isDeviceClosed) { return false; }
	if (!kinectSourceColor->isFrameNew()) { return false; }
	auto & pixColor = kinectSourceColor->getPixels();

	//---- Can't obtain color data
	if (!pixColor.size()) { return false; }

	data = pixColor;

	return true;
}

bool TrackerKinectV2::getframeDPT(ofShortPixels & data)
{
	if (isDeviceClosed) { return false; }
	if (!kinectSourceDepth->isFrameNew()) { return false;  }
	auto & pixDepth = kinectSourceDepth->getPixels();

	//---- Can't obtain depth data
	if (!pixDepth.size()) { return false; }

	data = pixDepth;

	return true;
}

bool TrackerKinectV2::getframeSKL(SkeletonBodyCollection & skeletonCollection)
{
	if (isDeviceClosed) { return false; }
	if (!kinectSourceBody->isFrameNew()) { return false; }

	vector<ofxKinectForWindows2::Data::Body> bodyList = kinectSourceBody->getBodies();

	if (bodyList.size() == 0) { return false; }

	int trackedBodyCount = 0;

	for (int i = 0; i < 6; i++)
	{
		auto body = bodyList.at(i);

		if (body.tracked) { trackedBodyCount++; }

		skeletonCollection.getSkeletonBody(i)->setIsTracked(body.tracked);

		for (auto jointID : body.joints)
		{
			ofVec2f j1 = jointID.second.getPositionInDepthMap();

			vector4f v; v.x = j1.x; v.y = j1.y;	v.z = 0;
			
			//---- Skeleton joint in world coordinates
			ofVec3f ovwc = jointID.second.getPosition();
			vector3f vWC(ovwc.x, ovwc.y, ovwc.z);


			//---- This is explicit setting, just to be sure
			if (jointID.second.getTrackingState() == TrackingState::TrackingState_Tracked) { v.w = 1; }
			else { v.w = 0; }

			int skeletonJointID = DICTIONARY.at(jointID.first);

			//---- Set data of the joint locations
			skeletonCollection.getSkeletonBody(i)->setSkeletonJoint(v, skeletonJointID);
			skeletonCollection.getSkeletonBody(i)->setSkeletonJointWC(vWC, skeletonJointID);
		}

		//---- Set hand state
		if (body.leftHandState == HandState::HandState_Open) { skeletonCollection.getSkeletonBody(i)->setHandStateLEFT(HAND_STATE::HAND_STATE_OPEN); }
		else if (body.leftHandState == HandState::HandState_Closed) { skeletonCollection.getSkeletonBody(i)->setHandStateLEFT(HAND_STATE::HAND_STATE_CLOSE); }
		else { skeletonCollection.getSkeletonBody(i)->setHandStateLEFT(HAND_STATE::HAND_STATE_UNKNOWN); }

		if (body.rightHandState == HandState::HandState_Open) { skeletonCollection.getSkeletonBody(i)->setHandStateRIGHT(HAND_STATE::HAND_STATE_OPEN); }
		else if (body.rightHandState == HandState::HandState_Closed) { skeletonCollection.getSkeletonBody(i)->setHandStateRIGHT(HAND_STATE::HAND_STATE_CLOSE); }
		else { skeletonCollection.getSkeletonBody(i)->setHandStateRIGHT(HAND_STATE::HAND_STATE_UNKNOWN); }

	}

	return true;
}

bool TrackerKinectV2::getframeBODY(ofPixels & data)
{
	if (!data.isAllocated()) { data.allocate(DEPTH_WIDTH, DEPTH_HEIGHT, ofImageType::OF_IMAGE_GRAYSCALE); }
	auto & pixDepth = kinectSourceBodyIndex->getPixels();

	//---- Can't obtain depth data
	if (!pixDepth.size()) { return false; }

	for (int y = 0; y < DEPTH_HEIGHT; y++)
	{
		for (int x = 0; x < DEPTH_WIDTH; x++)
		{
			int index = (y * DEPTH_WIDTH) + x;
			
			if (pixDepth[index] <= MAX_BODY_COUNT)
			{
				data[index] = 255;
			}
			else
			{
				data[index] = 0;
			}
		}
	}

	return true;
}

bool TrackerKinectV2::generateImageChromakey(ofPixels  & dataChromakey, ofPixels & dummy1, ofShortPixels & dummy2, ofPixels & dummy3)
{
	//---- Get data
	auto& dataDPT = kinect.getDepthSource()->getPixels();
	auto& dataBODY = kinect.getBodyIndexSource()->getPixels();
	auto& dataRGB = kinect.getColorSource()->getPixels();

	if (!dataDPT.isAllocated() || !dataRGB.isAllocated() || !dataBODY.isAllocated()) { return false; }

	ofImage imageCHROMA; 
	imageCHROMA.allocate(DEPTH_WIDTH, DEPTH_HEIGHT, OF_IMAGE_COLOR_ALPHA);
	
	kinectCoordinateMapper->MapDepthFrameToColorSpace(DEPTH_SIZE, (UINT16*)dataDPT.getPixels(), DEPTH_SIZE, (ColorSpacePoint*)colorCoords.data());

	// Loop through the depth image
	for (int y = 0; y < DEPTH_HEIGHT; y++)
	{
		for (int x = 0; x < DEPTH_WIDTH; x++) 
		{
			int index = (y * DEPTH_WIDTH) + x;

			imageCHROMA.setColor(x, y, ofColor(0,0,0,0));
			float val = dataBODY[index];
			if (val == 255) { continue; }

			//---- Map coordinates and convert to int
			ofVec2f mappedCoord = colorCoords[index];
			mappedCoord.x = floor(mappedCoord.x);
			mappedCoord.y = floor(mappedCoord.y);

			//----- Check the limits
			if (mappedCoord.x < 0 || mappedCoord.y < 0 || mappedCoord.x >= COLOR_WIDTH || mappedCoord.y >= COLOR_HEIGHT) {continue;}

			ofColor c = dataRGB.getColor(mappedCoord.x, mappedCoord.y);

			imageCHROMA.setColor(x, y, ofColor(c.r, c.g, c.b, 255));
		}
	}

	dataChromakey = imageCHROMA.getPixels();
	return true;
}


bool TrackerKinectV2::close()
{
	isDeviceClosed = true;

	kinect.close();

	return true;
}