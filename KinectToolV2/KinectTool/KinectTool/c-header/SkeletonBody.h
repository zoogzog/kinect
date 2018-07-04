#pragma once

#include <vector>
#include "DataVectors.h"

//--------------------------------------------------------------------------------
//----- Abstract class (and other helpers) to describe a general skeleton body.
//----- Should contain some container for storing skeleton joints.
//----- Author: Andrey G.
//--------------------------------------------------------------------------------

typedef struct BONE { int j1; int j2; };

enum HAND_STATE {HAND_STATE_OPEN, HAND_STATE_CLOSE, HAND_STATE_UNKNOWN};

using namespace std;

//--------------------------------------------------------------------------------

class SkeletonBody
{
public:
	//---- Set vertex coordinate limits (specific to each tracking device)
	virtual void setMaxCordinates(float maxx, float maxy, float maxz) = 0;

	//---- Get atlas that describes a skeleton.
	//---- The atlas is device specific
	virtual vector <BONE>  getSkeletonAtlasDescription() = 0;

	//---- Get all skeleton joints
	//---- If isNormed the coordinates returned as [0,1]
	virtual vector <vector4f> getSkeletonJoints(bool isNormed = false) = 0;

	//---- Get a specific skeleton joint defined by index
	//---- If isNormed the coordinates returned as [0,1]
	virtual vector4f getSkeletonJoints(int index, bool isNormed = false) = 0;

	//---- Set coordinates of a specific joint
	virtual void setSkeletonJoint(vector4f joint, int jointID) = 0;

	//---- Manipulation with the skeleton tracking state
	virtual bool getIsTracked() = 0;
	virtual void setIsTracked(bool value) = 0;

	//---- Set the hand state, if device does not obtain state set HAND_STATE_UNKNOWN
	virtual void setHandStateLEFT(int state) = 0;
	virtual void setHandStateRIGHT(int state) = 0;

	virtual int getHandStateLEFT() = 0;
	virtual int getHandStateRIGHT() = 0;

	virtual vector4f getJointHandLEFT(bool isNormed = false) = 0;
	virtual vector4f getJointHandRIGHT(bool isNormed = false) = 0;

	virtual vector4f getJointBodyCenter(bool isNormed = false) = 0;

	virtual vector4f getJointCoordinateRange(bool isNormed = false) = 0;

	virtual int getJointCount() = 0;

	//------ Add joint X,Y,Z in world coordinates
	virtual void setSkeletonJointWC(vector3f joint, int jointID) = 0;
	virtual vector3f getSkeletonJointWC(int jointID) = 0;



};