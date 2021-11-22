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



LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_aruco420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_bgsegm420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_bioinspired420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_calib3d420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_ccalib420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_core420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_cudaarithm420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_cudabgsegm420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_cudacodec420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_cudafeatures2d420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_cudafilters420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_cudaimgproc420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_cudalegacy420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_cudaobjdetect420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_cudaoptflow420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_cudastereo420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_cudawarping420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_cudev420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_datasets420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_dnn420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_dnn_objdetect420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_dnn_superres420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_dpm420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_face420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_features2d420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_flann420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_fuzzy420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_gapi420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_hfs420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_highgui420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_imgcodecs420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_imgproc420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_img_hash420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_line_descriptor420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_ml420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_objdetect420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_optflow420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_phase_unwrapping420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_photo420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_plot420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_quality420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_reg420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_saliency420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_shape420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_stereo420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_stitching420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_structured_light420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_superres420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_surface_matching420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_text420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_tracking420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_video420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_videoio420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_videostab420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_xfeatures2d420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_ximgproc420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_xobjdetect420.lib
LIBS += $$PWD\..\Thirdparty\OpenCV420\lib\Release\opencv_xphoto420.lib


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
