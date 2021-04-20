/*
 * Copyright (c) 2017-2018, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include "nsconfig.h"
#include <ns_types.h>
#include <nsdynmemLIB.h>
#include "thread_lowpower_api.h"
#include <string.h>
#include "ns_list.h"
#include "ns_trace.h"
#include "common_functions.h"


#include "thread_common.h"
#include "thread_config.h"
#include "eventOS_event_timer.h"
#include "MLE/mle.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "thread_lowpower_api.h"
#include "6LoWPAN/MAC/mac_data_poll.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "6LoWPAN/Thread/thread_tmfcop_lib.h"
#include "thread_management_if.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "NWK_INTERFACE/Include/protocol.h"

#ifdef HAVE_THREAD_V2

#define TRACE_GROUP "lpwr"

#define METRICS_LEN 18

typedef struct data_response_s {
    thread_lowpower_resp_cb *data_response_cb;
    uint16_t request_wait_timer;
    uint16_t series_flags;
    uint8_t probe_count;
    uint8_t destination_address[16];
    int8_t interface_id;
    uint8_t metrics_requested[METRICS_LEN];
    uint8_t metrics_len;
    uint8_t series_id;
} data_response_t;

typedef struct data_metrics_s {
    uint8_t metrics_requested[METRICS_LEN];
    uint8_t metrics_value[METRICS_LEN];
    uint8_t req_metrics_len;
    uint8_t address[16];
    uint16_t timeout_timer;
    bool link_metrics_ready: 1;
    uint16_t query_ID;
    uint8_t forward_series_id;
    uint8_t forward_series_flags;
    ns_list_link_t link;
} data_metrics_t;

static NS_LIST_DEFINE(thread_lowpower_data_metrics_instance_list, data_metrics_t, link);

static data_response_t data_response;
data_response_t *data_response_ptr = &data_response;
//Link Metrics Query TLV
#define LINK_METRIC_SUB_TLV_LINK_METRIC_REPORT         0
#define LINK_METRIC_SUB_TLV_LINK_METRIC_QUERY_ID       1
#define LINK_METRIC_SUB_TLV_LINK_METRIC_QUERY_OPTIONS  2

// Link Metrics Management TLV
#define LINK_METRIC_SUB_TLV_FORWARD_PROBE              3
#define LINK_METRIC_SUB_TLV_REVERSE_PROBE              4
#define LINK_METRIC_SUB_TLV_STATUS                     5
#define LINK_METRIC_SUB_TLV_TRACKING_CAPABILITIES      6
#define LINK_METRIC_SUB_TLV_ENHANCED_ACK_CONFIGURATION 7


static data_metrics_t *thread_lowpower_data_metrics_find_by_source_address(uint8_t *address)
{
    data_metrics_t *this = NULL;
    ns_list_foreach(data_metrics_t, cur_ptr, &thread_lowpower_data_metrics_instance_list) {
        if (memcmp(cur_ptr->address, address, 16) == 0) {
            this = cur_ptr;
            break;
        }
    }
    return this;
}

static data_metrics_t *thread_lowpower_data_metrics_create(uint8_t *address)
{
    data_metrics_t *this = thread_lowpower_data_metrics_find_by_source_address(address);

    if (!this) {
        this = ns_dyn_mem_alloc(sizeof(data_metrics_t));
    }
    if (!this) {
        return NULL;
    }
    memcpy(this->address, address, 16);
    memset(this->metrics_requested, 0, METRICS_LEN);
    memset(this->metrics_value, 0, METRICS_LEN);
    this->req_metrics_len = 0;
    this->timeout_timer = 0;
    this->link_metrics_ready = false;
    this->query_ID = 0;
    ns_list_add_to_start(&thread_lowpower_data_metrics_instance_list, this);
    return this;
}

void thread_lowpower_data_metrics_delete(uint8_t *address)
{
    data_metrics_t *this = thread_lowpower_data_metrics_find_by_source_address(address);
    if (!this) {
        return;
    }
    ns_list_remove(&thread_lowpower_data_metrics_instance_list, this);
    ns_dyn_mem_free(this);
    return;
}

static uint8_t *thread_lowpower_tlv_write_link_metrics_query_id(uint8_t *ptr, uint16_t query_id)
{
    *ptr++ = LINK_METRIC_SUB_TLV_LINK_METRIC_QUERY_ID;
    *ptr++ = 2;
    ptr = common_write_16_bit(query_id, ptr);
    return ptr;
}
static uint8_t *thread_lowpower_tlv_write_link_metrics_query_options(uint8_t *ptr, uint8_t *req_list_ptr, uint8_t req_list_len)
{
    *ptr++ = LINK_METRIC_SUB_TLV_LINK_METRIC_QUERY_OPTIONS;
    *ptr++ = req_list_len;
    memcpy(ptr, req_list_ptr, req_list_len);
    ptr += req_list_len;
    return ptr;
}
static uint8_t *thread_lowpower_tlv_write_link_metrics_query(uint8_t *ptr, uint8_t *req_list_ptr, uint8_t req_list_len, uint16_t query_id)
{
    *ptr++ = MLE_TYPE_LINK_METRICS_QUERY;
    if (req_list_len) {
        *ptr++ = 6 + req_list_len; // query id type + len + query id = 4 bytes, query options type + len + flags = 2 bytes + flags
    } else {
        *ptr++ = 4; // query id type + len + query id = 4 bytes
    }
    ptr = thread_lowpower_tlv_write_link_metrics_query_id(ptr, query_id);
    if (req_list_len) {
        ptr = thread_lowpower_tlv_write_link_metrics_query_options(ptr, req_list_ptr, req_list_len);
    }
    return ptr;
}
static uint8_t *thread_lowpower_tlv_write_metrics_forward_probe(uint8_t *ptr, uint8_t series_id, uint8_t series_flags, uint16_t timeout, uint8_t *req_list_ptr, uint8_t req_list_len)
{
    *ptr++ = LINK_METRIC_SUB_TLV_FORWARD_PROBE;
    *ptr++ = 4 + req_list_len; // query id type + len + query id = 4 bytes, query options type + len + flags = 2 bytes + flags
    *ptr++ = series_id;
    *ptr++ = series_flags;
    ptr = common_write_16_bit(timeout, ptr);
    memcpy(ptr, req_list_ptr, req_list_len);
    ptr += req_list_len;
    return ptr;
}
static uint8_t *thread_lowpower_tlv_write_status(uint8_t *ptr, uint16_t status)
{
    *ptr++ = LINK_METRIC_SUB_TLV_STATUS;
    *ptr++ = 2;
    ptr = common_write_16_bit(status, ptr);
    return ptr;
}
static uint8_t *thread_lowpower_tlv_write_metrics_management_forward_probe_request(uint8_t *ptr, uint8_t series_id, uint8_t series_flags, uint16_t timeout, uint8_t *req_list_ptr, uint8_t req_list_len)
{
    *ptr++ = MLE_TYPE_LINK_METRICS_MANAGEMENT;
    *ptr++ =  6 + req_list_len; // query id type + len + query id = 4 bytes, query options type + len + flags = 2 bytes + flags
    ptr = thread_lowpower_tlv_write_metrics_forward_probe(ptr, series_id, series_flags, timeout, req_list_ptr, req_list_len);
    return ptr;
}
static uint8_t *thread_lowpower_tlv_write_metrics_management_status(uint8_t *ptr, uint16_t status)
{
    *ptr++ = MLE_TYPE_LINK_METRICS_MANAGEMENT;
    *ptr++ =  4;
    ptr = thread_lowpower_tlv_write_status(ptr, status);
    return ptr;
}
static int thread_lowpower_requested_metrics_management_query_save(uint8_t *address, uint8_t *data_ptr, uint16_t data_len)
{
    mle_tlv_info_t query_info;
    uint8_t *forward_probe_ptr;
    uint8_t forward_probe_len;
    data_metrics_t *this;

    if (mle_tlv_option_discover(data_ptr, data_len, MLE_TYPE_LINK_METRICS_MANAGEMENT, &query_info) < 4) {
        // No query TLV present
        return 0;
    }
    forward_probe_len = thread_tmfcop_tlv_find(query_info.dataPtr, query_info.tlvLen, LINK_METRIC_SUB_TLV_FORWARD_PROBE, &forward_probe_ptr);

    if (forward_probe_len < 5) {
        // Not present or length not enough
        return 0;
    }

    this = thread_lowpower_data_metrics_create(address);

    if (!this) {
        //query aready exists
        tr_error("query not created");
        return -3;
    }
    tr_debug("saving link metrics requested");

    this->link_metrics_ready = false;
    memcpy(this->address, address, 16);
    this->forward_series_id = *forward_probe_ptr++;
    this->forward_series_flags = *forward_probe_ptr++;
    this->timeout_timer = common_read_16_bit(forward_probe_ptr);
    forward_probe_ptr += 2;
    this->req_metrics_len = forward_probe_len - 4; // after 4 bytes omes the query types

    // Check that we dont go over maximum query count
    if (this->req_metrics_len > METRICS_LEN) {
        this->req_metrics_len = METRICS_LEN;
    }
    for (uint8_t i = 0; i < this->req_metrics_len; i++) {
        //save the read bytes for computing the results
        this->metrics_requested[i] = *forward_probe_ptr++;
    }
    tr_info("Forward probe query made by %s id:%d timeout:%d", trace_ipv6(this->address), this->forward_series_id, this->timeout_timer);
    return 0;
}
static int thread_lowpower_requested_single_query_save(uint8_t *address, uint8_t *data_ptr, uint16_t data_len)
{
    mle_tlv_info_t query_info;
    uint8_t *query_options_ptr;
    uint8_t query_options_len;
    data_metrics_t *this = NULL;

    if (!mle_tlv_type_requested(MLE_TYPE_LINK_METRICS_REPORT, data_ptr, data_len)) {
        //No single link metric requested so cant respond no need to process query
        return 0;
    }
    if (mle_tlv_option_discover(data_ptr, data_len, MLE_TYPE_LINK_METRICS_QUERY, &query_info) < 6) {
        return 0;
    }
    tr_debug("Query tlv found %s", trace_array(data_ptr, data_len));

    if (query_info.tlvLen < 6) {
        tr_warn("malformed query tlv"); // query tlv contains query id (length 4 bytes), query options - atleast 2 bytes
        return -2;
    }

    query_options_len = thread_tmfcop_tlv_find(query_info.dataPtr, query_info.tlvLen, LINK_METRIC_SUB_TLV_LINK_METRIC_QUERY_OPTIONS, &query_options_ptr);

    if (!query_options_len) {
        tr_warn("No Option TLV found");
        return 0;
    }

    this = thread_lowpower_data_metrics_create(address);

    if (!this) {
        //query aready exists
        tr_error("query not created");
        return -3;
    }
    tr_debug("saving link metrics requested");
    memcpy(this->address, address, 16);
    this->timeout_timer = 2; // We wait max 2 seconds for single probe Although 0 is enough as response should be written immediately
    this->link_metrics_ready = true;
    this->req_metrics_len = query_options_len; // first 4 bytes are for query id, 5th byte is query options sub tlv type
    thread_tmfcop_tlv_data_get_uint16(query_info.dataPtr, query_info.tlvLen, LINK_METRIC_SUB_TLV_LINK_METRIC_QUERY_ID, &this->query_ID);

    /* Go through the requested data - first 4 bytes id and next two bytes type and length of query options subtlv.
     * So start from the 7th byte*/
    // query tlv contains query id (length 4 bytes), query options - atleast 2 bytes
    if (this->req_metrics_len > METRICS_LEN) {
        this->req_metrics_len = METRICS_LEN;
    }
    for (uint8_t i = 0; i < this->req_metrics_len; i++) {
        //save the read bytes for computing the results
        this->metrics_requested[i] = *query_options_ptr++;
    }

    return 0;
}

