#pragma once

#include <map>

#include "ofMain.h"
#include "ofxKinectForWindows2.h"

#include "SkeletonTracker.h"
#include "SkeletonBodyV2.h"

using namespace ofxKFW2;
using namespace std;

//--------------------------------------------------------------------------------
//---- Implementation of the skeleton tracker for the kinect V2
//---- The implementation uses ofxKinectForWindows2
//---- Author: Andrey G.
//--------------------------------------------------------------------------------

class TrackerKinectV2 : public SkeletonTracker
{
	//---- Kinect resolution properties
	const int DEPTH_WIDTH = 512;
	const int DEPTH_HEIGHT = 424;
	const int DEPTH_SIZE = DEPTH_WIDTH * DEPTH_HEIGHT;

	const int COLOR_WIDTH = 1920;
	const int COLOR_HEIGHT = 1080;

	const int MAX_BODY_COUNT = 6;

	//---- This is a dictionary for correct conversion before IDs used in OF library
	//---- and our internal SkeletonBody class
	const map  <JointType, int> DICTIONARY =
	{
		{JointType::JointType_AnkleLeft, JOINT_ID_KINECTV2::JOINT_LEFT_ANKLE},
		{JointType::JointType_AnkleRight, JOINT_ID_KINECTV2::JOINT_RIGHT_ANKLE},

		{JointType::JointType_ElbowLeft,JOINT_ID_KINECTV2::JOINT_LEFT_ELBOW},
		{JointType::JointType_ElbowRight, JOINT_ID_KINECTV2::JOINT_RIGHT_ELBOW},

		{JointType::JointType_FootLeft, JOINT_ID_KINECTV2::JOINT_LEFT_FOOT},
		{JointType::JointType_FootRight, JOINT_ID_KINECTV2::JOINT_RIGHT_FOOT},

		{JointType::JointType_HandLeft, JOINT_ID_KINECTV2::JOINT_LEFT_HAND},
		{JointType::JointType_HandRight, JOINT_ID_KINECTV2::JOINT_RIGHT_HAND},

		{JointType::JointType_HandTipLeft, JOINT_ID_KINECTV2::JOINT_LEFT_TIP_HAND},
		{JointType::JointType_HandTipRight, JOINT_ID_KINECTV2::JOINT_RIGHT_TIP_HAND},

		{JointType::JointType_Head, JOINT_ID_KINECTV2::JOINT_HEAD},

		{JointType::JointType_HipLeft, JOINT_ID_KINECTV2::JOINT_LEFT_HIP},
		{JointType::JointType_HipRight, JOINT_ID_KINECTV2::JOINT_RIGHT_HIP},

		{JointType::JointType_KneeLeft, JOINT_ID_KINECTV2::JOINT_LEFT_KNEE},
		{JointType::JointType_KneeRight, JOINT_ID_KINECTV2::JOINT_RIGHT_KNEE},

		{JointType::JointType_Neck, JOINT_ID_KINECTV2::JOINT_NECK},

		{JointType::JointType_ShoulderLeft, JOINT_ID_KINECTV2::JOINT_LEFT_SHOULDER},
		{JointType::JointType_ShoulderRight, JOINT_ID_KINECTV2::JOINT_RIGHT_SHOULDER},

		{JointType::JointType_SpineBase, JOINT_ID_KINECTV2::JOINT_SPINE_BASE},
		{JointType::JointType_SpineMid, JOINT_ID_KINECTV2::JOINT_SPINE_MID},
		{JointType::JointType_SpineShoulder, JOINT_ID_KINECTV2::JOINT_SPINE_SHOULDER},

		{JointType::JointType_ThumbLeft, JOINT_ID_KINECTV2::JOINT_LEFT_THUMB},
		{JointType::JointType_ThumbRight, JOINT_ID_KINECTV2::JOINT_RIGHT_THUMB},

		{JointType::JointType_WristLeft, JOINT_ID_KINECTV2::JOINT_LEFT_WRIST},
		{JointType::JointType_WristRight, JOINT_ID_KINECTV2::JOINT_RIGHT_WRIST},
	};

	//---- These is the kinect device.
	Device kinect;

	//---- This class handles transformation from depth coordinates
	//---- to color image coordinates. Useful when building masks
	ICoordinateMapper* kinectCoordinateMapper;
	vector<ofVec2f> colorCoords;

	//---- Streams of data
	shared_ptr<Source::Color> kinectSourceColor;
	shared_ptr<Source::Depth> kinectSourceDepth;
	shared_ptr<Source::Body> kinectSourceBody;
	shared_ptr<Source::BodyIndex> kinectSourceBodyIndex;

	//---- This flag signifies if there is any signal from Kinect
	bool isKinectOK;
	bool isDeviceClosed;

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

	//---- This implementation does not use the RGB, DPT, BODY (they are taken on the fly)
	//---- Mainly designed for compatibility with the Kinect V1
	bool generateImageChromakey(ofPixels  & dataChromakey, ofPixels & dataRGB, ofShortPixels & dataDPT, ofPixels & dataBODY);

	virtual bool close();
};