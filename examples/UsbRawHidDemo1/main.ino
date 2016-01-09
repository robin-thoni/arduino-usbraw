#define ARD_UTILS_DELAYMS
#include "ArdUtils/ArdUtils.h"

#include "UsbRawHid.h"

#define ledPin 13

void setup()
{
    pinMode (ledPin, OUTPUT);
    digitalWrite (ledPin, HIGH);
}

void loop()
{
    WAIT_USB;


    ArdUtils::delayMs(1000);
    digitalWrite(ledPin, !digitalRead(ledPin));
}
