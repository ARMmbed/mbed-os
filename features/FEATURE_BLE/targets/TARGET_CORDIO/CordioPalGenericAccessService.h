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

    virtual ble_error_t get_device_name_length(uint8_t& length) {
        const uint8_t* name = NULL;
        uint16_t actual_length = 0;

        gatt_server().getDeviceName(name, actual_length);
        length = actual_length;
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t get_device_name(ArrayView<uint8_t>& array) {
        const uint8_t* name = NULL;
        uint16_t length = 0;

        gatt_server().getDeviceName(name, length);

        if (length > array.size()) {
            return BLE_ERROR_PARAM_OUT_OF_RANGE;
        }

        memcpy(array.data(), name, length);
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t set_device_name(const uint8_t* device_name) {
        return gatt_server().setDeviceName(device_name);
    }

    virtual ble_error_t get_appearance(
        GapAdvertisingData::Appearance& appearance
    ) {
        appearance = gatt_server().getAppearance();
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t set_appearance(
        GapAdvertisingData::Appearance appearance
    ) {
        gatt_server().setAppearance(appearance);
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t get_peripheral_prefered_connection_parameters(
        ::Gap::ConnectionParams_t& parameters
    ) {
        parameters = gatt_server().getPreferredConnectionParams();
        return BLE_ERROR_NONE;
    }

    virtual ble_error_t set_peripheral_prefered_connection_parameters(
        const ::Gap::ConnectionParams_t& parameters
    ) {
        gatt_server().setPreferredConnectionParams(parameters);
        return BLE_ERROR_NONE;
    }

private:
    ble::vendor::cordio::GattServer& gatt_server() {
        return ble::vendor::cordio::GattServer::getInstance();
    }
};

} // cordio
} // vendor
} // pal
} // ble

#endif /* CORDIO_PAL_GENERIC_ACCESS_SERVICE_ */
