/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * \file thread_network_data_lib.c
 * \brief Add short description about this file!!!
 *
 */
#include "nsconfig.h"
#ifdef HAVE_THREAD
#include "thread_tmfcop_lib.h"
#include <string.h>
#include <ns_types.h>
#include <nsdynmemLIB.h>
#include "common_functions.h"
#include "ns_trace.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_joiner_application.h"
#include "6LoWPAN/Thread/thread_network_data_lib.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#include "6LoWPAN/Thread/thread_management_client.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"

#define TRACE_GROUP "tndl"

/**
 * Claculate Prefix bits to bytes count
 *
 * \param prefixLenInBits len in bits
 *
 * return Byte length
 */
uint8_t prefixBits_to_bytes(uint8_t prefixLenInBits)
{
    uint8_t prefix_len = 0;
    if (prefixLenInBits) {
        prefix_len = (prefixLenInBits / 8);
        if (prefixLenInBits % 8) {
            prefix_len++;
        }
    }
    return prefix_len;
}

static int thread_network_data_prefix_sub_tlv_malformed_check(uint8_t *data_ptr, uint8_t data_length)
{
    uint8_t *dptr;
    uint8_t length;
    uint8_t type;
    uint8_t prefix_len;
    dptr = data_ptr;
    //SKIP Domain ID
    dptr++;
    data_length--;
    //Read Prefix len

    prefix_len = prefixBits_to_bytes(*dptr);
    prefix_len++; //Add
    if (prefix_len >= data_length) {
        return -1;
    }

    data_length -= prefix_len;
    dptr += prefix_len;


    while (data_length) {
        if (data_length >= 2) {
            type  = *dptr++;
            length = *dptr++;
            data_length -= 2;
            if (data_length >= length) {
                type &= THREAD_NWK_DATA_TYPE_MASK;
                if (type == THREAD_NWK_DATA_TYPE_6LOWPAN_ID) {
                    if (length != 2) {
                        return -1;
                    }
                } else if (type == THREAD_NWK_DATA_TYPE_ROUTE) {
                    if (length == 0) {
                        return -1;
                    } else if ((length % 3) != 0) {
                        tr_debug("Has Route LengthF: %x", length % 3);
                        return -1;
                    }
                } else if (type == THREAD_NWK_DATA_TYPE_BORDER_ROUTER) {
                    if (length == 0) {
                        return -1;
                    } else if ((length % 4) != 0) {
                        tr_debug("BorderRouter LengthF: %x", length % 4);
                        return -1;
                    }
                } else {
                    tr_debug("TyF: %x", type);
                    return -1;
                }
                data_length -= length;
                dptr += length;
            } else {
                // buffer is overrun this is malformed.
                tr_debug("l!! %x < %x", data_length, length);
                return -1;
            }
        } else {
            tr_debug("MainLF: %x", data_length);
            return -1;
        }
    }
    return 0;
}

int thread_network_data_malformed_check(uint8_t *network_data_ptr, uint16_t network_data_length)
{
    uint8_t *dptr;
    uint8_t length;
    uint8_t type;
    dptr = network_data_ptr;
    while (network_data_length) {
        if (network_data_length >= 2) {
            type  = *dptr++;
            length = *dptr++;

            if (length == 0) {
                // 0 is not valid length for TLV
                return -1;
            }
            network_data_length -= 2;
            if (network_data_length >= length) {
                if (type == THREAD_NWK_DATA_TYPE_PREFIX) {
                    //Do subTLV Malformed test
                    if (thread_network_data_prefix_sub_tlv_malformed_check(dptr, length) != 0) {
                        tr_debug("prefixSubMal");
                        return -1;
                    }
                }
                network_data_length -= length;
                dptr += length;
            } else {
                // buffer is overrun this is malformed.
                tr_error("Lenf fail");
                return -1;
            }
        } else {
            return -1;
        }
    }
    return 0;
}

int thread_network_data_sub_tlv_malformed_check(uint8_t *network_data_ptr, uint8_t network_data_length)
{
    uint8_t *dptr;
    uint8_t length;

    if (!network_data_ptr) {
        return -1;
    }

    dptr = network_data_ptr;
    while (network_data_length) {
        if (network_data_length >= 2) {
            dptr++;
            length = *dptr++;
            network_data_length -= 2;
            if (length) {
                if (network_data_length >= length) {
                    network_data_length -= length;
                    dptr += length;
                } else {
                    // buffer is overrun this is malformed.
                    tr_error("Sub Tlv Length fail");
                    return -1;
                }
            }
        } else {
            return -1;
        }
    }
    return 0;
}


uint8_t *thread_nd_commission_data_write_steering_data(uint8_t *ptr, const uint8_t *steeringData, uint8_t length)
{
    if (length) {
        *ptr++ = THREAD_TLV_STEERING_DATA;
        *ptr++ = length;

        memcpy(ptr, steeringData, length);
        ptr += length;
    }
    return ptr;
}

uint8_t *thread_nd_network_data_border_router_tlv_write(uint8_t *ptr, uint16_t routerId, uint16_t flags)
{
    ptr = common_write_16_bit(routerId, ptr);
    ptr = common_write_16_bit(flags, ptr);
    return ptr;
}

uint8_t *thread_nd_network_data_has_route_tlv_write(uint8_t *ptr, uint16_t routerId, uint8_t prf)
{
    ptr = common_write_16_bit(routerId, ptr);
    *ptr++ = prf;
    return ptr;
}

#endif
