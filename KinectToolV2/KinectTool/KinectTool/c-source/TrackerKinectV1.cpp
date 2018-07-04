#include <windows.h>

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

#include "SkeletonBody.h"
#include "TrackerKinectV1.h"


int TrackerKinectV1::getSettingsSkeletonCount()
{
	return MAX_BODY_COUNT;
}


bool TrackerKinectV1::init() 
{
	HRESULT queryStatus;

	//---- Number of detected sensors
	int sensorCount = 0;

	//---- Get number of kinect sensors available
	queryStatus = NuiGetSensorCount(&sensorCount);
	if (FAILED(queryStatus)) { return false; }

	//---- Here we use only one kinect sensor, test if we can bind it
	queryStatus = NuiCreateSensorByIndex(0, &sensor);
	if (FAILED(queryStatus)) { return false; }

	//---- Get the status of the '0' sensor, if not OK then exit
	queryStatus = sensor->NuiStatus();
	if (queryStatus != S_OK) { return false; }


	//---- Attempt to initialize and exit if fail to do
	queryStatus = sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_SKELETON);
	if (!SUCCEEDED(queryStatus)) { return false; }

	eventStreamRGB = CreateEventW(NULL, TRUE, FALSE, NULL);
	eventStreamDEPTH = CreateEventW(NULL, TRUE, FALSE, NULL);
	eventStreamSKELETON = CreateEventW(NULL, TRUE, FALSE, NULL);

	//---- Open stream COLOR
	queryStatus = sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, eventStreamRGB, &streamRGB);
	if (!SUCCEEDED(queryStatus)) { return false; }

	//---- Open stream DEPTH 
	queryStatus = sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX, NUI_IMAGE_RESOLUTION_640x480, 0, 2, eventStreamDEPTH, &streamDEPTH);
	if (!SUCCEEDED(queryStatus)) { return false; }

	//---- Open stream SKELETON
	queryStatus = sensor->NuiSkeletonTrackingEnable(eventStreamSKELETON, 0);
	if (!SUCCEEDED(queryStatus)) { return false; }

	isInit = true;

	//---- For body mask
	tempMaskData.allocate(DEPTH_WIDTH, DEPTH_HEIGHT, 1);

	isDeviceClosed = false;

	return sensor;
}

bool TrackerKinectV1::update() 
{
	return true;
}

int TrackerKinectV1::getWidthRGB() 
{
	return COLOR_WIDTH;
}

int TrackerKinectV1::getWidthDPT() 
{
	return DEPTH_WIDTH;
}

int TrackerKinectV1::getHeightRGB() 
{
	return COLOR_HEIGHT;
}

int TrackerKinectV1::getHeightDPT() 
{
	return DEPTH_HEIGHT;
}


bool TrackerKinectV1::getframeRGB(ofPixels & data)
{
	if (isDeviceClosed) { return false; }

	data.allocate(COLOR_WIDTH, COLOR_HEIGHT, 4);
	data.setColor(ofColor::black);

	//---- Check if the device was initialized
	if (!isInit) { return false; }

	//---- Unlike previous implementation we skip this step
	//if (WAIT_OBJECT_0 != WaitForSingleObject(eventStreamRGB, 0)) { return false; }

	HRESULT queryStatus;

	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;

	//---- Attempt to grab the color frame. If can't set status and exit.
	queryStatus = sensor->NuiImageStreamGetNextFrame(streamRGB, 0, &imageFrame);
	if (FAILED(queryStatus)) { return false; }

	//---- Lock the frame texture
	INuiFrameTexture * texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);

	if (LockedRect.Pitch != 0)
	{
		const BYTE* curr = (const BYTE*)LockedRect.pBits;
		const BYTE* dataEnd = curr + (COLOR_WIDTH * COLOR_HEIGHT) * 4;

		//---- Copy the grabbed data to the output container
		for (int i = 0; i < (COLOR_WIDTH * COLOR_HEIGHT); i++)
		{
			data[4 * i + 0] = *(curr + 4 * i + 2);
			data[4 * i + 1] = *(curr + 4 * i + 1);
			data[4 * i + 2] = *(curr + 4 * i + 0);
			data[4 * i + 3] = 255;
		}

	}

	texture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(streamRGB, &imageFrame);

	return true;
}

bool TrackerKinectV1::getframeDPT(ofShortPixels & data)
{
	if (isDeviceClosed) { return false; }

	data.allocate(DEPTH_WIDTH, DEPTH_HEIGHT, 1);
	data.setColor(ofColor::black);

	//---- Check if the device was initialized
	if (!isInit) { return false; }

	//---- I am not sure why in the original samples it is necessary to check
	//---- If the frame is ready to be captured (everything works without this line).
	//---- So, let's add it just in case.
	if (WAIT_OBJECT_0 != WaitForSingleObject(eventStreamDEPTH, 0)) { return false; }

	HRESULT queryStatus;

	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;

	queryStatus = sensor->NuiImageStreamGetNextFrame(streamDEPTH, 0, &imageFrame);
	if (FAILED(queryStatus)) { return false; }

	//---- Lock the frame texture
	INuiFrameTexture* texture = imageFrame.pFrameTexture;

	BOOL bnear = FALSE;

	//----- Here we capture DEPTH data and PLAYER BODY MASK
	sensor->NuiImageFrameGetDepthImagePixelFrameTexture(streamDEPTH, &imageFrame, &bnear, &texture);

	texture->LockRect(0, &LockedRect, NULL, 0);

	if (LockedRect.Pitch != 0)
	{
		const USHORT* curr = (const USHORT*)LockedRect.pBits;

		//---- Data [RANGE] - 2*px + 1
		//---- Data [BODYMASK] - 2*px
		for (int px = 0; px < DEPTH_WIDTH * DEPTH_HEIGHT; px++)
		{
			data[px] = *(curr + 2 * px + 1);

			if (*(curr + 2 * px) != 0)
				tempMaskData[px] = 255;
			else
				tempMaskData[px] = 0;
		}
	}

	isMaskUpdated = true;

	texture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(streamDEPTH, &imageFrame);

	return true;
}

