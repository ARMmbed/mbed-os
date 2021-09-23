/*
 * Copyright (c) 2020, Pelion and affiliates.
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
#include "eventOS_event.h"
#include "ns_trace.h"
#include "string.h"
#include "common_functions.h"
#include "Security/protocols/radius_sec_prot/avp_helper.h"

#ifdef HAVE_WS

#define TRACE_GROUP "avp"

// RFC 2865

// 1 User-Name
#define AVP_TYPE_USER_NAME              1
// 4 for NAS-IP-Address
#define AVP_TYPE_NAS_IP_ADDRESS         4
// 5 NAS-Port
#define AVP_TYPE_NAS_PORT               5
// 12 Framed-MTU
#define AVP_TYPE_FRAMED_MTU             12
// 24 State
#define AVP_TYPE_STATE                  24
// 26 Vendor-Specific
#define AVP_TYPE_VENDOR_SPECIFIC        26
// 30 Called-Station-Id
#define AVP_TYPE_CALLED_STATION_ID      30
// 31 Calling-Station-Id
#define AVP_TYPE_CALLING_STATION_ID     31
// 32 NAS-Identifier
#define AVP_TYPE_NAS_IDENTIFIER         32
// 61 NAS-Port-Type
#define AVP_TYPE_NAS_PORT_TYPE          61

// RFC 3579

// 79 EAP-Message
#define AVP_TYPE_EAP_MESSAGE            79
// 80 Message-Authenticator
#define AVP_TYPE_MESSAGE_AUTHENTICATOR  80

// RFC 3162

// 95 NAS-IPv6-Address
#define AVP_TYPE_NAS_IPV6_ADDRESS       95


static uint8_t *avp_header_write(uint8_t *ptr, const uint8_t type, const uint8_t data_length)
{
    *ptr++ = type;
    *ptr++ = data_length + AVP_FIXED_LEN;

    return ptr;
}

static uint8_t *avp_search(uint8_t *ptr, uint16_t len, const uint8_t type, uint8_t *avp_len)
{
    while (len >= AVP_FIXED_LEN) {
        *avp_len = ptr[1];

        // Validates length field
        if (*avp_len > len) {
            return NULL;
        }

        if (ptr[0] == type) {
            return ptr + AVP_FIXED_LEN;
        }

        if (len > *avp_len) {
            len -= *avp_len;
            ptr += *avp_len;
        } else {
            return NULL;
        }
    }

    return NULL;
}

static uint8_t *avp_vpa_search(uint8_t *ptr, uint16_t len, const uint32_t vendor_id, const uint8_t vendor_type, uint8_t *vendor_len)
{
    uint8_t avp_len = 0;

    while (len >= AVP_FIXED_LEN) {
        avp_len = ptr[1];

        // Validates length field
        if (avp_len > len) {
            return NULL;
        }

        if (ptr[0] == AVP_TYPE_VENDOR_SPECIFIC && avp_len >= 9) {
            ptr[2] = 0;
            uint32_t avp_vendor_id = common_read_32_bit(&ptr[2]);
            *vendor_len = ptr[7];
            if (avp_vendor_id == vendor_id && ptr[6] == vendor_type) {
                return &ptr[8];
            }
        }

        if (len > avp_len) {
            len -= avp_len;
            ptr += avp_len;
        } else {
            return NULL;
        }
    }

    return NULL;
}

uint8_t *avp_user_name_write(uint8_t *ptr, const uint8_t name_len, const uint8_t *name)
{
    ptr = avp_header_write(ptr, AVP_TYPE_USER_NAME, name_len);
    memcpy(ptr, name, name_len);
    return ptr + name_len;
}

uint8_t *avp_nas_ip_address_write(uint8_t *ptr, uint32_t addr)
{
    ptr = avp_header_write(ptr, AVP_TYPE_NAS_IP_ADDRESS, 4);
    memcpy(ptr, &addr, 4);
    return ptr + 4;
}

uint8_t *avp_nas_port_write(uint8_t *ptr, const uint32_t port)
{
    ptr = avp_header_write(ptr, AVP_TYPE_NAS_PORT, 4);
    ptr = common_write_32_bit(port, ptr);
    return ptr;
}

uint8_t *avp_framed_mtu_write(uint8_t *ptr, const uint32_t mtu)
{
    ptr = avp_header_write(ptr, AVP_TYPE_FRAMED_MTU, 4);
    ptr = common_write_32_bit(mtu, ptr);
    return ptr;
}

uint8_t *avp_state_write(uint8_t *ptr, const uint8_t state_len, const uint8_t *state)
{
    ptr = avp_header_write(ptr, AVP_TYPE_STATE, state_len);
    memcpy(ptr, state, state_len);
    return ptr + state_len;
}

uint8_t *avp_called_station_id_write(uint8_t *ptr, const uint8_t id_len, const uint8_t *id)
{
    ptr = avp_header_write(ptr, AVP_TYPE_CALLED_STATION_ID, id_len);
    memcpy(ptr, id, id_len);
    return ptr + id_len;
}

uint8_t *avp_calling_station_id_write(uint8_t *ptr, const uint8_t id_len, const uint8_t *id)
{
    ptr = avp_header_write(ptr, AVP_TYPE_CALLING_STATION_ID, id_len);
    memcpy(ptr, id, id_len);
    return ptr + id_len;
}

uint8_t *avp_nas_identifier_write(uint8_t *ptr, const uint8_t id_len, const uint8_t *id)
{
    ptr = avp_header_write(ptr, AVP_TYPE_NAS_IDENTIFIER, id_len);
    memcpy(ptr, id, id_len);
    return ptr + id_len;
}

uint8_t *avp_nas_port_type_write(uint8_t *ptr, const uint32_t port_type)
{
    ptr = avp_header_write(ptr, AVP_TYPE_NAS_PORT_TYPE, 4);
    ptr = common_write_32_bit(port_type, ptr);
    return ptr;
}

uint8_t *avp_eap_message_write(uint8_t *ptr, const uint8_t eap_len, const uint8_t *eap)
{
    ptr = avp_header_write(ptr, AVP_TYPE_EAP_MESSAGE, eap_len);
    memcpy(ptr, eap, eap_len);
    return ptr + eap_len;
}

uint8_t *avp_message_authenticator_write(uint8_t *ptr, const uint8_t *auth)
{
    ptr = avp_header_write(ptr, AVP_TYPE_MESSAGE_AUTHENTICATOR, 16);
    memcpy(ptr, auth, 16);
    return ptr + 16;
}

uint8_t *avp_nas_ipv6_address_write(uint8_t *ptr, const uint8_t *address)
{
    ptr = avp_header_write(ptr, AVP_TYPE_NAS_IPV6_ADDRESS, 16);
    memcpy(ptr, address, 16);
    return ptr + 16;
}

uint8_t *avp_eap_message_read(uint8_t *ptr, uint16_t len, uint8_t *eap_len)
{
    ptr = avp_search(ptr, len, AVP_TYPE_EAP_MESSAGE, eap_len);
    if (ptr == NULL) {
        return NULL;
    }
    return ptr;
}

uint8_t *avp_message_authenticator_read(uint8_t *ptr, uint16_t len)
{
    uint8_t auth_len = 0;
    ptr = avp_search(ptr, len, AVP_TYPE_MESSAGE_AUTHENTICATOR, &auth_len);
    if (ptr == NULL) {
        return NULL;
    }
    if (auth_len < 18) {
        return NULL;
    }
    return ptr;
}

uint8_t *avp_state_read(uint8_t *ptr, uint16_t len, uint8_t *state_len)
{
    ptr = avp_search(ptr, len, AVP_TYPE_STATE, state_len);
    if (ptr == NULL) {
        return NULL;
    }
    return ptr;
}

uint8_t *avp_vsa_ms_mppe_recv_key_read(uint8_t *ptr, uint16_t len, uint8_t *recv_key_len)
{
    const uint32_t vendor_id = 311;
    const uint8_t vendor_type = 17;

    ptr = avp_vpa_search(ptr, len, vendor_id, vendor_type, recv_key_len);
    if (ptr == NULL) {
        return NULL;
    }
    return ptr;
}

#endif

