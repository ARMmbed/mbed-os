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
#include "fhss_config.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "mac_api.h"
#include "mac_mcps.h"
#include "Common_Protocols/ipv6_constants.h"
#include "socket_api.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mpx_api.h"
#include "6LoWPAN/ws/ws_config.h"
#include "6LoWPAN/ws/ws_eapol_pdu.h"

#ifdef HAVE_WS

#define TRACE_GROUP "wsep"

typedef struct {
    void *data_ptr;
    void *buffer;
    ws_eapol_pdu_tx_status *tx_status;
    uint8_t tx_identifier;
    uint8_t handle;
    ns_list_link_t link;
} eapol_pdu_msdu_t;

typedef NS_LIST_HEAD(eapol_pdu_msdu_t, link) eapol_pdu_msdu_list_t;

typedef struct {
    uint8_t priority;
    ws_eapol_pdu_address_check *addr_check;
    ws_eapol_pdu_receive *receive;
    ns_list_link_t link;
} eapol_pdu_recv_cb_t;

typedef NS_LIST_HEAD(eapol_pdu_recv_cb_t, link) eapol_pdu_recv_cb_list_t;

typedef struct {
    eapol_pdu_recv_cb_list_t recv_cb_list;                 /**< List of receive callbacks */
    eapol_pdu_msdu_list_t msdu_list;                       /**< MSDU list */
    ws_eapol_pdu_receive *receive;                         /**< data receive callback */
    protocol_interface_info_entry_t *interface_ptr;        /**< Interface pointer */
    mpx_api_t *mpx_api;                                    /**< MPX api */
    uint16_t mpx_user_id;                                  /**< MPX user identifier */
    uint8_t msdu_handle;                                   /**< MSDU handle */
    ns_list_link_t link;                                   /**< Link */
} eapol_pdu_data_t;

static void ws_eapol_pdu_mpx_data_confirm(const mpx_api_t *api, const struct mcps_data_conf_s *data);
static void ws_eapol_pdu_mpx_data_indication(const mpx_api_t *api, const struct mcps_data_ind_s *data);
static void ws_eapol_pdu_data_request_primitiv_set(mcps_data_req_t *dataReq, protocol_interface_info_entry_t *cur);
static eapol_pdu_data_t *ws_eapol_pdu_data_get(protocol_interface_info_entry_t *interface_ptr);

static NS_LIST_DEFINE(eapol_pdu_data_list, eapol_pdu_data_t, link);

int8_t ws_eapol_pdu_init(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return -1;
    }

    if (ws_eapol_pdu_data_get(interface_ptr) != NULL) {
        return 0;
    }

    eapol_pdu_data_t *eapol_pdu_data = ns_dyn_mem_alloc(sizeof(eapol_pdu_data_t));
    if (!eapol_pdu_data) {
        return -1;
    }

    eapol_pdu_data->interface_ptr = interface_ptr;
    ns_list_init(&eapol_pdu_data->recv_cb_list);
    ns_list_init(&eapol_pdu_data->msdu_list);
    eapol_pdu_data->msdu_handle = 0;

    ns_list_add_to_end(&eapol_pdu_data_list, eapol_pdu_data);

    return 0;
}

int8_t ws_eapol_pdu_delete(protocol_interface_info_entry_t *interface_ptr)
{
    if (!interface_ptr) {
        return -1;
    }

    eapol_pdu_data_t *eapol_pdu_data = ws_eapol_pdu_data_get(interface_ptr);

    if (!eapol_pdu_data) {
        return -1;
    }

    ns_list_foreach_safe(eapol_pdu_recv_cb_t, cb_entry, &eapol_pdu_data->recv_cb_list) {
        ns_list_remove(&eapol_pdu_data->recv_cb_list, cb_entry);
        ns_dyn_mem_free(cb_entry);
    }

    ns_list_foreach_safe(eapol_pdu_msdu_t, msdu_entry, &eapol_pdu_data->msdu_list) {
        ns_list_remove(&eapol_pdu_data->msdu_list, msdu_entry);
        ns_dyn_mem_free(msdu_entry);
    }

    ns_list_remove(&eapol_pdu_data_list, eapol_pdu_data);
    ns_dyn_mem_free(eapol_pdu_data);

    return 0;
}

