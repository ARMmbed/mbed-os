#include "AP3CordioHCIDriver.h"
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

using namespace ble::vendor::ambiq_micro;
using namespace ble::vendor::cordio;

AP3CordioHCIDriver::AP3CordioHCIDriver(cordio::CordioHCITransportDriver &transport_driver)
    : cordio::CordioHCIDriver(transport_driver)
{
    AP3CordioHCITransportDriver *p_trspt_drv = (AP3CordioHCITransportDriver *)&transport_driver;
    _ptr_to_handle = &p_trspt_drv->handle;
}

AP3CordioHCIDriver::~AP3CordioHCIDriver() {}

void AP3CordioHCIDriver::do_initialize()
{
    HciDrvRadioBoot(true);
}
void AP3CordioHCIDriver::do_terminate()
{
    HciDrvRadioShutdown();
}

ble::vendor::cordio::buf_pool_desc_t AP3CordioHCIDriver::get_buffer_pool_description()
{
    static union {
        uint8_t buffer[9000];
        uint64_t align;
    };
    static const wsfBufPoolDesc_t pool_desc[] = {
        {  16, 64 },
        {  32, 64 },
        {  64, 32 },
        { 128, 16 },
        { 272, 4 }
    };
    return buf_pool_desc_t(buffer, pool_desc);
}

ble::vendor::cordio::CordioHCIDriver &ble_cordio_get_hci_driver()
{
    static AP3CordioHCITransportDriver transport_driver;

    static AP3CordioHCIDriver hci_driver(transport_driver);

    return hci_driver;
}
