#include <iostream>
#include <windows.h>

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

#include "KinectDevice.h"
#include "SkeletonBody.h"

using namespace std;

//-----------------------------------------------------------------

//---- This is required to define a singleton class
KinectDevice * KinectDevice::instance = 0;

//---- Method to get the class instance. This class is a singleton.
KinectDevice* KinectDevice::getInstance()
{
    if (instance == 0)
    {
        instance = new KinectDevice();
    }

    return instance;
}

//---- This is a private constructor
KinectDevice::KinectDevice()
{
	isInit = false;

	 sensor = NULL;

	 //---- Default status is OK
	 STATUS_FLAG = STATUS_OK;


}

//-----------------------------------------------------------------

bool KinectDevice::init()
{
	//---- This variable is used to test if our queries to initialize
	//---- the Kinect device are successful or not
	HRESULT queryStatus;

	//---- Number of detected sensors
	int sensorCount = 0;

	//---- Get number of kinect sensors available
	queryStatus = NuiGetSensorCount(&sensorCount);
	if (FAILED(queryStatus)) { STATUS_FLAG = STATUS_NOSENSORFOUND; return false; }

	//---- Here we use only one kinect sensor, test if we can bind it
	queryStatus = NuiCreateSensorByIndex(0, &sensor);
	if (FAILED(queryStatus)) { STATUS_FLAG = STATUS_FAILCREATE; return false;}

	//---- Get the status of the '0' sensor, if not OK then exit
	queryStatus = sensor->NuiStatus();
	if (queryStatus != S_OK) { STATUS_FLAG = STATUS_FAILSTATUS; return false; } 


	//---- Attempt to initialize and exit if fail to do
	queryStatus = sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_SKELETON);
	if (!SUCCEEDED(queryStatus)) { STATUS_FLAG = STATUS_FAILINIT; return false; }


	eventStreamRGB = CreateEvent(NULL, TRUE, FALSE, NULL);
	eventStreamDEPTH = CreateEvent(NULL, TRUE, FALSE, NULL);
	eventStreamSKELETON = CreateEvent(NULL, TRUE, FALSE, NULL);

	//---- Open stream COLOR
    queryStatus = sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR,  NUI_IMAGE_RESOLUTION_640x480, 0, 2, eventStreamRGB,  &streamRGB);
	if (!SUCCEEDED(queryStatus)) { STATUS_FLAG = STATUS_OSTREAM_COLOR; return false; }

	//---- Open stream DEPTH 
	queryStatus = sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH,  NUI_IMAGE_RESOLUTION_640x480, 0, 2, eventStreamDEPTH,  &streamDEPTH);
	if (!SUCCEEDED(queryStatus)) { STATUS_FLAG = STATUS_OSTREAM_DEPTH; return false; }

	//---- Open stream SKELETON
	queryStatus = sensor->NuiSkeletonTrackingEnable(eventStreamSKELETON, 0);
	if (!SUCCEEDED(queryStatus)) { STATUS_FLAG = STATUS_OSTREAM_SKELETON; return false; }
	
	STATUS_FLAG = STATUS_OK;
	
	isInit = true;

    return sensor;

	
}

bool KinectDevice::release ()
{
	if (isInit)
	{
		//---- Release the main sensor
		if (sensor) { sensor->NuiShutdown(); }

		if (eventStreamRGB != INVALID_HANDLE_VALUE) { CloseHandle(eventStreamRGB); }
		if (eventStreamDEPTH != INVALID_HANDLE_VALUE) { CloseHandle(eventStreamDEPTH); }
		if (eventStreamSKELETON != INVALID_HANDLE_VALUE) { CloseHandle(eventStreamSKELETON); }

		return true;
	}

	return false;
}

//-----------------------------------------------------------------

int KinectDevice::getKinectStatus ()
{
	return STATUS_FLAG;
}