static int thread_lowpower_mle_command_send(protocol_interface_info_entry_t *cur, uint8_t *address, uint8_t series_id, uint8_t series_flags, uint16_t timeout, uint8_t *metrics_ptr, uint8_t metrics_len)
{
    uint32_t keySequence;
    uint16_t buf_id;
    uint8_t request_tlv[1];
    uint8_t mle_command = MLE_COMMAND_DATA_REQUEST;

    if (series_flags || timeout) {
        mle_command = MLE_COMMAND_METRIC_MANAGEMENT_REQUEST;
    }
    //Leader data 10 bytes
    //query: query tlv (1 byte), query length (1 byte), query_id (4 bytes), query_options (1 byte) + query_len (1 byte) + query (length bytes) = 8 + length
    //request tlv 3 bytes
    buf_id = mle_service_msg_allocate(cur->id, 10 + 8 + metrics_len + 3 + (1 + thread_leader_data_tlv_size(cur)), false, mle_command);

    if (0 == buf_id) {
        return -1;
    }
    uint8_t *ptr = mle_service_get_data_pointer(buf_id);
    //request TLV requesting link metrics report
    if (series_flags || timeout) {
        // Always enough room
        ptr = thread_lowpower_tlv_write_metrics_management_forward_probe_request(ptr, series_id, series_flags, timeout, metrics_ptr, metrics_len);
    } else {
        request_tlv[0] = MLE_TYPE_LINK_METRICS_REPORT;
        ptr = mle_tlv_req_tlv(ptr, request_tlv, 1);
        ptr = thread_lowpower_tlv_write_link_metrics_query(ptr, metrics_ptr, metrics_len, series_id);
        ptr = thread_active_timestamp_write(cur, ptr); // 10 bytes
        //SET Leader Data
        ptr = thread_leader_data_tlv_write(ptr, cur);
    }

    if (0 != mle_service_update_length_by_ptr(buf_id, ptr)) {
        tr_error("Buffer overflow at message write");
    }

    mac_data_poll_init_protocol_poll(cur);
    mle_service_set_msg_destination_address(buf_id, address);
    //Set Security
    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(buf_id, 5, 2, keySequence);
    mle_service_send_message(buf_id);

    return 0;
}
static int thread_lowpower_mle_probe_send(protocol_interface_info_entry_t *cur, uint8_t *address)
{
    uint32_t keySequence;
    uint16_t buf_id;

    //Leader data 10 bytes
    //query: query tlv (1 byte), query length (1 byte), query_id (4 bytes), query_options (1 byte) + query_len (1 byte) + query (length bytes) = 8 + length
    //request tlv 3 bytes
    buf_id = mle_service_msg_allocate(cur->id, 10, false, MLE_COMMAND_PROBE);

    if (0 == buf_id) {
        return -1;
    }
    uint8_t *ptr = mle_service_get_data_pointer(buf_id);
    ptr = thread_leader_data_tlv_write(ptr, cur);

    if (0 != mle_service_update_length_by_ptr(buf_id, ptr)) {
        tr_error("Buffer overflow at message write");
    }

    mac_data_poll_init_protocol_poll(cur);
    mle_service_set_msg_destination_address(buf_id, address);
    //Set Security
    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(buf_id, 5, 2, keySequence);
    mle_service_send_message(buf_id);

    return 0;
}
int thread_lowpower_test_probe_send(int8_t interface_id, uint8_t *address, uint8_t *metrics_ptr, uint8_t metrics_len, thread_lowpower_resp_cb response_cb)
{
    return thread_lowpower_metrics_management_request_send(interface_id, address, 0, 0, 0, metrics_ptr, metrics_len, response_cb);
}
int thread_lowpower_metrics_management_request_send(int8_t interface_id, uint8_t *address, uint8_t series_id, uint8_t series_flags, uint16_t timeout,  uint8_t *metrics_ptr, uint8_t metrics_len, thread_lowpower_resp_cb response_cb)
{
    uint8_t dest_address[16] = {0};
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);

    if (!cur) {
        return -1;
    }

    if (data_response_ptr->data_response_cb) {
        tr_warn("low power request already in process");
        return -2;
    }

    //if no address is specified use parent's address
    if (!address) {
        thread_management_get_parent_address(interface_id, dest_address);
    } else {
        memcpy(dest_address, address, 16);
    }

    tr_info("destination address = %s metrics queried %s", trace_ipv6(dest_address), trace_array(metrics_ptr, metrics_len));
    data_response_ptr->data_response_cb = response_cb;
    memcpy(data_response_ptr->destination_address, dest_address, 16);
    data_response_ptr->interface_id = interface_id;
    data_response_ptr->request_wait_timer = timeout; // wait for 3s for a response to the probe req
    data_response_ptr->series_flags = series_flags;
    data_response_ptr->series_id =  series_id;
    data_response_ptr->probe_count = 3 + 1; // last probe is result rerieval message
    thread_lowpower_mle_command_send(cur, dest_address, series_id, series_flags, timeout, metrics_ptr, metrics_len);

    return 0;
}

