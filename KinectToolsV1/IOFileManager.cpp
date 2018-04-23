#include <iostream>
#include <string>

#include <Windows.h>
#include <opencv2\opencv.hpp>

#include "lz4.h"

#include "IOFileManager.h"


using namespace std;
using namespace cv;

//-----------------------------------------------------------------

//---- This function is copied from the Kinect 1.8 Toolkit sample 'ColorBasics'
//---- See 'ColorBasics.cpp' method 'SaveBitmapToFile' (line 417)
bool IOFileManager::saveImageBMP_WINDOWS(byte * data, int width, int height, string path)
{
	//---- This is a conversion from amazing standard 'string' into shitty windows LPCWSTR
	std::wstring stemp = std::wstring(path.begin(), path.end());
	LPCWSTR pathFile = stemp.c_str();

	const int BITS_PER_PIXEL = 32;

	//------------ CColorBasics::SaveBitmapToFile ------------

	DWORD dwByteCount = width* height * (BITS_PER_PIXEL / 8);

	BITMAPINFOHEADER bmpInfoHeader = { 0 };

	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);  // Size of the header
	bmpInfoHeader.biBitCount = BITS_PER_PIXEL;        // Bit count
	bmpInfoHeader.biCompression = BI_RGB;             // Standard RGB, no compression
	bmpInfoHeader.biWidth = width;                    // Width in pixels
	bmpInfoHeader.biHeight = -height;                 // Height in pixels, negative indicates it's stored right-side-up
	bmpInfoHeader.biPlanes = 1;                       // Default
	bmpInfoHeader.biSizeImage = dwByteCount;          // Image size in bytes

	BITMAPFILEHEADER bfh = { 0 };

	bfh.bfType = 0x4D42;                                           // 'M''B', indicates bitmap
	bfh.bfOffBits = bmpInfoHeader.biSize + sizeof(BITMAPFILEHEADER);  // Offset to the start of pixel data
	bfh.bfSize = bfh.bfOffBits + bmpInfoHeader.biSizeImage;        // Size of image + headers


	HANDLE hFile = CreateFileW(pathFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	//---- EXIT if can't open file
	if (NULL == hFile)	{ return false; }

	DWORD dwBytesWritten = 0;

	//---- WRITE bitmap file header
	if (!WriteFile(hFile, &bfh, sizeof(bfh), &dwBytesWritten, NULL))
	{ CloseHandle(hFile);	return false; }

	//---- WRITE bitmap info header
	if (!WriteFile(hFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &dwBytesWritten, NULL)) 
	{ CloseHandle(hFile);	return false; }

	//---- WRITE bitmap rgb data
	if (!WriteFile(hFile, data, bmpInfoHeader.biSizeImage, &dwBytesWritten, NULL))
	{ CloseHandle(hFile);	return false; }

	//---- Close handler
	CloseHandle(hFile);
	//--------------------------------------------------------

	return true;
}


bool IOFileManager::saveImageBMP_OPENCV(byte * data, int width, int height, string path)
{
	//---- This try/catch is not really needed, but just in case.
	try
	{
		//---- Copy data to matrix
		Mat imgMat = Mat(height, width, CV_8UC4, data).clone();

		cv::imwrite(path, imgMat);

		return true;
	}
	catch (Exception e)
	{
		return false;
	}
}

bool IOFileManager::saveImagePNG_OPENCV(byte * data, int width, int height, string path)
{
	//---- This try/catch is not really needed, but just in case.
	try
	{
		//---- Copy data to matrix
		//---- For some reason kinect returns data with alpha channel = 0
		//---- We need to set the alpha channel to 255, to save correctly as PNG
		Mat imgMat = Mat(height, width, CV_8UC4, data).clone();
		
		//---- Split the original matrix into channel
		Mat bgra[4];
		cv::split(imgMat, bgra);

		//---- Set the value of the alpha channel, then merge with the original matrix
		bgra[3] = 255;
		cv::merge(bgra, 4, imgMat);

		//---- Save the matrix
		imwrite(path, imgMat);

		return true;
	}
	catch (Exception e)
	{
		return false;
	}
}

bool IOFileManager::saveImageJPG_OPENCV(byte * data, int width, int height, string path)
{
	//---- This try/catch is not really needed, but just in case.
	try
	{
		//---- Copy data to matrix
		Mat imgMat = Mat(height, width, CV_8UC4, data).clone();

		cv::imwrite(path, imgMat);

		return true;
	}
	catch (Exception e)
	{
		return false;
	}
}

bool IOFileManager::saveImageBIN_WINDOWS(byte * data, int width, int height, string path, int btpx) 
{
	ofstream os;
	os.open(path, ios::out | ios::binary);

	//---- Exit if can't open file
	if (!os.is_open()) { return false; }

	os.write((const char*)(&height), sizeof(int));
	os.write((const char*)(&width), sizeof(int));
	os.write((const char*)(&btpx), sizeof(int));

	os.write(reinterpret_cast<const char*>(data), (width * height * btpx));

	os.close();

	return true;
}

