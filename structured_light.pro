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
    dlp4500.cpp \
    jai.cpp \
    main.cpp \
    utils.cpp \
    mainwindow.cpp\
    DLPControl/dlpc350_BMPParser.cpp \
    DLPControl/dlpc350_api.cpp \
    DLPControl/dlpc350_common.cpp \
    DLPControl/dlpc350_firmware.cpp \
    DLPControl/dlpc350_usb.cpp

HEADERS += \
    Algorithm.h \
    ApparatusParamsCls.h \
    CalibrationCls.h \
    DLPControl/Helper.h \
    DLPControl/dlpc350_BMPParser.h \
    DLPControl/dlpc350_api.h \
    DLPControl/dlpc350_common.h \
    DLPControl/dlpc350_error.h \
    DLPControl/dlpc350_firmware.h \
    DLPControl/dlpc350_flashDevice.h \
    DLPControl/dlpc350_usb.h \
    DLPControl/dlpc350_version.h \
    DLPControl/hidapi.h \
    FileCls.h \
    calibration.h \
    cameraParams.h \
    utils.h \
    dlp4500.h \
    jai.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    structured_light_zh_CN.ts

INCLUDEPATH += $$PWD\..\Thirdparty\OpenCV420\include
INCLUDEPATH += $$PWD\..\Thirdparty\OpenCV420\include\opencv2
#INCLUDEPATH += D:\OpenCV420\include\opencv2
#INCLUDEPATH += D:\OpenCV420\include

LIBS += $$PWD\DLPControl\hidapi.lib

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

INCLUDEPATH += $$PWD\..\Thirdparty\PCL1.9.0\include\pcl-1.9
INCLUDEPATH += $$PWD\..\Thirdparty\PCL1.9.0\include\pcl-1.9\pcl
INCLUDEPATH += $$PWD\..\Thirdparty\PCL1.9.0\3rdParty\Boost\include\boost-1_68
INCLUDEPATH += $$PWD\..\Thirdparty\PCL1.9.0\3rdParty\Eigen\eigen3
INCLUDEPATH += $$PWD\..\Thirdparty\PCL1.9.0\3rdParty\FLANN\include
INCLUDEPATH += $$PWD\..\Thirdparty\PCL1.9.0\3rdParty\FLANN\include\flann
INCLUDEPATH += $$PWD\..\Thirdparty\PCL1.9.0\3rdParty\OpenNI2\Include
INCLUDEPATH += $$PWD\..\Thirdparty\PCL1.9.0\3rdParty\Qhull\include
INCLUDEPATH += $$PWD\..\Thirdparty\PCL1.9.0\3rdParty\VTK\include\vtk-8.1

LIBS += -L$$PWD/../Thirdparty/PCL1.9.0/lib\
       -lpcl_common_release         \
       -lpcl_features_release          \
       -lpcl_filters_release           \
       -lpcl_io_ply_release            \
       -lpcl_io_release                \
       -lpcl_kdtree_release            \
       -lpcl_keypoints_release         \
       -lpcl_ml_release                \
       -lpcl_octree_release           \
       -lpcl_outofcore_release          \
       -lpcl_people_release             \
       -lpcl_recognition_release        \
       -lpcl_registration_release       \
       -lpcl_sample_consensus_release   \
       -lpcl_search_release            \
       -lpcl_segmentation_release      \
       -lpcl_stereo_release            \
       -lpcl_surface_release           \
       -lpcl_tracking_release          \
       -lpcl_visualization_release

