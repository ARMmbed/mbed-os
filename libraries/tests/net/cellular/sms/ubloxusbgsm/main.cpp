#include "UbloxUSBGSMModem.h"
#include "smstest.h"

void test(const void* data)
{
    UbloxUSBGSMModem modem;

    smstest(modem);
    while (true);
}

int main()
{
    Thread testTask(test, NULL, osPriorityNormal, 1024 * 4);
    DigitalOut led(LED1);

    while (true)
    {
        led = !led;
        Thread::wait(1000);  
    }
    return 0;
}

