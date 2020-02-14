#include "AP3CordioHCITransportDriver.h"
#include "am_mcu_apollo.h"

using namespace ble::vendor::ambiq_micro;
using namespace ble::vendor::cordio;
//AP3CordioHCITransportDriver::AP3CordioHCITransportDriver(/* specific constructor arguments*/) {}

AP3CordioHCITransportDriver::~AP3CordioHCITransportDriver() {}

void AP3CordioHCITransportDriver::initialize()
{
    //getting a grip on our handle
    am_hal_ble_initialize(0, &handle);
}

void AP3CordioHCITransportDriver::terminate()
{
    am_hal_ble_deinitialize(handle);
    handle = NULL;
}

uint16_t AP3CordioHCITransportDriver::write(uint8_t packet_type, uint16_t len, uint8_t *data)
{
    if (handle)
    {
        return (uint16_t)am_hal_ble_blocking_hci_write(handle, packet_type, (uint32_t *)data, (uint16_t)len);
    }
    return 0;
}

// Callback from Cordio stack
extern "C" uint16_t ControllerToHostWrite(uint8_t prot, uint8_t hci_type, uint16_t len, uint8_t *pData)
{
    CordioHCITransportDriver::on_data_received(&hci_type, 1);
    CordioHCITransportDriver::on_data_received(pData, len);
    return len;
}
