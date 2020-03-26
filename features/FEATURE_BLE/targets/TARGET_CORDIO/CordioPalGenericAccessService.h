#ifndef CORDIO_PAL_GENERIC_ACCESS_SERVICE_
#define CORDIO_PAL_GENERIC_ACCESS_SERVICE_

#include "ble/pal/GenericAccessService.h"
#include "CordioGattServer.h"


namespace ble {
namespace pal {
namespace vendor {
namespace cordio {

/**
 * Implementation of ble::pal::GenericAccessService for the Cordio stack.
 */
class GenericAccessService : public ::ble::pal::GenericAccessService {
public:
    GenericAccessService() { }

    virtual ~GenericAccessService() { }

    virtual ble_error_t get_peripheral_prefered_connection_parameters(
        ::Gap::ConnectionParams_t& parameters
    ) {
#if BLE_FEATURE_GATT_SERVER
        parameters = gatt_server().getPreferredConnectionParams();
        return BLE_ERROR_NONE;
#else
        return BLE_ERROR_NOT_IMPLEMENTED;
#endif // BLE_FEATURE_GATT_SERVER
    }

    virtual ble_error_t set_peripheral_prefered_connection_parameters(
        const ::Gap::ConnectionParams_t& parameters
    ) {
#if BLE_FEATURE_GATT_SERVER
        gatt_server().setPreferredConnectionParams(parameters);
        return BLE_ERROR_NONE;
#else
        return BLE_ERROR_NOT_IMPLEMENTED;
#endif // BLE_FEATURE_GATT_SERVER
    }

private:
#if BLE_FEATURE_GATT_SERVER
    ble::vendor::cordio::GattServer& gatt_server() {
        return ble::vendor::cordio::GattServer::getInstance();
    }
#endif // BLE_FEATURE_GATT_SERVER
};

} // cordio
} // vendor
} // pal
} // ble

#endif /* CORDIO_PAL_GENERIC_ACCESS_SERVICE_ */
