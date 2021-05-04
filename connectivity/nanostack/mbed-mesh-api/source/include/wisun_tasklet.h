/*
 * Copyright (c) 2018-2019 ARM Limited
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

#ifndef __INCLUDE_WISUN_TASKLET__
#define __INCLUDE_WISUN_TASKLET__
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
 * \brief Read border router IP address
 *
 * \param address where router IP address will be written
 * \param len length of provided address buffer
 *
 * \return 0 on success
 * \return -1 if address reading fails
 */
int8_t wisun_tasklet_get_router_ip_address(char *address, int8_t len);

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
int8_t wisun_tasklet_connect(mesh_interface_cb callback, int8_t nwk_interface_id);

/*
 * \brief Initialize mesh system.
 * Memory pool, timers, traces and support are initialized.
 */
void wisun_tasklet_init(void);

/*
 * \brief Create network interface.
 *
 * \param device_id registered physical device
 * \return interface ID that can be used to communication with this interface
 * \return -1 in case of MAC initialization fails
 * \return -2 in case of error in parameters
 * \return -3 in case of memory allocation error
 */
int8_t wisun_tasklet_network_init(int8_t device_id);

/*
 * \brief Disconnect network interface.
 *
 * \param send_cb send possible network status change event if set to true.
 * \return >= 0 if disconnected successfully.
 * \return < 0 in case of errors
 */
int8_t wisun_tasklet_disconnect(bool send_cb);

/*
 * \brief Set own certificate to Wi-SUN network
 *
 * \param cert to use for networking
 * \param cert_len
 * \param cert_key
 * \param cert_key_len
 * \return 0 if certificate stored successfully
 * \return < 0 in case of errors
 */
int wisun_tasklet_set_own_certificate(uint8_t *cert, uint16_t cert_len, uint8_t *cert_key, uint16_t cert_key_len);

/*
 * \brief Remove own certificate from Wi-SUN network
 *
  * \return 0 if certificates removed successfully
 * \return < 0 in case of errors
 */
int wisun_tasklet_remove_own_certificates(void);

/*
 * \brief Set trusted certificate to Wi-SUN network
 *
 * \param cert to use for networking
 * \param cert_len
 * \return 0 if certificate stored successfully
 * \return < 0 in case of errors
 */
int wisun_tasklet_set_trusted_certificate(uint8_t *cert, uint16_t cert_len);

/*
 * \brief Remove trusted certificate from Wi-SUN network
 *
 * \return 0 if certificates removed successfully
 * \return < 0 in case of errors
 */
int wisun_tasklet_remove_trusted_certificates(void);

/*
 * \brief Start Wi-SUN statistics
 *
 * \return 0 Statistics start successful
 * \return < 0 in case of errors
 */
int wisun_tasklet_statistics_start(void);

/*
 * \brief Reset Wi-SUN statistics
 *
 * \return 0 Statistics start successful
 * \return < 0 in case of errors
 */
int wisun_tasklet_statistics_reset(void);

/*
 * \brief Reads Wi-SUN network statistics
 *
 * \return 0 Statistics read successful
 * \return < 0 in case of errors
 */
int wisun_tasklet_statistics_nw_read(mesh_nw_statistics_t *stats);

/*
 * \brief Reads Wi-SUN MAC statistics
 *
 * \return 0 Statistics read successful
 * \return < 0 in case of errors
 */
int wisun_tasklet_statistics_mac_read(mesh_mac_statistics_t *stats);

#ifdef __cplusplus
}
#endif
#endif /* __INCLUDE_WISUN_TASKLET__ */
