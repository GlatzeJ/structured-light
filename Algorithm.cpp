#include <cmath>

#include "Algorithm.h"

vector<vector<cv::Mat>> Algorithm::splitImages(vector<cv::Mat>& images) {
	vector<vector<cv::Mat>> splitResult;
	const int maskImageNum = 2;
	const int grayImageNum = 6;
	const int sinImageNum = 4;
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
	cv::Mat mask;
	cv::Mat white = maskImage[0];
	cv::Mat black = maskImage[1];
	cv::Mat sub = white - black;
	cv::threshold(sub, mask, 0, 1, cv::THRESH_BINARY|cv::THRESH_OTSU);
#ifdef DEBUG
	cv::imwrite("..//structured-light-data//result//mask.tif", mask);
#endif // DEBUG
	return mask;
}

cv::Mat Algorithm::decodeWrappedPhase(vector<cv::Mat>& sinImages, cv::Mat& mask, int height, int width) {
	cv::Mat wrappedPhase = cv::Mat::zeros(height, width, CV_32FC1);
	cv::Mat numerator = cv::Mat::zeros(height, width, CV_32FC1);
	cv::Mat denominator = cv::Mat::zeros(height, width, CV_32FC1);
	const int sinImageNum = sinImages.size();
	for (size_t i = 0; i < sinImages.size();i++) {
		sinImages[i].convertTo(sinImages[i], CV_32FC1);
		numerator += sinImages[i] * sin(i * 2 * pi / sinImageNum);
		denominator += sinImages[i] * cos(i * 2 * pi / sinImageNum);
	}
	wrappedPhase.forEach<float>([&numerator, &denominator, &mask](float& val, const int *pos) {
		if (mask.ptr<uchar>(pos[0])[pos[1]]) {
			const float numeratorPixel = numerator.ptr<float>(pos[0])[pos[1]];
			const float denominatorPixel = denominator.ptr<float>(pos[0])[pos[1]];
			val = atan2(-numeratorPixel, denominatorPixel);
		}else {
			return;
		}
		});
	wrappedPhase += pi;
#ifdef DEBUG
	cv::imwrite("..//structured-light-data//result//wrappedPhase.tif", wrappedPhase);
#endif // DEBUG
	return wrappedPhase;
}

cv::Mat Algorithm::decodeGrayCode(vector<cv::Mat>& grayCodeImages, cv::Mat& mask, int height, int width) {
	// 0-255
	cv::Mat codeOrder = cv::Mat::zeros(height, width, CV_8UC1);
	cv::Mat codeTmp = cv::Mat::zeros(height, width, CV_8UC1);
	const int graySize = grayCodeImages.size();
	for (size_t i = 0; i < grayCodeImages.size();i++) {
		cv::threshold(grayCodeImages[i], grayCodeImages[i], 0, 1, cv::THRESH_BINARY | cv::THRESH_OTSU);
		cv::bitwise_xor(codeTmp, grayCodeImages[i], codeTmp);
		codeOrder += codeTmp * pow( 2, graySize - 1 - i);
	}
	cv::multiply(codeOrder, mask, codeOrder);
#ifdef DEBUG
	cv::imwrite("..//structured-light-data//result//codeOrder.tif", codeOrder);
#endif
	return codeOrder;
}

cv::Mat Algorithm::decodeUnwrappedPhase(cv::Mat& wrappedPhase, cv::Mat& codeOrder, cv::Mat& mask){
	codeOrder.convertTo(codeOrder, CV_32FC1);
	cv::Mat unwrappedPhase = wrappedPhase + 2 * pi * codeOrder;
#ifdef DEBUG
	cv::imwrite("..//structured-light-data//result//unwrappedPhase.tif", unwrappedPhase);
#endif // DEBUG
	return unwrappedPhase;
}