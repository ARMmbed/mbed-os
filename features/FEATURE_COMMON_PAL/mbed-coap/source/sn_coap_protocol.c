/*
 * Copyright (c) 2011-2015 ARM Limited. All rights reserved.
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

/**
 * \file sn_coap_protocol.c
 *
 * \brief CoAP Protocol implementation
 *
 * Functionality: CoAP Protocol
 *
 */


/* * * * * * * * * * * * * * */
/* * * * INCLUDE FILES * * * */
/* * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h> /* For libary malloc() */
#include <string.h> /* For memset() and memcpy() */
#if defined __linux__ || defined TARGET_LIKE_MBED
#include <time.h>
#endif

#include "ns_types.h"
#include "mbed-coap/sn_coap_protocol.h"
#include "sn_coap_header_internal.h"
#include "sn_coap_protocol_internal.h"
#include "randLIB.h"
#include "mbed-trace/mbed_trace.h"

#define TRACE_GROUP "coap"
/* * * * * * * * * * * * * * * * * * * * */
/* * * * LOCAL FUNCTION PROTOTYPES * * * */
/* * * * * * * * * * * * * * * * * * * * */

static void                  sn_coap_protocol_send_rst(struct coap_s *handle, uint16_t msg_id, sn_nsdl_addr_s *addr_ptr, void *param);
#if SN_COAP_DUPLICATION_MAX_MSGS_COUNT/* If Message duplication detection is not used at all, this part of code will not be compiled */
static void                  sn_coap_protocol_linked_list_duplication_info_store(struct coap_s *handle, sn_nsdl_addr_s *src_addr_ptr, uint16_t msg_id, void *param);
static coap_duplication_info_s *sn_coap_protocol_linked_list_duplication_info_search(struct coap_s *handle, sn_nsdl_addr_s *scr_addr_ptr, uint16_t msg_id);
static void                  sn_coap_protocol_linked_list_duplication_info_remove(struct coap_s *handle, uint8_t *scr_addr_ptr, uint16_t port, uint16_t msg_id);
static void                  sn_coap_protocol_linked_list_duplication_info_remove_old_ones(struct coap_s *handle);
#endif
#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE /* If Message blockwising is not used at all, this part of code will not be compiled */
static void                  sn_coap_protocol_linked_list_blockwise_msg_remove(struct coap_s *handle, coap_blockwise_msg_s *removed_msg_ptr);
static void                  sn_coap_protocol_linked_list_blockwise_payload_store(struct coap_s *handle, sn_nsdl_addr_s *addr_ptr, uint16_t stored_payload_len, uint8_t *stored_payload_ptr, uint32_t block_number);
static uint8_t              *sn_coap_protocol_linked_list_blockwise_payload_search(struct coap_s *handle, sn_nsdl_addr_s *src_addr_ptr, uint16_t *payload_length);
static bool                  sn_coap_protocol_linked_list_blockwise_payload_compare_block_number(struct coap_s *handle, sn_nsdl_addr_s *src_addr_ptr, uint32_t block_number);
static void                  sn_coap_protocol_linked_list_blockwise_payload_remove(struct coap_s *handle, coap_blockwise_payload_s *removed_payload_ptr);
static void                  sn_coap_protocol_linked_list_blockwise_payload_remove_oldest(struct coap_s *handle);
static uint32_t              sn_coap_protocol_linked_list_blockwise_payloads_get_len(struct coap_s *handle, sn_nsdl_addr_s *src_addr_ptr);
static void                  sn_coap_protocol_linked_list_blockwise_remove_old_data(struct coap_s *handle);
static sn_coap_hdr_s        *sn_coap_handle_blockwise_message(struct coap_s *handle, sn_nsdl_addr_s *src_addr_ptr, sn_coap_hdr_s *received_coap_msg_ptr, void *param);
static int8_t                sn_coap_convert_block_size(uint16_t block_size);
static sn_coap_hdr_s        *sn_coap_protocol_copy_header(struct coap_s *handle, sn_coap_hdr_s *source_header_ptr);
#endif
#if ENABLE_RESENDINGS
static uint8_t               sn_coap_protocol_linked_list_send_msg_store(struct coap_s *handle, sn_nsdl_addr_s *dst_addr_ptr, uint16_t send_packet_data_len, uint8_t *send_packet_data_ptr, uint32_t sending_time, void *param);
static sn_nsdl_transmit_s   *sn_coap_protocol_linked_list_send_msg_search(struct coap_s *handle,sn_nsdl_addr_s *src_addr_ptr, uint16_t msg_id);
static void                  sn_coap_protocol_linked_list_send_msg_remove(struct coap_s *handle, sn_nsdl_addr_s *src_addr_ptr, uint16_t msg_id);
static coap_send_msg_s      *sn_coap_protocol_allocate_mem_for_msg(struct coap_s *handle, sn_nsdl_addr_s *dst_addr_ptr, uint16_t packet_data_len);
static void                  sn_coap_protocol_release_allocated_send_msg_mem(struct coap_s *handle, coap_send_msg_s *freed_send_msg_ptr);
static uint16_t              sn_coap_count_linked_list_size(const coap_send_msg_list_t *linked_list_ptr);
static uint32_t              sn_coap_calculate_new_resend_time(const uint32_t current_time, const uint8_t interval, const uint8_t counter);
#endif

/* * * * * * * * * * * * * * * * * */
/* * * * GLOBAL DECLARATIONS * * * */
/* * * * * * * * * * * * * * * * * */
static uint16_t message_id;

int8_t sn_coap_protocol_destroy(struct coap_s *handle)
{
    if (handle == NULL) {
        return -1;
    }
#if ENABLE_RESENDINGS /* If Message resending is not used at all, this part of code will not be compiled */

    sn_coap_protocol_clear_retransmission_buffer(handle);

#endif

#if SN_COAP_DUPLICATION_MAX_MSGS_COUNT /* If Message duplication detection is not used at all, this part of code will not be compiled */
    ns_list_foreach_safe(coap_duplication_info_s, tmp, &handle->linked_list_duplication_msgs) {
        if (tmp->coap == handle) {
            if (tmp->address) {
                if (tmp->address->addr_ptr) {
                    handle->sn_coap_protocol_free(tmp->address->addr_ptr);
                    tmp->address->addr_ptr = 0;
                }
                handle->sn_coap_protocol_free(tmp->address);
                tmp->address = 0;
            }
            if (tmp->packet_ptr) {
                handle->sn_coap_protocol_free(tmp->packet_ptr);
                tmp->packet_ptr = 0;
            }
            ns_list_remove(&handle->linked_list_duplication_msgs, tmp);
            handle->count_duplication_msgs--;
            handle->sn_coap_protocol_free(tmp);
            tmp = 0;
        }
    }

#endif


#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE /* If Message blockwise is not used at all, this part of code will not be compiled */
    ns_list_foreach_safe(coap_blockwise_msg_s, tmp, &handle->linked_list_blockwise_sent_msgs) {
        if (tmp->coap == handle) {
            if (tmp->coap_msg_ptr) {
                if (tmp->coap_msg_ptr->payload_ptr) {
                    handle->sn_coap_protocol_free(tmp->coap_msg_ptr->payload_ptr);
                    tmp->coap_msg_ptr->payload_ptr = 0;
                }
                sn_coap_parser_release_allocated_coap_msg_mem(tmp->coap, tmp->coap_msg_ptr);
            }
            ns_list_remove(&handle->linked_list_blockwise_sent_msgs, tmp);
            handle->sn_coap_protocol_free(tmp);
            tmp = 0;
        }
    }
    ns_list_foreach_safe(coap_blockwise_payload_s, tmp, &handle->linked_list_blockwise_received_payloads) {
        if (tmp->coap == handle) {
            if (tmp->addr_ptr) {
                handle->sn_coap_protocol_free(tmp->addr_ptr);
                tmp->addr_ptr = 0;
            }
            if (tmp->payload_ptr) {
                handle->sn_coap_protocol_free(tmp->payload_ptr);
                tmp->payload_ptr = 0;
            }
            ns_list_remove(&handle->linked_list_blockwise_received_payloads, tmp);
            handle->sn_coap_protocol_free(tmp);
            tmp = 0;
        }
    }
#endif

    handle->sn_coap_protocol_free(handle);
    handle = 0;
    return 0;
}

struct coap_s *sn_coap_protocol_init(void *(*used_malloc_func_ptr)(uint16_t), void (*used_free_func_ptr)(void *),
                                     uint8_t (*used_tx_callback_ptr)(uint8_t *, uint16_t, sn_nsdl_addr_s *, void *),
                                     int8_t (*used_rx_callback_ptr)(sn_coap_hdr_s *, sn_nsdl_addr_s *, void *param))
{
    /* Check paramters */
    if ((used_malloc_func_ptr == NULL) || (used_free_func_ptr == NULL) || (used_tx_callback_ptr == NULL)) {
        return NULL;
    }

    struct coap_s *handle;
    handle = used_malloc_func_ptr(sizeof(struct coap_s));
    if (handle == NULL) {
        return NULL;
    }

    memset(handle, 0, sizeof(struct coap_s));

    /* * * Handle tx callback * * */
    handle->sn_coap_tx_callback = used_tx_callback_ptr;

    handle->sn_coap_protocol_free = used_free_func_ptr;
    handle->sn_coap_protocol_malloc = used_malloc_func_ptr;

    /* * * Handle rx callback * * */
    /* If pointer = 0, then re-sending does not return error when failed */
    handle->sn_coap_rx_callback = used_rx_callback_ptr;



#if ENABLE_RESENDINGS  /* If Message resending is not used at all, this part of code will not be compiled */

    /* * * * Create Linked list for storing active resending messages  * * * */
    ns_list_init(&handle->linked_list_resent_msgs);
    handle->sn_coap_resending_queue_msgs = SN_COAP_RESENDING_QUEUE_SIZE_MSGS;
    handle->sn_coap_resending_queue_bytes = SN_COAP_RESENDING_QUEUE_SIZE_BYTES;
    handle->sn_coap_resending_intervall = DEFAULT_RESPONSE_TIMEOUT;
    handle->sn_coap_resending_count = SN_COAP_RESENDING_MAX_COUNT;


#endif /* ENABLE_RESENDINGS */

#if SN_COAP_DUPLICATION_MAX_MSGS_COUNT /* If Message duplication detection is not used at all, this part of code will not be compiled */
    /* * * * Create Linked list for storing Duplication info * * * */
    ns_list_init(&handle->linked_list_duplication_msgs);
    handle->sn_coap_duplication_buffer_size = SN_COAP_DUPLICATION_MAX_MSGS_COUNT;
#endif

#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE /* If Message blockwising is not used at all, this part of code will not be compiled */

    ns_list_init(&handle->linked_list_blockwise_sent_msgs);
    ns_list_init(&handle->linked_list_blockwise_received_payloads);
    handle->sn_coap_block_data_size = SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE;

#endif /* ENABLE_RESENDINGS */

    /* Randomize global message ID */
    randLIB_seed_random();
    message_id = randLIB_get_16bit();
    if (message_id == 0) {
        message_id = 1;
    }
    tr_debug("Coap random msg ID: %d", message_id);
    tr_debug("Coap BLOCKWISE_MAX_TIME_DATA_STORED: %d", SN_COAP_BLOCKWISE_MAX_TIME_DATA_STORED);

    return handle;
}

int8_t sn_coap_protocol_set_block_size(struct coap_s *handle, uint16_t block_size)
{
    (void) handle;
    (void) block_size;
#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
    if (handle == NULL) {
        return -1;
    }
    switch (block_size) {
        case 0:
        case 16:
        case 32:
        case 64:
        case 128:
        case 256:
        case 512:
        case 1024:
            handle->sn_coap_block_data_size = block_size;
            return 0;
        default:
            break;
    }
#endif
    return -1;

}

int8_t sn_coap_protocol_set_duplicate_buffer_size(struct coap_s *handle, uint8_t message_count)
{
    (void) handle;
    (void) message_count;
#if SN_COAP_DUPLICATION_MAX_MSGS_COUNT
    if (handle == NULL) {
        return -1;
    }
    if (message_count <= SN_COAP_MAX_ALLOWED_DUPLICATION_MESSAGE_COUNT) {
        handle->sn_coap_duplication_buffer_size = message_count;
        return 0;
    }
#endif
    return -1;
}

int8_t sn_coap_protocol_set_retransmission_parameters(struct coap_s *handle,
        uint8_t resending_count, uint8_t resending_intervall)
{
#if ENABLE_RESENDINGS
    if (handle == NULL) {
        return -1;
    }
    if (resending_count <= SN_COAP_MAX_ALLOWED_RESENDING_COUNT &&
            resending_intervall <= SN_COAP_MAX_ALLOWED_RESPONSE_TIMEOUT) {
        handle->sn_coap_resending_count = resending_count;

        if (resending_intervall == 0) {
            handle->sn_coap_resending_intervall = 1;
        } else {
            handle->sn_coap_resending_intervall = resending_intervall;
        }
        return 0;
    }
#endif
    return -1;
}

int8_t sn_coap_protocol_set_retransmission_buffer(struct coap_s *handle,
        uint8_t buffer_size_messages, uint16_t buffer_size_bytes)
{
#if ENABLE_RESENDINGS
    if (handle == NULL) {
        return -1;
    }
    if (buffer_size_bytes <= SN_COAP_MAX_ALLOWED_RESENDING_BUFF_SIZE_BYTES &&
        buffer_size_messages <= SN_COAP_MAX_ALLOWED_RESENDING_BUFF_SIZE_MSGS ) {
        handle->sn_coap_resending_queue_bytes = buffer_size_bytes;
        handle->sn_coap_resending_queue_msgs = buffer_size_messages;
        return 0;
    }

#endif
    return -1;

}

