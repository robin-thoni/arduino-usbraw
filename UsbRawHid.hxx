//
// Created by robin on 1/8/16.
//

UsbRawHidDevice UsbRawHid = UsbRawHidDevice();


/* We use a simplifed keyboard report descriptor which does not support the
 * boot protocol. We don't allow setting status LEDs and but we do allow
 * simultaneous key presses.
 * The report descriptor has been created with usb.org's "HID Descriptor Tool"
 * which can be downloaded from http://www.usb.org/developers/hidpage/.
 * Redundant entries (such as LOGICAL_MINIMUM and USAGE_PAGE) have been omitted
 * for the second INPUT item.
 */
PROGMEM const char usbHidReportDescriptor[32] = {
        0x06, 0x42, 0xff,
        0x0A, 0x42, 0xff,
        0xA1, 0x01,				// Collection 0x01
        0x75, 0x08,				// report size = 8 bits
        0x15, 0x00,				// logical minimum = 0
        0x26, 0xFF, 0x00,			// logical maximum = 255
        0x95, RAWHID_TX_SIZE,			// report count
        0x09, 0x01,				// usage
        0x81, 0x02,				// Input (array)
        0x95, RAWHID_RX_SIZE,			// report count
        0x09, 0x02,				// usage
        0x91, 0x02,				// Output (array)
        0xC0					// end collection
};

UsbRawHidDevice::UsbRawHidDevice()
{
    PORTD = 0; // TODO: Only for USB pins?
    DDRD |= ~USBMASK;

    cli();
    usbDeviceDisconnect();
    usbDeviceConnect();
    usbInit();
    sei();
    //sendData("", 0);
}

bool UsbRawHidDevice::isUsbReady()
{
    update();
    return usbInterruptIsReady();
}

void UsbRawHidDevice::sendData(const char* data, unsigned size)
{
    if (size > RAWHID_TX_SIZE || size == 0)
    {
        size = RAWHID_TX_SIZE;
    }
    memcpy(reportBuffer, data, size);
    memset(reportBuffer + size, 0, RAWHID_TX_SIZE - size);

    usbSetInterrupt((unsigned char*)(void *)&reportBuffer, sizeof(reportBuffer));
}

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t    *rq = (usbRequest_t*)(void*)data;

    /* The following requests are never used. But since they are required by
     * the specification, we implement them in this example.
     */
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* class request type */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* we only have one report type, so don't look at wValue */
            usbMsgPtr = (unsigned char*)(void *)&UsbRawHid.reportBuffer;
            return sizeof(UsbRawHid.reportBuffer);
        }else if(rq->bRequest == USBRQ_HID_GET_IDLE){
            usbMsgPtr = &idleRate;
            return 1;
        }else if(rq->bRequest == USBRQ_HID_SET_IDLE){
            idleRate = rq->wValue.bytes[1];
        }
    }else{
        /* no vendor specific requests implemented */
    }
    return 0;   /* default for not implemented requests: return no data back to host */
}