static void thread_lowpower_request_timeout_cb(data_response_t *data_req, uint8_t *metrics_ptr, uint8_t metrics_len)
{
    if (metrics_len == 0) {
        tr_warn("Clearing timers without response");
    }
    if (data_response_ptr->data_response_cb) {
        data_response_ptr->data_response_cb(data_req->destination_address, data_req->interface_id, metrics_ptr, metrics_len);
    }
    data_req->request_wait_timer = 0;
    data_req->data_response_cb = NULL;
    memset(data_req->destination_address, 0, 16);
    data_req->interface_id = 0;
    return;
}

void thread_lowpower_process_response(uint8_t *src_address, int8_t instance_id, uint8_t *data_ptr, uint16_t data_len)
{
    (void) instance_id;
    mle_tlv_info_t linkMetricsReport;
    if (memcmp(src_address, data_response_ptr->destination_address, 16) != 0) {
        return;
    }

    if (!mle_tlv_read_tlv(MLE_TYPE_LINK_METRICS_REPORT, data_ptr, data_len, &linkMetricsReport)) {
        return;
    }

    thread_lowpower_request_timeout_cb(data_response_ptr, data_ptr, data_len);

    return;
}

int thread_lowpower_process_request(mle_message_t *mle_msg)
{
    if (!mle_msg->packet_src_address) {
        return -1;
    }
    if (mle_msg->message_type != MLE_COMMAND_DATA_REQUEST &&
            mle_msg->message_type != MLE_COMMAND_METRIC_MANAGEMENT_REQUEST) {
        // No need to process
        return 0;
    }
    // Process single shot queries
    thread_lowpower_requested_single_query_save(mle_msg->packet_src_address, mle_msg->data_ptr, mle_msg->data_length);
    // Check if we have metrics management query
    thread_lowpower_requested_metrics_management_query_save(mle_msg->packet_src_address, mle_msg->data_ptr, mle_msg->data_length);

    if (mle_tlv_type_requested(MLE_TYPE_LINK_METRICS_REPORT, mle_msg->data_ptr, mle_msg->data_length)) {
        //request made for metrics report
        tr_debug("link metrics report requested");
        data_metrics_t *metrics = thread_lowpower_data_metrics_find_by_source_address(mle_msg->packet_src_address);
        if (metrics) {
            // make report ready so the next data response message will include the report
            tr_debug("link metrics report ready");
            metrics->link_metrics_ready = true;
        }
    }

    return 0;
}