void sn_coap_protocol_clear_retransmission_buffer(struct coap_s *handle)
{
#if ENABLE_RESENDINGS /* If Message resending is not used at all, this part of code will not be compiled */
    if (handle == NULL) {
        return;
    }
    ns_list_foreach_safe(coap_send_msg_s, tmp, &handle->linked_list_resent_msgs) {
        ns_list_remove(&handle->linked_list_resent_msgs, tmp);
        sn_coap_protocol_release_allocated_send_msg_mem(handle, tmp);
        --handle->count_resent_msgs;
    }
#endif
}

int8_t sn_coap_protocol_delete_retransmission(struct coap_s *handle, uint16_t msg_id)
{
#if ENABLE_RESENDINGS /* If Message resending is not used at all, this part of code will not be compiled */
    if (handle == NULL) {
        return -1;
    }
    ns_list_foreach_safe(coap_send_msg_s, tmp, &handle->linked_list_resent_msgs) {
        if (tmp->send_msg_ptr && tmp->send_msg_ptr->packet_ptr ) {
            uint16_t temp_msg_id = (tmp->send_msg_ptr->packet_ptr[2] << 8);
            temp_msg_id += (uint16_t)tmp->send_msg_ptr->packet_ptr[3];
            if(temp_msg_id == msg_id){
                ns_list_remove(&handle->linked_list_resent_msgs, tmp);
                --handle->count_resent_msgs;
                sn_coap_protocol_release_allocated_send_msg_mem(handle, tmp);
                return 0;
            }
        }
    }
#endif
    return -2;
}

#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE /* If Message blockwising is not used at all, this part of code will not be compiled */
int8_t prepare_blockwise_message(struct coap_s *handle, sn_coap_hdr_s *src_coap_msg_ptr)
{
    if ((src_coap_msg_ptr->payload_len > handle->sn_coap_block_data_size) && (handle->sn_coap_block_data_size > 0)) {
        /* * * * Add Blockwise option to send CoAP message * * */

        /* Allocate memory for less used options */
        if (sn_coap_parser_alloc_options(handle, src_coap_msg_ptr) == NULL) {
            return -2;
        }

        /* Check if Request message */
        if (src_coap_msg_ptr->msg_code < COAP_MSG_CODE_RESPONSE_CREATED) {
            tr_debug("prepare_blockwise_message - block1 request");
            /* Add Blockwise option, use Block1 because Request payload */
            src_coap_msg_ptr->options_list_ptr->block1 = 0x08;      /* First block  (BLOCK NUMBER, 4 MSB bits) + More to come (MORE, 1 bit) */
            src_coap_msg_ptr->options_list_ptr->block1 |= sn_coap_convert_block_size(handle->sn_coap_block_data_size);

            /* Add size1 parameter */
            tr_debug("prepare_blockwise_message block1 request - payload len %d", src_coap_msg_ptr->payload_len);

            src_coap_msg_ptr->options_list_ptr->use_size1 = true;
            src_coap_msg_ptr->options_list_ptr->use_size2 = false;
            src_coap_msg_ptr->options_list_ptr->size1 = src_coap_msg_ptr->payload_len;
        } else { /* Response message */
            tr_debug("prepare_blockwise_message - block2 response");
            /* Add Blockwise option, use Block2 because Response payload */
            src_coap_msg_ptr->options_list_ptr->block2 = 0x08;      /* First block  (BLOCK NUMBER, 4 MSB bits) + More to come (MORE, 1 bit) */
            src_coap_msg_ptr->options_list_ptr->block2 |= sn_coap_convert_block_size(handle->sn_coap_block_data_size);

            src_coap_msg_ptr->options_list_ptr->use_size1 = false;
            src_coap_msg_ptr->options_list_ptr->use_size2 = true;
            src_coap_msg_ptr->options_list_ptr->size2 = src_coap_msg_ptr->payload_len;
        }
    }
    return 0;
}
#endif

int16_t sn_coap_protocol_build(struct coap_s *handle, sn_nsdl_addr_s *dst_addr_ptr,
                               uint8_t *dst_packet_data_ptr, sn_coap_hdr_s *src_coap_msg_ptr, void *param)
{
    tr_debug("sn_coap_protocol_build - payload len %d", src_coap_msg_ptr->payload_len);
    int16_t  byte_count_built     = 0;
#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE /* If Message blockwising is not used at all, this part of code will not be compiled */
    uint16_t original_payload_len = 0;
#endif
    /* * * * Check given pointers  * * * */
    if ((dst_addr_ptr == NULL) || (dst_packet_data_ptr == NULL) || (src_coap_msg_ptr == NULL) || handle == NULL) {
        return -2;
    }

    if (dst_addr_ptr->addr_ptr == NULL) {
        return -2;
    }

    /* Check if built Message type is else than Acknowledgement or Reset i.e. message type is Confirmable or Non-confirmable */
    /* (for Acknowledgement and  Reset messages is written same Message ID than was in the Request message) */
    if (src_coap_msg_ptr->msg_type != COAP_MSG_TYPE_ACKNOWLEDGEMENT &&
            src_coap_msg_ptr->msg_type != COAP_MSG_TYPE_RESET &&
            src_coap_msg_ptr->msg_id == 0) {
        /* * * * Generate new Message ID and increase it by one  * * * */
        src_coap_msg_ptr->msg_id = message_id;
        message_id++;
        if (message_id == 0) {
            message_id = 1;
        }
    }

#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE /* If Message blockwising is not used at all, this part of code will not be compiled */

    /* If blockwising needed */
    if ((src_coap_msg_ptr->payload_len > handle->sn_coap_block_data_size) && (handle->sn_coap_block_data_size > 0)) {
        /* Store original Payload length */
        original_payload_len = src_coap_msg_ptr->payload_len;
        /* Change Payload length of send message because Payload is blockwised */
        src_coap_msg_ptr->payload_len = handle->sn_coap_block_data_size;
    }

#endif
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    /* * * * Build Packet data from CoAP message by using CoAP Header builder  * * * */
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    byte_count_built = sn_coap_builder_2(dst_packet_data_ptr, src_coap_msg_ptr, handle->sn_coap_block_data_size);

    if (byte_count_built < 0) {
        return byte_count_built;
    }

#if ENABLE_RESENDINGS /* If Message resending is not used at all, this part of code will not be compiled */

    /* Check if built Message type was confirmable, only these messages are resent */
    if (src_coap_msg_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
        /* Store message to Linked list for resending purposes */
        uint32_t resend_time = sn_coap_calculate_new_resend_time(handle->system_time, handle->sn_coap_resending_intervall, 0);
        if (sn_coap_protocol_linked_list_send_msg_store(handle, dst_addr_ptr, byte_count_built, dst_packet_data_ptr,
                resend_time,
                param) == 0) {
            return -4;
        }
    }

#endif /* ENABLE_RESENDINGS */

#if SN_COAP_DUPLICATION_MAX_MSGS_COUNT
    if (src_coap_msg_ptr->msg_type == COAP_MSG_TYPE_ACKNOWLEDGEMENT &&
            handle->sn_coap_duplication_buffer_size != 0) {
        coap_duplication_info_s* info = sn_coap_protocol_linked_list_duplication_info_search(handle,
                                                                                             dst_addr_ptr,
                                                                                             src_coap_msg_ptr->msg_id);
        /* Update package data to duplication info struct if it's not there yet */
        if (info && info->packet_ptr == NULL) {
            info->packet_ptr = handle->sn_coap_protocol_malloc(byte_count_built);
            if (info->packet_ptr) {
                memcpy(info->packet_ptr, dst_packet_data_ptr, byte_count_built);
                info->packet_len = byte_count_built;
            } else {
                return -4;
            }
        }
    }
#endif

#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE /* If Message blockwising is not used at all, this part of code will not be compiled */

    /* If blockwising needed */
    if ((original_payload_len > handle->sn_coap_block_data_size) && (handle->sn_coap_block_data_size > 0)) {

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
        /* * * * Manage rest blockwise messages sending by storing them to Linked list * * * */
        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        coap_blockwise_msg_s *stored_blockwise_msg_ptr;

        stored_blockwise_msg_ptr = handle->sn_coap_protocol_malloc(sizeof(coap_blockwise_msg_s));
        if (!stored_blockwise_msg_ptr) {
            //block paylaod save failed, only first block can be build. Perhaps we should return error.
            return byte_count_built;
        }
        memset(stored_blockwise_msg_ptr, 0, sizeof(coap_blockwise_msg_s));

        /* Fill struct */
        stored_blockwise_msg_ptr->timestamp = handle->system_time;

        stored_blockwise_msg_ptr->coap_msg_ptr = sn_coap_protocol_copy_header(handle, src_coap_msg_ptr);
        if( stored_blockwise_msg_ptr->coap_msg_ptr == NULL ){
            handle->sn_coap_protocol_free(stored_blockwise_msg_ptr);
            stored_blockwise_msg_ptr = 0;
            return -2;
        }

        stored_blockwise_msg_ptr->coap_msg_ptr->payload_len = original_payload_len;
        stored_blockwise_msg_ptr->coap_msg_ptr->payload_ptr = handle->sn_coap_protocol_malloc(stored_blockwise_msg_ptr->coap_msg_ptr->payload_len);

        if (!stored_blockwise_msg_ptr->coap_msg_ptr->payload_ptr) {
            //block payload save failed, only first block can be build. Perhaps we should return error.
            sn_coap_parser_release_allocated_coap_msg_mem(handle, stored_blockwise_msg_ptr->coap_msg_ptr);
            handle->sn_coap_protocol_free(stored_blockwise_msg_ptr);
            stored_blockwise_msg_ptr = 0;
            return byte_count_built;
        }
        memcpy(stored_blockwise_msg_ptr->coap_msg_ptr->payload_ptr, src_coap_msg_ptr->payload_ptr, stored_blockwise_msg_ptr->coap_msg_ptr->payload_len);

        stored_blockwise_msg_ptr->coap = handle;
        ns_list_add_to_end(&handle->linked_list_blockwise_sent_msgs, stored_blockwise_msg_ptr);
    }

    else if (src_coap_msg_ptr->msg_code == COAP_MSG_CODE_REQUEST_GET) {
        /* Add message to linked list - response can be in blocks and we need header to build response.. */
        coap_blockwise_msg_s *stored_blockwise_msg_ptr;

        stored_blockwise_msg_ptr = handle->sn_coap_protocol_malloc(sizeof(coap_blockwise_msg_s));
        if (!stored_blockwise_msg_ptr) {
            return byte_count_built;
        }
        memset(stored_blockwise_msg_ptr, 0, sizeof(coap_blockwise_msg_s));

        /* Fill struct */
        stored_blockwise_msg_ptr->timestamp = handle->system_time;

        stored_blockwise_msg_ptr->coap_msg_ptr = sn_coap_protocol_copy_header(handle, src_coap_msg_ptr);
        if( stored_blockwise_msg_ptr->coap_msg_ptr == NULL ){
            handle->sn_coap_protocol_free(stored_blockwise_msg_ptr);
            stored_blockwise_msg_ptr = 0;
            return -2;
        }

        stored_blockwise_msg_ptr->coap = handle;
        ns_list_add_to_end(&handle->linked_list_blockwise_sent_msgs, stored_blockwise_msg_ptr);
    }

#endif /* SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE */

    tr_debug("sn_coap_protocol_build - msg id: [%d], bytes: [%d]", src_coap_msg_ptr->msg_id, byte_count_built);

    /* * * * Return built CoAP message Packet data length  * * * */
    return byte_count_built;
}

