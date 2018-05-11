/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
#include <ns_types.h>
#include "ns_trace.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_event_timer.h"
#include "nsdynmemLIB.h"
#include "ns_list.h"
#include "randLIB.h"
#include "socket_api.h"
#include "Core/include/socket.h"
#include "net_interface.h"
#include "common_functions.h"
#include "Common_Protocols/ipv6_constants.h"
#include "NWK_INTERFACE/Include/protocol.h" // just for protocol_core_monotonic_time
#include "Service_Libs/mle_service/mle_service_api.h"
#include "Service_Libs/mle_service/mle_service_security.h"
#include "Service_Libs/mle_service/mle_service_buffer.h"
#include "Service_Libs/mle_service/mle_service_interface.h"
#include "MLE/mle.h"
#include "MLE/mle_tlv.h"
#include "mac_common_defines.h"
#include "6LoWPAN/MAC/mac_helper.h"

#define TRACE_GROUP "mleS"

/* Fixed-point randomisation limits for randlib_randomise_base() - RFC 3315
 * says RAND is uniformly distributed between -0.1 and +0.1
 */
#define MLE_RAND_LOW   0x7333 // 1 - 0.1; minimum for "1+RAND"
#define MLE_RAND_HIGH  0x8CCD // 1 + 0.1; maximum for "1+RAND"

typedef struct {
    int8_t mle_socket;
    int8_t mle_socket_service_tasklet;
    uint8_t mle_adata[46];
    uint8_t mle_adata_length;
    arm_event_storage_t *mle_service_timer_storage;
    bool mle_frame_counter_check_enabled: 1;
    bool mle_frag_msg_security_enabled: 1;
    bool mle_accept_invalid_frame_counter: 1;
} mle_service_class_t;

#define MLE_SOCKET_SERVICE_TASKLET_INIT      1
#define MLE_SOCKET_SERVICE_TIMER             2

#define MLE_SOCKET_SERVICE_TIMER_ID          1

#define MLE_SOCKET_TIMER_UPDATE_PERIOD_IN_MS 100

mle_service_class_t *mle_service = NULL;

#ifdef MLE_TEST
static mle_service_filter_cb *receive_filter_cb = NULL;
#endif

static uint8_t *mle_security_aux_header_write(uint8_t *ptr, const mle_security_header_t *auxHeader);
static void mle_security_aux_ccm_nonce_set(uint8_t *noncePtr, uint8_t *mac64, uint32_t securityFrameCounter, uint8_t securityLevel);
static uint8_t mle_security_aux_header_size(uint8_t keyIdMode);

/**
 * Enable service Timeout timer
 */
static void mle_service_timer_start(void);

/**
 * Disable service Timeout timer
 */
static void mle_service_timer_stop(void);

/**
 * MLE service event handler
 */
static void mle_socket_service_tasklet(arm_event_s *event);

/**
 * MLE service allocated and initialized
 */
static bool mle_service_allocate(void);

/**
 * Create TX buffer and add to list
 */
static mle_service_msg_buf_t *mle_tr_create(uint16_t buffer_length);

/**
 * Parse Security Header
 */
static buffer_t *mle_service_parse_security_header(buffer_t *buf, mle_security_header_t *securityHeader,
        uint16_t *header_len);

/**
 * Decode secured message
 */
static buffer_t *mle_service_message_security_decode(buffer_t *buf, mle_security_header_t *securityHeader,
        uint8_t *security_key);

/**
 *  Mle service generic MLE message handler
 */
static void mle_service_socket_callback(void *cb);

static int mle_service_build_packet_send(service_instance_t *srv_ptr, mle_security_components_t *sec_params, mle_service_msg_buf_t *buffer);

/**
 * Check message sent tokens
 */
static bool mle_service_message_tokens_check(service_instance_t *cur_ptr, bool priority);

/**
 * Service instance timeout handler
 */
static bool mle_service_instance_timeout_handler(uint16_t ticks, service_instance_t *cur_ptr);


static void mle_service_timer_start(void)
{
    if (!mle_service->mle_service_timer_storage) {

        arm_event_s event = {
                .receiver = mle_service->mle_socket_service_tasklet,
                .sender = 0,
                .event_id = MLE_SOCKET_SERVICE_TIMER_ID,
                .data_ptr = NULL,
                .event_type = MLE_SOCKET_SERVICE_TIMER,
                .priority = ARM_LIB_LOW_PRIORITY_EVENT,
        };

        mle_service->mle_service_timer_storage  = eventOS_event_timer_request_every(&event, eventOS_event_timer_ms_to_ticks(MLE_SOCKET_TIMER_UPDATE_PERIOD_IN_MS));
        if (!mle_service->mle_service_timer_storage) {
            tr_error("Mle servicetimer start fail");
        }
    }
}
/**
 * Disable service Timeout timer
 */
static void mle_service_timer_stop(void)
{
    if (mle_service->mle_service_timer_storage) {
        eventOS_cancel(mle_service->mle_service_timer_storage);
        mle_service->mle_service_timer_storage = NULL;
    }
}

