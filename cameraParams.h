#pragma once
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>

class camera
{
public:
	camera();
	~camera();
	cv::Mat instrisincMatrix;
	cv::Mat distortionCoeff;
	cv::Mat extrinsicsMatrix;

private:

};

camera::camera()
{
}

camera::~camera()
{
}
