/*!
* \file CalibrationCls.cpp
* \date 2017/05/26 17:49
*
* \author zhouyelihua
* Contact: yeahpingye@gmail.com
*  update version: 2020/07/17    denggaoxu
* \brief
*
* TODO:
*
* \note
*/
#include "CalibrationCls.h"
#include <iostream>


using namespace std;
using namespace cv;



CalibrationCls::CalibrationCls(void)
{

}
CalibrationCls::CalibrationCls(FileCls* fileCls) :fileCls(fileCls) {
	R = cv::Mat::zeros(3, 3, CV_64F);
	T = cv::Mat::zeros(3, 1, CV_64F);
	switch (fileCls->calibType) {
	case ONLYCAM: {
		monocularCalibration();   //1-单相机标定
	}
				break;
	case CAM_PRJ: {         //3-单结构光标定
		structureLightCalibration();
	}
				break;
	case CAM_CAM: {         //12-双目视觉标定
		stereoCalibration();
	}
				break;
	case CAM_PRJ_CAM: {     //14-相机-投影仪-相机标定
		camPrjcamCalibration();
	}
					break;

					//case CAM_CAM_EXTRISINC:{   //0
					//	calExtrinsicParams(fileCls->camInitParams,fileCls->eosInitparams);
					//					   }
					//					   break;
	}

}
void CalibrationCls::camPrjcamCalibration() {        //有问题
	structureLightCalibration_new();   //结构光标定
	findChessBoardCorner(fileCls->rightcamInitparams);
	calibrateOneCamera(fileCls->rightcamInitparams);
	stereoExtrinsicParamsCalibration(fileCls->leftcamInitparams, fileCls->rightcamInitparams);

}
void CalibrationCls::monocularCalibration() {       //单相机标定
	findChessBoardCorner(fileCls->camInitParams);  //寻找标定板角点
	calibrateOneCamera(fileCls->camInitParams);    //标定一个相机
	//写入到本地txt文件
	fileCls->writeInstrinsicParams(fileCls->camInitParams->instrisincMatrix, fileCls->camInitParams->distortionCoeff, "../../structured-light code/monocularParams.txt", std::ofstream::out);
	for (int i = 0; i < fileCls->camInitParams->successNumber; ++i)
		fileCls->writeRT((fileCls->camInitParams->rvecs)[i], (fileCls->camInitParams->tvecs)[i], "../../structured-light code/monocularParams.txt", std::ofstream::app);
}
void CalibrationCls::stereoCalibration() {     //双目立体视觉标定
	findChessBoardCorner(fileCls->leftcamInitparams);
	calibrateOneCamera(fileCls->leftcamInitparams);
	findChessBoardCorner(fileCls->rightcamInitparams);
	calibrateOneCamera(fileCls->rightcamInitparams);
	stereoExtrinsicParamsCalibration(fileCls->leftcamInitparams, fileCls->rightcamInitparams);//立体视觉外参标定函数
}

void CalibrationCls::calExtrinsicParams(ApparatusParamsCls* camParaCls1, ApparatusParamsCls* camParaCls2) {
	findChessBoardCorner(camParaCls1);
	findChessBoardCorner(camParaCls2);
	calRT(camParaCls1, camParaCls2);
	fileCls->writeRT(R, T, "../../structured-light code/camEosIntrisicParam.txt", std::ofstream::app);
}

void CalibrationCls::stereoExtrinsicParamsCalibration(ApparatusParamsCls* camParaCls1, ApparatusParamsCls* camParaCls2) {
	fileCls->writeInstrinsicParams(camParaCls1->instrisincMatrix, camParaCls1->distortionCoeff, "../../structured-light code/stereoParams.txt", std::ofstream::out);
	fileCls->writeInstrinsicParams(camParaCls2->instrisincMatrix, camParaCls2->distortionCoeff, "../../structured-light code/stereoParams.txt", std::ofstream::app);
	calRT(camParaCls1, camParaCls2);
	fileCls->writeRT(R, T, "../../structured-light code/stereoParams.txt", std::ofstream::app);
}