static void mle_service_tr_timeout_handler(mle_service_msg_buf_t *cur_ptr)
{
    uint16_t bufId = cur_ptr->msg_id;
    mle_service_message_timeout_cb *timeout = NULL;

    if (cur_ptr->timeout_cb && !cur_ptr->tokens_delay) {
        timeout = cur_ptr->timeout_cb;
    }

    if (!cur_ptr->delayed_response && !cur_ptr->tokens_delay) {
        //Update Retry Counter
        cur_ptr->retrans++;
        if (cur_ptr->retrans_max != 0 ) {

            if (cur_ptr->retrans >= cur_ptr->retrans_max) {
                // retransmission count exceeded.
                if (timeout) {
                    timeout(cur_ptr->interfaceId, cur_ptr->msg_id, true);
                }
                //Cuold be call timeout cb here
                mle_service_msg_free(bufId);
                return;
            } else {
                if (timeout) {
                    if (!timeout(cur_ptr->interfaceId, cur_ptr->msg_id, false) ) {
                        //User want to stop retry
                        mle_service_msg_free(bufId);
                        return;
                    }
                }
            }
        } else {
            mle_service_msg_free(bufId);
            return;
        }
    }
    service_instance_t *srv_ptr = mle_service_interface_find(cur_ptr->interfaceId);
    mle_security_components_t *sec_params = mle_service_security_params_get(cur_ptr->interfaceId);

    if (!srv_ptr || !sec_params) {
        mle_service_msg_free(bufId);
        return;
    }

    // Check if message needs to be delayed or removed because lack of message tokens
    if (!mle_service_message_tokens_check(srv_ptr, cur_ptr->tokens_priority)) {
        if (mle_service_buffer_tokens_delay_count() <= MLE_TOKEN_BUFFER_MAX_NBR) {
            // If message timeout has occurred delay because of tokens
            if (!cur_ptr->tokens_delay) {
                cur_ptr->tokens_delay = true;
                // Give time to wait free tokens
                if (cur_ptr->timeout < MLE_TOKEN_DELAY) {
                    cur_ptr->timeout = MLE_TOKEN_DELAY;
                }
            }
            // Delay has already been applied, now wait just for tokens
            cur_ptr->delayed_response = MLE_NO_DELAY;
        } else {
            mle_service_msg_free(bufId);
            tr_debug("MLE tokens message freed");
        }
        return;
    } else {
        cur_ptr->tokens_delay = false;
    }

    //Trig Buffer to socket
    mle_service_build_packet_send(srv_ptr, sec_params, cur_ptr);

    if (cur_ptr->retrans_max == 0) {
        mle_service_msg_free(bufId);//Remove packet which not need any retry after 1 retry
        return;
    }

    //Trig Retry or first packet
    if (cur_ptr->delayed_response ) {
        cur_ptr->delayed_response = MLE_NO_DELAY;
        cur_ptr->timeout = cur_ptr->timeout_init;
        return;
    }


    // RFC 3315 says:
    //     RT = 2*RTprev + RAND*RTprev,
    // We calculate this as
    //     RT = RTprev + (1+RAND)*RTprev
    cur_ptr->timeout = cur_ptr->timeout_init + randLIB_randomise_base(cur_ptr->timeout_init, MLE_RAND_LOW, MLE_RAND_HIGH);
    // Catch 16-bit integer overflow
    if (cur_ptr->timeout < cur_ptr->timeout_init) {
        cur_ptr->timeout = 0xFFFF;
    }
    // Check against MRT
    if (cur_ptr->timeout_max != 0 && cur_ptr->timeout > cur_ptr->timeout_max) {
        cur_ptr->timeout = randLIB_randomise_base(cur_ptr->timeout_max, MLE_RAND_LOW, MLE_RAND_HIGH);
    }
    cur_ptr->timeout_init = cur_ptr->timeout;
}

/**
 * MLE service timeout handling
 */
bool mle_service_timer_tick(uint16_t ticks)
{
    if (!mle_service ) {
        tr_debug("MLE not ready");
        return false;
    }

    bool active_timer_needed = false;

    // MLE service interface instance timeout handling
    if (mle_service_interface_timer_tick(ticks, mle_service_instance_timeout_handler)) {
        active_timer_needed = true;
    }

    // MLE service transaction timeout and retry handling
    if (mle_service_buffer_timer_tick(ticks, mle_service_tr_timeout_handler)) {
        active_timer_needed = true;
    }

    return active_timer_needed;
}

static void mle_socket_service_tasklet(arm_event_s *event)
{
    if (event->event_type == MLE_SOCKET_SERVICE_TIMER) {
        if (!mle_service_timer_tick(1)) {
            mle_service_timer_stop();
        }
    }
}

static bool mle_service_allocate(void)
{
    if (mle_service ) {
        return true;
    }
    mle_service = ns_dyn_mem_alloc(sizeof(mle_service_class_t));
    if (!mle_service) {
        return false;
    }

    mle_service->mle_socket = -1;
    mle_service->mle_socket_service_tasklet = eventOS_event_handler_create(mle_socket_service_tasklet, MLE_SOCKET_SERVICE_TASKLET_INIT);
    if (mle_service->mle_socket_service_tasklet < 0) {
        ns_dyn_mem_free(mle_service);
        mle_service = NULL;
        return false;
    }
    mle_service->mle_service_timer_storage = NULL;
    mle_service->mle_frame_counter_check_enabled = false;
    mle_service->mle_frag_msg_security_enabled = false;
    mle_service->mle_accept_invalid_frame_counter = false;

    return true;
}

static mle_service_msg_buf_t *mle_tr_create(uint16_t buffer_length)
{
    uint16_t tr_id;
    mle_service_msg_buf_t *msg_ptr;
    msg_ptr = mle_service_buffer_get(buffer_length);
    if (msg_ptr == NULL) {
        return NULL;
    }

    tr_id = randLIB_get_16bit();// 16 bits for random
    // Ensure a unique non-zero transaction id for each transaction
    while (tr_id == 0 || mle_service_buffer_find(tr_id) != NULL) {
        tr_id = tr_id + 1;
    }
    msg_ptr->msg_id = tr_id;
    return msg_ptr;
}

static void mle_service_set_src_ll64(service_instance_t *srv_ptr, uint8_t *address)
{
    memcpy(address,ADDR_LINK_LOCAL_PREFIX, 8);
    address += 8;
    memcpy(address,srv_ptr->mac64, 8);
    *address ^= 2;

}
static buffer_t *mle_security_interface_aux_header_parse(buffer_t *b, mle_security_header_t *auxSecHeader)
{
    uint8_t auxBaseHeader;
    uint8_t *ptr = buffer_data_pointer(b);

    auxBaseHeader = *ptr;

    auxSecHeader->KeyIdMode = (auxBaseHeader >> 3) & 3;
    auxSecHeader->securityLevel = auxBaseHeader & 7;
    auxSecHeader->frameCounter = common_read_32_bit_inverse(ptr + 1);
    ptr += 5;

    switch (auxSecHeader->KeyIdMode) {
        case MAC_KEY_ID_MODE_SRC8_IDX:
            memcpy(auxSecHeader->Keysource, ptr, 8);
            ptr += 8;
            auxSecHeader->KeyIndex = *ptr++;
            break;
        case MAC_KEY_ID_MODE_SRC4_IDX:
            memcpy(auxSecHeader->Keysource, ptr, 4);
            ptr += 4;
            auxSecHeader->KeyIndex = *ptr++;
            break;
        case MAC_KEY_ID_MODE_IDX:
            auxSecHeader->KeyIndex = *ptr++;
            break;
        case MAC_KEY_ID_MODE_IMPLICIT:
        default:
            auxSecHeader->KeyIndex = 0;
            break;
    }

    buffer_data_pointer_set(b, ptr);

    if (buffer_data_length(b) < 0) {
        b = buffer_free(b);
    }

    return b;
}

