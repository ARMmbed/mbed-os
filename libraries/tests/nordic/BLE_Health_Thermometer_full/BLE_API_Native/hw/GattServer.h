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

#ifndef __GATT_SERVER_H__
#define __GATT_SERVER_H__

#include "mbed.h"
#include "blecommon.h"
#include "GattService.h"
#include "GattServerEvents.h"

/**************************************************************************/
/*!
    \brief
    The base class used to abstract GATT Server functionality to a specific
    radio transceiver, SOC or BLE Stack.
*/
/**************************************************************************/
class GattServer
{
    private:
        GattServerEvents *m_pEventHandler;

    public:
        /* These functions must be defined in the sub-class */
        virtual ble_error_t addService(GattService &) = 0;
        virtual ble_error_t readValue(uint16_t, uint8_t[], uint16_t) = 0;
        virtual ble_error_t updateValue(uint16_t, uint8_t[], uint16_t) = 0;

        // ToDo: For updateValue, check the CCCD to see if the value we are
        // updating has the notify or indicate bits sent, and if BOTH are set
        // be sure to call sd_ble_gatts_hvx() twice with notify then indicate!
        // Strange use case, but valid and must be covered!

        /* Event callback handlers */
        void setEventHandler(GattServerEvents *pEventHandler) {m_pEventHandler = pEventHandler;}
        void handleEvent(GattServerEvents::gattEvent_e type, uint16_t charHandle) {
            if (NULL == m_pEventHandler)
                  return;
            switch(type) {
                  case GattServerEvents::GATT_EVENT_DATA_SENT:
                        m_pEventHandler->onDataSent(charHandle);
                        break;
                  case GattServerEvents::GATT_EVENT_DATA_WRITTEN:
                        m_pEventHandler->onDataWritten(charHandle);
                        break;
                  case GattServerEvents::GATT_EVENT_UPDATES_ENABLED:
                        m_pEventHandler->onUpdatesEnabled(charHandle);
                        break;
                  case GattServerEvents::GATT_EVENT_UPDATES_DISABLED:
                        m_pEventHandler->onUpdatesDisabled(charHandle);
                        break;
                  case GattServerEvents::GATT_EVENT_CONFIRMATION_RECEIVED:
                        m_pEventHandler->onConfirmationReceived(charHandle);
                        break;
            }
        }

        uint8_t serviceCount;
        uint8_t characteristicCount;
};

#endif
