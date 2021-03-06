#include <string>
#include <iomanip> 

#include "KinectMainDriver.h"
#include "KinectDevice.h"
#include "KinectOutput.h"

//-----------------------------------------------------------------

KinectMainDriver * KinectMainDriver::instance = 0;

KinectMainDriver::KinectMainDriver()
{
	kinectDevice = KinectDevice::getInstance();
	kinectOutput = KinectOutput::getInstance();
	
	isInit = false;
}

KinectMainDriver * KinectMainDriver::getInstance()
{
	if (instance == 0)
	{
		instance = new KinectMainDriver();
	}

	return instance;
}

//-----------------------------------------------------------------
bool KinectMainDriver::init(string pathDirRGB, string pathDirDPT, string pathDirDPTI, string pathDirSKLI, string pathFileSKL, string pathLoggerRGB, string pathLoggerDPT, bool isRGB, bool isDPT, bool isDPTI, bool isSKLI, bool isSKL)
{
	PATH_DIRECTORY_RGB = pathDirRGB;
	PATH_DIRECTORY_DPT = pathDirDPT;
	PATH_DIRECTORY_DPTI = pathDirDPTI;
	PATH_DIRECTORY_SKLI = pathDirSKLI;

	isCaptureRGB = isRGB;
	isCaptureDPT = isDPT;
	isCaptureDPTI = isDPTI;
	isCaptureSKLI = isSKLI;
	isCaptureSKL = isSKL;

	//---- Initialize the Kinect device.
	bool isKinectOK = kinectDevice->init();

	if (!isKinectOK) { return false; }

	//---- Initialize the evenet handlers for Kinect data streams.
	hevents[0] = kinectDevice->getEventStreamRGB();
	hevents[1] = kinectDevice->getEventStreamDEPTH();
	hevents[2] = kinectDevice->getEventStreamSKELETON();

	//---- Initialize the logger files

	kinectOutput->init(pathLoggerRGB, pathLoggerDPT, pathLoggerDPT);
	kinectOutput->loggerSkeletonJointOPEN(pathFileSKL);

	isInit = true;

	return true;
}

bool KinectMainDriver::init(string pathDirRGB, string pathDirDPT, string pathDirDPTI, string pathFileSKL, string pathLoggerRGB, string pathLoggerDPT, bool isRGB, bool isDPT, bool isDPTI, bool isSKL)
{
	return init(pathDirRGB, pathDirDPT, pathDirDPTI, "", pathFileSKL, pathLoggerRGB, pathLoggerDPT, isRGB, isDPT, isDPTI, false, isSKL);
}

bool KinectMainDriver::init(string pathDirRGB, string pathDirDPT, string pathFileSKL, string pathLoggerRGB, string pathLoggerDPT, bool isRGB, bool isDPT, bool isSKL)
{
	return init(pathDirRGB, pathDirDPT, "", pathFileSKL, pathLoggerRGB, pathLoggerDPT, isRGB, isDPT, false, isSKL);
}

void KinectMainDriver::update(double timestamp)
{
	if (!isInit) { return;  }

	MsgWaitForMultipleObjects(3, hevents, FALSE, INFINITE, QS_ALLINPUT);

	//---- Capture frames
	//---- In release DON'T use both DPTI and DPT use conversion
	if (isCaptureRGB) { captureFrameRGB(timestamp);  }
	if (isCaptureDPTI & !isCaptureDPT) { captureFrameDPTI(timestamp); }
	if (isCaptureDPT) 
	{ 
		captureFrameDPT(timestamp);

		//---- This conversion is done only if the image representation of the depth map is requested.
		if (isCaptureDPTI)
		{
			for (int i = 0, index = 0; i < KinectDevice::DEFAULT_WIDTH*KinectDevice::DEFAULT_HEIGHT; i++, index += 4)
			{
				dataDepth[index] = (int)((double)dataRange[i] * 255 / 8000);
				dataDepth[index + 1] = (int)((double)dataRange[i] * 255 / 8000);
				dataDepth[index + 2] = (int)((double)dataRange[i] * 255 / 8000);
				dataDepth[index + 3] = 255;
			}
		}
	}
	if (isCaptureSKL) { captureFrameSKL(timestamp);  }
}

//-----------------------------------------------------------------

void KinectMainDriver::captureFrameRGB(double timestamp)
{
	bool isCapturedFrame = kinectDevice->getKinectFrameColor(dataColor);

	//---- A new frame was captured
	if (isCapturedFrame)
	{
		//FIXME
		//---- Get the number of files saved
		//---- Maybe the indexes could be placed in this class, rather than KinectOutput
		int savedFileCount = kinectOutput->getCountSavedFileColor();
	
		//---- !DEBUG! SAVE ONLY ONE IMAGE
		savedFileCount = 0;

		//---- Generate file name, don't add extension!!!
		stringstream fileName;
		fileName << setw(8) << setfill('0') << savedFileCount;

		string pathFileAbsolute = PATH_DIRECTORY_RGB + fileName.str();

		kinectOutput->saveKinectColor(dataColor, KinectDevice::DEFAULT_WIDTH, KinectDevice::DEFAULT_HEIGHT, pathFileAbsolute, true, timestamp);
	}
}