static buffer_t *mle_service_parse_security_header(buffer_t *buf, mle_security_header_t *securityHeader,
        uint16_t *header_len)
{
    uint16_t msg_len;

    msg_len = buffer_data_length(buf);
    buf = mle_security_interface_aux_header_parse(buf, securityHeader);
    if (!buf) {
        return NULL;
    }
    //Clear Headers off
    *header_len = (msg_len - buffer_data_length(buf));
    return buf;
}

/**
 * Build MLE ADATA for CCM security
 */
static void mle_service_a_data_set(uint8_t *ptr, uint8_t *src_address, uint8_t *dest_address)
{
    memcpy(ptr, src_address, 16);
    ptr += 16;
    memcpy(ptr, dest_address, 16);
}

static buffer_t *mle_service_message_security_decode(buffer_t *buf, mle_security_header_t *securityHeader,
        uint8_t *security_key)
{
    ccm_globals_t *ccm_ptr;
    uint16_t payload_len = buffer_data_length(buf);

    ccm_ptr = ccm_sec_init(securityHeader->securityLevel, security_key, AES_CCM_DECRYPT, 2);
    if (!ccm_ptr) {
        return buffer_free(buf);
    } else if (ccm_ptr->mic_len >= payload_len) {
        return buffer_free(buf);
    }

    //SET Nonce
    buf->src_sa.address[8] ^= 2;
    mle_security_aux_ccm_nonce_set(ccm_ptr->exp_nonce, &(buf->src_sa.address[8]),
            securityHeader->frameCounter, securityHeader->securityLevel);
    buf->src_sa.address[8] ^= 2;

    if (ccm_ptr->mic_len) {
        payload_len -= ccm_ptr->mic_len;
        buf->buf_end -= ccm_ptr->mic_len;

        ccm_ptr->data_ptr = buffer_data_pointer(buf);
        ccm_ptr->adata_ptr = mle_service->mle_adata;
        ccm_ptr->data_len = payload_len;
        ccm_ptr->adata_len = mle_service->mle_adata_length;
        //SET MIC
        ccm_ptr->mic = ccm_ptr->data_ptr;
        ccm_ptr->mic += payload_len;
    } else {
        ccm_ptr->data_len = payload_len;
        ccm_ptr->data_ptr = buffer_data_pointer(buf);
    }

    if (ccm_process_run(ccm_ptr) != 0) {
        tr_error("MLE mic fail!");
        buf = buffer_free(buf);
    }
    return buf;
}


static buffer_t *mle_message_security_header_set(buffer_t *buf,service_instance_t *srv_ptr,mle_security_components_t *sec_params, mle_security_header_t *security_header)
{
    uint8_t *ptr;
    //Verify first security level
    if (security_header->securityLevel) {
        //Get Security keys
        ccm_globals_t *ccm_ptr;
        uint16_t header_size;
        uint16_t data_len;
        data_len = buffer_data_length(buf);
        ptr = mle_service_security_get_key(security_header,sec_params, srv_ptr->interface_id );
        if (!ptr) {
            goto drop_buffer;
        }
        // Init
        ccm_ptr = ccm_sec_init(security_header->securityLevel, ptr, AES_CCM_ENCRYPT, 2);
        if (!ccm_ptr) {
            goto drop_buffer;
        }

        header_size = mle_security_aux_header_size(security_header->KeyIdMode);

        //SET Nonce
        mle_security_aux_ccm_nonce_set(ccm_ptr->exp_nonce, srv_ptr->mac64, security_header->frameCounter, security_header->securityLevel);
        if (ccm_ptr->mic_len) {
            buf = buffer_headroom(buf, (ccm_ptr->mic_len + 32 + header_size));
            if (buf) {
                uint8_t *ptr2;
                //Move current data to left by mic_len bytes
                ptr = buffer_data_pointer(buf);
                //Set new data pointer
                ptr2 = ptr;
                ptr2 -= ccm_ptr->mic_len;

                memmove(ptr2, ptr, data_len);

                //Cut Mic len
                buf->buf_end -= ccm_ptr->mic_len;

                ptr -= header_size + ccm_ptr->mic_len;
                ptr = mle_security_aux_header_write(ptr, security_header);
                ptr -= header_size;
                //Set pointer to Adata
                ptr -= (32);
                mle_service_a_data_set(ptr, buf->src_sa.address, buf->dst_sa.address);
                //Create ADATA
                ccm_ptr->adata_ptr = ptr;
                ccm_ptr->adata_len = (32 + header_size);

                //SET ptr to show to real payload
                buf->buf_ptr -= ccm_ptr->mic_len;
            } else {
                tr_warn("Security header alloc fail");
                buf = (buffer_t *) 0;
                return buf;
            }
        }
        ptr = buffer_data_pointer(buf);
        ccm_ptr->data_ptr = ptr;
        ccm_ptr->data_len = data_len;

        ccm_ptr->mic = ptr;
        ccm_ptr->mic += data_len;
        ccm_process_run(ccm_ptr);
        if (ccm_ptr->mic_len) {
            //SET Calculated mic
            buf->buf_end += ccm_ptr->mic_len;
        }
        buffer_data_reserve_header(buf, header_size);
    }

    buf = buffer_headroom(buf, 1);
    if (buf) {
        ptr = buffer_data_reserve_header(buf, 1);
        if (security_header->securityLevel) {
            *ptr++ = 0; // security
        } else {
            *ptr++ = 0xff; // No security
        }
    } else {
        tr_warn("HeadRoom Fail");
    }
    return buf;
    drop_buffer:

    return buffer_free(buf);
}

