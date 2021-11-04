/*!
 * \file CalibrationCls.h
 * \date 2017/05/26 17:49
 *
 * \author zhouyelihua
 * Contact: yeahpingye@gmail.com
 * update version: 2020/07/17    denggaoxu
 *
 * \brief
 *
 * TODO:
 *
 * \note
*/
#pragma once
#include "FileCls.h"

class CalibrationCls
{
private:
	FileCls* fileCls;
public:
	cv::Mat R;
	cv::Mat T;

	void monocularCalibration();           //单目相机标定
	void structureLightCalibration();      //结构光标定

	void structureLightCalibration_new();  //双目+投影仪系统中的结构光标定
	void camPrjcamCalibration();           //双目+投影仪系统标定

	void stereoCalibration();               //双目视觉标定

	void calibrateOneCamera(ApparatusParamsCls* camParaCls);

	void calExtrinsicParams(ApparatusParamsCls* camParaCls1, ApparatusParamsCls* camParaCls2);

	void findChessBoardCorner(ApparatusParamsCls* camParaCls);
	void calRT(ApparatusParamsCls* camParaCls1, ApparatusParamsCls* camParaCls2);// similar to calExtinsicparams Func
	void calSLSRT(ApparatusParamsCls* camParaCls1, ApparatusParamsCls* camParaCls2);

	void stereoExtrinsicParamsCalibration(ApparatusParamsCls* camParaCls1, ApparatusParamsCls* camParaCls2);
	void preProccessPrjImg(ApparatusParamsCls* camInitParams, ApparatusParamsCls* prjInitParams);
	void calProjectorObjectCoords(ApparatusParamsCls* camParaCls, ApparatusParamsCls* prjParaCls);
	void findProjectorImgCoords(ApparatusParamsCls* prjParaCls);
	CalibrationCls(void);
	CalibrationCls(FileCls* fileCls);
	~CalibrationCls(void);

};

#pragma once
