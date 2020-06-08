/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include <string.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "mac_common_defines.h"
#include "mac_api.h"
#include "mac_mcps.h"
#include "common_functions.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mpx_api.h"
#include "6LoWPAN/MAC/mac_ie_lib.h"
#include "6LoWPAN/ws/ws_common_defines.h"
#include "6LoWPAN/ws/ws_common.h"
#include "6LoWPAN/ws/ws_bootstrap.h"
#include "6LoWPAN/ws/ws_ie_lib.h"
#include "6LoWPAN/ws/ws_llc.h"
#include "6LoWPAN/ws/ws_mpx_header.h"
#include "6LoWPAN/ws/ws_pae_controller.h"
#include "6LoWPAN/ws/ws_cfg_settings.h"
#include "Security/PANA/pana_eap_header.h"
#include "Security/eapol/eapol_helper.h"
#include "Service_Libs/etx/etx.h"
#include "fhss_ws_extension.h"

#ifdef HAVE_WS

#define TRACE_GROUP "wllc"

#define LLC_MESSAGE_QUEUE_LIST_SIZE_MAX   16 //Do not config over 30 never
#define MPX_USER_SIZE 2

typedef struct {
    uint16_t                user_id;        /**< User ID for identify MPX User */
    mpx_data_confirm        *data_confirm;  /**< User registred MPX Data confirmation call back */
    mpx_data_indication     *data_ind;      /**< User registred MPX Data indication call back */
} mpx_user_t;


typedef struct {
    mpx_api_t   mpx_api;                        /**< API for MPX user like Stack and EAPOL */
    mpx_user_t  mpx_user_table[MPX_USER_SIZE];  /**< MPX user list include registered call back pointers and user id's */
    unsigned    mpx_id: 4;                      /**< MPX class sequence number */
} mpx_class_t;


typedef struct {
    uint16_t                supported_channels;     /**< Configured Channel count. This will define Channel infor mask length to some information element */
    uint16_t                network_name_length;    /**< Network name length */
    uint16_t                vendor_payload_length;  /**< Vendor spesific payload length */
    uint8_t                 vendor_header_length;   /**< Vendor spesific header length */
    uint8_t                 gtkhash_length;         /**< GTK hash length */
    ws_pan_information_t    *pan_congiguration;     /**< Pan configururation */
    struct ws_hopping_schedule_s *hopping_schedule;/**< Channel hopping schedule */
    uint8_t                 *gtkhash;               /**< Pointer to GTK HASH user must give pointer which include 4 64-bit HASH array */
    uint8_t                 *network_name;          /**< Network name */
    uint8_t                 *vendor_header_data;    /**< Vendor spesific header data */
    uint8_t                 *vendor_payload;        /**< Vendor spesific payload data */
} llc_ie_params_t;

typedef struct {
    uint8_t dst_address[8];             /**< Destination address */
    uint16_t pan_id;                    /**< Destination Pan-Id */
    unsigned        messsage_type: 3;   /**< Frame type to UTT */
    unsigned        mpx_id: 5;          /**< MPX sequence */
    bool            ack_requested: 1;   /**< ACK requested */
    bool            eapol_temporary: 1; /**< EAPOL TX entry index used */
    unsigned        dst_address_type: 2; /**<  Destination address type */
    unsigned        src_address_type: 2; /**<  Source address type */
    uint8_t         msg_handle;         /**< LLC genetaed unique MAC handle */
    uint8_t         mpx_user_handle;    /**< This MPX user defined handle */
    ns_ie_iovec_t   ie_vector_list[3];  /**< IE vectors: 1 for Header's, 1 for Payload and for MPX payload */
    mcps_data_req_ie_list_t ie_ext;
    ns_list_link_t  link;               /**< List link entry */
    uint8_t         ie_buffer[];        /**< Trailing buffer data */
} llc_message_t;

/** get pointer to Mac header start point*/
#define ws_message_buffer_ptr_get(x)  (&(x)->ie_buffer[0])

typedef NS_LIST_HEAD(llc_message_t, link) llc_message_list_t;

#define MAX_NEIGH_TEMPORARY_MULTICAST_SIZE 5
#define MAX_NEIGH_TEMPORRY_EAPOL_SIZE 30
#define MAX_NEIGH_TEMPORAY_LIST_SIZE (MAX_NEIGH_TEMPORARY_MULTICAST_SIZE + MAX_NEIGH_TEMPORRY_EAPOL_SIZE)

typedef struct {
    ws_neighbor_temp_class_t        neighbour_temporary_table[MAX_NEIGH_TEMPORAY_LIST_SIZE];
    ws_neighbor_temp_list_t         active_multicast_temp_neigh;
    ws_neighbor_temp_list_t         active_eapol_temp_neigh;
    ws_neighbor_temp_list_t         free_temp_neigh;
    llc_message_list_t              llc_eap_pending_list;           /**< Active Message list */
    bool                            active_eapol_session: 1;        /**< Indicating active EAPOL message */
} temp_entriest_t;

typedef struct {
    uint8_t                         mac_handle_base;                /**< Mac handle id base this will be updated by 1 after use */
    uint8_t                         llc_message_list_size;          /**< llc_message_list list size */
    mpx_class_t                     mpx_data_base;                  /**< MPX data be including USER API Class and user call backs */
    llc_message_list_t              llc_message_list;               /**< Active Message list */
    llc_ie_params_t                 ie_params;                      /**< LLC IE header and Payload data configuration */
    temp_entriest_t                 *temp_entries;

    ws_asynch_ind                   *asynch_ind;                    /**< LLC Asynch data indication call back configured by user */
    ws_asynch_confirm               *asynch_confirm;                /**< LLC Asynch data confirmation call back configured by user */
    ws_neighbor_info_request        *ws_neighbor_info_request_cb;   /**< LLC Neighbour discover API*/
    uint8_t                         ws_enhanced_ack_elements[WH_IE_ELEMENT_HEADER_LENGTH + 4 + WH_IE_ELEMENT_HEADER_LENGTH + 1];
    ns_ie_iovec_t                   ws_header_vector;
    protocol_interface_info_entry_t *interface_ptr;                 /**< List link entry */

    ns_list_link_t                  link;                           /**< List link entry */
} llc_data_base_t;

static NS_LIST_DEFINE(llc_data_base_list, llc_data_base_t, link);

static uint16_t ws_wp_nested_message_length(wp_nested_ie_sub_list_t requested_list, llc_ie_params_t *params);
static uint16_t ws_wh_headers_length(wh_ie_sub_list_t requested_list, llc_ie_params_t *params);

/** LLC message local functions */
static llc_message_t *llc_message_discover_by_mac_handle(uint8_t handle, llc_message_list_t *list);
static llc_message_t *llc_message_discover_by_mpx_id(uint8_t handle, llc_message_list_t *list);
static llc_message_t *llc_message_discover_mpx_user_id(uint8_t handle, uint16_t user_id, llc_message_list_t *list);
static void llc_message_free(llc_message_t *message, llc_data_base_t *llc_base);
static void llc_message_id_allocate(llc_message_t *message, llc_data_base_t *llc_base, bool mpx_user);
static llc_message_t *llc_message_allocate(uint16_t ie_buffer_size, llc_data_base_t *llc_base);

/** LLC interface sepesific local functions */
static llc_data_base_t *ws_llc_discover_by_interface(struct protocol_interface_info_entry *interface);
static llc_data_base_t *ws_llc_discover_by_mac(const mac_api_t *api);
static llc_data_base_t *ws_llc_discover_by_mpx(const mpx_api_t *api);

static mpx_user_t *ws_llc_mpx_user_discover(mpx_class_t *mpx_class, uint16_t user_id);
static llc_data_base_t *ws_llc_base_allocate(void);
static void ws_llc_mac_confirm_cb(const mac_api_t *api, const mcps_data_conf_t *data, const mcps_data_conf_payload_t *conf_data);
static void ws_llc_mac_indication_cb(const mac_api_t *api, const mcps_data_ind_t *data, const mcps_data_ie_list_t *ie_ext);
static uint16_t ws_mpx_header_size_get(llc_data_base_t *base, uint16_t user_id);
static void ws_llc_mpx_data_request(const mpx_api_t *api, const struct mcps_data_req_s *data, uint16_t user_id);
static int8_t ws_llc_mpx_data_cb_register(const mpx_api_t *api, mpx_data_confirm *confirm_cb, mpx_data_indication *indication_cb, uint16_t user_id);
static uint16_t ws_llc_mpx_header_size_get(const mpx_api_t *api, uint16_t user_id);
static uint8_t ws_llc_mpx_data_purge_request(const mpx_api_t *api, struct mcps_purge_s *purge, uint16_t user_id);
static void ws_llc_mpx_init(mpx_class_t *mpx_class);

