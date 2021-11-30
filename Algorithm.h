#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <fstream>
#include <QTime>

#include "cameraParams.h"
#include "utils.h"

#define EPS 0
#define PI 3.1415926
using namespace std;

namespace Algorithm {
	vector<vector<cv::Mat>> splitImages(vector<cv::Mat>& images);

	cv::Mat decodeMask(vector<cv::Mat> maskImage, int height, int width);

	cv::Mat decodeWrappedPhase(vector<cv::Mat>& sinImages, cv::Mat& mask, int height, int width);
	
	cv::Mat decodeGrayCode(vector<cv::Mat>& grayCodeImages, cv::Mat& mask, int height, int width);

    cv::Mat decodeUnwrappedPhase(cv::Mat& wrappedPhase, cv::Mat& codeOrder, cv::Mat& mask);

    cv::Mat multiHeterodyne(std::vector<cv::Mat>& images, double f1, double f2, double f3, int n);

	cv::Mat unwrappingPhase(std::vector<cv::Mat>& images);

    vector<vector<float>> unsortTriangulate(cv::Mat& mask, cv::Mat& unwrapImg, cv::Mat& Ac, cv::Mat& Ap, int freq);

	void readParams(camera& leftCamera, camera& rightCamera, string& filePath);
}
