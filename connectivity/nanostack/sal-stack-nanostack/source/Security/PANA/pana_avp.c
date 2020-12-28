/*
 * Copyright (c) 2017-2018, Arm Limited and affiliates.
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
#include "Security/PANA/pana_avp.h"
#ifdef PANA

uint8_t *pana_avp_base_write(uint8_t avp_type, uint16_t length, uint8_t *dptr, uint16_t flags, uint32_t vendor_id)
{
    dptr = common_write_16_bit(avp_type, dptr); //AVP Type
    dptr = common_write_16_bit(flags, dptr); //FLAGS
    dptr = common_write_16_bit(length, dptr); //LEN
    dptr = common_write_16_bit(0, dptr); //RESERVED
    if (flags & PANA_EAP_VENDOR_FLAG) {
        dptr = common_write_32_bit(vendor_id, dptr); //SET VENDOR
    }
    return dptr;
}

uint8_t *pana_avp_32_bit_write(uint8_t avp_type, uint32_t value, uint8_t *dptr)
{
    dptr = pana_avp_base_write(avp_type, 4, dptr, 0, 0);
    return common_write_32_bit(value, dptr);
}


uint8_t *pana_avp_write_n_bytes(uint16_t avp_type, uint16_t length, const uint8_t *value, uint8_t *dptr)
{
    dptr = pana_avp_base_write(avp_type, length, dptr, 0, 0);
    if (value) {
        memcpy(dptr, value, length);
    } else {
        memset(dptr, 0, length);
    }
    dptr += length;
    //Padding
    while (length % 4) {
        *dptr++ = 0;
        length++;
    }
    return  dptr;
}

uint8_t *pana_avp_vendor_id_write_n_bytes(uint16_t avp_type, uint16_t length, const uint8_t *value, uint8_t *dptr, uint32_t vendor_id)
{
    dptr = pana_avp_base_write(avp_type, length, dptr, PANA_EAP_VENDOR_FLAG, vendor_id);
    if (value) {
        memcpy(dptr, value, length);
    } else {
        memset(dptr, 0, length);
    }
    dptr += length;

    while (length % 4) {
        *dptr++ = 0;
        length++;
    }
    return  dptr;

}

bool pana_avp_discover(uint8_t *dptr, uint16_t data_len, pana_avp_t *avp)
{
    uint16_t avp_code;
    uint16_t readed = 0, pana_temp_var_16 = 0;
    avp->avp_ptr = NULL;
    while (readed  < data_len) {
        avp->flags = 0;
        avp->vendor_id = 0;
        avp_code = common_read_16_bit(dptr);
        dptr += 2;
        avp->flags = common_read_16_bit(dptr);
        dptr += 2;
        pana_temp_var_16 = common_read_16_bit(dptr);
        if (pana_temp_var_16 == 0) {
            return false;
        }
        dptr += 4; //Update pointer and Skip Reserved
        avp->len = pana_temp_var_16;

        if (avp->flags & PANA_EAP_VENDOR_FLAG) {
            avp->vendor_id = common_read_32_bit(dptr);
            dptr += 4;
            readed += 4;
        }
        readed += 8;
        readed += pana_temp_var_16;
        //Set Data Start here already
        avp->avp_ptr = dptr;
        dptr += pana_temp_var_16;

        //Check division remainder
        while (pana_temp_var_16 % 4) {
            dptr++;
            readed++;
            pana_temp_var_16++;
        }

        if (readed > data_len) {
            avp->avp_ptr = NULL;
            return false;
        }

        if (avp_code == avp->code) {
            //Save AVP pointer
            return true;
        }
        avp->avp_ptr = NULL;

    }
    return false;

}
#endif

