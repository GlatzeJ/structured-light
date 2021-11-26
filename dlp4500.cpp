#include "dlp4500.h"

DLP4500::DLP4500()
{
    DlpTime = 70;
    numPatterns = 0;
}

DLP4500::~DLP4500()
{
}

bool DLP4500::Connect()
{
    DLPC350_USB_Init();
    if(DLPC350_USB_Open() != 0)
    {
        return false;
    }
    return true;
}


bool DLP4500::IsConnect()
{
    return DLPC350_USB_IsConnected();
}

void DLP4500::setPatternMode()
{
    int trigMode = 0;
    bool isExtPatDisplayMode = false;

    SetDLPC350InPatternMode();
}

void DLP4500::SetDLPC350InPatternMode()
{
    int i = 0;
    bool mode;
    unsigned int patMode;

    //Check if it is in Pattern Mode
    DLPC350_GetMode(&mode);
    if(mode == false)
    {
        //Switch to Pattern Mode
        DLPC350_SetMode(true);
        SleeperThread::msleep(100);
        while(1)
        {
            DLPC350_GetMode(&mode);
            if(mode)
                break;
            SleeperThread::msleep(100);
            if(i++ > MAX_NUM_RETRIES)
                break;
        }
    }
    else
    {
        //First stop pattern sequence
        DLPC350_GetPatternDisplay(&patMode);
        //if it is in PAUSE or RUN mode
        if(patMode != 0)
        {
            //emit on_pushButton_PatSeqCtrlStop_clicked();
        }
    }

    return;
}

void DLP4500::projectMulti()
{
    DLPC350_ClearPatLut();
    if (DLPC350_AddToPatLut(0, 0, 8, 7, 0, 0, 1, 0) < 0)
    {
        ShowError("error Updating LUT");
    }
    if (DLPC350_AddToPatLut(0, 1, 8, 7, 0, 1, 0, 0) < 0)
    {
        ShowError("error Updating LUT");
    }
    if (DLPC350_AddToPatLut(0, 2, 8, 7, 0, 1, 0, 0) < 0)
    {
        ShowError("error Updating LUT");
    }
    if (DLPC350_AddToPatLut(0, 3, 8, 7, 0, 1, 0, 0) < 0)
    {
        ShowError("error Updating LUT");
    }
    if (DLPC350_AddToPatLut(0, 4, 8, 7, 0, 1, 0, 0) < 0)
    {
        ShowError("error Updating LUT");
    }

    if (DLPC350_SetPatternDisplayMode(0)<0)
    {
        ShowError("error set from flash");
    }
    //三频四相 ＋ 黑 + 白
    numPatterns = 14;
    if (DLPC350_SetPatternConfig(10, false, numPatterns, 2) < 0)
    {
        ShowError("error Sending Pattern Config");
    }

    if(DLPC350_SetExposure_FramePeriod(DlpTime, DlpTime))
    {
        ShowError("error Sending Exposure period");
    }
    //Configure Trigger Mode - 0(External) or 1(internal)
    if (DLPC350_SetPatternTriggerMode(1) < 0)
    {
        ShowError("error Sending trigger Mode");
    }
    //Send Pattern LUT
    if (DLPC350_SendPatLut() < 0)
    {
        ShowError("error Sending Pattern LUT");
    }
    unsigned char splashLut[2];
    splashLut[0] = 0x03;
    splashLut[1] = 0x04;

    if (DLPC350_SendImageLut(&splashLut[0], 2) < 0)
    {
        ShowError("error Sending Image LUT");
    }
    /************************开始投影**********************************/
    if (DLPC350_StartPatLutValidate() < 0)
    {
        ShowError("error check LUT data");
    }
    int i = 0;
    unsigned int status;
    bool ready;

    QEventLoop loop;

    do
    {
        if (DLPC350_CheckPatLutValidate(&ready, &status) < 0)
        {
            ShowError("error validating LUT data");
        }

        if (ready)
        {
            break;
        }
        else
        {
            QTimer::singleShot(1000, &loop, SLOT(quit()));
            loop.exec();
        }

        if (i++ > MAX_NUM_RETRIES)
            break;
    } while (1);
    /*
    if (status != 0)
    {
        ShowError("Sequence validation FAILED!");
    }

    if ((status & BIT0) == BIT0)
    {
        ShowError("Exposure or frame period OUT OF RANGE");
    }
    if ((status & BIT1) == BIT1)
    {
        ShowError("Pattern number in lookup table INVALID");
    }
    if ((status & BIT2) == BIT2)
    {
        ShowError("Continued output trigger OVERLAPS black vector");
    }
    if ((status & BIT3) == BIT3)
    {
        ShowError("Black vector MISSING when exposure less than frame period");
    }
    if ((status & BIT4) == BIT4)
    {
        ShowError("Difference between exposure and frame period less than 230us");
    }
    */

    if (DLPC350_PatternDisplay(2) < 0)
    {
        ShowError("error play");
    }
}

