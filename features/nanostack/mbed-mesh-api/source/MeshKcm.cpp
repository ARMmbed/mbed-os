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
static const char MESH_WISUN_REGULATORY_DOMAIN[] = "mesh_wisun_regulatory_domain";
static const char MESH_WISUN_OPERATING_MODE[] = "mesh_wisun_operating_mode";
static const char MESH_WISUN_OPERATING_CLASS[] = "mesh_wisun_operating_class";
static const char MESH_WISUN_RADIUS_SERVER_ADDRESS[] = "mesh_wisun_br_radius_srv_address";
static const char MESH_WISUN_RADIUS_SERVER_SECRET[] = "mesh_wisun_br_radius_srv_secret";

#ifdef MBED_CONF_MBED_MESH_API_WISUN_NETWORK_NAME
const char mesh_kcm_wisun_network_name[] = {MBED_CONF_MBED_MESH_API_WISUN_NETWORK_NAME};
#endif

#ifdef MBED_CONF_MBED_MESH_API_RADIUS_SERVER_IPV6_ADDRESS
const char mesh_kcm_radius_server_ipv6_addr[] = {MBED_CONF_MBED_MESH_API_RADIUS_SERVER_IPV6_ADDRESS};
#endif

#ifdef MBED_CONF_MBED_MESH_API_RADIUS_SHARED_SECRET
const uint8_t mesh_kcm_radius_shared_secret[] = {MBED_CONF_MBED_MESH_API_RADIUS_SHARED_SECRET};
#endif


/*
 * Weak definition for kcm_item_get_data_size.
 * Will be overwritten by actual implementation if it is available.
 */
MBED_WEAK kcm_status_e kcm_item_get_data_size(const uint8_t  *kcm_item_name,
                                              size_t          kcm_item_name_len,
                                              kcm_item_type_e kcm_item_type,
                                              size_t         *kcm_item_data_size_out)
{
    return KCM_STATUS_ITEM_NOT_FOUND;
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
    return KCM_STATUS_ITEM_NOT_FOUND;
}

int mesh_kcm_wisun_network_name_init(char **network_name_buf_ptr)
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
        *network_name_buf_ptr = (char *)kcm_item_buffer;
        config_status = 0;
    } else {
#ifdef MBED_CONF_MBED_MESH_API_WISUN_NETWORK_NAME
        *network_name_buf_ptr = (char *)mesh_kcm_wisun_network_name;
        config_status = 1;
#endif
    }

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

    if (kcm_status == KCM_STATUS_SUCCESS) {
        // network size as hundreds of devices
        *network_size = kcm_item_buffer[0];
        free(kcm_item_buffer);
        config_status = 0;
    } else {
#ifdef MBED_CONF_MBED_MESH_API_WISUN_NETWORK_SIZE
        *network_size = MBED_CONF_MBED_MESH_API_WISUN_NETWORK_SIZE;
        config_status = 0;
#endif
    }

    return config_status;
}

int mesh_kcm_wisun_network_regulatory_domain_init(uint8_t *regulatory_domain, uint8_t *operating_class, uint8_t *operating_mode)
{
    kcm_status_e kcm_status;
    uint8_t *kcm_item_buffer = NULL;
    size_t kcm_item_buff_size;

    // Initialise to default values
    *regulatory_domain = MBED_CONF_MBED_MESH_API_WISUN_REGULATORY_DOMAIN;
    *operating_class = MBED_CONF_MBED_MESH_API_WISUN_OPERATING_CLASS;
    *operating_mode = MBED_CONF_MBED_MESH_API_WISUN_OPERATING_MODE;

    // Init REGULATORY DOMAIN
    kcm_status = kcm_item_get_size_and_data((uint8_t *)MESH_WISUN_REGULATORY_DOMAIN,
                                            sizeof(MESH_WISUN_REGULATORY_DOMAIN) - 1,
                                            KCM_CONFIG_ITEM,
                                            &kcm_item_buffer,
                                            &kcm_item_buff_size);

    if (kcm_status == KCM_STATUS_SUCCESS) {
        // network size as hundreds of devices
        *regulatory_domain = kcm_item_buffer[0];
        free(kcm_item_buffer);
    }

    // Init OPERATING MODE
    kcm_status = kcm_item_get_size_and_data((uint8_t *)MESH_WISUN_OPERATING_MODE,
                                            sizeof(MESH_WISUN_OPERATING_MODE) - 1,
                                            KCM_CONFIG_ITEM,
                                            &kcm_item_buffer,
                                            &kcm_item_buff_size);

    if (kcm_status == KCM_STATUS_SUCCESS) {
        *operating_mode = kcm_item_buffer[0];
        free(kcm_item_buffer);
    }

    // Init OPERATING CLASS
    kcm_status = kcm_item_get_size_and_data((uint8_t *)MESH_WISUN_OPERATING_CLASS,
                                            sizeof(MESH_WISUN_OPERATING_CLASS) - 1,
                                            KCM_CONFIG_ITEM,
                                            &kcm_item_buffer,
                                            &kcm_item_buff_size);

    if (kcm_status == KCM_STATUS_SUCCESS) {
        *operating_class = kcm_item_buffer[0];
        free(kcm_item_buffer);
    }

    return 0;
}

