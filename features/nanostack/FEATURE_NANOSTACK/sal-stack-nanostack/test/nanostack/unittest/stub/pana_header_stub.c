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

bool pana_header_parse(uint8_t *ptr, uint16_t data_length, pana_header_t *header)
{
    return true;
}

uint8_t *pana_header_write(uint8_t *ptr, const pana_header_t *header)
{
    return ptr + 16;
}
#endif
