//
// Created by robin on 1/8/16.
//

#include "UsbRaw.h"

UsbRawDevice UsbRaw = UsbRawDevice();

UsbRawDevice::UsbRawDevice()
    : _dataSend(0),
      _dataSendLen(0),
      _callback(0),
      _dataReceived(0),
      _dataReceivedLen(0)
{
}

void UsbRawDevice::init()
{
    PORTD = 0; // TODO: Only for USB pins?
    DDRD |= ~USBMASK;

    cli();
    usbDeviceDisconnect();
    usbDeviceConnect();
    usbInit();
    sei();
}

void UsbRawDevice::poll()
{
    usbPoll();
}

void UsbRawDevice::setData(const uchar* data, uchar len)
{
    _dataSend = data;
    _dataSendLen = len;
}

void UsbRawDevice::setDataString(const char* data)
{
    setData((const uchar*)data, strlen(data));
}

usbMsgLen_t UsbRawDevice::_usbFunctionSetup(usbRequest_t* rq)
{
    _rq = rq;
    if (_callback)
    {
        _callback(rq, this, 0, 0);
    }
    usbMsgPtr = (uchar*)_dataSend;
    uchar len = _dataSendLen;
    setData(0, 0);
    return len;
}

uchar UsbRawDevice::_usbFunctionWrite(uchar *data, uchar len)
{
    free(_dataReceived);
    _dataReceived = (uchar*)memcpy(malloc(len), data, _dataReceivedLen = len);
    if (_callback)
    {
        _callback(_rq, this, _dataReceived, len);
    }
    return 1;
}

void UsbRawDevice::setCallback(void (*callback)(CALLBACK_ARGS))
{
    _callback = callback;
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
    return UsbRaw._usbFunctionWrite(data, len);
}

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    return UsbRaw._usbFunctionSetup((usbRequest_t*)(void*)data);
}