/*!
* \file FileCls.cpp
* \date 2017/05/20 15:59
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
#include "FileCls.h"


FileCls::FileCls(void)
{//the default construct class

	camInitParams = new ApparatusParamsCls();   //结构光系统相机的标定
	prjInitparams = new ApparatusParamsCls();   //结构光系统投影仪的标定

	//eosInitparams=new ApparatusParamsCls();   //单个相机的标定

	leftcamInitparams = new ApparatusParamsCls();//双目+相机、双目视觉系统中的两个相机
	rightcamInitparams = new ApparatusParamsCls();

}
FileCls::FileCls(CalibrationType calibType) :calibType(calibType) {
	if ((calibType & ONLYCAM) == ONLYCAM)                 //1-结构光系统-相机
		camInitParams = new ApparatusParamsCls();
	if ((calibType & ONLYPRJ) == ONLYPRJ)                //2-结构光系统-投影仪
		prjInitparams = new ApparatusParamsCls();
	//if((calibType&ONLYEOS)==ONLYEOS)                //单个相机
	//	eosInitparams=new ApparatusParamsCls();
	if ((calibType & LEFT) == LEFT)                         //4-双目+相机、双目视觉系统中的左相机
		leftcamInitparams = new ApparatusParamsCls();
	if ((calibType & RIGHT) == RIGHT)                       //8-双目+相机、双目视觉系统中的右相机
		rightcamInitparams = new ApparatusParamsCls();

	//if((calibType&CAM_CAM_EXTRISINC)==CAM_CAM_EXTRISINC){   
	//	camInitParams=new ApparatusParamsCls();
	//	eosInitparams=new ApparatusParamsCls();
	//}

}

FileCls::~FileCls(void)
{


}


void FileCls::readDefaultData() {   //需要读取6个文件夹下的图像

	if ((calibType & ONLYCAM) == ONLYCAM) {     //calibType==1
		//std::cout<<"\n\ncamera parameters' initialization\n";
		//camInitParams->readInitParamsFromStdCin();
		//camInitParams->setFixedParams(30,10,8,10.5,10.5,"../cam/cam",".bmp");  //读结构光系统-相机标定图像
		camInitParams->setFixedParams(26, 11, 8, 18.5, 18.5, "../../structured-light code/cam/cam", ".bmp");
		//camInitParams->setFixedParams(5, 9, 6, 30, 30, "../left0/left0", ".jpg");
	}

	if ((calibType & ONLYPRJ) == ONLYPRJ) {     //calibType&2==2
		//std::cout<<"\n\nprojector parameters' initialization\n";
		prjInitparams->setFixedParams(26, 11, 8, 60, 60, "../../structured-light code/prj/prj", ".bmp", "../pattern/patten.bmp");  //读结构光系统-投影仪标定所需图像
		//prjInitparams->setFixedParams(30, 10, 8, 50, 50, "../prj/prj", ".bmp", "../pattern/pattern1024.bmp");
	}
	//if((calibType&ONLYEOS)==ONLYEOS){    

	//	eosInitparams->setFixedParams(45,14,10,20,20,"../eos/eos",".bmp");    //读单相机标定图像
	//	/*eosInitparams->setFixedParams(5, 9, 6, 30, 30, "../right0/right0", ".jpg");  */ 
	//}
	if ((calibType & LEFT) == LEFT) {      //calibType&4==4

		//eosInitparams->setFixedParams(45,14,10,20,20,"../eos/eos",".bmp");
		leftcamInitparams->setFixedParams(5, 9, 6, 30, 30, "../../structured-light code/left0/left0", ".bmp");  //读双目左相机标定图像
	}
	if ((calibType & RIGHT) == RIGHT) {   //calibType&8==8

		//eosInitparams->setFixedParams(45,14,10,20,20,"../eos/eos",".bmp");
		rightcamInitparams->setFixedParams(5, 9, 6, 30, 30, "../../structured-light code/right0/right0", ".bmp");   //读双目右相机标定图像
	}



	//if((calibType&CAM_CAM_EXTRISINC)==CAM_CAM_EXTRISINC){
	//	std::cout<<"\n\npls make sure the intrinsic params in ../camEosIntrisicParam.txt\n"
	//		"the format in ../camEosIntrisicParam.txt is:\n"
	//		"\t\timgNum\n\t\tboardW\t\tboardH\n\t\tdx\t\tdy\n"
	//		"\t\tcamFileName\t\timgType\n"
	//		"\t\teosFileName\t\timgType\n"
	//		"\t\tcamFx\t\tcamFy\n\t\tcamCx\t\tcamCy\n\t\tcamK1\t\tcamK2\t\tcamK3\t\tcamK4\n"
	//		"\t\teosFx\t\teosFy\n\t\teosCx\t\teosCy\n\t\teosK1\t\teosK2\t\teosK3\t\teosK4\n" 
	//		"And:\nthe images used for calculating extrinsic params must be  store in  the given file Path\n";
	//	readIntrinsicParamsFromLocalTxt("../camEosIntrisicParam.txt");
	//}


}
void FileCls::readIntrinsicParamsFromLocalTxt(std::string localTxtName) {
	std::fstream fileHandle;
	fileHandle.open(localTxtName.c_str(), std::ios::in);
	int imgNum;
	int boardW;// number of  checkerboard's grids in width direction 
	int boardH;
	double dx;//chessboard's width
	double dy;
	fileHandle >> imgNum >> boardW >> boardH >> dx >> dy;  //读取

	std::string fileName, imgType;
	fileHandle >> fileName >> imgType;
	leftcamInitparams->setFixedParams(imgNum, boardW, boardH, dx, dy, fileName, imgType);

	fileHandle >> fileName >> imgType;
	rightcamInitparams->setFixedParams(imgNum, boardW, boardH, dx, dy, fileName, imgType);

	double fx, fy, cx, cy, k1, k2, k3, k4;
	fileHandle >> fx >> fy >> cx >> cy >> k1 >> k2 >> k3 >> k4;
	leftcamInitparams->setIntrisincParams(fx, fy, cx, cy, k1, k2, k3, k4);
	fileHandle >> fx >> fy >> cx >> cy >> k1 >> k2 >> k3 >> k4;
	rightcamInitparams->setIntrisincParams(fx, fy, cx, cy, k1, k2, k3, k4);
	fileHandle.close();

}

