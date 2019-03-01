/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef TARGET_NRF51822 /* DFU only supported on nrf51 platforms */

#ifndef __BLE_DFU_SERVICE_H__
#define __BLE_DFU_SERVICE_H__

#if BLE_FEATURE_GATT_SERVER

#include "ble/BLE.h"
#include "ble/UUID.h"

extern "C" {
#include "dfu_app_handler.h"
}

extern const uint8_t  DFUServiceBaseUUID[];
extern const uint16_t DFUServiceShortUUID;
extern const uint16_t DFUServiceControlCharacteristicShortUUID;

extern const uint8_t  DFUServiceUUID[];
extern const uint8_t  DFUServiceControlCharacteristicUUID[];
extern const uint8_t  DFUServicePacketCharacteristicUUID[];

/**
* @class DFUService
* @brief Device Firmware Update Service.
*/
class DFUService {
public:
    /**
     * @brief Signature for the handover callback. The application may provide this
     * callback when setting up the DFU service. The callback is then
     * invoked before handing control over to the bootloader.
     */
    typedef void (*ResetPrepare_t)(void);

public:
    /**
    * @brief Adds Device Firmware Update Service to an existing BLE object.
    *
    * @param[ref] _ble
    *               BLE object for the underlying controller.
    * @param[in] _handoverCallback
    *                Application-specific handover callback.
    */
    DFUService(BLE &_ble, ResetPrepare_t _handoverCallback = NULL) :
        ble(_ble),
        controlPoint(DFUServiceControlCharacteristicUUID, controlBytes, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
        packet(DFUServicePacketCharacteristicUUID, packetBytes, SIZEOF_PACKET_BYTES, SIZEOF_PACKET_BYTES,
               GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE),
        controlBytes(),
        packetBytes() {
        static bool serviceAdded = false; /* We only add the DFU service once. */
        if (serviceAdded) {
            return;
        }

        /* Set an initial value for control bytes, so that the application's DFU service can
         * be distinguished from the real DFU service provided by the bootloader. */
        controlBytes[0] = 0xFF;
        controlBytes[1] = 0xFF;

        GattCharacteristic *dfuChars[] = {&controlPoint, &packet};
        GattService         dfuService(DFUServiceUUID, dfuChars, sizeof(dfuChars) / sizeof(GattCharacteristic *));

        ble.addService(dfuService);
        handoverCallback = _handoverCallback;
        serviceAdded     = true;

        ble.onDataWritten(this, &DFUService::onDataWritten);
    }

    /**
    * @brief Get the handle for the value attribute of the control characteristic.
    */
    uint16_t getControlHandle(void) const {
        return controlPoint.getValueHandle();
    }

    /**
     * @brief This callback allows the DFU service to receive the initial trigger to
     * hand control over to the bootloader. First, the application is given a
     * chance to clean up.
     *
     * @param[in] params
     *     Information about the characteristic being updated.
     */
    virtual void onDataWritten(const GattWriteCallbackParams *params) {
        if (params->handle == controlPoint.getValueHandle()) {
            /* At present, writing anything will do the trick - this needs to be improved. */
            if (handoverCallback) {
                handoverCallback();
            }

            // Call bootloader_start implicitly trough a event handler call
            // it is a work around for bootloader_start not being public in sdk 8.1
            ble_dfu_t p_dfu;
            ble_dfu_evt_t p_evt;

            p_dfu.conn_handle = params->connHandle;
            p_evt.ble_dfu_evt_type = BLE_DFU_START;

            dfu_app_on_dfu_evt(&p_dfu, &p_evt);
        }
    }

protected:
    static const unsigned SIZEOF_CONTROL_BYTES = 2;
    static const unsigned SIZEOF_PACKET_BYTES  = 20;

protected:
    BLE          &ble;

    /** Writing to the control characteristic triggers the handover to DFU
     *  bootloader. At present, writing anything will do the trick - this needs
     *  to be improved. */
    WriteOnlyArrayGattCharacteristic<uint8_t, SIZEOF_CONTROL_BYTES> controlPoint;

    /** The packet characteristic in this service doesn't do anything meaningful;
     *  it is only a placeholder to mimic the corresponding characteristic in the
     *  actual DFU service implemented by the bootloader. Without this, some
     *  FOTA clients might get confused, because service definitions change after
     *  handing control over to the bootloader. */
    GattCharacteristic  packet;

    uint8_t             controlBytes[SIZEOF_CONTROL_BYTES];
    uint8_t             packetBytes[SIZEOF_PACKET_BYTES];

    static ResetPrepare_t handoverCallback;  /**< Application-specific handover callback. */
};

#endif // BLE_FEATURE_GATT_SERVER

#endif /* #ifndef __BLE_DFU_SERVICE_H__*/
#endif /* #ifdef TARGET_NRF51822 */
