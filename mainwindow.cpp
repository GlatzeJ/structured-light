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

void MainWindow::widgetShow()
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    if (pcl::io::loadPCDFile<pcl::PointXYZ>("D:/projects/pointcloud/code2_guanzi.pcd", *cloud) == -1) //* load the file
    {
        PCL_ERROR("Couldn't read file test_pcd.pcd \n");
        system("PAUSE");
    }

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
    string filePath = "../structured-light-data/calibration/SLSParam.txt";
    camera leftCamera, rightCamera;
    Algorithm::readParams(leftCamera, rightCamera, filePath);

    cv::Mat mask, unWrappedPhase;

    if(ui->rbGray->isChecked()){
        //格雷码重建
        QTime timeWatch;
        timeWatch.start();
        vector<cv::Mat> images;
        vector<string> imageName;
        cv::glob("../structured-light-data/graycode/", imageName, false);
        for (size_t i = 0; i < 11; i++) {
            cv::Mat imgTmp = cv::imread(imageName[i], cv::IMREAD_GRAYSCALE);
            images.emplace_back(imgTmp);
        }
        vector<vector<cv::Mat>> splitRes = Algorithm::splitImages(images);
        int height = splitRes[0][0].rows, width = splitRes[0][0].cols;
        mask = Algorithm::decodeMask(splitRes[0], height, width);
        cv::Mat grayCode = Algorithm::decodeGrayCode(splitRes[1], mask, height, width);
        cv::Mat wrappedPhase = Algorithm::decodeWrappedPhase(splitRes[2], mask, height, width);
        unWrappedPhase = Algorithm::decodeUnwrappedPhase(wrappedPhase, grayCode, mask);
    }
    if(ui->rbMulti->isChecked()){
        //多频外差
        for(int i = 0; i< 12; i++)
        {
            images.push_back(jai.images[i]);
            string path = to_string(i) + ".bmp";
            imwrite(path,images[i]);

            images[i].convertTo(images[i], CV_32FC1);

        }
        std::cout << images.size() << std::endl;

        double f1 = 29;
        double f2 = 34;
        double f3 = 40;

        mask = cv::Mat::ones(images[0].rows,images[0].cols, CV_32FC1);

        unWrappedPhase = Algorithm::multiHeterodyne(images, f1, f2, f3, 4, mask);
    }

    // 生成点云数据txt，pointCloudTxt

    // 数据转换利用pcl,装换为pointCloudPcd

    // 点云显示
    widgetShow();
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
    /*
    PvBuffer *lBuffer = NULL;
    PvResult lOperationResult;

    // Retrieve next buffer
    PvResult lResult = jai.lPipeline->RetrieveNextBuffer( &lBuffer, 1000, &lOperationResult );
    std::cout << "111" << std::endl;
    if ( lResult.IsOK() )
    {
        std::cout << "112" << std::endl;
        if ( lOperationResult.IsOK() )
        {
            std::cout << "113" << std::endl;
            PvPayloadType lType;
            //
            // We now have a valid buffer. This is where you would typically process the buffer.
            // -----------------------------------------------------------------------------------------
            // If the buffer contains an image, display width and height.
            uint32_t lWidth = 0, lHeight = 0;
            lType = lBuffer->GetPayloadType();
            if ( lType == PvPayloadTypeImage )
            {
                std::cout << "114" << std::endl;
                // Get image specific buffer interface.
                PvImage *lImage = lBuffer->GetImage();

                // Read width, height.
                lWidth = lImage->GetWidth();
                lHeight = lImage->GetHeight();
                std::cout << "115" << std::endl;
                std::cout << "  W: " << dec << lWidth << " H: " << lHeight;
                lImage->Alloc(lWidth, lHeight, PvPixelMono16);
                // Get image data pointer so we can pass it to CV::MAT container
                unsigned char *img = lImage->GetDataPointer();
                // Copy/convert Pleora Vision image pointer to cv::Mat container
                cv::Mat lframe((int)lHeight, (int)lWidth, CV_16U, (int*)img, cv::Mat::AUTO_STEP);
                lframe.convertTo(lframe, CV_8UC1);

                cv::imwrite("1.bmp", lframe);

            }
            else {
                std::cout << " (buffer does not contain image)";
            }
        }
        // Release the buffer back to the pipeline
        jai.lPipeline->ReleaseBuffer( lBuffer );
    }
    */
}

void MainWindow::on_pbProject_clicked()
{
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
