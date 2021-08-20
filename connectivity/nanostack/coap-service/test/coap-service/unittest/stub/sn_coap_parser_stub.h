/*
 * Copyright (c) 2015-2017, Pelion and affiliates.
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

#ifndef __SN_COAP_PARSER_STUB_H__
#define __SN_COAP_PARSER_STUB_H__

#include "sn_coap_header.h"

typedef struct {
    sn_coap_hdr_s *expectedHeader;
} sn_coap_parser_def;

extern sn_coap_parser_def sn_coap_parser_stub;

#endif //__SN_COAP_PARSER_STUB_H__
