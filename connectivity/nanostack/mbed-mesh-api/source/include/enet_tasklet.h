/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ENET_TASKLET_H
#define ENET_TASKLET_H

#include "mesh_interface_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * \brief Initialize system.
 */
void enet_tasklet_init(void);

/*
 * \brief Create network interface.
 *
 * \param device_id Registered physical device.
 * \return interface ID used to communication with this interface.
 */
int8_t enet_tasklet_network_init(int8_t device_id);

/*
 * \brief Create PPP network interface.
 *
 * \param device_id Registered physical device.
 * \return interface ID used to communication with this interface.
 */
int8_t enet_tasklet_ppp_network_init(int8_t device_id);

/*
 * \brief Connect to Ethernet network.
 *
 * \param callback Call when network state changes.
 * \param nwk_interface_id To use for networking.
 *
 */
int8_t enet_tasklet_connect(void (*)(mesh_connection_status_t mesh_status), int8_t nwk_interface_id);

/*
 * \brief Disconnect network interface.
 *
 * \param send_cb Send possible network status change event if set to `true`.
 * \return >= 0 if disconnected successfully.
 * \return < 0 if error.
 */
int8_t enet_tasklet_disconnect(bool send_cb);

/*
 * \brief Callback to call when the link state changes.
 *
 * \param up Tells if link is up or down.
 */
void enet_tasklet_link_state_changed(bool up);

#ifdef __cplusplus
}
#endif

#endif // ENET_TASKLET_H
