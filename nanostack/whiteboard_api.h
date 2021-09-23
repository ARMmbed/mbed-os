/*
 * Copyright (c) 2011-2017, Pelion and affiliates.
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

#ifndef WHITEBOARD_API_H_
#define WHITEBOARD_API_H_

#include "net_interface.h"

/**
 * \file whiteboard_api.h
 * \brief An API for DAD, intercepting messages from backhaul network and keeping track of routes inside the PAN.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \struct whiteboard_entry_t
 * \brief A structure describing a whiteboarding entry.
 */
typedef struct whiteboard_entry_t {
    uint8_t                         address[16];        /**< Registered IP address. */
    uint_least24_t                  ttl;                /**< Whiteboard entry TTL (seconds). */
    uint8_t                         eui64[8];           /**< EUI-64 of node that IP address is registered to. */
    int8_t                          interface_index;    /**< Interface id */
} whiteboard_entry_t;

/**
 * \brief Read whiteboard entries
 * \param cur NULL to read first entry, or get next entry after cur.
 * \return A pointer to whiteboard_entry_t structure.
 */
extern whiteboard_entry_t *whiteboard_get(whiteboard_entry_t *cur);

/**
 * @brief Whiteboard_set_device_hard_limit Sets the absolut limit of child devices this device can handle.
 *        This is very useful in situations, where 1 GW drops from network and causes it's children
 *        to join other GW networks. This might cause other GWs to run out of memory.
 * @param limit Absolute maximum amount of devices allowed to join. Default value=0 means unlimited (as long as there is memory)
 */
extern void whiteboard_set_device_hard_limit(uint16_t limit);

#ifdef __cplusplus
}
#endif
#endif /* WHITEBOARD_API_H_ */
