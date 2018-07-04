#pragma once

#include "SkeletonBody.h"

//--------------------------------------------------------------------------------
//---- Implementation of the skeletong body for the Kinect V1 device.
//---- Author: Andrey G.
//--------------------------------------------------------------------------------

namespace JOINT_ID_KINECTV1
{
	static const int JOINT_HIP_CENTER = 0;
	static const int JOINT_SPINE = 1;
	static const int JOINT_SHOULDER_CENTER = 2;
	static const int JOINT_HEAD = 3;

	static const int JOINT_LEFT_SHOULDER = 4;
	static const int JOINT_LEFT_ELBOW = 5;
	static const int JOINT_LEFT_WRIST = 6;
	static const int JOINT_LEFT_HAND = 7;

	static const int JOINT_RIGHT_SHOULDER = 8;
	static const int JOINT_RIGHT_ELBOW = 9;
	static const int JOINT_RIGHT_WRIST = 10;
	static const int JOINT_RIGHT_HAND = 11;

	static const int JOINT_LEFT_HIP = 12;
	static const int JOINT_LEFT_KNEE = 13;
	static const int JOINT_LEFT_ANKLE = 14;
	static const int JOINT_LEFT_FOOT = 15;

	static const int JOINT_RIGHT_HIP = 16;
	static const int JOINT_RIGHT_KNEE = 17;
	static const int JOINT_RIGHT_ANKLE = 18;
	static const int JOINT_RIGHT_FOOT = 19;

	static const int ATLAS_SIZE = 19;

	static const int JOINT_COUNT = 20;

	static const BONE ATLAS[]
	{
		//---- Head & torso
		{ JOINT_HEAD, JOINT_SHOULDER_CENTER },
		{ JOINT_SHOULDER_CENTER, JOINT_LEFT_SHOULDER },
		{ JOINT_SHOULDER_CENTER, JOINT_RIGHT_SHOULDER },
		{ JOINT_SHOULDER_CENTER, JOINT_SPINE },
		{ JOINT_SPINE, JOINT_HIP_CENTER },
		{ JOINT_HIP_CENTER, JOINT_LEFT_HIP },
		{ JOINT_HIP_CENTER, JOINT_RIGHT_HIP },

		//----- Left arm
		{ JOINT_LEFT_SHOULDER, JOINT_LEFT_ELBOW },
		{ JOINT_LEFT_ELBOW, JOINT_LEFT_WRIST },
		{ JOINT_LEFT_WRIST, JOINT_LEFT_HAND },

		//----- Right arm
		{ JOINT_RIGHT_SHOULDER, JOINT_RIGHT_ELBOW },
		{ JOINT_RIGHT_ELBOW, JOINT_RIGHT_WRIST },
		{ JOINT_RIGHT_WRIST, JOINT_RIGHT_HAND },

		//----- Left leg
		{ JOINT_LEFT_HIP, JOINT_LEFT_KNEE },
		{ JOINT_LEFT_KNEE, JOINT_LEFT_ANKLE },
		{ JOINT_LEFT_ANKLE, JOINT_LEFT_FOOT },

		//----- Right leg
		{ JOINT_RIGHT_HIP, JOINT_RIGHT_KNEE },
		{ JOINT_RIGHT_KNEE, JOINT_RIGHT_ANKLE },
		{ JOINT_RIGHT_ANKLE, JOINT_RIGHT_FOOT }
	};
};

//--------------------------------------------------------------------------------

class SkeletonBodyV1 : public SkeletonBody
{
private:
	//--------------- VARIABLES --------------
	bool isTracked;

	//---- Skeleton joint vertexes (x,y,z), w - used for storing tracking state
	vector4f * skeletonPoints;
	//---- Skeleton joint vertexes (x,y,z) in world coordinates
	vector3f * skeletonPointsWC;

	float RANGE_MAX_X = 1;
	float RANGE_MAX_Y = 1;
	float RANGE_MAX_Z = 1;

	int handstateLEFT = HAND_STATE_UNKNOWN;
	int handstateRIGHT = HAND_STATE_UNKNOWN;

public:
	SkeletonBodyV1();
	SkeletonBodyV1(const SkeletonBodyV1 &skeleton);
	~SkeletonBodyV1();

	//---- Set vertex coordinate limis (specific to each tracking device)
	void setMaxCordinates(float maxx, float maxy, float maxz);

	//---- Get atlas that describes a skeleton.
	//---- The atlas is device specific
	vector <BONE>  getSkeletonAtlasDescription();

	//---- Get all skeleton joints
	vector <vector4f> getSkeletonJoints(bool isNormed = false);

	//---- Get a specific skeleton joint defined by index
	vector4f getSkeletonJoints(int index, bool isNormed = false);

	//---- Set coordinates of a specific joint
	void setSkeletonJoint(vector4f joint, int jointID);

	//---- Manipulation with the skeleton tracking state
	bool getIsTracked();
	void setIsTracked(bool value);

	//---- Set the hand state, if device does not obtain state set HAND_STATE_UNKNOWN
	void setHandStateLEFT(int state);
	void setHandStateRIGHT(int state);

	int getHandStateLEFT();
	int getHandStateRIGHT();


	vector4f getJointHandLEFT(bool isNormed = false);
	vector4f getJointHandRIGHT(bool isNormed = false);

	//---- Get the position of the body center (JOINT_HIP_CENETER for kinect v1)
	vector4f getJointBodyCenter(bool isNormed = false);

	//---- Get the range of joint coordinates xmin, ymin, xmax, ymax
	vector4f getJointCoordinateRange(bool isNormed = false);

	int getJointCount();

	void setSkeletonJointWC(vector3f joint, int jointID);
	vector3f getSkeletonJointWC(int jointID);
};