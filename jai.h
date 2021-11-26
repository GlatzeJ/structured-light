#ifndef JAI_H
#define JAI_H

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

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>


#define BUFFER_COUNT ( 16 )

class JAI : protected PvPipelineEventSink,PvDeviceEventSink
{
public:
    JAI();
    ~JAI();
    bool Run();

    int flag = 1;
    int index = 0;
    PvString mConnectionID;
    PvDevice* mDevice;
    PvStream* mStream;
    PvPipeline* mPipeline;
    bool mConnectionLost;

    vector<cv::Mat>images;

    bool SelectDevice();
    bool ConnectDevice();
    void DisconnectDevice();
    bool OpenStream();
    void CloseStream();
    bool StartAcquisition();
    bool StopAcquisition();
    bool PvSelectDeviceC( PvString *aConnectionID );
    void ApplicationLoop();
    void TearDown( bool aStopAcquisition );

    // Inherited from PvDeviceEventSink.
    void OnLinkDisconnected( PvDevice* aDevice );
    void OnBufferReady(PvPipeline* aPipeline);
};

#endif // JAI_H