int thread_lowpower_timer(protocol_interface_info_entry_t *cur, uint32_t ticks)
{
    (void) cur;
    if (data_response_ptr->request_wait_timer > 0) {
        if (data_response_ptr->series_flags) {
            if (data_response_ptr->probe_count > 0) {
                if (data_response_ptr->probe_count == 1) {
                    // Send response retrieval after last probe
                    thread_lowpower_mle_command_send(cur, data_response_ptr->destination_address, data_response_ptr->series_id, 0, 0, NULL, 0);

                } else {
                    thread_lowpower_mle_probe_send(cur, data_response_ptr->destination_address);
                }
                data_response_ptr->probe_count--;
            }
            // Send probe we only support MLE probe
        }

        if (data_response_ptr->request_wait_timer > ticks) {
            data_response_ptr->request_wait_timer -= ticks;
        } else {
            thread_lowpower_request_timeout_cb(data_response_ptr, NULL, 0);
        }
    }
    ns_list_foreach_safe(data_metrics_t, low_power_metric_ptr, &thread_lowpower_data_metrics_instance_list) {
        if (low_power_metric_ptr->timeout_timer > ticks) {
            low_power_metric_ptr->timeout_timer -= ticks;
        } else {
            //query is timed out.
            thread_lowpower_data_metrics_delete(low_power_metric_ptr->address);
        }
    }
    return 0;
}

