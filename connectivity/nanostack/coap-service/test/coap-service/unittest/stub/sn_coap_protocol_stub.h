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

#ifndef __SN_COAP_PROTOCOL_STUB_H__
#define __SN_COAP_PROTOCOL_STUB_H__

#include "sn_coap_header_internal.h"
#include "sn_coap_protocol_internal.h"

typedef struct {
    int8_t expectedInt8;
    int16_t expectedInt16;
    struct coap_s *expectedCoap;
    sn_coap_hdr_s *expectedHeader;
    coap_send_msg_s *expectedSendMsg;
} sn_coap_protocol_stub_def;

extern sn_coap_protocol_stub_def sn_coap_protocol_stub;

#endif //__SN_COAP_PROTOCOL_STUB_H__