static int mle_service_build_packet_send(service_instance_t *srv_ptr, mle_security_components_t *sec_params, mle_service_msg_buf_t *buffer)
{
    buffer_t *buf;
    uint8_t *ptr;
    uint16_t header_length = 1; //Include security header

    //allocate Buffer for MLE header and Message length
    if (buffer->security_header.securityLevel) {
        header_length += mle_security_aux_header_size(buffer->security_header.KeyIdMode);

        header_length += 32; //AuthData
    }

    buf = buffer_get(buffer->buf_end + header_length);
    if (!buf) {
        return -1;
    }

    if (buffer->security_header.securityLevel) {
        uint8_t *dada_ptr = buffer->buf + 1;
        uint16_t dada_length = buffer->buf_end - 1;

        mle_tlv_info_t option_info;
        //Update mle frame counter evry time for every packet
        buffer->security_header.frameCounter = mle_service_security_get_framecounter(sec_params, true);
        buffer->security_header.KeyIndex = mle_service_security_get_default_key_id(sec_params);
        //Update MAC frame Counter and MLE frame counter to current one allways
        if (mle_tlv_option_discover(dada_ptr, dada_length, MLE_TYPE_LL_FRAME_COUNTER, &option_info) == 4) {
            // GET frame counter from interface
            uint32_t counter;
            if (mac_helper_link_frame_counter_read(srv_ptr->interface_id, &counter) == 0) {
                common_write_32_bit(counter,option_info.dataPtr);
            }
        }

        if (mle_tlv_option_discover(dada_ptr, dada_length, MLE_TYPE_MLE_FRAME_COUNTER, &option_info) == 4) {
            common_write_32_bit(buffer->security_header.frameCounter, option_info.dataPtr);
        }
    }

    //XXX Everything below this point is so broken. API of socket_buffer_sendmsg says buf should not have any metadata

    //Set LL64 Source (for mle_message_security_header_set, and sendmsg picks it up too, although it shouldn't)
    mle_service_set_src_ll64(srv_ptr,buf->src_sa.address);
    //Set Destination address (for benefit of mle_message_security_header_set, not sendmsg)
    memcpy(buf->dst_sa.address, buffer->dst_address, 16);

    // Normal Thread behaviour is for end devices to send multicasts as unicasts
    // to parent - this overrides.
    if (addr_is_ipv6_multicast(buffer->dst_address)) {
        buf->options.ll_broadcast_tx = true;
    }
    buf->src_sa.addr_type = buf->dst_sa.addr_type = ADDR_IPV6;
    buf->src_sa.port = buf->dst_sa.port = MLE_ALLOCATED_PORT;
    //Copy Payload
    ptr = buffer_data_pointer(buf);
    memcpy(ptr, buffer->buf, buffer->buf_end);
    buffer_data_length_set(buf, buffer->buf_end);

    //Add security header and encode message
    buf = mle_message_security_header_set(buf, srv_ptr, sec_params, &buffer->security_header);

    if (!buf) {
        return -1;
    }

    if (mle_service->mle_frag_msg_security_enabled) {
        // Enable link layer security for fragmented packets
        buf->options.ll_sec_bypass_frag_deny = true;
    }

    if (buffer->selected_channel) {
        buf->link_specific.ieee802_15_4.rf_channel_switch = true;
        buf->link_specific.ieee802_15_4.selected_channel = buffer->selected_rf_channel;
    }

    //Marks message sent
    buffer->message_sent = true;

    if (buffer->selected_pan_id) {
        buf->link_specific.ieee802_15_4.useDefaultPanId = false;
        buf->link_specific.ieee802_15_4.dstPanId = buffer->packet_panid;
    }

    int8_t security;

    if (buffer->enable_link_layer_security) {
        security = 1;
        if (buffer->psk_key_id_mode_2) {
            buf->link_specific.ieee802_15_4.key_id_mode = B_SECURITY_KEY_ID_2;
        } else {
            buf->link_specific.ieee802_15_4.key_id_mode = B_SECURITY_KEY_ID_MODE_DEFAULT;
        }
    } else {
        security = 0;
    }
    socket_setsockopt(mle_service->mle_socket, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, &security, sizeof(int8_t));

    //sendmsg always takes destination from msg, not from the buffer, so must specify there
    ns_address_t dst_addr;
    dst_addr.type = ADDRESS_IPV6;
    memcpy(dst_addr.address, buffer->dst_address, 16);
    dst_addr.identifier = MLE_ALLOCATED_PORT;

    ns_msghdr_t msg = {
        .msg_name = &dst_addr,
        .msg_namelen = sizeof dst_addr
    };
    //Push to socket
    //XXX This is so broken. API of socket_buffer_sendmsg says buf should not have any metadata
    socket_buffer_sendmsg(mle_service->mle_socket, buf, &msg, 0);
    return 0;
}

