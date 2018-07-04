#pragma once

#include "SkeletonBody.h"
#include "SkeletonTrackerFactory.h"
#include "SkeletonTracker.h"
#include "TrackerKinectV2.h"

//--------------------------------------------------------------------------------
//---- Factories for creating specific type of tracker and skeleton body
//---- Author: Andrey G.
//--------------------------------------------------------------------------------


enum TrackerDeviceType
{
	DEVICE_KINECT_V2,
	DEVICE_KINECT_V1,
	DEVICE_REALSENSE
};

class SkeletonTrackerFactory
{
public:
	static SkeletonTracker * getDevice(TrackerDeviceType type);
};

class SkeletonBodyFactory
{
public:
	static SkeletonBody * getSkeletonBody(TrackerDeviceType type);
};