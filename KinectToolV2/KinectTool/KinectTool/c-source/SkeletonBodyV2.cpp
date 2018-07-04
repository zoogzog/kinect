#include <vector>
#include <iostream>
#include "SkeletonBodyV2.h"

using namespace std;

//--------------------------------------------------------------------------------
//---- CONSTRUCTORS
//--------------------------------------------------------------------------------

SkeletonBodyV2::SkeletonBodyV2()
{

	skeletonPoints = new vector4f[JOINT_ID_KINECTV2::JOINT_COUNT];
	skeletonPointsWC = new vector3f[JOINT_ID_KINECTV2::JOINT_COUNT];

	isTracked = false;
}

SkeletonBodyV2::SkeletonBodyV2(const SkeletonBodyV2 &skeleton)
{
	this->isTracked = skeleton.isTracked;
	
	skeletonPoints = new vector4f[JOINT_ID_KINECTV2::JOINT_COUNT];
	skeletonPointsWC = new vector3f[JOINT_ID_KINECTV2::JOINT_COUNT];
	
	for (int i = 0; i < JOINT_ID_KINECTV2::JOINT_COUNT; i++)
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

SkeletonBodyV2::~SkeletonBodyV2()
{
	delete this->skeletonPoints;
	this->skeletonPoints = NULL;

	delete this->skeletonPointsWC;
	this->skeletonPointsWC = NULL;
}

//--------------------------------------------------------------------------------

void SkeletonBodyV2::setSkeletonJoint(vector4f joint, int jointID)
{
	
	if (jointID >= 0 && jointID < JOINT_ID_KINECTV2::JOINT_COUNT)
	{
		skeletonPoints[jointID] = joint;
	}
}

vector <BONE> SkeletonBodyV2::getSkeletonAtlasDescription()
{
	vector <BONE> atlas;

	for (int i = 0; i < JOINT_ID_KINECTV2::ATLAS_SIZE; i++)
	{
		atlas.push_back(JOINT_ID_KINECTV2::ATLAS[i]);
	}
	return atlas;
}

//--------------------------------------------------------------------------------
//---- GET & SET
//--------------------------------------------------------------------------------


void SkeletonBodyV2::setMaxCordinates(float maxx, float maxy, float maxz)
{
	if (maxx > 0 && maxy > 0 && maxz > 0)
	{
		RANGE_MAX_X = maxx;
		RANGE_MAX_Y = maxy;
		RANGE_MAX_Z = maxz;
	}
}

bool SkeletonBodyV2::getIsTracked()
{
	return isTracked;
}


void SkeletonBodyV2::setIsTracked(bool value)
{
	isTracked = value;
}


vector <vector4f> SkeletonBodyV2::getSkeletonJoints(bool isNormed)
{
	vector <vector4f> ret;

	for (int i = 0; i < JOINT_ID_KINECTV2::JOINT_COUNT; i++)
	{
		ret.push_back(getSkeletonJoints(i, isNormed));
	}

	return ret;
}
vector4f SkeletonBodyV2::getSkeletonJoints(int index, bool isNormed)
{
	if (index >= 0 && index < JOINT_ID_KINECTV2::JOINT_COUNT)
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

void SkeletonBodyV2::setHandStateLEFT(int state)
{
	if (state == HAND_STATE_OPEN) { handstateLEFT = HAND_STATE_OPEN;  }
	if (state == HAND_STATE_CLOSE) { handstateLEFT = HAND_STATE_CLOSE; }
	if (state == HAND_STATE_UNKNOWN) { handstateLEFT = HAND_STATE_UNKNOWN; }
}

void SkeletonBodyV2::setHandStateRIGHT(int state)
{
	if (state == HAND_STATE_OPEN) { handstateRIGHT = HAND_STATE_OPEN; }
	if (state == HAND_STATE_CLOSE) { handstateRIGHT = HAND_STATE_CLOSE; }
	if (state == HAND_STATE_UNKNOWN) { handstateRIGHT = HAND_STATE_UNKNOWN; }
}

int SkeletonBodyV2::getHandStateLEFT()
{
	return handstateLEFT;
}

int SkeletonBodyV2::getHandStateRIGHT()
{
	return handstateRIGHT;
}

vector4f SkeletonBodyV2::getJointHandLEFT(bool isNormed)
{
	return getSkeletonJoints(JOINT_ID_KINECTV2::JOINT_LEFT_HAND, isNormed);
}

vector4f SkeletonBodyV2::getJointHandRIGHT(bool isNormed)
{
	return getSkeletonJoints(JOINT_ID_KINECTV2::JOINT_RIGHT_HAND, isNormed);
}

vector4f SkeletonBodyV2::getJointBodyCenter(bool isNormed)
{
	return getSkeletonJoints(JOINT_ID_KINECTV2::JOINT_SPINE_BASE, isNormed);
}

//---- Get the range of joint coordinates xmin, ymin, xmax, ymax
vector4f SkeletonBodyV2::getJointCoordinateRange(bool isNormed)
{
	vector4f output;
	output.x = 30000; output.y = 30000; output.z = 0; output.w = 0;

	for (int i = 0; i < JOINT_ID_KINECTV2::JOINT_COUNT; i++)
	{
		vector4f joint = getSkeletonJoints(i, true);



		if (joint.x >= 0 && joint.x <= 1 && joint.y >= 0 && joint.y <= 1 && joint.w == 1)
		{

			if (joint.x < output.x) { output.x = joint.x; }
			if (joint.y < output.y) { output.y = joint.y; }
			if (joint.x > output.z) { output.z = joint.x; }
			if (joint.y > output.w) { output.w = joint.y; }
		}
	}

	if (!isNormed)
	{
		output.x *= RANGE_MAX_X;
		output.y *= RANGE_MAX_Y;
		output.z *= RANGE_MAX_X;
		output.w *= RANGE_MAX_Y;
	}

	return output;
}

int SkeletonBodyV2::getJointCount()
{
	return JOINT_ID_KINECTV2::JOINT_COUNT;
}

void SkeletonBodyV2::setSkeletonJointWC(vector3f joint, int jointID)
{
	if (jointID >= 0 && jointID < JOINT_ID_KINECTV2::JOINT_COUNT)
	{
		skeletonPointsWC[jointID] = joint;
	}
}

vector3f SkeletonBodyV2::getSkeletonJointWC(int jointID)
{
	if (jointID >= 0 && jointID < JOINT_ID_KINECTV2::JOINT_COUNT)
	{
		return skeletonPointsWC[jointID];
	}

	return vector3f(0, 0, 0);
}