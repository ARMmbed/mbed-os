/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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
#include "ns_types.h"
#include "common_functions.h"
#include "Security/PANA/pana_header.h"

#ifdef PANA
#define PANA_HEADER_START 0x0000



bool pana_header_parse(uint8_t *ptr, uint16_t data_length, pana_header_t *header)
{
    if (data_length < 16) {
        return false;
    }

    if (common_read_16_bit(ptr) != PANA_HEADER_START) {
        return false;
    }

    header->payload_len = common_read_16_bit(ptr + 2);
    ptr += 4;
    if (header->payload_len != data_length) {
        return false;
    }

    header->flags = common_read_16_bit(ptr);
    header->type = common_read_16_bit(ptr + 2);
    ptr += 4;
    header->session_id = common_read_32_bit(ptr);
    ptr += 4;
    header->seq = common_read_32_bit(ptr);

    return true;
}

uint8_t *pana_header_write(uint8_t *ptr, const pana_header_t *header)
{
    ptr = common_write_16_bit(PANA_HEADER_START, ptr);
    ptr = common_write_16_bit(header->payload_len, ptr);
    ptr = common_write_16_bit(header->flags, ptr);
    ptr = common_write_16_bit(header->type, ptr);
    ptr = common_write_32_bit(header->session_id, ptr);
    return common_write_32_bit(header->seq, ptr);
}
#endif
