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

#ifndef MBED_DISCOVERED_SERVICE_H__
#define MBED_DISCOVERED_SERVICE_H__

#include "UUID.h"
#include "GattAttribute.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup gatt
 * @{
 * @addtogroup client
 * @{
 */

/**
 * Representation of a GATT service discovered.
 *
 * The discovery procedure discovers GATT Services are discovered on distant
 * GATT servers, which can be initiated by calling
 * GattClient::launchServiceDiscovery() or GattClient::discoverServices(). The
 * discovery process passes instances of this class to the callback handling
 * service discovered.
 *
 * Discovered services are characterized by the UUID of the service discovered
 * and the range of the GATT attributes belonging to the service.
 *
 * The UUID can be queried by calling getUUID() while the begining of the
 * attribute range can be obtained through getStartHandle() and the end of the
 * attribute range with a call to getEndHandle().
 *
 * The characteristics composing the service may be discovered by the function
 * GattClient::launchServiceDiscovery().
 */
class DiscoveredService {
public:
    /**
     * Get the UUID of the discovered service.
     *
     * @return A reference to the UUID of the discovered service.
     */
    const UUID &getUUID(void) const
    {
        return uuid;
    }

    /**
     * Get the start handle of the discovered service in the peer's GATT server.
     *
     * @return A reference to the start handle.
     */
    const GattAttribute::Handle_t& getStartHandle(void) const
    {
        return startHandle;
    }

    /**
     * Get the end handle of the discovered service in the peer's GATT server.
     *
     * @return A reference to the end handle.
     */
    const GattAttribute::Handle_t& getEndHandle(void) const
    {
        return endHandle;
    }

public:
    /**
     * Construct a DiscoveredService instance.
     *
     * @important This API is not meant to be used publicly. It is meant to be
     * used by internal APIs of Mbed BLE.
     */
    DiscoveredService() :
        uuid(UUID::ShortUUIDBytes_t(0)),
        startHandle(GattAttribute::INVALID_HANDLE),
        endHandle(GattAttribute::INVALID_HANDLE) {
    }

    /**
     * Set information about the discovered service.
     *
     * @important This API is not meant to be used publicly. It is meant to be
     * used by internal APIs of Mbed BLE.
     *
     * @param[in] uuidIn The UUID of the discovered service.
     * @param[in] startHandleIn The start handle of the discovered service in
     * the peer's GATT server.
     * @param[in] endHandleIn The end handle of the discovered service in the
     * peer's GATT server.
     */
    void setup(
        UUID uuidIn,
        GattAttribute::Handle_t startHandleIn,
        GattAttribute::Handle_t endHandleIn
    ) {
        uuid = uuidIn;
        startHandle = startHandleIn;
        endHandle = endHandleIn;
    }

    /**
     * Set the start and end handle of the discovered service.
     *
     * @important This API is not meant to be used publicly. It is meant to be
     * used by internal APIs of Mbed BLE.
     *
     * @param[in] startHandleIn The start handle of the discovered service in
     * the peer's GATT server.
     * @param[in] endHandleIn The end handle of the discovered service in the
     * peer's GATT server.
     */
    void setup(
        GattAttribute::Handle_t startHandleIn,
        GattAttribute::Handle_t endHandleIn
    ) {
        startHandle = startHandleIn;
        endHandle = endHandleIn;
    }

    /**
     * Set the long UUID of the discovered service.
     *
     * @important This API is not meant to be used publicly. It is meant to be
     * used by internal APIs of Mbed BLE.
     *
     * @param[in] longUUID The bytes composing the long UUID of this discovered
     * service.
     * @param[in] order The byte ordering of @p longUUID.
     */
    void setupLongUUID(
        UUID::LongUUIDBytes_t longUUID,
        UUID::ByteOrder_t order = UUID::MSB
    ) {
         uuid.setupLong(longUUID, order);
    }


private:
    DiscoveredService(const DiscoveredService &);

private:
    /**
     * UUID of the service.
     */
    UUID uuid;

    /**
     * Begining of the Service Handle Range.
     */
    GattAttribute::Handle_t startHandle;

    /**
     * Service Handle Range.
     */
    GattAttribute::Handle_t endHandle;
};

/**
 * @}
 * @}
 * @}
 */

#endif /* MBED_DISCOVERED_SERVICE_H__ */