sn_coap_hdr_s *sn_coap_protocol_parse(struct coap_s *handle, sn_nsdl_addr_s *src_addr_ptr, uint16_t packet_data_len, uint8_t *packet_data_ptr, void *param)
{
    tr_debug("sn_coap_protocol_parse");
    sn_coap_hdr_s   *returned_dst_coap_msg_ptr = NULL;
    coap_version_e   coap_version              = COAP_VERSION_UNKNOWN;

    /* * * * Check given pointer * * * */
    if (src_addr_ptr == NULL || src_addr_ptr->addr_ptr == NULL ||
        packet_data_ptr == NULL || handle == NULL) {
        return NULL;
    }

    /* * * * Parse Packet data to CoAP message by using CoAP Header parser * * * */
    returned_dst_coap_msg_ptr = sn_coap_parser(handle, packet_data_len, packet_data_ptr, &coap_version);

    /* Check status of returned pointer */
    if (returned_dst_coap_msg_ptr == NULL) {
        /* Memory allocation error in parser */
        return NULL;
    }
    /* * * * Send bad request response if parsing fails * * * */
    if (returned_dst_coap_msg_ptr->coap_status == COAP_STATUS_PARSER_ERROR_IN_HEADER) {
        sn_coap_protocol_send_rst(handle, returned_dst_coap_msg_ptr->msg_id, src_addr_ptr, param);
        sn_coap_parser_release_allocated_coap_msg_mem(handle, returned_dst_coap_msg_ptr);
        return NULL;
    }

    /* * * * Check validity of parsed Header values  * * * */
    if (sn_coap_header_validity_check(returned_dst_coap_msg_ptr, coap_version) != 0) {
        /* If message code is in a reserved class (1, 6 or 7), send reset. Message code class is 3 MSB of the message code byte     */
        if (((returned_dst_coap_msg_ptr->msg_code >> 5) == 1) ||        // if class == 1
                ((returned_dst_coap_msg_ptr->msg_code >> 5) == 6) ||    // if class == 6
                ((returned_dst_coap_msg_ptr->msg_code >> 5) == 7)) {    // if class == 7
            sn_coap_protocol_send_rst(handle, returned_dst_coap_msg_ptr->msg_id, src_addr_ptr, param);
        }

        /* Release memory of CoAP message */
        sn_coap_parser_release_allocated_coap_msg_mem(handle, returned_dst_coap_msg_ptr);

        /* Return NULL because Header validity check failed */
        return NULL;
    }

    /* Check if we need to send reset message */
    /*  A recipient MUST acknowledge a Confirmable message with an Acknowledgement
        message or, if it lacks context to process the message properly
        (including the case where the message is Empty, uses a code with a
        reserved class (1, 6 or 7), or has a message format error), MUST
        reject it; rejecting a Confirmable message is effected by sending a
        matching Reset message and otherwise ignoring it. */
    if (returned_dst_coap_msg_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE) {
        /* CoAP ping */
        if (returned_dst_coap_msg_ptr->msg_code == COAP_MSG_CODE_EMPTY) {
            sn_coap_protocol_send_rst(handle, returned_dst_coap_msg_ptr->msg_id, src_addr_ptr, param);

            /* Release memory of CoAP message */
            sn_coap_parser_release_allocated_coap_msg_mem(handle, returned_dst_coap_msg_ptr);

            /* Return NULL because Header validity check failed */
            return NULL;
        }
    }


#if !SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE /* If Message blockwising is used, this part of code will not be compiled */
    /* If blockwising used in received message */
    if (returned_dst_coap_msg_ptr->options_list_ptr != NULL &&
            (returned_dst_coap_msg_ptr->options_list_ptr->block1 != COAP_OPTION_BLOCK_NONE ||
             returned_dst_coap_msg_ptr->options_list_ptr->block2 != COAP_OPTION_BLOCK_NONE)) {
        /* Set returned status to User */
        returned_dst_coap_msg_ptr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_REJECTED;
        //todo: send response -> not implemented
        return returned_dst_coap_msg_ptr;
    }
#endif /* !SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE */

#if SN_COAP_DUPLICATION_MAX_MSGS_COUNT/* If Message duplication is used, this part of code will not be compiled */

    /* * * * Manage received CoAP message duplicate detection  * * * */

    /* If no message duplication detected */
    if ((returned_dst_coap_msg_ptr->msg_type == COAP_MSG_TYPE_CONFIRMABLE ||
            returned_dst_coap_msg_ptr->msg_type == COAP_MSG_TYPE_NON_CONFIRMABLE) &&
            handle->sn_coap_duplication_buffer_size != 0) {
        if (sn_coap_protocol_linked_list_duplication_info_search(handle, src_addr_ptr, returned_dst_coap_msg_ptr->msg_id) == NULL) {
            /* * * No Message duplication: Store received message for detecting later duplication * * */

            /* Get count of stored duplication messages */
            uint16_t stored_duplication_msgs_count = handle->count_duplication_msgs;

            /* Check if there is no room to store message for duplication detection purposes */
            if (stored_duplication_msgs_count >= handle->sn_coap_duplication_buffer_size) {
                /* Get oldest stored duplication message */
                coap_duplication_info_s *stored_duplication_info_ptr = ns_list_get_first(&handle->linked_list_duplication_msgs);

                /* Remove oldest stored duplication message for getting room for new duplication message */
                sn_coap_protocol_linked_list_duplication_info_remove(handle,
                                                                     stored_duplication_info_ptr->address->addr_ptr,
                                                                     stored_duplication_info_ptr->address->port,
                                                                     stored_duplication_info_ptr->msg_id);
            }

            /* Store Duplication info to Linked list */
            sn_coap_protocol_linked_list_duplication_info_store(handle, src_addr_ptr, returned_dst_coap_msg_ptr->msg_id, param);
        } else { /* * * Message duplication detected * * */
            /* Set returned status to User */
            returned_dst_coap_msg_ptr->coap_status = COAP_STATUS_PARSER_DUPLICATED_MSG;
            coap_duplication_info_s* response = sn_coap_protocol_linked_list_duplication_info_search(handle,
                                                                                                     src_addr_ptr,
                                                                                                     returned_dst_coap_msg_ptr->msg_id);
            /* Send ACK response */
            if (response) {
                /* Check that response has been created */
                if (response->packet_ptr) {
                    response->coap->sn_coap_tx_callback(response->packet_ptr,
                            response->packet_len, response->address, response->param);
                }
            }

            return returned_dst_coap_msg_ptr;
        }
    }
#endif

    /*** And here we check if message was block message ***/
    /*** If so, we call own block handling function and ***/
    /*** return to caller.                              ***/
#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE

    if (returned_dst_coap_msg_ptr->options_list_ptr != NULL &&
            (returned_dst_coap_msg_ptr->options_list_ptr->block1 != COAP_OPTION_BLOCK_NONE ||
             returned_dst_coap_msg_ptr->options_list_ptr->block2 != COAP_OPTION_BLOCK_NONE)) {
        returned_dst_coap_msg_ptr = sn_coap_handle_blockwise_message(handle, src_addr_ptr, returned_dst_coap_msg_ptr, param);
    } else {
        /* Get ... */
        coap_blockwise_msg_s *stored_blockwise_msg_temp_ptr = NULL;

        ns_list_foreach(coap_blockwise_msg_s, msg, &handle->linked_list_blockwise_sent_msgs) {
            if (returned_dst_coap_msg_ptr->msg_id == msg->coap_msg_ptr->msg_id) {
                stored_blockwise_msg_temp_ptr = msg;
                break;
            }
        }
        /* Remove from the list if not an notification message.
         * Initial notification message is needed for sending rest of the blocks (GET request).
        */
        bool remove_from_the_list = false;
        if (stored_blockwise_msg_temp_ptr) {
            if (stored_blockwise_msg_temp_ptr->coap_msg_ptr &&
                stored_blockwise_msg_temp_ptr->coap_msg_ptr->options_list_ptr &&
                stored_blockwise_msg_temp_ptr->coap_msg_ptr->options_list_ptr->observe != COAP_OBSERVE_NONE) {
                remove_from_the_list = false;
            } else {
                remove_from_the_list = true;
            }
        }
        if (remove_from_the_list) {
            ns_list_remove(&handle->linked_list_blockwise_sent_msgs, stored_blockwise_msg_temp_ptr);
            if (stored_blockwise_msg_temp_ptr->coap_msg_ptr) {
                if(stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_ptr){
                    handle->sn_coap_protocol_free(stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_ptr);
                    stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_ptr = 0;
                }
                sn_coap_parser_release_allocated_coap_msg_mem(stored_blockwise_msg_temp_ptr->coap, stored_blockwise_msg_temp_ptr->coap_msg_ptr);
            }

            handle->sn_coap_protocol_free(stored_blockwise_msg_temp_ptr);
            stored_blockwise_msg_temp_ptr = 0;
        }
    }

    if (!returned_dst_coap_msg_ptr) {
        return NULL;
    }

#endif


#if ENABLE_RESENDINGS  /* If Message resending is not used at all, this part of code will not be compiled */

    /* Check if received Message type was acknowledgement */
    if ((returned_dst_coap_msg_ptr->msg_type == COAP_MSG_TYPE_ACKNOWLEDGEMENT) || (returned_dst_coap_msg_ptr->msg_type == COAP_MSG_TYPE_RESET)) {
        /* * * * Manage CoAP message resending by removing active resending message from Linked list * * */

        /* Get node count i.e. count of active resending messages */
        uint16_t stored_resending_msgs_count = handle->count_resent_msgs;

        /* Check if there is ongoing active message resendings */
        if (stored_resending_msgs_count > 0) {
            sn_nsdl_transmit_s *removed_msg_ptr = NULL;

            /* Check if received message was confirmation for some active resending message */
            removed_msg_ptr = sn_coap_protocol_linked_list_send_msg_search(handle, src_addr_ptr, returned_dst_coap_msg_ptr->msg_id);

            if (removed_msg_ptr != NULL) {
                /* Remove resending message from active message resending Linked list */
                sn_coap_protocol_linked_list_send_msg_remove(handle, src_addr_ptr, returned_dst_coap_msg_ptr->msg_id);
            }
        }
    }
#endif /* ENABLE_RESENDINGS */

    /* * * * Return parsed CoAP message  * * * */
    return returned_dst_coap_msg_ptr;
}


int8_t sn_coap_protocol_exec(struct coap_s *handle, uint32_t current_time)
{
    if( !handle ){
       return -1;
    }

    /* * * * Store current System time * * * */
    handle->system_time = current_time;
#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
    /* * * * Remove old blocwise data * * * */
    sn_coap_protocol_linked_list_blockwise_remove_old_data(handle);
#endif


#if SN_COAP_DUPLICATION_MAX_MSGS_COUNT
    /* * * * Remove old duplication messages * * * */
    sn_coap_protocol_linked_list_duplication_info_remove_old_ones(handle);
#endif

#if ENABLE_RESENDINGS
    /* Check if there is ongoing active message sendings */
    ns_list_foreach_safe(coap_send_msg_s, stored_msg_ptr, &handle->linked_list_resent_msgs) {
        // First check that msg belongs to handle
        if( stored_msg_ptr->coap == handle ){
            /* Check if it is time to send this message */
            if (current_time >= stored_msg_ptr->resending_time) {
                /* * * Increase Resending counter  * * */
                stored_msg_ptr->resending_counter++;

                /* Check if all re-sendings have been done */
                if (stored_msg_ptr->resending_counter > handle->sn_coap_resending_count) {
                    coap_version_e coap_version = COAP_VERSION_UNKNOWN;

                    /* Get message ID from stored sending message */
                    uint16_t temp_msg_id = (stored_msg_ptr->send_msg_ptr->packet_ptr[2] << 8);
                    temp_msg_id += (uint16_t)stored_msg_ptr->send_msg_ptr->packet_ptr[3];

                    /* Remove message from Linked list */
                    ns_list_remove(&handle->linked_list_resent_msgs, stored_msg_ptr);
                    --handle->count_resent_msgs;

                    /* If RX callback have been defined.. */
                    if (stored_msg_ptr->coap->sn_coap_rx_callback != 0) {
                        sn_coap_hdr_s *tmp_coap_hdr_ptr;
                        /* Parse CoAP message, set status and call RX callback */
                        tmp_coap_hdr_ptr = sn_coap_parser(stored_msg_ptr->coap, stored_msg_ptr->send_msg_ptr->packet_len, stored_msg_ptr->send_msg_ptr->packet_ptr, &coap_version);

                        if (tmp_coap_hdr_ptr != 0) {
                            tmp_coap_hdr_ptr->coap_status = COAP_STATUS_BUILDER_MESSAGE_SENDING_FAILED;
                            stored_msg_ptr->coap->sn_coap_rx_callback(tmp_coap_hdr_ptr, stored_msg_ptr->send_msg_ptr->dst_addr_ptr, stored_msg_ptr->param);

                            sn_coap_parser_release_allocated_coap_msg_mem(stored_msg_ptr->coap, tmp_coap_hdr_ptr);
                        }
                    }

                    /* Free memory of stored message */
                    sn_coap_protocol_release_allocated_send_msg_mem(handle, stored_msg_ptr);
                } else {
                    /* Send message  */
                    stored_msg_ptr->coap->sn_coap_tx_callback(stored_msg_ptr->send_msg_ptr->packet_ptr,
                            stored_msg_ptr->send_msg_ptr->packet_len, stored_msg_ptr->send_msg_ptr->dst_addr_ptr, stored_msg_ptr->param);

                    /* * * Count new Resending time  * * */
                    stored_msg_ptr->resending_time = sn_coap_calculate_new_resend_time(current_time,
                                                                                       handle->sn_coap_resending_intervall,
                                                                                       stored_msg_ptr->resending_counter);
                }

            }
        }
    }

#endif /* ENABLE_RESENDINGS */

    return 0;
}

#if ENABLE_RESENDINGS  /* If Message resending is not used at all, this part of code will not be compiled */

/**************************************************************************//**
 * \fn static uint8_t sn_coap_protocol_linked_list_send_msg_store(sn_nsdl_addr_s *dst_addr_ptr, uint16_t send_packet_data_len, uint8_t *send_packet_data_ptr, uint32_t sending_time)
 *
 * \brief Stores message to Linked list for sending purposes.

 * \param *dst_addr_ptr is pointer to destination address where CoAP message will be sent
 *
 * \param send_packet_data_len is length of Packet data to be stored
 *
 * \param *send_packet_data_ptr is Packet data to be stored
 *
 * \param sending_time is stored sending time
 *
 * \return 0 Allocation or buffer limit reached
 *
 * \return 1 Msg stored properly
 *****************************************************************************/