static void ws_llc_temp_neigh_info_table_reset(temp_entriest_t *base);
static ws_neighbor_temp_class_t *ws_allocate_multicast_temp_entry(temp_entriest_t *base, const uint8_t *mac64);
static ws_neighbor_temp_class_t *ws_llc_discover_eapol_temp_entry(temp_entriest_t *base, const uint8_t *mac64);
static void ws_llc_release_eapol_temp_entry(temp_entriest_t *base, const uint8_t *mac64);
static ws_neighbor_temp_class_t *ws_allocate_eapol_temp_entry(temp_entriest_t *base, const uint8_t *mac64);

static void ws_llc_mpx_eapol_send(llc_data_base_t *base, llc_message_t *message);

/** Discover Message by message handle id */
static llc_message_t *llc_message_discover_by_mac_handle(uint8_t handle, llc_message_list_t *list)
{
    ns_list_foreach(llc_message_t, message, list) {
        if (message->msg_handle == handle) {
            return message;
        }
    }
    return NULL;
}

static llc_message_t *llc_message_discover_by_mpx_id(uint8_t handle, llc_message_list_t *list)
{
    ns_list_foreach(llc_message_t, message, list) {
        if ((message->messsage_type == WS_FT_DATA || message->messsage_type == WS_FT_EAPOL) && message->mpx_id == handle) {
            return message;
        }
    }
    return NULL;
}


static llc_message_t *llc_message_discover_mpx_user_id(uint8_t handle, uint16_t user_id, llc_message_list_t *list)
{
    uint8_t message_type;
    if (user_id == MPX_LOWPAN_ENC_USER_ID) {
        message_type = WS_FT_DATA;
    } else {
        message_type = WS_FT_EAPOL;
    }

    ns_list_foreach(llc_message_t, message, list) {
        if (message->messsage_type == message_type && message->mpx_user_handle == handle) {
            return message;
        }
    }
    return NULL;
}




//Free message and delete from list
static void llc_message_free(llc_message_t *message, llc_data_base_t *llc_base)
{
    ns_list_remove(&llc_base->llc_message_list, message);
    ns_dyn_mem_free(message);
    llc_base->llc_message_list_size--;
}

static void llc_message_id_allocate(llc_message_t *message, llc_data_base_t *llc_base, bool mpx_user)
{
    //Guarantee
    while (1) {
        if (llc_message_discover_by_mac_handle(llc_base->mac_handle_base, &llc_base->llc_message_list)) {
            llc_base->mac_handle_base++;
        } else {
            break;
        }
    }
    if (mpx_user) {
        while (1) {
            if (llc_message_discover_by_mpx_id(llc_base->mpx_data_base.mpx_id, &llc_base->llc_message_list)) {
                llc_base->mpx_data_base.mpx_id++;
            } else {
                break;
            }
        }
    }

    //Storage handle and update base
    message->msg_handle = llc_base->mac_handle_base++;
    if (mpx_user) {
        message->mpx_id = llc_base->mpx_data_base.mpx_id++;
    }
}

static llc_message_t *llc_message_allocate(uint16_t ie_buffer_size, llc_data_base_t *llc_base)
{
    if (llc_base->llc_message_list_size >= LLC_MESSAGE_QUEUE_LIST_SIZE_MAX) {
        return NULL;
    }

    llc_message_t *message = ns_dyn_mem_temporary_alloc(sizeof(llc_message_t) + ie_buffer_size);
    if (!message) {
        return NULL;
    }
    message->ack_requested = false;
    message->eapol_temporary = false;
    return message;
}

static llc_data_base_t *ws_llc_discover_by_interface(struct protocol_interface_info_entry *interface)
{
    ns_list_foreach(llc_data_base_t, base, &llc_data_base_list) {
        if (base->interface_ptr == interface) {
            return base;
        }
    }
    return NULL;
}

static llc_data_base_t *ws_llc_discover_by_mac(const mac_api_t *api)
{
    ns_list_foreach(llc_data_base_t, base, &llc_data_base_list) {
        if (base->interface_ptr->mac_api == api) {
            return base;
        }
    }
    return NULL;
}

static llc_data_base_t *ws_llc_discover_by_mpx(const mpx_api_t *api)
{
    ns_list_foreach(llc_data_base_t, base, &llc_data_base_list) {
        if (&base->mpx_data_base.mpx_api == api) {
            return base;
        }
    }
    return NULL;
}

static uint16_t ws_wh_headers_length(wh_ie_sub_list_t requested_list, llc_ie_params_t *params)
{
    uint16_t length = 0;
    if (requested_list.utt_ie) {
        //Static 4 bytes allways UTT
        length += WH_IE_ELEMENT_HEADER_LENGTH + 4;
    }

    if (requested_list.bt_ie) {
        //Static 5 bytes allways
        length += WH_IE_ELEMENT_HEADER_LENGTH + 5;
    }

    if (requested_list.fc_ie) {
        //Static 1 bytes allways
        length += WH_IE_ELEMENT_HEADER_LENGTH + 1;
    }

    if (requested_list.rsl_ie) {
        //Static 1 bytes allways
        length += WH_IE_ELEMENT_HEADER_LENGTH + 1;
    }

    if (requested_list.vh_ie) {
        //Dynamic length
        length += WH_IE_ELEMENT_HEADER_LENGTH + params->vendor_header_length;
    }

    if (requested_list.ea_ie) {
        length += WH_IE_ELEMENT_HEADER_LENGTH + 8;
    }

    return length;
}

static uint16_t ws_wp_nested_message_length(wp_nested_ie_sub_list_t requested_list, llc_ie_params_t *params)
{
    uint16_t length = 0;
    if (requested_list.gtkhash_ie) {
        //Static 32 bytes allways
        length += WS_WP_SUB_IE_ELEMENT_HEADER_LENGTH + params->gtkhash_length;
    }

    if (requested_list.net_name_ie) {
        //Dynamic length
        length += WS_WP_SUB_IE_ELEMENT_HEADER_LENGTH + params->network_name_length;
    }

    if (requested_list.vp_ie && params->vendor_payload_length) {
        //Dynamic length
        length += WS_WP_SUB_IE_ELEMENT_HEADER_LENGTH + params->vendor_payload_length;
    }

    if (requested_list.pan_ie) {
        //Static 5 bytes allways
        length += WS_WP_SUB_IE_ELEMENT_HEADER_LENGTH;
        if (params->pan_congiguration) {
            length += 5;
        }
    }

    if (requested_list.pan_version_ie) {
        //Static 2 bytes allways
        length += WS_WP_SUB_IE_ELEMENT_HEADER_LENGTH;
        if (params->pan_congiguration) {
            length += 2;
        }
    }

    if (requested_list.bs_ie) {
        ///Dynamic length
        length += WS_WP_SUB_IE_ELEMENT_HEADER_LENGTH + ws_wp_nested_hopping_schedule_length(params->hopping_schedule, false);
    }

    if (requested_list.us_ie) {
        //Dynamic length
        length += WS_WP_SUB_IE_ELEMENT_HEADER_LENGTH + ws_wp_nested_hopping_schedule_length(params->hopping_schedule, true);
    }

    return length;
}

static mpx_user_t *ws_llc_mpx_user_discover(mpx_class_t *mpx_class, uint16_t user_id)
{
    for (int i = 0; i < MPX_USER_SIZE; i++) {
        if (mpx_class->mpx_user_table[i].user_id == user_id) {
            return &mpx_class->mpx_user_table[i];
        }
    }
    return NULL;
}

static llc_data_base_t *ws_llc_base_allocate(void)
{
    llc_data_base_t *base = ns_dyn_mem_alloc(sizeof(llc_data_base_t));
    temp_entriest_t *temp_entries = ns_dyn_mem_alloc(sizeof(temp_entriest_t));
    if (!base || !temp_entries) {
        ns_dyn_mem_free(base);
        ns_dyn_mem_free(temp_entries);
        return NULL;
    }
    memset(base, 0, sizeof(llc_data_base_t));
    memset(temp_entries, 0, sizeof(temp_entriest_t));
    ns_list_init(&temp_entries->active_multicast_temp_neigh);
    ns_list_init(&temp_entries->active_eapol_temp_neigh);
    ns_list_init(&temp_entries->free_temp_neigh);
    ns_list_init(&temp_entries->llc_eap_pending_list);
    base->temp_entries = temp_entries;

    ns_list_init(&base->llc_message_list);

    ns_list_add_to_end(&llc_data_base_list, base);
    return base;
}

