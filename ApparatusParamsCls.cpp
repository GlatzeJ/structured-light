/*!
* \file ApparatusParamsCls.cpp
* \date 2017/05/20 16:03
*
* \author zhouyelihua
* Contact: yeahpingye@gmail.com
*  update version: 2020/07/17    denggaoxu
*
* \brief
*
* TODO:parameters of the specified apparatus
*
* \note
*/
#include "ApparatusParamsCls.h"


ApparatusParamsCls::ApparatusParamsCls(void)
{

}

ApparatusParamsCls::ApparatusParamsCls(int imgNum, int boardW, int boardH, int boardN, double dx, double dy, std::string imgStoreDirName, std::string imgType)
	:imgNum(imgNum), boardW(boardW), boardH(boardH), boardN(boardN), dx(dx), dy(dy), imgStoreDirName(imgStoreDirName), imgType(imgType) {
	initRemainingParams();
}
void ApparatusParamsCls::readInitParamsFromStdCin() {

	std::cout << "输入的图像的组数\nthe number of image pairs\n";
	std::cin >> imgNum;
	std::cout << "输入**真实**棋盘格的##横轴##方向的角点个数\n";
	std::cin >> boardW;
	std::cout << "输入**真实**棋盘格的##纵轴##方向的角点个数\n";
	std::cin >> boardH;
	std::cout << "输入**真实**棋盘格的##横轴##单个格子的长度\n";
	std::cin >> dx;
	std::cout << "输入**真实**棋盘格的##纵轴##单个格子的长度\n";
	std::cin >> dy;
	std::cout << "照片存储的文件夹的名字eg: ../cam/cam  \n";
	std::cin >> imgStoreDirName;
	std::cout << "照片的格式  eg: .jpg  \n";
	std::cin >> imgStoreDirName;
	initRemainingParams();
}
void ApparatusParamsCls::setFixedParams(int imgNum, int boardW, int boardH, double dx, double dy, std::string imgStoreDirName, std::string imgType, std::string patternfileName) {
	this->imgNum = imgNum;
	this->boardW = boardW;
	this->boardH = boardH;
	this->dx = dx;
	this->dy = dy;
	this->imgStoreDirName = imgStoreDirName;
	this->imgType = imgType;
	this->patternfileName = patternfileName;
	initRemainingParams();
}

void ApparatusParamsCls::setIntrisincParams(double fx, double fy, double cx, double cy, double k1, double k2, double k3, double k4) {
	instrisincMatrix.at<double>(0, 0) = fx;
	instrisincMatrix.at<double>(1, 1) = fy;
	instrisincMatrix.at<double>(0, 2) = cx;
	instrisincMatrix.at<double>(1, 2) = cy;
	distortionCoeff.at<double>(0, 0) = k1;
	distortionCoeff.at<double>(1, 0) = k2;
	distortionCoeff.at<double>(2, 0) = k3;
	distortionCoeff.at<double>(3, 0) = k4;
}




void ApparatusParamsCls::initRemainingParams() {
	boardN = boardH * boardW;
	std::string firstImgName = imgStoreDirName + "1" + imgType;
	imgSize = (cv::imread((firstImgName), 0)).size();//read the image size
	checkerBoardSize = cv::Size(boardW, boardH);
	isUsedArray = new bool[imgNum];
	for (int i = 0; i < imgNum; ++i)
		isUsedArray[i] = false;
	instrisincMatrix = cv::Mat::eye(3, 3, CV_64F);
	distortionCoeff = cv::Mat::zeros(4, 1, CV_64F);
}
ApparatusParamsCls::~ApparatusParamsCls(void)
{
	instrisincMatrix.release();
	distortionCoeff.release();
	std::vector<std::vector<cv::Point3f>>().swap(objectPoint);
	std::vector<std::vector<cv::Point2f>>().swap(imagePoint);
	std::vector<cv::Mat>().swap(rvecs);
	std::vector<cv::Mat>().swap(tvecs);
	//objectPoint.swap(std::vector<std::vector<cv::Point3f>>());
	//imagePoint.swap(std::vector<std::vector<cv::Point2f>>());
	//rvecs.swap(std::vector<cv::Mat>());
	//tvecs.swap(std::vector<cv::Mat>());
}
