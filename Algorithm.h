#pragma once
#include <iostream>
#include <vector>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>

using namespace std;

namespace Algorithm {
	cv::Mat decodeWrappedPhase(vector<cv::Mat>& sinImages);
	
	cv::Mat decodeGrayCode(vector<cv::Mat>& grayCodeImages);

	cv::Mat decodeUnwrappedPhase(cv::Mat& sinImages, cv::Mat& grayCodeImages, cv::Mat mask);
}