int mesh_kcm_wisun_network_radius_addr_init(uint8_t **srv_addr, size_t *srv_addr_len)
{
    kcm_status_e kcm_status;
    int config_status = -1;
    uint8_t *kcm_item_buffer = NULL;
    size_t kcm_item_buff_size;

    kcm_status = kcm_item_get_size_and_data((uint8_t *)MESH_WISUN_RADIUS_SERVER_ADDRESS,
                                            sizeof(MESH_WISUN_RADIUS_SERVER_ADDRESS) - 1,
                                            KCM_CONFIG_ITEM,
                                            &kcm_item_buffer,
                                            &kcm_item_buff_size);

    if (kcm_status == KCM_STATUS_SUCCESS) {
        *srv_addr = kcm_item_buffer;
        *srv_addr_len = kcm_item_buff_size;
        config_status = 0;
    }

    if (config_status != 0) {
#ifdef MBED_CONF_MBED_MESH_API_RADIUS_SERVER_IPV6_ADDRESS
        *srv_addr = (uint8_t *)mesh_kcm_radius_server_ipv6_addr;
        *srv_addr_len = sizeof(mesh_kcm_radius_server_ipv6_addr);
        config_status = 1;
#else
        // Server address is not set in Mbed OS config, and not found from KCM
        config_status = -1;
#endif
    }

    return config_status;
}

int mesh_kcm_wisun_network_radius_secret_init(uint8_t **srv_secret_buf, size_t *actual_secret_len)
{
    kcm_status_e kcm_status;
    int config_status = -1;
    uint8_t *kcm_item_buffer = NULL;
    size_t kcm_item_buff_size;

    kcm_status = kcm_item_get_size_and_data((uint8_t *)MESH_WISUN_RADIUS_SERVER_SECRET,
                                            sizeof(MESH_WISUN_RADIUS_SERVER_SECRET) - 1,
                                            KCM_CONFIG_ITEM,
                                            &kcm_item_buffer,
                                            &kcm_item_buff_size);

    if (kcm_status == KCM_STATUS_SUCCESS) {
        *srv_secret_buf =  kcm_item_buffer;
        *actual_secret_len = kcm_item_buff_size;
        config_status = 0;
    }

    if (config_status < 0) {
#ifdef MBED_CONF_MBED_MESH_API_RADIUS_SHARED_SECRET
        size_t cfg_secret_len;
#ifdef MBED_CONF_MBED_MESH_API_RADIUS_SHARED_SECRET_LEN
        cfg_secret_len = MBED_CONF_MBED_MESH_API_RADIUS_SHARED_SECRET_LEN;
#else
        cfg_secret_len = sizeof(MBED_CONF_MBED_MESH_API_RADIUS_SHARED_SECRET) - 1;
#endif
        *srv_secret_buf = (uint8_t *)mesh_kcm_radius_shared_secret;
        *actual_secret_len = cfg_secret_len;
        config_status = 1;
#else
        // Server shared secret not set in Mbed OS config, and not found from KCM
        config_status = -1;
#endif /* MBED_CONF_MBED_MESH_API_RADIUS_SHARED_SECRET */
    }

    return config_status;
}