/** WS LLC MAC data extension confirmation  */
static void ws_llc_mac_confirm_cb(const mac_api_t *api, const mcps_data_conf_t *data, const mcps_data_conf_payload_t *conf_data)
{
    (void) conf_data;
    llc_data_base_t *base = ws_llc_discover_by_mac(api);
    if (!base) {
        return;
    }

    protocol_interface_info_entry_t *interface = base->interface_ptr;

    llc_message_t *message = llc_message_discover_by_mac_handle(data->msduHandle, &base->llc_message_list);
    if (!message) {
        return;
    }

    uint8_t messsage_type = message->messsage_type;
    uint8_t mpx_user_handle = message->mpx_user_handle;
    if (message->eapol_temporary) {
        //Clear
        ws_bootstrap_eapol_tx_temporary_clear(interface);

        if (data->status == MLME_SUCCESS || data->status == MLME_NO_DATA) {
            //Update timeout
            ws_neighbor_temp_class_t *temp_entry = ws_llc_discover_eapol_temp_entry(base->temp_entries, message->dst_address);
            if (temp_entry) {
                //Update Temporary Lifetime
                temp_entry->eapol_temp_info.eapol_timeout = interface->ws_info->cfg->timing.temp_eapol_min_timeout + 1;
            }
        }
    }
    //ETX update
    if (message->ack_requested && messsage_type == WS_FT_DATA) {
        llc_neighbour_req_t neighbor_info;
        bool success = false;

        switch (data->status) {
            case MLME_SUCCESS:
            case MLME_TX_NO_ACK:
            case MLME_NO_DATA:
                if (data->status == MLME_SUCCESS || data->status == MLME_NO_DATA) {
                    success = true;
                }

                if (message->dst_address_type == MAC_ADDR_MODE_64_BIT && base->ws_neighbor_info_request_cb(interface, message->dst_address, &neighbor_info, false)) {
                    etx_transm_attempts_update(interface->id, 1 + data->tx_retries, success, neighbor_info.neighbor->index, neighbor_info.neighbor->mac64);
                    //TODO discover RSL from Enchanced ACK Header IE elements
                    ws_utt_ie_t ws_utt;
                    if (ws_wh_utt_read(conf_data->headerIeList, conf_data->headerIeListLength, &ws_utt)) {
                        //UTT header
                        if (success) {
                            neighbor_info.neighbor->lifetime = neighbor_info.neighbor->link_lifetime;
                        }

                        ws_neighbor_class_neighbor_unicast_time_info_update(neighbor_info.ws_neighbor, &ws_utt, data->timestamp);
                    }

                    int8_t rsl;
                    if (ws_wh_rsl_read(conf_data->headerIeList, conf_data->headerIeListLength, &rsl)) {
                        ws_neighbor_class_rsl_out_calculate(neighbor_info.ws_neighbor, rsl);
                    }
                }

                break;
            default:
                break;
        }
    }
    //Free message
    llc_message_free(message, base);

    if (messsage_type == WS_FT_DATA || messsage_type == WS_FT_EAPOL) {
        mpx_user_t *user_cb;
        uint16_t mpx_user_id;
        if (messsage_type == WS_FT_DATA) {
            mpx_user_id = MPX_LOWPAN_ENC_USER_ID;
        } else {
            mpx_user_id = MPX_KEY_MANAGEMENT_ENC_USER_ID;
            base->temp_entries->active_eapol_session = false;
        }

        user_cb = ws_llc_mpx_user_discover(&base->mpx_data_base, mpx_user_id);
        if (user_cb && user_cb->data_confirm) {
            //Call MPX registered call back
            mcps_data_conf_t data_conf = *data;
            data_conf.msduHandle = mpx_user_handle;
            user_cb->data_confirm(&base->mpx_data_base.mpx_api, &data_conf);
        }

        if (messsage_type == WS_FT_EAPOL) {
            message = ns_list_get_first(&base->temp_entries->llc_eap_pending_list);
            if (message) {
                //Start A pending EAPOL
                ns_list_remove(&base->temp_entries->llc_eap_pending_list, message);
                ws_llc_mpx_eapol_send(base, message);
            }
        }

        return;
    }
    //Async message Confirmation
    base->asynch_confirm(base->interface_ptr, messsage_type);

}

static void ws_llc_ack_data_req_ext(const mac_api_t *api, mcps_ack_data_payload_t *data, int8_t rssi, uint8_t lqi)
{
    (void) lqi;
    llc_data_base_t *base = ws_llc_discover_by_mac(api);
    if (!base) {
        return;
    }
    /* Init all by zero */
    memset(data, 0, sizeof(mcps_ack_data_payload_t));
    //Add just 2 header elements to inside 1 block
    data->ie_elements.headerIeVectorList = &base->ws_header_vector;
    base->ws_header_vector.ieBase = base->ws_enhanced_ack_elements;
    base->ws_header_vector.iovLen = sizeof(base->ws_enhanced_ack_elements);
    data->ie_elements.headerIovLength = 1;

    //Write Data to block
    uint8_t *ptr = base->ws_enhanced_ack_elements;
    ptr = ws_wh_utt_write(ptr, WS_FT_ACK);
    ws_wh_rsl_write(ptr, ws_neighbor_class_rsl_from_dbm_calculate(rssi));
}


static llc_data_base_t *ws_llc_mpx_frame_common_validates(const mac_api_t *api, const mcps_data_ind_t *data, ws_utt_ie_t ws_utt)
{
    llc_data_base_t *base = ws_llc_discover_by_mac(api);
    if (!base) {
        return NULL;
    }

    if (!base->ie_params.gtkhash && ws_utt.message_type == WS_FT_DATA) {
        return NULL;
    }

    if (data->SrcAddrMode != ADDR_802_15_4_LONG) {
        return NULL;
    }

    protocol_interface_info_entry_t *interface = base->interface_ptr;

    if (interface->mac_parameters->pan_id != 0xffff && data->SrcPANId != interface->mac_parameters->pan_id) {
        //Drop wrong PAN-id messages in this phase.
        return NULL;
    }

    return base;

}

static mpx_user_t *ws_llc_mpx_header_parse(llc_data_base_t *base, const mcps_data_ie_list_t *ie_ext, mpx_msg_t *mpx_frame, mac_payload_IE_t *mpx_ie)
{

    mpx_ie->id = MAC_PAYLOAD_MPX_IE_GROUP_ID;
    if (mac_ie_payload_discover(ie_ext->payloadIeList, ie_ext->payloadIeListLength, mpx_ie) < 1) {
        // NO MPX
        return NULL;
    }
    //Validate MPX header
    if (!ws_llc_mpx_header_frame_parse(mpx_ie->content_ptr, mpx_ie->length, mpx_frame)) {
        return NULL;
    }

    if (mpx_frame->transfer_type != MPX_FT_FULL_FRAME) {
        return NULL; //Support only FULL Frame's
    }

    // Discover MPX handler
    mpx_user_t *user_cb = ws_llc_mpx_user_discover(&base->mpx_data_base, mpx_frame->multiplex_id);
    if (!user_cb || !user_cb->data_ind) {
        return NULL;
    }

    return user_cb;
}


