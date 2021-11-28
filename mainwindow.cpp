#include "mainwindow.h"
#include "ui_mainwindow.h"



using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbConnect_clicked()
{
    QString T;
    T = ui->pbConnect->text();
    if(T == "连接相机"){
        jai.Run();
        if(jai.mDevice == nullptr) return;
        ui->pbConnect->setText("断开相机");
    }
    else{
        jai.TearDown( true );
        ui->pbConnect->setText("连接相机");
    }

}
void MainWindow::on_pbReconstruction_clicked()
{
    if(ui->rbGray->isChecked()){
        //格雷码重建
        QTime timeWatch;
        timeWatch.start();
        string filePath = "../structured-light-data/calibration/SLSParam.txt";
        camera leftCamera, rightCamera;
        Algorithm::readParams(leftCamera, rightCamera, filePath);
        //vector<cv::Mat> images;
        //vector<string> imageName;
        //cv::glob("../structured-light-data/graycode/", imageName, false);
        //for (size_t i = 0; i < 12; i++) {
        //    cv::Mat imgTmp = cv::imread(imageName[i], cv::IMREAD_GRAYSCALE);
        //    images.emplace_back(imgTmp);
        //}
        std::cout << jai.images.size() << std::endl;
        for(int i = 0; i< 11; i++)
        {
            images.push_back(jai.images[i]);
            string path = to_string(i) + ".bmp";
            imwrite(path,images[i]);
        }

        vector<vector<cv::Mat>> splitRes = Algorithm::splitImages(images);
        int height = splitRes[0][0].rows, width = splitRes[0][0].cols;
        cv::Mat mask = Algorithm::decodeMask(splitRes[0], height, width);
        cv::Mat grayCode = Algorithm::decodeGrayCode(splitRes[1], mask, height, width);
        cv::Mat wrappedPhase = Algorithm::decodeWrappedPhase(splitRes[2], mask, height, width);
        cv::Mat unWrappedPhase = Algorithm::decodeUnwrappedPhase(wrappedPhase, grayCode, mask);
        qDebug()<<"The time of graycode is :" << timeWatch.elapsed() << "ms";
        cv::Mat res1;
        unWrappedPhase.convertTo(res1, CV_8UC1);
        QImage qimg = cvMat2QImage(res1);
        //QImage img=QImage((const unsigned char*)(frame.data),frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
        QPixmap Pixmap = QPixmap::fromImage(qimg);
        Pixmap.scaled(ui->lbShow->size(), Qt::KeepAspectRatio);
        ui->lbShow->setScaledContents(true);
        ui->lbShow->setPixmap(Pixmap);
    }
    if(ui->rbMulti->isChecked()){
        //多频外差
        QTime timeWatch;
        timeWatch.start();

        string filePath = "../structured-light-data/calibration/SLSParam.txt";

        camera leftCamera, rightCamera;
        Algorithm::readParams(leftCamera, rightCamera, filePath);
        std::cout << leftCamera.instrisincMatrix << std::endl;
        std::cout << leftCamera.distortionCoeff<< std::endl;
        std::cout << rightCamera.extrinsicsMatrix << std::endl;
        /*
        vector<cv::Mat> images;
        vector<string> imageName;
        cv::glob("../structured-light-data/multiFre/", imageName, false);
        for (size_t i = 0; i < 12; i++) {
            cv::Mat imgTmp = cv::imread(imageName[i], cv::IMREAD_GRAYSCALE);
            imgTmp.convertTo(imgTmp, CV_32FC1);
            images.emplace_back(imgTmp);
        }

#ifdef DEBUG
        for(int i = 0; i < imageName.size(); i ++){
            cout << imageName[i] << endl;
            cout << images[i].size()<<endl;
        }
#endif
*/
        std::cout << jai.images.size() << std::endl;


        for(int i = 0; i< 12; i++)
        {
            images.push_back(jai.images[i]);
            string path = to_string(i) + ".bmp";
            imwrite(path,images[i]);

            images[i].convertTo(images[i], CV_8UC1);

        }


         double f1 = 59;
         double f2 = 64;
         double f3 = 70;
         /*
         cv::Mat test;
         images[0].copyTo(test);
         for(int i = 0; i < test.rows ; i++){
             for(int j = 0; j < test.cols; j++){
                 cout << test.ptr<float>(i)[j] << endl;
             }
         }
        */
         cv::Mat mask = cv::Mat::ones(images[0].rows,images[0].cols, CV_32FC1);

         cv::Mat res = Algorithm::multiHeterodyne(images, f1, f2, f3, 4, mask);
         qDebug()<<"The time of graycode is :" << timeWatch.elapsed() << "ms";

         cv::Mat res1;
         res.convertTo(res1, CV_8UC1);
         QImage qimg = cvMat2QImage(res1);
         //QImage img=QImage((const unsigned char*)(frame.data),frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
         QPixmap Pixmap = QPixmap::fromImage(qimg);
         Pixmap.scaled(ui->lbShow->size(), Qt::KeepAspectRatio);
         ui->lbShow->setScaledContents(true);
         ui->lbShow->setPixmap(Pixmap);

    }
}

//Mat 转 Qimage
QImage MainWindow::cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256); //5.9版本之后取消了setNumColors函数，可以使用image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        return QImage();
    }
}