static uint8_t sn_coap_protocol_linked_list_send_msg_store(struct coap_s *handle, sn_nsdl_addr_s *dst_addr_ptr, uint16_t send_packet_data_len,
        uint8_t *send_packet_data_ptr, uint32_t sending_time, void *param)
{

    coap_send_msg_s *stored_msg_ptr              = NULL;

    /* If both queue parameters are "0" or resending count is "0", then re-sending is disabled */
    if (((handle->sn_coap_resending_queue_msgs == 0) && (handle->sn_coap_resending_queue_bytes == 0)) || (handle->sn_coap_resending_count == 0)) {
        return 1;
    }

    if (handle->sn_coap_resending_queue_msgs > 0) {
        if (handle->count_resent_msgs >= handle->sn_coap_resending_queue_msgs) {
            return 0;
        }
    }

    /* Count resending queue size, if buffer size is defined */
    if (handle->sn_coap_resending_queue_bytes > 0) {
        if ((sn_coap_count_linked_list_size(&handle->linked_list_resent_msgs) + send_packet_data_len) > handle->sn_coap_resending_queue_bytes) {
            return 0;
        }
    }

    /* Allocating memory for stored message */
    stored_msg_ptr = sn_coap_protocol_allocate_mem_for_msg(handle, dst_addr_ptr, send_packet_data_len);

    if (stored_msg_ptr == 0) {
        return 0;
    }

    /* Filling of coap_send_msg_s with initialization values */
    stored_msg_ptr->resending_counter = 0;
    stored_msg_ptr->resending_time = sending_time;

    /* Filling of sn_nsdl_transmit_s */
    stored_msg_ptr->send_msg_ptr->protocol = SN_NSDL_PROTOCOL_COAP;
    stored_msg_ptr->send_msg_ptr->packet_len = send_packet_data_len;
    memcpy(stored_msg_ptr->send_msg_ptr->packet_ptr, send_packet_data_ptr, send_packet_data_len);

    /* Filling of sn_nsdl_addr_s */
    stored_msg_ptr->send_msg_ptr->dst_addr_ptr->type = dst_addr_ptr->type;
    stored_msg_ptr->send_msg_ptr->dst_addr_ptr->addr_len = dst_addr_ptr->addr_len;
    memcpy(stored_msg_ptr->send_msg_ptr->dst_addr_ptr->addr_ptr, dst_addr_ptr->addr_ptr, dst_addr_ptr->addr_len);
    stored_msg_ptr->send_msg_ptr->dst_addr_ptr->port = dst_addr_ptr->port;

    stored_msg_ptr->coap = handle;
    stored_msg_ptr->param = param;

    /* Storing Resending message to Linked list */
    ns_list_add_to_end(&handle->linked_list_resent_msgs, stored_msg_ptr);
    ++handle->count_resent_msgs;
    return 1;
}

/**************************************************************************//**
 * \fn static sn_nsdl_transmit_s *sn_coap_protocol_linked_list_send_msg_search(sn_nsdl_addr_s *src_addr_ptr, uint16_t msg_id)
 *
 * \brief Searches stored resending message from Linked list
 *
 * \param *src_addr_ptr is searching key for searched message
 *
 * \param msg_id is searching key for searched message
 *
 * \return Return value is pointer to found stored resending message in Linked
 *         list or NULL if message not found
 *****************************************************************************/

static sn_nsdl_transmit_s *sn_coap_protocol_linked_list_send_msg_search(struct coap_s *handle,
        sn_nsdl_addr_s *src_addr_ptr, uint16_t msg_id)
{
    /* Loop all stored resending messages Linked list */
    ns_list_foreach(coap_send_msg_s, stored_msg_ptr, &handle->linked_list_resent_msgs) {
        /* Get message ID from stored resending message */
        uint16_t temp_msg_id = (stored_msg_ptr->send_msg_ptr->packet_ptr[2] << 8);
        temp_msg_id += (uint16_t)stored_msg_ptr->send_msg_ptr->packet_ptr[3];

        /* If message's Message ID is same than is searched */
        if (temp_msg_id == msg_id) {
            /* If message's Source address is same than is searched */
            if (0 == memcmp(src_addr_ptr->addr_ptr, stored_msg_ptr->send_msg_ptr->dst_addr_ptr->addr_ptr, src_addr_ptr->addr_len)) {
                /* If message's Source address port is same than is searched */
                if (stored_msg_ptr->send_msg_ptr->dst_addr_ptr->port == src_addr_ptr->port) {
                    /* * * Message found, return pointer to that stored resending message * * * */
                    return stored_msg_ptr->send_msg_ptr;
                }
            }
        }
    }

    /* Message not found */
    return NULL;
}
/**************************************************************************//**
 * \fn static void sn_coap_protocol_linked_list_send_msg_remove(sn_nsdl_addr_s *src_addr_ptr, uint16_t msg_id)
 *
 * \brief Removes stored resending message from Linked list
 *
 * \param *src_addr_ptr is searching key for searched message
 * \param msg_id is searching key for removed message
 *****************************************************************************/

static void sn_coap_protocol_linked_list_send_msg_remove(struct coap_s *handle, sn_nsdl_addr_s *src_addr_ptr, uint16_t msg_id)
{
    /* Loop all stored resending messages in Linked list */
    ns_list_foreach(coap_send_msg_s, stored_msg_ptr, &handle->linked_list_resent_msgs) {
        /* Get message ID from stored resending message */
        uint16_t temp_msg_id = (stored_msg_ptr->send_msg_ptr->packet_ptr[2] << 8);
        temp_msg_id += (uint16_t)stored_msg_ptr->send_msg_ptr->packet_ptr[3];

        /* If message's Message ID is same than is searched */
        if (temp_msg_id == msg_id) {
            /* If message's Source address is same than is searched */
            if (0 == memcmp(src_addr_ptr->addr_ptr, stored_msg_ptr->send_msg_ptr->dst_addr_ptr->addr_ptr, src_addr_ptr->addr_len)) {
                /* If message's Source address port is same than is searched */
                if (stored_msg_ptr->send_msg_ptr->dst_addr_ptr->port == src_addr_ptr->port) {
                    /* * * Message found * * */

                    /* Remove message from Linked list */
                    ns_list_remove(&handle->linked_list_resent_msgs, stored_msg_ptr);
                    --handle->count_resent_msgs;

                    /* Free memory of stored message */
                    sn_coap_protocol_release_allocated_send_msg_mem(handle, stored_msg_ptr);

                    return;
                }
            }
        }
    }
}

uint32_t sn_coap_calculate_new_resend_time(const uint32_t current_time, const uint8_t interval, const uint8_t counter)
{
    uint32_t resend_time = interval << counter;
    uint16_t random_factor = randLIB_get_random_in_range(100, RESPONSE_RANDOM_FACTOR * 100);
    return current_time + ((resend_time * random_factor) / 100);
}

#endif /* ENABLE_RESENDINGS */

static void sn_coap_protocol_send_rst(struct coap_s *handle, uint16_t msg_id, sn_nsdl_addr_s *addr_ptr, void *param)
{
    uint8_t packet_ptr[4];

    /* Add CoAP version and message type */
    packet_ptr[0] = COAP_VERSION_1;
    packet_ptr[0] |= COAP_MSG_TYPE_RESET;

    /* Add message code */
    packet_ptr[1] = COAP_MSG_CODE_EMPTY;

    /* Add message ID */
    packet_ptr[2] = msg_id >> 8;
    packet_ptr[3] = (uint8_t)msg_id;

    /* Send RST */
    handle->sn_coap_tx_callback(packet_ptr, 4, addr_ptr, param);

}
#if SN_COAP_DUPLICATION_MAX_MSGS_COUNT /* If Message duplication detection is not used at all, this part of code will not be compiled */

/**************************************************************************//**
 * \fn static void sn_coap_protocol_linked_list_duplication_info_store(sn_nsdl_addr_s *addr_ptr, uint16_t msg_id)
 *
 * \brief Stores Duplication info to Linked list
 *
 * \param msg_id is Message ID to be stored
 * \param *addr_ptr is pointer to Address information to be stored
 *****************************************************************************/

static void sn_coap_protocol_linked_list_duplication_info_store(struct coap_s *handle, sn_nsdl_addr_s *addr_ptr,
        uint16_t msg_id, void *param)
{
    coap_duplication_info_s *stored_duplication_info_ptr = NULL;

    /* * * * Allocating memory for stored Duplication info * * * */

    /* Allocate memory for stored Duplication info's structure */
    stored_duplication_info_ptr = handle->sn_coap_protocol_malloc(sizeof(coap_duplication_info_s));

    if (stored_duplication_info_ptr == NULL) {
        return;
    }
    memset(stored_duplication_info_ptr, 0, sizeof(coap_duplication_info_s));

    /* Allocate memory for stored Duplication info's address */
    stored_duplication_info_ptr->address = handle->sn_coap_protocol_malloc(sizeof(sn_nsdl_addr_s));
    if (stored_duplication_info_ptr->address == NULL) {
        handle->sn_coap_protocol_free(stored_duplication_info_ptr);
        stored_duplication_info_ptr = 0;
        return;
    }
    memset(stored_duplication_info_ptr->address, 0, sizeof(sn_nsdl_addr_s));

    stored_duplication_info_ptr->address->addr_ptr = handle->sn_coap_protocol_malloc(addr_ptr->addr_len);

    if (stored_duplication_info_ptr->address->addr_ptr == NULL) {
        handle->sn_coap_protocol_free(stored_duplication_info_ptr->address);
        stored_duplication_info_ptr->address = 0;
        handle->sn_coap_protocol_free(stored_duplication_info_ptr);
        stored_duplication_info_ptr = 0;
        return;
    }

    /* * * * Filling fields of stored Duplication info * * * */
    stored_duplication_info_ptr->timestamp = handle->system_time;
    stored_duplication_info_ptr->address->addr_len = addr_ptr->addr_len;
    memcpy(stored_duplication_info_ptr->address->addr_ptr, addr_ptr->addr_ptr, addr_ptr->addr_len);
    stored_duplication_info_ptr->address->port = addr_ptr->port;
    stored_duplication_info_ptr->msg_id = msg_id;

    stored_duplication_info_ptr->coap = handle;

    stored_duplication_info_ptr->param = param;
    /* * * * Storing Duplication info to Linked list * * * */

    ns_list_add_to_end(&handle->linked_list_duplication_msgs, stored_duplication_info_ptr);
    ++handle->count_duplication_msgs;
}

/**************************************************************************//**
 * \fn static int8_t sn_coap_protocol_linked_list_duplication_info_search(sn_nsdl_addr_s *addr_ptr, uint16_t msg_id)
 *
 * \brief Searches stored message from Linked list (Address and Message ID as key)
 *
 * \param *addr_ptr is pointer to Address key to be searched
 * \param msg_id is Message ID key to be searched
 *
 * \return Return value is 0 when message found and -1 if not found
 *****************************************************************************/

static coap_duplication_info_s* sn_coap_protocol_linked_list_duplication_info_search(struct coap_s *handle,
        sn_nsdl_addr_s *addr_ptr, uint16_t msg_id)
{
    /* Loop all nodes in Linked list for searching Message ID */
    ns_list_foreach(coap_duplication_info_s, stored_duplication_info_ptr, &handle->linked_list_duplication_msgs) {
        /* If message's Message ID is same than is searched */
        if (stored_duplication_info_ptr->msg_id == msg_id) {
            /* If message's Source address is same than is searched */
            if (0 == memcmp(addr_ptr->addr_ptr, stored_duplication_info_ptr->address->addr_ptr, addr_ptr->addr_len)) {
                /* If message's Source address port is same than is searched */
                if (stored_duplication_info_ptr->address->port == addr_ptr->port) {
                    /* * * Correct Duplication info found * * * */
                    return stored_duplication_info_ptr;
                }
            }
        }
    }
    return NULL;
}

/**************************************************************************//**
 * \fn static void sn_coap_protocol_linked_list_duplication_info_remove(struct coap_s *handle, uint8_t *addr_ptr, uint16_t port, uint16_t msg_id)
 *
 * \brief Removes stored Duplication info from Linked list
 *
 * \param *addr_ptr is pointer to Address key to be removed
 *
 * \param port is Port key to be removed
 *
 * \param msg_id is Message ID key to be removed
 *****************************************************************************/

static void sn_coap_protocol_linked_list_duplication_info_remove(struct coap_s *handle, uint8_t *addr_ptr, uint16_t port, uint16_t msg_id)
{
    /* Loop all stored duplication messages in Linked list */
    ns_list_foreach(coap_duplication_info_s, removed_duplication_info_ptr, &handle->linked_list_duplication_msgs) {
        /* If message's Address is same than is searched */
        if (handle == removed_duplication_info_ptr->coap && 0 == memcmp(addr_ptr,
                                                                        removed_duplication_info_ptr->address->addr_ptr,
                                                                        removed_duplication_info_ptr->address->addr_len)) {
            /* If message's Address prt is same than is searched */
            if (removed_duplication_info_ptr->address->port == port) {
                /* If Message ID is same than is searched */
                if (removed_duplication_info_ptr->msg_id == msg_id) {
                    /* * * * Correct Duplication info found, remove it from Linked list * * * */
                    ns_list_remove(&handle->linked_list_duplication_msgs, removed_duplication_info_ptr);
                    --handle->count_duplication_msgs;

                    /* Free memory of stored Duplication info */
                    handle->sn_coap_protocol_free(removed_duplication_info_ptr->address->addr_ptr);
                    removed_duplication_info_ptr->address->addr_ptr = 0;
                    handle->sn_coap_protocol_free(removed_duplication_info_ptr->address);
                    removed_duplication_info_ptr->address = 0;
                    handle->sn_coap_protocol_free(removed_duplication_info_ptr->packet_ptr);
                    removed_duplication_info_ptr->packet_ptr = 0;
                    handle->sn_coap_protocol_free(removed_duplication_info_ptr);
                    removed_duplication_info_ptr = 0;
                    return;
                }
            }
        }
    }
}

/**************************************************************************//**
 * \fn static void sn_coap_protocol_linked_list_duplication_info_remove_old_ones(struct coap_s *handle)
 *
 * \brief Removes old stored Duplication detection infos from Linked list
 *****************************************************************************/

static void sn_coap_protocol_linked_list_duplication_info_remove_old_ones(struct coap_s *handle)
{
    /* Loop all stored duplication messages in Linked list */
    ns_list_foreach_safe(coap_duplication_info_s, removed_duplication_info_ptr, &handle->linked_list_duplication_msgs) {
        if ((handle->system_time - removed_duplication_info_ptr->timestamp)  > SN_COAP_DUPLICATION_MAX_TIME_MSGS_STORED) {
            /* * * * Old Duplication info found, remove it from Linked list * * * */
            ns_list_remove(&handle->linked_list_duplication_msgs, removed_duplication_info_ptr);
            --handle->count_duplication_msgs;

            /* Free memory of stored Duplication info */
            handle->sn_coap_protocol_free(removed_duplication_info_ptr->address->addr_ptr);
            removed_duplication_info_ptr->address->addr_ptr = 0;
            handle->sn_coap_protocol_free(removed_duplication_info_ptr->address);
            removed_duplication_info_ptr->address = 0;
            handle->sn_coap_protocol_free(removed_duplication_info_ptr->packet_ptr);
            removed_duplication_info_ptr->packet_ptr = 0;
            handle->sn_coap_protocol_free(removed_duplication_info_ptr);
            removed_duplication_info_ptr = 0;
        }
    }
}