void CalibrationCls::structureLightCalibration() {   //单结构光标定
	findChessBoardCorner(fileCls->camInitParams);   //相机标定参数初始化
	calibrateOneCamera(fileCls->camInitParams);     //标定相机

	preProccessPrjImg(fileCls->camInitParams, fileCls->prjInitparams);
	findChessBoardCorner(fileCls->prjInitparams);   //投影仪标定参数初始化
	calProjectorObjectCoords(fileCls->camInitParams, fileCls->prjInitparams);
	findProjectorImgCoords(fileCls->prjInitparams);
	calibrateOneCamera(fileCls->prjInitparams);
	fileCls->writeInstrinsicParams(fileCls->camInitParams->instrisincMatrix, fileCls->camInitParams->distortionCoeff, "../../structured-light code/SLSParam.txt", std::ofstream::out);
	fileCls->writeInstrinsicParams(fileCls->prjInitparams->instrisincMatrix, fileCls->prjInitparams->distortionCoeff, "../../structured-light code/SLSParam.txt", std::ofstream::app);
	calSLSRT(fileCls->camInitParams, fileCls->prjInitparams);    //计算结构光的R,T
	fileCls->writeRT(R, T, "../../structured-light code/SLSParam.txt", std::ofstream::app);
	fileCls->writeParamsForLib("../../structured-light code/SLSParamForLib.txt", fileCls->camInitParams->instrisincMatrix, fileCls->camInitParams->distortionCoeff,
		fileCls->prjInitparams->instrisincMatrix, fileCls->prjInitparams->distortionCoeff, R, T);

}

void CalibrationCls::structureLightCalibration_new() {   //单结构光标定
	findChessBoardCorner(fileCls->leftcamInitparams);   //相机标定参数初始化
	calibrateOneCamera(fileCls->leftcamInitparams);     //标定相机

	preProccessPrjImg(fileCls->leftcamInitparams, fileCls->prjInitparams);
	findChessBoardCorner(fileCls->prjInitparams);   //投影仪标定参数初始化
	calProjectorObjectCoords(fileCls->leftcamInitparams, fileCls->prjInitparams);
	findProjectorImgCoords(fileCls->prjInitparams);
	calibrateOneCamera(fileCls->prjInitparams);
	fileCls->writeInstrinsicParams(fileCls->leftcamInitparams->instrisincMatrix, fileCls->leftcamInitparams->distortionCoeff, "../../structured-light code/SLSParam.txt", std::ofstream::out);
	fileCls->writeInstrinsicParams(fileCls->prjInitparams->instrisincMatrix, fileCls->prjInitparams->distortionCoeff, "../../structured-light code/SLSParam.txt", std::ofstream::app);
	calSLSRT(fileCls->leftcamInitparams, fileCls->prjInitparams);    //计算结构光的R,T
	fileCls->writeRT(R, T, "../../structured-light code/SLSParam.txt", std::ofstream::app);
	fileCls->writeParamsForLib("../../structured-light code/SLSParamForLib.txt", fileCls->leftcamInitparams->instrisincMatrix, fileCls->leftcamInitparams->distortionCoeff,
		fileCls->prjInitparams->instrisincMatrix, fileCls->prjInitparams->distortionCoeff, R, T);

}

