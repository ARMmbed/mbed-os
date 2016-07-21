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

#ifndef __DISCOVERED_SERVICE_H__
#define __DISCOVERED_SERVICE_H__

#include "UUID.h"
#include "GattAttribute.h"

/**@brief Type for holding information about the service and the characteristics found during
 *        the discovery process.
 */
class DiscoveredService {
public:
    /**
     * Set information about the discovered service.
     *
     * @param[in] uuidIn
     *              The UUID of the discovered service.
     * @param[in] startHandleIn
     *              The start handle of the discovered service in the peer's
     *              ATT table.
     * @param[in] endHandleIn
     *              The end handle of the discovered service in the peer's
     *              ATT table.
     */
    void setup(UUID uuidIn, GattAttribute::Handle_t startHandleIn, GattAttribute::Handle_t endHandleIn) {
        uuid        = uuidIn;
        startHandle = startHandleIn;
        endHandle   = endHandleIn;
    }

    /**
     * Set the start and end handle of the discovered service.
     * @param[in] startHandleIn
     *              The start handle of the discovered service in the peer's
     *              ATT table.
     * @param[in] endHandleIn
     *              The end handle of the discovered service in the peer's
     *              ATT table.
     */
    void setup(GattAttribute::Handle_t startHandleIn, GattAttribute::Handle_t endHandleIn) {
        startHandle = startHandleIn;
        endHandle   = endHandleIn;
    }

    /**
     * Set the long UUID of the discovered service.
     *
     * @param[in] longUUID
     *              The long UUID of the discovered service.
     * @param[in] order
     *              The byte ordering of @p longUUID.
     */
    void setupLongUUID(UUID::LongUUIDBytes_t longUUID, UUID::ByteOrder_t order = UUID::MSB) {
         uuid.setupLong(longUUID, order);
    }

public:
    /**
     * Get the UUID of the discovered service.
     *
     * @return A reference to the UUID of the discovered service.
     */
    const UUID &getUUID(void) const {
        return uuid;
    }

    /**
     * Get the start handle of the discovered service in the peer's ATT table.
     *
     * @return A reference to the start handle.
     */
    const GattAttribute::Handle_t& getStartHandle(void) const {
        return startHandle;
    }

    /**
     * Get the end handle of the discovered service in the peer's ATT table.
     *
     * @return A reference to the end handle.
     */
    const GattAttribute::Handle_t& getEndHandle(void) const {
        return endHandle;
    }

public:
    /**
     * Construct a DiscoveredService instance.
     */
    DiscoveredService() : uuid(UUID::ShortUUIDBytes_t(0)),
                          startHandle(GattAttribute::INVALID_HANDLE),
                          endHandle(GattAttribute::INVALID_HANDLE) {
        /* empty */
    }

private:
    DiscoveredService(const DiscoveredService &);

private:
    UUID                    uuid;        /**< UUID of the service.  */
    GattAttribute::Handle_t startHandle; /**< Service Handle Range. */
    GattAttribute::Handle_t endHandle;   /**< Service Handle Range. */
};

#endif /*__DISCOVERED_SERVICE_H__*/
