#pragma once
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>

class camera
{
public:
	// 3x3 Mat CV32F
	cv::Mat instrisincMatrix;
	// 1x5 Mat CV32F
	cv::Mat distortionCoeff;
	// 3x4 Mat CV32F
	cv::Mat extrinsicsMatrix;

	camera();
	~camera();
	void setIntrisincParams(cv::Mat instrisinc);
	void setDistortionCoeff(cv::Mat Dcoff);
	void setExtrinsicsMatrix(cv::Mat ExtriM);



};

