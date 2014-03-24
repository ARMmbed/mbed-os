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
  

#ifndef __GATT_SERVICE_H__
#define __GATT_SERVICE_H__

#include "blecommon.h"
#include "UUID.h"
#include "GattCharacteristic.h"

#define BLE_SERVICE_MAX_CHARACTERISTICS (5)

/**************************************************************************/
/*!
    \brief  GATT service
*/
/**************************************************************************/
class GattService
{
    private:
    
    public:
        GattService(uint8_t[16]);  /* 128-bit Base UUID */
        GattService(uint16_t);     /* 16-bit BLE UUID */
        virtual ~GattService(void);
    
        UUID                primaryServiceID;
        uint8_t             characteristicCount;
        GattCharacteristic* characteristics[BLE_SERVICE_MAX_CHARACTERISTICS];
        uint16_t            handle;

        ble_error_t         addCharacteristic(GattCharacteristic &);

        enum {
          UUID_ALERT_NOTIFICATION_SERVICE     = 0x1811,
          UUID_BATTERY_SERVICE                = 0x180F,
          UUID_BLOOD_PRESSURE_SERVICE         = 0x1810,
          UUID_CURRENT_TIME_SERVICE           = 0x1805,
          UUID_CYCLING_SPEED_AND_CADENCE      = 0x1816,
          UUID_DEVICE_INFORMATION_SERVICE     = 0x180A,
          UUID_GLUCOSE_SERVICE                = 0x1808,
          UUID_HEALTH_THERMOMETER_SERVICE     = 0x1809,
          UUID_HEART_RATE_SERVICE             = 0x180D,
          UUID_HUMAN_INTERFACE_DEVICE_SERVICE = 0x1812,
          UUID_IMMEDIATE_ALERT_SERVICE        = 0x1802,
          UUID_LINK_LOSS_SERVICE              = 0x1803,
          UUID_NEXT_DST_CHANGE_SERVICE        = 0x1807,
          UUID_PHONE_ALERT_STATUS_SERVICE     = 0x180E,
          UUID_REFERENCE_TIME_UPDATE_SERVICE  = 0x1806,
          UUID_RUNNING_SPEED_AND_CADENCE      = 0x1814,
          UUID_SCAN_PARAMETERS_SERVICE        = 0x1813,
          UUID_TX_POWER_SERVICE               = 0x1804
        };
};

#endif