bool IOFileManager::saveImageBIN_WINDOWS(unsigned short * data, int width, int height, string path) 
{
	ofstream os;
	os.open(path, ios::out | ios::binary);
	//---- Exit if can't open file
	if (!os.is_open()) { return false; }

	int type = CV_16UC1;

	os.write((const char*)(&height), sizeof(int));
	os.write((const char*)(&width), sizeof(int));
	os.write((const char*)(&type), sizeof(int));

	os.write((char *)& data, sizeof(unsigned short) * width * height);

	os.close();

	return true;
}

bool IOFileManager::saveImageBIN_OPENCV(byte * data, int width, int height, string path, int btpx) 
{
	ofstream os;
	os.open(path, ios::out | ios::binary);

	//---- Exit if can't open file
	if (!os.is_open()) { return false; }

	int matType = 0;

	switch (btpx)
	{
	case 1: matType = CV_8UC1; break;
	case 3: matType = CV_8UC3; break;
	case 4: matType = CV_8UC4; break;
	default: return false; break;
	}

	//---- Convert to mat
	Mat imgMat = Mat(height, width, matType, data).clone();

	int type = imgMat.type();
	os.write((const char*)(&imgMat.rows), sizeof(int));
	os.write((const char*)(&imgMat.cols), sizeof(int));
	os.write((const char*)(&type), sizeof(int));
	os.write((const char*)(imgMat.data), imgMat.elemSize() * imgMat.total());

	os.close();

	return true;
}

bool IOFileManager::saveImageBIN_OPENCV(unsigned short * data, int width, int height, string path) 
{
	ofstream os;
	os.open(path, ios::out | ios::binary);

	//---- Exit if can't open file
	if (!os.is_open()) { return false; }

	//---- Convert data into OpenCV mat
	Mat imgMat = Mat(height, width, CV_16UC1, data).clone();

	int type = imgMat.type();
	os.write((const char*)(&imgMat.rows), sizeof(int));
	os.write((const char*)(&imgMat.cols), sizeof(int));
	os.write((const char*)(&type), sizeof(int));
	os.write((const char*)(imgMat.data), imgMat.elemSize() * imgMat.total());

	os.close();

	return true;
}

bool IOFileManager::saveImageBIN_LZ4(byte * data, int width, int height, string path, int btpx) 
{
	ofstream os;
	os.open(path, ios::out | ios::binary);

	//---- Exit if can't open file
	if (!os.is_open()) { return false; }

	char * compressedData = (char *)calloc(COMPRESSED_WIDTH * COMPRESSED_HEIGHT, sizeof(char));

	int size = LZ4_compress_fast((char *)data, compressedData, COMPRESSED_WIDTH * COMPRESSED_HEIGHT * sizeof(char), COMPRESSED_WIDTH * COMPRESSED_HEIGHT * sizeof(char), 1);

	os.write((const char*)compressedData, size);
	os.close();

	return true;
}

bool IOFileManager::saveImageBIN_LZ4(unsigned short * data, int width, int height, string path) 
{
	ofstream os;
	os.open(path, ios::out | ios::binary);

	//---- Exit if can't open file
	if (!os.is_open()) { return false; }

	char * compressedData = (char *)calloc(COMPRESSED_WIDTH * COMPRESSED_HEIGHT, sizeof(short));

	int size = LZ4_compress_fast((char *)data, compressedData, COMPRESSED_WIDTH * COMPRESSED_HEIGHT * sizeof(short), COMPRESSED_WIDTH * COMPRESSED_HEIGHT * sizeof(short), 1);

	os.write((const char*)compressedData, size);
	os.close();

	return true;
}

//-----------------------------------------------------------------

bool IOFileManager::saveImageBMP(byte * data, int width, int height, string path, int flag)
{
	//---- We assume here that the extension of the file was not set!
	path = path + ".bmp";

	bool procStatus = true;

	switch (flag)
	{
	case METHOD_WINDOWS: procStatus = saveImageBMP_WINDOWS(data, width, height, path);  break;
	case METHOD_OPENCV: procStatus = saveImageBMP_OPENCV(data, width, height, path); break;
	default: procStatus = false; break;
	}

	return procStatus;
}

bool IOFileManager::saveImagePNG(byte * data, int width, int height, string path, int flag)
{
	//---- We assume here that the extension of the file was not set!
	path = path + ".png";

	bool procStatus = true;

	switch (flag)
	{
	case METHOD_OPENCV: procStatus = saveImagePNG_OPENCV(data, width, height, path); break;
	default: procStatus = false; break;
	}

	return procStatus;
}

