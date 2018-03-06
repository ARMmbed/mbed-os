/*
 * Copyright (c) 2017, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest.h"

#if MBED_CONF_APP_TEST_WIFI || MBED_CONF_APP_TEST_ETHERNET

#include "mbed.h"

#include "EMAC.h"
#include "EMACMemoryManager.h"
#include "emac_TestMemoryManager.h"

#include "emac_tests.h"
#include "emac_ctp.h"

#include "emac_initialize.h"
#include "emac_util.h"
#include "emac_membuf.h"

using namespace utest::v1;

// Unique identifier for message
static int receipt_number = 0;

static int emac_if_ctp_header_build(unsigned char *eth_frame, const unsigned char *dest_addr, const unsigned char *origin_addr, const unsigned char *forward_addr)
{
    memcpy(&eth_frame[0], dest_addr, 6);
    memcpy(&eth_frame[6], origin_addr, 6);

    eth_frame[12] = 0x90; /* loop back */
    eth_frame[13] = 0x00;

    eth_frame[14] = 0x00; /* skip count */
    eth_frame[15] = 0x00;

    eth_frame[16] = 0x02; /* function, forward */
    eth_frame[17] = 0x00;

    memcpy(&eth_frame[18], forward_addr, 6);

    eth_frame[24] = 0x01; /* function, reply */
    eth_frame[25] = 0x00;

    receipt_number++;

    eth_frame[26] = receipt_number; /* receipt number */
    eth_frame[27] = receipt_number >> 8;

    return receipt_number;
}

ctp_function emac_if_ctp_header_handle(unsigned char *eth_input_frame, unsigned char *eth_output_frame, unsigned char *origin_addr, int *receipt_number)
{
    if (eth_input_frame[12] != 0x90 || eth_input_frame[13] != 0x00) {
        return CTP_NONE;
    }

    int skip_count = eth_input_frame[15] << 8 | eth_input_frame[14];
    unsigned char *ethernet_ptr = &eth_input_frame[16] + skip_count;

    int function = ethernet_ptr[1] << 8 | ethernet_ptr[0];
    ethernet_ptr += 2;

    // Forward
    if (function == 0x0002) {
        memcpy(eth_output_frame, eth_input_frame, ETH_FRAME_HEADER_LEN);
        // Update skip count
        skip_count += 8;
        eth_output_frame[14] = skip_count;
        eth_output_frame[15] = skip_count >> 8;
        // Set forward address to destination address
        memcpy(&eth_output_frame[0], ethernet_ptr, 6);
        // Copy own address to origin
        memcpy(&eth_output_frame[6], origin_addr, 6);
        return CTP_FORWARD;
    // reply
    } else if (function == 0x0001) {
        *receipt_number = ethernet_ptr[1] << 8 | ethernet_ptr[0];
        return CTP_REPLY;
    }

    return CTP_NONE;
}

void emac_if_ctp_msg_build(int eth_frame_len, const unsigned char *dest_addr, const unsigned char *origin_addr, const unsigned char *forward_addr, int options)
{
    if (eth_frame_len < ETH_FRAME_HEADER_LEN) {
        eth_frame_len = ETH_FRAME_HEADER_LEN;
    }

    if (emac_if_get_trace_level() & TRACE_SEND) {
        printf("message sent %x:%x:%x:%x:%x:%x\r\n\r\n", dest_addr[0], dest_addr[1], dest_addr[2], dest_addr[3], dest_addr[4], dest_addr[5]);
    }

    int outgoing_msg_index = emac_if_add_outgoing_msg(eth_frame_len);

    if (outgoing_msg_index < 0) {
        SET_ERROR_FLAGS(OUT_OF_MSG_DATA);
        return;
    }

    int alloc_opt = 0;
    int align = 0;
    if (options & CTP_OPT_NON_ALIGNED) {
        alloc_opt |= MEM_NO_ALIGN; // Force align to odd address
        align = 1;                 // Reserve memory overhead to align to odd address
    }

    emac_mem_buf_t *buf;
    if (options & CTP_OPT_HEAP) {
        buf = emac_m_mngr_get()->alloc_heap(eth_frame_len, align, alloc_opt);
    } else {
        // Default allocation is from pool
        buf = emac_m_mngr_get()->alloc_pool(eth_frame_len, align, alloc_opt);
    }

    if (!buf) {
        SET_ERROR_FLAGS(NO_FREE_MEM_BUF);
        emac_if_free_outgoing_msg(outgoing_msg_index);
        return;
    }

    if (memcmp(dest_addr, eth_mac_broadcast_addr, 6) == 0) {
        emac_if_set_outgoing_msg_flags(outgoing_msg_index, BROADCAST);
    }

    unsigned char eth_output_frame_data[ETH_FRAME_HEADER_LEN];
    int receipt_number = emac_if_ctp_header_build(eth_output_frame_data, dest_addr, origin_addr, forward_addr);
    emac_if_set_outgoing_msg_receipt_num(outgoing_msg_index, receipt_number);

    emac_if_memory_buffer_write(buf, eth_output_frame_data, true);

    emac_if_check_memory(true);
    emac_if_get()->link_out(buf);
    emac_if_check_memory(false);
}

#endif

