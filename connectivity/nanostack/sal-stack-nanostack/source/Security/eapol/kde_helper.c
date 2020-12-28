/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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
#include "Security/eapol/kde_helper.h"

#ifdef HAVE_WS

#define TRACE_GROUP "kdeh"

#define IEEE_802_11_OUI          ieee_802_11_oui
#define WISUN_OUI                wisun_oui

const uint8_t ieee_802_11_oui[3] = {0x00, 0x0F, 0xAC};
const uint8_t wisun_oui[3] = {0x0C, 0x5A, 0x9E};

#define KDE_TYPE    0xdd

// IEEE 802.11
#define KDE_GTK                  0x01
#define KDE_PMKID                0x04
#define KDE_LIFETIME             0x07
// Wi-Sun
#define KDE_PTKID                0x01
#define KDE_GTKL                 0x02

#define GTK_LEN                  16
#define PMKID_LEN                16
#define PTKID_LEN                16

#define KDE_MIN_LEN              6
#define KDE_FIXED_LEN            2

#define KDE_PADDED_MIN_LEN       16

#define KDE_TYPE_INDEX           0
#define KDE_LENGTH_INDEX         1
#define KDE_OUI1_INDEX           2
#define KDE_OUI2_INDEX           3
#define KDE_OUI3_INDEX           4
#define KDE_DATA_TYPE_INDEX      5
#define KDE_DATA_INDEX           6

/*
    From IEEE 802.11 chapter 11.6.2 EAPOL-Key frames

    If the Encrypted Key Data subfield (of the Key Information field) is 1,
    the entire Key Data field shall be encrypted. If the Key Data field
    uses the NIST AES key wrap, then the Key Data field shall be padded before
    encrypting if the key data length is less than 16 octets or if it is not
    a multiple of 8.
    The padding consists of appending a single octet 0xdd followed by zero
    or more 0x00 octets. When processing a received EAPOL-Key message, the
    receiver shall ignore this trailing padding.
*/
uint16_t kde_padded_length_calc(uint16_t kde_length)
{
    if (kde_length < KDE_PADDED_MIN_LEN) {
        return KDE_PADDED_MIN_LEN;
    }

    return ((kde_length + 7) / 8) * 8;
}

void kde_padding_write(uint8_t *start_ptr, uint8_t *end_ptr)
{
    uint8_t padding = 0xdd;

    while (start_ptr < end_ptr) {
        *start_ptr++ = padding;
        padding = 0x00;
    }
}

static uint8_t *kde_header_write(uint8_t *ptr, const uint8_t *oui, uint8_t data_type, uint8_t data_length)
{
    *ptr++ = KDE_TYPE;
    *ptr++ = data_length - KDE_FIXED_LEN;
    *ptr++ = oui[0];
    *ptr++ = oui[1];
    *ptr++ = oui[2];
    *ptr++ = data_type;

    return ptr;
}

static const uint8_t *kde_search(const uint8_t *ptr, uint16_t len, const uint8_t *oui, uint8_t data_type, uint16_t kde_min_len)
{
    while (len >= KDE_MIN_LEN) {
        uint16_t kde_len = ptr[1] + KDE_FIXED_LEN;

        // For the type 0xdd the length shall be at least 6 */
        if ((ptr[KDE_TYPE_INDEX] == 0xdd && kde_len < KDE_MIN_LEN) || kde_len > len) {
            return NULL;
        }

        if (kde_len >= kde_min_len && ptr[KDE_OUI1_INDEX] == oui[0] && ptr[KDE_OUI2_INDEX] == oui[1]
                && ptr[KDE_OUI3_INDEX] == oui[2] && ptr[KDE_DATA_TYPE_INDEX] == data_type) {
            return &ptr[KDE_DATA_INDEX];
        }

        if (len > kde_len) {
            len -= kde_len;
            ptr += kde_len;
        } else {
            return NULL;
        }
    }

    return NULL;
}

uint8_t *kde_gtk_write(uint8_t *ptr, uint8_t key_id, const uint8_t *gtk)
{
    ptr = kde_header_write(ptr, IEEE_802_11_OUI, KDE_GTK, KDE_GTK_LEN);

    // bits 0-1: keyid (0,1,2, or 3), bit 2: Tx, other: reserved (0)
    *ptr++ = key_id;
    *ptr++ = 0x00; // reserved
    memcpy(ptr, gtk, GTK_LEN);
    ptr += GTK_LEN;

    return ptr;
}

uint8_t *kde_pmkid_write(uint8_t *ptr, const uint8_t *pmkid)
{
    ptr = kde_header_write(ptr, IEEE_802_11_OUI, KDE_PMKID, KDE_PMKID_LEN);

    memcpy(ptr, pmkid, PMKID_LEN);
    ptr += PMKID_LEN;

    return ptr;
}

uint8_t *kde_ptkid_write(uint8_t *ptr, const uint8_t *ptkid)
{
    ptr = kde_header_write(ptr, WISUN_OUI, KDE_PTKID, KDE_PTKID_LEN);

    memcpy(ptr, ptkid, PTKID_LEN);
    ptr += PTKID_LEN;

    return ptr;
}

uint8_t *kde_lifetime_write(uint8_t *ptr, uint32_t lifetime)
{
    ptr = kde_header_write(ptr, IEEE_802_11_OUI, KDE_LIFETIME, KDE_LIFETIME_LEN);
    ptr = common_write_32_bit(lifetime, ptr);

    return ptr;
}

uint8_t *kde_gtkl_write(uint8_t *ptr, uint8_t gtkl)
{
    ptr = kde_header_write(ptr, WISUN_OUI, KDE_GTKL, KDE_GTKL_LEN);
    *ptr++ = gtkl;

    return ptr;
}

int8_t kde_gtk_read(const uint8_t *ptr, uint16_t len, uint8_t *key_id, uint8_t *gtk)
{
    ptr = kde_search(ptr, len, IEEE_802_11_OUI, KDE_GTK, KDE_GTK_LEN);
    if (ptr == NULL) {
        return -1;
    }

    *key_id = *ptr++ & 0x03;
    ptr++;
    memcpy(gtk, ptr, GTK_LEN);

    return 0;
}

int8_t kde_pmkid_read(const uint8_t *ptr, uint16_t len, uint8_t *pmkid)
{
    ptr = kde_search(ptr, len, IEEE_802_11_OUI, KDE_PMKID, KDE_PMKID_LEN);
    if (ptr == NULL) {
        return -1;
    }

    memcpy(pmkid, ptr, PMKID_LEN);

    return 0;
}

int8_t kde_ptkid_read(const uint8_t *ptr, uint16_t len, uint8_t *ptkid)
{
    ptr = kde_search(ptr, len, WISUN_OUI, KDE_PTKID, KDE_PTKID_LEN);
    if (ptr == NULL) {
        return -1;
    }

    memcpy(ptkid, ptr, PTKID_LEN);

    return 0;
}

int8_t kde_lifetime_read(const uint8_t *ptr, uint16_t len, uint32_t *lifetime)
{
    ptr = kde_search(ptr, len, IEEE_802_11_OUI, KDE_LIFETIME, KDE_LIFETIME_LEN);
    if (ptr == NULL) {
        return -1;
    }

    *lifetime = common_read_32_bit(ptr);

    return 0;
}

int8_t kde_gtkl_read(const uint8_t *ptr, uint16_t len, uint8_t *gtkl)
{
    ptr = kde_search(ptr, len, WISUN_OUI, KDE_GTKL, KDE_GTKL_LEN);
    if (ptr == NULL) {
        return -1;
    }

    *gtkl = *ptr;

    return 0;
}

#endif

