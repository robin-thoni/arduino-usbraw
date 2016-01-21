/*
 * Based on Obdev's AVRUSB code and under the same license.
 * Modified by Robin Thoni <robin@rthoni.com>
 */
#ifndef __UsbRaw_h__
#define __UsbRaw_h__

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#include "usbdrv.h"

#define CALLBACK_ARGS usbRequest_t* rq, UsbRawDevice* dev, uchar* data, uchar len

#ifndef USB_RAW_DEVICE_BUFFER_SIZE
#define USB_RAW_DEVICE_BUFFER_SIZE 32
#endif

class UsbRawDevice {
public:
    UsbRawDevice();

    void init();
    
    void poll();

    void setData(const uchar* data, uchar len);

    void setDataString(const char* data);

    void setDataUsbNoMsg();

    usbMsgLen_t _usbFunctionSetup(usbRequest_t* rq);

    uchar _usbFunctionWrite(uchar *data, uchar len);

    void setCallback(void (*callback)(CALLBACK_ARGS));

private:

    const uchar* _dataSend;

    uchar _dataSendLen;

    void (*_callback)(CALLBACK_ARGS);

    usbRequest_t _rq;

    uchar _dataReceive[USB_RAW_DEVICE_BUFFER_SIZE];

    uchar _dataReceivePos;

    uchar _dataReceiveLen;
};

extern UsbRawDevice UsbRaw;

#endif // __UsbRaw_h__
