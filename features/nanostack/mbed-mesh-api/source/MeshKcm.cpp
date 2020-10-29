/*
 * Copyright (c) 2020 ARM Limited. All rights reserved.
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


#include <MeshKcm.h>
#include "mbed_toolchain.h"
#include "ns_trace.h"
#include "ws_management_api.h"
#include <string.h>
#include <stdlib.h>

#ifdef MBED_CONF_MBED_CLOUD_CLIENT_EXTERNAL_SST_SUPPORT
/* key_config_manager is available via mbed-cloud-client */
#include "key_config_manager.h"
#define MESH_KCM_CLOUD_CLIENT_AVAILABLE
#endif

/* Trace name */
#define TRACE_GROUP "mkcm"

#ifndef MESH_KCM_CLOUD_CLIENT_AVAILABLE
/* data structures copied from kcm_status.h */
typedef enum {
    KCM_STATUS_SUCCESS,
    KCM_STATUS_ERROR,
    KCM_STATUS_ITEM_NOT_FOUND
} kcm_status_e;

typedef enum {
    KCM_CONFIG_ITEM
} kcm_item_type_e;

#endif

/* KCM configuration item names */
static const char MESH_WISUN_NETWORK_NAME_KEY[] = "mesh_wisun_network_name";
static const char MESH_WISUN_NETWORK_SIZE_KEY[] = "mesh_wisun_network_size";

/*
 * Weak definition for kcm_item_get_data_size.
 * Will be overwritten by actual implementation if it is available.
 */
MBED_WEAK kcm_status_e kcm_item_get_data_size(const uint8_t  *kcm_item_name,
                                              size_t          kcm_item_name_len,
                                              kcm_item_type_e kcm_item_type,
                                              size_t         *kcm_item_data_size_out)
{
    return KCM_STATUS_ERROR;
}

/*
 * Weak definition for kcm_item_get_size_and_data.
 * Will be overwritten by actual implementation if it is available.
 */
MBED_WEAK kcm_status_e kcm_item_get_size_and_data(const uint8_t *kcm_item_name,
                                                  size_t kcm_item_name_len,
                                                  kcm_item_type_e kcm_item_type,
                                                  uint8_t **kcm_item_data_out,
                                                  size_t *kcm_item_data_size_out)
{
    return KCM_STATUS_ERROR;
}

int mesh_kcm_wisun_network_name_init(char *network_name_buf, size_t network_name_buf_len)
{
    kcm_status_e kcm_status;
    int config_status = -1;
    uint8_t *kcm_item_buffer = NULL;
    size_t kcm_item_buff_size;

    kcm_status = kcm_item_get_size_and_data((uint8_t *)MESH_WISUN_NETWORK_NAME_KEY,
                                            sizeof(MESH_WISUN_NETWORK_NAME_KEY) - 1,
                                            KCM_CONFIG_ITEM,
                                            &kcm_item_buffer,
                                            &kcm_item_buff_size);

    if (kcm_status == KCM_STATUS_SUCCESS) {
        // network name found from KCM
        if (kcm_item_buff_size < network_name_buf_len) {
            // configuration has valid length
            memcpy(network_name_buf, kcm_item_buffer, kcm_item_buff_size);
            network_name_buf[kcm_item_buff_size] = 0;
            config_status = 0;
        }
    } else {
        strncpy(network_name_buf, MBED_CONF_MBED_MESH_API_WISUN_NETWORK_NAME, network_name_buf_len);
        network_name_buf[network_name_buf_len] = 0;
        config_status = 0;
    }

    free(kcm_item_buffer);
    return config_status;
}

int mesh_kcm_wisun_network_size_init(uint8_t *network_size)
{
    kcm_status_e kcm_status;
    int config_status = -1;
    uint8_t *kcm_item_buffer = NULL;
    size_t kcm_item_buff_size;

    kcm_status = kcm_item_get_size_and_data((uint8_t *)MESH_WISUN_NETWORK_SIZE_KEY,
                                            sizeof(MESH_WISUN_NETWORK_SIZE_KEY) - 1,
                                            KCM_CONFIG_ITEM,
                                            &kcm_item_buffer,
                                            &kcm_item_buff_size);

    if (kcm_status == KCM_STATUS_SUCCESS && kcm_item_buff_size == 1) {
        // network size as hundreds of devices
        *network_size = kcm_item_buffer[0];
        config_status = 0;
    }

    if (kcm_status == KCM_STATUS_ITEM_NOT_FOUND) {
#ifdef MBED_CONF_MBED_MESH_API_WISUN_NETWORK_SIZE
        *network_size = MBED_CONF_MBED_MESH_API_WISUN_NETWORK_SIZE;
        config_status = 0;
#else
        // size is not set to Mbed OS config, and not found from KCM
        config_status = 1;
#endif
    }

    free(kcm_item_buffer);

    return config_status;
}
