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

void MainWindow::widgetShow(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
//    if (pcl::io::loadPCDFile<pcl::PointXYZ>("E:/Projects/structured-light-data/code2_guanzi.pcd", *cloud) == -1) //* load the file
//    {
//        PCL_ERROR("Couldn't read file test_pcd.pcd \n");
//        system("PAUSE");
//    }

    //pcl::visualization::CloudViewer viewer("Viewer");
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowIt;

    vtkObject::GlobalWarningDisplayOff();
    viewer =
         boost::make_shared<pcl::visualization::PCLVisualizer>("3DViewer", false);
    renderWindow = viewer->getRenderWindow();
    renderWindowIt = vtkRenderWindowInteractor::New();

    const QRect vtkSize = ui->VTKwidget->geometry();
    renderWindow->SetSize(vtkSize.right() - vtkSize.left(),
                          vtkSize.bottom() - vtkSize.top());

    renderWindow->SetParentId((HWND)ui->VTKwidget->winId());
    renderWindowIt->SetRenderWindow(renderWindow);
    viewer->createInteractor();
    renderWindow->Render();
    viewer->addPointCloud(cloud, "cloud");
    viewer->resetCamera ();
    ui->VTKwidget->update ();
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
    QTime timeWatch;
    timeWatch.start();
    string filePath = "../structured-light-data/calibration/SLSParam.txt";
    camera leftCamera, rightCamera;
    Algorithm::readParams(leftCamera, rightCamera, filePath);

    cv::Mat mask, unWrappedPhase;
    /*
    if(jai.images.size() == 0)
    {
        std::cerr << "没有图片";
        return;
    }
    */


    if(ui->rbGray->isChecked()){
        //格雷码重建

        /*
        vector<cv::Mat> images;
        vector<string> imageName;
        cv::glob("../structured-light-data/graycode/", imageName, false);
        for (size_t i = 0; i < 11; i++) {
            cv::Mat imgTmp = cv::imread(imageName[i], cv::IMREAD_GRAYSCALE);
            images.emplace_back(imgTmp);
        }
        */
        for (size_t i = 0; i < 11; i++)
        {
            images.push_back(jai.images[i]);
        }
        vector<vector<cv::Mat>> splitRes = Algorithm::splitImages(images);
        int height = splitRes[0][0].rows, width = splitRes[0][0].cols;
        mask = Algorithm::decodeMask(splitRes[0], height, width);
        cv::Mat grayCode = Algorithm::decodeGrayCode(splitRes[1], mask, height, width);
        cv::Mat wrappedPhase = Algorithm::decodeWrappedPhase(splitRes[2], mask, height, width);
        unWrappedPhase = Algorithm::decodeUnwrappedPhase(wrappedPhase, grayCode, mask);
    }
    if(ui->rbMulti->isChecked()){
        //多频外差重建
        string filePath = "../structured-light-data/calibration/SLSParam.txt";

        camera leftCamera, rightCamera;
        Algorithm::readParams(leftCamera, rightCamera, filePath);
        std::cout << "leftCamera.instrisincMatrix:" << leftCamera.instrisincMatrix << std::endl;
        std::cout << "leftCamera.distortionCoeff:" << leftCamera.distortionCoeff<< std::endl;
        std::cout << "rightCamera.extrinsicsMatrix:" << rightCamera.extrinsicsMatrix << std::endl;

        vector<cv::Mat> images;
        vector<string> imageName;
        cv::glob("../structured-light-data/multiFre/", imageName, false);
        vector<cv::Mat>mk;
        for (size_t i = 0; i < 14; i++) {
            if(i < 2)
            {
                cv::Mat imgTmp = cv::imread(imageName[i], cv::IMREAD_GRAYSCALE);
                mk.emplace_back(imgTmp);
            }
            else
            {
                cv::Mat imgTmp = cv::imread(imageName[i], cv::IMREAD_GRAYSCALE);
                images.emplace_back(imgTmp);
            }
        }
        mask = Algorithm::decodeMask(mk, mk[0].rows, mk[0].cols);
        cv::imwrite("mask.tiff", mask);
        double f1 = 70;
        double f2 = 64;
        double f3 = 59;
        unWrappedPhase = Algorithm::multiHeterodyne(images, f1, f2, f3, 4);
    }
    //保存图片
#ifdef DEBUG
    cv::imwrite("unWrappedPhase.tiff", unWrappedPhase);
#endif
    qDebug()<<"The time of graycode is :" << timeWatch.elapsed() << "ms";

    // 生成点云数据txt，pointCloudTxt
//    unWrappedPhase.convertTo(unWrappedPhase, CV_32FC1);
//    leftCamera.extrinsicsMatrix = (cv::Mat_<float>(3,4) << 1,0,0,0,0,1,0,0,0,0,1,0);
//    cv::Mat Ac = leftCamera.instrisincMatrix * leftCamera.extrinsicsMatrix;
//    cv::Mat Ap = rightCamera.instrisincMatrix * rightCamera.extrinsicsMatrix;
//    std::cout << Ac << std::endl;
//    std::cout << Ap << std::endl;
    //多频外差 频率为64
    cv::Mat res = Algorithm::unsortTriangulate(mask, unWrappedPhase, leftCamera.instrisincMatrix,
                                            rightCamera.instrisincMatrix, rightCamera.extrinsicsMatrix, leftCamera.distortionCoeff, 64);
    // 数据转换利用pcl,装换为pointCloudPcd
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);


    for(int i = 0 ; i < res.cols; i++)
    {
        pcl::PointXYZ point(res.ptr<float>(0)[i], res.ptr<float>(1)[i], res.ptr<float>(2)[i]);
        cloud->push_back(point);
    }
    pcl::PCDWriter writer;
    writer.write("res.pcd", *cloud);
    // 点云显示
    widgetShow(cloud);
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

}

void MainWindow::on_pbProject_clicked()
{
    std::vector<cv::Mat>().swap(jai.images);
    std::vector<cv::Mat>().swap(images);
    dlp.test();
}
