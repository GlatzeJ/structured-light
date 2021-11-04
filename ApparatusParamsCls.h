/*!
* \file ApparatusParamsCls.h
* \date 2017/05/20 16:02
*
* \author zhouyelihua
* Contact: yeahpingye@gmail.com
*  update version: 2020/07/17    denggaoxu
*
* \brief
*
* TODO:
*
* \note
*/
#pragma once
#include <fstream>
#include <iostream>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
using namespace std;
class ApparatusParamsCls
{
private:
	void initRemainingParams();
public:
	//just for convenience , avoiding writing set/get method,
	int imgNum;
	int boardW;// number of  checkerboard's grids in width direction 
	int boardH;
	int boardN;


	double dx;//chessboard's width
	double dy;

	std::vector<std::vector<cv::Point3f> > objectPoint;
	std::vector<std::vector<cv::Point2f> > imagePoint;
	cv::Mat pointCount;//the mat used for storing  the corners' number in all the images;
	cv::Mat instrisincMatrix;
	cv::Mat distortionCoeff;

	std::vector<cv::Mat> rvecs;
	std::vector<cv::Mat> tvecs;

	bool* isUsedArray;//this array is used to indicate the image is used or not

	cv::Size imgSize;
	cv::Size checkerBoardSize;//this size is composed by the number of grid in width and height direction
	int successNumber;//the  number of  the  successfully found images


	std::string imgStoreDirName;
	std::string imgType;
	std::string localTxtFileName;
	std::string patternfileName;

	ApparatusParamsCls(void);
	ApparatusParamsCls(int imgNum, int boardW, int boardH, int boardN, double dx, double dy, std::string imgStoreDirName, std::string imgType);
	void readInitParamsFromStdCin();
	//void initParams();
	void setFixedParams(int imgNum, int boardW, int boardH, double dx, double dy, std::string imgStoreDirName, std::string imgType, std::string patternfileName = "");
	void setIntrisincParams(double fx, double fy, double cx, double cy, double k1, double k2, double k3, double k4);
	~ApparatusParamsCls(void);
};

