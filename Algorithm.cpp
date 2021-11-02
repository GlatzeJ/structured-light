#include <cmath>

#include "Algorithm.h"

vector<vector<cv::Mat>> Algorithm::splitImages(vector<cv::Mat>& images) {
	vector<vector<cv::Mat>> splitResult;
	const int maskImageNum = 2;
	const int grayImageNum = 8;
	const int sinImageNum = 8;
	vector<cv::Mat> maskImage, grayImage, sinImage;
	// The picture projection order is white/black image, gray image, sin image
	for (size_t i = 0; i < images.size(); i++) {
		if (i < maskImageNum) 
			maskImage.emplace_back(images[i]);
		else if (i < maskImageNum+grayImageNum && i >= maskImageNum)
			grayImage.emplace_back(images[i]);
		else if (i < maskImageNum + grayImageNum + sinImageNum && i >= maskImageNum + grayImageNum) 
			sinImage.emplace_back(images[i]);
	}
	splitResult.emplace_back(maskImage);
	splitResult.emplace_back(grayImage);
	splitResult.emplace_back(sinImage);
	return splitResult;
}

cv::Mat Algorithm::decodeMask(vector<cv::Mat> maskImage, int height, int width) {
	cv::Mat mask = cv::Mat::zeros(height, width, CV_8UC1);
	cv::adaptiveThreshold((maskImage[0]-maskImage[1]), mask, 255, 
							cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 5, 3);
	return mask;
}

cv::Mat Algorithm::decodeWrappedPhase(vector<cv::Mat>& sinImages, cv::Mat& mask, int height, int width) {
	cv::Mat wrappedPhase = cv::Mat::zeros(height, width, CV_16FC1);
	cv::Mat numerator = cv::Mat::zeros(height, width, CV_16FC1);
	cv::Mat denominator = cv::Mat::zeros(height, width, CV_16FC1);
	const int sinImageNum = sinImages.size();
	
	for (size_t i = 0; i < sinImages.size();i++) {
		numerator += sinImages[i] * sin(i*2*pi / sinImageNum);
		denominator += sinImages[i] * cos(i * 2 * pi / sinImageNum);
	}
	wrappedPhase.forEach<float>([&numerator, &denominator, &mask](float& val, const int *pos) {
		if (mask.ptr<uchar>(pos[0])[pos[1]]) {
			const float numeratorPixel = numerator.ptr<float>(pos[0])[pos[1]];
			const float denominatorPixel = denominator.ptr<float>(pos[0])[pos[1]];
			val = -atan2(numeratorPixel, denominatorPixel);
		}else {
			return;
		}
		});
	return wrappedPhase;
}

cv::Mat Algorithm::decodeGrayCode(vector<cv::Mat>& grayCodeImages, cv::Mat& mask, int height, int width) {
	// 0-255
	cv::Mat codeOrder = cv::Mat::zeros(height, width, CV_8UC1);
	for (size_t i = 0; i < grayCodeImages.size();i++) {
		cv::adaptiveThreshold(grayCodeImages[i], grayCodeImages[i], 255,
			cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 5, 3);

	}

	return codeOrder;
}