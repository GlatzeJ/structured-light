
/*!
 * \file FileCls.h
 * \date 2017/05/20 15:58
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
#include "ApparatusParamsCls.h"
#include <fstream>
#include <iostream>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>

#include <iomanip>
#include <limits.h>

typedef enum calibrationType {
	ONLYCAM = 1,       //结构光-相机
	ONLYPRJ = 2,     //结构光-投影仪

	//ONLYEOS = 1,       //单相机

	CAM_PRJ = 3,       //结构光系统
	CAM_CAM = 12,       //双目
	CAM_PRJ_CAM = 14,   //双目+投影仪

	LEFT = 4,   //
	RIGHT = 8,   //

	//CAM_CAM_EXTRISINC=0,


} CalibrationType;
class FileCls
{
private:

public:
	FileCls(void);
	~FileCls(void);
	FileCls(CalibrationType calibType);
	void readIntrinsicParamsFromLocalTxt(std::string localTxtName);
	void readDataFromStdCin();
	void readDefaultData();
	void writeInstrinsicParams(cv::Mat instrisincMatrix, cv::Mat distortionCoeff, std::string fileName, std::ios_base::openmode mode);
	void writeRT(cv::Mat R, cv::Mat T, std::string fileName, std::ios_base::openmode mode);

	void writeParamsForLib(std::string fileName, cv::Mat camInstrisincMatrix, cv::Mat camDistortionCoeff, cv::Mat prjInstrisincMatrix, cv::Mat prjDistortionCoeff, cv::Mat R, cv::Mat T);


	//it is convenient to set all the variables' property  public
	//
	int img_num;
	CalibrationType calibType;
	ApparatusParamsCls* camInitParams;       //结构光-相机
	ApparatusParamsCls* prjInitparams;       //结构光-投影仪

	//ApparatusParamsCls*eosInitparams;       //单相机

	ApparatusParamsCls* leftcamInitparams;   //双目或双目+投影仪系统的左右相机
	ApparatusParamsCls* rightcamInitparams;
	int  eosRatio;
};

#pragma once
