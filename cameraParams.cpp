#include "cameraParams.h"

camera::camera()
{
}

camera::~camera()
{
}


void camera::setIntrisincParams(cv::Mat instrisinc)
{
	instrisinc.copyTo(this->instrisincMatrix);
}

void camera::setDistortionCoeff(cv::Mat Dcoff)
{
	Dcoff.copyTo(this->distortionCoeff);
}

void camera::setExtrinsicsMatrix(cv::Mat ExtriM)
{
	ExtriM.copyTo(this->extrinsicsMatrix);
}

