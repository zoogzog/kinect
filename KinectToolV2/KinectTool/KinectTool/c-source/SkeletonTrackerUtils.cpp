#include "SkeletonTrackerUtils.h"


ofPixels SkeletonTrackerUtils::convertRangeToGrayscale(ofShortPixels rangepx)
{
	ofPixels output;

	output.allocate(rangepx.getWidth(), rangepx.getHeight(), ofImageType::OF_IMAGE_GRAYSCALE);

	for (int i = 0; i < rangepx.getWidth() * rangepx.getHeight(); i++)
	{
		output[i] = (double)rangepx[i] / 8000 * 255;
	}

	return output;
}