#include "calibration.h"

calibration::calibration(int cal) :calibType(cal) {
	int typeMapArr[10] = { 1,3,12,14,0 };
	FileCls* fileCls = new FileCls(static_cast<CalibrationType>(typeMapArr[calibType]));
	fileCls->readDefaultData();
	CalibrationCls* calibrationCls = new CalibrationCls(fileCls);
	std::cout << "\n\n\nAll work has done!\n\n" << std::endl;
}