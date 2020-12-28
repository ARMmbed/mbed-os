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

#ifndef PANA_HEADER_H_
#define PANA_HEADER_H_

/****************************************
 * Pana Header Base
 * Reserved     Length  FlaGS   Type    Session ID  Sequncy Numb
 * 0x00 0x00    16-bit  16-bit  16-bit  32-bit      32-bit
 *
 */

#define PANA_HEADER_LENGTH 16

/**
 * Pana Message types
 */
#define PANA_MSG_PCI 0x0001
#define PANA_MSG_PA 0x0002
#define PANA_MSG_PNA 0x0004
#define PANA_MSG_RELAY 0x0005

/**
 * Pana Message flags
 */
#define PANA_FLAGS_REQUEST          0x8000
#define PANA_FLAGS_RESPONSE         0x0000
#define PANA_FLAGS_START            0x4000
#define PANA_FLAGS_COMPLETE         0x2000
#define PANA_FLAG_START_REQ_MASK    (PANA_FLAGS_REQUEST | PANA_FLAGS_START)
#define PANA_FLAGS_PING_RESPONSE    0x0000
#define PANA_FLAGS_PING             0x0800

typedef struct {
    uint16_t flags;
    uint16_t type;
    uint16_t payload_len;
    uint32_t session_id;
    uint32_t seq;
    bool agent_retry;
} pana_header_t;

bool pana_header_parse(uint8_t *ptr, uint16_t data_length, pana_header_t *header);
uint8_t *pana_header_write(uint8_t *ptr, const pana_header_t *header);

#endif /* PANA_HEADER_H_ */
