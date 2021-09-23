/*
 * Copyright (c) 2016-2018, 2020-2021, Pelion and affiliates.
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

struct channel_list_s;

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

/*!
 * \enum mlme_primitive
 * \brief Enum for MLME primitive types.
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

/**
 * \struct mac_description_storage_size_s
 * \brief Container for MAC storage sizes.
 */
typedef struct mac_description_storage_size_s {
    uint8_t device_decription_table_size;   /**< MAC Device description list size */
    uint8_t key_description_table_size;     /**< MAC Key description list size */
    uint8_t key_lookup_size;                /**< Key description key lookup list size */
    uint8_t key_usage_size;                 /**< Key description key usage list size */
} mac_description_storage_size_t;

/*!
 * \enum mac_extended_address_type
 * \brief Enum for MAC extended address types.
 */
typedef enum mac_extended_address_type {
    MAC_EXTENDED_READ_ONLY, /** EUID64 which is unique */
    MAC_EXTENDED_DYNAMIC /** Configured MAC 64-bit address to RAM and Radio */
} mac_extended_address_type;
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
typedef void mlme_request(const mac_api_t *api, mlme_primitive id, const void *data);

/**
 * @brief mcps_request MCPS_DATA request call
 * @param api API to handle the request
 * @param data MCPS-DATA.request specific values
 */
typedef void mcps_data_request(const mac_api_t *api, const mcps_data_req_t *data);

/**
 * @brief mcps_request MCPS_DATA with IE extions request call
 * @param api API to handle the request
 * @param data MCPS-DATA.request specific values
 * @param ie_ext Information element list to MCPS-DATA.request
 * @param asynch_channel_list Optional channel list to asynch data request. Give NULL when normal data request.
 * @param priority Data request priority level
 * @param phy_mode_id Use mode switch if given phy_mode_id > 0
 *
 * Asynch data request is mac standard extension. asynch_channel_list include channel mask which channel message is requested to send.
 */
typedef void mcps_data_request_ext(const mac_api_t *api, const mcps_data_req_t *data, const mcps_data_req_ie_list_t *ie_ext, const struct channel_list_s *asynch_channel_list, mac_data_priority_t priority, uint8_t phy_mode_id);

/**
 * @brief mcps_purge_request MCPS_PURGE request call
 * @param api API to handle the request
 * @param data MCPS-PURGE.request specific values
 * @return 0 in case of success, non-zero otherwise
 */
typedef uint8_t mcps_purge_request(const mac_api_t *api, const mcps_purge_t *data);

//Upper layer specific callback functions (will also be set by Upper layer after mac_api_t has been created and given to it)

/**
 * @brief mcps_data_confirm MCPS-DATA confirm is called as a response to MCPS-DATA request
 * @param api The API which handled the response
 * @param data MCPS-DATA.confirm specific values
 */
typedef void mcps_data_confirm(const mac_api_t *api, const mcps_data_conf_t *data);

/**
 * @brief mcps_data_confirm_ext MCPS-DATA confirm with Enhanced ACK payload is called as a response to MCPS-DATA request
 * @param api The API which handled the response
 * @param data MCPS-DATA.confirm specific values
 * @param conf_data Possible Confirmation Data
 */
typedef void mcps_data_confirm_ext(const mac_api_t *api, const mcps_data_conf_t *data, const mcps_data_conf_payload_t *conf_data);

/**
 * @brief mcps_data_indication MCPS-DATA indication is called when MAC layer has received data
 * @param api The API which handled the response
 * @param data MCPS-DATA.indication specific values
 */
typedef void mcps_data_indication(const mac_api_t *api, const mcps_data_ind_t *data);

/**
 * @brief mcps_data_indication MCPS-DATA indication is called when MAC layer has received data
 * @param api The API which handled the response
 * @param data MCPS-DATA.indication specific values
 * @param ie_ext Information element list
 */
typedef void mcps_data_indication_ext(const mac_api_t *api, const mcps_data_ind_t *data, const mcps_data_ie_list_t *ie_ext);

/**
 * @brief mcps_ack_data_req_ext Callback for request IE elements and payload to enhanced ACK
 * @param api The API which handled the response
 * @param data Pointer where MAC user set Payload and IE element pointers and length
 * @param rssi Signal strength for received packet
 * @param lqi Link quality to neighbor
 */
typedef void mcps_ack_data_req_ext(const mac_api_t *api, mcps_ack_data_payload_t *data, int8_t rssi, uint8_t lqi);


