#include "UBloxUSBGSMModem.h"
#include "httptest.h"

#ifndef MODEM_APN
#warning APN not specified, using "internet"
#define MODEM_APN "internet"
#endif

#ifndef MODEM_USERNAME
#warning username not specified
#define MODEM_USERNAME NULL
#endif

#ifndef MODEM_PASSWORD
#warning password not specified
#define MODEM_PASSWORD NULL
#endif

void test(void const* data)
{
    UbloxUSBGSMModem modem;
    httptest(modem, MODEM_APN, MODEM_USERNAME, MODEM_PASSWORD);
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