LIBS += -L$$PWD/../Thirdparty/PCL1.9.0/3rdParty/Boost/lib\
    -llibboost_atomic-vc141-mt-x64-1_68                     \
    -llibboost_bzip2-vc141-mt-x64-1_68                      \
    -llibboost_chrono-vc141-mt-x64-1_68                     \
    -llibboost_container-vc141-mt-x64-1_68                  \
    -llibboost_context-vc141-mt-x64-1_68                    \
    -llibboost_contract-vc141-mt-x64-1_68                   \
    -llibboost_coroutine-vc141-mt-x64-1_68                  \
    -llibboost_date_time-vc141-mt-x64-1_68                  \
    -llibboost_exception-vc141-mt-x64-1_68                  \
    -llibboost_fiber-vc141-mt-x64-1_68                      \
    -llibboost_filesystem-vc141-mt-x64-1_68                 \
    -llibboost_graph-vc141-mt-x64-1_68                      \
    -llibboost_graph_parallel-vc141-mt-x64-1_68             \
    -llibboost_iostreams-vc141-mt-x64-1_68                  \
    -llibboost_locale-vc141-mt-x64-1_68                     \
    -llibboost_log-vc141-mt-x64-1_68                        \
    -llibboost_log_setup-vc141-mt-x64-1_68                  \
    -llibboost_math_c99-vc141-mt-x64-1_68                   \
    -llibboost_math_c99f-vc141-mt-x64-1_68                  \
    -llibboost_math_c99l-vc141-mt-x64-1_68                  \
    -llibboost_math_tr1-vc141-mt-x64-1_68                   \
    -llibboost_math_tr1f-vc141-mt-x64-1_68                  \
    -llibboost_math_tr1l-vc141-mt-x64-1_68                  \
    -llibboost_mpi-vc141-mt-x64-1_68                        \
    -llibboost_numpy27-vc141-mt-x64-1_68                    \
    -llibboost_numpy37-vc141-mt-x64-1_68                    \
    -llibboost_prg_exec_monitor-vc141-mt-x64-1_68           \
    -llibboost_program_options-vc141-mt-x64-1_68            \
    -llibboost_python27-vc141-mt-x64-1_68                   \
    -llibboost_python37-vc141-mt-x64-1_68                   \
    -llibboost_random-vc141-mt-x64-1_68                     \
    -llibboost_regex-vc141-mt-x64-1_68                      \
    -llibboost_serialization-vc141-mt-x64-1_68              \
    -llibboost_signals-vc141-mt-x64-1_68                    \
    -llibboost_stacktrace_noop-vc141-mt-x64-1_68            \
    -llibboost_stacktrace_windbg-vc141-mt-x64-1_68          \
    -llibboost_stacktrace_windbg_cached-vc141-mt-x64-1_68   \
    -llibboost_system-vc141-mt-x64-1_68                     \
    -llibboost_test_exec_monitor-vc141-mt-x64-1_68          \
    -llibboost_thread-vc141-mt-x64-1_68                     \
    -llibboost_timer-vc141-mt-x64-1_68                      \
    -llibboost_type_erasure-vc141-mt-x64-1_68               \
    -llibboost_unit_test_framework-vc141-mt-x64-1_68        \
    -llibboost_wave-vc141-mt-x64-1_68                       \
    -llibboost_wserialization-vc141-mt-x64-1_68             \
    -llibboost_zlib-vc141-mt-x64-1_68

LIBS += -L$$PWD/../Thirdparty/PCL1.9.0/3rdParty/FLANN/lib\
    -lflann\
    -lflann_cpp\
    -lflann_cpp_s\
    -lflann_s

LIBS += -L$$PWD/../Thirdparty/PCL1.9.0/3rdParty/OpenNI2/Lib\
    -lOpenNI2

LIBS += -L$$PWD/../Thirdparty/PCL1.9.0/3rdParty/Qhull/lib\
    -lqhull\
    -lqhull_p\
    -lqhull_r\
    -lqhullcpp\
    -lqhullstatic\
    -lqhullstatic_r