void MainWindow::on_pushButton_6_clicked()
{
    QTime timeWatch;
    timeWatch.start();

    dlp.test();
    while(1)
    {
        if(jai.images.size() >= 12) break;
        Sleep(200);
    }
    if(ui->rbGray->isChecked()){
        //格雷码重建
        QTime timeWatch;
        timeWatch.start();
        string filePath = "../structured-light-data/calibration/SLSParam.txt";
        camera leftCamera, rightCamera;
        Algorithm::readParams(leftCamera, rightCamera, filePath);
        //vector<cv::Mat> images;
        //vector<string> imageName;
        //cv::glob("../structured-light-data/graycode/", imageName, false);
        //for (size_t i = 0; i < 12; i++) {
        //    cv::Mat imgTmp = cv::imread(imageName[i], cv::IMREAD_GRAYSCALE);
        //    images.emplace_back(imgTmp);
        //}
        std::cout << jai.images.size() << std::endl;
        for(int i = 0; i< 11; i++)
        {
            images.push_back(jai.images[i]);
            string path = to_string(i) + ".bmp";
            imwrite(path,images[i]);
        }

        vector<vector<cv::Mat>> splitRes = Algorithm::splitImages(images);
        int height = splitRes[0][0].rows, width = splitRes[0][0].cols;
        cv::Mat mask = Algorithm::decodeMask(splitRes[0], height, width);
        cv::Mat grayCode = Algorithm::decodeGrayCode(splitRes[1], mask, height, width);
        cv::Mat wrappedPhase = Algorithm::decodeWrappedPhase(splitRes[2], mask, height, width);
        cv::Mat unWrappedPhase = Algorithm::decodeUnwrappedPhase(wrappedPhase, grayCode, mask);
        qDebug()<<"The time of graycode is :" << timeWatch.elapsed() << "ms";
        cv::Mat res1;
        unWrappedPhase.convertTo(res1, CV_8UC1);
        QImage qimg = cvMat2QImage(res1);
        //QImage img=QImage((const unsigned char*)(frame.data),frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
        QPixmap Pixmap = QPixmap::fromImage(qimg);
        Pixmap.scaled(ui->lbShow->size(), Qt::KeepAspectRatio);
        ui->lbShow->setScaledContents(true);
        ui->lbShow->setPixmap(Pixmap);
    }
    if(ui->rbMulti->isChecked()){
        //多频外差

        string filePath = "../structured-light-data/calibration/SLSParam.txt";

        camera leftCamera, rightCamera;
        Algorithm::readParams(leftCamera, rightCamera, filePath);
        std::cout << leftCamera.instrisincMatrix << std::endl;
        std::cout << leftCamera.distortionCoeff<< std::endl;
        std::cout << rightCamera.extrinsicsMatrix << std::endl;
        /*
        vector<cv::Mat> images;
        vector<string> imageName;
        cv::glob("../structured-light-data/multiFre/", imageName, false);
        for (size_t i = 0; i < 12; i++) {
            cv::Mat imgTmp = cv::imread(imageName[i], cv::IMREAD_GRAYSCALE);
            imgTmp.convertTo(imgTmp, CV_32FC1);
            images.emplace_back(imgTmp);
        }

#ifdef DEBUG
        for(int i = 0; i < imageName.size(); i ++){
            cout << imageName[i] << endl;
            cout << images[i].size()<<endl;
        }
#endif
*/
        std::cout << jai.images.size() << std::endl;


        for(int i = 0; i< 12; i++)
        {
            images.push_back(jai.images[i]);
            string path = to_string(i) + ".bmp";
            imwrite(path,images[i]);

            images[i].convertTo(images[i], CV_8UC1);

        }


        double f1 = 59;
        double f2 = 64;
        double f3 = 70;
        /*
         cv::Mat test;
         images[0].copyTo(test);
         for(int i = 0; i < test.rows ; i++){
             for(int j = 0; j < test.cols; j++){
                 cout << test.ptr<float>(i)[j] << endl;
             }
         }
        */
        cv::Mat mask = cv::Mat::ones(images[0].rows,images[0].cols, CV_32FC1);

        cv::Mat res = Algorithm::multiHeterodyne(images, f1, f2, f3, 4, mask);
        qDebug()<<"The time of graycode is :" << timeWatch.elapsed() << "ms";

        cv::Mat res1;
        res.convertTo(res1, CV_8UC1);
        QImage qimg = cvMat2QImage(res1);
        //QImage img=QImage((const unsigned char*)(frame.data),frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
        QPixmap Pixmap = QPixmap::fromImage(qimg);
        Pixmap.scaled(ui->lbShow->size(), Qt::KeepAspectRatio);
        ui->lbShow->setScaledContents(true);
        ui->lbShow->setPixmap(Pixmap);
        }
}

void MainWindow::on_pbProject_clicked()
{
    std::vector<cv::Mat>().swap(jai.images);
    std::vector<cv::Mat>().swap(images);
    dlp.test();
    /*
    if(!dlp.Connect())
    {
        std::cout << "dlp is none" << std::endl;
        return;
    }
    if(!dlp.IsConnect())
    {
        std::cout << "dlp is disconnect" << std::endl;
        return;
    }
    dlp.setPatternMode();
    dlp.projectMulti();
    */

}
