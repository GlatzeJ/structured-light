#ifndef DLP4500_H
#define DLP4500_H

#include "DLPControl/dlpc350_common.h"
#include "DLPControl/dlpc350_firmware.h"
#include "DLPControl/hidapi.h"
#include "DLPControl/dlpc350_error.h"
#include "DLPControl/dlpc350_usb.h"
#include "DLPControl/dlpc350_api.h"
#include "DLPControl/dlpc350_flashDevice.h"
#include "DLPControl/dlpc350_BMPParser.h"
#include "DLPControl/dlpc350_version.h"

#include <QThread>
#include <QTimer>
#include <QEventLoop>
#include <iostream>


#define MAX_NUM_RETRIES 5

class SleeperThread : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};


class DLP4500
{
public:
    DLP4500();
    ~DLP4500();

    bool Connect();
    bool IsConnect();
    void setPatternMode();
    void SetDLPC350InPatternMode();
    void projectMulti();
    void test();
    void ShowError(const char *str);

    unsigned int DlpTime; //投影间隔时间
    int numPatterns;
};

#endif // DLP4500_H