uint8_t thread_lowpower_link_metrics_length(protocol_interface_info_entry_t *cur, uint8_t *destination_address)
{
    (void) cur;
    data_metrics_t *metrics = thread_lowpower_data_metrics_find_by_source_address(destination_address);
    if (!metrics) {
        return 0;
    }
    if (!metrics->link_metrics_ready) {
        return 0;
    }
    //link metrics report (4 bytes)
    return 4 + metrics->req_metrics_len * 2;

}

uint8_t *thread_lowpower_link_metrics_write(protocol_interface_info_entry_t *cur, uint8_t *destination_address, uint8_t *ptr)
{
    (void)cur;
    data_metrics_t *metrics = thread_lowpower_data_metrics_find_by_source_address(destination_address);
    if (!metrics) {
        return ptr;
    }
    if (!metrics->link_metrics_ready) {
        tr_debug("link metrics not ready for destination");
        return ptr;
    }
    *ptr++ = MLE_TYPE_LINK_METRICS_REPORT;
    uint8_t req_metrics_len = metrics->req_metrics_len; // first 4 bytes are for query id, 5th byte is query options sub tlv type
    *ptr++ = req_metrics_len * 4; // metric report subtlv, metric report length, metric, and response - each one byte

    /* Go through the requested data - first 4 bytes id and next two bytes type and length of query options subtlv.
     * So start from the 7th byte*/
    // assuming one byte result follows one byte request flag
    for (uint8_t i = 0; i < req_metrics_len; i++) {
        //save the read bytes for computing the results
        *ptr++ = LINK_METRIC_SUB_TLV_LINK_METRIC_REPORT;
        *ptr++ = 2; // report length 2 bytes
        *ptr++ = metrics->metrics_requested[i];
        *ptr++ = metrics->metrics_value[i];
    }

    tr_debug("link metrics requested %s", trace_array(metrics->metrics_requested, metrics->req_metrics_len));
    tr_debug("link metrics values %s", trace_array(metrics->metrics_value, metrics->req_metrics_len));

    thread_lowpower_data_metrics_delete(metrics->address);
    // this assumes request flags are written first followed by results
    //    for (uint8_t i = (6 + req_metrics_len); i < (6 + req_metrics_len*2); i++) {
    //        *ptr++ = thread_lowpower_data_metrics_requested_ptr->thread_link_metrics_responses[i-(6 + req_metrics_len)] ; //add zeros as results - needs to be updated
    //    }
    return ptr;
}

