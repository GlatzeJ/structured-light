#include "jai.h"

JAI::JAI()
{
    mDevice = nullptr;
    mStream = nullptr;
    mPipeline = nullptr;
    mConnectionLost = false;
}

JAI::~JAI()
{
}

bool JAI::Run()
{
    // Select device
    if ( !SelectDevice() )
    {
        return false;
    }

    if ( mConnectionLost && ( mDevice != NULL ) )
    {
        // Device lost: no need to stop acquisition
        TearDown( false );
    }

    // If the device is not connected, attempt reconnection
    if ( mDevice == NULL )
    {
        if ( ConnectDevice() )
        {
            // Device is connected, open the stream
            if ( OpenStream() )
            {
                // Device is connected, stream is opened: start acquisition
                if ( !StartAcquisition() )
                {
                    TearDown( false );
                }
            }
            else
            {
                TearDown( false );
            }
        }
    }
    // Acquisition loop. Will break when user hits a key
    //ApplicationLoop();

    // Closes, disconnects, etc.
    //TearDown( true );

    return true;
}



bool JAI::SelectDevice()
{
    // Select the device
    if ( !PvSelectDeviceC( &mConnectionID ) )
    {
        std::cout << "No device selected." << std::endl;
        return false;
    }
    return true;
}

bool JAI::ConnectDevice()
{
    //std::cout << "--> ConnectDevice " <<  mConnectionID.GetAscii() << std::endl;
    // Connect to the selected Device
    PvResult lResult = PvResult::Code::INVALID_PARAMETER;
    mDevice = PvDevice::CreateAndConnect( mConnectionID, &lResult );
    if ( !lResult.IsOK() )
    {
        return false;
    }

    // Register this class as an event sink for PvDevice call-backs
    mDevice->RegisterEventSink( this );

    //设置触发模式
    int64_t gCount;
    PvGenParameterArray* lGenDevice = mDevice->GetParameters();
    PvGenEnum* lMode = lGenDevice->GetEnum("TriggerMode");
    lMode->GetEntriesCount(gCount);

    int64_t StartTriggerMode = 1;
    lMode->SetValue(StartTriggerMode);
    PvGenEnum* lMode1 = lGenDevice->GetEnum("TriggerSource");
    lMode1->GetEntriesCount(gCount);

    //low 0;high 1;software 2;jaipulsegenrator0 8;jaiuseroutput0 3;Line5 13;
    int64_t Triggersource = 13;
    lMode1->SetValue(Triggersource);

    // Clear connection lost flag as we are now connected to the device
    mConnectionLost = false;

    return true;
}


bool JAI::OpenStream()
{
    //std::cout << "--> OpenStream" << std::endl;
    // Creates and open the stream object based on the selected device.
    PvResult lResult = PvResult::Code::INVALID_PARAMETER;
    mStream = PvStream::CreateAndOpen( mConnectionID, &lResult );
    if ( !lResult.IsOK() )
    {
        std::cout << "Unable to open the stream" << std::endl;
        return false;
    }
    mPipeline = new PvPipeline( mStream );

    // Reading payload size from device
    int64_t lSize = mDevice->GetPayloadSize();

    // Create, init the PvPipeline object
    mPipeline->SetBufferSize( static_cast<uint32_t>( lSize ) );
    mPipeline->SetBufferCount( 16 );

    mPipeline->RegisterEventSink( this );

    // The pipeline needs to be "armed", or started before  we instruct the device to send us images
    lResult = mPipeline->Start();
    if ( !lResult.IsOK() )
    {
        std::cout << "Unable to start pipeline" << std::endl;
        return false;
    }

    // Only for GigE Vision, if supported
    PvGenBoolean *lRequestMissingPackets = dynamic_cast<PvGenBoolean *>( mStream->GetParameters()->GetBoolean( "RequestMissingPackets" ) );
    if ( ( lRequestMissingPackets != NULL ) && lRequestMissingPackets->IsAvailable() )
    {
        // Disabling request missing packets.
        lRequestMissingPackets->SetValue( false );
    }

    return true;
}

//
// Closes the stream, pipeline
//

void JAI::CloseStream()
{
    std::cout << "--> CloseStream" << std::endl;
    if ( mPipeline != NULL )
    {
        if (  mPipeline->IsStarted() )
        {
            if ( !mPipeline->Stop().IsOK() )
            {
                std::cout << "Unable to stop the pipeline." << std::endl;
            }
        }

        delete mPipeline;
        mPipeline = NULL;
    }

    if ( mStream != NULL )
    {
        if ( mStream->IsOpen() )
        {
            if ( !mStream->Close().IsOK() )
            {
                std::cout << "Unable to stop the stream." << std::endl;
            }
        }

        PvStream::Free( mStream );
        mStream = NULL;
    }
}

//
// Starts image acquisition
//