bool KinectDevice::getKinectFrameColor (byte * dest)
{
	//---- Check if the device was initialized
	if (!isInit) { STATUS_FLAG = STATUS_NOT_INIT; return false; }

	//---- I am not sure why in the original samples it is necessary to check
	//---- If the frame is ready to be captured (everything works without this line).
	//---- So, let's add it just in case.
	if ( WAIT_OBJECT_0 != WaitForSingleObject(eventStreamRGB, 0)) { STATUS_FLAG = STATUS_WAIT_COLOR; return false; }

	HRESULT queryStatus;

	NUI_IMAGE_FRAME imageFrame;
    NUI_LOCKED_RECT LockedRect;

	//---- Attempt to grab the color frame. If can't set status and exit.
	queryStatus = sensor->NuiImageStreamGetNextFrame(streamRGB, 0, &imageFrame);
	if (FAILED(queryStatus)) { STATUS_FLAG =  STATUS_FAILFRAMERGB; return false; }

	//---- Lock the frame texture
    INuiFrameTexture * texture = imageFrame.pFrameTexture;
    texture->LockRect(0, &LockedRect, NULL, 0);

    if (LockedRect.Pitch != 0)
    {
        const BYTE* curr = (const BYTE*) LockedRect.pBits;
        const BYTE* dataEnd = curr + (DEFAULT_WIDTH*DEFAULT_HEIGHT)*4;

		//---- Copy the grabbed data to the output container
		while (curr < dataEnd) 
		{
            *dest++ = *curr++;
        }
    }

    texture->UnlockRect(0);
    sensor->NuiImageStreamReleaseFrame(streamRGB, &imageFrame);

	return true;

}

bool KinectDevice::getKinectFrameDepth (byte *dest)
{
	//---- Check if the device was initialized
	if (!isInit) { STATUS_FLAG = STATUS_NOT_INIT; return false; }

	//---- I am not sure why in the original samples it is necessary to check
	//---- If the frame is ready to be captured (everything works without this line).
	//---- So, let's add it just in case.
	if (WAIT_OBJECT_0 != WaitForSingleObject(eventStreamDEPTH, 0)) { STATUS_FLAG = STATUS_WAIT_DEPTH; return false; }

	HRESULT queryStatus;

	NUI_IMAGE_FRAME imageFrame;
    NUI_LOCKED_RECT LockedRect;

	queryStatus = sensor->NuiImageStreamGetNextFrame(streamDEPTH, 0, &imageFrame);
	if (FAILED(queryStatus)) { STATUS_FLAG = STATUS_FAILFRAMEDPTH; return false; }

	//---- Lock the frame texture
    INuiFrameTexture* texture = imageFrame.pFrameTexture;
    texture->LockRect(0, &LockedRect, NULL, 0);

    if (LockedRect.Pitch != 0)
    {
        const USHORT* curr = (const USHORT*) LockedRect.pBits;
		const USHORT* dataEnd = curr + (DEFAULT_WIDTH*DEFAULT_HEIGHT);


        while (curr < dataEnd) 
		{
            //---- Get depth in millimeters
            USHORT depth = NuiDepthPixelToDepth(*curr++);

            //---- Convert to grayscale
            for (int i = 0; i < 3; ++i)
                *dest++ = (BYTE)((double) depth / 8000 * 255);
            *dest++ = 0xff;
        }
    }
    texture->UnlockRect(0);
    sensor->NuiImageStreamReleaseFrame(streamDEPTH, &imageFrame);

	return true;
}

bool KinectDevice::getKinectFrameRange (unsigned short *dest)
{
	//---- Check if the device was initialized
	if (!isInit) { STATUS_FLAG = STATUS_NOT_INIT; return false; }

	//---- I am not sure why in the original samples it is necessary to check
	//---- If the frame is ready to be captured (everything works without this line).
	//---- So, let's add it just in case.
	if (WAIT_OBJECT_0 != WaitForSingleObject(eventStreamDEPTH, 0)) { STATUS_FLAG = STATUS_WAIT_DEPTH; return false; }

	HRESULT queryStatus;

	NUI_IMAGE_FRAME imageFrame;
    NUI_LOCKED_RECT LockedRect;

	queryStatus = sensor->NuiImageStreamGetNextFrame(streamDEPTH, 0, &imageFrame);
	if (FAILED(queryStatus)) { STATUS_FLAG = STATUS_FAILFRAMEDPTH; return false; }

	//---- Lock the frame texture
    INuiFrameTexture* texture = imageFrame.pFrameTexture;
    texture->LockRect(0, &LockedRect, NULL, 0);

    if (LockedRect.Pitch != 0)
    {
        const USHORT* curr = (const USHORT*) LockedRect.pBits;
        const USHORT* dataEnd = curr + (DEFAULT_WIDTH*DEFAULT_HEIGHT);

        while (curr < dataEnd) 
		{
            //---- Get depth in millimeters
            USHORT depth = NuiDepthPixelToDepth(*curr++);

            *dest++ = depth;
        }
    }
    texture->UnlockRect(0);
    sensor->NuiImageStreamReleaseFrame(streamDEPTH, &imageFrame);

	return true;
}