int8_t ws_eapol_pdu_cb_register(protocol_interface_info_entry_t *interface_ptr, const eapol_pdu_recv_cb_data_t *cb_data)
{
    if (!interface_ptr || !cb_data) {
        return -1;
    }

    eapol_pdu_data_t *eapol_pdu_data =  ws_eapol_pdu_data_get(interface_ptr);

    if (!eapol_pdu_data) {
        return -1;
    }

    eapol_pdu_recv_cb_t *new_cb = ns_dyn_mem_alloc(sizeof(eapol_pdu_recv_cb_t));
    if (!new_cb) {
        return -1;
    }

    new_cb->priority = cb_data->priority;
    new_cb->addr_check = cb_data->addr_check;
    new_cb->receive = cb_data->receive;

    ns_list_foreach(eapol_pdu_recv_cb_t, entry, &eapol_pdu_data->recv_cb_list) {
        if (new_cb->priority <= entry->priority) {
            ns_list_add_before(&eapol_pdu_data->recv_cb_list, entry, new_cb);
            return 0;
        }
    }

    ns_list_add_to_end(&eapol_pdu_data->recv_cb_list, new_cb);
    return 0;
}

int8_t ws_eapol_pdu_cb_unregister(protocol_interface_info_entry_t *interface_ptr, const eapol_pdu_recv_cb_data_t *cb_data)
{
    if (!interface_ptr || !cb_data) {
        return -1;
    }

    eapol_pdu_data_t *eapol_pdu_data = ws_eapol_pdu_data_get(interface_ptr);

    if (!eapol_pdu_data) {
        return -1;
    }

    ns_list_foreach_safe(eapol_pdu_recv_cb_t, entry, &eapol_pdu_data->recv_cb_list) {
        if (entry->receive == cb_data->receive) {
            ns_list_remove(&eapol_pdu_data->recv_cb_list, entry);
            ns_dyn_mem_free(entry);
            return 0;
        }
    }

    return -1;
}

int8_t ws_eapol_pdu_send_to_mpx(protocol_interface_info_entry_t *interface_ptr, const uint8_t *eui_64, void *data, uint16_t size, void *buffer, ws_eapol_pdu_tx_status *tx_status, uint8_t tx_identifier)
{
    eapol_pdu_data_t *eapol_pdu_data = ws_eapol_pdu_data_get(interface_ptr);

    if (!eapol_pdu_data) {
        return -1;
    }

    mcps_data_req_t data_request;
    ws_eapol_pdu_data_request_primitiv_set(&data_request, eapol_pdu_data->interface_ptr);

    eapol_pdu_msdu_t *msdu_entry = ns_dyn_mem_temporary_alloc(sizeof(eapol_pdu_msdu_t));
    if (!msdu_entry) {
        return -1;
    }
    msdu_entry->data_ptr = data;
    msdu_entry->buffer = buffer;
    msdu_entry->handle = eapol_pdu_data->msdu_handle;
    msdu_entry->tx_status = tx_status;
    msdu_entry->tx_identifier = tx_identifier;
    ns_list_add_to_start(&eapol_pdu_data->msdu_list, msdu_entry);

    memcpy(data_request.DstAddr, eui_64, 8);
    data_request.msdu = data;
    data_request.msduLength = size;
    data_request.msduHandle = eapol_pdu_data->msdu_handle;

    eapol_pdu_data->msdu_handle++;

    eapol_pdu_data->mpx_api->mpx_data_request(eapol_pdu_data->mpx_api, &data_request, eapol_pdu_data->mpx_user_id);
    return 0;
}

static void ws_eapol_pdu_data_request_primitiv_set(mcps_data_req_t *dataReq, protocol_interface_info_entry_t *cur)
{
    memset(dataReq, 0, sizeof(mcps_data_req_t));

    dataReq->InDirectTx = false;
    dataReq->TxAckReq = true;
    dataReq->SrcAddrMode = ADDR_802_15_4_LONG;
    dataReq->DstAddrMode = ADDR_802_15_4_LONG;
    dataReq->DstPANId = mac_helper_panid_get(cur);
}

