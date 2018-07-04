#include "VisualizerKinectData.h"

#include "Debugger.h"

#include "ofMain.h"

#include "ofxKinectForWindows2.h"

#include "SkeletonBody.h"

//--------------------------------------------------------------------------------

void  VisualizerKinectData::drawSkeleton()
{
	int resolutionWidth = sklw;
	int resolutionHeight = sklh;

	ofPushStyle();
	for (int i = 0; i < collection->getCollectionSize(); i++)
	{
		//---- Draw a body if it is tracked
		if (collection->getSkeletonBody(i)->getIsTracked())
		{
			SkeletonBody * body = collection->getSkeletonBody(i);
			vector <vector4f> jointList = body->getSkeletonJoints(true);

			//----------------------------------------------------------
			//---- Draw skeleton bones
			ofSetColor(ofColor::white);

			vector <BONE> atlasx = body->getSkeletonAtlasDescription();

			for (int j = 0; j < atlasx.size(); j++)
			{
				int jointID1 = atlasx.at(j).j1;
				int jointID2 = atlasx.at(j).j2;

				//---- Get joints by their IDs
				vector4f j1xy = body->getSkeletonJoints(jointID1, true);
				vector4f j2xy = body->getSkeletonJoints(jointID2, true);

				//---- Convert coordinates from internal (KINECT) to displayed
				ofVec2f j1 = ofVec2f(j1xy.x * resolutionWidth, j1xy.y * resolutionHeight);
				ofVec2f j2 = ofVec2f(j2xy.x * resolutionWidth, j2xy.y * resolutionHeight);

				ofLine(sklx + j1.x, skly + j1.y, sklx + j2.x, skly + j2.y);
			}

			//----------------------------------------------------------
			//---- Draw skeleton joints
			ofSetColor(ofColor::red);
			for (int k = 0; k < jointList.size(); k++)
			{
				//---- Convert coordinates from internal (KINECT) to displayed
				ofVec2f j1 = ofVec2f(jointList.at(k).x * resolutionWidth, jointList.at(k).y * resolutionHeight);

				ofDrawCircle(sklx + j1.x, skly + j1.y, 5);
			}

			//----------------------------------------------------------
			//---- Draw hand state as hand color
			vector4f jointHandLEFT = body->getJointHandLEFT(true);
			vector4f jointHandRIGHT = body->getJointHandRIGHT(true);

			//---- LEFT HAND
			if (body->getHandStateLEFT() == HAND_STATE::HAND_STATE_OPEN)
			{
				ofSetColor(ofColor::green);
				ofDrawCircle(sklx + jointHandLEFT.x * resolutionWidth, skly + jointHandLEFT.y * resolutionHeight, 5);
			}
			else if (body->getHandStateLEFT() == HAND_STATE::HAND_STATE_CLOSE)
			{
				ofSetColor(ofColor::blue);
				ofDrawCircle(sklx + jointHandLEFT.x * resolutionWidth, skly + jointHandLEFT.y * resolutionHeight, 5);
			}

			//---- RIGHT HAND
			if (body->getHandStateRIGHT() == HAND_STATE::HAND_STATE_OPEN)
			{
				ofSetColor(ofColor::green);
				ofDrawCircle(sklx + jointHandRIGHT.x * resolutionWidth, skly + jointHandRIGHT.y * resolutionHeight, 5);
			}
			else if (body->getHandStateRIGHT() == HAND_STATE::HAND_STATE_CLOSE)
			{
				ofSetColor(ofColor::blue);
				ofDrawCircle(sklx + jointHandRIGHT.x * resolutionWidth, skly + jointHandRIGHT.y * resolutionHeight, 5);
			}

		}
	}
	ofPopStyle();
}

void VisualizerKinectData::drawImageRGB()
{
	if (dataRGB == NULL) { return;  }
	if (!dataRGB->isAllocated()) { return; }
	ofImage imgrgb(* dataRGB);
	imgrgb.resize(rgbw, rgbh);
	imgrgb.draw(rgbx, rgby);
}

void VisualizerKinectData::drawImageDPT()
{
	if (dataDPT == NULL) { return; }
	if (!dataDPT->isAllocated()) { return; }
	ofImage imgdpt(*dataDPT);
	imgdpt.resize(dptw, dpth);
	imgdpt.draw(dptx, dpty);
}


void VisualizerKinectData::drawImageBODY()
{
	if (dataBODY == NULL) { return;  }
	if (!dataBODY->isAllocated()) { return; }
	ofImage imgbd(*dataBODY);
	imgbd.resize(bdw, bdh);
	imgbd.draw(bdx, bdy);
}
//--------------------------------------------------------------------------------

void VisualizerKinectData::draw()
{
	drawImageRGB();
	drawImageDPT();
	drawImageBODY();
	drawSkeleton();
}

ofVec2f VisualizerKinectData::convertCoordinate(ofVec2f in)
{
	ofVec2f out(0, 0);

//	out.x = (double)in.x / 512 * resolutionWidth;
//	out.y = (double)in.y / 424 * resolutionHeight;

	return out;
}

void VisualizerKinectData::setup(
	ofPixels * imgrgb, int xrgb, int yrgb, int wrgb, int hrgb,
	ofPixels * imgdpt, int xdpt, int ydpt, int wdpt, int hdpt,
	ofPixels * imgbd, int xbd, int ybd, int wbd, int hbd,
	SkeletonBodyCollection * c, int xsk, int ysk, int wsk, int hsk)
{
	dataRGB = imgrgb; rgbx = xrgb; rgby = yrgb; rgbw = wrgb; rgbh = hrgb;
	dataDPT = imgdpt; dptx = xdpt; dpty = ydpt; dptw = wdpt; dpth = hdpt;
	dataBODY = imgbd; bdx = xbd; bdy = ybd; bdw = wbd; bdh = hbd;

	collection = c; sklx = xsk; skly = ysk; sklw = wsk; sklh = hsk;
}


void VisualizerKinectData::resize(int width, int height)
{

}

void VisualizerKinectData::update()
{

}