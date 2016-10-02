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

/**
 * \file mac_api.h
 * \brief A API class to support different MACs from multiple vendors.
 * Vendor must implement a function which fills supported callback functions which Upper layer will use.
 */

#ifndef MAC_API_H
#define MAC_API_H

#include <inttypes.h>
#include "mlme.h"
#include "mac_mcps.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mac_api_s mac_api_t;

/**
 * Every MAC adapting to Upper layer must implement a function which creates mac_api_t pointer, e.g 'mac_api_t* create_mac_api();'
 * In the function external Mac needs to fill necessary function pointers so that Upper layer can use it.
 * For Nanostack to work, following (mlme/mcps) request functions are mandatory:
 *   - mcps-data
 *   - scan
 *   - start
 *   - poll
 *   - get
 *   - set
 *   - reset
 *   - (purge)
 * Also indication and confirm callbacks for above are needed plus
 *   - beacon notify
 *   - comm status
 *
 * \return mac_api_t Ownership of newly created object
 */

typedef enum {
    MLME_ASSOCIATE,
    MLME_DISASSOCIATE,
    MLME_BEACON_NOTIFY,
    MLME_GET,
    MLME_GTS,
    MLME_ORPHAN,
    MLME_RESET,
    MLME_RX_ENABLE,
    MLME_SCAN,
    MLME_COMM_STATUS,
    MLME_SET,
    MLME_START,
    MLME_SYNC,
    MLME_SYNC_LOSS,
    MLME_POLL
} mlme_primitive;


typedef struct mac_description_storage_size_s {
    uint8_t device_decription_table_size;   /** MAC Device description list size */
    uint8_t key_description_table_size;     /** MAC Key description list size */
    uint8_t key_lookup_size;                /** Key description key lookup list size */
    uint8_t key_usage_size;                 /** Key description key usage list size */
} mac_description_storage_size_t;

typedef enum mac_extended_address_type {
    MAC_EXTENDED_READ_ONLY, /** EUID64 which is unique */
    MAC_EXTENDED_DYNAMIC /** Configured MAC 64-bit address to RAM and Radio */
}mac_extended_address_type;
//External MAC functions

/**
 * @brief mlme_associate_response
 * @param api API to handle the request
 * @param data MLME-ASSOCIATE.response specific values
 */
//typedef void mlme_associate_response(const mac_api_t* api, mlme_associate_resp_t *data);

//typedef void mlme_orphan_response(const mac_api_t* api, mlme_orphan_resp_t *data);

/**
 * @brief mlme_request
 * @param api API to handle the request
 * @param id The identifier of the MLME primitive
 * @param data Primitive specific data (\see mlme.h)
 */
typedef void mlme_request(const mac_api_t* api, mlme_primitive id, const void *data);

/**
 * @brief mcps_request MCPS_DATA request call
 * @param api API to handle the request
 * @param data MCPS-DATA.request specific values
 */
typedef void mcps_data_request(const mac_api_t* api, const mcps_data_req_t *data);

/**
 * @brief mcps_purge_request MCPS_PURGE request call
 * @param api API to handle the request
 * @param data MCPS-PURGE.request specific values
 */
typedef void mcps_purge_request(const mac_api_t* api, const mcps_purge_t *data);

//Upper layer specific callback functions (will also be set by Upper layer after mac_api_t has been created and given to it)

/**
 * @brief mcps_data_confirm MCPS-DATA confirm is called as a response to MCPS-DATA request
 * @param api The API which handled the response
 * @param data MCPS-DATA.confirm specific values
 */
typedef void mcps_data_confirm(const mac_api_t* api, const mcps_data_conf_t *data);

/**
 * @brief mcps_data_indication MCPS-DATA indication is called when MAC layer has received data
 * @param api The API which handled the response
 * @param data MCPS-DATA.indication specific values
 */
typedef void mcps_data_indication(const mac_api_t* api, const mcps_data_ind_t *data);