/**
 * @brief mcps_edfe_handler Callback for handle and detect edfe frame
 * @param api The API which handled the response
 * @param response_message Enhanced message response data and status
 */
typedef void mcps_edfe_handler(const mac_api_t *api, mcps_edfe_response_t *response_message);


/**
 * @brief mode_switch_resolver Callback to resolve configuration behind received PHY mode ID
 * @param api The API which handled the response
 * @param phy_mode_id PHY mode ID to be resolved
 * @param rf_config Resolved configuration
 * @return 0 in case of success, negative otherwise
 */
typedef int8_t mode_switch_resolver(const mac_api_t *api, uint8_t phy_mode_id, phy_rf_channel_configuration_s *rf_config);


/**
 * @brief mcps_purge_confirm MCPS-PURGE confirm is called as a response to MCPS-PURGE request
 * @param api The API which handled the request
 * @param data MCPS-PURGE.confirm specific values
 */
typedef void mcps_purge_confirm(const mac_api_t *api, mcps_purge_conf_t *data);

/**
 * @brief mlme_confirm One of the MLME primitive confirm callbacks
 * @param api API which handled the response
 * @param id The identifier of the MLME primitive
 * @param data Primitive specific data (\see mlme.h)
 */
typedef void mlme_confirm(const mac_api_t *api, mlme_primitive id, const void *data);

/**
 * @brief mlme_indication One of the
 * @param api API which handled the response
 * @param id The identifier of the MLME primitive
 * @param data Primitive specific data (\see mlme.h)
 */
typedef void mlme_indication(const mac_api_t *api, mlme_primitive id, const void *data);

/**
 * @brief Set extended address from MAC
 * @param api API to handle the request
 * @param mac64 pointer
 */
typedef int8_t mac_ext_mac64_address_set(const mac_api_t *api, const uint8_t *mac64);

/**
 * @brief Read extended address from MAC
 * @param api API to handle the request
 * @param mac64_buf Pointer where mac extended address can be written
 */
typedef int8_t mac_ext_mac64_address_get(const mac_api_t *api, mac_extended_address_type type, uint8_t *mac64_buf);

/**
 * @brief Read MAC security description storage sizes from MAC
 * @param api API to handle the request
 * @param buffer Pointer where supported sizes can be written
 */
typedef int8_t mac_storage_decription_sizes_get(const mac_api_t *api, mac_description_storage_size_t *buffer);

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
 * @brief mac_api_enable_mcps_ext Initialises MAC 2015 extension for MCPS layer into use, callbacks must be non-NULL.
 * @param api mac_api_t pointer, which is created by application.
 * @param data_ind_cb Upper layer function to handle MCPS indications
 * @param data_cnf_cb Upper layer function to handle MCPS confirmation
 * @param ack_data_req_cb Upper layer function for set Enhanced ACK payload
 * @return -1 if error, -2 if OOM, 0 otherwise
 */
typedef int8_t mac_api_enable_mcps_ext(mac_api_t *api,
                                       mcps_data_indication_ext *data_ind_cb,
                                       mcps_data_confirm_ext *data_cnf_cb,
                                       mcps_ack_data_req_ext *ack_data_req_cb);

/**
 * @brief mac_api_enable_mcps_edfe_ext Initialises MAC 2015 extension for EDFE handler callbacks must be non-NULL.
 * @param api mac_api_t pointer, which is created by application.
 * @param edfe_ind_cb Upper layer function to handle and detect EDFE's
 * @return -1 if error, -2 if OOM, 0 otherwise
 */
typedef int8_t mac_api_enable_mcps_edfe_ext(mac_api_t *api,
                                            mcps_edfe_handler *edfe_ind_cb);

/**
 * @brief mac_api_mode_switch_resolver_ext Initialises mode switch resolver callback. Upper layer must configure function when mode switch is used.
 * @param api mac_api_t pointer, which is created by application.
 * @param mode_resolver_cb Upper layer function to resolve received PHY mode ID
 * @param base_phy_mode Base PHY mode, device returns to this mode after mode switch transmission or reception
 * @return -1 if error, 0 otherwise
 */
typedef int8_t mac_api_mode_switch_resolver_ext(mac_api_t *api,
                                                mode_switch_resolver *mode_resolver_cb, uint8_t base_phy_mode);