#endif /* SN_COAP_DUPLICATION_MAX_MSGS_COUNT */

#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
/**************************************************************************//**
 * \fn static void sn_coap_protocol_linked_list_blockwise_msg_remove(struct coap_s *handle, coap_blockwise_msg_s *removed_msg_ptr)
 *
 * \brief Removes stored blockwise message from Linked list
 *
 * \param removed_msg_ptr is message to be removed
 *****************************************************************************/

static void sn_coap_protocol_linked_list_blockwise_msg_remove(struct coap_s *handle, coap_blockwise_msg_s *removed_msg_ptr)
{
    if( removed_msg_ptr->coap == handle ){
        ns_list_remove(&handle->linked_list_blockwise_sent_msgs, removed_msg_ptr);

        if( removed_msg_ptr->coap_msg_ptr ){
            if (removed_msg_ptr->coap_msg_ptr->payload_ptr) {
                handle->sn_coap_protocol_free(removed_msg_ptr->coap_msg_ptr->payload_ptr);
                removed_msg_ptr->coap_msg_ptr->payload_ptr = 0;
            }

            sn_coap_parser_release_allocated_coap_msg_mem(handle, removed_msg_ptr->coap_msg_ptr);
        }

        handle->sn_coap_protocol_free(removed_msg_ptr);
        removed_msg_ptr = 0;
    }
}

/**************************************************************************//**
 * \fn static void sn_coap_protocol_linked_list_blockwise_payload_store(sn_nsdl_addr_s *addr_ptr, uint16_t stored_payload_len, uint8_t *stored_payload_ptr)
 *
 * \brief Stores blockwise payload to Linked list
 *
 * \param *addr_ptr is pointer to Address information to be stored
 * \param stored_payload_len is length of stored Payload
 * \param *stored_payload_ptr is pointer to stored Payload
 *****************************************************************************/

static void sn_coap_protocol_linked_list_blockwise_payload_store(struct coap_s *handle, sn_nsdl_addr_s *addr_ptr,
        uint16_t stored_payload_len,
        uint8_t *stored_payload_ptr,
        uint32_t block_number)
{
    if (!addr_ptr || !stored_payload_len || !stored_payload_ptr) {
        return;
    }

    coap_blockwise_payload_s *stored_blockwise_payload_ptr = NULL;

    /* * * * Allocating memory for stored Payload  * * * */

    /* Allocate memory for stored Payload's structure */
    stored_blockwise_payload_ptr = handle->sn_coap_protocol_malloc(sizeof(coap_blockwise_payload_s));

    if (stored_blockwise_payload_ptr == NULL) {
        return;
    }


    /* Allocate memory for stored Payload's data */
    stored_blockwise_payload_ptr->payload_ptr = handle->sn_coap_protocol_malloc(stored_payload_len);

    if (stored_blockwise_payload_ptr->payload_ptr == NULL) {
        handle->sn_coap_protocol_free(stored_blockwise_payload_ptr);
        stored_blockwise_payload_ptr = 0;
        return;
    }

    /* Allocate memory for stored Payload's address */
    stored_blockwise_payload_ptr->addr_ptr = handle->sn_coap_protocol_malloc(addr_ptr->addr_len);

    if (stored_blockwise_payload_ptr->addr_ptr == NULL) {
        handle->sn_coap_protocol_free(stored_blockwise_payload_ptr->payload_ptr);
        stored_blockwise_payload_ptr->payload_ptr = 0;
        handle->sn_coap_protocol_free(stored_blockwise_payload_ptr);
        stored_blockwise_payload_ptr = 0;

        return;
    }

    /* * * * Filling fields of stored Payload  * * * */

    stored_blockwise_payload_ptr->timestamp = handle->system_time;

    memcpy(stored_blockwise_payload_ptr->addr_ptr, addr_ptr->addr_ptr, addr_ptr->addr_len);
    stored_blockwise_payload_ptr->port = addr_ptr->port;
    memcpy(stored_blockwise_payload_ptr->payload_ptr, stored_payload_ptr, stored_payload_len);
    stored_blockwise_payload_ptr->payload_len = stored_payload_len;

    stored_blockwise_payload_ptr->coap = handle;

    stored_blockwise_payload_ptr->block_number = block_number;

    /* * * * Storing Payload to Linked list  * * * */

    ns_list_add_to_end(&handle->linked_list_blockwise_received_payloads, stored_blockwise_payload_ptr);
}

/**************************************************************************//**
 * \fn static uint8_t *sn_coap_protocol_linked_list_blockwise_payload_search(sn_nsdl_addr_s *src_addr_ptr, uint16_t *payload_length)
 *
 * \brief Searches stored blockwise payload from Linked list (Address as key)
 *
 * \param *addr_ptr is pointer to Address key to be searched
 * \param *payload_length is pointer to returned Payload length
 *
 * \return Return value is pointer to found stored blockwise payload in Linked
 *         list or NULL if payload not found
 *****************************************************************************/

static uint8_t *sn_coap_protocol_linked_list_blockwise_payload_search(struct coap_s *handle, sn_nsdl_addr_s *src_addr_ptr, uint16_t *payload_length)
{
    /* Loop all stored blockwise payloads in Linked list */
    ns_list_foreach(coap_blockwise_payload_s, stored_payload_info_ptr, &handle->linked_list_blockwise_received_payloads) {
        /* If payload's Source address is same than is searched */
        if (0 == memcmp(src_addr_ptr->addr_ptr, stored_payload_info_ptr->addr_ptr, src_addr_ptr->addr_len)) {
            /* If payload's Source address port is same than is searched */
            if (stored_payload_info_ptr->port == src_addr_ptr->port) {
                /* * * Correct Payload found * * * */
                *payload_length = stored_payload_info_ptr->payload_len;

                return stored_payload_info_ptr->payload_ptr;
            }
        }
    }

    return NULL;
}

static bool sn_coap_protocol_linked_list_blockwise_payload_compare_block_number(struct coap_s *handle,
                                                                                   sn_nsdl_addr_s *src_addr_ptr,
                                                                                   uint32_t block_number)
{
    /* Loop all stored blockwise payloads in Linked list */
    ns_list_foreach(coap_blockwise_payload_s, stored_payload_info_ptr, &handle->linked_list_blockwise_received_payloads) {
        /* If payload's Source address is same than is searched */
        if (0 == memcmp(src_addr_ptr->addr_ptr, stored_payload_info_ptr->addr_ptr, src_addr_ptr->addr_len)) {
            /* If payload's Source address port is same than is searched */
            if (stored_payload_info_ptr->port == src_addr_ptr->port) {
                // Check that incoming block number matches to last received one
                if (block_number - 1 == stored_payload_info_ptr->block_number) {
                    tr_debug("sn_coap_protocol_linked_list_blockwise_payload_search_block_number - found %d", stored_payload_info_ptr->block_number);
                    return true;
                }
            }
        }
    }
    return false;
}

/**************************************************************************//**
 * \fn static void sn_coap_protocol_linked_list_blockwise_payload_remove_oldest(struct coap_s *handle)
 *
 * \brief Removes current stored blockwise paylod from Linked list
 *****************************************************************************/

static void sn_coap_protocol_linked_list_blockwise_payload_remove_oldest(struct coap_s *handle)
{
    coap_blockwise_payload_s *removed_payload_ptr;

    /* Remove oldest node in Linked list*/
    removed_payload_ptr = ns_list_get_first(&handle->linked_list_blockwise_received_payloads);

    if (removed_payload_ptr != NULL) {
        sn_coap_protocol_linked_list_blockwise_payload_remove(handle, removed_payload_ptr);
    }
}

/**************************************************************************//**
 * \fn static void sn_coap_protocol_linked_list_blockwise_payload_remove(struct coap_s *handle,
 *                                                      coap_blockwise_msg_s *removed_msg_ptr)
 *
 * \brief Removes stored blockwise payload from Linked list
 *
 * \param removed_payload_ptr is payload to be removed
 *****************************************************************************/

static void sn_coap_protocol_linked_list_blockwise_payload_remove(struct coap_s *handle,
                                                                  coap_blockwise_payload_s *removed_payload_ptr)
{
    ns_list_remove(&handle->linked_list_blockwise_received_payloads, removed_payload_ptr);
    /* Free memory of stored payload */
    if (removed_payload_ptr->addr_ptr != NULL) {
        handle->sn_coap_protocol_free(removed_payload_ptr->addr_ptr);
        removed_payload_ptr->addr_ptr = 0;
    }

    if (removed_payload_ptr->payload_ptr != NULL) {
        handle->sn_coap_protocol_free(removed_payload_ptr->payload_ptr);
        removed_payload_ptr->payload_ptr = 0;
    }

    handle->sn_coap_protocol_free(removed_payload_ptr);
    removed_payload_ptr = 0;
}

/**************************************************************************//**
 * \fn static uint32_t sn_coap_protocol_linked_list_blockwise_payloads_get_len(sn_nsdl_addr_s *src_addr_ptr)
 *
 * \brief Counts length of Payloads in Linked list (Address as key)
 *
 * \param *addr_ptr is pointer to Address key
 *
 * \return Return value is length of Payloads as bytes
 *****************************************************************************/

static uint32_t sn_coap_protocol_linked_list_blockwise_payloads_get_len(struct coap_s *handle, sn_nsdl_addr_s *src_addr_ptr)
{
    uint32_t ret_whole_payload_len = 0;
    /* Loop all stored blockwise payloads in Linked list */
    ns_list_foreach(coap_blockwise_payload_s, searched_payload_info_ptr, &handle->linked_list_blockwise_received_payloads) {
        /* If payload's Source address is same than is searched */
        if (0 == memcmp(src_addr_ptr->addr_ptr, searched_payload_info_ptr->addr_ptr, src_addr_ptr->addr_len)) {
            /* If payload's Source address port is same than is searched */
            if (searched_payload_info_ptr->port == src_addr_ptr->port) {
                /* * * Correct Payload found * * * */
                ret_whole_payload_len += searched_payload_info_ptr->payload_len;
            }
        }
    }

    return ret_whole_payload_len;
}

/**************************************************************************//**
 * \fn static void sn_coap_protocol_linked_list_blockwise_remove_old_data(struct coap_s *handle)
 *
 * \brief Removes old stored Blockwise messages and payloads from Linked list
 *****************************************************************************/

static void sn_coap_protocol_linked_list_blockwise_remove_old_data(struct coap_s *handle)
{
    /* Loop all stored Blockwise messages in Linked list */
    ns_list_foreach_safe(coap_blockwise_msg_s, removed_blocwise_msg_ptr, &handle->linked_list_blockwise_sent_msgs) {
        if ((handle->system_time - removed_blocwise_msg_ptr->timestamp)  > SN_COAP_BLOCKWISE_MAX_TIME_DATA_STORED) {
            //TODO: Check do we need to check handle == removed_blocwise_msg_ptr->coap here?

            /* * * * Old Blockise message found, remove it from Linked list * * * */
            if( removed_blocwise_msg_ptr->coap_msg_ptr ){
                if(removed_blocwise_msg_ptr->coap_msg_ptr->payload_ptr){
                    handle->sn_coap_protocol_free(removed_blocwise_msg_ptr->coap_msg_ptr->payload_ptr);
                    removed_blocwise_msg_ptr->coap_msg_ptr->payload_ptr = 0;
                }
                sn_coap_parser_release_allocated_coap_msg_mem(handle, removed_blocwise_msg_ptr->coap_msg_ptr);
                removed_blocwise_msg_ptr->coap_msg_ptr = 0;
            }
            sn_coap_protocol_linked_list_blockwise_msg_remove(handle, removed_blocwise_msg_ptr);
        }
    }

    /* Loop all stored Blockwise payloads in Linked list */
    ns_list_foreach_safe(coap_blockwise_payload_s, removed_blocwise_payload_ptr, &handle->linked_list_blockwise_received_payloads) {
        if ((handle->system_time - removed_blocwise_payload_ptr->timestamp)  > SN_COAP_BLOCKWISE_MAX_TIME_DATA_STORED) {
            /* * * * Old Blockise payload found, remove it from Linked list * * * */
            sn_coap_protocol_linked_list_blockwise_payload_remove(handle, removed_blocwise_payload_ptr);
        }
    }
}

#endif /* SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE */


#if ENABLE_RESENDINGS  /* If Message resending is not used at all, this part of code will not be compiled */
/***************************************************************************//**
 * \fn int8_t sn_coap_protocol_allocate_mem_for_msg(sn_nsdl_addr_s *dst_addr_ptr, uint16_t packet_data_len, coap_send_msg_s *msg_ptr)
 *
 * \brief Allocates memory for given message (send or blockwise message)
 *
 * \param *dst_addr_ptr is pointer to destination address where message will be sent
 * \param packet_data_len is length of allocated Packet data
 * \param uri_path_len is length of messages path url
 *
 * \return pointer to allocated struct
 *****************************************************************************/