static void ws_llc_data_indication_cb(const mac_api_t *api, const mcps_data_ind_t *data, const mcps_data_ie_list_t *ie_ext, ws_utt_ie_t ws_utt)
{
    llc_data_base_t *base = ws_llc_mpx_frame_common_validates(api, data, ws_utt);
    if (!base) {
        return;
    }

    //Discover MPX header and handler
    mac_payload_IE_t mpx_ie;
    mpx_msg_t mpx_frame;
    mpx_user_t *user_cb = ws_llc_mpx_header_parse(base, ie_ext, &mpx_frame, &mpx_ie);
    if (!user_cb) {
        return;
    }

    mac_payload_IE_t ws_wp_nested;
    ws_us_ie_t us_ie;
    bool us_ie_inline = false;
    bool bs_ie_inline = false;
    ws_wp_nested.id = WS_WP_NESTED_IE;
    ws_bs_ie_t ws_bs_ie;
    if (mac_ie_payload_discover(ie_ext->payloadIeList, ie_ext->payloadIeListLength, &ws_wp_nested) > 2) {
        us_ie_inline = ws_wp_nested_us_read(ws_wp_nested.content_ptr, ws_wp_nested.length, &us_ie);
        bs_ie_inline = ws_wp_nested_bs_read(ws_wp_nested.content_ptr, ws_wp_nested.length, &ws_bs_ie);
    }

    protocol_interface_info_entry_t *interface = base->interface_ptr;

    //Validate Unicast shedule Channel Plan
    if (us_ie_inline && !ws_bootstrap_validate_channel_plan(&us_ie, interface)) {
        //Channel plan configuration mismatch
        return;
    }

    //Free Old temporary entry
    if (data->Key.SecurityLevel) {
        ws_llc_release_eapol_temp_entry(base->temp_entries, data->SrcAddr);
    }

    llc_neighbour_req_t neighbor_info;
    bool multicast;
    bool request_new_entry;
    if (data->DstAddrMode == ADDR_802_15_4_LONG) {
        multicast = false;
        request_new_entry = us_ie_inline;
    } else {
        multicast = true;
        request_new_entry = false;
    }

    if (!base->ws_neighbor_info_request_cb(interface, data->SrcAddr, &neighbor_info, request_new_entry)) {
        if (!multicast) {
            //tr_debug("Drop message no neighbor");
            return;
        } else {
            //Allocate temporary entry
            ws_neighbor_temp_class_t *temp_entry = ws_allocate_multicast_temp_entry(base->temp_entries, data->SrcAddr);
            neighbor_info.ws_neighbor = &temp_entry->neigh_info_list;
            //Storage Signal info for future ETX update possibility
            temp_entry->mpduLinkQuality = data->mpduLinkQuality;
            temp_entry->signal_dbm = data->signal_dbm;
        }
    }

    if (!multicast && !ws_neighbor_class_neighbor_duplicate_packet_check(neighbor_info.ws_neighbor, data->DSN, data->timestamp)) {
        tr_info("Drop duplicate message");
        return;
    }

    ws_neighbor_class_neighbor_unicast_time_info_update(neighbor_info.ws_neighbor, &ws_utt, data->timestamp);
    if (us_ie_inline) {
        ws_neighbor_class_neighbor_unicast_schedule_set(neighbor_info.ws_neighbor, &us_ie);
    }
    //Update BS if it is part of message
    if (bs_ie_inline) {
        ws_neighbor_class_neighbor_broadcast_schedule_set(neighbor_info.ws_neighbor, &ws_bs_ie);
    }

    //Update BT if it is part of message
    ws_bt_ie_t ws_bt;
    if (ws_wh_bt_read(ie_ext->headerIeList, ie_ext->headerIeListLength, &ws_bt)) {
        ws_neighbor_class_neighbor_broadcast_time_info_update(neighbor_info.ws_neighbor, &ws_bt, data->timestamp);
        if (neighbor_info.neighbor && neighbor_info.neighbor->link_role == PRIORITY_PARENT_NEIGHBOUR) {
            ns_fhss_ws_set_parent(interface->ws_info->fhss_api, neighbor_info.neighbor->mac64, &neighbor_info.ws_neighbor->fhss_data.bc_timing_info, false);
        }
    }

    if (data->DstAddrMode == ADDR_802_15_4_LONG) {
        neighbor_info.ws_neighbor->unicast_data_rx = true;
    }

    // Calculate RSL for all UDATA packages heard
    ws_neighbor_class_rsl_in_calculate(neighbor_info.ws_neighbor, data->signal_dbm);

    if (neighbor_info.neighbor) {
        //Refresh ETX dbm
        etx_lqi_dbm_update(interface->id, data->mpduLinkQuality, data->signal_dbm, neighbor_info.neighbor->index, neighbor_info.neighbor->mac64);
        if (data->Key.SecurityLevel) {
            //SET trusted state
            mac_neighbor_table_trusted_neighbor(mac_neighbor_info(interface), neighbor_info.neighbor, true);
        }
    }

    mcps_data_ind_t data_ind = *data;
    if (!neighbor_info.neighbor) {
        data_ind.Key.SecurityLevel = 0; //Mark unknow device
    }
    data_ind.msdu_ptr = mpx_frame.frame_ptr;
    data_ind.msduLength = mpx_frame.frame_length;
    user_cb->data_ind(&base->mpx_data_base.mpx_api, &data_ind);

}

static void ws_llc_eapol_indication_cb(const mac_api_t *api, const mcps_data_ind_t *data, const mcps_data_ie_list_t *ie_ext, ws_utt_ie_t ws_utt)
{
    llc_data_base_t *base = ws_llc_mpx_frame_common_validates(api, data, ws_utt);
    if (!base) {
        return;
    }

    if (data->DstAddrMode != ADDR_802_15_4_LONG) {
        return;
    }

    //Discover MPX header and handler
    mac_payload_IE_t mpx_ie;
    mpx_msg_t mpx_frame;
    mpx_user_t *user_cb = ws_llc_mpx_header_parse(base, ie_ext, &mpx_frame, &mpx_ie);
    if (!user_cb) {
        return;
    }

    mac_payload_IE_t ws_wp_nested;
    ws_us_ie_t us_ie;
    bool us_ie_inline = false;
    bool bs_ie_inline = false;
    ws_wp_nested.id = WS_WP_NESTED_IE;
    ws_bs_ie_t ws_bs_ie;
    if (mac_ie_payload_discover(ie_ext->payloadIeList, ie_ext->payloadIeListLength, &ws_wp_nested) > 2) {
        us_ie_inline = ws_wp_nested_us_read(ws_wp_nested.content_ptr, ws_wp_nested.length, &us_ie);
        bs_ie_inline = ws_wp_nested_bs_read(ws_wp_nested.content_ptr, ws_wp_nested.length, &ws_bs_ie);
    }

    protocol_interface_info_entry_t *interface = base->interface_ptr;

    //Validate Unicast shedule Channel Plan
    if (us_ie_inline && !ws_bootstrap_validate_channel_plan(&us_ie, interface)) {
        //Channel plan configuration mismatch
        return;
    }

    llc_neighbour_req_t neighbor_info;

    if (!base->ws_neighbor_info_request_cb(interface, data->SrcAddr, &neighbor_info, false)) {
        //Allocate temporary entry
        ws_neighbor_temp_class_t *temp_entry = ws_allocate_eapol_temp_entry(base->temp_entries, data->SrcAddr);
        if (!temp_entry) {
            tr_warn("EAPOL temp pool empty");
            return;
        }
        //Update Temporary Lifetime
        temp_entry->eapol_temp_info.eapol_timeout = interface->ws_info->cfg->timing.temp_eapol_min_timeout + 1;

        neighbor_info.ws_neighbor = &temp_entry->neigh_info_list;
        //Storage Signal info for future ETX update possibility
        temp_entry->mpduLinkQuality = data->mpduLinkQuality;
        temp_entry->signal_dbm = data->signal_dbm;
    }
    uint8_t auth_eui64[8];
    ws_neighbor_class_neighbor_unicast_time_info_update(neighbor_info.ws_neighbor, &ws_utt, data->timestamp);
    if (us_ie_inline) {
        ws_neighbor_class_neighbor_unicast_schedule_set(neighbor_info.ws_neighbor, &us_ie);
    }
    //Update BS if it is part of message
    if (bs_ie_inline) {
        ws_neighbor_class_neighbor_broadcast_schedule_set(neighbor_info.ws_neighbor, &ws_bs_ie);
    }

    //Discover and write Auhtenticator EUI-64
    if (ws_wh_ea_read(ie_ext->headerIeList, ie_ext->headerIeListLength, auth_eui64)) {
        ws_pae_controller_border_router_addr_write(base->interface_ptr, auth_eui64);
    }

    //Update BT if it is part of message
    ws_bt_ie_t ws_bt;
    if (ws_wh_bt_read(ie_ext->headerIeList, ie_ext->headerIeListLength, &ws_bt)) {
        ws_neighbor_class_neighbor_broadcast_time_info_update(neighbor_info.ws_neighbor, &ws_bt, data->timestamp);
        if (neighbor_info.neighbor) {
            ws_bootstrap_eapol_parent_synch(interface, &neighbor_info);
        }
    }


    mcps_data_ind_t data_ind = *data;
    data_ind.msdu_ptr = mpx_frame.frame_ptr;
    data_ind.msduLength = mpx_frame.frame_length;
    user_cb->data_ind(&base->mpx_data_base.mpx_api, &data_ind);
}

static void ws_llc_asynch_indication(const mac_api_t *api, const mcps_data_ind_t *data, const mcps_data_ie_list_t *ie_ext, ws_utt_ie_t ws_utt)
{
    llc_data_base_t *base = ws_llc_discover_by_mac(api);
    if (!base || !base->asynch_ind) {
        return;
    }

    //Asynch Message

    mac_payload_IE_t ws_wp_nested;

    ws_wp_nested.id = WS_WP_NESTED_IE;
    if (mac_ie_payload_discover(ie_ext->payloadIeList, ie_ext->payloadIeListLength, &ws_wp_nested) < 2) {
        // NO WS_WP_NESTED_IE Payload
        return;
    }

    switch (ws_utt.message_type) {
        case WS_FT_PAN_ADVERT:
        case WS_FT_PAN_CONF:
        case WS_FT_PAN_CONF_SOL:
            ws_llc_release_eapol_temp_entry(base->temp_entries, data->SrcAddr);
            break;
        default:
            break;
    }

    mcps_data_ie_list_t asynch_ie_list;
    asynch_ie_list.headerIeList = ie_ext->headerIeList,
    asynch_ie_list.headerIeListLength = ie_ext->headerIeListLength;
    asynch_ie_list.payloadIeList = ws_wp_nested.content_ptr;
    asynch_ie_list.payloadIeListLength = ws_wp_nested.length;
    base->asynch_ind(base->interface_ptr, data, &asynch_ie_list, ws_utt.message_type);
}

