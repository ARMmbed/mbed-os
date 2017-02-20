/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and limitations under the License.
 *
 */

/** \file ethernet_mac_api.h
 * \brief Ethernet MAC API
 */

#ifndef ETHERNET_MAC_API_H
#define ETHERNET_MAC_API_H

#include <inttypes.h>
#include "platform/arm_hal_phy.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ETHERTYPE_IPV4              0x0800
#define ETHERTYPE_ARP               0x0806
#define ETHERTYPE_802_1Q_TAG        0x8100
#define ETHERTYPE_IPV6              0x86dd

typedef struct eth_mac_api_s eth_mac_api_t;

typedef struct eth_data_conf_s {
    uint8_t msduHandle;
    uint8_t status;
}eth_data_conf_t;

typedef struct eth_data_req_s {
    uint16_t msduLength;
    uint8_t *msdu;
    uint8_t *srcAddress;
    uint8_t *dstAddress;
    uint16_t etehernet_type;
    uint8_t msduHandle;
} eth_data_req_t;

typedef struct eth_data_ind_s {
    uint16_t msduLength;
    uint8_t *msdu;
    uint8_t srcAddress[6];
    uint8_t dstAddress[6];
    uint16_t etehernet_type;
    uint8_t link_quality;
    int8_t dbm;
}eth_data_ind_t;

/**
 * @brief Creates ethernet MAC API instance which will use driver given
 * @param driver_id Ethernet driver id. Must be valid
 * @return New MAC instance if successful, NULL otherwise
 */
extern eth_mac_api_t *ethernet_mac_create(int8_t driver_id);

/**
 * @brief Destroy ethernet MAC API instance
 * Call this only for freeing all allocated memory and when mac is total unused
 *
 * @param mac_api Removed mac class pointer
 * @return -1 Unknow MAC
 * @return 0 Mac class is removed
 */
extern int8_t ethernet_mac_destroy(eth_mac_api_t *mac_api);

/**
 * @brief data_request data request call
 * @param api API to handle the request
 * @param data Data containing request parameters
 */
typedef void eth_mac_data_request(const eth_mac_api_t* api, const eth_data_req_t *data);

/**
 * @brief data_confirm confirm is called as a response to data_request
 * @param api The API which handled the request
 * @param data Data containing confirm parameters
 */
typedef void eth_mac_data_confirm(const eth_mac_api_t* api, const eth_data_conf_t *data );

/**
 * @brief data_indication Data indication is called when MAC layer has received data
 * @param api The API which handled the response
 * @param data Data containing indication parameters
 */
typedef void eth_mac_data_indication(const eth_mac_api_t* api, const eth_data_ind_t *data );

/**
 * @brief Set 48 bit address from MAC
 * @param api API to handle the request
 * @param mac48 Pointer having mac address to be set
 * @return 0 if successful, -1 otherwise
 */
typedef int8_t eth_mac_mac48_address_set(const eth_mac_api_t* api, const uint8_t *mac48);

/**
 * @brief Read 48 bit address from MAC
 * @param api API to handle the request
 * @param mac48_buf Pointer where mac address can be written
 * @return 0 if successful, -1 otherwise
 */
typedef int8_t eth_mac_mac48_address_get(const eth_mac_api_t* api, uint8_t *mac48_buf);

/**
 * @brief Upper layer will call this function, when MAC is taken into use
 * @param api API to initialize
 * @param conf_cb Callback for confirm type of messages
 * @param ind_cb Callback for indication type of messages
 * @param parent_id Upper layer identifier
 * @return 0 if success; -1 if api is NULL or not found
 */
typedef int8_t eth_mac_api_initialize(eth_mac_api_t *api, eth_mac_data_confirm *conf_cb,
                                       eth_mac_data_indication *ind_cb, uint8_t parent_id);

struct eth_mac_api_s {
    eth_mac_api_initialize      *mac_initialize;

    eth_mac_data_request                *data_req;
    eth_mac_data_confirm                *data_conf_cb;
    eth_mac_data_indication             *data_ind_cb;

    eth_mac_mac48_address_set   *mac48_set;
    eth_mac_mac48_address_get   *mac48_get;

    uint8_t                     parent_id;
    bool                        address_resolution_needed; //Normal ethernet should set this true and tunnel or slip false
};

#ifdef __cplusplus
}
#endif

#endif // ETHERNET_MAC_API_H

