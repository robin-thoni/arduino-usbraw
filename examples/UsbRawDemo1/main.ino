#define ARD_UTILS_DELAYMS
#include "ArdUtils/ArdUtils.h"

#include "UsbRaw.h"

#define ledPin 13

uchar buffer[50];

void callback(CALLBACK_ARGS)
{
    /*digitalWrite(ledPin, !digitalRead(ledPin));
    delay(100);*/

    /*if (data)
    {
        strcpy((char *) buffer, (const char *) data);

        digitalWrite(ledPin, !digitalRead(ledPin));
        delay(200);
        digitalWrite(ledPin, !digitalRead(ledPin));
    }*/

    if (rq->bRequest == 0)
    {
        digitalWrite(ledPin, 0);
    }
    else if (rq->bRequest == 1)
    {
        digitalWrite(ledPin, 1);
    }
    else if (rq->bRequest == 4)
    {
        if (data)
        {
            digitalWrite(ledPin, !digitalRead(ledPin));
            delay(200);
            digitalWrite(ledPin, !digitalRead(ledPin));
            strcpy((char *) buffer, (const char *) data);
        }
        else
        {
            dev->setDataUsbNoMsg();
        }
    }
    else
    {
        dev->setData(buffer, strlen((const char*)buffer) + 1);
    }
}

void setup()
{
    strcpy((char*)buffer, "test");
    UsbRaw.init();
    UsbRaw.setCallback(callback);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
}

void loop()
{
    UsbRaw.poll();
    //WAIT_USB;

    //UsbRawHid.sendData("test", 4);

    //ArdUtils::delayMs(1000);
    //digitalWrite(ledPin, !digitalRead(ledPin));
}
