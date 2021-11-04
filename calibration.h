#pragma
#include <iostream>
#include <opencv2/opencv.hpp>
#include "CalibrationCls.h"

class calibration {
private:
	/*  单目相机标定请输入：          0\n"
		结构光标定请输入：            1\n"
		双目标定请输入：              2\n"
		双目+投影仪标定请输入：       3\n"
	*/
	int calibType;

public:
	calibration(int cal);
	~calibration();

};