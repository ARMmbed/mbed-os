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
#include "Security/PANA/pana_avp.h"

#define ZIGBEE_VENDOR_ID 0x0000917c


uint8_t *pana_avp_base_write(uint8_t avp_type, uint16_t length, uint8_t *dptr, uint16_t flags, uint32_t vendor_id)
{
    return dptr;
}

uint8_t *pana_avp_32_bit_write(uint8_t avp_type, uint32_t value, uint8_t *dptr)
{
    return dptr;
}



uint8_t *pana_avp_write_n_bytes(uint16_t avp_type, uint16_t length, const uint8_t *value, uint8_t *dptr)
{

    return  dptr;
}

uint8_t *pana_avp_vendor_id_write_n_bytes(uint16_t avp_type, uint16_t length, const uint8_t *value, uint8_t *dptr, uint32_t vendor_id)
{
    return  dptr;

}

bool pana_avp_discover(uint8_t *dptr, uint16_t data_len, pana_avp_t *avp)
{
    return false;
}
