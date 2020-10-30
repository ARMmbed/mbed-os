/*
 * Copyright (c) 2020 ARM Limited
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

#ifndef __INCLUDE_MESH_KCM__
#define __INCLUDE_MESH_KCM__
#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * \brief Initialize Wi-SUN network name
 *
 * \param network_name_buf Buffer for the network name
 * \param network_name_buf_len Length of provided buffer
 * \return 0 on success, -1 on errors
 *
 */
int mesh_kcm_wisun_network_name_init(char *network_name_buf, size_t network_name_buf_len);

/*
 * \brief Initialize Wi-SUN network size
 *
 * \param network_size Buffer for the network size, size 1 byte
 * \return 0 on success.
 * \return 1 if network size is not configured.
 * \return -1 if network size configuration error.
 *
 */
int mesh_kcm_wisun_network_size_init(uint8_t *network_size);

/*
 * \brief Initialize Wi-SUN network regulatory domain including operating mode and class.
 *
 * \param regulatory_domain
 * \param operating_class
 * \param operating_mode
 * \return 0 on success.
 * \return -1 in case of configuration error
 *
 */
int mesh_kcm_wisun_network_regulatory_domain_init(uint8_t *regulatory_domain, uint8_t *operating_class, uint8_t *operating_mode);

/*
 * \brief Initialize Wi-SUN network radius server address.
 *
 * \param srv_addr Pointer where address can be read
 * \param srv_addr_len Length of data
 * \return 0 if configuration is read successfully, caller must free the memory.
 * \return 1 in case of static configuration is used, no memory free needed.
 * \return -1 in case of configuration is not available.
 *
 */
int mesh_kcm_wisun_network_radius_addr_init(uint8_t **srv_addr, size_t *srv_addr_len);

/*
 * \brief Initialize Wi-SUN network radius server secret.
 *
 * Shared secret can be ascii string (not NULL terminated) or byte array.
 *
 * \param srv_secret_buf Pointer where secret data will be available
 * \param actual_secret_len Actual secret length in successful case.
 * \return 0 if configuration read successfully, caller must free the memory.
 * \return 1 in case of static configuration is used, no memory free needed
 * \return -1 in case of configuration is not available
 *
 */
int mesh_kcm_wisun_network_radius_secret_init(uint8_t **srv_secret_buf, size_t *actual_secret_len);


#ifdef __cplusplus
}
#endif
#endif /* __INCLUDE_MESH_KCM__ */