/**
 * @brief mcps_purge_confirm MCPS-PURGE confirm is called as a response to MCPS-PURGE request
 * @param api The API which handled the request
 * @param data MCPS-PURGE.confirm specific values
 */
typedef void mcps_purge_confirm( const mac_api_t* api, mcps_purge_conf_t *data );

/**
 * @brief mlme_confirm One of the MLME primitive confirm callbacks
 * @param api API which handled the response
 * @param id The identifier of the MLME primitive
 * @param data Primitive specific data (\see mlme.h)
 */
typedef void mlme_confirm(const mac_api_t* api, mlme_primitive id, const void *data);

/**
 * @brief mlme_indication One of the
 * @param api API which handled the response
 * @param id The identifier of the MLME primitive
 * @param data Primitive specific data (\see mlme.h)
 */
typedef void mlme_indication(const mac_api_t* api, mlme_primitive id, const void *data);

/**
 * @brief Set extended address from MAC
 * @param api API to handle the request
 * @param mac64 pointer
 */
typedef int8_t mac_ext_mac64_address_set(const mac_api_t* api, const uint8_t *mac64);

/**
 * @brief Read extended address from MAC
 * @param api API to handle the request
 * @param mac64_buf Pointer where mac extended address can be written
 */
typedef int8_t mac_ext_mac64_address_get(const mac_api_t* api, mac_extended_address_type type, uint8_t *mac64_buf);

/**
 * @brief Read MAC security description storage sizes from MAC
 * @param api API to handle the request
 * @param buffer Pointer where supported sizes can be written
 */
typedef int8_t mac_storage_decription_sizes_get(const mac_api_t* api, mac_description_storage_size_t *buffer);

/**
 * @brief mac_api_initialize Initialises MAC layer into use, callbacks must be non-NULL.
 * @param api mac_api_t pointer, which is created by application.
 * @param data_conf_cb Upper layer function to handle MCPS confirmations
 * @param data_ind_cb Upper layer function to handle MCPS indications
 * @param mlme_conf_cb Upper layer function to handle MLME confirmations
 * @param mlme_ind_cb Upper layer function to handle MLME indications
 * @param parent_id Upper layer id, which is used in confirmation and indication callbacks
 * @return -1 if error, -2 if OOM, 0 otherwise
 */
typedef int8_t mac_api_initialize(mac_api_t *api, mcps_data_confirm *data_conf_cb,
                                  mcps_data_indication *data_ind_cb, mcps_purge_confirm *purge_conf_cb, mlme_confirm *mlme_conf_cb,
                                  mlme_indication *mlme_ind_cb, int8_t parent_id);

/**
 * \brief Struct mac_api_s defines functions for two-way communications between external MAC and Upper layer.
 * Application creates mac_api_t object by calling external MAC's creator function.
 * Then object is passed to Upper layer which then initializes it's own callback functions.
 * Then MAC is operated by Upper layer by calling MLME or MCPS primitive functions.
 */
struct mac_api_s {
    mac_api_initialize          *mac_initialize;
    //External MAC callbacks
    mlme_request                *mlme_req;
    mcps_data_request           *mcps_data_req;
    mcps_purge_request          *mcps_purge_req;

    //Upper layer callbacksMLME_ASSOCIATE
    mcps_data_confirm           *data_conf_cb;
    mcps_data_indication        *data_ind_cb;
    mcps_purge_confirm          *purge_conf_cb;
    mlme_confirm                *mlme_conf_cb;
    mlme_indication             *mlme_ind_cb;
    mac_ext_mac64_address_set   *mac64_set;
    mac_ext_mac64_address_get   *mac64_get;
    mac_storage_decription_sizes_get *mac_storage_sizes_get;

    int8_t                      parent_id;
    uint16_t                    phyMTU;
};

#ifdef __cplusplus
}
#endif

#endif // MAC_API_H
