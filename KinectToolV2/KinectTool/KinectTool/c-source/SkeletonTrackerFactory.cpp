#include "ofMain.h"

#include "SkeletonTrackerFactory.h"
#include "SkeletonTracker.h"
#include "TrackerKinectV2.h"
#include "TrackerKinectV1.h"
#include "SkeletonBodyV1.h"

SkeletonTracker * SkeletonTrackerFactory::getDevice(TrackerDeviceType type)
{
	switch (type)
	{
	case DEVICE_KINECT_V2:
		return new TrackerKinectV2;
		break;
	case DEVICE_KINECT_V1:
		return new TrackerKinectV1;
		break;
	default:
		break;
	}

	return 0;
}

SkeletonBody * SkeletonBodyFactory::getSkeletonBody(TrackerDeviceType type)
{
	switch (type)
	{
	case DEVICE_KINECT_V2:
		return new SkeletonBodyV2;
		break;
	case DEVICE_KINECT_V1:
		return new SkeletonBodyV1;
		break;
	default:
		break;
	}

	return 0;
}