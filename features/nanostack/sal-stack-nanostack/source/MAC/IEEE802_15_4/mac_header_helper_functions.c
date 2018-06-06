/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "ns_types.h"
#include "string.h"
#include "mlme.h"
#include "mac_api.h"
#include "common_functions.h"
#include "mac_common_defines.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/IEEE802_15_4/mac_header_helper_functions.h"



uint8_t mac_security_mic_length_get(uint8_t security_level)
{
    uint8_t mic_length;
    switch (security_level) {
        case 1:
        case 5:
            mic_length = 4;
            break;
        case 2:
        case 6:
            mic_length = 8;
            break;
        case 3:
        case 7:
            mic_length = 16;
            break;
        default:
            mic_length = 0;
            break;
    }
    return mic_length;
}

uint8_t mac_header_security_aux_header_length(uint8_t security_level, uint8_t keyIdmode)
{
    if (security_level == 0) {
        return 0;
    }
    uint8_t header_length = 5; //Header + 32-bit counter
    switch (keyIdmode) {
        case MAC_KEY_ID_MODE_SRC8_IDX:
            header_length += 4; //64-bit key source first part
            /* fall through */
        case MAC_KEY_ID_MODE_SRC4_IDX:
            header_length += 4; //32-bit key source inline
            /* fall through */
        case MAC_KEY_ID_MODE_IDX:
            header_length += 1;
            break;
        default:

            break;
    }
    return header_length;
}

uint8_t mac_header_address_length(const mac_fcf_sequence_t *fcf)
{
    uint8_t address_length = 0;
    if( !fcf ){
        return address_length;
    }

    if(fcf->DstAddrMode && fcf->SrcAddrMode) {
        if (fcf->DstAddrMode == MAC_ADDR_MODE_16_BIT) {
            address_length = 4;
        } else {
            address_length = 10;
        }

        if (fcf->SrcAddrMode == MAC_ADDR_MODE_16_BIT) {
            address_length += 2;
        } else {
            address_length += 8;
        }

        if (!fcf->intraPan) {
            address_length += 2;
        }
    } else if (fcf->DstAddrMode) {
        if (fcf->DstAddrMode == MAC_ADDR_MODE_16_BIT) {
            address_length = 4;
        } else {
            address_length = 10;
        }
    } else if (fcf->SrcAddrMode){
        if (fcf->SrcAddrMode == MAC_ADDR_MODE_16_BIT) {
            address_length = 4;
        } else {
            address_length = 10;
        }
    }
    return address_length;

}

void mac_header_security_parameter_set(mac_aux_security_header_t *header, const mlme_security_t *frame_setup)
{
    if( !header || !frame_setup ){
        return;
    }
    header->securityLevel = frame_setup->SecurityLevel;

    if (header->securityLevel) {
        uint8_t keysource_len = 0;

        header->KeyIdMode = frame_setup->KeyIdMode;
        switch (header->KeyIdMode) {
            case MAC_KEY_ID_MODE_IMPLICIT:
                //Security header + 32-bit security counter
                break;

            case MAC_KEY_ID_MODE_SRC8_IDX:
                keysource_len += 4; //64-bit key source first part
                /* fall through */
            case MAC_KEY_ID_MODE_SRC4_IDX:
                keysource_len += 4; //32-bit key source inline
                /* fall through */
            case MAC_KEY_ID_MODE_IDX:
                //Security header + 32-bit security counter + Key index
                header->KeyIndex = frame_setup->KeyIndex;
                break;
        }
        if (keysource_len) {
            memcpy(header->Keysource, frame_setup->Keysource , keysource_len);
        }
    }
}

void mac_header_parse_fcf_dsn(mac_fcf_sequence_t *header, const uint8_t *ptr)
{
    if( !header || !ptr ){
        return;
    }
    uint16_t fcf = common_read_16_bit_inverse(ptr);
    ptr += 2;
    header->DSN = *ptr;

    //Read Frame Type
    header->frametype = ((fcf & MAC_FCF_FRAME_TYPE_MASK) >> MAC_FCF_FRAME_TYPE_SHIFT);
    header->securityEnabled = ((fcf & MAC_FCF_SECURITY_BIT_MASK) >> MAC_FCF_SECURITY_BIT_SHIFT);
    header->framePending = ((fcf & MAC_FCF_PENDING_BIT_MASK) >> MAC_FCF_PENDING_BIT_SHIFT);
    header->ackRequested = ((fcf & MAC_FCF_ACK_REQ_BIT_MASK) >> MAC_FCF_ACK_REQ_BIT_SHIFT);
    header->intraPan = ((fcf & MAC_FCF_INTRA_PANID_MASK ) >> MAC_FCF_INTRA_PANID_SHIFT);
    header->DstAddrMode = ((fcf & MAC_FCF_DST_ADDR_MASK ) >> MAC_FCF_DST_ADDR_SHIFT);
    header->frameVersion = ((fcf & MAC_FCF_VERSION_MASK) >> MAC_FCF_VERSION_SHIFT);
    header->SrcAddrMode = ((fcf & MAC_FCF_SRC_ADDR_MASK ) >> MAC_FCF_SRC_ADDR_SHIFT);
}

