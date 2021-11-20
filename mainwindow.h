#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <PvDevice.h>
#include <PvSampleUtils.h>
//#include <PvDeviceFinderWnd.h>
#include <PvGenParameter.h>
//#include <PvGenBrowserWnd.h>
#include <PvBuffer.h>
#include <PvStream.h>
#include <PvPipeline.h>
//#include <PvDisplayWnd.h>
#include <PvAcquisitionStateManager.h>
#include <PvDeviceGEV.h>
#include <PvDeviceInfoGEV.h>
#include <PvDeviceInfoU3V.h>
#include <PvStreamGEV.h>

#include <QPixmap>
#include <QImage>
#include <qstring.h>

#include <time.h>

#include "Algorithm.h"
#include "ApparatusParamsCls.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);


    QImage cvMat2QImage(const cv::Mat& mat);
    PvDevice* ConnectDevice();
    bool PvSelectDeviceC( PvString *aConnectionID );
    void Disconnect();

    ~MainWindow();

private slots:
    void on_pbConnect_clicked();

    void on_pbReconstruction_clicked();


private:

    Ui::MainWindow *ui;

    PvDevice* mDevice;
    PvStream* mStream;
    PvPipeline* mPipeline;
    bool mAcquiringImages;

    PvAcquisitionStateManager *mAcquisitionStateManager;
};
#endif // MAINWINDOW_H
