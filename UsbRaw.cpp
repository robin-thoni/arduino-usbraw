//
// Created by robin on 1/8/16.
//

#include "UsbRaw.h"

UsbRawDevice UsbRaw = UsbRawDevice();

UsbRawDevice::UsbRawDevice()
    : _dataSend(0),
      _dataSendLen(0),
      _callback(0),
      _dataReceivePos(0),
      _dataReceiveLen(0)
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
    setData((const uchar*)data, strlen(data) + 1);
}

void UsbRawDevice::setDataUsbNoMsg()
{
    setData(0, USB_NO_MSG);
}

usbMsgLen_t UsbRawDevice::_usbFunctionSetup(usbRequest_t* rq)
{
    _rq = *rq;
    _dataReceiveLen = min((uchar)rq->wLength.word, USB_RAW_DEVICE_BUFFER_SIZE);
    _dataReceivePos = 0;
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
    for (uchar i = 0; i < len && _dataReceivePos < _dataReceiveLen; ++i, ++_dataReceivePos)
    {
        _dataReceive[_dataReceivePos] = data[i];
    }
    if (_dataReceiveLen == _dataReceivePos)
    {
        if (_callback)
        {
            _callback(&_rq, this, _dataReceive, _dataReceiveLen);
        }
        return 1;
    }
    return 0;
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