#define ARD_UTILS_DELAYMS
#include "ArdUtils/ArdUtils.h"

#define RAWHID_TX_SIZE       64
#define RAWHID_RX_SIZE       64
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

    //UsbRawHid.sendData("test", 4);

    ArdUtils::delayMs(1000);
    digitalWrite(ledPin, !digitalRead(ledPin));
}
