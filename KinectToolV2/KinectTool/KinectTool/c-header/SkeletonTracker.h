#pragma once

#include "ofMain.h"
#include "SkeletonBodyCollection.h"

//--------------------------------------------------------------------------------
//---- Abstract class that describes a skeleton tracker (kinectV1, kinectV2, other)
//---- Author: Andrey G.
//--------------------------------------------------------------------------------

class SkeletonTracker
{
	public:
		//----- Return queried value of the device specifications
		virtual int getSettingsSkeletonCount() = 0;

		virtual bool init() = 0;

		//---- This method is called in loop, capture all frames here
		virtual bool update() = 0;

		//----- These functions should return resolution of the streams
		virtual int getWidthRGB() = 0;
		virtual int getWidthDPT() = 0;
		virtual int getHeightRGB() = 0;
		virtual int getHeightDPT() = 0;

		//----- These functions should store data into containers
		//----- Return true if data has been stored, false otherwise
		virtual bool getframeRGB(ofPixels & data) = 0;
		virtual bool getframeDPT(ofShortPixels & data) = 0;
		virtual bool getframeSKL(SkeletonBodyCollection & skeletonCollection) = 0;
		virtual bool getframeBODY(ofPixels & data) = 0;

		//----- Generate a colored body image
		//----- OUTPUT dataChromakey - chromakey image 
		//----- INPUT dataRGB - color data [width * height * 4]
		//----- INPUT dataDPT - depth data [width * height]
		//----- INPUT dataBODY - body mask [width * height]
		virtual bool generateImageChromakey(ofPixels  & dataChromakey, ofPixels & dataRGB, ofShortPixels & dataDPT, ofPixels & dataBODY) = 0;

		virtual bool close() = 0;


};
