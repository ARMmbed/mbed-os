/*
 * Copyright (c) 2018-2021, Pelion and affiliates.
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

#ifndef WS_LLC_H_
#define WS_LLC_H_

#include "6LoWPAN/ws/ws_neighbor_class.h"
#include "Service_Libs/mac_neighbor_table/mac_neighbor_table.h"

struct protocol_interface_info_entry;
struct mcps_data_ind_s;
struct mcps_data_ie_list;
struct channel_list_s;
struct ws_pan_information_s;
struct mlme_security_s;
struct ws_hopping_schedule_s;
struct ws_neighbor_class_entry;
struct mac_neighbor_table_entry;
struct ws_neighbor_temp_class_s;


/**
 * @brief wh_ie_sub_list_t ws asynch header IE elemnt request list
 */
typedef struct wh_ie_sub_list_s {
    bool utt_ie: 1;  /**< Unicast Timing and Frame type information */
    bool bt_ie: 1;   /**< Broadcast timing information */
    bool fc_ie: 1;   /**< Flow Control for Extended Direct Frame Exchange */
    bool rsl_ie: 1;  /**< Received Signal Level information */
    bool vh_ie: 1;   /**< Vendor header information */
    bool ea_ie: 1;   /**< EAPOL autheticator EUI-64 header information */
} wh_ie_sub_list_t;

/**
 * @brief wp_nested_ie_sub_list_t ws asynch Nested Payload sub IE element request list
 */
typedef struct wp_nested_ie_sub_list_s {
    bool us_ie: 1;          /**< Unicast Schedule information */
    bool bs_ie: 1;          /**< Broadcast Schedule information */
    bool vp_ie: 1;          /**< Vendor Payload information */
    bool pan_ie: 1;         /**< PAN Information */
    bool net_name_ie: 1;    /**< Network Name information */
    bool pan_version_ie: 1; /**< Pan configuration version */
    bool gtkhash_ie: 1;     /**< GTK Hash information */
} wp_nested_ie_sub_list_t;

/**
 * @brief asynch_request_t Asynch message request parameters
 */
typedef struct asynch_request_s {
    unsigned  message_type: 3;                              /**< Asynch message type: WS_FT_PAN_ADVERT, WS_FT_PAN_ADVERT_SOL, WS_FT_PAN_CONF or WS_FT_PAN_CONF_SOL. */
    wh_ie_sub_list_t wh_requested_ie_list;                  /**< WH-IE header list to message. */
    wp_nested_ie_sub_list_t wp_requested_nested_ie_list;    /**< WP-IE Nested IE list to message. */
    struct mlme_security_s security;                               /**< Request MAC security paramaters */
    struct channel_list_s channel_list;                     /**< Channel List. */
} asynch_request_t;


/**
 * @brief LLC neighbour info request parameters
 */
typedef struct llc_neighbour_req {
    struct mac_neighbor_table_entry *neighbor;                  /**< Generic Link Layer Neighbor information entry. */
    struct ws_neighbor_class_entry *ws_neighbor;                /**< Wi-sun Neighbor information entry. */
} llc_neighbour_req_t;

typedef struct eapol_temporary_info_s {
    uint8_t eapol_rx_relay_filter; /*!< seconds for dropping duplicate id */
    uint8_t last_rx_mac_sequency; /*!< Only compared when Timer is active */
    uint16_t eapol_timeout; /*!< EAPOL relay Temporary entry lifetime */
} eapol_temporary_info_t;

/**
 * Neighbor temporary structure for storage FHSS data before create a real Neighbour info
 */
typedef struct ws_neighbor_temp_class_s {
    struct ws_neighbor_class_entry neigh_info_list;  /*!< Allocated hopping info array*/
    eapol_temporary_info_t eapol_temp_info;
    uint8_t mac64[8];
    uint8_t mpduLinkQuality;
    int8_t signal_dbm;
    ns_list_link_t link;
} ws_neighbor_temp_class_t;

typedef NS_LIST_HEAD(ws_neighbor_temp_class_t, link) ws_neighbor_temp_list_t;

/**
 * @brief ws_asynch_ind ws asynch data indication
 * @param interface Interface pointer
 * @param data MCPS-DATA.indication specific values
 * @param ie_ext Information element list
 */
typedef void ws_asynch_ind(struct protocol_interface_info_entry *interface, const struct mcps_data_ind_s *data, const struct mcps_data_ie_list *ie_ext, uint8_t message_type);

/**
 * @brief ws_asynch_confirm ws asynch data confirmation to asynch message request
 * @param api The API which handled the response
 * @param data MCPS-DATA.confirm specific values
 * @param user_id MPX user ID
 */
typedef void ws_asynch_confirm(struct protocol_interface_info_entry *interface, uint8_t asynch_message);

/**
 * @brief ws_asynch_confirm ws asynch data confirmation to asynch message request
 * @param interface The interface pointer
 * @param mac_64 Neighbor 64-bit address
 * @param neighbor_buffer Buffer where neighbor infor is buffered
 * @param request_new true if is possible to allocate new entry
 *
 * @return true when neighbor info is available
 * @return false when no neighbor info
 */
typedef bool ws_neighbor_info_request(struct protocol_interface_info_entry *interface, const uint8_t *mac_64, llc_neighbour_req_t *neighbor_buffer, bool request_new);

