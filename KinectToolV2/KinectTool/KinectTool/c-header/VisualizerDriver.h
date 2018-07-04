#pragma once

#include "ofMain.h"

#include "VisualizerKinectData.h"

class VisualizerDriver
{
private:
	int resolutionWidth;
	int resolutionHeight;

	VisualizerKinectData driverKinect;

public:
	ofImage mainImage;

	void setup(int width, int height);

	void update();
	void draw();

	void resize(int width, int height);
};