/** WS LLC MAC data extension indication  */
static void ws_llc_mac_indication_cb(const mac_api_t *api, const mcps_data_ind_t *data, const mcps_data_ie_list_t *ie_ext)
{


    //Discover Header WH_IE_UTT_TYPE
    ws_utt_ie_t ws_utt;
    if (!ws_wh_utt_read(ie_ext->headerIeList, ie_ext->headerIeListLength, &ws_utt)) {
        // NO UTT header
        return;
    }

    if (ws_utt.message_type < WS_FT_DATA) {
        ws_llc_asynch_indication(api, data, ie_ext, ws_utt);
        return;
    }

    if (ws_utt.message_type == WS_FT_DATA) {
        ws_llc_data_indication_cb(api, data, ie_ext, ws_utt);
        return;
    }

    if (ws_utt.message_type == WS_FT_EAPOL) {
        ws_llc_eapol_indication_cb(api, data, ie_ext, ws_utt);
        return;
    }
}

static uint16_t ws_mpx_header_size_get(llc_data_base_t *base, uint16_t user_id)
{
    //TODO add WS_WP_NESTED_IE support
    uint16_t header_size = 0;
    if (user_id == MPX_LOWPAN_ENC_USER_ID) {
        header_size += 7 + 8 + 5 + 2; //UTT+BTT+ MPX + Padding
        if (base->ie_params.vendor_header_length) {
            header_size += base->ie_params.vendor_header_length + 3;
        }

        if (base->ie_params.vendor_payload_length) {
            header_size += base->ie_params.vendor_payload_length + 2;
        }

        //Dynamic length
        header_size += 2 + WS_WP_SUB_IE_ELEMENT_HEADER_LENGTH + ws_wp_nested_hopping_schedule_length(base->ie_params.hopping_schedule, true) + ws_wp_nested_hopping_schedule_length(base->ie_params.hopping_schedule, false);

    } else if (MPX_KEY_MANAGEMENT_ENC_USER_ID) {
        header_size += 7 + 5 + 2;
        //Dynamic length
        header_size += 2 + WS_WP_SUB_IE_ELEMENT_HEADER_LENGTH + ws_wp_nested_hopping_schedule_length(base->ie_params.hopping_schedule, true);
    }
    return header_size;
}

static bool ws_eapol_handshake_first_msg(uint8_t *pdu, uint16_t length, protocol_interface_info_entry_t *cur)
{
    if (!ws_eapol_relay_state_active(cur)) {
        return false;
    }

    eapol_pdu_t eapol_pdu;
    uint8_t kmp_type = *pdu++;
    length--;
    if (!eapol_parse_pdu_header(pdu, length, &eapol_pdu)) {
        return false;
    }
    if (eapol_pdu.packet_type == EAPOL_EAP_TYPE) {
        if (eapol_pdu.msg.eap.eap_code == EAP_REQ && eapol_pdu.msg.eap.type == EAP_IDENTITY) {
            return true;
        }
    } else {

        uint8_t key_mask = eapol_pdu_key_mask_get(&eapol_pdu);
        if (kmp_type == 6 && key_mask == KEY_INFO_KEY_ACK) {
            //FWK first message validation
            return true;
        } else if (kmp_type == 7 && key_mask == (KEY_INFO_KEY_ACK | KEY_INFO_KEY_MIC | KEY_INFO_SECURED_KEY_FRAME)) {
            //GWK first message validation
            return true;
        }
    }

    return false;
}

static void ws_llc_lowpan_mpx_header_set(llc_message_t *message, uint16_t user_id)
{
    uint8_t *ptr = (uint8_t *)message->ie_vector_list[1].ieBase;
    ptr += message->ie_vector_list[1].iovLen;
    ptr  = mac_ie_payload_base_write(ptr, MAC_PAYLOAD_MPX_IE_GROUP_ID, message->ie_vector_list[2].iovLen + 3);
    mpx_msg_t mpx_header;
    mpx_header.transfer_type = MPX_FT_FULL_FRAME;
    mpx_header.transaction_id = message->mpx_id;
    mpx_header.multiplex_id = user_id;
    ptr = ws_llc_mpx_header_write(ptr, &mpx_header);
    message->ie_vector_list[1].iovLen = ptr - (uint8_t *)message->ie_vector_list[1].ieBase;
}

static void ws_llc_lowpan_mpx_data_request(llc_data_base_t *base, mpx_user_t *user_cb, const struct mcps_data_req_s *data)
{
    wh_ie_sub_list_t ie_header_mask;
    memset(&ie_header_mask, 0, sizeof(wh_ie_sub_list_t));

    wp_nested_ie_sub_list_t nested_wp_id;
    memset(&nested_wp_id, 0, sizeof(wp_nested_ie_sub_list_t));
    ie_header_mask.utt_ie = true;

    ie_header_mask.bt_ie = true;
    if (base->ie_params.vendor_header_length) {
        ie_header_mask.vh_ie = true;
    }

    if (base->ie_params.vendor_payload_length) {
        nested_wp_id.vp_ie = true;
    }

    if (!data->TxAckReq) {
        nested_wp_id.bs_ie = true;
    }

    nested_wp_id.us_ie = true;

    uint16_t ie_header_length = ws_wh_headers_length(ie_header_mask, &base->ie_params);
    uint16_t nested_ie_length = ws_wp_nested_message_length(nested_wp_id, &base->ie_params);

    uint16_t over_head_size = ie_header_length;
    if (nested_ie_length) {
        over_head_size += nested_ie_length + 2;
    }
    //Mpx header size
    over_head_size += 5; //MPX FuLL frame 3 bytes + IE header 2 bytes

    //Allocate Message
    llc_message_t *message = llc_message_allocate(over_head_size, base);
    if (!message) {
        mcps_data_conf_t data_conf;
        memset(&data_conf, 0, sizeof(mcps_data_conf_t));
        data_conf.msduHandle = data->msduHandle;
        data_conf.status = MLME_TRANSACTION_OVERFLOW;
        user_cb->data_confirm(&base->mpx_data_base.mpx_api, &data_conf);
        return;
    }

    //Add To active list
    llc_message_id_allocate(message, base, true);
    base->llc_message_list_size++;
    ns_list_add_to_end(&base->llc_message_list, message);

    mcps_data_req_t data_req;
    message->mpx_user_handle = data->msduHandle;
    message->ack_requested = data->TxAckReq;
    if (data->TxAckReq) {
        message->dst_address_type = data->DstAddrMode;
        memcpy(message->dst_address, data->DstAddr, 8);
    }
    data_req = *data;
    data_req.msdu = NULL;
    data_req.msduLength = 0;
    data_req.msduHandle = message->msg_handle;

    if (!data->TxAckReq) {
        data_req.PanIdSuppressed = false;
        data_req.DstAddrMode = MAC_ADDR_MODE_NONE;
    } else {
        data_req.PanIdSuppressed = true;
    }

    uint8_t *ptr = ws_message_buffer_ptr_get(message);
    message->messsage_type = WS_FT_DATA;

    message->ie_vector_list[0].ieBase = ptr;
    //Write UTT

    ptr = ws_wh_utt_write(ptr, message->messsage_type);
    if (ie_header_mask.bt_ie) {
        ptr = ws_wh_bt_write(ptr);
    }

    if (ie_header_mask.vh_ie) {
        ptr = ws_wh_vh_write(ptr, base->ie_params.vendor_header_data, base->ie_params.vendor_header_length);
    }

    message->ie_vector_list[0].iovLen = ie_header_length;
    message->ie_ext.headerIeVectorList = &message->ie_vector_list[0];
    message->ie_ext.headerIovLength = 1;
    message->ie_ext.payloadIeVectorList = &message->ie_vector_list[1];
    message->ie_ext.payloadIovLength = 2;
    message->ie_vector_list[1].ieBase = ptr;

    if (nested_ie_length) {
        ptr = ws_wp_base_write(ptr, nested_ie_length);
        //Write unicast schedule
        ptr = ws_wp_nested_hopping_schedule_write(ptr, base->ie_params.hopping_schedule, true);

        if (nested_wp_id.bs_ie) {
            //Write Broadcastcast schedule
            ptr = ws_wp_nested_hopping_schedule_write(ptr, base->ie_params.hopping_schedule, false);
        }
    }
    // SET Payload IE Length
    message->ie_vector_list[1].iovLen = ptr - (uint8_t *)message->ie_vector_list[1].ieBase;
    message->ie_vector_list[2].ieBase = data->msdu;
    message->ie_vector_list[2].iovLen =  data->msduLength;

    ws_llc_lowpan_mpx_header_set(message, MPX_LOWPAN_ENC_USER_ID);

    base->interface_ptr->mac_api->mcps_data_req_ext(base->interface_ptr->mac_api, &data_req, &message->ie_ext, NULL);
}