/**
 * \brief Struct mac_api_s defines functions for two-way communications between external MAC and Upper layer.
 * Application creates mac_api_t object by calling external MAC's creator function.
 * Then object is passed to Upper layer which then initializes it's own callback functions.
 * Then MAC is operated by Upper layer by calling MLME or MCPS primitive functions.
 */
struct mac_api_s {
    mac_api_initialize                  *mac_initialize;                /**< MAC initialize function to use */
    mac_api_enable_mcps_ext             *mac_mcps_extension_enable;     /**< MAC MCPS IE extension enable function, optional feature */
    mac_api_enable_mcps_edfe_ext        *mac_mcps_edfe_enable;          /**< MAC MCPS MCPS EDFE frame extension enable function, optional feature */
    mac_api_mode_switch_resolver_ext    *mac_mode_switch_resolver_set;  /**< MAC Mode switch resolver function set, optional feature */
    //External MAC callbacks
    mlme_request                        *mlme_req;                      /**< MAC MLME request function to use */
    mcps_data_request                   *mcps_data_req;                 /**< MAC MCPS data request function to use */
    mcps_data_request_ext               *mcps_data_req_ext;             /**< MAC MCPS data request with Information element extension function to use */
    mcps_purge_request                  *mcps_purge_req;                /**< MAC MCPS purge request function to use */
    //Upper layer callbacksMLME_ASSOCIATE
    mcps_data_confirm                   *data_conf_cb;                  /**< MAC MCPS data confirm callback function */
    mcps_data_confirm_ext               *data_conf_ext_cb;              /**< MAC MCPS data confirm with payload callback function */
    mcps_data_indication                *data_ind_cb;                   /**< MAC MCPS data indication callback function */
    mcps_data_indication_ext            *data_ind_ext_cb;               /**< MAC MCPS data indication with IE extension's callback function */
    mcps_edfe_handler                   *edfe_ind_cb;                   /**< MAC MCPS EDFE detection extension's callback function */
    mode_switch_resolver                *mode_resolver_cb;              /**< MAC Mode switch resolver callback function */
    mcps_ack_data_req_ext               *enhanced_ack_data_req_cb;      /**< Enhanced ACK IE element and payload request from MAC user */
    mcps_purge_confirm                  *purge_conf_cb;                 /**< MAC MCPS purge confirm callback function */
    mlme_confirm                        *mlme_conf_cb;                  /**< MAC MLME confirm callback function */
    mlme_indication                     *mlme_ind_cb;                   /**< MAC MLME indication callback function */
    mac_ext_mac64_address_set           *mac64_set;                     /**< MAC extension function to set mac64 address */
    mac_ext_mac64_address_get           *mac64_get;                     /**< MAC extension function to get mac64 address */
    mac_storage_decription_sizes_get    *mac_storage_sizes_get;         /**< Getter function to query data storage sizes from MAC */

    int8_t                              parent_id;                      /**< Upper layer id */
    uint16_t                            phyMTU;                         /**< Maximum Transmission Unit(MTU) used by MAC*/
};

/**
 * \struct mac_statistics_s
 * \brief MAC statistics structure.
 */
typedef struct mac_statistics_s {
    uint16_t mac_tx_queue_size;         /**< MAC TX queue current size. */
    uint16_t mac_tx_queue_peak;         /**< MAC TX queue peak size. */
    uint32_t mac_rx_count;              /**< MAC RX packet count. */
    uint32_t mac_tx_count;              /**< MAC TX packet count. */
    uint32_t mac_bc_rx_count;           /**< MAC broadcast RX packet count. */
    uint32_t mac_bc_tx_count;           /**< MAC broadcast TX packet count. */
    uint32_t mac_beacon_rx_count;       /**< MAC Beacon RX packet count. */
    uint32_t mac_beacon_tx_count;       /**< MAC Beacon TX packet count. */
    uint32_t mac_rx_drop_count;         /**< MAC RX packet drop count. */
    uint32_t mac_tx_bytes;              /**< MAC TX bytes count. */
    uint32_t mac_rx_bytes;              /**< MAC RX bytes count. */
    uint32_t mac_tx_failed_count;       /**< MAC TX failed count. */
    uint32_t mac_retry_count;           /**< MAC TX retry count. */
    uint32_t mac_cca_attempts_count;    /**< MAC CCA attempts count. */
    uint32_t mac_failed_cca_count;      /**< MAC failed CCA count. */
    uint32_t mac_tx_latency_max;        /**< MAC data request max latency. */
} mac_statistics_t;

#ifdef __cplusplus
}
#endif

#endif // MAC_API_H
