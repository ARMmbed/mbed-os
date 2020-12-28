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

#ifndef WS_MPX_HEADER_H_
#define WS_MPX_HEADER_H_

#define MPX_FT_FULL_FRAME                   0
#define MPX_FT_FULL_FRAME_SMALL_MULTILEX_ID 1
#define MPX_FT_FIRST_OR_SUB_FRAGMENT        2
#define MPX_FT_LAST_FRAGMENT                4
#define MPX_FT_ABORT                        6

typedef struct {
    unsigned    transfer_type: 3;
    unsigned    transaction_id: 5;
    uint8_t     fragment_number;
    uint16_t    total_upper_layer_size;
    uint16_t    multiplex_id;
    uint8_t     *frame_ptr;
    uint16_t    frame_length;
} mpx_msg_t;

bool ws_llc_mpx_header_frame_parse(uint8_t *ptr, uint16_t length, mpx_msg_t *msg);
uint8_t *ws_llc_mpx_header_write(uint8_t *ptr, const mpx_msg_t *msg);


#endif /* WS_MPX_HEADER_H_ */
