#pragma once

#include "DataDriver.h"
#include "VisualizerDriver.h"

#include "ofMain.h"


class ApplicationMain : public ofBaseApp
{
private: 

	VisualizerDriver vdriver;
	
public:
	ApplicationMain();

	void setup();

	//---- Logical loop 
	void update();

	//--- Drawing loop
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void exit();

	
};