bool JAI::StartAcquisition()
{
    std::cout << "--> StartAcquisition" << std::endl;

    // Flush packet queue to make sure there is no left over from previous disconnect event
    PvStreamGEV* lStreamGEV = dynamic_cast<PvStreamGEV*>( mStream );
    if ( lStreamGEV != NULL )
    {
        lStreamGEV->FlushPacketQueue();
    }

    // Set streaming destination (only GigE Vision devces)
    PvDeviceGEV* lDeviceGEV = dynamic_cast<PvDeviceGEV*>( mDevice );
    if ( lDeviceGEV != NULL )
    {
        // If using a GigE Vision, it is same to assume the stream object is GigE Vision as well
        PvStreamGEV* lStreamGEV = static_cast<PvStreamGEV*>( mStream );

        // Have to set the Device IP destination to the Stream
        PvResult lResult = lDeviceGEV->SetStreamDestination( lStreamGEV->GetLocalIPAddress(), lStreamGEV->GetLocalPort() );
        if ( !lResult.IsOK() )
        {
            std::cout << "Setting stream destination failed" << std::endl;
            return false;
        }
    }

    // Enables stream before sending the AcquisitionStart command.
    mDevice->StreamEnable();

    // The pipeline is already "armed", we just have to tell the device to start sending us images
    PvResult lResult = mDevice->GetParameters()->ExecuteCommand( "AcquisitionStart" );
    if ( !lResult.IsOK() )
    {
        std::cout << "Unable to start acquisition" << std::endl;
        return false;
    }

    return true;
}

//
// Stops acquisition
//

bool JAI::StopAcquisition()
{
    std::cout << "--> StopAcquisition" << std::endl;

    // Tell the device to stop sending images.
    mDevice->GetParameters()->ExecuteCommand( "AcquisitionStop" );

    // Disable stream after sending the AcquisitionStop command.
    mDevice->StreamDisable();

    PvDeviceGEV* lDeviceGEV = dynamic_cast<PvDeviceGEV*>( mDevice );
    if ( lDeviceGEV != NULL )
    {
        // Reset streaming destination (optional...)
        lDeviceGEV->ResetStreamDestination();
    }

    return true;
}


//
// \brief Disconnects the device
//

void JAI::DisconnectDevice()
{
    std::cout << "--> DisconnectDevice" << std::endl;

    if ( mDevice != NULL )
    {
        if ( mDevice->IsConnected() )
        {
            // Unregister event sink (call-backs).
            mDevice->UnregisterEventSink( this );
        }

        PvDevice::Free( mDevice );
        mDevice = NULL;
    }
}

void JAI::TearDown( bool aStopAcquisition )
{
    std::cout << "--> TearDown" << std::endl;

    if ( aStopAcquisition )
    {
        StopAcquisition();
    }

    CloseStream();
    DisconnectDevice();
}

void JAI::ApplicationLoop()
{
    std::cout << "--> ApplicationLoop" << std::endl;
    char  lDoodle[] = "|\\-|-/";
    int lDoodleIndex = 0;

    bool lFirstTimeout = true;

    int64_t lImageCountVal = 0;
    double lFrameRateVal = 0.0;
    double lBandwidthVal = 0.0;

    // Acquire images until the user instructs us to stop.
    while ( flag )
    {
        // If connection flag is up, teardown device/stream
        if ( mConnectionLost && ( mDevice != NULL ) )
        {
            // Device lost: no need to stop acquisition
            TearDown( false );
        }

        // If the device is not connected, attempt reconnection
        if ( mDevice == NULL )
        {
            if ( ConnectDevice() )
            {
                // Device is connected, open the stream
                if ( OpenStream() )
                {
                    // Device is connected, stream is opened: start acquisition
                    if ( !StartAcquisition() )
                    {
                        TearDown( false );
                    }
                }
                else
                {
                    TearDown( false );
                }
            }
        }

        // If still no device, no need to continue the loop
        if ( mDevice == NULL )
        {
            continue;
        }

        if ( ( mStream != NULL ) && mStream->IsOpen() &&
             ( mPipeline != NULL ) && mPipeline->IsStarted() )
        {
            // Retrieve next buffer
            PvBuffer *lBuffer = NULL;
            PvResult  lOperationResult;
            PvResult lResult = mPipeline->RetrieveNextBuffer( &lBuffer, 1000, &lOperationResult );

            if ( lResult.IsOK() )
            {
                if (lOperationResult.IsOK())
                {
                    //
                    // We now have a valid buffer. This is where you would typically process the buffer.
                    // -----------------------------------------------------------------------------------------
                    // ...

                    // If the buffer contains an image, display width and height.
                    uint32_t lWidth = 0, lHeight = 0;
                    if ( lBuffer->GetPayloadType() == PvPayloadTypeImage )
                    {
                        // Get image specific buffer interface.
                        PvImage *lImage = lBuffer->GetImage();

                        // Read width, height.
                        lWidth = lImage->GetWidth();
                        lHeight = lImage->GetHeight();
                        /*
                        unsigned char* data = lImage->GetDataPointer();
                        cv::Mat aMat(1, lWidth * lHeight, CV_8UC1);
                        for (int i = 0; i < lWidth * lHeight; i++)
                        {
                            aMat.ptr<uchar>(0)[i] = data[i];
                        }
                        aMat = aMat.reshape(0, lHeight);
                        //cv::Mat aMat(lWidth, lHeight, CV_8UC1, data);
                        string path = to_string(flag++) + ".bmp";

                        cv::imwrite(path, aMat);
                        */
                    }

                    std::cout << fixed << setprecision( 1 );
                    std::cout << lDoodle[ lDoodleIndex ];
                    std::cout << " BlockID: " << uppercase << hex << setfill('0') << setw(16) << lBuffer->GetBlockID() << " W: " << dec << lWidth << " H: "
                         << lHeight << " " << lFrameRateVal << " FPS " << ( lBandwidthVal / 1000000.0 ) << " Mb/s  \r";

                    lFirstTimeout = true;
                }
                // We have an image - do some processing (...) and VERY IMPORTANT,
                // release the buffer back to the pipeline.
                mPipeline->ReleaseBuffer( lBuffer );
            }
            else
            {
                // Timeout

                if ( lFirstTimeout )
                {
                    std::cout << "" << std::endl;
                    lFirstTimeout = false;
                }

                std::cout << "Image timeout " << lDoodle[ lDoodleIndex ] << std::endl;
            }

            ++lDoodleIndex %= 6;
        }
        else
        {
            // No stream/pipeline, must be in recovery. Wait a bit...
            PvSleepMs( 100 );
        }
    }

    PvGetChar(); // Flush key buffer for next stop.
    std::cout << "" << std::endl;
}


