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

#ifndef __BLE_LINK_LOSS_SERVICE_H__
#define __BLE_LINK_LOSS_SERVICE_H__

#include "ble/Gap.h"

/**
* @class LinkLossService
* @brief This service defines behavior when a link is lost between two devices.
* Service:  https://developer.bluetooth.org/gatt/services/Pages/ServiceViewer.aspx?u=org.bluetooth.service.link_loss.xml
* Alertness Level Char: https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.alert_level.xml
*/
class LinkLossService {
public:
    enum AlertLevel_t {
        NO_ALERT   = 0,
        MILD_ALERT = 1,
        HIGH_ALERT = 2
    };

    typedef void (* callback_t)(AlertLevel_t level);

    /**
     * @param bleIn
     *               BLE object for the underlying controller.
     * @param callbackIn Callback invoked upon disconnection.
     * @param levelIn Alert level.
     */
    LinkLossService(BLE &bleIn, callback_t callbackIn, AlertLevel_t levelIn = NO_ALERT) :
        ble(bleIn),
        alertLevel(levelIn),
        callback(callbackIn),
        alertLevelChar(GattCharacteristic::UUID_ALERT_LEVEL_CHAR, reinterpret_cast<uint8_t *>(&alertLevel)) {
        static bool serviceAdded = false; /* We should only ever add one LinkLoss service. */
        if (serviceAdded) {
            return;
        }

        GattCharacteristic *charTable[] = {&alertLevelChar};
        GattService         linkLossService(GattService::UUID_LINK_LOSS_SERVICE, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));

        ble.gattServer().addService(linkLossService);
        serviceAdded = true;

        ble.gap().onDisconnection(this, &LinkLossService::onDisconnectionFilter);
        ble.gattServer().onDataWritten(this, &LinkLossService::onDataWritten);
    }

    /**
     * Update the callback.
     */
    void setCallback(callback_t newCallback) {
        callback = newCallback;
    }

    /**
     * Update alertness level.
     */
    void setAlertLevel(AlertLevel_t newLevel) {
        alertLevel = newLevel;
    }

protected:
    /**
     * This callback allows receiving updates to the AlertLevel characteristic.
     *
     * @param[in] params
     *     Information about the characteristic being updated.
     */
    virtual void onDataWritten(const GattWriteCallbackParams *params) {
        if (params->handle == alertLevelChar.getValueHandle()) {
            alertLevel = *reinterpret_cast<const AlertLevel_t *>(params->data);
        }
    }

    void onDisconnectionFilter(const Gap::DisconnectionCallbackParams_t *params) {
        if (alertLevel != NO_ALERT) {
            callback(alertLevel);
        }
    }

protected:
    BLE          &ble;
    AlertLevel_t  alertLevel;
    callback_t    callback;

    ReadWriteGattCharacteristic<uint8_t> alertLevelChar;
};

#endif /* __BLE_LINK_LOSS_SERVICE_H__ */
