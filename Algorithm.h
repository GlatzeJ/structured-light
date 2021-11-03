#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>

#define pi 3.1415926
#define DEBUG true

using namespace std;

namespace Algorithm {
	vector<vector<cv::Mat>> splitImages(vector<cv::Mat>& images);

	cv::Mat decodeMask(vector<cv::Mat> maskImage, int height, int width);

	cv::Mat decodeWrappedPhase(vector<cv::Mat>& sinImages, cv::Mat& mask, int height, int width);
	
	cv::Mat decodeGrayCode(vector<cv::Mat>& grayCodeImages, cv::Mat& mask, int height, int width);

	cv::Mat decodeUnwrappedPhase(cv::Mat& sinImages, cv::Mat& grayCodeImages, cv::Mat& mask);
}