int8_t ws_eapol_pdu_mpx_register(protocol_interface_info_entry_t *interface_ptr, struct mpx_api_s *mpx_api, uint16_t mpx_user_id)
{
    if (!interface_ptr) {
        return -1;
    }

    eapol_pdu_data_t *eapol_pdu_data = ws_eapol_pdu_data_get(interface_ptr);

    if (!eapol_pdu_data) {
        return -1;
    }

    if (!mpx_api && eapol_pdu_data->mpx_api) {
        //Disable Data Callbacks from MPX Class
        eapol_pdu_data->mpx_api->mpx_user_registration(eapol_pdu_data->mpx_api, NULL, NULL, eapol_pdu_data->mpx_user_id);
    }

    eapol_pdu_data->mpx_api = mpx_api;
    eapol_pdu_data->mpx_user_id = mpx_user_id;

    if (eapol_pdu_data->mpx_api) {
        eapol_pdu_data->mpx_api->mpx_user_registration(eapol_pdu_data->mpx_api, ws_eapol_pdu_mpx_data_confirm, ws_eapol_pdu_mpx_data_indication, eapol_pdu_data->mpx_user_id);
    }
    return 0;
}

static void ws_eapol_pdu_mpx_data_confirm(const mpx_api_t *api, const struct mcps_data_conf_s *data)
{
    eapol_pdu_data_t *eapol_pdu_data = NULL;

    ns_list_foreach(eapol_pdu_data_t, entry, &eapol_pdu_data_list) {
        if (entry->mpx_api == api) {
            eapol_pdu_data = entry;
            break;
        }
    }

    if (!eapol_pdu_data) {
        return;
    }

    ns_list_foreach(eapol_pdu_msdu_t, msdu, &eapol_pdu_data->msdu_list) {
        if (msdu->handle == data->msduHandle) {
            if (msdu->tx_status) {
                eapol_pdu_tx_status_e status = EAPOL_PDU_TX_ERR_UNSPEC;
                if (data->status == MLME_SUCCESS) {
                    status = EAPOL_PDU_TX_OK;
                } else if (data->status == MLME_TX_NO_ACK) {
                    status = EAPOL_PDU_TX_ERR_TX_NO_ACK;
                }
                msdu->tx_status(eapol_pdu_data->interface_ptr, status, msdu->tx_identifier);
            }
            ns_dyn_mem_free(msdu->buffer);
            ns_list_remove(&eapol_pdu_data->msdu_list, msdu);
            ns_dyn_mem_free(msdu);
            return;
        }
    }
}

static void ws_eapol_pdu_mpx_data_indication(const mpx_api_t *api, const struct mcps_data_ind_s *data)
{
    if (!data || !data->msduLength || !data->msdu_ptr) {
        return;
    }

    eapol_pdu_data_t *eapol_pdu_data = NULL;

    ns_list_foreach(eapol_pdu_data_t, entry, &eapol_pdu_data_list) {
        if (entry->mpx_api == api) {
            eapol_pdu_data = entry;
            break;
        }
    }

    if (!eapol_pdu_data) {
        return;
    }

    ns_list_foreach(eapol_pdu_recv_cb_t, entry, &eapol_pdu_data->recv_cb_list) {
        if (entry->addr_check(eapol_pdu_data->interface_ptr, data->SrcAddr) >= 0) {
            entry->receive(eapol_pdu_data->interface_ptr, data->SrcAddr, data->msdu_ptr, data->msduLength);
            break;
        }
    }
}

static eapol_pdu_data_t *ws_eapol_pdu_data_get(protocol_interface_info_entry_t *interface_ptr)
{
    eapol_pdu_data_t *eapol_pdu_data = NULL;

    ns_list_foreach(eapol_pdu_data_t, entry, &eapol_pdu_data_list) {
        if (entry->interface_ptr == interface_ptr) {
            eapol_pdu_data = entry;
            break;
        }
    }

    return eapol_pdu_data;
}

#endif /* HAVE_WS */

