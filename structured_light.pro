QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Algorithm.cpp \
    ApparatusParamsCls.cpp \
    CalibrationCls.cpp \
    FileCls.cpp \
    calibration.cpp \
    cameraParams.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Algorithm.h \
    ApparatusParamsCls.h \
    CalibrationCls.h \
    FileCls.h \
    calibration.h \
    cameraParams.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    structured_light_zh_CN.ts

INCLUDEPATH += $$PWD\..\Thirdparty\OpenCV420\include
INCLUDEPATH += $$PWD\..\Thirdparty\OpenCV420\include\opencv2
#INCLUDEPATH += D:\OpenCV420\include\opencv2
#INCLUDEPATH += D:\OpenCV420\include



LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_aruco420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_bgsegm420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_bioinspired420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_calib3d420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_ccalib420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_core420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_cudaarithm420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_cudabgsegm420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_cudacodec420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_cudafeatures2d420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_cudafilters420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_cudaimgproc420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_cudalegacy420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_cudaobjdetect420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_cudaoptflow420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_cudastereo420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_cudawarping420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_cudev420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_datasets420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_dnn420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_dnn_objdetect420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_dnn_superres420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_dpm420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_face420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_features2d420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_flann420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_fuzzy420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_gapi420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_hfs420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_highgui420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_imgcodecs420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_imgproc420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_img_hash420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_line_descriptor420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_ml420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_objdetect420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_optflow420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_phase_unwrapping420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_photo420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_plot420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_quality420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_reg420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_saliency420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_shape420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_stereo420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_stitching420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_structured_light420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_superres420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_surface_matching420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_text420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_tracking420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_video420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_videoio420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_videostab420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_xfeatures2d420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_ximgproc420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_xobjdetect420d.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Debug\opencv_xphoto420d.lib


#LIBS += D:\Opencv4.5\opencv-build\lib\libopencv_*.a

# INCLUDEPATH += E:\Projects\library\Includes
# INCLUDEPATH += E:\Projects\library\Includes\EbUniversalProForU3V

INCLUDEPATH += $$PWD\..\Thirdparty\eBUSSDK\Includes

LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\SimpleImagingLib.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\SimpleImagingLib64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvVirtualDevice.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvVirtualDevice64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvTransmitter.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvTransmitter64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvSystem.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvSystem64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvStream.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvStream64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvSerial.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvSerial64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvPersistence.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvPersistence64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvGUI64_VC10.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvGUI64_VC12.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvGUI64_VC11.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvGUI64_VC14.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvGUI_VC10.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvGUI_VC11.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvGUI_VC12.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvGUI_VC14.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvGenICam.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvGenICam64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvDSSource.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvDSSource64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvDevice.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvDevice64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvCameraBridge.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvCameraBridge64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvBuffer.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvBuffer64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvBase.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvBase64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvAppUtils.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PvAppUtils64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PtUtilsLib.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PtUtilsLib64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PtConvertersLib.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\PtConvertersLib64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\EbUtilsLib.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\EbUtilsLib64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\EbTransportLayerLib.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\EbTransportLayerLib64.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\EbInstallerLib.lib
LIBS += $$PWD\..\Thirdparty\eBUSSDK\Libraries\EbInstallerLib64.lib


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
