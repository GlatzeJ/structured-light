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
    const int grayImageNum = 5;
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
            if(val < 0)
                val += 2*PI;
		}else {
			return;
		}
        });
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

cv::Mat Algorithm::multiHeterodyne(std::vector<cv::Mat>& images, double f1, double f2, double f3, int n) {
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
#ifdef DEBUG
    std::cout << "PH12 is ready" << std::endl;
#endif
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
          images[i].convertTo(images[i], CV_32FC1);
		imgUp = imgUp + sin(2 * PI * i / n) * images[i];
		imgDown = imgDown + cos(2 * PI * i / n) * images[i];
	}
    cv::Mat res = cv::Mat::zeros(imgUp.rows, imgUp.cols, CV_32FC1);
	res.forEach<float>([&imgUp, &imgDown](float& val, const int* pos) {
		const float upPixel = imgUp.ptr<float>(pos[0])[pos[1]];
		const float downPixel = imgDown.ptr<float>(pos[0])[pos[1]] + EPS;
        val = -atan2(upPixel, downPixel);
        if(val < 0) val +=2*PI;
		});

	return res;
}

cv::Mat Algorithm::normalize_pixel(cv::Mat& xc, cv::Mat& camerainstrisincMatrix, cv::Mat& distortionCoeff){
    // xc:2*N, camerainstrisincMatrix:3*3, distortionCoeff:1*5
    cv::Mat res;
    const float fx = camerainstrisincMatrix.ptr<float>(0)[0], fy = camerainstrisincMatrix.ptr<float>(1)[1];
    const float u0 = camerainstrisincMatrix.ptr<float>(0)[2], v0 = camerainstrisincMatrix.ptr<float>(1)[2];
    const float k1 = distortionCoeff.ptr<float>(0)[0], k2 = distortionCoeff.ptr<float>(0)[1],
                k3 = distortionCoeff.ptr<float>(0)[2], p1 = distortionCoeff.ptr<float>(0)[3],
                p2 = distortionCoeff.ptr<float>(0)[4];
    cv::Mat u = xc.colRange(0, 1).clone(), v = xc.colRange(1,2).clone();
    u = (u+1-u0)/fx, xc.colRange(0, 1) = u;
    v = (v+1-v0)/fy, xc.colRange(1, 2) = v;
    cv::Mat u2, v2;
    cv::pow(u, 2, u2), cv::pow(v, 2, v2);
    cv::Mat r2 = u2 + v2;
    cv::Mat r22, r23;
    cv::pow(r2, 2, r22), cv::pow(r2, 3, r23);
    cv::Mat kRadial = 1+k1*r2+k2*r22+k3*r23;
    cv::Mat delta_x(xc.rows, xc.cols, CV_32F);
    delta_x.colRange(0, 1) = 2*p1*(u.mul(v))+p2*(r2+2*u2);
    delta_x.colRange(1, 2) = p1*(r2+2*v2)+2*p2*(u.mul(v));
    cv::Mat resxUp = xc-delta_x;
    cv::Mat one = cv::Mat::ones(2,1,CV_32F);
    cv::transpose(kRadial, kRadial);
    cv::Mat resxDown = one*kRadial;
    cv::transpose(resxDown, resxDown);
    cv::divide(resxUp, resxDown, res);
    return res;
}

