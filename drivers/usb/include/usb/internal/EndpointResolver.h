/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef ENDPOINT_RESOLVER_H
#define ENDPOINT_RESOLVER_H

#include "USBPhy.h"

/**
 * \defgroup drivers_EndpointResolver EndpointResolver class
 * \ingroup drivers-internal-api-usb
 * @{
 */

/**
 * Utility class for resolving endpoints
 *
 * This class is intended to make the process of
 * selecting the correct endpoint from a device endpoint
 * table easier. It also provides a verification function
 * to check if the device has enough resources for the
 * given configuration.
 *
 */
class EndpointResolver {
public:
    EndpointResolver(const usb_ep_table_t *table);
    ~EndpointResolver();

    /**
     * Add control endpoint size
     *
     * @param size Space reserved for control in and control out
     */
    void endpoint_ctrl(uint32_t size);

    /**
     * Return a free IN endpoint of the given size
     *
     * @param type Desired endpoint type
     * @param size Space to reserve for this endpoint
     * @return Endpoint index or 0 if there are not enough resources
     */
    usb_ep_t endpoint_in(usb_ep_type_t type, uint32_t size);

    /**
     * Return a free OUT endpoint of the given size
     *
     * @param type Desired endpoint type
     * @param size Space to reserve for this endpoint
     * @return Endpoint index or 0 if there are not enough resources
     */
    usb_ep_t endpoint_out(usb_ep_type_t type, uint32_t size);

    /**
     * Get next free endpoint
     */
    usb_ep_t next_free_endpoint(bool in_not_out, usb_ep_type_t type, uint32_t size);

    /**
     * Check if the endpoint configuration created so far is valid
     *
     * @return true if all endpoint sizes are available and fit, false otherwise
     */
    bool valid();

    /**
     * Reset this class's state to when it was constructed
     */
    void reset();

private:

    usb_ep_t index_to_endpoint(int index);
    int next_index(usb_ep_type_t type, bool in_not_out);

    const usb_ep_table_t *_table;
    uint32_t _cost;
    uint32_t _used;
    bool _valid;
};

/** @}*/

#endif
