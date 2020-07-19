#include "USBSerial.h"

FileHandle* mbed::mbed_override_console(int)
{
    static USBSerial  usbSerialConsole;
    return &usbSerialConsole;
}