void CalibrationCls::calRT(ApparatusParamsCls* camParaCls1, ApparatusParamsCls* camParaCls2) {//计算R,T



	int curIdx1 = 0, curIdx2 = 0;
	//cv::stereoCalibrate(,)
	//////////////////////////////////////////////////////////////////////////
	std::vector<std::vector<cv::Point3f> > objectPoint;
	std::vector<std::vector<cv::Point2f> > imagePoint1;
	std::vector<std::vector<cv::Point2f> > imagePoint2;
	//////////////////////////////////////////////////////////////////////////


	for (int i = 0; i < std::min(camParaCls1->imgNum, camParaCls2->imgNum); ++i)
	{

		if ((camParaCls1->isUsedArray)[i] && (camParaCls2->isUsedArray)[i]) {
			//////////////////////////////////////////////////////////////////////////
			objectPoint.push_back((camParaCls1->objectPoint)[curIdx1]);
			imagePoint1.push_back((camParaCls1->imagePoint)[curIdx1]);
			imagePoint2.push_back((camParaCls2->imagePoint)[curIdx2]);
			////////////////////////////////////////////////////////////////////////////
			////// extracting the R&T manually. 
			cv::Mat R1 = (camParaCls1->rvecs)[curIdx1];
			cv::Mat R2 = (camParaCls2->rvecs)[curIdx2];
			cv::Mat T1 = (camParaCls1->tvecs)[curIdx1];
			cv::Mat T2 = (camParaCls2->tvecs)[curIdx2];
			cv::Mat matrixR1 = cv::Mat::zeros(3, 3, CV_64F);
			cv::Mat InvMatrixR1 = cv::Mat::zeros(3, 3, CV_64F);
			cv::Mat matrixR2 = cv::Mat::zeros(3, 3, CV_64F);
			cv::Rodrigues(R1, matrixR1);
			cv::Rodrigues(R2, matrixR2);
			InvMatrixR1 = matrixR1.inv();
			cv::gemm(matrixR2, InvMatrixR1, 1, NULL, 0, R, 0);
			cv::gemm(R, T1, -1, T2, 1, T, 0);
			fileCls->writeRT(R, T, "../../structured-light code/RTCheck.txt", std::ofstream::app);
		}
		((camParaCls1->isUsedArray)[i]) ? (++curIdx1) : 0;
		((camParaCls2->isUsedArray)[i]) ? (++curIdx2) : 0;

	}
	//////////////////////////////////////////////////////////////////////////
	cv::Mat E, F;
	cv::stereoCalibrate(objectPoint,
		imagePoint1,
		imagePoint2,
		camParaCls1->instrisincMatrix,
		camParaCls1->distortionCoeff,
		camParaCls2->instrisincMatrix,
		camParaCls2->distortionCoeff,
		camParaCls2->imgSize,
		R,
		T,
		E,
		F,
		CALIB_FIX_INTRINSIC + CALIB_USE_INTRINSIC_GUESS,
		cv::TermCriteria(0, 50, 1e-6)
	);

	//////////////////////////////////////////////////////////////////////////

}
void CalibrationCls::calSLSRT(ApparatusParamsCls* camParaCls1, ApparatusParamsCls* camParaCls2) {



	int curIdx1 = 0, curIdx2 = 0;
	//cv::stereoCalibrate(,)
	////////////////////////////////////////////////////////////////////////////
	//std::vector<std::vector<cv::Point3f> > objectPoint;
	//std::vector<std::vector<cv::Point2f> > imagePoint1;
	//std::vector<std::vector<cv::Point2f> > imagePoint2;
	//////////////////////////////////////////////////////////////////////////




	for (int i = 0; i < std::min(camParaCls1->imgNum, camParaCls2->imgNum); ++i)
	{

		if ((camParaCls1->isUsedArray)[i] && (camParaCls2->isUsedArray)[i]) {
			//////////////////////////////////////////////////////////////////////////
			/*		objectPoint.push_back((camParaCls1->objectPoint)[curIdx1]);
			imagePoint1.push_back((camParaCls1->imagePoint)[curIdx1]);
			imagePoint2.push_back((camParaCls2->imagePoint)[curIdx2]);*/
			////////////////////////////////////////////////////////////////////////////
			////// extracting the R&T manually. 
			cv::Mat R1 = (camParaCls1->rvecs)[curIdx1];
			cv::Mat R2 = (camParaCls2->rvecs)[curIdx2];
			cv::Mat T1 = (camParaCls1->tvecs)[curIdx1];
			cv::Mat T2 = (camParaCls2->tvecs)[curIdx2];
			cv::Mat matrixR1 = cv::Mat::zeros(3, 3, CV_64F);
			cv::Mat InvMatrixR1 = cv::Mat::zeros(3, 3, CV_64F);
			cv::Mat matrixR2 = cv::Mat::zeros(3, 3, CV_64F);
			cv::Rodrigues(R1, matrixR1);
			cv::Rodrigues(R2, matrixR2);
			InvMatrixR1 = matrixR1.inv();
			cv::gemm(matrixR2, InvMatrixR1, 1, NULL, 0, R, 0);
			cv::gemm(R, T1, -1, T2, 1, T, 0);
			break;
			//fileCls->writeRT(R,T,"../RTCheck.txt",std::ofstream::app);
		}
		((camParaCls1->isUsedArray)[i]) ? (++curIdx1) : 0;
		((camParaCls2->isUsedArray)[i]) ? (++curIdx2) : 0;

	}
	//////////////////////////////////////////////////////////////////////////
	/*cv::Mat E,F;
	cv::stereoCalibrate(objectPoint,
		imagePoint1,
		imagePoint2,
		camParaCls1->instrisincMatrix,
		camParaCls1->distortionCoeff,
		camParaCls2->instrisincMatrix,
		camParaCls2->distortionCoeff,
		camParaCls2->imgSize,
		R,
		T,
		E,
		F,
		cv::TermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 50, 1e-6),
		CV_CALIB_FIX_INTRINSIC +
		CV_CALIB_USE_INTRINSIC_GUESS);

*/

//////////////////////////////////////////////////////////////////////////

}
void CalibrationCls::findChessBoardCorner(ApparatusParamsCls* camParaCls) {
	std::vector<cv::Point2f> pointBuf;
	camParaCls->successNumber = 0;
	int failureNumber = 0;
	std::cout << "\n\n\nthe situation of corner detection\n";
	for (int idx = 1; idx <= camParaCls->imgNum; ++idx)
	{
		std::string curImgName = camParaCls->imgStoreDirName + std::to_string(static_cast<__int64>(idx)) + camParaCls->imgType;
		cv::Mat camImg = cv::imread(curImgName, 0);
		//extract image corner
		int camFound = cv::findChessboardCorners(camImg, camParaCls->checkerBoardSize, pointBuf, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FILTER_QUADS);//the  input arguments  is important
		if (camFound) {
			cv::cornerSubPix(camImg, pointBuf, cv::Size(11, 11),
				cv::Size(-1, -1), cv::TermCriteria(0, 30, 0.1));
			cv::drawChessboardCorners(camImg, camParaCls->checkerBoardSize, pointBuf, camFound);
			cv::namedWindow("window", WINDOW_NORMAL);
			cv::imshow("window", camImg);
			cv::waitKey(500);
			std::vector<cv::Point3f> objPointVec;
			cv::Point3f    objPoint;
			for (int j = 0; j < camParaCls->boardN; ++j) {
				objPoint.x = (j / (camParaCls->boardW)) * camParaCls->dx;
				objPoint.y = (j % (camParaCls->boardW)) * camParaCls->dy;
				objPoint.z = 0.0f;
				objPointVec.push_back(objPoint);
			}
			camParaCls->imagePoint.push_back(pointBuf);
			camParaCls->objectPoint.push_back(objPointVec);
			(camParaCls->isUsedArray)[idx - 1] = true;
			++(camParaCls->successNumber);



			std::cout << "successNumber=" << camParaCls->successNumber << "\t\t\t\tcurIdx=" << idx << std::endl;
		}
		else {
			std::cout << "Fail!!\t\t" << ++failureNumber << "\t\tcurIdx=" << idx << std::endl;
		}
	}
	std::vector<cv::Point2f>().swap(pointBuf);
	//pointBuf.swap(std::vector<cv::Point2f>());

}