void KinectMainDriver::captureFrameDPT(double timestamp)
{
	//---- Get RANGE data AND the BODYMASK
	//---- If you don't want to get body mask, simply send NULL instead of dataBodyMask
	bool isCapturedRange = kinectDevice->getKinectFrameRange(dataRange, dataBodyMask);

	if (isCapturedRange)
	{
		//FIXME
		//---- Get the number of files saved
		//---- Maybe the indexes could be placed in this class, rather than KinectOutput
		int savedFileCount = kinectOutput->getCountSavedFileRange();

		//---- !DEBUG! SAVE ONLY ONE IMAGE
		savedFileCount = 0;

		//---- Generate file name, don't add extension!!!
		stringstream fileName;
		fileName << setw(8) << setfill('0') << savedFileCount;

		string pathFileAbsolute = PATH_DIRECTORY_DPT + fileName.str();

		kinectOutput->saveKinectRange(dataRange, KinectDevice::DEFAULT_WIDTH, KinectDevice::DEFAULT_HEIGHT, pathFileAbsolute, true, timestamp);
	}
}

void KinectMainDriver::captureFrameDPTI(double timestamp)
{
	bool isCapturedDepth = kinectDevice->getKinectFrameDepth(dataDepth);


	if (isCapturedDepth)
	{
		//FIXME
		//---- Get the number of files saved
		//---- Maybe the indexes could be placed in this class, rather than KinectOutput
		int savedFileCount = kinectOutput->getCountSavedFileDepth();

		savedFileCount = 0;

		//---- Generate file name, don't add extension!!!
		stringstream fileName;
		fileName << setw(8) << setfill('0') << savedFileCount;

		string pathFileAbsolute = PATH_DIRECTORY_DPTI + fileName.str();

		kinectOutput->saveKinectDepth(dataDepth, KinectDevice::DEFAULT_WIDTH, KinectDevice::DEFAULT_HEIGHT, pathFileAbsolute, false);
	}
}

void KinectMainDriver::captureFrameSKL(double timestamp)
{
	bool isCapturedSkeleton = kinectDevice->getKinectSkeleton(skeletonPool);

	kinectOutput->saveKinectSkeleton(skeletonPool, timestamp);

	if (isCaptureSKLI)
	{
		int savedFileCount = kinectOutput->getCountSavedFileDepth();

		savedFileCount = 0;

		//---- Generate file name, don't add extension!!!
		stringstream fileName;
		fileName << setw(8) << setfill('0') << savedFileCount;

		string pathFileAbsolute = PATH_DIRECTORY_SKLI + fileName.str();

		kinectOutput->saveKinectSkeletonImage(skeletonPool, KinectDevice::DEFAULT_WIDTH, KinectDevice::DEFAULT_HEIGHT, pathFileAbsolute);
	}
}

//-----------------------------------------------------------------

byte * KinectMainDriver::getDataColor()
{
	return dataColor;
}

byte * KinectMainDriver::getDataDepth()
{
	return dataDepth;
}

byte * KinectMainDriver::getDataChromakey(bool isColor)
{
	byte  * chromaKey = new byte[KinectDevice::DEFAULT_WIDTH*KinectDevice::DEFAULT_HEIGHT * 4];
	
	//---- Draw chromakey with color (if isColor = true) or just as body mask
	if (isColor)
	{
		for (int y = 0; y < KinectDevice::DEFAULT_HEIGHT; y++)
		{
			for (int x = 0; x < KinectDevice::DEFAULT_WIDTH; x++)
			{
				int cindex = x + y * KinectDevice::DEFAULT_WIDTH;

				long colorX = 0;
				long  colorY = 0;

				KinectDevice::convertColorToDepthXY(x, y, dataRange[cindex], &colorX, &colorY);

				//---- IF the obtained coordinates are in range
				//---- IF the range data is correctly captured by the device
				//---- IF the pixel belongs to body mask
				//---- THEN set color to pixel, otherwise - black
				if (colorX >= 0 && colorY >= 0 && colorX < KinectDevice::DEFAULT_WIDTH && colorY < KinectDevice::DEFAULT_HEIGHT && dataRange[cindex] != 0 && dataBodyMask[cindex] != 0)
				{
					if (dataBodyMask[cindex] != 0)
					{
						long colorIndex = (colorX)+(colorY)* KinectDevice::DEFAULT_WIDTH;

						//---- Need to copy RGBA
						chromaKey[4 * cindex] = dataColor[4 * colorIndex];
						chromaKey[4 * cindex + 1] = dataColor[4 * colorIndex + 1];
						chromaKey[4 * cindex + 2] = dataColor[4 * colorIndex + 2];
						chromaKey[4 * cindex + 3] = dataColor[4 * colorIndex + 3];
					}
				}
				else
				{
					chromaKey[4 * cindex] = 0;
					chromaKey[4 * cindex + 1] = 0;
					chromaKey[4 * cindex + 2] = 0;
					chromaKey[4 * cindex + 3] = 0;
				}
			}
		}
	}

	//---- Draw body mask only as binary image
	else
	{
		for (int i = 0; i < KinectDevice::DEFAULT_WIDTH * KinectDevice::DEFAULT_HEIGHT; i++)
		{
			if (dataBodyMask[i] != 0)
			{
				chromaKey[4 * i] = 255;
				chromaKey[4 * i + 1] = 255;
				chromaKey[4 * i + 2] = 255;
				chromaKey[4 * i + 3] = 255;
			}
			else
			{
				chromaKey[4 * i] = 0;
				chromaKey[4 * i + 1] = 0;
				chromaKey[4 * i + 2] = 0;
				chromaKey[4 * i + 3] = 0;
			}
		}
	}

	return chromaKey;
}

SkeletonBody * KinectMainDriver::getDataSkeletonPool()
{
	return skeletonPool;
}

unsigned short * KinectMainDriver::getDataRange()
{
	return dataRange;
}