coap_send_msg_s *sn_coap_protocol_allocate_mem_for_msg(struct coap_s *handle, sn_nsdl_addr_s *dst_addr_ptr, uint16_t packet_data_len)
{

    coap_send_msg_s *msg_ptr = handle->sn_coap_protocol_malloc(sizeof(coap_send_msg_s));

    if (msg_ptr == NULL) {
        return NULL;
    }

    //Locall structure for 1 malloc for send msg
    struct
    {
        sn_nsdl_transmit_s transmit;
        sn_nsdl_addr_s addr;
        uint8_t trail_data[];
    } *m;
    int trail_size = dst_addr_ptr->addr_len + packet_data_len;

    m = handle->sn_coap_protocol_malloc(sizeof *m + trail_size);
    if (!m) {
        handle->sn_coap_protocol_free(msg_ptr);
        return NULL;
    }
    //Init data
    memset(m, 0, sizeof(*m) + trail_size);
    memset(msg_ptr, 0, sizeof(coap_send_msg_s));

    msg_ptr->send_msg_ptr = &m->transmit;
    msg_ptr->send_msg_ptr->dst_addr_ptr = &m->addr;

    msg_ptr->send_msg_ptr->dst_addr_ptr->addr_ptr = m->trail_data;
    if (packet_data_len) {
        msg_ptr->send_msg_ptr->packet_ptr = m->trail_data + dst_addr_ptr->addr_len;
    }

    return msg_ptr;
}


/**************************************************************************//**
 * \fn static void sn_coap_protocol_release_allocated_send_msg_mem(struct coap_s *handle, coap_send_msg_s *freed_send_msg_ptr)
 *
 * \brief Releases memory of given Sending message (coap_send_msg_s)
 *
 * \param *freed_send_msg_ptr is pointer to released Sending message
 *****************************************************************************/

static void sn_coap_protocol_release_allocated_send_msg_mem(struct coap_s *handle, coap_send_msg_s *freed_send_msg_ptr)
{
    if (freed_send_msg_ptr != NULL) {
        handle->sn_coap_protocol_free(freed_send_msg_ptr->send_msg_ptr);
        freed_send_msg_ptr->send_msg_ptr = NULL;
        handle->sn_coap_protocol_free(freed_send_msg_ptr);
        freed_send_msg_ptr = NULL;
    }
}

/**************************************************************************//**
 * \fn static uint16_t sn_coap_count_linked_list_size(const coap_send_msg_list_t *linked_list_ptr)
 *
 * \brief Counts total message size of all messages in linked list
 *
 * \param const coap_send_msg_list_t *linked_list_ptr pointer to linked list
 *****************************************************************************/
static uint16_t sn_coap_count_linked_list_size(const coap_send_msg_list_t *linked_list_ptr)
{
    uint16_t total_size = 0;

    ns_list_foreach(coap_send_msg_s, stored_msg_ptr, linked_list_ptr) {
        if (stored_msg_ptr->send_msg_ptr) {
            total_size += stored_msg_ptr->send_msg_ptr->packet_len;
        }
    }

    return total_size;
}

#endif

#if SN_COAP_MAX_BLOCKWISE_PAYLOAD_SIZE
void sn_coap_protocol_block_remove(struct coap_s *handle, sn_nsdl_addr_s *source_address, uint16_t payload_length, void *payload)
{
    if(!handle || !source_address || !payload){
        return;
    }

    /* Loop all stored blockwise payloads in Linked list */
    ns_list_foreach(coap_blockwise_payload_s, stored_payload_info_ptr, &handle->linked_list_blockwise_received_payloads) {
        /* If payload's Source address is not the same than is searched */
        if (memcmp(source_address->addr_ptr, stored_payload_info_ptr->addr_ptr, source_address->addr_len)) {
            continue;
        }

        /* If payload's Source address port is not the same than is searched */
        if (stored_payload_info_ptr->port != source_address->port) {
            continue;
        }

        /* Check the payload */
        if(payload_length != stored_payload_info_ptr->payload_len){
            continue;
        }

        if(!memcmp(stored_payload_info_ptr->payload_ptr, payload, stored_payload_info_ptr->payload_len))
        {
            /* Everything matches, remove and return. */
            sn_coap_protocol_linked_list_blockwise_payload_remove(handle, stored_payload_info_ptr);
            return;
        }
    }
}
/**************************************************************************//**
 * \fn static int8_t sn_coap_handle_blockwise_message(void)
 *
 * \brief Handles all received blockwise messages
 *
 * \param *src_addr_ptr pointer to source address information struct
 * \param *received_coap_msg_ptr pointer to parsed CoAP message structure
 *****************************************************************************/

