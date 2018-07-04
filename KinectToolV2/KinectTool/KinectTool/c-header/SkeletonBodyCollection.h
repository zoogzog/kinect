#pragma once

#include <vector>

#include "SkeletonBody.h"

using namespace std; 

//--------------------------------------------------------------------------------
//----- List of skeleton bodies
//----- Author: Andrey G.
//--------------------------------------------------------------------------------

class SkeletonBodyCollection
{
private:

	vector <SkeletonBody *> collection;

public:

	SkeletonBodyCollection() {} 
	SkeletonBodyCollection(int type, int skeletonCount);

	void setMaxCordinates(float maxx, float maxy, float maxz);

	SkeletonBody * getSkeletonBody(int index);

	int getCollectionSize();

	//---- Returns ID of a SkeletonBody closest to point x, y 
	//---- X,Y should be in depth coordinates
	int getIndexClosestToPoint(int x, int y);


	//---- Free resources, delete the collection
	void clear();
};