//#define MESH_KCM_TEST_DATA
#ifdef MESH_KCM_TEST_DATA
void mesh_kcm_test_data_store(void)
{

    static bool initialized = false;

    if (initialized) {
        return;
    }
    initialized = true;

    kcm_status_e kcm_status;

    static const char mesh_kcm_network_name[] = "KCM-Network-name";
    static const uint8_t mesh_kcm_network_size = 2;
    static const uint8_t mesh_kcm_regulatory_domain = 5; //india
    static const uint8_t mesh_kcm_operating_mode = 255;
    static const uint8_t mesh_kcm_operating_class = 1;
    static const char mesh_kcm_radius_srv_addr[] = "2001:14b8:1830:b000:1e69:7aff:fe03:dad7";
    static const char mesh_kcm_radius_srv_secret[] = "wisun_radius_password";

    tr_info("Set mesh_kcm_network_name: %s", mesh_kcm_network_name);
    kcm_status = kcm_item_store((uint8_t *)MESH_WISUN_NETWORK_NAME_KEY,
                                sizeof(MESH_WISUN_NETWORK_NAME_KEY) - 1,
                                KCM_CONFIG_ITEM,
                                true,
                                (uint8_t *)mesh_kcm_network_name,
                                sizeof(mesh_kcm_network_name),
                                NULL);
    if (kcm_status != KCM_STATUS_SUCCESS) {
        tr_error("Failed to set mesh_kcm_network_name");
    }

    tr_info("Set mesh_kcm_network_size: %d", mesh_kcm_network_size);
    kcm_status = kcm_item_store((uint8_t *)MESH_WISUN_NETWORK_SIZE_KEY,
                                sizeof(MESH_WISUN_NETWORK_SIZE_KEY) - 1,
                                KCM_CONFIG_ITEM,
                                true,
                                (uint8_t *)&mesh_kcm_network_size,
                                sizeof(mesh_kcm_network_size),
                                NULL);
    if (kcm_status != KCM_STATUS_SUCCESS) {
        tr_error("Failed to set mesh_kcm_network_size");
    }

    tr_info("Set mesh_kcm_regulatory_domain: %d", mesh_kcm_regulatory_domain);
    kcm_status = kcm_item_store((uint8_t *)MESH_WISUN_REGULATORY_DOMAIN,
                                sizeof(MESH_WISUN_REGULATORY_DOMAIN) - 1,
                                KCM_CONFIG_ITEM,
                                true,
                                (uint8_t *)&mesh_kcm_regulatory_domain,
                                sizeof(mesh_kcm_regulatory_domain),
                                NULL);
    if (kcm_status != KCM_STATUS_SUCCESS) {
        tr_error("Failed to set mesh_kcm_regulatory_domain");
    }

    tr_info("Set mesh_kcm_operating_class: %d", mesh_kcm_operating_class);
    kcm_status = kcm_item_store((uint8_t *)MESH_WISUN_OPERATING_CLASS,
                                sizeof(MESH_WISUN_OPERATING_CLASS) - 1,
                                KCM_CONFIG_ITEM,
                                true,
                                (uint8_t *)&mesh_kcm_operating_class,
                                sizeof(mesh_kcm_operating_class),
                                NULL);
    if (kcm_status != KCM_STATUS_SUCCESS) {
        tr_error("Failed to set mesh_kcm_operating_class");
    }

    tr_info("Set mesh_kcm_operating_mode: %d", mesh_kcm_operating_mode);
    kcm_status = kcm_item_store((uint8_t *)MESH_WISUN_OPERATING_MODE,
                                sizeof(MESH_WISUN_OPERATING_MODE) - 1,
                                KCM_CONFIG_ITEM,
                                true,
                                (uint8_t *)&mesh_kcm_operating_mode,
                                sizeof(mesh_kcm_operating_mode),
                                NULL);
    if (kcm_status != KCM_STATUS_SUCCESS) {
        tr_error("Failed to set mesh_kcm_operating_mode");
    }


    tr_info("Set mesh_kcm_radius_srv_addr: %s", mesh_kcm_radius_srv_addr);
    kcm_status = kcm_item_store((uint8_t *)MESH_WISUN_RADIUS_SERVER_ADDRESS,
                                sizeof(MESH_WISUN_RADIUS_SERVER_ADDRESS) - 1,
                                KCM_CONFIG_ITEM,
                                true,
                                (uint8_t *)mesh_kcm_radius_srv_addr,
                                strlen(mesh_kcm_radius_srv_addr) + 1,
                                NULL);
    if (kcm_status != KCM_STATUS_SUCCESS) {
        tr_error("Failed to set mesh_kcm_radius_srv_addr");
    }

    tr_info("Set mesh_kcm_radius_srv_secret: %s", tr_array((uint8_t *)mesh_kcm_radius_srv_secret, sizeof(mesh_kcm_radius_srv_secret) / sizeof(uint8_t)));
    kcm_status = kcm_item_store((uint8_t *)MESH_WISUN_RADIUS_SERVER_SECRET,
                                sizeof(MESH_WISUN_RADIUS_SERVER_SECRET) - 1,
                                KCM_CONFIG_ITEM,
                                true,
                                (uint8_t *)mesh_kcm_radius_srv_secret,
                                sizeof(mesh_kcm_radius_srv_secret) / sizeof(uint8_t) -1,
                                NULL);
    if (kcm_status != KCM_STATUS_SUCCESS) {
        tr_error("Failed to set mesh_kcm_radius_srv_secret");
    }
}

void mesh_kcm_test_data_delete(void)
{
    kcm_item_delete((uint8_t *)MESH_WISUN_NETWORK_NAME_KEY, strlen(MESH_WISUN_NETWORK_NAME_KEY), KCM_CONFIG_ITEM);
    kcm_item_delete((uint8_t *)MESH_WISUN_NETWORK_SIZE_KEY, strlen(MESH_WISUN_NETWORK_SIZE_KEY), KCM_CONFIG_ITEM);
    kcm_item_delete((uint8_t *)MESH_WISUN_REGULATORY_DOMAIN, strlen(MESH_WISUN_REGULATORY_DOMAIN), KCM_CONFIG_ITEM);
    kcm_item_delete((uint8_t *)MESH_WISUN_OPERATING_CLASS, strlen(MESH_WISUN_OPERATING_CLASS), KCM_CONFIG_ITEM);
    kcm_item_delete((uint8_t *)MESH_WISUN_OPERATING_MODE, strlen(MESH_WISUN_OPERATING_MODE), KCM_CONFIG_ITEM);
    kcm_item_delete((uint8_t *)MESH_WISUN_RADIUS_SERVER_ADDRESS, strlen(MESH_WISUN_RADIUS_SERVER_ADDRESS), KCM_CONFIG_ITEM);
    kcm_item_delete((uint8_t *)MESH_WISUN_RADIUS_SERVER_SECRET, strlen(MESH_WISUN_RADIUS_SERVER_SECRET), KCM_CONFIG_ITEM);
}

#endif
