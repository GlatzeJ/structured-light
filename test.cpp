#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "Algorithm.h"

int main() {
	vector<cv::Mat> images;
	vector<string> imageName;
	cv::glob("D://projects//structured-light-data//grayCode//", imageName, false);
	for (size_t i = 0; i < 12; i++) {
		cv::Mat imgTmp = cv::imread(imageName[i], cv::IMREAD_GRAYSCALE);
		images.emplace_back(imgTmp);
	}
	const int height = images[0].rows;
	const int width = images[0].cols;
	// split image
	vector<vector<cv::Mat>> imgVector = Algorithm::splitImages(images);
	// decode mask
	cv::Mat mask = Algorithm::decodeMask(imgVector[0], height, width);
	// decode gray code
	cv::Mat grayCodeOrder = Algorithm::decodeGrayCode(imgVector[1], mask, height, width);
	// decode wrapped phase
	cv::Mat wrappedPhase = Algorithm::decodeWrappedPhase(imgVector[2], mask, height, width);
	// decode unwrapped phase
	cv::Mat unwrappedPhase = Algorithm::decodeUnwrappedPhase(wrappedPhase, grayCodeOrder, mask);

	return 0;
}