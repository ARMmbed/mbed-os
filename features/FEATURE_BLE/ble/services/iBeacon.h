/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#ifndef __BLE_IBEACON_H__
#define __BLE_IBEACON_H__

#include "core_cmInstr.h"
#include "ble/BLE.h"

/**
* @class iBeacon
* @brief iBeacon Service. This sets up a device to broadcast advertising packets to mimic an iBeacon.
*/
class iBeacon
{
public:
    typedef const uint8_t LocationUUID_t[16];

    union Payload {
        uint8_t raw[25];
        struct {
            uint16_t companyID;
            uint8_t ID;
            uint8_t len;
            uint8_t proximityUUID[16];
            uint16_t majorNumber;
            uint16_t minorNumber;
            uint8_t txPower;
        };

        Payload(LocationUUID_t uuid, uint16_t majNum, uint16_t minNum, uint8_t transmitPower, uint16_t companyIDIn) :
            companyID(companyIDIn), ID(0x02), len(0x15), majorNumber(__REV16(majNum)), minorNumber(__REV16(minNum)), txPower(transmitPower)
        {
            memcpy(proximityUUID, uuid, sizeof(LocationUUID_t));
        }
    };

public:
    iBeacon(BLE            &_ble,
            LocationUUID_t  uuid,
            uint16_t        majNum,
            uint16_t        minNum,
            uint8_t         txP    = 0xC8,
            uint16_t        compID = 0x004C) :
        ble(_ble), data(uuid, majNum, minNum, txP, compID)
    {
        // Generate the 0x020106 part of the iBeacon Prefix.
        ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE );
        // Generate the 0x1AFF part of the iBeacon Prefix.
        ble.accumulateAdvertisingPayload(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, data.raw, sizeof(data.raw));

        // Set advertising type.
        ble.setAdvertisingType(GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED);
    }

protected:
    BLE     &ble;
    Payload  data;
};

typedef iBeacon iBeaconService; /* This type-alias is deprecated. Please use iBeacon directly. This alias may be dropped from a future release. */

#endif //__BLE_IBEACON_H__