cv::Mat Algorithm::unsortTriangulate(cv::Mat& mask, cv::Mat& unwrapImg, cv::Mat& cameraInstrisincMatrix,
                                                   cv::Mat& projectorInstrisincMatrix, cv::Mat& extrinsicsMatrix,
                                                   cv::Mat& distortionCoeff, int freq) {
    int width = 912;
    /*
    unwrapImg.forEach<float>([&dp, &Ac, &Ap, &mask, &width, &freq](float& val, const int* pos){
        if(mask.ptr<uchar>(pos[0])[pos[1]])
        {
            float up = val * width / (2 * PI * freq);
            float uc = float(pos[0]-1);
            float vc = float(pos[1]-1);
            cv::Mat A = (cv::Mat_<float>(3,3) << Ac.ptr<float>(0)[0] - Ac.ptr<float>(2)[0] * uc, Ac.ptr<float>(0)[1] - Ac.ptr<float>(2)[1] * uc, Ac.ptr<float>(0)[2] - Ac.ptr<float>(2)[2] * uc,
                                                 Ac.ptr<float>(1)[0] - Ac.ptr<float>(2)[0] * vc, Ac.ptr<float>(1)[1] - Ac.ptr<float>(2)[1] * vc, Ac.ptr<float>(1)[2] - Ac.ptr<float>(2)[2] * vc,
                                                 Ap.ptr<float>(0)[0] - Ap.ptr<float>(2)[0] * up, Ap.ptr<float>(0)[1] - Ap.ptr<float>(2)[1] * up, Ap.ptr<float>(0)[2] - Ap.ptr<float>(2)[2] * up);
            cv::Mat b = (cv::Mat_<float>(3, 1) << Ac.ptr<float>(2)[3] * uc - Ac.ptr<float>(0)[3],
                                                  Ac.ptr<float>(2)[3] * vc - Ac.ptr<float>(1)[3],
                                                  Ap.ptr<float>(2)[3] * up - Ap.ptr<float>(0)[3]);
            cv::Mat res = A.inv() * b;
            //std::cout << res.size() << std::endl;
            vector<float>tmp(3,0);
            tmp[0] = res.ptr<float>(0)[0];
            tmp[1] = res.ptr<float>(1)[0];
            tmp[2] = res.ptr<float>(2)[0];
            dp.push_back(tmp);
        }
    });
    */
    /*for(int i = 0; i < unwrapImg.rows; i++)
    {
        for( int j = 0 ; j < unwrapImg.cols; j++)
        {
            if(mask.ptr<uchar>(i)[j])
            {
                if(unwrapImg.ptr<float>(i)[j] < 130) continue;
                float up = unwrapImg.ptr<float>(i)[j] * width / (2 * PI * freq);
                float uc = float(i);
                float vc = float(j);
                cv::Mat A = (cv::Mat_<float>(3,3) << Ac.ptr<float>(0)[0] - Ac.ptr<float>(2)[0] * uc, Ac.ptr<float>(0)[1] - Ac.ptr<float>(2)[1] * uc, Ac.ptr<float>(0)[2] - Ac.ptr<float>(2)[2] * uc,
                                                     Ac.ptr<float>(1)[0] - Ac.ptr<float>(2)[0] * vc, Ac.ptr<float>(1)[1] - Ac.ptr<float>(2)[1] * vc, Ac.ptr<float>(1)[2] - Ac.ptr<float>(2)[2] * vc,
                                                     Ap.ptr<float>(0)[0] - Ap.ptr<float>(2)[0] * up, Ap.ptr<float>(0)[1] - Ap.ptr<float>(2)[1] * up, Ap.ptr<float>(0)[2] - Ap.ptr<float>(2)[2] * up);
                cv::Mat b = (cv::Mat_<float>(3, 1) << Ac.ptr<float>(2)[3] * uc - Ac.ptr<float>(0)[3],
                                                      Ac.ptr<float>(2)[3] * vc - Ac.ptr<float>(1)[3],
                                                      Ap.ptr<float>(2)[3] * up - Ap.ptr<float>(0)[3]);
                cv::Mat res = A.inv() * b;
                //std::cout << res.size() << std::endl;
                vector<float>tmp(3,0);
                tmp[0] = res.ptr<float>(0)[0];
                tmp[1] = res.ptr<float>(1)[0];
                tmp[2] = res.ptr<float>(2)[0];
                dp.push_back(tmp);
            }
        }
    }*/
    /*
    cv::Mat unsortImg;
    cv::undistort(unwrapImg, unsortImg, k1, d1);

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

    std::cout << E << std::endl;

	//投影仪的宽

    cv::Mat2i underLine;
    cv::findNonZero(mask, underLine);
    cv::Mat p1 = cv::Mat::zeros(2, underLine.rows, CV_32FC1);
    cv::Mat p2 = cv::Mat::zeros(2, underLine.rows, CV_32FC1);

    std::cout << "E" << std::endl;
    std::cout << underLine.rows << "  " << underLine.cols << std::endl;
    //std::vector<cv::Point2f> p1;
    //std::vector<cv::Point2f> p2;
    int index = 0;
    for(int i = 0; i < unsortImg.rows; i ++)
    {
        for(int j = 0; j< unsortImg.cols; j ++)
        {
            if (mask.ptr<uchar>(i)[j] == 0) continue;
            cv::Mat points = (cv::Mat_<float>(1, 3) << i, j, 1);
            cv::Mat coef = points * E;

            const float a = coef.ptr<float>(0)[0];
            const float b = coef.ptr<float>(0)[1];
            const float c = coef.ptr<float>(0)[2];

            float proY = unwrapImg.ptr<float>(i)[j] * width / (2 * PI * freq);
            float proX = ((-b * proY) - c) / a;

            p1.ptr<float>(0)[index] = i;
            p1.ptr<float>(1)[index] = j;

            p2.ptr<float>(0)[index] = proX;
            p2.ptr<float>(1)[index] = proY;
            index++;
        }
    }
    std::cout << index << std::endl;
    /*
    unsortImg.forEach<float>([&mask, &p1, &p2, &E, &width, &freq](float& val, const int* pos) {
        if (mask.ptr<uchar>(pos[0])[pos[1]]) {
            cv::Mat points = (cv::Mat_<float>(1, 3) << pos[0], pos[1], 1);


            cv::Mat coef = points * E;



            const float a = coef.ptr<float>(0)[0];
            const float b = coef.ptr<float>(0)[1];
            const float c = coef.ptr<float>(0)[2];

            //std::cout << "a: " << a << " b: " << b << " c:" << c << std::endl;
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
    });


	cv::Mat Tw_ = (cv::Mat_<float>(3, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);
	cv::Mat Tp_ = cv::Mat::zeros(3, 4, CV_32F);
	Tp_.forEach<float>([&R, &t](float& val, const int* pos) {
		if (pos[1] == 3) val = t.ptr<float>(pos[0])[0];
		else val = R.ptr<float>(pos[0])[pos[1]];
		});

	cv::Mat res;
	cv::triangulatePoints(Tw_, Tp_, p1, p2, res);

    std::cout << "3D point is finish" << std::endl;
    */
    cv::Scalar totalNum = cv::sum(mask);
    const float fx = cameraInstrisincMatrix.ptr<float>(0)[0], fy = cameraInstrisincMatrix.ptr<float>(1)[1];
    const float u0 = cameraInstrisincMatrix.ptr<float>(0)[2], v0 = cameraInstrisincMatrix.ptr<float>(1)[2];
    cv::Mat xc = cv::Mat::ones(2, totalNum[0], CV_32F);
    cv::Mat xp = cv::Mat::ones(1, totalNum[0], CV_32F);
    cv::Mat xyz = cv::Mat::ones(3, totalNum[0], CV_32FC1);
    const float up0=projectorInstrisincMatrix.ptr<float>(0)[2];
    const float fp0 = projectorInstrisincMatrix.ptr<float>(0)[0];
    const float r11=extrinsicsMatrix.ptr<float>(0)[0], r12=extrinsicsMatrix.ptr<float>(0)[1], r13=extrinsicsMatrix.ptr<float>(0)[2];
    const float r31=extrinsicsMatrix.ptr<float>(2)[0], r32=extrinsicsMatrix.ptr<float>(2)[1], r33=extrinsicsMatrix.ptr<float>(2)[2];
    const float t1 = extrinsicsMatrix.ptr<float>(0)[3], t3 = extrinsicsMatrix.ptr<float>(2)[3];
    int num = 0;
    float molecular, denominator;
    for(int row=0;row<mask.rows;row++)
        for(int col=0;col<mask.cols;col++){
            if(mask.ptr<uchar>(row)[col]){
                xc.ptr<float>(0)[num] = (col+1-u0)/fx;
                xc.ptr<float>(1)[num] = (row+1-v0)/fy;
                xp.ptr<float>(0)[num] = (unwrapImg.ptr<float>(row)[col]*width)/(2*PI*freq);
                xp.ptr<float>(0)[num] = (xp.ptr<float>(0)[num]-up0)/fp0;
                molecular = t1 - t3*xp.ptr<float>(0)[num];
                denominator = xp.ptr<float>(0)[num]*(r31*xc.ptr<float>(0)[num]+r32*xc.ptr<float>(1)[num]+r33)-\
                        (r11*xc.ptr<float>(0)[num]+r12*xc.ptr<float>(1)[num]+r13);
                float z = molecular / denominator;
                xyz.ptr<float>(0)[num] = xc.ptr<float>(0)[num]*z;
                xyz.ptr<float>(1)[num] = xc.ptr<float>(1)[num]*z;
                xyz.ptr<float>(2)[num] = z;
                num++;
            }
        }
//    cv::transpose(xc, xc);
//    cv::Mat xcNorm = normalize_pixel(xc, cameraInstrisincMatrix, distortionCoeff);

//    xp.forEach<float>([&xcNorm,&r11,&r12,&r13,&r31,&r32,&r33,&t1,&t3,&xyz](float& val, const int* pos){
//        const float xcu = xcNorm.ptr<float>(0)[pos[1]];
//        const float xcv = xcNorm.ptr<float>(1)[pos[1]];
//        const float molecular = t1 - t3*val;
//        const float denominator = r31*xcu*val+r32*xcv*val+r33*val-(r11*xcu+r12*xcv+r13);
//        const float z = molecular / denominator;
//        xyz.ptr<float>(0)[pos[1]] = xcu*z;
//        xyz.ptr<float>(1)[pos[1]] = xcv*z;
//        xyz.ptr<float>(2)[pos[1]] = z;
//    });
    return xyz;
}