bool TrackerKinectV1::getframeSKL(SkeletonBodyCollection & skeletonCollection)
{
	if (isDeviceClosed) { return false; }

	if (!isInit) { return false; }

	//---- Unlike previous implementation we skip this step
	if (WAIT_OBJECT_0 != WaitForSingleObject(eventStreamSKELETON, 0)) {  return false; }

	HRESULT queryStatus;

	NUI_SKELETON_FRAME skeletonFrame = { 0 };

	queryStatus = sensor->NuiSkeletonGetNextFrame(0, &skeletonFrame);
	if (FAILED(queryStatus)) { return false; }

	//---- For each skeleton body 
	for (int i = 0; i < MAX_BODY_COUNT; i++)
	{
		//---- Get tracking state. 
		//---- In Kinect SDK memory is allocated for all SKELETON_MAX_COUNT (default 6)
		//---- bodies. Need to check if the paticular body is being tracked.
		NUI_SKELETON_TRACKING_STATE trackingState = skeletonFrame.SkeletonData[i].eTrackingState;

		if (trackingState == NUI_SKELETON_TRACKED)
		{
			const NUI_SKELETON_DATA& skeleton = skeletonFrame.SkeletonData[i];

			//---- Copy the skeleton tracking points to the skeleton pool
			for (int jointID = 0; jointID < NUI_SKELETON_POSITION_COUNT; jointID++)
			{
				Vector4 joint = skeleton.SkeletonPositions[jointID];

				LONG x, y;
				USHORT depth;
				
				//---- Skeleton join in workd coordinates
				vector3f vWC(joint.x, joint.y, joint.z);

				//---- Transform coordinates
				NuiTransformSkeletonToDepthImage(joint, &x, &y, &depth);

				//---- Image based coordinates
				float tx = static_cast<float>(x * 640 / 320);
				float ty = static_cast<float>(y * 480 / 240);

				vector4f v(tx, ty, 0, 1);

				//---- Set flag if a particular joint is not tracked
				if (skeleton.eSkeletonPositionTrackingState[jointID] == NUI_SKELETON_POSITION_NOT_TRACKED)
				{
					v.w = 0;
				}

				//---- Push data
				skeletonCollection.getSkeletonBody(i)->setSkeletonJoint(v, jointID);
				skeletonCollection.getSkeletonBody(i)->setSkeletonJointWC(vWC, jointID);
			}

			skeletonCollection.getSkeletonBody(i)->setIsTracked(true);
		}
		else
		{

			skeletonCollection.getSkeletonBody(i)->setIsTracked(false);
		}
	}

	return true;
}

bool TrackerKinectV1::getframeBODY(ofPixels & data)
{
	data.allocate(DEPTH_WIDTH, DEPTH_HEIGHT, 1);
	data.setColor(ofColor::black);

	if (isMaskUpdated)
	{
		data = ofPixels(tempMaskData);
		isMaskUpdated = false;
		return true;
	}

	return false;
}

bool TrackerKinectV1::generateImageChromakey(ofPixels  & dataChromakey, ofPixels & dataRGB, ofShortPixels & dataDPT, ofPixels & dataBODY)
{

	if (!dataRGB.isAllocated() || !dataDPT.isAllocated() || !dataBODY.isAllocated()) { return false; }
	if (dataRGB.size() == 0 || dataDPT.size() == 0 || dataBODY.size() == 0) {  return false; }

	dataChromakey.allocate(DEPTH_WIDTH, DEPTH_HEIGHT, 4);

	unsigned short * d = dataDPT.getPixels();

	for (int y = 0; y < DEPTH_HEIGHT; y++)
	{
		for (int x = 0; x < DEPTH_WIDTH; x++)
		{
			
			int cindex = x + y * DEPTH_WIDTH;

			long colorX = 0;
			long  colorY = 0;

			unsigned short depth = d[cindex];
			

			try
			{
				HRESULT queryStatus = NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(NUI_IMAGE_RESOLUTION_640x480, NUI_IMAGE_RESOLUTION_640x480, NULL, x, y, depth << 3, &colorX, &colorY);
				if (FAILED(queryStatus)) { return false; }
			}
			catch (...) { cout << "Exception!" << endl;  return false; }
			

			if (dataBODY[cindex] == 255)
			{
				//colorX >= 0 && colorY >= 0 && colorX < COLOR_WIDTH && colorY < COLOR_HEIGHT &&  &&
			

				ofColor c = dataRGB.getColor(colorX, colorY);

				dataChromakey.setColor(x, y, ofColor(c.r, c.g, c.b, 255));
			}
			else
			{
				dataChromakey.setColor(x, y, ofColor(0, 0, 0, 0));
			}
			

		}
	}

	return true;
}

bool TrackerKinectV1::close()
{
	isDeviceClosed = true; 
	//---- Release the main sensor
	if (sensor) { sensor->NuiShutdown(); }

	if (eventStreamRGB != INVALID_HANDLE_VALUE) { CloseHandle(eventStreamRGB); }
	if (eventStreamDEPTH != INVALID_HANDLE_VALUE) { CloseHandle(eventStreamDEPTH); }
	if (eventStreamSKELETON != INVALID_HANDLE_VALUE) { CloseHandle(eventStreamSKELETON); }

	return true;
}