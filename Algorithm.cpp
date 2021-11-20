#include "Algorithm.h"

void Algorithm::readParams(camera& leftCamera, camera& rightCamera, string& filePath) 
{
	std::fstream fileHandle;
	fileHandle.open(filePath.c_str(), ios_base::in);
	if (!fileHandle.is_open()) {
		cout << "can not open this file" << endl;
		return;
	}

	float  params[14];
	for (int i = 0; i < 14; i++) {
		fileHandle >> params[i];
	}

	cv::Mat Rl = (cv::Mat_<float>(3, 3) << params[0], params[1], params[2], params[3], params[4], params[5],
		params[6], params[7], params[8]);
	cv::Mat Dl = (cv::Mat_<float>(1, 5) << params[9], params[10], params[11], params[12], params[13]);
	leftCamera.setIntrisincParams(Rl);
	leftCamera.setDistortionCoeff(Dl);

	for (int i = 0; i < 14; i++) {
		fileHandle >> params[i];
	}
	cv::Mat Rr = (cv::Mat_<float>(3, 3) << params[0], params[1], params[2], params[3], params[4], params[5],
		params[6], params[7], params[8]);
	cv::Mat Dr = (cv::Mat_<float>(1, 5) << params[9], params[10], params[11], params[12], params[13]);
	rightCamera.setIntrisincParams(Rr);
	rightCamera.setDistortionCoeff(Dr);

	float  params1[12];
	for (int i = 0; i < 12; i++) {
		fileHandle >> params1[i];
	}
	cv::Mat T = (cv::Mat_<float>(3, 4) << params1[0], params1[1], params1[2], params1[9], params1[3],
		params1[4], params1[5], params1[10], params1[6], params1[7], params1[8], params1[11]);
	rightCamera.setExtrinsicsMatrix(T);
}


vector<vector<cv::Mat>> Algorithm::splitImages(vector<cv::Mat>& images) {
	vector<vector<cv::Mat>> splitResult;
	const int maskImageNum = 2;
	const int grayImageNum = 6;
	const int sinImageNum = 4;
	vector<cv::Mat> maskImage, grayImage, sinImage;
	// The PIcture projection order is white/black image, gray image, sin image
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
		numerator += sinImages[i] * sin(i*2* PI / sinImageNum);
		denominator += sinImages[i] * cos(i * 2 * PI / sinImageNum);
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
	wrappedPhase += PI;
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
	cv::Mat unwrappedPhase = wrappedPhase + 2 * PI * codeOrder;
#ifdef DEBUG
	cv::imwrite("..//structured-light-data//result//unwrappedPhase.bmp", unwrappedPhase);
#endif // DEBUG
	return unwrappedPhase;
}

cv::Mat Algorithm::multiHeterodyne(std::vector<cv::Mat>& images, double f1, double f2, double f3, int n, cv::Mat& mask) {
	//f1 f2  f3 为 条纹频率
	int m = images.size();
	if (m % n != 0) {
		std::cout << "输出出错" << std::endl;
		cv::Mat res;
		return res;
	}
	int num = m / n;

	std::vector<cv::Mat> wrappingPhase;
	for (int i = 0; i < num; i++) {
		std::vector<cv::Mat> temp;
		for (int j = 0; j < n; j++) {
			temp.push_back(images[i * n + j]);
		}
		wrappingPhase.push_back(unwrappingPhase(temp));
		std::vector<cv::Mat>().swap(temp);
	}

	cv::Mat PH1 = wrappingPhase[0].clone();
	cv::Mat PH2 = wrappingPhase[1].clone();
	cv::Mat PH3 = wrappingPhase[2].clone();


    cv::Mat PH12 = cv::Mat::zeros(PH1.rows, PH1.cols, CV_32FC1);
	PH12.forEach<float>([&PH1, &PH2](float& val, const int* pos) {
		const float temp = PH2.ptr<float>(pos[0])[pos[1]] - PH1.ptr<float>(pos[0])[pos[1]];
		if (temp >= 0) val = temp;
		else val = 2 * PI + temp;
		});

	

    cv::Mat PH23 = cv::Mat::zeros(PH2.rows, PH2.cols, CV_32FC1);
	PH23.forEach<float>([&PH3, &PH2](float& val, const int* pos) {
		const float temp = PH3.ptr<float>(pos[0])[pos[1]] - PH2.ptr<float>(pos[0])[pos[1]];
		if (temp >= 0) val = temp;
		else val = 2 * PI + temp;
		});

    cv::Mat PH123 = cv::Mat::zeros(PH1.rows, PH1.cols, CV_32FC1);
	PH123.forEach<float>([&PH12, &PH23](float& val, const int* pos) {
		const float temp = PH23.ptr<float>(pos[0])[pos[1]] - PH12.ptr<float>(pos[0])[pos[1]];
		if (temp >= 0) val = temp;
		else val = 2 * PI + temp;
		});
	

	//频率差
	double f12 = f2 - f1;
	double f23 = f3 - f2;
	double f123 = f23 - f12;

	//解包裹23：求PH23的绝对相位
	double R = f23 / f123;
    cv::Mat Nwrap = cv::Mat::zeros(PH123.rows, PH123.cols, CV_32FC1);
	Nwrap.forEach<float>([&PH123, &PH23, &R](float& val, const int* pos) {
		val = floor(0.5 + (R * PH123.ptr<float>(pos[0])[pos[1]] - PH23.ptr<float>(pos[0])[pos[1]]) / (2 * PI));
		});

	cv::Mat PH23UnWrap = 2 * PI * Nwrap + PH23;

	R = f2 / f23;
    cv::Mat Nwrap1 = cv::Mat::zeros(PH23UnWrap.rows, PH23UnWrap.cols, CV_32FC1);
	Nwrap1.forEach<float>([&PH23UnWrap, &PH2, &R](float& val, const int* pos) {
		val = floor(0.5 + (R * PH23UnWrap.ptr<float>(pos[0])[pos[1]] - PH2.ptr<float>(pos[0])[pos[1]]) / (2 * PI));
		});
	cv::Mat PH2UnWrap = 2 * PI * Nwrap1 + PH2;

	//cv::multiply(PH2UnWrap, mask, PH2UnWrap);
	return PH2UnWrap;
}

