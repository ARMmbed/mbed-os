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
#ifndef MBED_BLE_IBEACON_H__
#define MBED_BLE_IBEACON_H__

#include "cmsis_compiler.h"
#include "ble/BLE.h"

/**
 * iBeacon Service.
 *
 * @par Purpose
 *
 * iBeacons are Bluetooth Low Energy (BLE) devices advertising an identification
 * number generally used to identify the location of devices or physical objects
 * nearby a mobile phone user.
 *
 * iOS scans for iBeacon devices in a background task and will notify Apps
 * subscribed to a specific region when the area is entered or left. Apps may
 * use this information to display context-aware content to users.
 *
 * As an example a museum can deploy an apps which inform the user when one of
 * its exhibition is enterred then display specific information about exposed
 * pieces of art when the user is sufficiently close to them.
 *
 * @par Positioning
 *
 * Location information is hierarchically structured. A UUID specific to the
 * application and its deployment is used to identify a region. That region
 * usually identify an organization. The region is divided into subregions
 * identified by a major ID. The subregion may contain related points of
 * interrest. Finally specific points of interrest within a subregion are
 * distinguished by a minor ID.
 *
 * As an example a city willing to improve tourists experience can deploy a fleet
 * of iBeacons in relevant touristic locations it operates. The UUID may
 * identify a place managed by the city. The major ID would identify the place
 * it can be something such as a museum, an historic monument or a metro station.
 * The minor ID would locate a specific spot within the city place. It can be a
 * piece of art, a ticket dispenser or a relevant point of interrest. Each
 * iBeacon device is physically attached to the spot it identify and advertise
 * those three location information.
 *
 * @par Proximity
 *
 * The beacon advertise the signal strength measured by an iOS device at a
 * distance of one meter. This information is used by iOS to approximate the
 * proximity to a given beacon:
 *   - Immediate: The beacon is less than one meter away from the user.
 *   - Near: The beacon is one to three meter away from the user.
 *   - Far: The application is not near the device, distance highly depends on
 *     the physical environment.
 *
 * Ideally beacons should be calibrated at there deployment location because the
 * surrounding environment affect the strength of the advertised signal.
 *
 * @par Usage
 *
 * Mbed OS applications can use this class to set up a device to broadcast
 * advertising packets mimicking an iBeacon. The construction will automatically
 * create the payload identifying the beacon and register it as part of the
 * advertising payload of the device.
 *
 * @important If you are interested in manufacturing iBeacons, you must obtain a
 * license from Apple. More information at https://developer.apple.com/ibeacon/.
 * The licence also grant access to the iBeacons technical specification.
 *
 * @note More information at https://developer.apple.com/ibeacon/Getting-Started-with-iBeacon.pdf
 */
class iBeacon
{
public:
    /**
     * Data buffer of a location UUID.
     */
    typedef const uint8_t LocationUUID_t[16];

    /**
     * iBeacon payload builder.
     *
     * This data structure contains the payload of an iBeacon. The payload is
     * built at construction time and application code can set up an iBeacon by
     * injecting the raw field into the GAP advertising payload as a
     * GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA.
     */
    union Payload {
        /**
         * Raw data of the payload.
         */
        uint8_t raw[25];
        struct {
            /**
             * Beacon manufacturer identifier.
             */
            uint16_t companyID;

            /**
             * Packet ID; Equal to 2 for an iBeacon.
             */
            uint8_t ID;

            /**
             * Length of the remaining data presents in the payload.
             */
            uint8_t len;

            /**
             * Beacon UUID.
             */
            uint8_t proximityUUID[16];

            /**
             * Beacon Major group ID.
             */
            uint16_t majorNumber;

            /**
             * Beacon minor ID.
             */
            uint16_t minorNumber;

            /**
             * Tx power received at 1 meter; in dBm.
             */
            uint8_t txPower;
        };

        /**
         * Assemble an iBeacon payload.
         *
         * @param[in] uuid Beacon network ID. iBeacon operators use this value
         * to group their iBeacons into a single network, a single region and
         * identify their organization amongst others.
         *
         * @param[in] majNum Beacon major group ID. iBeacon exploitants may use
         * this field to divide the region into subregions, their network into
         * sub networks.
         *
         * @param[in] minNum Identifier of the Beacon in its subregion.
         *
         * @param[in] transmitPower Measured transmit power of the beacon at 1
         * meter. This parameter is used by scanners to approximate the distance
         * to the beacon.
         *
         * @param[in] companyIDIn ID of the beacon manufacturer.
         */
        Payload(
            LocationUUID_t uuid,
            uint16_t majNum,
            uint16_t minNum,
            uint8_t transmitPower,
            uint16_t companyIDIn
        ) : companyID(companyIDIn),
            ID(0x02),
            len(0x15),
            majorNumber(__REV16(majNum)),
            minorNumber(__REV16(minNum)),
            txPower(transmitPower)
        {
            memcpy(proximityUUID, uuid, sizeof(LocationUUID_t));
        }
    };

public:
    /**
     * Construct an iBeacon::Payload and register it into Gap.
     *
     * @param[in] _ble The ble interface to configure with the iBeacon payload.
     *
     * @param[in] uuid Beacon network ID. iBeacon operators use this value
     * to group their iBeacons into a single network, a single region and
     * identify their organization amongst others.
     *
     * @param[in] majNum Beacon major group ID. iBeacon exploitants may use
     * this field to divide the region into subregions, their network into
     * sub networks.
     *
     * @param[in] minNum Identifier of the Beacon in its subregion.
     *
     * @param[in] txP Measured transmit power of the beacon at 1
     * meter. This parameter is used by scanners to approximate the distance
     * to the beacon.
     *
     * @param[in] compID ID of the beacon manufacturer.
     */
    iBeacon(
        BLE &_ble,
        LocationUUID_t uuid,
        uint16_t majNum,
        uint16_t minNum,
        uint8_t txP = 0xC8,
        uint16_t compID = 0x004C
    ) : ble(_ble),
        data(uuid, majNum, minNum, txP, compID)
    {
        // Generate the 0x020106 part of the iBeacon Prefix.
        ble.accumulateAdvertisingPayload(
            GapAdvertisingData::BREDR_NOT_SUPPORTED |
            GapAdvertisingData::LE_GENERAL_DISCOVERABLE
        );
        // Generate the 0x1AFF part of the iBeacon Prefix.
        ble.accumulateAdvertisingPayload(
            GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA,
            data.raw,
            sizeof(data.raw)
        );

        // Set advertising type.
        ble.setAdvertisingType(
            GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED
        );
    }

protected:
    BLE &ble;
    Payload data;
};

/**
 * iBeacon alias.
 *
 * @deprecated Please use iBeacon directly. This alias may be dropped from a
 * future release.
 */
typedef iBeacon iBeaconService;

#endif //MBED_BLE_IBEACON_H__
