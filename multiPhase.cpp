#pragma
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#define PI 3.14159
#define EPS 1e-8

namespace Algorithm {
	using namespace cv;

	cv::Mat multiHeterodyne(std::vector<Mat>& images, double f1, double f2, double f3, int n, Mat& mask) {
		//f1 f2  f3 为 条纹频率
		int m = images.size();
		if (m % n != 0) {
			std::cout << "输出出错" << std::endl;
			return;
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

		cv::Mat PH12 = Mat::zeros(PH1.size(), CV_32FC1);
		PH12.forEach<float>([&PH1, &PH2](float& val, const int* pos) {
			const float temp = PH1.ptr<float>(pos[0])[pos[1]] >= PH2.ptr<float>(pos[0])[pos[1]];
			if (temp >= 0) val = temp;
			else val = 2 * PI + temp;
			});

		cv::Mat PH23 = Mat::zeros(PH2.size(), CV_32FC1);
		PH23.forEach<float>([&PH2, &PH3](float& val, const int* pos) {
			const float temp = PH2.ptr<float>(pos[0])[pos[1]] >= PH3.ptr<float>(pos[0])[pos[1]];
			if (temp >= 0) val = temp;
			else val = 2 * PI + temp;
			});

		cv::Mat PH123 = Mat::zeros(PH1.size(), CV_32FC1);
		PH123.forEach<float>([&PH12, &PH23](float& val, const int* pos) {
			const float temp = PH12.ptr<float>(pos[0])[pos[1]] >= PH23.ptr<float>(pos[0])[pos[1]];
			if (temp >= 0) val = temp;
			else val = 2 * PI + temp;
			});

		//频率差
		double f12 = f2 - f1;
		double f23 = f3 - f2;
		double f123 = f12 - f23;

		//解包裹23：求PH23的绝对相位
		double R = f23 / f123;
		cv::Mat Nwrap = Mat::zeros(PH123.size(), CV_32FC1);
		Nwrap.forEach<float>([&PH123, &PH23, &R](float& val, const int* pos) {
			val = floor(0.5 + (R * PH123.ptr<float>(pos[0])[pos[1]] - PH23.ptr<float>(pos[0])[pos[1]]) / (2 * PI));
			});

		cv::Mat PH23UnWrap = 2 * PI * Nwrap + PH23;

		R = f2 / f23;
		cv::Mat Nwrap1 = Mat::zeros(PH23UnWrap.size(), CV_32FC1);
		Nwrap1.forEach<float>([&PH23UnWrap, &PH2, &R](float& val, const int* pos) {
			val = floor(0.5 + (R * PH23UnWrap.ptr<float>(pos[0])[pos[1]] - PH2.ptr<float>(pos[0])[pos[1]]) / (2 * PI));
			});
		cv::Mat PH2UnWrap = 2 * PI * Nwrap1 + PH23UnWrap;

		cv::multiply(PH2UnWrap, mask, PH2UnWrap);

		return PH2UnWrap;
	}

	cv::Mat unwrappingPhase(std::vector<cv::Mat>& images) {
		int n = images.size();

		cv::Mat imgUp = Mat::zeros(images[0].size(), CV_32FC1);
		cv::Mat imgDown = Mat::zeros(images[0].size(), CV_32FC1);
		for (int i = 0; i < images.size(); i++) {
			imgUp = imgUp + sin(2 * PI * i / n) * images[i];
			imgDown = imgDown + cos(2 * PI * i / n) * images[i];
		}
		cv::Mat res = Mat::zeros(imgUp.size(), CV_32FC1);
		res.forEach<float>([&imgUp, &imgDown](float& val, const int* pos) {
			const float upPixel = imgUp.ptr<float>(pos[0])[pos[1]];
			const float downPixel = imgDown.ptr<float>(pos[0])[pos[1]] + EPS;
			val = -atan2(upPixel, downPixel);
			});
		return res;
	}

	void unsortTriangulate(Mat& mask, Mat& unwrapImg, Mat& proImg, Mat& R, Mat& t, Mat& k1, Mat& k2, Mat& d1, int freq) {
		cv::undistort(unwrapImg, unwrapImg, k1, d1);
		
		Mat T = Mat::zeros(3, 3, CV_32F);
		T.ptr<float>(0)[1] = -t.ptr<float>(0)[2];
		T.ptr<float>(0)[2] = t.ptr<float>(0)[1];
		T.ptr<float>(1)[0] = t.ptr<float>(0)[2];

		T.ptr<float>(1)[2] = -t.ptr<float>(0)[0];
		T.ptr<float>(2)[0] = -t.ptr<float>(0)[1];
		T.ptr<float>(2)[1] = t.ptr<float>(0)[0];
		cv::transpose(k1, k1);
		cv::transpose(R, R);
		Mat E = k1 * R * T * k2;

		//投影仪的宽
		int width = 1140;

		std::vector<Point2f> p1;
		std::vector<Point2f> p2;

		unwrapImg.forEach<float>([&mask, &p1, &p2, &E, &width ,&freq](float& val, const int* pos) {
			if (mask.ptr<uchar>(pos[0])[pos[1]]) {
				Mat points = (Mat_<float>(1, 3) << pos[0], pos[1], 1);
				Mat coef = points * E;
				const float a = coef.ptr<float>(0)[0];
				const float b = coef.ptr<float>(0)[1];
				const float c = coef.ptr<float>(0)[2];

				//计算坐标
				float proY = val * width / (2 * PI * freq);
				float proX = ((-b * proY) - c) / a;
				Point2f tmp1;
				Point2f tmp2;
				tmp1.x = pos[0]; tmp1.y = pos[1];
				tmp2.x = proX; tmp2.y = proY;

				p1.push_back(tmp1);
				p2.push_back(tmp2);
			}
			}
			);

		cv::Mat Tw_ = (Mat_<float>(3, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);
		cv::Mat Tp_ = Mat::zeros(3, 4, CV_32F);
		Tp_.forEach<float>([&R, &t](float& val, const int* pos) {
			if (pos[1] == 3) val = t.ptr<float>(pos[0])[0];
			else val = R.ptr<float>(pos[0])[pos[1]];
			});
		Mat res;
		cv::triangulatePoints(Tw_, Tp_, p1, p2, res);

	}

}