bool JAI::PvSelectDeviceC( PvString *aConnectionID )
{
    PvResult lResult;
    const PvDeviceInfo *lSelectedDI = NULL;
    PvSystem lSystem;
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

//
// PvDeviceEventSink callback
//
// Notification that the device just got disconnected.
//

void JAI::OnLinkDisconnected( PvDevice *aDevice )
{
    std::cout << "=====> PvDeviceEventSink::OnLinkDisconnected callback" << std::endl;
    // Just set flag indicating we lost the device. The main loop will tear down the
    // device/stream and attempt reconnection.
    mConnectionLost = true;
    // IMPORTANT:
    // The PvDevice MUST NOT be explicitly disconnected from this callback.
    // Here we just raise a flag that we lost the device and let the main loop
    // of the application (from the main application thread) perform the
    // disconnect.
    //
}

void JAI::OnBufferReady(PvPipeline* aPipeline)
{
    std::cout << "--> ApplicationLoop" << std::endl;
    char  lDoodle[] = "|\\-|-/";
    int lDoodleIndex = 0;

    bool lFirstTimeout = true;

    int64_t lImageCountVal = 0;
    double lFrameRateVal = 0.0;
    double lBandwidthVal = 0.0;

        // Retrieve next buffer
        PvBuffer *lBuffer = NULL;
        PvResult  lOperationResult;
        PvResult lResult = aPipeline->RetrieveNextBuffer( &lBuffer, 1000, &lOperationResult );

        if ( lResult.IsOK() )
        {
            if (lOperationResult.IsOK())
            {
                //
                // We now have a valid buffer. This is where you would typically process the buffer.
                // -----------------------------------------------------------------------------------------
                // ...

                // If the buffer contains an image, display width and height.
                uint32_t lWidth = 0, lHeight = 0;
                if ( lBuffer->GetPayloadType() == PvPayloadTypeImage )
                {
                    // Get image specific buffer interface.
                    PvImage *lImage = lBuffer->GetImage();

                    // Read width, height.
                    lWidth = lImage->GetWidth();
                    lHeight = lImage->GetHeight();

                    unsigned char* data = lImage->GetDataPointer();
                    cv::Mat aMat(1, lWidth * lHeight, CV_8UC1);
                    for (int i = 0; i < lWidth * lHeight; i++)
                    {
                        aMat.ptr<uchar>(0)[i] = data[i];
                    }
                    aMat = aMat.reshape(0, lHeight);
                    images.push_back(aMat);                    //cv::Mat aMat(lWidth, lHeight, CV_8UC1, data);

                    //string path = to_string(index++) + ".bmp";

                    //cv::imwrite(path, aMat);

                }

                std::cout << fixed << setprecision( 1 );
                std::cout << lDoodle[ lDoodleIndex ];
                std::cout << " BlockID: " << uppercase << hex << setfill('0') << setw(16) << lBuffer->GetBlockID() << " W: " << dec << lWidth << " H: "
                     << lHeight << " " << lFrameRateVal << " FPS " << ( lBandwidthVal / 1000000.0 ) << " Mb/s  \r";

                lFirstTimeout = true;
            }
            // We have an image - do some processing (...) and VERY IMPORTANT,
            // release the buffer back to the pipeline.
            mPipeline->ReleaseBuffer( lBuffer );
        }

}