static void mle_service_socket_callback(void *cb)
{
    socket_buffer_callback_t *cb_buf = cb;
    uint8_t mle_security_byte;
    uint16_t security_header_length;
    mle_security_header_t securityHeader;
    mle_message_t mle_msg;
    if (!cb_buf) {
        tr_warn("mle sck cb without buf!");
        return;
    }
    buffer_t *buf = cb_buf->buf;
    if (cb_buf->event_type != SOCKET_DATA || !buf) {
        return;
    }

    service_instance_t *service_handler = mle_service_interface_find(cb_buf->interface_id);
    mle_security_components_t * sec_params = mle_service_security_params_get(cb_buf->interface_id);

    if (!service_handler || !sec_params) {
        tr_warn("service handler not registerd");
        goto error_handler;
    }

    // MLE messages are only allowed to Link local address
    if (!addr_is_ipv6_link_local(buf->src_sa.address) ||
         addr_ipv6_scope(buf->dst_sa.address, NULL) != IPV6_SCOPE_LINK_LOCAL) {
        // Security failure so drop packet
        goto error_handler;
    }

    //Start Parsing Header
    mle_security_byte = buffer_pull_uint8(buf);

    if (mle_security_byte == 0xff) {
        securityHeader.KeyIndex = 0;
        securityHeader.KeyIdMode = MAC_KEY_ID_MODE_IDX;
        securityHeader.securityLevel = 0;
    } else if(mle_security_byte == 0) {

        //SET adata
        uint8_t *sec_header_ptr = buffer_data_pointer(buf);


        buf = mle_service_parse_security_header(buf, &securityHeader, &security_header_length);
        if (!buf) {
            goto error_handler;
        }

        //SET adata and length
        mle_service->mle_adata_length = (32 + security_header_length);

        //copy data
        mle_service_a_data_set(mle_service->mle_adata, buf->src_sa.address, buf->dst_sa.address);
        memcpy(&mle_service->mle_adata[32],sec_header_ptr, security_header_length);
    } else {
        goto error_handler;
    }

    bool security_bypass;
    securityHeader.invalid_frame_counter = false;
    if (securityHeader.securityLevel) {

        if (sec_params->sec_level == 0) {
            tr_debug("Drop secured packet when security is disabled");
            goto error_handler;
        }
        //Get A key here
        uint8_t *key_ptr = mle_service_security_get_key(&securityHeader, sec_params, service_handler->interface_id);
        if (!key_ptr) {
            goto error_handler;
        }


        /* MLE neighbour table frame counter check */
        mle_neigh_table_entry_t *neighbour = NULL;
        uint32_t keySeq;
        if (securityHeader.KeyIdMode == MAC_KEY_ID_MODE_SRC4_IDX) {
            keySeq = common_read_32_bit(securityHeader.Keysource);
        } else {
            keySeq = (uint32_t)securityHeader.KeyIndex;
        }

        if (mle_service->mle_frame_counter_check_enabled) {
            neighbour = mle_class_discover_entry_by_ll64(service_handler->interface_id,buf->src_sa.address);
            if (neighbour){
                //key pending is set - incoming frame counter will be reset when new sequence is heard or lower framecounter is heard
                if (neighbour->new_key_pending) {
                    if ((securityHeader.frameCounter < neighbour->mle_frame_counter || keySeq != neighbour->last_key_sequence)) {
                        neighbour->mle_frame_counter = 0;
                        neighbour->last_key_sequence = keySeq;
                        neighbour->new_key_pending = false;
                        goto start_decode; //Skip validation part
                    }
                }
                //key pending not set - continue normal operation
                if (keySeq < neighbour->last_key_sequence) {
                    tr_warn("Dropping packet because key sequence smaller: %"PRIu32" < %"PRIu32,
                            keySeq, neighbour->last_key_sequence);
                    goto error_handler;
                }
                if (securityHeader.frameCounter <= neighbour->mle_frame_counter && keySeq == neighbour->last_key_sequence){
                    if (mle_service->mle_accept_invalid_frame_counter) {
                        securityHeader.invalid_frame_counter = true;
                    } else {
                        tr_warn("Dropping packet MLE frame counter is not higher:%"PRIu32" <= %"PRIu32,securityHeader.frameCounter,neighbour->mle_frame_counter);
                        goto error_handler;
                    }
                }
            }
        }
        start_decode:
        buf = mle_service_message_security_decode(buf, &securityHeader,key_ptr);
        if (!buf) {
            goto error_handler;
        }
        security_bypass = false;
        if (mle_service->mle_frame_counter_check_enabled && !securityHeader.invalid_frame_counter) {
            if (neighbour) {
                neighbour->mle_frame_counter = securityHeader.frameCounter;
                neighbour->last_key_sequence = keySeq;
            }
        }
    } else {
        if (sec_params->sec_level) {
            if (!service_handler->recv_security_bypass_cb) {
                tr_debug("Drop unsecured packet when security is enabled");
                goto error_handler;
            }
            security_bypass = true;
        } else {
            security_bypass = false;
        }
    }



    mle_msg.message_type = buffer_pull_uint8(buf);
    mle_msg.data_length = buffer_data_length(buf);
    mle_msg.data_ptr = buffer_data_pointer(buf);
    mle_msg.dst_pan_id = buf->link_specific.ieee802_15_4.dstPanId;
    mle_msg.src_pan_id = buf->link_specific.ieee802_15_4.srcPanId;
    if (mle_message_malformed_check(mle_msg.data_ptr, mle_msg.data_length) != 0) {
        tr_debug("Malfor");
        goto error_handler;
    }
    mle_msg.packet_src_address = buf->src_sa.address;
    mle_msg.packet_dst_address = buf->dst_sa.address;
    mle_msg.dbm = buf->options.dbm;
    mle_msg.lqi = buf->options.lqi;

#ifdef MLE_TEST
    if (receive_filter_cb) {
        if (!receive_filter_cb(service_handler->interface_id, &mle_msg, &securityHeader)) {
            goto error_handler;
        }
    }
#endif
    if (security_bypass) {
        /* Security by pass message handler call */
        service_handler->recv_security_bypass_cb(service_handler->interface_id, &mle_msg);
    } else {

        if (service_handler->recv_cb) {
            service_handler->recv_cb(service_handler->interface_id, &mle_msg, &securityHeader);
        }
    }

error_handler:
    if (buf) {
        buffer_free(buf);
    }
}

static bool mle_service_message_tokens_check(service_instance_t *cur_ptr, bool priority)
{
    if (!cur_ptr || cur_ptr->token_bucket_size == 0) {
        return true;
    }

    if (priority) {
        if (cur_ptr->token_bucket > MLE_TOKEN_BUFFER_MIN_NBR) {
            cur_ptr->token_bucket--;
        }
        return true;
    }

    if (cur_ptr->token_bucket > 0) {
        cur_ptr->token_bucket--;
        return true;
    } else {
        return false;
    }
}

static bool mle_service_instance_timeout_handler(uint16_t ticks, service_instance_t *cur_ptr)
{
    if (!cur_ptr || cur_ptr->token_bucket_size == 0 || cur_ptr->token_bucket_rate == 0) {
        return false;
        }

    cur_ptr->token_bucket_ticks += ticks;

    while (cur_ptr->token_bucket_ticks >= cur_ptr->token_bucket_rate) {
        cur_ptr->token_bucket_ticks -= cur_ptr->token_bucket_rate;
        cur_ptr->token_bucket += cur_ptr->token_bucket_count;
    }

    if (cur_ptr->token_bucket < cur_ptr->token_bucket_size) {
        return true;
    } else {
        cur_ptr->token_bucket = cur_ptr->token_bucket_size;
        return false;
    }
}

int mle_service_interface_register(int8_t interface_id, mle_service_receive_cb *receive_cb, uint8_t *mac64, uint8_t challengeLength)
{
    service_instance_t *srv_ptr;

    if (challengeLength < 4 || challengeLength > 32) {
        return -1;
    } else if (!receive_cb) {
        return -1;
    } else if (!mac64) {
        return -1;
    }

    if (!mle_service_allocate()) {
        tr_error("dhcp Sockets data base alloc fail");
        return -1;
    }

    if (mle_service_security_instance_allocate(interface_id) != 0) {
        return -1;
    }

    srv_ptr = mle_service_interface_get(interface_id);
    if (!srv_ptr) {
        goto error_handler;
    }

    srv_ptr->recv_cb = receive_cb;
    srv_ptr->challenge_length = challengeLength;
    memcpy(srv_ptr->mac64, mac64, 8);

    if (mle_service->mle_socket < 0) {
        if (socket_create(SOCKET_FAMILY_IPV6, SOCKET_TYPE_DGRAM, 0, &mle_service->mle_socket, MLE_ALLOCATED_PORT, mle_service_socket_callback, true) != eOK) {
            mle_service->mle_socket = -1;
        }
    }

    if (mle_service->mle_socket < 0 ) {
        tr_error("Sockets not available");
        goto error_handler;
    } else {
        const int16_t hops = 255;
        const uint32_t address_pref = SOCKET_IPV6_PREFER_SRC_6LOWPAN_LONG;
        const int8_t security = 0;
        const bool loop = false;
        socket_setsockopt(mle_service->mle_socket, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_UNICAST_HOPS, &hops, sizeof hops);
        socket_setsockopt(mle_service->mle_socket, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_MULTICAST_HOPS, &hops, sizeof hops);
        socket_setsockopt(mle_service->mle_socket, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_MULTICAST_LOOP, &loop, sizeof loop);
        socket_setsockopt(mle_service->mle_socket, SOCKET_IPPROTO_IPV6, SOCKET_IPV6_ADDR_PREFERENCES, &address_pref, sizeof address_pref);
        socket_setsockopt(mle_service->mle_socket, SOCKET_IPPROTO_IPV6, SOCKET_LINK_LAYER_SECURITY, &security, sizeof security);
        //Set service interface id
        socket_setsockopt(mle_service->mle_socket, SOCKET_IPPROTO_IPV6, SOCKET_INTERFACE_SELECT, &srv_ptr->interface_id, sizeof srv_ptr->interface_id);
    }

    return 0;

    error_handler:
    mle_service_interface_delete(interface_id);
    mle_service_security_instance_delete(interface_id);
    return -1;
}