static uint8_t * mac_header_write_fcf_dsn(const mac_fcf_sequence_t *header, uint8_t *ptr)
{
    uint16_t fcf= 0;
    //Read Frame Type
    fcf |= (header->frametype << MAC_FCF_FRAME_TYPE_SHIFT);
    fcf |= (header->securityEnabled << MAC_FCF_SECURITY_BIT_SHIFT);
    fcf |= (header->framePending << MAC_FCF_PENDING_BIT_SHIFT);
    fcf |= (header->ackRequested << MAC_FCF_ACK_REQ_BIT_SHIFT);
    fcf |= (header->intraPan << MAC_FCF_INTRA_PANID_SHIFT);
    fcf |= (header->DstAddrMode << MAC_FCF_DST_ADDR_SHIFT);
    fcf |= (header->frameVersion << MAC_FCF_VERSION_SHIFT);
    fcf |= (header->SrcAddrMode << MAC_FCF_SRC_ADDR_SHIFT);
    ptr = common_write_16_bit_inverse(fcf,ptr);
    *ptr++ = header->DSN;
    return ptr;
}

void mac_header_security_components_read(mac_pre_parsed_frame_t *buffer, mlme_security_t *security_params)
{
    if( !buffer || !security_params ){
        return;
    }
    uint8_t *ptr = mcps_mac_security_aux_header_start_pointer_get(buffer);
    memset(security_params, 0, sizeof(mlme_security_t));
    uint8_t key_source_len = 0;
    if (!buffer->fcf_dsn.securityEnabled) {
        return;
    }

    security_params->KeyIdMode = (*ptr >> 3);
    security_params->SecurityLevel = *ptr++;
    ptr += 4;
    switch (security_params->KeyIdMode) {
        case MAC_KEY_ID_MODE_IMPLICIT:
            break;
        case MAC_KEY_ID_MODE_SRC8_IDX:
            key_source_len += 4;
            /* fall through */
        case MAC_KEY_ID_MODE_SRC4_IDX:
            key_source_len += 4;
            memcpy(security_params->Keysource, ptr, key_source_len);
            ptr += key_source_len;
            /* fall through */
        case MAC_KEY_ID_MODE_IDX:
            security_params->KeyIndex = *ptr;
            break;
    }
}

uint16_t mac_header_get_src_panid(const mac_fcf_sequence_t *header, const uint8_t *ptr)
{
    if( !header || !ptr ){
        return 0;
    }
    ptr += 3; //Skip FCF + DSN

    if (!header->intraPan) {
        switch (header->DstAddrMode) {
            case MAC_ADDR_MODE_NONE:
                break;
            case MAC_ADDR_MODE_16_BIT:
                ptr += 4;
                break;
            case MAC_ADDR_MODE_64_BIT:
                ptr += 10;
                break;
        }
    }
    uint16_t panid = 0;
    panid += *ptr++;
    panid += (uint16_t)(*ptr++) << 8;

    return panid;
}

uint16_t mac_header_get_dst_panid(const mac_fcf_sequence_t *header, const uint8_t *ptr)
{
    if( !header || !ptr ){
        return 0;
    }
    if (header->DstAddrMode == MAC_ADDR_MODE_NONE) {
        return 0xffff;
    }
    ptr += 3;
    uint16_t panid = 0;
    panid += *ptr++;
    panid += (uint16_t)(*ptr++) << 8;

    return panid;
}

void mac_header_get_src_address(const mac_fcf_sequence_t *header, const uint8_t *ptr, uint8_t *address_ptr)
{
    if( !header || !ptr || !address_ptr ){
        return;
    }
    ptr += 3; //Skip FCF + DSN
    switch (header->DstAddrMode) {
        case MAC_ADDR_MODE_NONE:
            ptr += 2;
            break;
        case MAC_ADDR_MODE_16_BIT:
            ptr += 4;
            if (!header->intraPan) {
                ptr += 2;
            }
            break;
        case MAC_ADDR_MODE_64_BIT:
            ptr += 10;
            if (!header->intraPan) {
                ptr += 2;
            }
            break;
    }


    if (header->SrcAddrMode == MAC_ADDR_MODE_NONE) {
        return;
    }
    uint8_t address_len, address_index, i;
    if (header->SrcAddrMode == MAC_ADDR_MODE_16_BIT) {
        address_len = 2;
        address_index = 1;
    } else  {
        address_len = 8;
        address_index = 7;
    }

    for (i = 0; i < address_len; i++) {
        address_ptr[address_index - i] = *ptr++;
    }
}