static sn_coap_hdr_s *sn_coap_handle_blockwise_message(struct coap_s *handle, sn_nsdl_addr_s *src_addr_ptr, sn_coap_hdr_s *received_coap_msg_ptr, void *param)
{
    tr_debug("sn_coap_handle_blockwise_message");
    sn_coap_hdr_s *src_coap_blockwise_ack_msg_ptr = NULL;
    uint16_t dst_packed_data_needed_mem = 0;
    uint8_t *dst_ack_packet_data_ptr = NULL;
    uint8_t block_temp = 0;

    uint16_t original_payload_len = 0;
    uint8_t *original_payload_ptr = NULL;

    /* Block1 Option in a request (e.g., PUT or POST) */
    // Blocked request sending, received ACK, sending next block..
    if (received_coap_msg_ptr->options_list_ptr->block1 != COAP_OPTION_BLOCK_NONE) {
        tr_debug("sn_coap_handle_blockwise_message - block1, message code: [%d]", received_coap_msg_ptr->msg_code);
        if (received_coap_msg_ptr->msg_code > COAP_MSG_CODE_REQUEST_DELETE) {
            tr_debug("sn_coap_handle_blockwise_message - send block1 request");
            if (received_coap_msg_ptr->options_list_ptr->block1 & 0x08) {
                coap_blockwise_msg_s *stored_blockwise_msg_temp_ptr = NULL;

                /* Get  */
                ns_list_foreach(coap_blockwise_msg_s, msg, &handle->linked_list_blockwise_sent_msgs) {
                    if (msg->coap_msg_ptr && received_coap_msg_ptr->msg_id == msg->coap_msg_ptr->msg_id) {
                        stored_blockwise_msg_temp_ptr = msg;
                        break;
                    }
                }

                if (stored_blockwise_msg_temp_ptr) {
                    /* Build response message */

                    uint16_t block_size;
                    uint32_t block_number;

                    /* Get block option parameters from received message */
                    block_number = received_coap_msg_ptr->options_list_ptr->block1 >> 4;
                    block_temp = received_coap_msg_ptr->options_list_ptr->block1 & 0x07;
                    block_size = 1u << (block_temp + 4);

                    /* Build next block message */
                    src_coap_blockwise_ack_msg_ptr = stored_blockwise_msg_temp_ptr->coap_msg_ptr;

                    if (src_coap_blockwise_ack_msg_ptr->options_list_ptr) {
                        src_coap_blockwise_ack_msg_ptr->options_list_ptr->block1 = COAP_OPTION_BLOCK_NONE;
                        src_coap_blockwise_ack_msg_ptr->options_list_ptr->block2 = COAP_OPTION_BLOCK_NONE;
                    } else {
                        if (!sn_coap_parser_alloc_options(handle, src_coap_blockwise_ack_msg_ptr)) {
                            sn_coap_parser_release_allocated_coap_msg_mem(handle, received_coap_msg_ptr);
                            return 0;
                        }
                    }

                    block_number++;
                    src_coap_blockwise_ack_msg_ptr->options_list_ptr->block1 = (block_number << 4) | block_temp;

                    original_payload_len = stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_len;
                    original_payload_ptr = stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_ptr;

                    if ((block_size * (block_number + 1)) > stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_len) {
                        src_coap_blockwise_ack_msg_ptr->payload_len = stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_len - (block_size * (block_number));
                        src_coap_blockwise_ack_msg_ptr->payload_ptr = src_coap_blockwise_ack_msg_ptr->payload_ptr + (block_size * block_number);
                    }

                    /* Not last block */
                    else {
                        /* set more - bit */
                        src_coap_blockwise_ack_msg_ptr->options_list_ptr->block1 |= 0x08;
                        src_coap_blockwise_ack_msg_ptr->payload_len = block_size;
                        src_coap_blockwise_ack_msg_ptr->payload_ptr = src_coap_blockwise_ack_msg_ptr->payload_ptr + (block_size * block_number);
                    }
                    /* Build and send block message */
                    dst_packed_data_needed_mem = sn_coap_builder_calc_needed_packet_data_size_2(src_coap_blockwise_ack_msg_ptr, handle->sn_coap_block_data_size);

                    dst_ack_packet_data_ptr = handle->sn_coap_protocol_malloc(dst_packed_data_needed_mem);
                    if (!dst_ack_packet_data_ptr) {
                        handle->sn_coap_protocol_free(src_coap_blockwise_ack_msg_ptr->options_list_ptr);
                        src_coap_blockwise_ack_msg_ptr->options_list_ptr = 0;
                        handle->sn_coap_protocol_free(original_payload_ptr);
                        original_payload_ptr = 0;
                        handle->sn_coap_protocol_free(src_coap_blockwise_ack_msg_ptr);
                        src_coap_blockwise_ack_msg_ptr = 0;
                        stored_blockwise_msg_temp_ptr->coap_msg_ptr = NULL;
                        sn_coap_parser_release_allocated_coap_msg_mem(handle, received_coap_msg_ptr);
                        return NULL;
                    }
                    src_coap_blockwise_ack_msg_ptr->msg_id = message_id++;
                    if (message_id == 0) {
                        message_id = 1;
                    }

                    sn_coap_builder_2(dst_ack_packet_data_ptr, src_coap_blockwise_ack_msg_ptr, handle->sn_coap_block_data_size);
                    tr_debug("sn_coap_handle_blockwise_message - block1 request, send block msg id: [%d]", src_coap_blockwise_ack_msg_ptr->msg_id);
                    handle->sn_coap_tx_callback(dst_ack_packet_data_ptr, dst_packed_data_needed_mem, src_addr_ptr, param);

                    handle->sn_coap_protocol_free(dst_ack_packet_data_ptr);
                    dst_ack_packet_data_ptr = 0;

                    stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_len = original_payload_len;
                    stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_ptr = original_payload_ptr;

                    received_coap_msg_ptr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_ACK;
                }
            } else {
                // XXX what was this trying to free?
                tr_debug("sn_coap_handle_blockwise_message - block1 request - last block sent");
                received_coap_msg_ptr->coap_status = COAP_STATUS_OK;

            }
        }

        // Blocked request receiving
        else {
            tr_debug("sn_coap_handle_blockwise_message - block1 received");
            if (received_coap_msg_ptr->payload_len > handle->sn_coap_block_data_size) {
                received_coap_msg_ptr->payload_len = handle->sn_coap_block_data_size;
            }

            // Check that incoming block number is in order.
            uint32_t block_number = received_coap_msg_ptr->options_list_ptr->block1 >> 4;
            bool blocks_in_order = true;
            if (block_number > 0 &&
                !sn_coap_protocol_linked_list_blockwise_payload_compare_block_number(handle,
                                                                                     src_addr_ptr,
                                                                                     block_number)) {
                blocks_in_order = false;
            }

            sn_coap_protocol_linked_list_blockwise_payload_store(handle,
                                                                 src_addr_ptr,
                                                                 received_coap_msg_ptr->payload_len,
                                                                 received_coap_msg_ptr->payload_ptr,
                                                                 block_number);

            /* If not last block (more value is set) */
            /* Block option length can be 1-3 bytes. First 4-20 bits are for block number. Last 4 bits are ALWAYS more bit + block size. */
            if (received_coap_msg_ptr->options_list_ptr->block1 & 0x08) {
                tr_debug("sn_coap_handle_blockwise_message - block1 received, send ack");
                src_coap_blockwise_ack_msg_ptr = sn_coap_parser_alloc_message(handle);
                if (src_coap_blockwise_ack_msg_ptr == NULL) {
                    sn_coap_parser_release_allocated_coap_msg_mem(handle, received_coap_msg_ptr);
                    return NULL;
                }

                if (sn_coap_parser_alloc_options(handle, src_coap_blockwise_ack_msg_ptr) == NULL) {
                    handle->sn_coap_protocol_free(src_coap_blockwise_ack_msg_ptr);
                    src_coap_blockwise_ack_msg_ptr = 0;
                    sn_coap_parser_release_allocated_coap_msg_mem(handle, received_coap_msg_ptr);
                    return NULL;
                }

                // Response with COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_TOO_LARGE if the payload size is more than we can handle
                tr_debug("sn_coap_handle_blockwise_message - block1 received - incoming size: [%d]", received_coap_msg_ptr->options_list_ptr->size1);

                uint32_t max_size = SN_COAP_MAX_INCOMING_BLOCK_MESSAGE_SIZE;
                if (!blocks_in_order) {
                    src_coap_blockwise_ack_msg_ptr->msg_code = COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_INCOMPLETE;
                } else if (received_coap_msg_ptr->options_list_ptr->size1 > max_size) {
                    // Include maximum size that stack can handle into response
                    src_coap_blockwise_ack_msg_ptr->msg_code = COAP_MSG_CODE_RESPONSE_REQUEST_ENTITY_TOO_LARGE;
                    src_coap_blockwise_ack_msg_ptr->options_list_ptr->size1 = max_size;
                } else if (received_coap_msg_ptr->msg_code == COAP_MSG_CODE_REQUEST_GET) {
                    src_coap_blockwise_ack_msg_ptr->msg_code = COAP_MSG_CODE_RESPONSE_CONTENT;
                } else if (received_coap_msg_ptr->msg_code == COAP_MSG_CODE_REQUEST_POST) {
                    src_coap_blockwise_ack_msg_ptr->msg_code = COAP_MSG_CODE_RESPONSE_CONTINUE;
                } else if (received_coap_msg_ptr->msg_code == COAP_MSG_CODE_REQUEST_PUT) {
                    src_coap_blockwise_ack_msg_ptr->msg_code = COAP_MSG_CODE_RESPONSE_CONTINUE;
                } else if (received_coap_msg_ptr->msg_code == COAP_MSG_CODE_REQUEST_DELETE) {
                    src_coap_blockwise_ack_msg_ptr->msg_code = COAP_MSG_CODE_RESPONSE_DELETED;
                }

                src_coap_blockwise_ack_msg_ptr->options_list_ptr->block1 = received_coap_msg_ptr->options_list_ptr->block1;
                src_coap_blockwise_ack_msg_ptr->msg_type = COAP_MSG_TYPE_ACKNOWLEDGEMENT;

                /* Check block size */
                block_temp = (src_coap_blockwise_ack_msg_ptr->options_list_ptr->block1 & 0x07);
                if (block_temp > sn_coap_convert_block_size(handle->sn_coap_block_data_size)) {
                    src_coap_blockwise_ack_msg_ptr->options_list_ptr->block1 &= 0xFFFFF8;
                    src_coap_blockwise_ack_msg_ptr->options_list_ptr->block1 |= sn_coap_convert_block_size(handle->sn_coap_block_data_size);
                }

                src_coap_blockwise_ack_msg_ptr->msg_id = received_coap_msg_ptr->msg_id;

                // Copy token to response
                src_coap_blockwise_ack_msg_ptr->token_ptr = handle->sn_coap_protocol_malloc(received_coap_msg_ptr->token_len);
                if (src_coap_blockwise_ack_msg_ptr->token_ptr) {
                    memcpy(src_coap_blockwise_ack_msg_ptr->token_ptr, received_coap_msg_ptr->token_ptr, received_coap_msg_ptr->token_len);
                    src_coap_blockwise_ack_msg_ptr->token_len = received_coap_msg_ptr->token_len;
                }

                dst_packed_data_needed_mem = sn_coap_builder_calc_needed_packet_data_size_2(src_coap_blockwise_ack_msg_ptr, handle->sn_coap_block_data_size);

                dst_ack_packet_data_ptr = handle->sn_coap_protocol_malloc(dst_packed_data_needed_mem);
                if (!dst_ack_packet_data_ptr) {
                    sn_coap_parser_release_allocated_coap_msg_mem(handle, received_coap_msg_ptr);
                    handle->sn_coap_protocol_free(src_coap_blockwise_ack_msg_ptr->options_list_ptr);
                    src_coap_blockwise_ack_msg_ptr->options_list_ptr = 0;
                    handle->sn_coap_protocol_free(src_coap_blockwise_ack_msg_ptr);
                    src_coap_blockwise_ack_msg_ptr = 0;
                    return NULL;
                }

                sn_coap_builder_2(dst_ack_packet_data_ptr, src_coap_blockwise_ack_msg_ptr, handle->sn_coap_block_data_size);
                tr_debug("sn_coap_handle_blockwise_message - block1 received - send msg id [%d]", src_coap_blockwise_ack_msg_ptr->msg_id);
                handle->sn_coap_tx_callback(dst_ack_packet_data_ptr, dst_packed_data_needed_mem, src_addr_ptr, param);

                sn_coap_parser_release_allocated_coap_msg_mem(handle, src_coap_blockwise_ack_msg_ptr);
                handle->sn_coap_protocol_free(dst_ack_packet_data_ptr);
                dst_ack_packet_data_ptr = 0;

                received_coap_msg_ptr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVING;

            } else {
                tr_debug("sn_coap_handle_blockwise_message - block1 received, last block received");
                /* * * This is the last block when whole Blockwise payload from received * * */
                /* * * blockwise messages is gathered and returned to User               * * */

                /* Store last Blockwise payload to Linked list */
                uint16_t payload_len            = 0;
                uint8_t *payload_ptr            = sn_coap_protocol_linked_list_blockwise_payload_search(handle, src_addr_ptr, &payload_len);
                uint32_t whole_payload_len      = sn_coap_protocol_linked_list_blockwise_payloads_get_len(handle, src_addr_ptr);
                uint8_t *temp_whole_payload_ptr = NULL;

                tr_debug("sn_coap_handle_blockwise_message - block1 received, whole_payload_len %d", whole_payload_len);
                temp_whole_payload_ptr = handle->sn_coap_protocol_malloc(whole_payload_len);
                if (temp_whole_payload_ptr == NULL || whole_payload_len > UINT16_MAX) {
                    tr_debug("sn_coap_handle_blockwise_message - block1 received, last block received alloc fails");
                    sn_coap_parser_release_allocated_coap_msg_mem(handle, received_coap_msg_ptr);
                    handle->sn_coap_protocol_free(temp_whole_payload_ptr);
                    return 0;
                }

                // In block message case, payload_ptr freeing must be done in application level
                received_coap_msg_ptr->payload_ptr = temp_whole_payload_ptr;
                received_coap_msg_ptr->payload_len = whole_payload_len;

                /* Copy stored Blockwise payloads to returned whole Blockwise payload pointer */
                while (payload_ptr != NULL) {
                    memcpy(temp_whole_payload_ptr, payload_ptr, payload_len);
                    temp_whole_payload_ptr += payload_len;
                    sn_coap_protocol_linked_list_blockwise_payload_remove_oldest(handle);
                    payload_ptr = sn_coap_protocol_linked_list_blockwise_payload_search(handle, src_addr_ptr, &payload_len);
                }
                received_coap_msg_ptr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;
            }
        }
    }


    /* Block2 Option in a response (e.g., a 2.05 response for GET) */
    /* Message ID must be same than in received message */
    else {
        tr_debug("sn_coap_handle_blockwise_message - block2 - message code: [%d]", received_coap_msg_ptr->msg_code);
        //This is response to request we made
        if (received_coap_msg_ptr->msg_code > COAP_MSG_CODE_REQUEST_DELETE) {
            tr_debug("sn_coap_handle_blockwise_message - send block2 request");
            uint32_t block_number = 0;

            /* Store blockwise payload to Linked list */
            //todo: add block number to stored values - just to make sure all packets are in order
            sn_coap_protocol_linked_list_blockwise_payload_store(handle,
                                                                 src_addr_ptr,
                                                                 received_coap_msg_ptr->payload_len,
                                                                 received_coap_msg_ptr->payload_ptr,
                                                                 received_coap_msg_ptr->options_list_ptr->block1 >> 4);

            /* If not last block (more value is set) */
            if (received_coap_msg_ptr->options_list_ptr->block2 & 0x08) {
                coap_blockwise_msg_s *previous_blockwise_msg_ptr = NULL;
                //build and send ack
                received_coap_msg_ptr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVING;

                ns_list_foreach(coap_blockwise_msg_s, msg, &handle->linked_list_blockwise_sent_msgs) {
                    if (received_coap_msg_ptr->msg_id == msg->coap_msg_ptr->msg_id) {
                        previous_blockwise_msg_ptr = msg;
                        break;
                    }
                }

                if (!previous_blockwise_msg_ptr || !previous_blockwise_msg_ptr->coap_msg_ptr) {
                    sn_coap_parser_release_allocated_coap_msg_mem(handle, received_coap_msg_ptr);
                    return 0;
                }

                src_coap_blockwise_ack_msg_ptr = sn_coap_parser_alloc_message(handle);
                if (src_coap_blockwise_ack_msg_ptr == NULL) {
                    return 0;
                }

                ns_list_remove(&handle->linked_list_blockwise_sent_msgs, previous_blockwise_msg_ptr);
                if( previous_blockwise_msg_ptr->coap_msg_ptr ){
                    if(previous_blockwise_msg_ptr->coap_msg_ptr->payload_ptr){
                        handle->sn_coap_protocol_free(previous_blockwise_msg_ptr->coap_msg_ptr->payload_ptr);
                        previous_blockwise_msg_ptr->coap_msg_ptr->payload_ptr = 0;
                    }
                    sn_coap_parser_release_allocated_coap_msg_mem(handle, previous_blockwise_msg_ptr->coap_msg_ptr);
                    previous_blockwise_msg_ptr->coap_msg_ptr = 0;
                }
                handle->sn_coap_protocol_free(previous_blockwise_msg_ptr);
                previous_blockwise_msg_ptr = 0;

                /* * * Then build CoAP Acknowledgement message * * */

                if (sn_coap_parser_alloc_options(handle, src_coap_blockwise_ack_msg_ptr) == NULL) {
                    handle->sn_coap_protocol_free(src_coap_blockwise_ack_msg_ptr);
                    src_coap_blockwise_ack_msg_ptr = 0;
                    sn_coap_parser_release_allocated_coap_msg_mem(handle, received_coap_msg_ptr);
                    return NULL;
                }

                src_coap_blockwise_ack_msg_ptr->msg_id = message_id++;
                if (message_id == 0) {
                    message_id = 1;
                }

                /* Update block option */
                block_temp = received_coap_msg_ptr->options_list_ptr->block2 & 0x07;

                block_number = received_coap_msg_ptr->options_list_ptr->block2 >> 4;
                block_number ++;

                src_coap_blockwise_ack_msg_ptr->options_list_ptr->block2 = (block_number << 4) | block_temp;

                /* Then get needed memory count for Packet data */
                dst_packed_data_needed_mem = sn_coap_builder_calc_needed_packet_data_size_2(src_coap_blockwise_ack_msg_ptr ,handle->sn_coap_block_data_size);

                /* Then allocate memory for Packet data */
                dst_ack_packet_data_ptr = handle->sn_coap_protocol_malloc(dst_packed_data_needed_mem);

                if (dst_ack_packet_data_ptr == NULL) {
                    handle->sn_coap_protocol_free(src_coap_blockwise_ack_msg_ptr->options_list_ptr);
                    src_coap_blockwise_ack_msg_ptr->options_list_ptr = 0;
                    handle->sn_coap_protocol_free(src_coap_blockwise_ack_msg_ptr);
                    src_coap_blockwise_ack_msg_ptr = 0;
                    sn_coap_parser_release_allocated_coap_msg_mem(handle, received_coap_msg_ptr);
                    return NULL;
                }
                memset(dst_ack_packet_data_ptr, 0, dst_packed_data_needed_mem);

                /* * * Then build Acknowledgement message to Packed data * * */
                if ((sn_coap_builder_2(dst_ack_packet_data_ptr, src_coap_blockwise_ack_msg_ptr, handle->sn_coap_block_data_size)) < 0) {
                    handle->sn_coap_protocol_free(dst_ack_packet_data_ptr);
                    dst_ack_packet_data_ptr = 0;
                    handle->sn_coap_protocol_free(src_coap_blockwise_ack_msg_ptr->options_list_ptr);
                    src_coap_blockwise_ack_msg_ptr->options_list_ptr = 0;
                    handle->sn_coap_protocol_free(src_coap_blockwise_ack_msg_ptr);
                    src_coap_blockwise_ack_msg_ptr = 0;
                    sn_coap_parser_release_allocated_coap_msg_mem(handle, received_coap_msg_ptr);
                    return NULL;
                }

                /* * * Save to linked list * * */
                coap_blockwise_msg_s *stored_blockwise_msg_ptr;

                stored_blockwise_msg_ptr = handle->sn_coap_protocol_malloc(sizeof(coap_blockwise_msg_s));
                if (!stored_blockwise_msg_ptr) {
                    handle->sn_coap_protocol_free(dst_ack_packet_data_ptr);
                    dst_ack_packet_data_ptr = 0;
                    handle->sn_coap_protocol_free(src_coap_blockwise_ack_msg_ptr->options_list_ptr);
                    src_coap_blockwise_ack_msg_ptr->options_list_ptr = 0;
                    handle->sn_coap_protocol_free(src_coap_blockwise_ack_msg_ptr);
                    src_coap_blockwise_ack_msg_ptr = 0;
                    sn_coap_parser_release_allocated_coap_msg_mem(handle, received_coap_msg_ptr);
                    return 0;
                }
                memset(stored_blockwise_msg_ptr, 0, sizeof(coap_blockwise_msg_s));

                stored_blockwise_msg_ptr->timestamp = handle->system_time;

                stored_blockwise_msg_ptr->coap_msg_ptr = src_coap_blockwise_ack_msg_ptr;
                stored_blockwise_msg_ptr->coap = handle;
                ns_list_add_to_end(&handle->linked_list_blockwise_sent_msgs, stored_blockwise_msg_ptr);

                /* * * Then release memory of CoAP Acknowledgement message * * */
                handle->sn_coap_tx_callback(dst_ack_packet_data_ptr,
                                            dst_packed_data_needed_mem, src_addr_ptr, param);

#if ENABLE_RESENDINGS
                uint32_t resend_time = sn_coap_calculate_new_resend_time(handle->system_time, handle->sn_coap_resending_intervall, 0);
                sn_coap_protocol_linked_list_send_msg_store(handle, src_addr_ptr,
                        dst_packed_data_needed_mem,
                        dst_ack_packet_data_ptr,
                        resend_time, param);
#endif
                handle->sn_coap_protocol_free(dst_ack_packet_data_ptr);
                dst_ack_packet_data_ptr = 0;
            }

            //Last block received
            else {
                /* * * This is the last block when whole Blockwise payload from received * * */
                /* * * blockwise messages is gathered and returned to User               * * */

                /* Store last Blockwise payload to Linked list */
                uint16_t payload_len            = 0;
                uint8_t *payload_ptr            = sn_coap_protocol_linked_list_blockwise_payload_search(handle, src_addr_ptr, &payload_len);
                uint16_t whole_payload_len      = sn_coap_protocol_linked_list_blockwise_payloads_get_len(handle, src_addr_ptr);
                uint8_t *temp_whole_payload_ptr = NULL;

                temp_whole_payload_ptr = handle->sn_coap_protocol_malloc(whole_payload_len);
                if (!temp_whole_payload_ptr) {
                    return 0;
                }

                received_coap_msg_ptr->payload_ptr = temp_whole_payload_ptr;
                received_coap_msg_ptr->payload_len = whole_payload_len;

                /* Copy stored Blockwise payloads to returned whole Blockwise payload pointer */
                while (payload_ptr != NULL) {
                    memcpy(temp_whole_payload_ptr, payload_ptr, payload_len);

                    temp_whole_payload_ptr += payload_len;

                    sn_coap_protocol_linked_list_blockwise_payload_remove_oldest(handle);
                    payload_ptr = sn_coap_protocol_linked_list_blockwise_payload_search(handle, src_addr_ptr, &payload_len);
                }
                received_coap_msg_ptr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_MSG_RECEIVED;

                //todo: remove previous msg from list
            }

        }

        //Now we send data to request
        else {
            tr_debug("sn_coap_handle_blockwise_message - block2 received");
            //Get message by using block number
            //NOTE: Getting the first from list might not be correct one
            coap_blockwise_msg_s *stored_blockwise_msg_temp_ptr = ns_list_get_first(&handle->linked_list_blockwise_sent_msgs);
            if (stored_blockwise_msg_temp_ptr) {
                uint16_t block_size;
                uint32_t block_number;

                /* Resolve block parameters */
                block_number = received_coap_msg_ptr->options_list_ptr->block2 >> 4;
                block_temp = received_coap_msg_ptr->options_list_ptr->block2 & 0x07;
                block_size = 1u << (block_temp + 4);

                /* Build response message */
                src_coap_blockwise_ack_msg_ptr = stored_blockwise_msg_temp_ptr->coap_msg_ptr;

                if (src_coap_blockwise_ack_msg_ptr->options_list_ptr) {
                    src_coap_blockwise_ack_msg_ptr->options_list_ptr->block1 = COAP_OPTION_BLOCK_NONE;
                    src_coap_blockwise_ack_msg_ptr->options_list_ptr->block2 = COAP_OPTION_BLOCK_NONE;
                } else {
                    if (sn_coap_parser_alloc_options(handle, src_coap_blockwise_ack_msg_ptr) == NULL) {
                        return 0;
                    }
                }

                src_coap_blockwise_ack_msg_ptr->msg_id = received_coap_msg_ptr->msg_id;

                src_coap_blockwise_ack_msg_ptr->options_list_ptr->block2 = received_coap_msg_ptr->options_list_ptr->block2;

                /* * Payload part * */

                /* Check if last block */

                original_payload_len = stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_len;
                original_payload_ptr = stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_ptr;

                if ((block_size * (block_number + 1)) >= stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_len) {
                    src_coap_blockwise_ack_msg_ptr->payload_len = stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_len - (block_size * block_number);
                    src_coap_blockwise_ack_msg_ptr->payload_ptr = stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_ptr + (block_size * block_number);
                }
                /* Not last block */
                else {
                    /* set more - bit */
                    src_coap_blockwise_ack_msg_ptr->options_list_ptr->block2 |= 0x08;
                    src_coap_blockwise_ack_msg_ptr->payload_len = block_size;
                    src_coap_blockwise_ack_msg_ptr->payload_ptr = stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_ptr + (block_size * block_number);
                }

                /* Update token to match one which is in GET request.
                 * This is needed only in case of notification message.
                */
                if (src_coap_blockwise_ack_msg_ptr->options_list_ptr &&
                    src_coap_blockwise_ack_msg_ptr->options_list_ptr->observe != COAP_OBSERVE_NONE) {
                    if (received_coap_msg_ptr->token_len && src_coap_blockwise_ack_msg_ptr->token_ptr) {
                        handle->sn_coap_protocol_free(src_coap_blockwise_ack_msg_ptr->token_ptr);
                        src_coap_blockwise_ack_msg_ptr->token_ptr = handle->sn_coap_protocol_malloc(received_coap_msg_ptr->token_len);
                        if (src_coap_blockwise_ack_msg_ptr->token_ptr) {
                            memcpy(src_coap_blockwise_ack_msg_ptr->token_ptr, received_coap_msg_ptr->token_ptr, received_coap_msg_ptr->token_len);
                            src_coap_blockwise_ack_msg_ptr->token_len = received_coap_msg_ptr->token_len;
                        }
                    }
                }

                /* Build and send block message */
                dst_packed_data_needed_mem = sn_coap_builder_calc_needed_packet_data_size_2(src_coap_blockwise_ack_msg_ptr, handle->sn_coap_block_data_size);

                dst_ack_packet_data_ptr = handle->sn_coap_protocol_malloc(dst_packed_data_needed_mem);
                if (!dst_ack_packet_data_ptr) {
                    if(original_payload_ptr){
                        handle->sn_coap_protocol_free(original_payload_ptr);
                        original_payload_ptr = NULL;
                    }
                    handle->sn_coap_protocol_free(src_coap_blockwise_ack_msg_ptr->options_list_ptr);
                    src_coap_blockwise_ack_msg_ptr->options_list_ptr = 0;
                    handle->sn_coap_protocol_free(src_coap_blockwise_ack_msg_ptr);
                    stored_blockwise_msg_temp_ptr->coap_msg_ptr = NULL;
                    return NULL;
                }

                sn_coap_builder_2(dst_ack_packet_data_ptr, src_coap_blockwise_ack_msg_ptr, handle->sn_coap_block_data_size);
                tr_debug("sn_coap_handle_blockwise_message - block2 received, send message: [%d]", src_coap_blockwise_ack_msg_ptr->msg_id);
                handle->sn_coap_tx_callback(dst_ack_packet_data_ptr, dst_packed_data_needed_mem, src_addr_ptr, param);

                handle->sn_coap_protocol_free(dst_ack_packet_data_ptr);
                dst_ack_packet_data_ptr = 0;

                stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_len = original_payload_len;
                stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_ptr = original_payload_ptr;

                if ((block_size * (block_number + 1)) >= stored_blockwise_msg_temp_ptr->coap_msg_ptr->payload_len) {
                    sn_coap_protocol_linked_list_blockwise_msg_remove(handle, stored_blockwise_msg_temp_ptr);
                }

                received_coap_msg_ptr->coap_status = COAP_STATUS_PARSER_BLOCKWISE_ACK;
            }
        }
    }
    return received_coap_msg_ptr;
}

