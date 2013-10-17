#include "UBloxUSBGSMModem.h"
#include "test_env.h"
#include "httptest.h"

#ifndef MODEM_APN
#warning APN not specified, using "internet"
#define APN "internet"
#endif

#ifndef MODEM_USERNAME
#warning username not specified
#define USERNAME NULL
#endif

#ifndef MODEM_PASSWORD
#warning password not specified
#define PASSWORD NULL
#endif

int main()
{
    UbloxUSBGSMModem modem;

    notify_completion(httptest(modem, APN, USERNAME, PASSWORD));
}