bool mle_service_interface_registeration_validate(int8_t interface_id)
{
    if (mle_service_interface_find(interface_id) ) {
        return true;
    }

    return false;
}


int mle_service_interface_receiver_handler_update(int8_t interface_id, mle_service_receive_cb *receive_cb)
{
    service_instance_t *srv_ptr;

    srv_ptr = mle_service_interface_find(interface_id);
    if (!srv_ptr) {
        return -1;
    }

    srv_ptr->recv_cb = receive_cb;

    return 0;
}

int mle_service_interface_receiver_bypass_handler_update(int8_t interface_id, mle_service_receive_security_bypass_cb *receive_cb)
{
    service_instance_t *srv_ptr;

    srv_ptr = mle_service_interface_find(interface_id);
    if (!srv_ptr) {
        return -1;
    }

    srv_ptr->recv_security_bypass_cb = receive_cb;

    return 0;
}

void mle_service_interface_unregister(int8_t interface_id)
{
    //Remove service from list and free class
    mle_service_interface_delete(interface_id);
    //Remove Security Instance
    mle_service_security_instance_delete(interface_id);
    //Free Tx queue by unregistered interface id
    mle_service_buffer_clean_buffers_by_interface_id(interface_id);

    if (mle_service) {
        if (mle_service_interface_list_empty()) {
            tr_debug("Free Socket");
            socket_close(mle_service->mle_socket);
            mle_service->mle_socket = -1;
        }
    }
}

int mle_service_reset_frame_counters(int8_t interfaceId)
{
    mle_service_security_set_frame_counter(interfaceId, 0);
    mle_class_set_new_key_pending(interfaceId);
    return 0;
}

void mle_service_interface_tx_queue_clean(int8_t interface_id)
{
    mle_service_buffer_clean_buffers_by_interface_id(interface_id);
}

uint16_t mle_service_interface_tx_queue_size(int8_t interface_id)
{
    return mle_service_buffer_count_by_interface_id(interface_id);
}

int mle_service_interface_token_bucket_settings_set(int8_t interface_id, uint8_t size, uint8_t rate, uint8_t count)
{
    service_instance_t *srv_ptr;

    srv_ptr = mle_service_interface_find(interface_id);
    if (!srv_ptr) {
        return -1;
    }
    srv_ptr->token_bucket = size;
    srv_ptr->token_bucket_ticks = 0;
    srv_ptr->token_bucket_size = size;
    srv_ptr->token_bucket_rate = rate;
    srv_ptr->token_bucket_count = count;

    return 0;
}

uint16_t mle_service_msg_allocate(int8_t interface_id, uint16_t data_length, bool challengeAllocate, uint8_t type)
{
    uint16_t temporary_length = 64;
    mle_service_msg_buf_t *buf;
    //Verify that Interface is registered
    service_instance_t *srv_ptr = mle_service_interface_find(interface_id);
    if (!srv_ptr) {
        return 0;
    }

    mle_security_components_t * sec_params = mle_service_security_params_get(interface_id);
    if (!sec_params) {
        return 0;
    }

    if (data_length > 63) {
        temporary_length = data_length  + 1;
    }

    if (challengeAllocate) {
        temporary_length += (srv_ptr->challenge_length + 2 );
    }

    buf = mle_tr_create(temporary_length);
    if (!buf) {
        return 0;
    }
    //Set interface id to buffer
    buf->interfaceId = interface_id;

    mle_service_buffer_set_msg_type(buf->msg_id, type);

    if (challengeAllocate) {
        //SET Challenge all ways to front
        uint8_t *ptr = mle_msg_data_pointer(buf);
        *ptr++ = MLE_TYPE_CHALLENGE;
        *ptr++ = srv_ptr->challenge_length;
        buf->challengeLen = srv_ptr->challenge_length;
        buf->challengePtr = ptr;
        randLIB_get_n_bytes_random(ptr, srv_ptr->challenge_length);
        mle_msg_length_set(buf,(srv_ptr->challenge_length +2));
    } else {
        //NO retry by default
        buf->timeout_init = buf->timeout = buf->timeout_max = 0;
        buf->retrans_max = 0;
    }
    //Set default security level and key id mode
    //Key id, frame counter will be updated when default need to be change
    buf->security_header.securityLevel = sec_params->sec_level;
    buf->security_header.KeyIdMode = MAC_KEY_ID_MODE_IDX;

    return buf->msg_id;
}

uint8_t * mle_service_get_data_pointer(uint16_t msgId)
{
    return mle_service_buffer_get_data_pointer(msgId);
}

uint8_t * mle_service_get_payload_start_point(uint16_t msgId)
{
    mle_service_msg_buf_t *buf = mle_service_buffer_find(msgId);
    if (!buf) {
        return NULL;
    }
    uint8_t *ptr = buf->buf;

    return (ptr + 1);
}

uint16_t  mle_service_get_payload_length(uint16_t msgId)
{
    mle_service_msg_buf_t *buf = mle_service_buffer_find(msgId);
    if (!buf) {
        return 0;
    }

    if (buf->buf_end == 0) {
        return 0;
    }

    return (buf->buf_end -1);

}

int mle_service_update_length(uint16_t msgId, uint16_t tail_length)
{
    return mle_service_buffer_update_length(msgId, tail_length);
}

int mle_service_update_length_by_ptr(uint16_t msgId, uint8_t *data_end_ptr)
{
    return mle_service_buffer_update_length_by_ptr(msgId, data_end_ptr);
}

