#ifndef APOLLO3_CORDIO_HCI_DRIVER_H_
#define APOLLO3_CORDIO_HCI_DRIVER_H_

#include "CordioHCIDriver.h"
#include "am_mcu_apollo.h"

namespace ble
{
namespace vendor
{
namespace ambiq_micro
{

class AP3CordioHCIDriver : public cordio::CordioHCIDriver
{
public:
    AP3CordioHCIDriver(
        cordio::CordioHCITransportDriver &transport_driver
        /* specific constructor arguments*/);

    virtual ~AP3CordioHCIDriver();

    virtual void do_initialize();

    virtual void do_terminate();

    virtual ble::vendor::cordio::buf_pool_desc_t get_buffer_pool_description();

    // virtual void start_reset_sequence();

    // virtual void handle_reset_sequence(uint8_t *msg);

private:
    void **_ptr_to_handle;
    am_hal_ble_config_t _ble_config;
    //AP3CordioHCITransportDriver *_transport_driver;
    // private driver declarations
};

} // namespace ambiq_micro
} // namespace vendor
} // namespace ble

#endif /* APOLLO3_CORDIO_HCI_TRANSPORT_DRIVER_H_ */
