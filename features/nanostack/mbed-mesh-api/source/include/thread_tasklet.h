/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
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

#ifndef __INCLUDE_THREAD_TASKLET__
#define __INCLUDE_THREAD_TASKLET__
#include "ns_types.h"
#include "eventOS_event.h"
#include "mbed-mesh-api/mesh_interface_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Type of the network status callback.
 */
typedef void (*mesh_interface_cb)(mesh_connection_status_t mesh_status);

/*
 * \brief Read own global IP address
 *
 * \param address where own IP address will be written
 * \param len length of provided address buffer
 *
 * \return 0 on success
 * \return -1 if address reading fails
 */
int8_t thread_tasklet_get_ip_address(char *address, int8_t len);

/*
 * \brief Connect to mesh network
 *
 * \param callback to be called when network state changes
 * \param nwk_interface_id to use for networking
 *
 * \return >= 0 on success
 * \return -1 if callback function is used in another tasklet
 * \return -2 if memory allocation fails
 * \return -3 if network is already connected
 */
int8_t thread_tasklet_connect(mesh_interface_cb callback, int8_t nwk_interface_id);

/*
 * \brief Initialize mesh system.
 * Memory pool, timers, traces and support are initialized.
 */
void thread_tasklet_init(void);

/*
 * \brief Create network interface.
 *
 * \param device_id registered physical device
 * \return interface ID that can be used to communication with this interface
 */
int8_t thread_tasklet_network_init(int8_t device_id);

/*
 * \brief Sets eui64 for the device configuration
 * \param eui64 eui64 to be set
 * \param pskd private shared key
 */
void thread_tasklet_device_eui64_set(const uint8_t *eui64);

/*
 * \brief Sets PSKd for the device configuration
 * \param pskd private shared key to be set
 */
uint8_t thread_tasklet_device_pskd_set(const char *pskd);

/*
 * \brief Disconnect network interface.
 *
 * \param send_cb send possible network status change event if set to true.
 * \return >= 0 if disconnected successfully.
 * \return < 0 in case of errors
 */
int8_t thread_tasklet_disconnect(bool send_cb);

/*
 * \brief Set device data polling rate
 *
 * \param timeout timeout between data polls
 * \return  0 on success
 * \return < 0 in case of errors
 */
int8_t thread_tasklet_data_poll_rate_set(uint32_t timeout);

#ifdef __cplusplus
}
#endif
#endif /* __INCLUDE_THREAD_TASKLET__ */
