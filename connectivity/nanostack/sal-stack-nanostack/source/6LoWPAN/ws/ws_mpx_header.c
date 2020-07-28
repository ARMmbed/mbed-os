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
#include "common_functions.h"
#include "mac_common_defines.h"
#include "ws_mpx_header.h"

bool ws_llc_mpx_header_frame_parse(uint8_t *ptr, uint16_t length, mpx_msg_t *msg)
{
    if (!length) {
        return false;
    }
    memset(msg, 0, sizeof(mpx_msg_t));
    bool fragmented_number_present = false;
    bool multiplex_id_present = false;
    bool fragment_total_size = false;

    msg->transfer_type = *ptr & 7;
    msg->transaction_id = ((*ptr++ & 0xf8) >> 3);
    length--;


    switch (msg->transfer_type) {
        case MPX_FT_FULL_FRAME:
            multiplex_id_present = true;
            break;
        case MPX_FT_FULL_FRAME_SMALL_MULTILEX_ID:
            break;
        case MPX_FT_FIRST_OR_SUB_FRAGMENT:
        case MPX_FT_LAST_FRAGMENT:
            fragmented_number_present = true;
            if (length < 2) {
                return false;
            }
            break;
        case MPX_FT_ABORT:
            if (length == 2) {
                fragment_total_size = true;
            } else if (length) {
                return false;
            }
            break;
        default:
            return false;
    }

    if (fragmented_number_present) {

        msg->fragment_number = *ptr++;
        length--;
        if (msg->fragment_number == 0) { //First fragment
            fragment_total_size = true;
            multiplex_id_present = true;
        }
    }

    if (fragment_total_size) {
        if (length < 2) {
            return false;
        }
        msg->total_upper_layer_size = common_read_16_bit_inverse(ptr);
        ptr += 2;
        length -= 2;
    }

    if (multiplex_id_present) {
        if (length < 3) {
            return false;
        }
        msg->multiplex_id = common_read_16_bit_inverse(ptr);
        ptr += 2;
        length -= 2;
    }

    msg->frame_ptr = ptr;
    msg->frame_length = length;
    return true;
}


uint8_t *ws_llc_mpx_header_write(uint8_t *ptr, const mpx_msg_t *msg)
{

    bool fragmented_number_present = false;
    bool multiplex_id_present = false;
    bool fragment_total_size = false;

    *ptr = msg->transfer_type;
    *ptr++ |= ((msg->transaction_id << 3) & 0xf8);

    switch (msg->transfer_type) {
        case MPX_FT_FULL_FRAME:
            multiplex_id_present = true;
            break;
        case MPX_FT_FULL_FRAME_SMALL_MULTILEX_ID:
            break;
        case MPX_FT_FIRST_OR_SUB_FRAGMENT:
        case MPX_FT_LAST_FRAGMENT:
            fragmented_number_present = true;
            if (msg->fragment_number == 0) {
                fragment_total_size = true;
                multiplex_id_present = true;
            }
            break;
        case MPX_FT_ABORT:
            if (msg->total_upper_layer_size) {
                fragment_total_size = true;
            }
            break;
        default:
            break;
    }

    if (fragmented_number_present) {
        *ptr++ = msg->fragment_number;
    }

    if (fragment_total_size) {
        ptr = common_write_16_bit_inverse(msg->total_upper_layer_size, ptr);
    }

    if (multiplex_id_present) {
        ptr = common_write_16_bit_inverse(msg->multiplex_id, ptr);
    }
    return ptr;
}
