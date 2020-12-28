/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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
#include "mac_common_defines.h"
#include "mac_ie_lib.h"


#define MAC_IE_HEADER_LENGTH_MASK 0x007f
#define MAC_IE_HEADER_ID_MASK 0x7f80
#define MAC_IE_PAYLOAD_LENGTH_MASK 0x07ff
#define MAC_IE_PAYLOAD_ID_MASK 0x7800
#define MAC_IE_TYPE_PAYLOAD_MASK 0x8000

#define MAC_NESTED_LONG_IE_PAYLOAD_LENGTH_MASK 0x07ff
#define MAC_NESTED_LONG_IE_PAYLOAD_ID_MASK 0x7800
#define MAC_NESTED_SHORT_IE_PAYLOAD_LENGTH_MASK 0x00ff
#define MAC_NESTED_SHORT_IE_PAYLOAD_ID_MASK 0x7f00
#define MAC_NESTED_IE_TYPE_LONG_MASK 0x8000

static void mac_ie_header_parse(mac_header_IE_t *header_element, uint8_t *ptr)
{
    uint16_t ie_dummy = common_read_16_bit_inverse(ptr);
    header_element->length = (ie_dummy & MAC_IE_HEADER_LENGTH_MASK);
    header_element->id = ((ie_dummy & MAC_IE_HEADER_ID_MASK) >> 7);
    header_element->content_ptr = ptr + 2;
}

static void mac_ie_payload_parse(mac_payload_IE_t *payload_element, uint8_t *ptr)
{
    uint16_t ie_dummy = common_read_16_bit_inverse(ptr);
    payload_element->length = (ie_dummy & MAC_IE_PAYLOAD_LENGTH_MASK);
    payload_element->id = ((ie_dummy & MAC_IE_PAYLOAD_ID_MASK) >> 11);
    payload_element->content_ptr = ptr + 2;
}

static void mac_ie_nested_id_parse(mac_nested_payload_IE_t *element, uint8_t *ptr)
{
    uint16_t ie_dummy = common_read_16_bit_inverse(ptr);

    if (ie_dummy & MAC_NESTED_IE_TYPE_LONG_MASK) {
        element->type_long = true;
        element->length = (ie_dummy & MAC_NESTED_LONG_IE_PAYLOAD_LENGTH_MASK);
        element->id = ((ie_dummy & MAC_NESTED_LONG_IE_PAYLOAD_ID_MASK) >> 11);
    } else {
        element->type_long = false;
        element->length = (ie_dummy & MAC_NESTED_SHORT_IE_PAYLOAD_LENGTH_MASK);
        element->id = ((ie_dummy & MAC_NESTED_SHORT_IE_PAYLOAD_ID_MASK) >> 8);
    }

    element->content_ptr = ptr + 2;
}


uint8_t *mac_ie_header_base_write(uint8_t *ptr, uint8_t type, uint16_t length)
{
    uint16_t ie_dummy = 0; //Header Type
    ie_dummy |= (length & MAC_IE_HEADER_LENGTH_MASK);
    ie_dummy |= ((type << 7) &  MAC_IE_HEADER_ID_MASK);
    return common_write_16_bit_inverse(ie_dummy, ptr);
}

uint8_t *mac_ie_payload_base_write(uint8_t *ptr, uint8_t type, uint16_t length)
{

    uint16_t ie_dummy = MAC_IE_TYPE_PAYLOAD_MASK; //Payload type
    ie_dummy |= (length & MAC_IE_PAYLOAD_LENGTH_MASK);
    ie_dummy |= ((type << 11) &  MAC_IE_PAYLOAD_ID_MASK);
    return common_write_16_bit_inverse(ie_dummy, ptr);
}

uint8_t *mac_ie_nested_ie_long_base_write(uint8_t *ptr, uint8_t sub_id, uint16_t length)
{
    uint16_t ie_dummy = MAC_NESTED_IE_TYPE_LONG_MASK;
    ie_dummy |= (length & MAC_NESTED_LONG_IE_PAYLOAD_LENGTH_MASK);
    ie_dummy |= ((sub_id << 11) &  MAC_NESTED_LONG_IE_PAYLOAD_ID_MASK);

    return common_write_16_bit_inverse(ie_dummy, ptr);
}

uint8_t *mac_ie_nested_ie_short_base_write(uint8_t *ptr, uint8_t sub_id, uint16_t length)
{
    uint16_t ie_dummy = 0;
    ie_dummy |= (length & MAC_NESTED_SHORT_IE_PAYLOAD_LENGTH_MASK);
    ie_dummy |= ((sub_id << 8) &  MAC_NESTED_SHORT_IE_PAYLOAD_ID_MASK);

    return common_write_16_bit_inverse(ie_dummy, ptr);
}

uint16_t mac_ie_payload_discover(uint8_t *payload_ptr, uint16_t length, mac_payload_IE_t *payload_ie)
{
    mac_payload_IE_t ie_element;
    while (length >= 2) {
        mac_ie_payload_parse(&ie_element, payload_ptr);
        if (payload_ie->id == ie_element.id) {
            payload_ie->content_ptr = ie_element.content_ptr;
            payload_ie->length = ie_element.length;
            return ie_element.length;
        }

        length -= ie_element.length + 2;

        payload_ptr += ie_element.length + 2;
    }
    return 0;
}

uint16_t mac_ie_nested_discover(uint8_t *payload_ptr, uint16_t length, mac_nested_payload_IE_t *nested_ie)
{
    mac_nested_payload_IE_t ie_element;
    while (length >= 2) {
        mac_ie_nested_id_parse(&ie_element, payload_ptr);

        if (length < ie_element.length + 2) {
            return 0;
        }

        if (nested_ie->id == ie_element.id && nested_ie->type_long == ie_element.type_long) {
            nested_ie->content_ptr = ie_element.content_ptr;
            nested_ie->length = ie_element.length;
            return ie_element.length;
        }

        length -= ie_element.length + 2;

        payload_ptr += ie_element.length + 2;
    }
    return 0;
}

uint8_t mac_ie_header_discover(uint8_t *header_ptr, uint16_t length, mac_header_IE_t *header_ie)
{
    mac_header_IE_t ie_element;
    while (length >= 2) {
        mac_ie_header_parse(&ie_element, header_ptr);
        if (header_ie->id == ie_element.id) {
            header_ie->content_ptr = ie_element.content_ptr;
            header_ie->length = ie_element.length;
            return ie_element.length;
        }

        length -= ie_element.length + 2;

        header_ptr += ie_element.length + 2;
    }
    return 0;
}

uint8_t mac_ie_header_sub_id_discover(uint8_t *header_ptr, uint16_t length, mac_header_IE_t *header_ie, uint8_t sub_id)
{
    mac_header_IE_t ie_element;
    uint8_t *sub_id_ptr;
    while (length > 2) {
        mac_ie_header_parse(&ie_element, header_ptr);
        sub_id_ptr = ie_element.content_ptr;
        if (ie_element.length && header_ie->id == ie_element.id && *sub_id_ptr == sub_id) {
            sub_id_ptr++;
            ie_element.length--;
            header_ie->content_ptr = sub_id_ptr;
            header_ie->length = ie_element.length;
            return ie_element.length;
        }

        length -= ie_element.length + 2;

        header_ptr += ie_element.length + 2;
    }
    return 0;
}
