#pragma once

#include "ofMain.h"

#include "SkeletonBodyCollection.h"

class VisualizerKinectData
{
private:
	//--------- DATA ---------
	SkeletonBodyCollection * collection;
	int sklx, skly, sklw, sklh;

	ofPixels * dataRGB;
	int rgbx, rgby, rgbw, rgbh;

	ofPixels * dataDPT;
	int dptx, dpty, dptw, dpth;

	ofPixels * dataBODY;
	int bdx, bdy, bdw, bdh;

	//-------------------------

	ofVec2f convertCoordinate(ofVec2f in);

	void drawSkeleton();
	void drawImageRGB();
	void drawImageDPT();
	void drawImageBODY();

public:

	//---------------------------------------------------------------------------------
	//---- INPUT: imgrgb - pointer to color data captured with the device
	//---- INPUT: xrgb, yrgb, wrgb, hrgb - position (x,y) and resolution (w,h) of the color image
	//---- INPUT: imgdpt - pointer to depth data (grayscale) captured with the device
	//---- INPUT: xdpt, ydpt, wdpt, hdpt - poistion (x,y) and resolition (w,h) of the depth image
	//---- INPUT: imgbd - pointer to body mask
	//---- INPUT: xbd, ybd, wbd, hbd - position (x,y) and resolution (w,h) of the body mask image
	//---- INPUT: c - pointer to skeleton data
	//---- INPUT: xsk, ysk, wsk, hsk - position (x,y) and resolution (w,h) of an image with skeleton

	void setup(ofPixels * imgrgb, int xrgb, int yrgb, int wrgb, int hrgb,
		       ofPixels * imgdpt, int xdpt, int ydpt, int wdpt, int hdpth,
			   ofPixels * imgbd, int xbd, int ybd, int wbd, int hbd,
		       SkeletonBodyCollection * c, int xsk, int ysk, int wsk, int hsk);

	void update();
	void draw();

	void resize(int width, int height);
};