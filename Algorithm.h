#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <fstream>

#include "cameraParams.h"

#define EPS 0
#define DEBUG true
#define PI 3.1415926
using namespace std;

namespace Algorithm {
	vector<vector<cv::Mat>> splitImages(vector<cv::Mat>& images);

	cv::Mat decodeMask(vector<cv::Mat> maskImage, int height, int width);

	cv::Mat decodeWrappedPhase(vector<cv::Mat>& sinImages, cv::Mat& mask, int height, int width);
	
	cv::Mat decodeGrayCode(vector<cv::Mat>& grayCodeImages, cv::Mat& mask, int height, int width);

	cv::Mat decodeUnwrappedPhase(cv::Mat& sinImages, cv::Mat& grayCodeImages, cv::Mat& mask);

	cv::Mat multiHeterodyne(std::vector<cv::Mat>& images, double f1, double f2, double f3, int n, cv::Mat& mask);

	cv::Mat unwrappingPhase(std::vector<cv::Mat>& images);

	cv::Mat unsortTriangulate(cv::Mat& mask, cv::Mat& unwrapImg, cv::Mat& proImg, cv::Mat& R, cv::Mat& t, cv::Mat& k1,cv::Mat& k2, cv::Mat& d1, int freq);

	void readParams(camera& leftCamera, camera& rightCamera, string& filePath);
}