void DLP4500::ShowError(const char *str)
{
    std::cout << str << std::endl;
}


void DLP4500::test()
{
    //初始化
    DLPC350_USB_Init();
    DLPC350_USB_Open();
    if (!DLPC350_USB_IsConnected()) {
        return ;
    }

    //显示设置
    DLPC350_SetPowerMode(false);
    DLPC350_SetMode(true);
    DLPC350_PatternDisplay(0);
    DLPC350_SetPatternDisplayMode(false);

    //图片序列设置
    const int numberOfPatterns = 3;
    const int numberofImages = 4;
    unsigned char imageIndices[numberOfPatterns*numberofImages];
    int countr = 0;
    for (int j = 0; j < numberofImages; j++) {
        for (int i = 0; i < numberOfPatterns; i++) {
            imageIndices[countr] = j;
            DLPC350_AddToPatLut(0, i, 8, 7, false, false, true, false);
            /*
            if (i == 0) {
                DLPC350_AddToPatLut(0, i, 8, 7, false, false, false, false);
            }
            else {
                DLPC350_AddToPatLut(0, i, 8, 7, false, false, true, false);
            }
            */

            countr++;
        }
    }
    DLPC350_SendImageLut(imageIndices, countr);
    DLPC350_SendPatLut();
    DLPC350_SetPatternConfig(numberOfPatterns*numberofImages, false, numberOfPatterns*numberofImages, numberOfPatterns*numberofImages);

    DLPC350_SetLongAxisImageFlip(false);
    DLPC350_SetShortAxisImageFlip(true);


    //曝光设置
    DLPC350_ClearPatLut();
    DLPC350_ClearExpLut();
    DLPC350_SetPatternTriggerMode(1);
    DLPC350_SetExposure_FramePeriod(500000, 500000);

    //设置触发输出延时
    DLPC350_SetTrigIn1Delay(1);
    DLPC350_SetTrigIn2Pol(false);//rising
    /*
        DLPC350_SetTrigOutConfig(unsigned int trigOutNum,bool invert,unsigned int rising,unsigned int falliing)
        trigOutNum:1=TRIG_OUT_1;2=TRIG_OUT_2
        invert:0=active high signal,1=active low signal
        rising:rising edge delay control.each bit adds 101.2ns 0xBB=0.00us
        falling:falling edge delay control.

        return >=0 = pass
        */
    DLPC350_SetTrigOutConfig(1, false, 187, 187);
    DLPC350_SetTrigOutConfig(2, false, 187, 187);

    DLPC350_SetRedLEDStrobeDelay(187, 187);
    DLPC350_SetGreenLEDStrobeDelay(187, 187);
    DLPC350_SetBlueLEDStrobeDelay(187, 187);

    DLPC350_SetLedEnables(true, false, false, false);
    DLPC350_SetLEDPWMInvert(false);
    DLPC350_SetLedCurrents(127, 127, 127);
    DLPC350_SetLedEnables(true, true, true, true);

    unsigned int status;
    DLPC350_ValidatePatLutData(&status);

    DLPC350_PatternDisplay(2);

    std::cout << "ok..." << std::endl;

}

