/*
 * Copyright (c) 2016-2017 ARM Limited. All rights reserved.
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

#define ETHERTYPE_IPV4              0x0800  /**< ethernet type for IPv4 */
#define ETHERTYPE_ARP               0x0806  /**< ethernet type for ARP */
#define ETHERTYPE_802_1Q_TAG        0x8100  /**< ethernet type for 802_1Q */
#define ETHERTYPE_IPV6              0x86dd  /**< ethernet type for IPv6 */

typedef struct eth_mac_api_s eth_mac_api_t;

/**
  * \brief Struct eth_data_conf_s defines arguments for data confirm message
  */
typedef struct eth_data_conf_s {
    uint8_t msduHandle;         /**< Handle associated with MSDU */
    uint8_t status;             /**< Status of the last transaction */
}eth_data_conf_t;

/**
  * \brief Struct eth_data_req_s defines arguments for data request message
  */
typedef struct eth_data_req_s {
    uint16_t msduLength;        /**< Service data unit length */
    uint8_t *msdu;              /**< Service data unit */
    uint8_t *srcAddress;        /**< Source address */
    uint8_t *dstAddress;        /**< Destination address */
    uint16_t etehernet_type;    /**< Ethernet type */
    uint8_t msduHandle;         /**< Handle associated with MSDU */
} eth_data_req_t;

/**
  * \brief Struct eth_data_ind_s defines arguments for data indication message
  */
typedef struct eth_data_ind_s {
    uint16_t msduLength;        /**< Service data unit length */
    uint8_t *msdu;              /**< Service data unit */
    uint8_t srcAddress[6];      /**< Source address */
    uint8_t dstAddress[6];      /**< Destination address */
    uint16_t etehernet_type;    /**< Ethernet type */
    uint8_t link_quality;       /**< Link quality */
    int8_t dbm;                 /**< measured dBm */
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

/**
  * \brief Struct eth_mac_api_s defines functions for two-way communications between ethernet MAC and Upper layer.
  */
struct eth_mac_api_s {
    eth_mac_api_initialize      *mac_initialize;                /**< Callback function for MAC initialization */

    eth_mac_data_request        *data_req;                      /**< Callback function for data request */
    eth_mac_data_confirm        *data_conf_cb;                  /**< Callback function for data confirmation */
    eth_mac_data_indication     *data_ind_cb;                   /**< Callback function for data indication */

    eth_mac_mac48_address_set   *mac48_set;                     /**< Setter for MAC address */
    eth_mac_mac48_address_get   *mac48_get;                     /**< Getter for MAC address */

    uint8_t                     parent_id;                      /**< Upper layer ID */
    bool                        address_resolution_needed;      /**< Normal ethernet should set this true for tunnel or false for slip */
};

#ifdef __cplusplus
}
#endif

#endif // ETHERNET_MAC_API_H