static bool ws_llc_eapol_temp_entry_set(llc_data_base_t *base, const uint8_t *mac64)
{
    //Discover Temporary entry
    ws_neighbor_temp_class_t *temp_neigh = ws_llc_discover_eapol_temp_entry(base->temp_entries, mac64);

    if (!temp_neigh) {
        return false;
    }
    ws_neighbor_class_entry_t *entry = ws_bootstrap_eapol_tx_temporary_set(base->interface_ptr, mac64);
    if (!entry) {
        return false;
    }
    *entry = temp_neigh->neigh_info_list;
    return true;

}


static void ws_llc_eapol_data_req_init(mcps_data_req_t *data_req, llc_message_t *message)
{
    memset(data_req, 0, sizeof(mcps_data_req_t));
    data_req->TxAckReq = message->ack_requested;
    data_req->DstPANId = message->pan_id;
    data_req->SrcAddrMode = message->src_address_type;
    if (!data_req->TxAckReq) {
        data_req->PanIdSuppressed = false;
        data_req->DstAddrMode = MAC_ADDR_MODE_NONE;
    } else {
        data_req->PanIdSuppressed = true;
        data_req->DstAddrMode = message->dst_address_type;
        memcpy(data_req->DstAddr, message->dst_address, 8);
    }


    data_req->msdu = NULL;
    data_req->msduLength = 0;
    data_req->msduHandle = message->msg_handle;

    ws_llc_lowpan_mpx_header_set(message, MPX_KEY_MANAGEMENT_ENC_USER_ID);
}

static void ws_llc_mpx_eapol_send(llc_data_base_t *base, llc_message_t *message)
{
    mcps_data_req_t data_req;
    llc_message_id_allocate(message, base, true);
    base->llc_message_list_size++;
    ns_list_add_to_end(&base->llc_message_list, message);
    message->eapol_temporary = ws_llc_eapol_temp_entry_set(base, message->dst_address);
    ws_llc_eapol_data_req_init(&data_req, message);
    base->temp_entries->active_eapol_session = true;
    base->interface_ptr->mac_api->mcps_data_req_ext(base->interface_ptr->mac_api, &data_req, &message->ie_ext, NULL);
}


static void ws_llc_mpx_eapol_request(llc_data_base_t *base, mpx_user_t *user_cb, const struct mcps_data_req_s *data)
{
    wh_ie_sub_list_t ie_header_mask;
    memset(&ie_header_mask, 0, sizeof(wh_ie_sub_list_t));

    wp_nested_ie_sub_list_t nested_wp_id;
    memset(&nested_wp_id, 0, sizeof(wp_nested_ie_sub_list_t));
    ie_header_mask.utt_ie = true;
    ie_header_mask.bt_ie = ws_eapol_relay_state_active(base->interface_ptr);
    ie_header_mask.ea_ie = ws_eapol_handshake_first_msg(data->msdu, data->msduLength, base->interface_ptr);
    nested_wp_id.bs_ie = ie_header_mask.ea_ie;


    nested_wp_id.us_ie = true;

    uint16_t ie_header_length = ws_wh_headers_length(ie_header_mask, &base->ie_params);
    uint16_t nested_ie_length = ws_wp_nested_message_length(nested_wp_id, &base->ie_params);

    uint16_t over_head_size = ie_header_length;
    if (nested_ie_length) {
        over_head_size += nested_ie_length + 2;
    }
    //Mpx header size
    over_head_size += 5; //MPX FuLL frame 3 bytes + IE header 2 bytes

    //Allocate Message
    llc_message_t *message = llc_message_allocate(over_head_size, base);
    if (!message) {
        mcps_data_conf_t data_conf;
        memset(&data_conf, 0, sizeof(mcps_data_conf_t));
        data_conf.msduHandle = data->msduHandle;
        data_conf.status = MLME_TRANSACTION_OVERFLOW;
        user_cb->data_confirm(&base->mpx_data_base.mpx_api, &data_conf);
        return;
    }
    message->mpx_user_handle = data->msduHandle;
    message->ack_requested = data->TxAckReq;

    message->src_address_type = data->SrcAddrMode;
    memcpy(message->dst_address, data->DstAddr, 8);
    message->dst_address_type = data->DstAddrMode;
    message->pan_id = data->DstPANId;
    message->messsage_type = WS_FT_EAPOL;

    uint8_t *ptr = ws_message_buffer_ptr_get(message);

    message->ie_vector_list[0].ieBase = ptr;
    //Write UTT
    ptr = ws_wh_utt_write(ptr, message->messsage_type);
    if (ie_header_mask.bt_ie) {
        ptr = ws_wh_bt_write(ptr);
    }

    if (ie_header_mask.ea_ie) {
        uint8_t eapol_auth_eui64[8];
        ws_pae_controller_border_router_addr_read(base->interface_ptr, eapol_auth_eui64);
        ptr = ws_wh_ea_write(ptr, eapol_auth_eui64);
    }

    message->ie_vector_list[0].iovLen = ie_header_length;
    message->ie_ext.headerIeVectorList = &message->ie_vector_list[0];
    message->ie_ext.headerIovLength = 1;
    message->ie_ext.payloadIeVectorList = &message->ie_vector_list[1];
    message->ie_ext.payloadIovLength = 2;
    message->ie_vector_list[1].ieBase = ptr;

    if (nested_ie_length) {
        ptr = ws_wp_base_write(ptr, nested_ie_length);
        //Write unicast schedule
        ptr = ws_wp_nested_hopping_schedule_write(ptr, base->ie_params.hopping_schedule, true);

        if (nested_wp_id.bs_ie) {
            //Write Broadcastcast schedule
            ptr = ws_wp_nested_hopping_schedule_write(ptr, base->ie_params.hopping_schedule, false);
        }
    }

    // SET Payload IE Length
    message->ie_vector_list[1].iovLen = ptr - (uint8_t *)message->ie_vector_list[1].ieBase;
    message->ie_vector_list[2].ieBase = data->msdu;
    message->ie_vector_list[2].iovLen =  data->msduLength;

    if (base->temp_entries->active_eapol_session) {
        //Move to pending list
        ns_list_add_to_end(&base->temp_entries->llc_eap_pending_list, message);
    } else {
        ws_llc_mpx_eapol_send(base, message);
    }

}


static void ws_llc_mpx_data_request(const mpx_api_t *api, const struct mcps_data_req_s *data, uint16_t user_id)
{
    llc_data_base_t *base = ws_llc_discover_by_mpx(api);
    if (!base) {
        return;
    }

    mpx_user_t *user_cb = ws_llc_mpx_user_discover(&base->mpx_data_base, user_id);
    if (!user_cb || !user_cb->data_confirm || !user_cb->data_ind) {
        return;
    }

    if (user_id == MPX_KEY_MANAGEMENT_ENC_USER_ID) {
        ws_llc_mpx_eapol_request(base, user_cb, data);
    } else if (user_id == MPX_LOWPAN_ENC_USER_ID) {
        ws_llc_lowpan_mpx_data_request(base, user_cb, data);
    }
}


static int8_t ws_llc_mpx_data_cb_register(const mpx_api_t *api, mpx_data_confirm *confirm_cb, mpx_data_indication *indication_cb, uint16_t user_id)
{
    llc_data_base_t *base = ws_llc_discover_by_mpx(api);
    if (!base) {
        return -1;
    }

    mpx_user_t *user_cb = ws_llc_mpx_user_discover(&base->mpx_data_base, user_id);
    if (!user_cb) {
        return -1;
    }
    user_cb->data_confirm = confirm_cb;
    user_cb->data_ind = indication_cb;
    return 0;
}

static uint16_t ws_llc_mpx_header_size_get(const mpx_api_t *api, uint16_t user_id)
{
    llc_data_base_t *base = ws_llc_discover_by_mpx(api);
    if (!base) {
        return 0;
    }

    return ws_mpx_header_size_get(base, user_id);
}

static uint8_t ws_llc_mpx_data_purge_request(const mpx_api_t *api, struct mcps_purge_s *purge, uint16_t user_id)
{
    llc_data_base_t *base = ws_llc_discover_by_mpx(api);
    if (!base) {
        return MLME_INVALID_HANDLE;
    }
    llc_message_t *message = llc_message_discover_mpx_user_id(purge->msduHandle, user_id, &base->llc_message_list);
    if (!message) {
        return MLME_INVALID_HANDLE;
    }

    mcps_purge_t purge_req;
    uint8_t purge_status;
    purge_req.msduHandle = message->msg_handle;
    purge_status = base->interface_ptr->mac_api->mcps_purge_req(base->interface_ptr->mac_api, &purge_req);
    if (purge_status == 0) {
        llc_message_free(message, base);
    }

    return purge_status;
}

