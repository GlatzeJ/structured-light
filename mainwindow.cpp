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
        mDevice = ConnectDevice();
        if(mDevice == nullptr) return;
        ui->pbConnect->setText("断开相机");
    }
    else{
        Disconnect();
        ui->pbConnect->setText("连接相机");
    }
    //std::cout << T.toStdString() << std::endl;

}




void MainWindow::on_pbReconstruction_clicked()
{


    if(ui->rbGray->isChecked()){
        //格雷码重建

    }
    if(ui->rbMulti->isChecked()){
        //多频外差
        string filePath = "../structured-light-data/calibration/SLSParam.txt";

        camera leftCamera, rightCamera;
        Algorithm::readParams(leftCamera, rightCamera, filePath);
        std::cout << leftCamera.instrisincMatrix << std::endl;
        std::cout << leftCamera.distortionCoeff<< std::endl;
        std::cout << rightCamera.extrinsicsMatrix << std::endl;
        vector<cv::Mat> images;
        vector<string> imageName;
        cv::glob("../structured-light-data/multiFre/", imageName, false);
        for (size_t i = 0; i < 12; i++) {
             cv::Mat imgTmp = cv::imread(imageName[i], cv::IMREAD_GRAYSCALE);
             imgTmp.convertTo(imgTmp, CV_32FC1);
             images.emplace_back(imgTmp);
         }

        for(int i = 0; i < imageName.size(); i ++){
            cout << imageName[i] << endl;
            cout << images[i].size()<<endl;
        }

         double f1 = 59;
         double f2 = 64;
         double f3 = 70;

         cv::Mat mask = cv::Mat::ones(images[0].rows,images[0].cols, CV_32FC1);

         cv::Mat res = Algorithm::multiHeterodyne(images, f1, f2, f3, 4, mask);
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

//相机连接
PvDevice* MainWindow::ConnectDevice()
{
    PvResult lResult = PvResult::Code::INVALID_PARAMETER;

    // Select device
    PvString lConnectionID;
    if ( !PvSelectDeviceC( &lConnectionID ) )
    {
        cout << "No device selected." << endl;
        return NULL;
    }

    // Creates and connects the device controller based on the selected device.
    cout << "Connecting to device" << endl;
    PvDevice *lDevice = PvDevice::CreateAndConnect( lConnectionID, &lResult );
    if ( ( lDevice == NULL ) || !lResult.IsOK() )
    {
        cout << "Unable to connect to device" << endl;
        return NULL;
    }

    return lDevice;
}

bool MainWindow::PvSelectDeviceC( PvString *aConnectionID )
{
    PvResult lResult;
    const PvDeviceInfo *lSelectedDI = NULL;
    PvSystem lSystem;

    //cout << endl << "Detecting devices." << endl;

    while( 1 )
    {

        lSystem.Find();

        // Detect, select device.
        vector<const PvDeviceInfo *> lDIVector;
        for ( uint32_t i = 0; i < lSystem.GetInterfaceCount(); i++ )
        {
            const PvInterface *lInterface = dynamic_cast<const PvInterface *>( lSystem.GetInterface( i ) );
            if ( lInterface != NULL )
            {
                cout << "   " << lInterface->GetDisplayID().GetAscii() << endl;
                for ( uint32_t j = 0; j < lInterface->GetDeviceCount(); j++ )
                {
                    const PvDeviceInfo *lDI = dynamic_cast<const PvDeviceInfo *>( lInterface->GetDeviceInfo( j ) );
                    if ( lDI != NULL )
                    {
                        lDIVector.push_back( lDI );
                        cout << "[" << ( lDIVector.size() - 1 ) << "]" << "\t" << lDI->GetDisplayID().GetAscii() << endl;
                    }
                }
            }
        }

        if ( lDIVector.size() == 0)
        {
            cout << "No device found!" << endl;
            return false;
        }


        // Read device selection, optional new IP address.
        uint32_t lIndex = 0;

        if ( lIndex == lDIVector.size() )
        {
            // We abort the selection process.
            return false;
        }
        else if ( lIndex < lDIVector.size() )
        {
            // The device is selected
            lSelectedDI = lDIVector[ lIndex ];
            break;
        }
    }

#ifndef WIN32
    // Flush the keyboard buffer so subsequent !kbhit() will work
    PvFlushKeyboard();
#endif // WIN32

    // If the IP Address valid?
    if ( lSelectedDI->IsConfigurationValid() )
    {
        cout << endl;
        *aConnectionID = lSelectedDI->GetConnectionID();
//        if ( aType != NULL )
//        {
//            *aType = lSelectedDI->GetType();
//        }

        return true;
    }

    if( ( lSelectedDI->GetType() == PvDeviceInfoTypeUSB ) || ( lSelectedDI->GetType() == PvDeviceInfoTypeU3V ) )
    {
        cout << "This device must be connected to a USB 3.0 (SuperSpeed) port." << endl;
        return false;
    }

    // Ask the user for a new IP address.
    cout << "The IP configuration of the device is not valid." << endl;
    cout << "Which IP address should be assigned to the device?" << endl;
    cout << ">";

    // Read new IP address.
    string lNewIPAddress;
    PV_DISABLE_SIGNAL_HANDLER();
    cin >> lNewIPAddress;
    PV_ENABLE_SIGNAL_HANDLER();
    if ( !lNewIPAddress.length() )
    {
        return false;
    }
    PvFlushKeyboard();

#ifndef WIN32
    // Flush the keyboard buffer so subsequent !kbhit() will work
    PvFlushKeyboard();
#endif // WIN32

    const PvDeviceInfoGEV* lDeviceGEV = dynamic_cast<const PvDeviceInfoGEV *>( lSelectedDI );
    if ( lDeviceGEV != NULL )
    {
        // Force new IP address.
        lResult = PvDeviceGEV::SetIPConfiguration( lDeviceGEV->GetMACAddress().GetAscii(), lNewIPAddress.c_str(),
                                                   lDeviceGEV->GetSubnetMask().GetAscii(), lDeviceGEV->GetDefaultGateway().GetAscii() );
        if ( !lResult.IsOK() )
        {
            cout << "Unable to force new IP address." << endl;
            return false;
        }
    }

    // Wait for the device to come back on the network.
    int lTimeout;
    while( 1 )
    {
#ifdef _UNIX_
        if ( gStop )
        {
            return false;
        }
#endif
        lTimeout = 10;
        while( lTimeout )
        {
#ifdef _UNIX_
            if ( gStop )
            {
                return false;
            }
#endif

            lSystem.Find();

            vector<const PvDeviceInfo *> lDIVector;
            for ( uint32_t i = 0; i < lSystem.GetInterfaceCount(); i++ )
            {
                const PvInterface *lInterface = lSystem.GetInterface( i );
                for ( uint32_t j = 0; j < lInterface->GetDeviceCount(); j++ )
                {
                    if ( lInterface->GetDeviceInfo( j )->GetType() == PvDeviceInfoTypeGEV )
                    {
                        const PvDeviceInfoGEV *lDI = dynamic_cast<const PvDeviceInfoGEV*>( lInterface->GetDeviceInfo( j ) );
                        if ( lDI != NULL )
                        {
                            if ( lNewIPAddress == lDI->GetIPAddress().GetAscii() )
                            {
                                cout << endl;
                                *aConnectionID = lDI->GetConnectionID();
//                                if ( aType != NULL )
//                                {
//                                    *aType = lDI->GetType();
//                                }

                                return true;
                            }
                        }
                    }
                }
            }
            PvSleepMs( 1000 );

            lTimeout--;
        }

        cout << "The device " << lNewIPAddress << " was not located. Do you want to continue waiting? yes or no" << endl;
        cout << ">";

        string lAnswer;
        PV_DISABLE_SIGNAL_HANDLER();
        cin >> lAnswer;
        PV_ENABLE_SIGNAL_HANDLER();
        if ( ( lAnswer == "n" ) || ( lAnswer == "no" ) )
        {
            break;
        }
    }

    cout << endl;
    return false;
}

void MainWindow::Disconnect(){

    // Release acquisition state manager
    if ( mAcquisitionStateManager != NULL )
    {
        //delete mAcquisitionStateManager;
        mAcquisitionStateManager = NULL;
    }


    if ( mDevice != NULL )
    {
        PvDevice::Free( mDevice );
        mDevice = NULL;
    }

    if ( mPipeline != NULL )
    {
        //delete mPipeline;
        mPipeline = NULL;
    }

    if ( mStream != NULL )
    {
        //PvStream::Free( mStream );
        mStream = NULL;
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


