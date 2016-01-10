/*
 * Based on Obdev's AVRUSB code and under the same license.
 * Modified by Robin Thoni <robin@rthoni.com>
 */
#ifndef __UsbKeyboard_h__
#define __UsbKeyboard_h__

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>

#include "usbdrv.h"

// TODO: Work around Arduino 12 issues better.
//#include <WConstants.h>
//#undef int()

typedef uint8_t byte;

#define WAIT_USB do {           \
  UsbRawHid.update();           \
  if (!usbInterruptIsReady())   \
  {                             \
    return;                     \
  }                             \
} while(0)

#define BUFFER_SIZE 64


static uchar    idleRate;/* repeat rate for keyboards, never used for mice */

class UsbRawHidDevice {
public:
    UsbRawHidDevice();

    bool isUsbReady();
    
    void update() {
        usbPoll();
    }

    void sendData(const char* data, unsigned size = 0);
    
    //private: TODO: Make friend?
    byte reportBuffer[BUFFER_SIZE];    // buffer for HID reports [ 1 modifier byte + (len-1) key strokes]

private:

};

#include "UsbRawHid.hxx"

#endif // __UsbKeyboard_h__
