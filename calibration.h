#pragma
#include <iostream>
#include <opencv2/opencv.hpp>
#include "CalibrationCls.h"

class calibration {
private:
	/*  ��Ŀ����궨�����룺          0\n"
		�ṹ��궨�����룺            1\n"
		˫Ŀ�궨�����룺              2\n"
		˫Ŀ+ͶӰ�Ǳ궨�����룺       3\n"
	*/
	int calibType;

public:
	calibration(int cal);
	~calibration();

};