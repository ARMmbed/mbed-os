/*
 * Copyright (c) 2014-2018, Pelion and affiliates.
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
 * \file thread_network_data_lib.h
 * \brief Add short description about this file!!!
 *
 */

#ifndef THREAD_NETWOR_DATA_LIB_H_
#define THREAD_NETWOR_DATA_LIB_H_
#include "ns_types.h"

/* Thread Network Data TLV types  */
#define THREAD_NWK_DATA_TYPE_ROUTE (0 << 1)
#define THREAD_NWK_DATA_TYPE_PREFIX (1 << 1)
#define THREAD_NWK_DATA_TYPE_BORDER_ROUTER  (2 << 1)
#define THREAD_NWK_DATA_TYPE_6LOWPAN_ID (3 << 1)
#define THREAD_NWK_DATA_TYPE_COMMISSION_DATA (4 << 1)
#define THREAD_NWK_DATA_TYPE_SERVICE_DATA (5 << 1)
#define THREAD_NWK_DATA_TYPE_SERVER_DATA (6 << 1)

#define THREAD_NWK_STABLE_DATA  0x01
#define THREAD_NWK_DATA_TYPE_MASK  0xfe

#define THREAD_NWK_CONTEXT_COMPRESS_ENABLED  0x10

/* Bit shift for BorderRouterTLV (16-bit)*/
#define THREAD_PRF_BIT_MOVE             14  /* Prf-bits */
#define THREAD_P_PREFERRED_BIT_MOVE     13  /* P-bit */
#define THREAD_P_SLAAC_BIT_MOVE         12  /* S-bit */
#define THREAD_P_DHCP_BIT_MOVE          11  /* D-bit */
#define THREAD_P_CONFIGURE_BIT_MOVE     10  /* C-bit */
#define THREAD_P_DEF_ROUTE_BIT_MOVE     9   /* R-bit */
#define THREAD_P_ON_MESH_BIT_MOVE       8   /* O-bit */
#define THREAD_P_ND_DNS_BIT_MOVE        7   /* N-bit */
#define THREAD_P_ND_RES_BIT_MOVE        6   /* First reserved bit */

/* Bit shift for HasRouteTLV preference bit */
#define THREAD_HAS_ROUTE_PRF_BIT_MOVE   6   /* Prf-bits */

#define THREAD_TLV_NETWORK_NAME 3
#define THREAD_TLV_COMMISSIONING_CREDENTIALS    4
#define THREAD_TLV_COMMISSIONER_ID  10
#define THREAD_TLV_COMMISSIONER_SESSION_ID  11
#define THREAD_TLV_SECURITY_POLICY          12
#define THREAD_TLV_COMMISSIONING_TIME_STAMP 14
#define THREAD_TLV_JOINER_UDP_PORT          18
#define THREAD_TLV_STEERING_DATA            8   //Optional
#define THREAD_TLV_BORDER_ROUTER_LOCATOR    9           //optional

typedef struct thread_network_data_tlv_buf_s {
    uint8_t   *dataPtr;  /** Allocated Pointer which need to be freed by library user  */
    uint8_t     length; /** Allocated Pointer data length  */
} thread_network_data_tlv_buf_t;

typedef struct thread_prefix_tlv_params_s {
    uint8_t   *routePrefixPtr;
    uint8_t     prefixLength;
} thread_prefix_tlv_params_t;

typedef struct thread_network_router_info_s {
    bool        stableData;
    uint16_t    router16BitSddress;
} thread_network_router_info_s;

typedef struct thread_network_context_info_s {
    bool stableData: 1;
    bool compress_supported: 1;
    unsigned cid: 4;
    uint8_t context_length;
} thread_network_context_info_t;




/**
 * Verify Network Data TLV data structure
 *
 * \param network_data_ptr pointer to Network Data TLV
 * \param network_data_length Data Length
 *
 * return 0, TLV  OK
 * return <0 TLV Malformed
 */
int thread_network_data_malformed_check(uint8_t *network_data_ptr, uint16_t network_data_length);

int thread_network_data_sub_tlv_malformed_check(uint8_t *network_data_ptr, uint8_t network_data_length);

/**
 * Claculate Prefix bits to bytes count
 *
 * \param prefixLenInBits len in bits
 *
 * return Byte length
 */
uint8_t prefixBits_to_bytes(uint8_t prefixLenInBits);

uint8_t *thread_nd_commission_data_write_steering_data(uint8_t *ptr, const uint8_t *steeringData, uint8_t length);

uint8_t *thread_nd_network_data_border_router_tlv_write(uint8_t *ptr, uint16_t routerId, uint16_t flags);

uint8_t *thread_nd_network_data_has_route_tlv_write(uint8_t *ptr, uint16_t routerId, uint8_t prf);

#endif /* THREAD_NETWOR_DATA_LIB_H_ */
