#pragma
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <time.h>

#include "Algorithm.h"
#include "ApparatusParamsCls.h"



int main() {
	clock_t start, end;

	string filePath = "..//structured-light-data//calibration//SLSParam.txt";
	camera leftCamera, rightCamera;
	Algorithm::readParams(leftCamera, rightCamera, filePath);
	std::cout << leftCamera.instrisincMatrix << std::endl;
	std::cout << leftCamera.distortionCoeff<< std::endl;
	std::cout << rightCamera.extrinsicsMatrix << std::endl;
	vector<cv::Mat> images;
	vector<string> imageName;
	cv::glob("..//structured-light-data//multiFre//", imageName, false);
	for (size_t i = 0; i < 12; i++) {
		cv::Mat imgTmp = cv::imread(imageName[i], cv::IMREAD_GRAYSCALE);
		imgTmp.convertTo(imgTmp, CV_32FC1);
		images.emplace_back(imgTmp);
	}

	double f1 = 59;
	double f2 = 64;
	double f3 = 70;
	
	cv::Mat mask = cv::Mat::ones(images[0].size(), CV_32FC1);
	cv::Mat res = Algorithm::multiHeterodyne(images, f1, f2, f3, 4, mask);
	//res.convertTo(res, CV_8UC1);
	cv::namedWindow("ss", cv::WINDOW_AUTOSIZE);
	cv::normalize(res, res, 0, 1, cv::NORM_MINMAX);
	cv::imshow("ss", res);
	cv::waitKey(0);
	/*
	vector<cv::Mat> images;
	vector<string> imageName;
	cv::glob("..//structured-light-data//grayCode//", imageName, false);
	for (size_t i = 0; i < 12; i++) {
		cv::Mat imgTmp = cv::imread(imageName[i], cv::IMREAD_GRAYSCALE);
		images.emplace_back(imgTmp);
	}
	const int height = images[0].rows;
	const int width = images[0].cols;
	start = clock();
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
	end = clock();
	cout << "running time" << (double)(end - start) / CLOCKS_PER_SEC << endl;
	*/
	return 0;
}