//
void FileCls::readDataFromStdCin() {
	if ((calibType & ONLYCAM) == ONLYCAM) {       //calibType&1==1
		std::cout << "\n\ncamera parameters' initialization\n";
		camInitParams->readInitParamsFromStdCin();
	}

	if ((calibType & ONLYPRJ) == ONLYPRJ) {       //calibType&1==2
		std::cout << "\n\nprojector parameters' initialization\n";
		prjInitparams->readInitParamsFromStdCin();

	}
	//if((calibType&ONLYEOS)==ONLYEOS){
	//	std::cout<<"\n\ndigital camera parameters' initialization\n";
	//	eosInitparams->readInitParamsFromStdCin();
	//}
	if ((calibType & LEFT) == LEFT) {          //calibType&4==4
		std::cout << "\n\nleft camera parameters' initialization\n";
		leftcamInitparams->readInitParamsFromStdCin();
	}
	if ((calibType & RIGHT) == RIGHT) {        //calibType&8==8
		std::cout << "\n\nright camera parameters' initialization\n";
		rightcamInitparams->readInitParamsFromStdCin();
	}

}
void FileCls::writeInstrinsicParams(cv::Mat instrisincMatrix, cv::Mat distortionCoeff, std::string fileName, std::ios_base::openmode mode) {
	std::fstream fileHandle;
	fileHandle.open(fileName.c_str(), mode);
	fileHandle << "\n";
	for (int i = 0; i < instrisincMatrix.rows; ++i)
	{
		for (int j = 0; j < instrisincMatrix.cols; ++j)
			fileHandle << std::setprecision(12) << instrisincMatrix.at<double>(i, j) << "\t\t";
		if (instrisincMatrix.cols > 1)
			fileHandle << "\n";
	}
	fileHandle << "\n";
	for (int i = 0; i < distortionCoeff.rows; ++i)
	{
		for (int j = 0; j < distortionCoeff.cols; ++j)
			fileHandle << std::setprecision(12) << distortionCoeff.at<double>(i, j) << "\t\t";
		if (distortionCoeff.cols > 1)
			fileHandle << "\n";
	}
	fileHandle << "\n\n\n";
	fileHandle.close();
}
void FileCls::writeRT(cv::Mat R, cv::Mat T, std::string fileName, std::ios_base::openmode mode) {
	std::fstream fileHandle;
	fileHandle.open(fileName.c_str(), mode);
	fileHandle << "\n";
	for (int i = 0; i < R.rows; ++i)
	{
		for (int j = 0; j < R.cols; ++j)
			fileHandle << std::setprecision(12) << R.at<double>(i, j) << "\t\t";
		if (R.cols > 1)
			fileHandle << "\n";
	}
	fileHandle << "\n";
	for (int i = 0; i < T.rows; ++i)
	{
		for (int j = 0; j < T.cols; ++j)
			fileHandle << std::setprecision(12) << T.at<double>(i, j) << "\t\t";
		if (T.cols > 1)
			fileHandle << "\n";
	}
	fileHandle << "\n";
	fileHandle.close();

}
void FileCls::writeParamsForLib(std::string fileName, cv::Mat camInstrisincMatrix, cv::Mat camDistortionCoeff, cv::Mat prjInstrisincMatrix, cv::Mat prjDistortionCoeff, cv::Mat R, cv::Mat T) {
	std::fstream fileHandle;
	fileHandle.open(fileName.c_str(), std::ofstream::out);
	fileHandle << std::setprecision(12) << "fc[0] =" << camInstrisincMatrix.at<double>(0, 0) << "; fc[1] =" << camInstrisincMatrix.at<double>(1, 1) << "; //CAM的焦距\n";
	fileHandle << std::setprecision(12) << "cc[0] = " << camInstrisincMatrix.at<double>(0, 2)
		<< "; cc[1] = " << camInstrisincMatrix.at<double>(1, 2) << ";//CAM中心点\n";
	fileHandle << std::setprecision(12) << "kc[0] =" << camDistortionCoeff.at<double>(0, 0) <<
		"; kc[1] =" << camDistortionCoeff.at<double>(1, 0) <<
		";  kc[2] =" << camDistortionCoeff.at<double>(2, 0) <<
		";  kc[3] =" << camDistortionCoeff.at<double>(3, 0)
		<< ";  kc[4] =0;//畸变参数，请参照MATLAB里的定义\n";
	fileHandle << "alpha_c = 0;\n\n//下面是prj的参数，类似\n";
	fileHandle << std::setprecision(12) << "fp[0] =" << prjInstrisincMatrix.at<double>(0, 0) << "; fp[1] =" << prjInstrisincMatrix.at<double>(1, 1) << "; \n";
	fileHandle << std::setprecision(12) << "cp[0] = " << prjInstrisincMatrix.at<double>(0, 2)
		<< "; cp[1] = " << prjInstrisincMatrix.at<double>(1, 2) << ";\n";
	fileHandle << std::setprecision(12) << "kp[0] =" << prjDistortionCoeff.at<double>(0, 0) <<
		"; kp[1] =" << prjDistortionCoeff.at<double>(1, 0) <<
		";  kp[2] =" << prjDistortionCoeff.at<double>(2, 0) <<
		";  kp[3] =" << prjDistortionCoeff.at<double>(3, 0)
		<< ";  kp[4] =0;\n";
	fileHandle << "alpha_p = 0;\n\n// R ---CAM-PRJ的相对旋转矩阵\n";
	fileHandle << std::setprecision(12) << " Matrix_Rot[0][0] =" <<
		R.at<double>(0, 0) << ";\nMatrix_Rot[0][1] =  " <<
		R.at<double>(0, 1) << ";\nMatrix_Rot[0][2] = " <<
		R.at<double>(0, 2) << ";\n\n";
	fileHandle << std::setprecision(12) << " Matrix_Rot[1][0] =" <<
		R.at<double>(1, 0) << ";\nMatrix_Rot[1][1] =  " <<
		R.at<double>(1, 1) << ";\nMatrix_Rot[1][2] = " <<
		R.at<double>(1, 2) << ";\n\n";
	fileHandle << std::setprecision(12) << " Matrix_Rot[2][0] =" <<
		R.at<double>(2, 0) << ";\nMatrix_Rot[2][1] =  " <<
		R.at<double>(2, 1) << ";\nMatrix_Rot[2][2] = " <<
		R.at<double>(2, 2) << ";\n\n   // T ---CAM-PRJ的相对平移矩阵\n";
	fileHandle << std::setprecision(12) << "Vector_Trans[0] ="
		<< T.at<double>(0, 0) << ";\nVector_Trans[1] = "
		<< T.at<double>(1, 0) << ";\nVector_Trans[2] = "
		<< T.at<double>(2, 0) << ";\n ";

	fileHandle << "\n\n\n";
	fileHandle.close();
}