void CalibrationCls::calibrateOneCamera(ApparatusParamsCls* camParaCls) {//标定单相机

	std::cout << "\n\n\ncalibrating....\npls wait a moment!\n...\n\n...";
	if (camParaCls->successNumber < 2)
		exit(0);

	//std::cout<<"\n\n"<<camParaCls->instrisincMatrix<<camParaCls->distortionCoeff;
	cv::calibrateCamera(camParaCls->objectPoint,
		camParaCls->imagePoint,
		camParaCls->imgSize,
		camParaCls->instrisincMatrix,
		camParaCls->distortionCoeff,
		camParaCls->rvecs,
		camParaCls->tvecs, 0 | CALIB_FIX_K4 | CALIB_FIX_K5);
	std::cout << "\n\nAll the images' corners found status: \n\n";
	for (int i = 0; i < camParaCls->imgNum; ++i)
		std::cout << camParaCls->isUsedArray[i] << "\t";
	std::cout << "\n\n";


	//计算重投影误差
	double total_err = 0.0;                   /* 所有图像的平均误差的总和 */
	double err = 0.0;                        /* 每幅图像的平均误差 */
	vector<Point2f>  image_points2;             /****   保存重新计算得到的投影点    ****/
	for (int i = 0; i < camParaCls->successNumber; i++)
	{

		vector<Point3f> tempPointSet = camParaCls->objectPoint[i];
		/****    通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点     ****/
		projectPoints(tempPointSet, camParaCls->rvecs[i], camParaCls->tvecs[i], camParaCls->instrisincMatrix, camParaCls->distortionCoeff, image_points2);
		/* 计算新的投影点和旧的投影点之间的误差*/
		vector<Point2f> tempImagePoint = camParaCls->imagePoint[i];
		Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
		Mat image_points2Mat = Mat(1, image_points2.size(), CV_32FC2);
		for (int j = 0; j < (int)tempImagePoint.size(); j++)
		{
			image_points2Mat.at<Vec2f>(0, j) = Vec2f(image_points2[j].x, image_points2[j].y);
			tempImagePointMat.at<Vec2f>(0, j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
		}
		err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
		/*vector<int>  point_counts;
		point_counts.push_back(camParaCls->imagePoint[i].size());*/
		total_err += err /= camParaCls->imagePoint[i].size();
	}

	double mean_err;
	mean_err = total_err / camParaCls->successNumber;
	std::cout << "总体平均误差：" << mean_err << "像素" << endl;
	ofstream fout("../../structured-light code/calib_error.txt", ios::app);
	fout << "总体平均误差：" << mean_err << '\n' << endl << endl;


}
void CalibrationCls::preProccessPrjImg(ApparatusParamsCls* camInitParams, ApparatusParamsCls* prjInitParams) {//投影仪图片预处理
	std::string pprjString = prjInitParams->imgStoreDirName + "Processed";
	for (int idx = 1; idx <= std::min(camInitParams->imgNum, prjInitParams->imgNum); ++idx) {
		std::string curCamImgName = camInitParams->imgStoreDirName + std::to_string(static_cast<__int64>(idx)) + camInitParams->imgType;
		std::string curPrjImgName = prjInitParams->imgStoreDirName + std::to_string(static_cast<__int64>(idx)) + prjInitParams->imgType;
		std::string curPPrjImgName = pprjString + std::to_string(static_cast<__int64>(idx)) + prjInitParams->imgType;
		double minVal, maxVal;
		cv::Mat camImg = cv::imread(curCamImgName, 0);
		cv::Mat prjImg = cv::imread(curPrjImgName, 0);
		cv::subtract(camImg, prjImg, prjImg);
		cv::minMaxLoc(prjImg, &minVal, &maxVal);
		cv::convertScaleAbs(prjImg, prjImg, -255.0 / (maxVal - minVal), 255 + (255.0 + minVal) / (maxVal - minVal));
		//cv::threshold(prjImg, prjImg, 230, 255, cv::THRESH_BINARY);
		cv::imwrite(curPPrjImgName, prjImg);
	}
	prjInitParams->imgStoreDirName = pprjString;
}
void CalibrationCls::calProjectorObjectCoords(ApparatusParamsCls* camParaCls, ApparatusParamsCls* prjParaCls) {
	std::vector<cv::Point2f> camImgUndistortPointVec;
	std::vector<cv::Point2f> prjImgUndistortPointVec;
	std::vector<cv::Point2f> prjObjectPointVec;
	std::vector<std::vector<cv::Point3f> > objectPoint;
	cv::Mat camHomoSrc = cv::Mat::ones(camParaCls->boardN, 2, CV_64F);
	cv::Mat camHomoDst = cv::Mat::ones(camParaCls->boardN, 2, CV_64F);
	int curIdx1 = 0, curIdx2 = 0;
	int successNumber = 0;

	for (int idx = 0; idx < std::min(camParaCls->imgNum, prjParaCls->imgNum); ++idx)
	{
		//////////////////////////////////////////////////////////////////////////
		std::vector<cv::Point3f> prjObjectPoints;
		if ((camParaCls->isUsedArray)[idx] && (prjParaCls->isUsedArray)[idx]) {
			cv::undistortPoints((camParaCls->imagePoint)[curIdx1], camImgUndistortPointVec, camParaCls->instrisincMatrix, camParaCls->distortionCoeff);
			cv::undistortPoints((prjParaCls->imagePoint)[curIdx2], prjImgUndistortPointVec, camParaCls->instrisincMatrix, camParaCls->distortionCoeff);
			for (int i = 0; i < camParaCls->boardN; ++i)
			{
				camHomoSrc.at<double>(i, 0) = camImgUndistortPointVec[i].x;
				camHomoSrc.at<double>(i, 1) = camImgUndistortPointVec[i].y;
				camHomoDst.at<double>(i, 0) = ((camParaCls->objectPoint)[curIdx1][i]).x;
				camHomoDst.at<double>(i, 1) = ((camParaCls->objectPoint)[curIdx1][i]).y;
			}
			cv::Mat H = cv::findHomography(camHomoSrc, camHomoDst);
			cv::perspectiveTransform(prjImgUndistortPointVec, prjObjectPointVec, H);
			successNumber++;

			//store the prj objects into prjCls

			for (int i = 0; i < prjParaCls->boardN; ++i)
				prjObjectPoints.push_back(cv::Point3f(prjObjectPointVec[i].x, prjObjectPointVec[i].y, 0.0f));
			(objectPoint).push_back(prjObjectPoints);
		}
		((camParaCls->isUsedArray)[idx]) ? (++curIdx1) : 0;
		((prjParaCls->isUsedArray)[idx]) ? (++curIdx2) : 0;
	}
	prjParaCls->successNumber = successNumber;
	(prjParaCls->objectPoint).swap(objectPoint);
	//objectPoint.swap(std::vector<std::vector<cv::Point3f> >());
	std::vector<std::vector<cv::Point3f> >().swap(objectPoint);

}
void CalibrationCls::findProjectorImgCoords(ApparatusParamsCls* prjParaCls) {
	std::vector<cv::Point2f> pointBuf;
	std::vector<std::vector<cv::Point2f> > imagePoint;
	std::cout << "\n\n\nthe situation of pattern detection\n";

	cv::Mat prjImg = cv::imread(prjParaCls->patternfileName, 0);
	//extract image corner
	int prjFound = cv::findChessboardCorners(prjImg, prjParaCls->checkerBoardSize, pointBuf, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FILTER_QUADS);//the  input arguments  is important
	if (prjFound) {
		cv::cornerSubPix(prjImg, pointBuf, cv::Size(11, 11),
			cv::Size(-1, -1), cv::TermCriteria(0, 30, 0.1));
		cv::drawChessboardCorners(prjImg, prjParaCls->checkerBoardSize, pointBuf, prjFound);
		cv::namedWindow("window", WINDOW_NORMAL);
		cv::imshow("window", prjImg);
		cv::waitKey(500);
		for (int idx = 0; idx < prjParaCls->successNumber; idx++)
			imagePoint.push_back(pointBuf);

	}
	prjParaCls->imgSize = prjImg.size();
	prjParaCls->imagePoint.swap(imagePoint);
	std::vector<std::vector<cv::Point2f> >().swap(imagePoint);
	std::vector<cv::Point2f>().swap(pointBuf);
	//imagePoint.swap(std::vector<std::vector<cv::Point2f> >());
	//pointBuf.swap(std::vector<cv::Point2f>());

}
CalibrationCls::~CalibrationCls(void)
{
}