cv::Mat Algorithm::unwrappingPhase(std::vector<cv::Mat>& images) {
	int n = images.size();

    cv::Mat imgUp = cv::Mat::zeros(images[0].rows, images[0].cols, CV_32FC1);
    cv::Mat imgDown = cv::Mat::zeros(images[0].rows, images[0].cols, CV_32FC1);
	for (int i = 0; i < images.size(); i++) {
		imgUp = imgUp + sin(2 * PI * i / n) * images[i];
		imgDown = imgDown + cos(2 * PI * i / n) * images[i];
	}
    cv::Mat res = cv::Mat::zeros(imgUp.rows, imgUp.cols, CV_32FC1);
	res.forEach<float>([&imgUp, &imgDown](float& val, const int* pos) {
		const float upPixel = imgUp.ptr<float>(pos[0])[pos[1]];
		const float downPixel = imgDown.ptr<float>(pos[0])[pos[1]] + EPS;
		val = -atan2(upPixel, downPixel);
		});

	
	return res;
}

cv::Mat Algorithm::unsortTriangulate(cv::Mat& mask, cv::Mat& unwrapImg, cv::Mat& proImg, cv::Mat& R, cv::Mat& t, cv::Mat& k1, cv::Mat& k2, cv::Mat& d1, int freq) {
	cv::undistort(unwrapImg, unwrapImg, k1, d1);

	cv::Mat T = cv::Mat::zeros(3, 3, CV_32F);
	T.ptr<float>(0)[1] = -t.ptr<float>(0)[2];
	T.ptr<float>(0)[2] = t.ptr<float>(0)[1];
	T.ptr<float>(1)[0] = t.ptr<float>(0)[2];

	T.ptr<float>(1)[2] = -t.ptr<float>(0)[0];
	T.ptr<float>(2)[0] = -t.ptr<float>(0)[1];
	T.ptr<float>(2)[1] = t.ptr<float>(0)[0];
	cv::transpose(k1, k1);
	cv::transpose(R, R);
	cv::Mat E = k1 * R * T * k2;

	//投影仪的宽
	int width = 1140;

	std::vector<cv::Point2f> p1;
	std::vector<cv::Point2f> p2;

	unwrapImg.forEach<float>([&mask, &p1, &p2, &E, &width, &freq](float& val, const int* pos) {
		if (mask.ptr<uchar>(pos[0])[pos[1]]) {
			cv::Mat points = (cv::Mat_<float>(1, 3) << pos[0], pos[1], 1);
			cv::Mat coef = points * E;
			const float a = coef.ptr<float>(0)[0];
			const float b = coef.ptr<float>(0)[1];
			const float c = coef.ptr<float>(0)[2];

			//计算坐标
			float proY = val * width / (2 * PI * freq);
			float proX = ((-b * proY) - c) / a;
			cv::Point2f tmp1;
			cv::Point2f tmp2;
			tmp1.x = pos[0]; tmp1.y = pos[1];
			tmp2.x = proX; tmp2.y = proY;

			p1.push_back(tmp1);
			p2.push_back(tmp2);
		}
		}
	);

	cv::Mat Tw_ = (cv::Mat_<float>(3, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);
	cv::Mat Tp_ = cv::Mat::zeros(3, 4, CV_32F);
	Tp_.forEach<float>([&R, &t](float& val, const int* pos) {
		if (pos[1] == 3) val = t.ptr<float>(pos[0])[0];
		else val = R.ptr<float>(pos[0])[pos[1]];
		});
	cv::Mat res;
	cv::triangulatePoints(Tw_, Tp_, p1, p2, res);
	return res;
}
