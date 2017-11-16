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
#include "string.h"
#include "common_functions.h"
#include "Security/PANA/pana_eap_header.h"
#ifdef PANA

bool eap_header_parse(uint8_t *data_ptr, uint16_t length, eap_header_t *header)
{
    return true;
}

uint8_t eap_header_size(uint8_t eap_code)
{
    return 5;
}

uint8_t *eap_header_build(uint8_t *ptr, uint16_t data_length, uint8_t eap_code, uint8_t id_seq, uint8_t type)
{
    return ptr;
}

bool eap_tls_header_parse(uint8_t *eap_data_ptr, uint16_t eap_datalength, eap_tls_header_t *header)
{
    return true;
}

uint8_t eap_tls_header_size(uint16_t eap_tls_flags)
{
    return 5;
}

uint8_t *eap_tls_header_build(uint8_t *ptr, uint16_t eap_tls_flags, uint16_t frame_length)
{
    return ptr;
}
#endif