static int8_t sn_coap_convert_block_size(uint16_t block_size)
{
    if (block_size == 16) {
        return 0;
    } else if (block_size == 32) {
        return 1;
    } else if (block_size == 64) {
        return 2;
    } else if (block_size == 128) {
        return 3;
    } else if (block_size == 256) {
        return 4;
    } else if (block_size == 512) {
        return 5;
    } else if (block_size == 1024) {
        return 6;
    }

    return 0;
}

static sn_coap_hdr_s *sn_coap_protocol_copy_header(struct coap_s *handle, sn_coap_hdr_s *source_header_ptr)
{
    sn_coap_hdr_s *destination_header_ptr;

    destination_header_ptr = sn_coap_parser_alloc_message(handle);
    if (!destination_header_ptr) {
        return 0;
    }

    destination_header_ptr->coap_status = source_header_ptr->coap_status;
    destination_header_ptr->msg_type = source_header_ptr->msg_type;
    destination_header_ptr->msg_code = source_header_ptr->msg_code;
    destination_header_ptr->msg_id = source_header_ptr->msg_id;

    if (source_header_ptr->uri_path_ptr) {
        destination_header_ptr->uri_path_len = source_header_ptr->uri_path_len;
        destination_header_ptr->uri_path_ptr = handle->sn_coap_protocol_malloc(source_header_ptr->uri_path_len);
        if (!destination_header_ptr->uri_path_ptr) {
            sn_coap_parser_release_allocated_coap_msg_mem(handle, destination_header_ptr);
            return 0;
        }
        memcpy(destination_header_ptr->uri_path_ptr, source_header_ptr->uri_path_ptr, source_header_ptr->uri_path_len);
    }

    if (source_header_ptr->token_ptr) {
        destination_header_ptr->token_len = source_header_ptr->token_len;
        destination_header_ptr->token_ptr = handle->sn_coap_protocol_malloc(source_header_ptr->token_len);
        if (!destination_header_ptr->token_ptr) {
            sn_coap_parser_release_allocated_coap_msg_mem(handle, destination_header_ptr);
            return 0;
        }
        memcpy(destination_header_ptr->token_ptr, source_header_ptr->token_ptr, source_header_ptr->token_len);
    }

    destination_header_ptr->content_format = source_header_ptr->content_format;

    /* Options list */
    if (source_header_ptr->options_list_ptr) {
        if (sn_coap_parser_alloc_options(handle, destination_header_ptr) == NULL) {
            sn_coap_parser_release_allocated_coap_msg_mem(handle, destination_header_ptr);
            return 0;
        }

        destination_header_ptr->options_list_ptr->max_age = source_header_ptr->options_list_ptr->max_age;

        if (source_header_ptr->options_list_ptr->proxy_uri_ptr) {
            destination_header_ptr->options_list_ptr->proxy_uri_len = source_header_ptr->options_list_ptr->proxy_uri_len;
            destination_header_ptr->options_list_ptr->proxy_uri_ptr = handle->sn_coap_protocol_malloc(source_header_ptr->options_list_ptr->proxy_uri_len);
            if (!destination_header_ptr->options_list_ptr->proxy_uri_ptr) {
                sn_coap_parser_release_allocated_coap_msg_mem(handle, destination_header_ptr);
                return 0;
            }
            memcpy(destination_header_ptr->options_list_ptr->proxy_uri_ptr, source_header_ptr->options_list_ptr->proxy_uri_ptr, source_header_ptr->options_list_ptr->proxy_uri_len);
        }

        if (source_header_ptr->options_list_ptr->etag_ptr) {
            destination_header_ptr->options_list_ptr->etag_len = source_header_ptr->options_list_ptr->etag_len;
            destination_header_ptr->options_list_ptr->etag_ptr = handle->sn_coap_protocol_malloc(source_header_ptr->options_list_ptr->etag_len);
            if (!destination_header_ptr->options_list_ptr->etag_ptr) {
                sn_coap_parser_release_allocated_coap_msg_mem(handle, destination_header_ptr);
                return 0;
            }
            memcpy(destination_header_ptr->options_list_ptr->etag_ptr, source_header_ptr->options_list_ptr->etag_ptr, source_header_ptr->options_list_ptr->etag_len);
        }

        if (source_header_ptr->options_list_ptr->uri_host_ptr) {
            destination_header_ptr->options_list_ptr->uri_host_len = source_header_ptr->options_list_ptr->uri_host_len;
            destination_header_ptr->options_list_ptr->uri_host_ptr = handle->sn_coap_protocol_malloc(source_header_ptr->options_list_ptr->uri_host_len);
            if (!destination_header_ptr->options_list_ptr->uri_host_ptr) {
                sn_coap_parser_release_allocated_coap_msg_mem(handle, destination_header_ptr);
                return 0;
            }
            memcpy(destination_header_ptr->options_list_ptr->uri_host_ptr, source_header_ptr->options_list_ptr->uri_host_ptr, source_header_ptr->options_list_ptr->uri_host_len);
        }

        if (source_header_ptr->options_list_ptr->location_path_ptr) {
            destination_header_ptr->options_list_ptr->location_path_len = source_header_ptr->options_list_ptr->location_path_len;
            destination_header_ptr->options_list_ptr->location_path_ptr = handle->sn_coap_protocol_malloc(source_header_ptr->options_list_ptr->location_path_len);
            if (!destination_header_ptr->options_list_ptr->location_path_ptr) {
                sn_coap_parser_release_allocated_coap_msg_mem(handle, destination_header_ptr);
                return 0;
            }
            memcpy(destination_header_ptr->options_list_ptr->location_path_ptr, source_header_ptr->options_list_ptr->location_path_ptr, source_header_ptr->options_list_ptr->location_path_len);
        }

        destination_header_ptr->options_list_ptr->uri_port = source_header_ptr->options_list_ptr->uri_port;

        if (source_header_ptr->options_list_ptr->location_query_ptr) {
            destination_header_ptr->options_list_ptr->location_query_len = source_header_ptr->options_list_ptr->location_query_len;
            destination_header_ptr->options_list_ptr->location_query_ptr = handle->sn_coap_protocol_malloc(source_header_ptr->options_list_ptr->location_query_len);
            if (!destination_header_ptr->options_list_ptr->location_query_ptr) {
                sn_coap_parser_release_allocated_coap_msg_mem(handle, destination_header_ptr);
                return 0;
            }
            memcpy(destination_header_ptr->options_list_ptr->location_query_ptr, source_header_ptr->options_list_ptr->location_query_ptr, source_header_ptr->options_list_ptr->location_query_len);
        }

        destination_header_ptr->options_list_ptr->observe = source_header_ptr->options_list_ptr->observe;
        destination_header_ptr->options_list_ptr->accept = source_header_ptr->options_list_ptr->accept;

        if (source_header_ptr->options_list_ptr->uri_query_ptr) {
            destination_header_ptr->options_list_ptr->uri_query_len = source_header_ptr->options_list_ptr->uri_query_len;
            destination_header_ptr->options_list_ptr->uri_query_ptr = handle->sn_coap_protocol_malloc(source_header_ptr->options_list_ptr->uri_query_len);
            if (!destination_header_ptr->options_list_ptr->uri_query_ptr) {
                sn_coap_parser_release_allocated_coap_msg_mem(handle, destination_header_ptr);
                return 0;
            }
            memcpy(destination_header_ptr->options_list_ptr->uri_query_ptr, source_header_ptr->options_list_ptr->uri_query_ptr, source_header_ptr->options_list_ptr->uri_query_len);
        }

        destination_header_ptr->options_list_ptr->block1 = source_header_ptr->options_list_ptr->block1;
        destination_header_ptr->options_list_ptr->block2 = source_header_ptr->options_list_ptr->block2;
    }

    return destination_header_ptr;
}
#endif
