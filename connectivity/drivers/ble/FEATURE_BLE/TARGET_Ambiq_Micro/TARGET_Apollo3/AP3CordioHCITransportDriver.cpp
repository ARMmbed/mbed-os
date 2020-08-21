#include "AP3CordioHCITransportDriver.h"
#include "am_mcu_apollo.h"
#include "stdio.h"
#include <cstring>

#include "wsf_types.h"
#include "wsf_timer.h"
#include "bstream.h"
#include "wsf_msg.h"
#include "wsf_cs.h"

#include "hci_drv_apollo3.h"

#define PRINT_DEBUG_HCI 0

#if PRINT_DEBUG_HCI
#include "mbed.h"
DigitalOut debugGPIO(D28, 0);
DigitalOut debugGPIO2(D25, 0);
#endif

using namespace ble;

AP3CordioHCITransportDriver::~AP3CordioHCITransportDriver() {}

void AP3CordioHCITransportDriver::initialize()
{
    wsfHandlerId_t handlerId = WsfOsSetNextHandler(HciDrvHandler);
    HciDrvHandlerInit(handlerId);
}

void AP3CordioHCITransportDriver::terminate()
{
}

uint16_t AP3CordioHCITransportDriver::write(uint8_t packet_type, uint16_t len, uint8_t *data)
{
#if PRINT_DEBUG_HCI
    printf("sent tx packet_type: %02X data: ", packet_type);
    for (int i = 0; i < len; i++)
    {
        printf(" %02X", data[i]);
    }
    printf("\r\n");
#endif

    //Temporary workaround, random address not working, suppress it.
    if (data[0] == 0x06 && data[1] == 0x20)
    {
#if PRINT_DEBUG_HCI
        printf("LE Set Advertising Params\r\n");
#endif
        data[8] = 0;
    }
    return ap3_hciDrvWrite(packet_type, len, data);
}

//Ugly Mutlifile implementation
void CordioHCITransportDriver_on_data_received(uint8_t *data, uint16_t len)
{
#if PRINT_DEBUG_HCI
    printf("data rx: ");
    for (int i = 0; i < len; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\r\n");
#endif

    CordioHCITransportDriver::on_data_received(data, len);
}