LIBS += -L$$PWD/../Thirdparty/PCL1.9.0/3rdParty/VTK/lib\
    -lvtkalglib-8.1                          \
    -lvtkChartsCore-8.1                      \
    -lvtkCommonColor-8.1                     \
    -lvtkCommonComputationalGeometry-8.1     \
    -lvtkCommonCore-8.1                      \
    -lvtkCommonDataModel-8.1                 \
    -lvtkCommonExecutionModel-8.1            \
    -lvtkCommonMath-8.1                      \
    -lvtkCommonMisc-8.1                      \
    -lvtkCommonSystem-8.1                    \
    -lvtkCommonTransforms-8.1                \
    -lvtkDICOMParser-8.1                     \
    -lvtkDomainsChemistry-8.1                \
    -lvtkexoIIc-8.1                          \
    -lvtkexpat-8.1                           \
    -lvtkFiltersAMR-8.1                      \
    -lvtkFiltersCore-8.1                     \
    -lvtkFiltersExtraction-8.1               \
    -lvtkFiltersFlowPaths-8.1                \
    -lvtkFiltersGeneral-8.1                  \
    -lvtkFiltersGeneric-8.1                  \
    -lvtkFiltersGeometry-8.1                 \
    -lvtkFiltersHybrid-8.1                   \
    -lvtkFiltersHyperTree-8.1                \
    -lvtkFiltersImaging-8.1                  \
    -lvtkFiltersModeling-8.1                 \
    -lvtkFiltersParallel-8.1                 \
    -lvtkFiltersParallelImaging-8.1          \
    -lvtkFiltersPoints-8.1                   \
    -lvtkFiltersProgrammable-8.1             \
    -lvtkFiltersSelection-8.1                \
    -lvtkFiltersSMP-8.1                      \
    -lvtkFiltersSources-8.1                  \
    -lvtkFiltersStatistics-8.1               \
    -lvtkFiltersTexture-8.1                  \
    -lvtkFiltersTopology-8.1                 \
    -lvtkFiltersVerdict-8.1                  \
    -lvtkfreetype-8.1                        \
    -lvtkGeovisCore-8.1                      \
    -lvtkgl2ps-8.1                           \
    -lvtkhdf5-8.1                            \
    -lvtkhdf5_hl-8.1                         \
    -lvtkImagingColor-8.1                    \
    -lvtkImagingCore-8.1                     \
    -lvtkImagingFourier-8.1                  \
    -lvtkImagingGeneral-8.1                  \
    -lvtkImagingHybrid-8.1                   \
    -lvtkImagingMath-8.1                     \
    -lvtkImagingMorphological-8.1            \
    -lvtkImagingSources-8.1                  \
    -lvtkImagingStatistics-8.1               \
    -lvtkImagingStencil-8.1                  \
    -lvtkInfovisCore-8.1                     \
    -lvtkInfovisLayout-8.1                   \
    -lvtkInteractionImage-8.1                \
    -lvtkInteractionStyle-8.1                \
    -lvtkInteractionWidgets-8.1              \
    -lvtkIOAMR-8.1                           \
    -lvtkIOCore-8.1                          \
    -lvtkIOEnSight-8.1                       \
    -lvtkIOExodus-8.1                        \
    -lvtkIOExport-8.1                        \
    -lvtkIOExportOpenGL-8.1                  \
    -lvtkIOGeometry-8.1                      \
    -lvtkIOImage-8.1                         \
    -lvtkIOImport-8.1                        \
    -lvtkIOInfovis-8.1                       \
    -lvtkIOLegacy-8.1                        \
    -lvtkIOLSDyna-8.1                        \
    -lvtkIOMINC-8.1                          \
    -lvtkIOMovie-8.1                         \
    -lvtkIONetCDF-8.1                        \
    -lvtkIOParallel-8.1                      \
    -lvtkIOParallelXML-8.1                   \
    -lvtkIOPLY-8.1                           \
    -lvtkIOSQL-8.1                           \
    -lvtkIOTecplotTable-8.1                  \
    -lvtkIOVideo-8.1                         \
    -lvtkIOXML-8.1                           \
    -lvtkIOXMLParser-8.1                     \
    -lvtkjpeg-8.1                            \
    -lvtkjsoncpp-8.1                         \
    -lvtklibharu-8.1                         \
    -lvtklibxml2-8.1                         \
    -lvtklz4-8.1                             \
    -lvtkmetaio-8.1                          \
    -lvtkNetCDF-8.1                          \
    -lvtknetcdfcpp-8.1                       \
    -lvtkoggtheora-8.1                       \
    -lvtkParallelCore-8.1                    \
    -lvtkpng-8.1                             \
    -lvtkproj4-8.1                           \
    -lvtkRenderingAnnotation-8.1             \
    -lvtkRenderingContext2D-8.1              \
    -lvtkRenderingContextOpenGL-8.1          \
    -lvtkRenderingCore-8.1                   \
    -lvtkRenderingFreeType-8.1               \
    -lvtkRenderingGL2PS-8.1                  \
    -lvtkRenderingImage-8.1                  \
    -lvtkRenderingLabel-8.1                  \
    -lvtkRenderingLIC-8.1                    \
    -lvtkRenderingLOD-8.1                    \
    -lvtkRenderingOpenGL-8.1                 \
    -lvtkRenderingVolume-8.1                 \
    -lvtkRenderingVolumeOpenGL-8.1           \
    -lvtksqlite-8.1                          \
    -lvtksys-8.1                             \
    -lvtktiff-8.1                            \
    -lvtkverdict-8.1                         \
    -lvtkViewsContext2D-8.1                  \
    -lvtkViewsCore-8.1                       \
    -lvtkViewsInfovis-8.1                    \
    -lgdi32\
    -lUser32\
    -lvtkzlib-8.1

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