/**
 * @brief ws_llc_create ws LLC module create
 * @param interface Interface pointer
 * @param asynch_ind_cb Asynch indication
 * @param ie_ext Information element list
 *
 * Function allocate and init LLC class and init it 2 supported 2 API: ws asynch and MPX user are internally registered.
 */
int8_t ws_llc_create(struct protocol_interface_info_entry *interface, ws_asynch_ind *asynch_ind_cb, ws_asynch_confirm *asynch_cnf_cb, ws_neighbor_info_request *ws_neighbor_info_request_cb);

/**
 * @brief ws_llc_reset Reset ws LLC parametrs and clean messages
 * @param interface Interface pointer
 *
 */
void ws_llc_reset(struct protocol_interface_info_entry *interface);

/**
 * @brief ws_llc_delete Delete LLC interface. ONLY for Test purpose.
 * @param interface Interface pointer
 *
 */
int8_t ws_llc_delete(struct protocol_interface_info_entry *interface);

/**
 * @brief ws_llc_mpx_api_get Get MPX api for registration purpose.
 * @param interface Interface pointer
 *
 * @return NULL when MPX is not vailabale
 * @return Pointer to MPX API
 *
 */
mpx_api_t *ws_llc_mpx_api_get(struct protocol_interface_info_entry *interface);

/**
 * @brief ws_llc_asynch_request ws asynch message request to all giving channels
 * @param interface Interface pointer
 * @param request Asynch message parameters: type, IE and channel list
 *
 * @return 0 Asynch message pushed to MAC
 * @return -1 memory allocate problem
 * @return -2 Parameter problem
 *
 */
int8_t ws_llc_asynch_request(struct protocol_interface_info_entry *interface, asynch_request_t *request);


/**
 * @brief ws_llc_set_vendor_header_data Configure WS vendor Header data information (Data of WH_IE_VH_TYPE IE element)
 * @param interface Interface pointer
 * @param vendor_header pointer to vendor header this pointer must keep alive when it is configured to LLC
 * @param vendor_header_length configured vendor header length
 *
 */
void ws_llc_set_vendor_header_data(struct protocol_interface_info_entry *interface, uint8_t *vendor_header, uint8_t vendor_header_length);

/**
 * @brief ws_llc_set_vendor_payload_data Configure WS vendor payload data information (Data of WP_PAYLOAD_IE_VP_TYPE IE element)
 * @param interface Interface pointer
 * @param vendor_payload pointer to vendor payload this pointer must keep alive when it is configured to LLC
 * @param vendor_payload_length configured vendor payload length
 *
 */
void ws_llc_set_vendor_payload_data(struct protocol_interface_info_entry *interface, uint8_t *vendor_payload, uint8_t vendor_payload_length);

/**
 * @brief ws_llc_set_network_name Configure WS Network name (Data of WP_PAYLOAD_IE_NETNAME_TYPE IE element)
 * @param interface Interface pointer
 * @param name_length configured network name length
 * @param name pointer to network name this pointer must keep alive when it is configured to LLC
 *
 */
void ws_llc_set_network_name(struct protocol_interface_info_entry *interface, uint8_t *name, uint8_t name_length);

/**
 * @brief ws_llc_set_gtkhash Configure WS GTK hash information (Data of WP_PAYLOAD_IE_GTKHASH_TYPE IE element)
 * @param interface Interface pointer
 * @param gtkhash pointer to GTK hash which length is 32 bytes this pointer must keep alive when it is configured to LLC
 *
 */
void  ws_llc_set_gtkhash(struct protocol_interface_info_entry *interface, uint8_t *gtkhash);

/**
 * @brief ws_llc_set_pan_information_pointer Configure WS PAN information (Data of WP_PAYLOAD_IE_PAN_TYPE IE element)
 * @param interface Interface pointer
 * @param pan_information_pointer pointer to Pan information this pointer must keep alive when it is configured to LLC
 *
 */
void ws_llc_set_pan_information_pointer(struct protocol_interface_info_entry *interface, struct ws_pan_information_s *pan_information_pointer);

/**
 * @brief ws_llc_hopping_schedule_config Configure channel hopping
 * @param interface Interface pointer
 * @param hopping_schedule pointer to Channel hopping schedule
 *
 */
void ws_llc_hopping_schedule_config(struct protocol_interface_info_entry *interface, struct ws_hopping_schedule_s *hopping_schedule);

void ws_llc_timer_seconds(struct protocol_interface_info_entry *interface, uint16_t seconds_update);

void ws_llc_fast_timer(struct protocol_interface_info_entry *interface, uint16_t ticks);

bool ws_llc_eapol_relay_forward_filter(struct protocol_interface_info_entry *interface, const uint8_t *joiner_eui64, uint8_t mac_sequency, uint32_t rx_timestamp);

ws_neighbor_temp_class_t *ws_llc_get_multicast_temp_entry(struct protocol_interface_info_entry *interface, const uint8_t *mac64);

ws_neighbor_temp_class_t *ws_llc_get_eapol_temp_entry(struct protocol_interface_info_entry *interface, const uint8_t *mac64);



void ws_llc_free_multicast_temp_entry(struct protocol_interface_info_entry *interface, ws_neighbor_temp_class_t *neighbor);

#endif /* WS_LLC_H_ */