bool KinectDevice::getKinectSkeleton (SkeletonBody * skeletonPool)
{
	if (!isInit) { STATUS_FLAG = STATUS_NOT_INIT; return false; }

	//---- I am not sure why in the original samples it is necessary to check
	//---- If the frame is ready to be captured (everything works without this line).
	//---- So, let's add it just in case.
	if (WAIT_OBJECT_0 != WaitForSingleObject(eventStreamSKELETON, 0)) { STATUS_FLAG = STATUS_WAIT_SKELETON; return false; }

	HRESULT queryStatus;

	NUI_SKELETON_FRAME skeletonFrame = {0};

	queryStatus = sensor->NuiSkeletonGetNextFrame(0, &skeletonFrame);
	if (FAILED(queryStatus)) { STATUS_FLAG = STATUS_FAILFRAMESKLT;  return false; }

	//---- For each skeleton body 
	for (int i = 0; i < SKELETON_MAX_COUNT; i++)
	{
		//---- Get tracking state. 
		//---- In Kinect SDK memory is allocated for all SKELETON_MAX_COUNT (default 6)
		//---- bodies. Need to check if the paticular body is being tracked.
		NUI_SKELETON_TRACKING_STATE trackingState = skeletonFrame.SkeletonData[i].eTrackingState;

		if (trackingState == NUI_SKELETON_TRACKED)
		{
			const NUI_SKELETON_DATA& skeleton = skeletonFrame.SkeletonData[i];

			//---- Set the flag that the skeleton is tracked
			skeletonPool[i].isTracked = true;

			//---- Copy the skeleton tracking points to the skeleton pool
			for (int jointID = 0; jointID < NUI_SKELETON_POSITION_COUNT; jointID++)
			{
				skeletonPool[i].skeletonPosition[jointID] = skeleton.SkeletonPositions[jointID];

				//---- Set flag if a particular joint is not tracked
				if (skeleton.eSkeletonPositionTrackingState[jointID] == NUI_SKELETON_POSITION_NOT_TRACKED) 
				{
					skeletonPool[i].skeletonPosition[jointID].w = 0;
				}
			}
		}
		else
		{
			//---- Set the flag that the skeleton is not tracked
			skeletonPool[i].isTracked = false;
		}
	}

	return true;
}

//-----------------------------------------------------------------

HANDLE & KinectDevice::getEventStreamRGB ()
{
	return eventStreamRGB;
}

HANDLE & KinectDevice::getEventStreamDEPTH ()
{
	return eventStreamDEPTH;
}

HANDLE & KinectDevice::getEventStreamSKELETON ()
{
	return eventStreamSKELETON;
}

//-----------------------------------------------------------------

SkeletonBody KinectDevice::convertSkeletonCoordinates(SkeletonBody  skeletonIN, int width, int height)
{
	SkeletonBody skeletonOUT;

	for (int i = 0; i < SkeletonBody::MAX_SKELETON_JOINT; i++)
	{
		LONG x, y;
		USHORT depth;
		
		Vector4 joint = skeletonIN.skeletonPosition[i];
		
		//---- Transform coordinates
		NuiTransformSkeletonToDepthImage(joint, &x, &y, &depth);

		//---- Image based coordinates
		float tx = static_cast<float>(x * width) / DEFAULT_TRANSFORM_WIDTH;
		float ty = static_cast<float>(y * height) / DEFAULT_TRANSFORM_HEIGHT;
	
		//---- Store
		skeletonOUT.skeletonPosition[i].x = tx;
		skeletonOUT.skeletonPosition[i].y = ty;
		skeletonOUT.skeletonPosition[i].z = joint.z;
		skeletonOUT.skeletonPosition[i].w = joint.w;
	
	}

	skeletonOUT.isTracked = skeletonIN.isTracked;

	return skeletonOUT;
}