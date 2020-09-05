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
#if defined(MBED_CONF_RTOS_PRESENT)

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "mbed.h"

#include "EMAC.h"
#include "EMACMemoryManager.h"
#include "emac_TestMemoryManager.h"

#include "emac_initialize.h"
#include "emac_membuf.h"
#include "emac_util.h"

int emac_if_memory_buffer_read(void *buf, unsigned char *eth_frame)
{
    int eth_frame_index = 0;
    int invalid_data_index = 0;

    for (emac_mem_buf_t *mem_buf = buf; mem_buf != NULL; mem_buf = emac_m_mngr_get()->get_next(mem_buf)) {
        unsigned char *buf_payload = (unsigned char *) emac_m_mngr_get()->get_ptr(mem_buf);
        int buf_payload_len = emac_m_mngr_get()->get_len(mem_buf);

        for (int index = 0; index < buf_payload_len; index++) {
            if (eth_frame_index < ETH_FRAME_HEADER_LEN) {
                eth_frame[eth_frame_index] = buf_payload[index];
            } else {
                if (buf_payload[index] != (uint8_t) eth_frame_index) {
                    invalid_data_index = eth_frame_index;
                    break;
                }
            }
            eth_frame_index++;
        }
    }

    return invalid_data_index;
}

void emac_if_memory_buffer_write(void *buf, unsigned char *eth_frame, bool write_data)
{
    int eth_frame_index = 0;

    for (emac_mem_buf_t *mem_buf = buf; mem_buf != NULL; mem_buf = emac_m_mngr_get()->get_next(mem_buf)) {
        unsigned char *buf_payload = (unsigned char *) emac_m_mngr_get()->get_ptr(mem_buf);
        int buf_payload_len = emac_m_mngr_get()->get_len(mem_buf);

        for (int index = 0; index < buf_payload_len; index++) {
            if (eth_frame_index < ETH_FRAME_HEADER_LEN) {
                buf_payload[index] = eth_frame[eth_frame_index];
            } else if (write_data) {
                buf_payload[index] = (char) eth_frame_index;
            } else {
                break;
            }
            eth_frame_index++;
        }
    }
}
#endif // defined(MBED_CONF_RTOS_PRESENT)
