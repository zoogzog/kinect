#include <iostream>

#include "SkeletonBodyV1.h"

using namespace std;
//--------------------------------------------------------------------------------
//---- CONSTRUCTORS
//--------------------------------------------------------------------------------

SkeletonBodyV1::SkeletonBodyV1()
{
	skeletonPoints = new vector4f[JOINT_ID_KINECTV1::JOINT_COUNT];
	skeletonPointsWC = new vector3f[JOINT_ID_KINECTV1::JOINT_COUNT];

	isTracked = false;
}

SkeletonBodyV1::SkeletonBodyV1(const SkeletonBodyV1 &skeleton)
{
	this->isTracked = skeleton.isTracked;

	skeletonPoints = new vector4f[JOINT_ID_KINECTV1::JOINT_COUNT];
	skeletonPointsWC = new vector3f[JOINT_ID_KINECTV1::JOINT_COUNT];

	for (int i = 0; i < JOINT_ID_KINECTV1::JOINT_COUNT; i++)
	{
		this->skeletonPoints[i].x = skeleton.skeletonPoints[i].x;
		this->skeletonPoints[i].y = skeleton.skeletonPoints[i].y;
		this->skeletonPoints[i].z = skeleton.skeletonPoints[i].z;
		this->skeletonPoints[i].w = skeleton.skeletonPoints[i].w;

		this->skeletonPointsWC[i].x = skeleton.skeletonPointsWC[i].x;
		this->skeletonPointsWC[i].y = skeleton.skeletonPointsWC[i].y;
		this->skeletonPointsWC[i].z = skeleton.skeletonPointsWC[i].z;
	}
}

SkeletonBodyV1::~SkeletonBodyV1()
{
	delete this->skeletonPoints;
	this->skeletonPoints = NULL;

	delete this->skeletonPointsWC;
	this->skeletonPointsWC = NULL;
}

//--------------------------------------------------------------------------------

void SkeletonBodyV1::setMaxCordinates(float maxx, float maxy, float maxz)
{
	if (maxx > 0 && maxy > 0 && maxz > 0)
	{
		RANGE_MAX_X = maxx;
		RANGE_MAX_Y = maxy;
		RANGE_MAX_Z = maxz;
	}
}

vector <BONE>  SkeletonBodyV1::getSkeletonAtlasDescription() 
{
	vector <BONE> atlas;

	for (int i = 0; i < JOINT_ID_KINECTV1::ATLAS_SIZE; i++)
	{
		atlas.push_back(JOINT_ID_KINECTV1::ATLAS[i]);
	}
	return atlas;
}

vector <vector4f> SkeletonBodyV1::getSkeletonJoints(bool isNormed) 
{
	vector <vector4f> ret;

	for (int i = 0; i < JOINT_ID_KINECTV1::JOINT_COUNT; i++)
	{
		ret.push_back(getSkeletonJoints(i, isNormed));
	}

	return ret;
}

vector4f SkeletonBodyV1::getSkeletonJoints(int index, bool isNormed) 
{ 
	if (index >= 0 && index < JOINT_ID_KINECTV1::JOINT_COUNT)
	{
		if (!isNormed)
		{
			return skeletonPoints[index];
		}
		else
		{
			vector4f output;

			//---- Normalize coordinates to be in range [0,1]
			output.x = skeletonPoints[index].x / RANGE_MAX_X;
			output.y = skeletonPoints[index].y / RANGE_MAX_Y;
			output.z = skeletonPoints[index].z / RANGE_MAX_Z;
			output.w = skeletonPoints[index].w;

			return output;
		}
	}

	return vector4f(0, 0, 0, 0);
}

void SkeletonBodyV1::setSkeletonJoint(vector4f joint, int jointID) 
{
	if (jointID >= 0 && jointID < JOINT_ID_KINECTV1::JOINT_COUNT)
	{
		skeletonPoints[jointID] = joint;
	}
}

bool SkeletonBodyV1::getIsTracked()
{
	return isTracked;
}

void SkeletonBodyV1::setIsTracked(bool value)
{
	isTracked = value;
}

void SkeletonBodyV1::setHandStateLEFT(int state)
{
	if (state == HAND_STATE_OPEN) { handstateLEFT = HAND_STATE_OPEN; }
	if (state == HAND_STATE_CLOSE) { handstateLEFT = HAND_STATE_CLOSE; }
	if (state == HAND_STATE_UNKNOWN) { handstateLEFT = HAND_STATE_UNKNOWN; }
}

void SkeletonBodyV1::setHandStateRIGHT(int state)
{
	if (state == HAND_STATE_OPEN) { handstateRIGHT = HAND_STATE_OPEN; }
	if (state == HAND_STATE_CLOSE) { handstateRIGHT = HAND_STATE_CLOSE; }
	if (state == HAND_STATE_UNKNOWN) { handstateRIGHT = HAND_STATE_UNKNOWN; }
}

int SkeletonBodyV1::getHandStateLEFT()
{
	return handstateLEFT;
}

int SkeletonBodyV1::getHandStateRIGHT()
{
	return handstateRIGHT;
}

vector4f SkeletonBodyV1::getJointHandLEFT(bool isNormed)
{
	return getSkeletonJoints(JOINT_ID_KINECTV1::JOINT_LEFT_HAND, isNormed);
}

vector4f SkeletonBodyV1::getJointHandRIGHT(bool isNormed)
{
	return getSkeletonJoints(JOINT_ID_KINECTV1::JOINT_RIGHT_HAND, isNormed);
}

vector4f SkeletonBodyV1::getJointBodyCenter(bool isNormed)
{
	return getSkeletonJoints(JOINT_ID_KINECTV1::JOINT_HIP_CENTER, isNormed);
}

//---- Get the range of joint coordinates xmin, ymin, xmax, ymax
vector4f SkeletonBodyV1::getJointCoordinateRange(bool isNormed)
{
	vector4f output; 
	output.x = 30000; output.y = 30000; output.z = 0; output.w =0;

	for (int i = 0; i < JOINT_ID_KINECTV1::JOINT_COUNT; i++)
	{
		vector4f joint = getSkeletonJoints(i, true);

		if (joint.x < output.x) { output.x = joint.x;  }
		if (joint.y < output.y) { output.y = joint.y;  }
		if (joint.x > output.z) { output.z = joint.x;  }
		if (joint.y > output.w) { output.w = joint.y;  }
	}

	return output;
}

int SkeletonBodyV1::getJointCount()
{
	return JOINT_ID_KINECTV1::JOINT_COUNT;
}

void SkeletonBodyV1::setSkeletonJointWC(vector3f joint, int jointID)
{
	if (jointID >= 0 && jointID < JOINT_ID_KINECTV1::JOINT_COUNT)
	{
		skeletonPointsWC[jointID] = joint;
	}
}

vector3f SkeletonBodyV1::getSkeletonJointWC(int jointID)
{
	if (jointID >= 0 && jointID < JOINT_ID_KINECTV1::JOINT_COUNT)
	{
		return skeletonPointsWC[jointID];
	}

	return vector3f(0, 0, 0);
}