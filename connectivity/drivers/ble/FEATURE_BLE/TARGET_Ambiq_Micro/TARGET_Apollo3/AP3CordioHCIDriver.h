#ifndef APOLLO3_CORDIO_HCI_DRIVER_H_
#define APOLLO3_CORDIO_HCI_DRIVER_H_

#include "CordioHCIDriver.h"
#include "am_mcu_apollo.h"

namespace ble
{
    class AP3CordioHCIDriver : public CordioHCIDriver
    {
    public:
        AP3CordioHCIDriver(
            CordioHCITransportDriver &transport_driver
            /* specific constructor arguments*/);

        virtual ~AP3CordioHCIDriver();

        virtual void do_initialize();

        virtual void do_terminate();

        virtual ble::buf_pool_desc_t get_buffer_pool_description();

    private:
        void **_ptr_to_handle;
        am_hal_ble_config_t _ble_config;
    };
} // namespace ble

#endif /* APOLLO3_CORDIO_HCI_TRANSPORT_DRIVER_H_ */