static int thread_lowpower_metrics_management_query_response_msg(protocol_interface_info_entry_t *cur, uint8_t *dst_address)
{
    uint16_t length = 16; // Leader data 10 bytes + link metrics status 6 bytes
    uint8_t *ptr;

    //link metrics info
    length += thread_lowpower_link_metrics_length(cur, dst_address);

    uint16_t bufId =  mle_service_msg_allocate(cur->id, length, false, MLE_COMMAND_METRIC_MANAGEMENT_RESPONSE);

    if (bufId == 0) {
        return -1;
    }

    tr_debug("Send MLE Management Query response, %s", trace_ipv6(dst_address));
    ptr = mle_service_get_data_pointer(bufId);

    ptr = thread_leader_data_tlv_write(ptr, cur);
    ptr = thread_lowpower_link_metrics_write(cur, dst_address, ptr);
    ptr = thread_lowpower_tlv_write_metrics_management_status(ptr, 0);

    if (mle_service_update_length_by_ptr(bufId, ptr) != 0) {
        tr_debug("Buffer overflow at message write");
    }
    mle_service_set_msg_destination_address(bufId, dst_address);
    //Set Security
    uint32_t keySequence;
    thread_management_get_current_keysequence(cur->id, &keySequence);
    mle_service_msg_update_security_params(bufId, 5, 2, keySequence);

    mle_service_send_message(bufId);
    return 0;


}

void thread_lowpower_metrics_management_query_request_process(protocol_interface_info_entry_t *cur, mle_message_t *mle_msg, mle_security_header_t *security_headers, uint8_t linkMargin)
{
    (void)cur;
    (void)security_headers;
    (void)linkMargin;

    tr_info("Recv MLE Metrics Management Query");

    thread_lowpower_metrics_management_query_response_msg(cur, mle_msg->packet_src_address);
}


#endif /* HAVE_THREAD_V2 */
