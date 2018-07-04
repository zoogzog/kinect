#ifndef SKELETONB_H
#define SKELETONB_H

#include <windows.h>


#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

typedef struct BONE { int j1; int j2; };

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

	static const int ATLAS_SIZE = 20;

	static const BONE ATLAS[]
	{
		//---- Head & torso
		{JOINT_HEAD, JOINT_SHOULDER_CENTER},
		{JOINT_SHOULDER_CENTER, JOINT_LEFT_SHOULDER},
		{JOINT_SHOULDER_CENTER, JOINT_RIGHT_SHOULDER},
		{JOINT_SHOULDER_CENTER, JOINT_SPINE},
		{JOINT_SPINE, JOINT_HIP_CENTER},
		{JOINT_HIP_CENTER, JOINT_LEFT_HIP},
		{JOINT_HIP_CENTER, JOINT_RIGHT_HIP},

		//----- Left arm
		{JOINT_LEFT_SHOULDER, JOINT_LEFT_ELBOW},
		{JOINT_LEFT_ELBOW, JOINT_LEFT_WRIST},
		{JOINT_LEFT_WRIST, JOINT_LEFT_HAND},

		//----- Right arm
		{JOINT_RIGHT_SHOULDER, JOINT_RIGHT_ELBOW },
		{JOINT_RIGHT_ELBOW, JOINT_RIGHT_WRIST },
		{JOINT_RIGHT_WRIST, JOINT_RIGHT_HAND },

		//----- Left leg
		{JOINT_LEFT_HIP, JOINT_LEFT_KNEE},
		{JOINT_LEFT_KNEE, JOINT_LEFT_ANKLE},
		{JOINT_LEFT_ANKLE, JOINT_LEFT_FOOT},

		//----- Right leg
		{ JOINT_RIGHT_HIP, JOINT_RIGHT_KNEE},
		{ JOINT_RIGHT_KNEE, JOINT_RIGHT_ANKLE},
		{ JOINT_RIGHT_ANKLE, JOINT_RIGHT_FOOT}
	};

	static const int JOINT_COUNT = 20;

	static const int JOINT_COLLECTION[]
	{
		JOINT_HIP_CENTER, 
		JOINT_SPINE,
		JOINT_SHOULDER_CENTER,
		JOINT_HEAD,

		JOINT_LEFT_SHOULDER,
		JOINT_LEFT_ELBOW,
		JOINT_LEFT_WRIST,
		JOINT_LEFT_HAND,

		JOINT_RIGHT_SHOULDER,
		JOINT_RIGHT_ELBOW,
		JOINT_RIGHT_WRIST,
		JOINT_RIGHT_HAND,

		JOINT_LEFT_HIP,
		JOINT_LEFT_KNEE,
		JOINT_LEFT_ANKLE,
		JOINT_LEFT_FOOT,

		JOINT_RIGHT_HIP,
		JOINT_RIGHT_KNEE,
		JOINT_RIGHT_ANKLE,
		JOINT_RIGHT_FOOT
	};
}


//-----------------------------------------------------------------
//----- This class is just a container to store skeleton data.
//----- Author: Andrey G.
//-----------------------------------------------------------------

class SkeletonBody
{
public:
	//----------------------------------------

	//---- Total count of joints in the skeleton
	static const int MAX_SKELETON_JOINT = NUI_SKELETON_POSITION_COUNT;

	//---- This represents number of elements int skeleton atlas 
	static const int SKELETON_ATLAS_SIZE = 38;

	//------------- SKELETON ATLAS v2 --> v1 -----------
	//-- 00: SPINE BASE ----- (!) N/A or HIP_CENTER (00)
	//-- 01: SPINE MID ------ (!) N/A or SPINE (01)
	//-- 02: NECK ----------- (!) N/A or SHOULDER_CENTER (02)
	//-- 03: HEAD ----------- HEAD (03)
	//-- 04: LEFT-SHOULDER -- SHOULDER_LEFT (04)
	//-- 05: LEFT-ELBOW ----- ELBOW_LEFT (05)
	//-- 06: LEFT-WRIST ----- WRIST_LEFT (06)
	//-- 07: LEFT-HAND ------ HAND_LEFT (07)
	//-- 08: RIGHT-SHOULDER - SHOULDER_RIGHT (08)
	//-- 09: RIGHT-ELBOW ---- ELBOW_RIGHT (09)
	//-- 10: RIGHT-WRIST ---- WRIST_RIGHT (10)
	//-- 11: RIGHT-HAND ----- HAND_RIGHT (11)
	//-- 12: LEFT-HIP ------- HIP_LEFT (12)
	//-- 13: LEFT-KNEE ------ KNEE_LEFT (13)
	//-- 14: LEFT-ANKLE ----- ANKLE_LEFT (14)
	//-- 15: LEFT-FOOT ------ FOOT_LEFT (15)
	//-- 16: RIGHT-HIP ------ HIP_RIGHT (16)
	//-- 17: RIGHT-KNEE ----- KNEE_RIGHT (17)
	//-- 18: RIGHT-ANKLE ---- ANKLE_RIGHT (18)
	//-- 19: RIGHT-FOOT ----- FOOT_RIGHT (19)
	//-- 20: SPINE-SHOULDER - (!) N/A or SHOULDER_CENTER (02)
	//-- 21: LEFT-TIP-HAND -- (!) N/A
	//-- 22: LEFT-THUMB ----- (!) N/A
	//-- 23: RIGHT-TIP-HAND - (!) N/A
	//-- 24: RIGHT-THUMB ---- (!) N/A
	//---------------------------------------------------

	//---- These are location of the skeleton joints
	Vector4 skeletonPosition[NUI_SKELETON_POSITION_COUNT];

	//---- Flag - true if this skeleton tracked, falst otherwise
	bool isTracked;

	//----------------------------------------

	SkeletonBody();
	SkeletonBody(const SkeletonBody &skeleton);

	//---- Get skeleton joint coordinates as an array of values x,y
	//---- The array's memory should be defined before using this function
	//---- Size of the array is 2 * MAX_SKELETON_JOINT
	//---- Even elements of the array are X coordinates
	//---- Odd elements of the array are Y coordinates
	void getSkeletonAtlasCoordinates(float * coordinates);

	void getSkeletonAtlasDescription(int * indexList);

	
};

#endif 