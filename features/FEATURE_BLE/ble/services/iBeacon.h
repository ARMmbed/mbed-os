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
#include "ble/pal/Deprecated.h"

#if BLE_FEATURE_GATT_SERVER

BLE_DEPRECATED_API_USE_BEGIN()

/**
 * iBeacon Service.
 *
 * @par Purpose
 *
 * iBeacons are Bluetooth Low Energy (BLE) devices advertising an identification
 * number generally used to determine the location of devices or physical objects
 * near a mobile phone user.
 *
 * iOS scans for iBeacon devices in a background task and notifies apps
 * that subscribe to a specific region when a device enters or leaves an area. Apps may
 * use this information to display context-aware content to users.
 *
 * As an example, a museum can deploy an app that informs the user when one of
 * its exhibitions is entered and then displays specific information about exposed
 * pieces of art when the user is sufficiently close to them.
 *
 * @par Positioning
 *
 * Location information is hierarchically structured. A UUID specific to the
 * application and its deployment is used to identify a region. That region
 * usually identifies an organization. The region is divided into subregions
 * identified by a major ID. The subregion contains related points of interest
 * which a minor ID distinguishes.
 *
 * As an example, a city willing to improve tourist's experience can deploy a fleet
 * of iBeacons in relevant touristic locations it operates. The UUID may
 * identify a place managed by the city. The major ID would identify the place;
 * it can be a museum, a historic monument, a metro station and so on. The minor ID
 * would identify a specific spot within a specific city place. It can be a
 * piece of art, a ticket dispenser or a relevant point of interest.
 *
 * Each iBeacon device is physically attached to the spot it locates and
 * advertises the triplet UUID, major ID and minor ID.
 *
 * @par Proximity
 *
 * The beacon advertises the signal strength measured by an iOS device at a
 * distance of one meter. iOS uses this information to approximate the
 * proximity to a given beacon:
 *   - Immediate: The beacon is less than one meter away from the user.
 *   - Near: The beacon is one to three meters away from the user.
 *   - Far: The user is not near the beacon; the distance highly depends on
 *     the physical environment.
 *
 * Ideally, beacons should be calibrated at their deployment location because the
 * surrounding environment affects the strength of the advertised signal.
 *
 * @par Usage
 *
 * Mbed OS applications can use this class to configure a device to broadcast
 * advertising packets mimicking an iBeacon. The construction automatically
 * creates the payload identifying the beacon and registers it as part of the
 * advertising payload of the device.
 *
 * Beacon configuration and advertising commencement is left to the user.
 *
 * @attention If you are interested in manufacturing iBeacons, you must obtain a
 * license from Apple. More information at https://developer.apple.com/ibeacon/.
 * The license also grant access to the iBeacons technical specification.
 *
 * @note More information at https://developer.apple.com/ibeacon/Getting-Started-with-iBeacon.pdf
 *
 * @deprecated This service is deprecated, and no replacement is currently available.
 */
MBED_DEPRECATED_SINCE(
    "mbed-os-5.11",
    "This service is deprecated, and no replacement is currently available."
)
class iBeacon {
public:
#if !(DOXYGEN_ONLY)
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
             * Packet ID; equal to 2 for an iBeacon.
             */
            uint8_t ID;

            /**
             * Length of the remaining data present in the payload.
             */
            uint8_t len;

            /**
             * Beacon UUID.
             */
            uint8_t proximityUUID[16];

            /**
             * Beacon major group ID.
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
         * to group their iBeacons into a single network, a single region, and
         * identify their organization among others.
         *
         * @param[in] majNum Beacon major group ID. iBeacon exploitants may use
         * this field to divide the region into subregions, and their network into
         * subnetworks.
         *
         * @param[in] minNum Identifier of the Beacon in its subregion.
         *
         * @param[in] transmitPower Measured transmit power of the beacon at 1
         * meter. Scanners use this parameter to approximate the distance
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
#endif //#if !(DOXYGEN_ONLY)
public:
    /**
     * Construct an iBeacon::Payload and register it into Gap.
     *
     * @param[in] _ble The BLE interface to configure with the iBeacon payload.
     *
     * @param[in] uuid Beacon network ID. iBeacon operators use this value
     * to group their iBeacons into a single network, a single region, and
     * identify their organization among others.
     *
     * @param[in] majNum Beacon major group ID. iBeacon fleet operators may use
     * this field to divide the region into subregions, and their network into
     * subnetworks.
     *
     * @param[in] minNum Identifier of the Beacon in its subregion.
     *
     * @param[in] txP Measured transmit power of the beacon at distance of
     * one meter. Scanners use this parameter to approximate the distance
     * to the beacon.
     *
     * @param[in] compID ID of the beacon manufacturer.
     *
     * @deprecated This service is deprecated, and no replacement is currently available.
     */
    MBED_DEPRECATED_SINCE(
        "mbed-os-5.11",
        "This service is deprecated, and no replacement is currently available."
    )
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

BLE_DEPRECATED_API_USE_END()

#endif // BLE_FEATURE_GATT_SERVER

#endif //MBED_BLE_IBEACON_H__
