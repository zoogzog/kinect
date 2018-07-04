#include <vector>
#include <iostream>
#include <typeinfo>
#include <float.h>

#include "SkeletonBodyCollection.h"
#include "SkeletonTrackerFactory.h"

using namespace std;
SkeletonBodyCollection::SkeletonBodyCollection(int type, int skeletonCount)
{
	//---- For some reason if you switch from int to TrackerDeviceType everything breaks
	//---- So... integers then
	TrackerDeviceType device;

	if (type == TrackerDeviceType::DEVICE_KINECT_V1) { device = TrackerDeviceType::DEVICE_KINECT_V1; }
	if (type == TrackerDeviceType::DEVICE_KINECT_V2) { device = TrackerDeviceType::DEVICE_KINECT_V2; }
	
	collection.resize(skeletonCount);

	for (int i = 0; i < skeletonCount; i++)
	{
		SkeletonBody * body = SkeletonBodyFactory::getSkeletonBody(device);
		collection.at(i) = body;
	}
}


SkeletonBody * SkeletonBodyCollection::getSkeletonBody(int index)
{
	if (index >= 0 && index < collection.size())
	{
		return collection.at(index);
	}

	return NULL;
}

void SkeletonBodyCollection::setMaxCordinates(float maxx, float maxy, float maxz)
{
	for (int i = 0; i < collection.size(); i++)
	{
		collection.at(i)->setMaxCordinates(maxx, maxy, maxz);
	}
}


int SkeletonBodyCollection::getCollectionSize()
{
	return collection.size();
}


int SkeletonBodyCollection::getIndexClosestToPoint(int x, int y)
{
	double distanceMin = 4;
	double distanceID = -1;

	for (int i = 0; i < collection.size(); i++)
	{
		if (collection.at(i)->getIsTracked())
		{
			vector4f bodyCenter = collection.at(i)->getJointBodyCenter(true);
			double distance = (x - bodyCenter.x) * (x - bodyCenter.x) + (y - bodyCenter.y) * (y - bodyCenter.y);

			if (distance < distanceMin)
			{
				distanceMin = distance;
				distanceID = i;
			}
		}
	}

	if (distanceID == -1) { return -1; }

	return distanceID;
}

void SkeletonBodyCollection::clear()
{
	for (int i = 0; i < collection.size(); i++)
	{
		delete collection.at(i);
		collection.at(i) = NULL;
	}

	collection.clear();
}