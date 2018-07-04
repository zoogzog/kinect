#ifndef IOFILEMANAGER_H
#define IOFILEMANAGER_H

#include <iostream>
#include <string>

#include <Windows.h>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;
//-----------------------------------------------------------------
//----- This class is a wrapper that provides various saving methods
//----- Author: Andrey G.
//----- Speed comparison for different file extensions (default settings, 640x480)
//----- BMP -> Windows ~1ms
//----- BMP -> OpenCV ~2ms
//----- PNG -> OpenCV ~30ms
//----- JPG -> OpenCV ~20ms
//-----------------------------------------------------------------

class IOFileManager
{
private:

	//---- Width and height of compressed data, used for saving binary files in LZ4 format
	static const int COMPRESSED_WIDTH = 640;
	static const int COMPRESSED_HEIGHT = 480;


	static bool saveImageBMP_WINDOWS(byte * data, int width, int height, string path);

	static bool saveImageBMP_OPENCV(byte * data, int width, int height, string path);
	static bool saveImagePNG_OPENCV(byte * data, int width, int height, string path);
	static bool saveImageJPG_OPENCV(byte * data, int width, int height, string path);

	static bool saveImageBIN_WINDOWS(byte * data, int width, int height, string path, int btpx);
	static bool saveImageBIN_WINDOWS(unsigned short * data, int width, int height, string path);
	static bool saveImageBIN_OPENCV(byte * data, int width, int height, string path, int btpx);
	static bool saveImageBIN_OPENCV(unsigned short * data, int width, int height, string path);
	static bool saveImageBIN_LZ4(byte * data, int width, int height, string path, int btpx);
	static bool saveImageBIN_LZ4(unsigned short * data, int width, int height, string path);

	//-------------------------------------
	//---- Helper functions to render the skeleton and save as image
	static void renderSkeletonBody(Mat * mat, float * data, int dataSize, int * atlas, int atlasSize);
	static void renderSkeletonBone(Mat * mat, float * data, int joint1, int joint2);

public:
	//-------------------------------------
	static const int METHOD_WINDOWS = 0;
	static const int METHOD_OPENCV = 1;
	static const int METHOD_LZ4 = 2;

	//-------------------------------------

	static bool saveImageBMP(byte * data, int width, int height, string path, int flag = METHOD_WINDOWS);
	static bool saveImagePNG(byte * data, int width, int height, string path, int flag = METHOD_OPENCV);
	static bool saveImageJPG(byte * data, int width, int height, string path, int flag = METHOD_OPENCV);

	static bool saveImageBIN(byte * data, int width, int height, string path, int btpx, int flag);
	static bool saveImageBIN(unsigned short * data, int width, int height, string path, int flag);

	//-------------------------------------
	//----These functions load binary data from file. Use them to check if everything works correctly.
	//----These functions don:t store data anywhere at the moment - only for DEBUG

	static bool loadBinaryRange_WINDOWS(string path);
	static bool loadBinaryRange_OPENCV(string path);
	static bool loadBinaryRange_LZ4(string path);

	//-------------------------------------
	//---- This function renders skeleton and saves as image (only 1 skeleton is saved!)
	//---- Primarily use this for debug purpose 

	static bool saveSkeletonImage(float * data, int dataSize, int * atlas, int atlasSize, int width, int height, string path);
};

#endif 