bool IOFileManager::saveImageJPG(byte * data, int width, int height, string path, int flag)
{
	//---- We assume here that the extension of the file was not set!
	path = path + ".jpg";

	bool procStatus = true;

	switch (flag)
	{
	case METHOD_OPENCV: procStatus = saveImageJPG_OPENCV(data, width, height, path); break;
	default: procStatus = false; break;
	}

	return procStatus;
}

bool IOFileManager::saveImageBIN(byte * data, int width, int height, string path, int btpx, int flag)
{
	bool procStatus = true;

	switch (flag)
	{
	case METHOD_WINDOWS:  	path = path + ".dat";  saveImageBIN_WINDOWS(data, width, height, path, btpx);  break;
	case METHOD_OPENCV:  	path = path + ".dat"; saveImageBIN_OPENCV(data, width, height, path, btpx);  break;
	case METHOD_LZ4:  	path = path + ".lz4";  saveImageBIN_LZ4(data, width, height, path, btpx);  break;
	}

	return procStatus;
}

bool IOFileManager::saveImageBIN(unsigned short * data, int width, int height, string path, int flag)
{
	bool procStatus = true;

	switch (flag)
	{
	case METHOD_WINDOWS: path = path + ".dat"; saveImageBIN_WINDOWS(data, width, height, path);  break;
	case METHOD_OPENCV:  path = path + ".dat"; saveImageBIN_OPENCV(data, width, height, path);  break;
	case METHOD_LZ4: path = path + ".lz4"; saveImageBIN_LZ4(data, width, height, path);  break;
	}

	return procStatus;
}

//-----------------------------------------------------------------

bool IOFileManager::loadBinaryRange_WINDOWS(string path)
{
	return false;
}

bool IOFileManager::loadBinaryRange_OPENCV(string path)
{
	ifstream ifs(path, ios::binary);
	if (!ifs.is_open()) { return false; }

	Mat  in_mat;

	if (!ifs.is_open()) {
		return false;
	}

	int rows, cols, type;
	ifs.read((char*)(&rows), sizeof(int));
	if (rows == 0) {
		return true;
	}
	ifs.read((char*)(&cols), sizeof(int));
	ifs.read((char*)(&type), sizeof(int));

	in_mat.release();
	in_mat.create(rows, cols, CV_16UC1);
	ifs.read((char*)(in_mat.data), in_mat.elemSize() * in_mat.total());

	ifs.close();

	//DEBUG---- TEST THAT THE IMAGE IS THE SAME
	//in_mat.convertTo(in_mat, CV_8UC3, 0.032);
	//imwrite("testcv.jpg", in_mat);

	return false;
}

bool IOFileManager::loadBinaryRange_LZ4(string path)
{
	ifstream ist(path, ios::binary);
	if (!ist.is_open()) { return false;  }

	char * compressedData = (char *)calloc(COMPRESSED_WIDTH * COMPRESSED_HEIGHT, sizeof(short));

	Mat test = Mat::zeros(COMPRESSED_HEIGHT, COMPRESSED_WIDTH, CV_16UC1);

	ist.read((char *)(compressedData), COMPRESSED_WIDTH * COMPRESSED_HEIGHT * sizeof(short));

	streamsize ssize = ist.gcount();

	LZ4_decompress_safe(compressedData, (char *)test.data, ssize, COMPRESSED_WIDTH * COMPRESSED_HEIGHT * sizeof(short));

	test.convertTo(test, CV_8UC3, 0.032);
	imwrite("testlz.jpg", test);

	return true;
}

//-----------------------------------------------------------------

void IOFileManager::renderSkeletonBody(Mat * mat, float * data, int dataSize, int * atlas, int atlasSize)
{
	for (int i = 0; i < atlasSize; i+=2)
	{
		renderSkeletonBone(mat, data, atlas[i], atlas[i+1]);
	}
}

void IOFileManager::renderSkeletonBone(Mat * mat, float * data, int joint1, int joint2)
{
	//---- Here extract the data from the data array. 
	int ix1 = 2 * joint1, iy1 = 2 * joint1 + 1;
	float x1 = data[ix1], y1 = data[iy1];

	int ix2 = 2 * joint2, iy2 = 2 * joint2 + 1;
	float x2 = data[ix2], y2 = data[iy2];

	cv::line(*mat, Point(x1, y1), Point(x2, y2), Scalar(0, 0, 255), 3);
	cv::circle(*mat, Point(x1, y1), 3, Scalar(0, 255, 0), 3);
	cv::circle(*mat, Point(x2, y2), 3, Scalar(0, 255, 0), 3);
}

bool IOFileManager::saveSkeletonImage(float * data, int dataSize, int * atlas, int atlasSize, int width, int height, string path)
{
	try
	{
		path = path + ".jpg";

		Mat imgMat = Mat::zeros(height, width, CV_8UC3);

		renderSkeletonBody(&imgMat, data, dataSize, atlas, atlasSize);

		cv::imwrite(path, imgMat);

		return true;
	}
	catch (Exception e)
	{
		return false;
	}
}