void mac_header_get_dst_address(const mac_fcf_sequence_t *header, const uint8_t *ptr, uint8_t *address_ptr)
{
    if( !header || !ptr || !address_ptr ){
        return;
    }

    if (header->DstAddrMode == MAC_ADDR_MODE_NONE) {
        return;
    }
    uint8_t address_len, address_index, i;
    if (header->DstAddrMode == MAC_ADDR_MODE_16_BIT) {

        address_len = 2;
        address_index = 1;
    } else  {
        address_len = 8;
        address_index = 7;
    }
    ptr += 5; //Skip fcf, dsn & PANID

    for (i = 0; i < address_len; i++) {
        address_ptr[address_index - i] = *ptr++;
    }
}

uint16_t mcps_payload_length_from_received_frame(const mac_pre_parsed_frame_t *buffer)
{
    if( !buffer ){
        return 0;
    }
    return buffer->mac_payload_length;
}

uint8_t mcps_mac_header_length_from_received_frame(const mac_pre_parsed_frame_t *buffer)
{
    if( !buffer ){
        return 0;
    }
    return (buffer->mac_header_length + buffer->security_aux_header_length);
}

uint8_t *mcps_mac_payload_pointer_get(const mac_pre_parsed_frame_t *buffer)
{
    if( !buffer ){
        return NULL;
    }
    uint8_t *ptr = (uint8_t *) mac_header_message_start_pointer(buffer);
    ptr += (buffer->mac_header_length + buffer->security_aux_header_length);
    return ptr;
}

uint8_t *mcps_security_mic_pointer_get(const mac_pre_parsed_frame_t *buffer)
{
    if (!buffer) {
        return NULL;
    }
    uint8_t *ptr = mcps_mac_payload_pointer_get(buffer);
    return (ptr + buffer->mac_payload_length);
}

uint8_t *mcps_mac_security_aux_header_start_pointer_get(const mac_pre_parsed_frame_t *buffer)
{
    if (!buffer || !buffer->fcf_dsn.securityEnabled) {
        return NULL;
    }
    return (uint8_t *) (mac_header_message_start_pointer(buffer) + buffer->mac_header_length);
}

uint8_t mcps_mac_command_frame_id_get(const mac_pre_parsed_frame_t *buffer)
{
    if ( !buffer ) {
        return 0;
    }
    const uint8_t *ptr = mcps_mac_payload_pointer_get(buffer);
    return *ptr;
}

uint32_t mcps_mac_security_frame_counter_read(const mac_pre_parsed_frame_t *buffer)
{
    if (!buffer || !buffer->fcf_dsn.securityEnabled) {
        return 0xffffffff;
    }

    const uint8_t *ptr = (mac_header_message_start_pointer(buffer) + buffer->mac_header_length + 1);
    return common_read_32_bit_inverse(ptr);

}


static uint8_t *mcps_mac_frame_address_write(uint8_t *ptr, uint8_t addressType, const uint8_t *addressPtr)
{
    if (addressType == MAC_ADDR_MODE_16_BIT) {
        uint16_t tempMac16 = common_read_16_bit(addressPtr);
        ptr = common_write_16_bit_inverse(tempMac16, ptr);
    } else if (addressType == MAC_ADDR_MODE_64_BIT) {
        uint8_t i;
        for (i = 0; i < 8; i++) {
            *ptr++ = addressPtr[7 - i];
        }
    }
    return ptr;
}

uint8_t * mcps_generic_header_write(uint8_t *ptr, const mac_pre_build_frame_t *buffer)
{
    ptr = mac_header_write_fcf_dsn(&buffer->fcf_dsn, ptr);

    if (buffer->fcf_dsn.DstAddrMode) {
        ptr = common_write_16_bit_inverse(buffer->DstPANId, ptr);
        //Write DST
        ptr = mcps_mac_frame_address_write(ptr, buffer->fcf_dsn.DstAddrMode,  buffer->DstAddr);
    }

    if (buffer->fcf_dsn.SrcAddrMode ){
        if (!(buffer->fcf_dsn.intraPan)) {
            ptr = common_write_16_bit_inverse(buffer->SrcPANId, ptr);
        }
        ptr = mcps_mac_frame_address_write(ptr, buffer->fcf_dsn.SrcAddrMode, buffer->SrcAddr);
    }
    return ptr;
}

