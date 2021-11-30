#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <vtkRenderWidget.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <boost/make_shared.hpp>

#include <QPixmap>
#include <QImage>
#include <qstring.h>
#include <QTime>
#include <QtDebug>

#include <time.h>

#include "Algorithm.h"
#include "ApparatusParamsCls.h"
#include "jai.h"
#include "dlp4500.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);


    QImage cvMat2QImage(const cv::Mat& mat);

    void widgetShow(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);

    ~MainWindow();

private slots:
    void on_pbConnect_clicked();

    void on_pbReconstruction_clicked();

    void on_pushButton_6_clicked();

    void on_pbProject_clicked();

private:

    Ui::MainWindow *ui;
    JAI jai;
    DLP4500 dlp;
    vector<cv::Mat>images;
};
#endif // MAINWINDOW_H
