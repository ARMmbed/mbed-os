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

#ifndef EMAC_CTP_H
#define EMAC_CTP_H

enum ctp_function {
    CTP_NONE,
    CTP_FORWARD,
    CTP_REPLY
};

// Test memory manager options
#define CTP_OPT_HEAP           0x01   // Allocate link_out() frame from heap
#define CTP_OPT_NON_ALIGNED    0x02   // Force memory buffers to be non-aligned

/* Builds and sends CTP message. Forward to address is the address where echo server sends the reply.
   Default is own Ethernet MAC address. Options can be used to specify test memory manager options.
 */
#define CTP_MSG_SEND(length, send_to_address, own_address, forward_to_address, mem_mngr_options) \
    emac_if_ctp_msg_build(length, send_to_address, own_address, forward_to_address, mem_mngr_options)

ctp_function emac_if_ctp_header_handle(unsigned char *eth_input_frame, unsigned char *eth_output_frame, unsigned char *origin_addr, int *receipt_number);
void emac_if_ctp_msg_build(int eth_frame_len, const unsigned char *dest_addr, const unsigned char *origin_addr, const unsigned char *forward_addr, int options);
void emac_if_ctp_reply_handle(int lenght, int invalid_data_index);

#endif /* EMAC_CTP_H */