static void ws_llc_mpx_init(mpx_class_t *mpx_class)
{
    //Init Mbed Class and API
    mpx_class->mpx_user_table[0].user_id = MPX_LOWPAN_ENC_USER_ID;
    mpx_class->mpx_user_table[1].user_id = MPX_KEY_MANAGEMENT_ENC_USER_ID;
    mpx_class->mpx_api.mpx_headroom_size_get = &ws_llc_mpx_header_size_get;
    mpx_class->mpx_api.mpx_user_registration = &ws_llc_mpx_data_cb_register;
    mpx_class->mpx_api.mpx_data_request = &ws_llc_mpx_data_request;
    mpx_class->mpx_api.mpx_data_purge = &ws_llc_mpx_data_purge_request;
}

static void ws_llc_clean(llc_data_base_t *base)
{
    //Clean Message queue's
    mcps_purge_t purge_req;
    ns_list_foreach_safe(llc_message_t, message, &base->llc_message_list) {
        purge_req.msduHandle = message->msg_handle;
        llc_message_free(message, base);
        base->interface_ptr->mac_api->mcps_purge_req(base->interface_ptr->mac_api, &purge_req);

    }

    ns_list_foreach_safe(llc_message_t, message, &base->temp_entries->llc_eap_pending_list) {
        ns_list_remove(&base->temp_entries->llc_eap_pending_list, message);
        ns_dyn_mem_free(message);
    }
    base->temp_entries->active_eapol_session = false;
    memset(&base->ie_params, 0, sizeof(llc_ie_params_t));

    ws_llc_temp_neigh_info_table_reset(base->temp_entries);
}

static void ws_llc_temp_neigh_info_table_reset(temp_entriest_t *base)
{
    //Empty active list
    ns_list_init(&base->active_multicast_temp_neigh);
    ns_list_init(&base->active_eapol_temp_neigh);
    ns_list_init(&base->free_temp_neigh);

    //Add to free list to full
    for (int i = 0; i < MAX_NEIGH_TEMPORAY_LIST_SIZE; i++) {
        ns_list_add_to_end(&base->free_temp_neigh, &base->neighbour_temporary_table[i]);
    }
}

static ws_neighbor_temp_class_t *ws_llc_discover_mc_temp_entry(temp_entriest_t *base, const uint8_t *mac64)
{
    ns_list_foreach(ws_neighbor_temp_class_t, entry, &base->active_multicast_temp_neigh) {
        if (memcmp(entry->mac64, mac64, 8) == 0) {
            return entry;
        }
    }
    return NULL;
}

static ws_neighbor_temp_class_t *ws_llc_discover_eapol_temp_entry(temp_entriest_t *base, const uint8_t *mac64)
{
    ns_list_foreach(ws_neighbor_temp_class_t, entry, &base->active_eapol_temp_neigh) {
        if (memcmp(entry->mac64, mac64, 8) == 0) {
            return entry;
        }
    }
    return NULL;
}

static void ws_llc_release_eapol_temp_entry(temp_entriest_t *base, const uint8_t *mac64)
{
    ws_neighbor_temp_class_t *neighbor = ws_llc_discover_eapol_temp_entry(base, mac64);
    if (!neighbor) {
        return;
    }

    ns_list_remove(&base->active_eapol_temp_neigh, neighbor);
    ns_list_add_to_end(&base->free_temp_neigh, neighbor);
}

ws_neighbor_temp_class_t *ws_llc_get_multicast_temp_entry(protocol_interface_info_entry_t *interface, const uint8_t *mac64)
{
    llc_data_base_t *base = ws_llc_discover_by_interface(interface);
    if (!base) {
        return NULL;
    }

    return ws_llc_discover_mc_temp_entry(base->temp_entries, mac64);
}


static void ws_init_temporary_neigh_data(ws_neighbor_temp_class_t *entry, const uint8_t *mac64)
{
    //Clear Old data
    memset(&entry->neigh_info_list, 0, sizeof(ws_neighbor_class_entry_t));
    entry->neigh_info_list.rsl_in = RSL_UNITITIALIZED;
    entry->neigh_info_list.rsl_out = RSL_UNITITIALIZED;
    memcpy(entry->mac64, mac64, 8);
    entry->eapol_temp_info.eapol_rx_relay_filter = 0;
}



static ws_neighbor_temp_class_t *ws_allocate_multicast_temp_entry(temp_entriest_t *base, const uint8_t *mac64)
{

    ws_neighbor_temp_class_t *entry = ws_llc_discover_mc_temp_entry(base, mac64);
    if (entry) {
        ns_list_remove(&base->active_multicast_temp_neigh, entry);
        ns_list_add_to_start(&base->active_multicast_temp_neigh, entry);
        return entry;
    }

    if (ns_list_count(&base->active_multicast_temp_neigh) < MAX_NEIGH_TEMPORARY_MULTICAST_SIZE) {
        entry = ns_list_get_first(&base->free_temp_neigh);
    }

    if (entry) {
        ns_list_remove(&base->free_temp_neigh, entry);
    } else {
        //Replace last entry and put it to first
        entry = ns_list_get_last(&base->active_multicast_temp_neigh);
        ns_list_remove(&base->active_multicast_temp_neigh, entry);
    }
    //Add to list
    ns_list_add_to_start(&base->active_multicast_temp_neigh, entry);
    //Clear Old data
    ws_init_temporary_neigh_data(entry, mac64);
    return entry;
}

static ws_neighbor_temp_class_t *ws_allocate_eapol_temp_entry(temp_entriest_t *base, const uint8_t *mac64)
{

    ws_neighbor_temp_class_t *entry = ws_llc_discover_eapol_temp_entry(base, mac64);
    if (entry) {
        //TODO referesh Timer here
        return entry;
    }

    if (ns_list_count(&base->active_eapol_temp_neigh) < MAX_NEIGH_TEMPORRY_EAPOL_SIZE) {
        entry = ns_list_get_first(&base->free_temp_neigh);
    }

    if (!entry) {
        return NULL;
    }

    ns_list_remove(&base->free_temp_neigh, entry);
    //Add to list
    ns_list_add_to_start(&base->active_eapol_temp_neigh, entry);
    //Clear Old data
    ws_init_temporary_neigh_data(entry, mac64);
    return entry;
}

void ws_llc_free_multicast_temp_entry(protocol_interface_info_entry_t *cur, ws_neighbor_temp_class_t *neighbor)
{
    llc_data_base_t *base = ws_llc_discover_by_interface(cur);
    if (!base) {
        return;
    }
    ns_list_remove(&base->temp_entries->active_multicast_temp_neigh, neighbor);
    ns_list_add_to_end(&base->temp_entries->free_temp_neigh, neighbor);
}


int8_t ws_llc_create(struct protocol_interface_info_entry *interface, ws_asynch_ind *asynch_ind_cb, ws_asynch_confirm *asynch_cnf_cb, ws_neighbor_info_request *ws_neighbor_info_request_cb)
{
    llc_data_base_t *base = ws_llc_discover_by_interface(interface);
    if (base) {
        ws_llc_clean(base);
        return 0;
    }

    //Allocate Data base
    base = ws_llc_base_allocate();
    if (!base) {
        return -2;
    }

    base->interface_ptr = interface;
    base->asynch_ind = asynch_ind_cb;
    base->asynch_confirm = asynch_cnf_cb;
    base->ws_neighbor_info_request_cb = ws_neighbor_info_request_cb;
    //Register MAC Extensions
    base->interface_ptr->mac_api->mac_mcps_extension_enable(base->interface_ptr->mac_api, &ws_llc_mac_indication_cb, &ws_llc_mac_confirm_cb, &ws_llc_ack_data_req_ext);
    //Init MPX class
    ws_llc_mpx_init(&base->mpx_data_base);
    ws_llc_temp_neigh_info_table_reset(base->temp_entries);
    return 0;
}

int8_t ws_llc_delete(struct protocol_interface_info_entry *interface)
{
    llc_data_base_t *base = ws_llc_discover_by_interface(interface);
    if (!base) {
        return -1;
    }

    ws_llc_clean(base);

    ns_list_remove(&llc_data_base_list, base);
    //Disable Mac extension
    base->interface_ptr->mac_api->mac_mcps_extension_enable(base->interface_ptr->mac_api, NULL, NULL, NULL);
    ns_dyn_mem_free(base->temp_entries);
    ns_dyn_mem_free(base);
    return 0;
}



void ws_llc_reset(struct protocol_interface_info_entry *interface)
{
    llc_data_base_t *base = ws_llc_discover_by_interface(interface);
    if (!base) {
        return;
    }
    ws_llc_clean(base);
}

mpx_api_t *ws_llc_mpx_api_get(struct protocol_interface_info_entry *interface)
{
    llc_data_base_t *base = ws_llc_discover_by_interface(interface);
    if (!base) {
        return NULL;
    }
    return &base->mpx_data_base.mpx_api;
}