int mle_service_msg_update_security_params(uint16_t msgId, uint8_t security_level, uint8_t key_id_mode, uint32_t keysequence)
{
    mle_service_msg_buf_t *buf = mle_service_buffer_find(msgId);
    if (!buf) {
        return -1;
    }

    if (security_level > AES_SECURITY_LEVEL_ENC_MIC128 ) {
        return -2;
    } else if (security_level && key_id_mode != MAC_KEY_ID_MODE_IDX && key_id_mode != MAC_KEY_ID_MODE_SRC4_IDX) {
        return -2;
    }

    buf->security_header.securityLevel = security_level;
    buf->security_header.KeyIdMode = key_id_mode;
    common_write_32_bit(keysequence,buf->security_header.Keysource);
    return 0;
}

int mle_service_set_packet_callback(uint16_t msgId, mle_service_message_timeout_cb *cb)
{
    return mle_service_buffer_set_timeout_cb(msgId, cb);
}

int mle_service_set_msg_response_true(uint16_t msgId)
{
    return mle_service_buffer_set_response(msgId);
}

uint8_t * mle_service_get_msg_destination_address_pointer(uint16_t msgId)
{
    return mle_service_buffer_get_msg_destination_address_pointer(msgId);
}

int mle_service_set_msg_destination_address(uint16_t msgId, const uint8_t *dstAddress)
{
    if (!dstAddress) {
        return -2;
    }

    uint8_t *address_ptr = mle_service_buffer_get_msg_destination_address_pointer(msgId);

    if (!address_ptr) {
        return -1;
    }
    memcpy(address_ptr, dstAddress, 16);
    return 0;
}

int mle_service_set_msg_panid(uint16_t msgId, uint16_t panid)
{
    mle_service_msg_buf_t *buf = mle_service_buffer_find(msgId);

    if (!buf) {
        return -1;
    }
    buf->selected_pan_id = true;
    buf->packet_panid = panid;
    return 0;
}

int mle_service_msg_free(uint16_t msgId)
{
    return mle_service_buffer_free(mle_service_buffer_find(msgId));
}

bool mle_service_transaction_buffer_get_for_response(uint8_t *responseData, uint16_t length, uint16_t *msgId)
{
    mle_service_msg_buf_t *buffer = mle_service_buffer_find_for_response(responseData, length);
    if (!buffer) {
        return false;
    }

    *msgId = buffer->msg_id;
    return true;
}

bool mle_service_check_msg_sent(uint16_t msgId)
{
    mle_service_msg_buf_t *buf = mle_service_buffer_find(msgId);

    if (!buf) {
        return false;
    }

    return buf->message_sent;
}

int mle_service_message_tail_get(uint16_t msgId, uint16_t tail_length)
{
    return mle_service_buffer_tail_get(msgId,tail_length);
}

static int mle_service_timeout_fill(uint16_t msgId, mle_message_timeout_params_t *timeout_params, bool timeout_in_seconds)
{
    mle_service_msg_buf_t *buffer = mle_service_buffer_find(msgId);

    if (!buffer) {
        return -1;
    }

    buffer->timeout_max = timeout_params->timeout_max;
    buffer->retrans_max = timeout_params->retrans_max;
    buffer->delayed_response = timeout_params->delay;
    buffer->timeout_init = timeout_params->timeout_init;

    if (timeout_in_seconds) {
        buffer->timeout_max = buffer->timeout_max * 10;
        buffer->timeout_init = buffer->timeout_init * 10;
    }

    buffer->timeout_init = randLIB_randomise_base(buffer->timeout_init, MLE_RAND_LOW, MLE_RAND_HIGH);

    buffer->timeout = buffer->timeout_init;

    return 0;
}

int mle_service_set_msg_timeout_parameters(uint16_t msgId, mle_message_timeout_params_t *timeout_params)
{
    return mle_service_timeout_fill(msgId, timeout_params, true);
}

int mle_service_set_msg_timeout_parameters_fast(uint16_t msgId, mle_message_timeout_params_t *timeout_params)
{
    return mle_service_timeout_fill(msgId, timeout_params, false);
}

int mle_service_set_msg_token_bucket_priority(uint16_t msgId)
{
    mle_service_msg_buf_t *buffer = mle_service_buffer_find(msgId);

    if (!buffer) {
        return -1;
    }

    buffer->tokens_priority = true;
    return 0;
}

int mle_service_send_message(uint16_t msgId)
{
    mle_service_msg_buf_t *buffer = mle_service_buffer_find(msgId);

    if (!buffer) {
        return -1;
    }

    service_instance_t *srv_ptr = mle_service_interface_find(buffer->interfaceId);
    mle_security_components_t *sec_params = mle_service_security_params_get(buffer->interfaceId);

    if (!srv_ptr || !sec_params) {
        mle_service_buffer_free(buffer);
        return -1;
    }

    //Trig timer
    mle_service_timer_start();

    if (buffer->delayed_response) {
        //set random jitter for response
        buffer->timeout = randLIB_get_random_in_range(1, buffer->delayed_response);
        return 0;
    }

    // Check if message needs to be delayed or removed because lack of message tokens
    if (!mle_service_message_tokens_check(srv_ptr, buffer->tokens_priority)) {
        if (mle_service_buffer_tokens_delay_count() <= MLE_TOKEN_BUFFER_MAX_NBR) {
            buffer->tokens_delay = true;
            // Give time to wait free tokens
            if (buffer->timeout < MLE_TOKEN_DELAY) {
                buffer->timeout = MLE_TOKEN_DELAY;
            }
        } else {
            mle_service_buffer_free(mle_service_buffer_find(buffer->msg_id));
            tr_debug("MLE tokens message freed");
        }
        return 0;
    }

    if (mle_service_build_packet_send(srv_ptr,sec_params, buffer) != 0 ) {
        tr_warn("MLE message sending failed, ipv6=%s", trace_ipv6(buffer->dst_address));
        buffer->delayed_response = 1;
        buffer->timeout = 1;
        buffer->retrans_max++;
    } else {
        if (buffer->retrans_max == 0 && !buffer->timeout) {
            //Free msg
            mle_service_buffer_free(mle_service_buffer_find(buffer->msg_id));
        }
    }

    return 0;
}

int mle_service_security_init(int8_t interfaceId, uint8_t security_level, uint32_t security_frame_counter, mle_service_key_request_by_counter_cb * key_counter_req, mle_service_security_notify_cb *notify)
{
    mle_security_components_t *srv_ptr = mle_service_security_params_get(interfaceId);
    if (!srv_ptr) {
        return -1;
    }

    if (security_level > AES_SECURITY_LEVEL_ENC_MIC128) {
        return -2;
    }

    mle_service_security_parameters_init(srv_ptr);
    srv_ptr->sec_level = security_level;
    srv_ptr->key_req = key_counter_req;
    srv_ptr->security_notify = notify;
    srv_ptr->security_frame_counter = security_frame_counter;
    return 0;
}


bool mle_service_security_set_frame_counter(int8_t interfaceId, uint32_t frame_counter)
{
    mle_security_components_t *sec_ptr = mle_service_security_params_get(interfaceId);
    if (!sec_ptr) {
        return false;
    }

    sec_ptr->security_frame_counter = frame_counter;

    return true;
}

uint32_t mle_service_security_get_frame_counter(int8_t interfaceId)
{
    mle_security_components_t *sec_ptr = mle_service_security_params_get(interfaceId);
    if (!sec_ptr) {
        return 0;
    }

    return sec_ptr->security_frame_counter;
}

uint8_t * mle_service_security_default_key_get(int8_t interfaceId)
{
    mle_security_components_t *sec_ptr = mle_service_security_params_get(interfaceId);
    if (!sec_ptr) {
        return NULL;
    }

    return mle_service_security_get_default_key(sec_ptr);
}

uint8_t  mle_service_security_default_key_id_get(int8_t interfaceId)
{
    mle_security_components_t *sec_ptr = mle_service_security_params_get(interfaceId);
    if (!sec_ptr) {
        return 0;
    }

    return mle_service_security_get_default_key_id(sec_ptr);
}

uint8_t * mle_service_security_next_key_get(int8_t interfaceId)
{
    mle_security_components_t *sec_ptr = mle_service_security_params_get(interfaceId);
    if (!sec_ptr) {
        return NULL;
    }

    return mle_service_security_get_next_key(sec_ptr);
}

uint8_t  mle_service_security_next_key_id_get(int8_t interfaceId)
{
    mle_security_components_t *sec_ptr = mle_service_security_params_get(interfaceId);
    if (!sec_ptr) {
        return 0;
    }

    return mle_service_security_get_next_key_id(sec_ptr);
}

uint8_t  mle_service_security_level_get(int8_t interfaceId)
{
    mle_security_components_t *sec_ptr = mle_service_security_params_get(interfaceId);
    if (!sec_ptr) {
        return 0;
    }

    return sec_ptr->sec_level;
}

bool mle_service_security_key_trig(int8_t interfaceId, uint8_t keyId)
{
    return mle_service_security_key_update_trig(interfaceId, mle_service_security_params_get(interfaceId), keyId);

}

bool mle_service_security_set_security_key(int8_t interfaceId, const uint8_t *security_key, uint8_t keyId, bool primary)
{
    bool master_key_changed = false;
    master_key_changed = mle_service_security_key_set(mle_service_security_params_get(interfaceId), security_key, keyId, primary);
    if (master_key_changed && primary) {
        mle_service_reset_frame_counters(interfaceId);
    }
    return master_key_changed;
}

int mle_service_reject_message_build(int8_t interfaceId, uint8_t *dstIpv6, bool force_unsecure)
{
    uint16_t buf_id = mle_service_msg_allocate(interfaceId, 32, false, MLE_COMMAND_REJECT);
    if (buf_id == 0) {
        return -1;
    }
    tr_debug("MLE Reject MSG Build");

    if (force_unsecure) {
        mle_service_msg_update_security_params(buf_id, 0, 0, 0);
    }

    //SET Destination address
    mle_service_set_msg_destination_address(buf_id, dstIpv6);

    //Set message priority
    mle_service_set_msg_token_bucket_priority(buf_id);

    mle_service_send_message(buf_id);

    return 0;
}

static uint8_t *mle_security_aux_header_write(uint8_t *ptr, const mle_security_header_t *auxHeader)
{
    uint8_t auxBaseHeader;
    auxBaseHeader = auxHeader->securityLevel;
    auxBaseHeader |= (auxHeader->KeyIdMode << 3);
    *ptr++ = auxBaseHeader;
    ptr = common_write_32_bit_inverse(auxHeader->frameCounter, ptr);

    switch (auxHeader->KeyIdMode) {
        case MAC_KEY_ID_MODE_SRC8_IDX:
            memcpy(ptr, auxHeader->Keysource, 8);
            ptr += 8;
            *ptr++ = auxHeader->KeyIndex;
            break;
        case MAC_KEY_ID_MODE_SRC4_IDX:
            memcpy(ptr, auxHeader->Keysource, 4);
            ptr += 4;
            *ptr++ = auxHeader->KeyIndex;
            break;
        case MAC_KEY_ID_MODE_IDX:
            *ptr++ = auxHeader->KeyIndex;
            break;
        case MAC_KEY_ID_MODE_IMPLICIT:
        default:
            break;
    }
    return ptr;
}

static void mle_security_aux_ccm_nonce_set(uint8_t *noncePtr, uint8_t *mac64, uint32_t securityFrameCounter, uint8_t securityLevel)
{
    memcpy(noncePtr, mac64, 8);
    noncePtr += 8;
    noncePtr = common_write_32_bit(securityFrameCounter, noncePtr);
    *noncePtr = securityLevel;
}

static uint8_t mle_security_aux_header_size(uint8_t keyIdMode)
{
    uint8_t auxHeaderLength;
    if (keyIdMode == 0) {
        auxHeaderLength = 5; //No Key ID selected
    } else if (keyIdMode == 1) {
        auxHeaderLength = 6; //key ID without Source
    } else if (keyIdMode == 2) {
        auxHeaderLength = 10; //key ID with 32-bitSource
    } else {
        auxHeaderLength = 14; //key ID with 64-bitSource
    }

    return auxHeaderLength;
}

void mle_service_set_frame_counter_check(bool value)
{
    if (mle_service) {
        mle_service->mle_frame_counter_check_enabled = value;
    }
}

void mle_service_set_fragmented_msg_ll_security(bool value)
{
    if (mle_service) {
        mle_service->mle_frag_msg_security_enabled = value;
    }
}

int mle_service_set_msg_rf_channel(uint16_t msgId, uint8_t channel)
{
    return mle_service_buffer_set_msg_rf_channel(msgId, channel);
}

int mle_service_set_msg_link_layer_security_mode(uint16_t msgId, bool use_key_id_mode_2)
{
    return mle_service_buffer_set_msg_security_mode(msgId, use_key_id_mode_2);
}

void mle_service_set_accept_invalid_frame_counter(bool value)
{
    if (mle_service) {
        mle_service->mle_accept_invalid_frame_counter = value;
    }
}

#ifdef MLE_TEST
void mle_service_receive_filter_cb_set(mle_service_filter_cb *filter_cb)
{
    receive_filter_cb = filter_cb;
}

#endif