int8_t ws_llc_asynch_request(struct protocol_interface_info_entry *interface, asynch_request_t *request)
{
    llc_data_base_t *base = ws_llc_discover_by_interface(interface);
    if (!base) {
        return -1;
    }

    //Calculate IE Buffer size
    request->wh_requested_ie_list.fc_ie = false; //Never should not be a part Asynch message
    request->wh_requested_ie_list.rsl_ie = false; //Never should not be a part Asynch message
    request->wh_requested_ie_list.vh_ie = false;
    uint16_t header_buffer_length = ws_wh_headers_length(request->wh_requested_ie_list, &base->ie_params);
    uint16_t wp_nested_payload_length = ws_wp_nested_message_length(request->wp_requested_nested_ie_list, &base->ie_params);

    //Allocated
    uint16_t total_length = header_buffer_length;
    if (wp_nested_payload_length) {
        total_length += 2 + wp_nested_payload_length;
    }
    //Allocate LLC message pointer

    llc_message_t *message = llc_message_allocate(total_length, base);
    if (!message) {
        if (base->asynch_confirm) {
            base->asynch_confirm(interface, request->message_type);
        }
        return 0;
    }

    //Add To active list
    llc_message_id_allocate(message, base, false);
    base->llc_message_list_size++;
    ns_list_add_to_end(&base->llc_message_list, message);
    message->messsage_type = request->message_type;


    mcps_data_req_t data_req;
    memset(&data_req, 0, sizeof(mcps_data_req_t));
    data_req.SeqNumSuppressed = true;
    data_req.SrcAddrMode = MAC_ADDR_MODE_64_BIT;
    data_req.Key = request->security;
    data_req.msduHandle = message->msg_handle;
    if (request->message_type == WS_FT_PAN_ADVERT_SOL) {
        // PANID not know yet must be supressed
        data_req.PanIdSuppressed = true;
    }

    uint8_t *ptr = ws_message_buffer_ptr_get(message);

    message->ie_vector_list[0].ieBase = ptr;
    message->ie_vector_list[0].iovLen = header_buffer_length;

    message->ie_ext.headerIeVectorList = &message->ie_vector_list[0];
    message->ie_ext.headerIovLength = 1;


    //Write UTT
    if (request->wh_requested_ie_list.utt_ie) {
        ptr = ws_wh_utt_write(ptr, message->messsage_type);
    }

    if (request->wh_requested_ie_list.bt_ie) {
        //Static 5 bytes allways
        ptr = ws_wh_bt_write(ptr);
    }

    if (wp_nested_payload_length) {
        message->ie_vector_list[1].ieBase = ptr;
        message->ie_vector_list[1].iovLen = 2 + wp_nested_payload_length;
        message->ie_ext.payloadIeVectorList = &message->ie_vector_list[1];
        message->ie_ext.payloadIovLength = 1;
        ptr = ws_wp_base_write(ptr, wp_nested_payload_length);

        if (request->wp_requested_nested_ie_list.us_ie) {
            //Write unicast schedule
            ptr = ws_wp_nested_hopping_schedule_write(ptr, base->ie_params.hopping_schedule, true);
        }

        if (request->wp_requested_nested_ie_list.bs_ie) {
            //Write Broadcastcast schedule
            ptr = ws_wp_nested_hopping_schedule_write(ptr, base->ie_params.hopping_schedule, false);
        }

        if (request->wp_requested_nested_ie_list.pan_ie) {
            //Write Pan information
            ptr = ws_wp_nested_pan_info_write(ptr, base->ie_params.pan_congiguration);
        }

        if (request->wp_requested_nested_ie_list.net_name_ie) {
            //Write network name
            ptr = ws_wp_nested_netname_write(ptr, base->ie_params.network_name, base->ie_params.network_name_length);
        }

        if (request->wp_requested_nested_ie_list.pan_version_ie) {
            //Write pan version
            ptr = ws_wp_nested_pan_ver_write(ptr, base->ie_params.pan_congiguration);
        }

        if (request->wp_requested_nested_ie_list.gtkhash_ie) {
            //Write GTKHASH
            ptr = ws_wp_nested_gtkhash_write(ptr, base->ie_params.gtkhash, base->ie_params.gtkhash_length);
        }

        if (request->wp_requested_nested_ie_list.vp_ie) {
            //Write Vendor spesific payload
            ptr = ws_wp_nested_vp_write(ptr, base->ie_params.vendor_payload, base->ie_params.vendor_payload_length);
        }
    }

    base->interface_ptr->mac_api->mcps_data_req_ext(base->interface_ptr->mac_api, &data_req, &message->ie_ext, &request->channel_list);

    return 0;
}


void ws_llc_set_vendor_header_data(struct protocol_interface_info_entry *interface, uint8_t *vendor_header, uint8_t vendor_header_length)
{
    llc_data_base_t *base = ws_llc_discover_by_interface(interface);
    if (!base) {
        return;
    }
    base->ie_params.vendor_header_data = vendor_header;
    base->ie_params.vendor_header_length = vendor_header_length;
}


void ws_llc_set_vendor_payload_data(struct protocol_interface_info_entry *interface, uint8_t *vendor_payload, uint8_t vendor_payload_length)
{
    llc_data_base_t *base = ws_llc_discover_by_interface(interface);
    if (!base) {
        return;
    }

    base->ie_params.vendor_payload = vendor_payload;
    base->ie_params.vendor_payload_length = vendor_payload_length;
}


void ws_llc_set_network_name(struct protocol_interface_info_entry *interface, uint8_t *name, uint8_t name_length)
{
    llc_data_base_t *base = ws_llc_discover_by_interface(interface);
    if (!base) {
        return;
    }

    base->ie_params.network_name = name;
    base->ie_params.network_name_length = name_length;
}

void  ws_llc_set_gtkhash(struct protocol_interface_info_entry *interface, uint8_t *gtkhash)
{
    llc_data_base_t *base = ws_llc_discover_by_interface(interface);
    if (!base) {
        return;
    }

    base->ie_params.gtkhash = gtkhash;
    if (base->ie_params.gtkhash) {
        base->ie_params.gtkhash_length = 32;
    } else {
        base->ie_params.gtkhash_length = 0;
    }
}



void ws_llc_set_pan_information_pointer(struct protocol_interface_info_entry *interface, struct ws_pan_information_s *pan_information_pointer)
{
    llc_data_base_t *base = ws_llc_discover_by_interface(interface);
    if (!base) {
        return;
    }

    base->ie_params.pan_congiguration = pan_information_pointer;
}

void ws_llc_hopping_schedule_config(struct protocol_interface_info_entry *interface, struct ws_hopping_schedule_s *hopping_schedule)
{
    llc_data_base_t *base = ws_llc_discover_by_interface(interface);
    if (!base) {
        return;
    }
    base->ie_params.hopping_schedule = hopping_schedule;
}

void ws_llc_timer_seconds(struct protocol_interface_info_entry *interface, uint16_t seconds_update)
{
    llc_data_base_t *base = ws_llc_discover_by_interface(interface);
    if (!base) {
        return;
    }

    ns_list_foreach_safe(ws_neighbor_temp_class_t, entry, &base->temp_entries->active_eapol_temp_neigh) {
        if (entry->eapol_temp_info.eapol_timeout <= seconds_update) {
            ns_list_remove(&base->temp_entries->active_eapol_temp_neigh, entry);
            ns_list_add_to_end(&base->temp_entries->free_temp_neigh, entry);
        } else {
            entry->eapol_temp_info.eapol_timeout -= seconds_update;
            if (entry->eapol_temp_info.eapol_rx_relay_filter == 0) {
                //No active filter period
                continue;
            }

            //Update filter time
            if (entry->eapol_temp_info.eapol_rx_relay_filter <= seconds_update) {
                entry->eapol_temp_info.eapol_rx_relay_filter = 0;
            } else {
                entry->eapol_temp_info.eapol_rx_relay_filter -= seconds_update;
            }
        }
    }
}

bool ws_llc_eapol_relay_forward_filter(struct protocol_interface_info_entry *interface, const uint8_t *joiner_eui64, uint8_t mac_sequency, uint32_t rx_timestamp)
{
    llc_data_base_t *base = ws_llc_discover_by_interface(interface);
    if (!base) {
        return false;
    }

    ws_neighbor_temp_class_t *neighbor = ws_llc_discover_eapol_temp_entry(base->temp_entries, joiner_eui64);
    if (!neighbor) {
        llc_neighbour_req_t neighbor_info;
        //Discover here Normal Neighbour
        if (!base->ws_neighbor_info_request_cb(interface, joiner_eui64, &neighbor_info, false)) {
            return false;
        }
        return ws_neighbor_class_neighbor_duplicate_packet_check(neighbor_info.ws_neighbor, mac_sequency, rx_timestamp);
    }

    if (neighbor->eapol_temp_info.eapol_rx_relay_filter && neighbor->eapol_temp_info.last_rx_mac_sequency == mac_sequency) {
        return false;
    }
    neighbor->eapol_temp_info.last_rx_mac_sequency = mac_sequency;
    neighbor->eapol_temp_info.eapol_rx_relay_filter = 6; //Activate 5-5.99 seconds filter time
    return true;

}


#endif
