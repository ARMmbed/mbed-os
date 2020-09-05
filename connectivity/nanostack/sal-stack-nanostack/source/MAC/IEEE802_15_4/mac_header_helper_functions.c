/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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
#include "ns_trace.h"
#include "mlme.h"
#include "mac_api.h"
#include "fhss_api.h"
#include "common_functions.h"
#include "mac_common_defines.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/IEEE802_15_4/mac_header_helper_functions.h"
#include "MAC/rf_driver_storage.h"

static uint8_t *mcps_mac_security_aux_header_start_pointer_get(const mac_pre_parsed_frame_t *buffer);
static uint8_t *mac_header_information_elements_write(const mac_pre_build_frame_t *buffer, uint8_t *ptr);


static uint8_t mac_fcf_length(const mac_fcf_sequence_t *header)
{
    uint8_t length;
    if (header->frameVersion == MAC_FRAME_VERSION_2015) {
        if (header->sequenceNumberSuppress) {
            length = 2; //Skip FCF
        } else {
            length = 3; //Skip FCF + DSN
        }
    } else {
        length = 3; //Skip FCF + DSN
    }
    return length;
}

bool mac_dst_panid_present(const mac_fcf_sequence_t *header)
{
    bool presents = false;
    if (header->DstAddrMode && header->SrcAddrMode) {
        if (header->frameVersion == MAC_FRAME_VERSION_2015) {
            if (header->DstAddrMode == MAC_ADDR_MODE_64_BIT && header->SrcAddrMode == MAC_ADDR_MODE_64_BIT &&  header->intraPan) {

            } else {
                presents = true;
            }
        } else {
            presents = true;
        }

    } else if (header->DstAddrMode && !header->SrcAddrMode) {
        if (header->frameVersion == MAC_FRAME_VERSION_2015) {
            if (!header->intraPan) {
                presents = true;
            }
        } else {
            presents = true;
        }

    } else if (!header->DstAddrMode && !header->SrcAddrMode) {
        if (header->frameVersion == MAC_FRAME_VERSION_2015) {
            if (header->intraPan) {
                presents = true;
            }
        }
    }

    return presents;
}

bool mac_src_panid_present(const mac_fcf_sequence_t *header)
{
    bool presents = false;
    if (header->DstAddrMode && header->SrcAddrMode) {
        if (header->frameVersion == MAC_FRAME_VERSION_2015) {
            if (header->DstAddrMode == MAC_ADDR_MODE_64_BIT && header->SrcAddrMode == MAC_ADDR_MODE_64_BIT) {

            } else if (!header->intraPan) {
                presents = true;
            }
        } else if (!header->intraPan) {
            presents = true;
        }

    } else if (header->SrcAddrMode) {
        if (header->frameVersion == MAC_FRAME_VERSION_2015) {
            if (!header->intraPan) {
                presents = true;
            }
        } else {
            presents = true;
        }
    }
    return presents;
}

uint8_t mac_address_length(uint8_t address_mode)
{
    uint8_t length = 0;
    switch (address_mode) {
        case MAC_ADDR_MODE_NONE:
            break;
        case MAC_ADDR_MODE_16_BIT:
            length = 2;
            break;
        case MAC_ADDR_MODE_64_BIT:
            length = 8;
            break;
    }
    return length;
}

static uint8_t mac_dst_address_length_with_panid(const mac_fcf_sequence_t *header)
{
    uint8_t length = 0;

    if (header->DstPanPresents) {
        length += 2;
    }

    length += mac_address_length(header->DstAddrMode);

    return length;
}

static uint8_t mac_src_address_length_with_panid(const mac_fcf_sequence_t *header)
{
    uint8_t length = 0;

    if (header->SrcPanPresents) {
        length += 2;
    }

    length += mac_address_length(header->SrcAddrMode);

    return length;
}


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

    address_length += mac_dst_address_length_with_panid(fcf);
    address_length += mac_src_address_length_with_panid(fcf);

    return address_length;

}

void mac_header_security_parameter_set(mac_aux_security_header_t *header, const mlme_security_t *frame_setup)
{
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
            memcpy(header->Keysource, frame_setup->Keysource, keysource_len);
        }
    }
}

const uint8_t *mac_header_parse_fcf_dsn(mac_fcf_sequence_t *header, const uint8_t *ptr)
{
    uint16_t fcf = common_read_16_bit_inverse(ptr);
    ptr += 2;

    //Read Frame Type
    header->frametype = ((fcf & MAC_FCF_FRAME_TYPE_MASK) >> MAC_FCF_FRAME_TYPE_SHIFT);
    header->securityEnabled = ((fcf & MAC_FCF_SECURITY_BIT_MASK) >> MAC_FCF_SECURITY_BIT_SHIFT);
    header->framePending = ((fcf & MAC_FCF_PENDING_BIT_MASK) >> MAC_FCF_PENDING_BIT_SHIFT);
    header->ackRequested = ((fcf & MAC_FCF_ACK_REQ_BIT_MASK) >> MAC_FCF_ACK_REQ_BIT_SHIFT);
    header->intraPan = ((fcf & MAC_FCF_INTRA_PANID_MASK) >> MAC_FCF_INTRA_PANID_SHIFT);

    header->DstAddrMode = ((fcf & MAC_FCF_DST_ADDR_MASK) >> MAC_FCF_DST_ADDR_SHIFT);
    header->frameVersion = ((fcf & MAC_FCF_VERSION_MASK) >> MAC_FCF_VERSION_SHIFT);
    header->SrcAddrMode = ((fcf & MAC_FCF_SRC_ADDR_MASK) >> MAC_FCF_SRC_ADDR_SHIFT);

    if (header->frameVersion == MAC_FRAME_VERSION_2015) {
        header->sequenceNumberSuppress = ((fcf & MAC_FCF_SEQ_NUM_SUPPRESS_MASK) >> MAC_FCF_SEQ_NUM_SUPPRESS_SHIFT);
        header->informationElementsPresets = ((fcf & MAC_FCF_IE_PRESENTS_MASK) >> MAC_FCF_IE_PRESENTS_SHIFT);
    } else {
        //SET False to ALL 2015 Extension's by default
        header->sequenceNumberSuppress = false;
        header->informationElementsPresets = false;
    }

    if (header->frameVersion < MAC_FRAME_VERSION_2015 || (header->frameVersion ==  MAC_FRAME_VERSION_2015 &&  !header->sequenceNumberSuppress)) {
        header->DSN = *ptr++;
    } else {
        header->DSN = 0;
    }
    //Check PanID presents at header
    header->DstPanPresents = mac_dst_panid_present(header);
    header->SrcPanPresents = mac_src_panid_present(header);
    return ptr;

}

static uint8_t *mac_header_write_fcf_dsn(const mac_fcf_sequence_t *header, uint8_t *ptr)
{
    uint16_t fcf = 0;
    //Read Frame Type
    fcf |= (header->frametype << MAC_FCF_FRAME_TYPE_SHIFT);
    fcf |= (header->securityEnabled << MAC_FCF_SECURITY_BIT_SHIFT);
    fcf |= (header->framePending << MAC_FCF_PENDING_BIT_SHIFT);
    fcf |= (header->ackRequested << MAC_FCF_ACK_REQ_BIT_SHIFT);
    fcf |= (header->intraPan << MAC_FCF_INTRA_PANID_SHIFT);
    fcf |= (header->sequenceNumberSuppress << MAC_FCF_SEQ_NUM_SUPPRESS_SHIFT);
    fcf |= (header->informationElementsPresets << MAC_FCF_IE_PRESENTS_SHIFT);
    fcf |= (header->DstAddrMode << MAC_FCF_DST_ADDR_SHIFT);
    fcf |= (header->frameVersion << MAC_FCF_VERSION_SHIFT);
    fcf |= (header->SrcAddrMode << MAC_FCF_SRC_ADDR_SHIFT);
    ptr = common_write_16_bit_inverse(fcf, ptr);
    if (header->frameVersion < MAC_FRAME_VERSION_2015 || (header->frameVersion ==  MAC_FRAME_VERSION_2015 &&  !header->sequenceNumberSuppress)) {
        *ptr++ = header->DSN;
    }
    return ptr;
}

uint16_t mac_header_off_set_to_aux_header(const mac_fcf_sequence_t *fcf)
{
    //Skip first FCF & address field
    uint16_t offset = mac_fcf_length(fcf);//Skip FCF + DSN
    offset += mac_dst_address_length_with_panid(fcf);
    offset += mac_address_length(fcf->SrcAddrMode);
    if (fcf->SrcPanPresents) {
        offset += 2; //Skip PanId
    }
    return offset;
}

void mac_header_security_aux_header_parse(const uint8_t *ptr, mlme_security_t *security_params)
{
    uint8_t key_source_len = 0;
    security_params->KeyIdMode = (*ptr >> 3);
    security_params->SecurityLevel = *ptr++;
    ptr += 4; //Skip Frame counter
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

void mac_header_security_components_read(mac_pre_parsed_frame_t *buffer, mlme_security_t *security_params)
{
    memset(security_params, 0, sizeof(mlme_security_t));
    if (!buffer->fcf_dsn.securityEnabled) {
        return;
    }

    mac_header_security_aux_header_parse(mcps_mac_security_aux_header_start_pointer_get(buffer), security_params);

}

static bool mac_header_pan_full_compressed(const mac_fcf_sequence_t *header)
{
    if (header->frameVersion == MAC_FRAME_VERSION_2015 && (!header->DstPanPresents && !header->SrcPanPresents) && header->intraPan) {
        return true;
    }
    return false;
}

static uint16_t mac_header_read_src_pan(const mac_fcf_sequence_t *header, const uint8_t *ptr)
{
    ptr += mac_fcf_length(header);//Skip FCF + DSN

    ptr += mac_dst_address_length_with_panid(header); //Skip Dst panID & Address

    return common_read_16_bit_inverse(ptr);
}

static uint16_t mac_header_read_dst_pan(const mac_fcf_sequence_t *header, const uint8_t *ptr)
{
    ptr += mac_fcf_length(header);//Skip FCF + DSN

    return common_read_16_bit_inverse(ptr);
}

uint16_t mac_header_get_src_panid(const mac_fcf_sequence_t *header, const uint8_t *ptr, uint16_t configured_pan_id)
{
    if (mac_header_pan_full_compressed(header)) {
        return configured_pan_id;
    }

    if (!header->SrcPanPresents) {
        if (!header->DstPanPresents) {
            return 0xffff;
        }
        return mac_header_read_dst_pan(header, ptr);
    }

    return mac_header_read_src_pan(header, ptr);
}

uint16_t mac_header_get_dst_panid(const mac_fcf_sequence_t *header, const uint8_t *ptr, uint16_t configured_pan_id)
{
    if (mac_header_pan_full_compressed(header)) {
        return configured_pan_id;
    }
    if (!header->DstPanPresents) {
        if (header->SrcPanPresents && header->frameVersion == MAC_FRAME_VERSION_2015 && header->DstAddrMode == MAC_ADDR_MODE_NONE) {
            return mac_header_read_src_pan(header, ptr);
        }
        return 0xffff;
    }

    return mac_header_read_dst_pan(header, ptr);
}

void mac_header_get_src_address(const mac_fcf_sequence_t *header, const uint8_t *ptr, uint8_t *address_ptr)
{

    if (header->SrcAddrMode == MAC_ADDR_MODE_NONE) {
        return;
    }

    ptr += mac_fcf_length(header);//Skip FCF + DSN

    ptr += mac_dst_address_length_with_panid(header);

    uint8_t address_len, address_index, i;

    address_len = mac_address_length(header->SrcAddrMode);

    if (header->SrcPanPresents) {
        ptr += 2; //Skip PanId
    }
    address_index = address_len - 1;


    for (i = 0; i < address_len; i++) {
        address_ptr[address_index - i] = *ptr++;
    }
}

void mac_header_get_dst_address(const mac_fcf_sequence_t *header, const uint8_t *ptr, uint8_t *address_ptr)
{

    if (header->DstAddrMode == MAC_ADDR_MODE_NONE) {
        return;
    }
    uint8_t address_len, address_index, i;

    ptr += mac_fcf_length(header);//Skip FCF + DSN

    address_len = mac_address_length(header->DstAddrMode);

    if (header->DstPanPresents) {
        ptr += 2; //Skip PanId
    }
    address_index = address_len - 1;

    for (i = 0; i < address_len; i++) {
        address_ptr[address_index - i] = *ptr++;
    }
}

static uint16_t mac_payload_length_calc_with_ie(uint16_t payload_length, uint16_t payload_ie_length)
{
    uint16_t length = payload_length;
    if (payload_ie_length) {
        if (length) {
            length += 2;
        }
        length += payload_ie_length;
    }
    return length;
}

uint8_t mcps_mac_header_length_from_received_frame(const mac_pre_parsed_frame_t *buffer)
{
    return (buffer->mac_header_length + buffer->security_aux_header_length + buffer->header_ie_length);
}

uint8_t *mcps_mac_payload_pointer_get(const mac_pre_parsed_frame_t *buffer)
{
    uint8_t *ptr = (uint8_t *) mac_header_message_start_pointer(buffer);
    ptr += mcps_mac_header_length_from_received_frame(buffer);
    return ptr;
}

uint8_t *mcps_security_mic_pointer_get(const mac_pre_parsed_frame_t *buffer)
{
    uint8_t *ptr = mcps_mac_payload_pointer_get(buffer) + buffer->mac_payload_length;
    return ptr;
}

static uint8_t *mcps_mac_security_aux_header_start_pointer_get(const mac_pre_parsed_frame_t *buffer)
{
    if (!buffer->fcf_dsn.securityEnabled) {
        return NULL;
    }
    return (uint8_t *)(mac_header_message_start_pointer(buffer) + buffer->mac_header_length);
}

uint8_t mcps_mac_command_frame_id_get(const mac_pre_parsed_frame_t *buffer)
{
    const uint8_t *ptr = mcps_mac_payload_pointer_get(buffer);
    return *ptr;
}

uint32_t mcps_mac_security_frame_counter_read(const mac_pre_parsed_frame_t *buffer)
{
    if (!buffer->fcf_dsn.securityEnabled) {
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

static uint8_t *mac_security_interface_aux_security_header_write(uint8_t *ptr, const mac_aux_security_header_t *auxHeader)
{
    uint8_t auxBaseHeader;
    auxBaseHeader = auxHeader->securityLevel;
    auxBaseHeader |= (auxHeader->KeyIdMode << 3);
    *ptr++ = auxBaseHeader;
    ptr = common_write_32_bit_inverse(auxHeader->frameCounter, ptr);

    switch (auxHeader->KeyIdMode) {
        case MAC_KEY_ID_MODE_SRC8_IDX:
            memcpy(ptr, auxHeader->Keysource, 8);
            ptr += 8;
            *ptr++ = auxHeader->KeyIndex;
            break;
        case MAC_KEY_ID_MODE_SRC4_IDX:
            memcpy(ptr, auxHeader->Keysource, 4);
            ptr += 4;
            *ptr++ = auxHeader->KeyIndex;
            break;
        case MAC_KEY_ID_MODE_IDX:
            *ptr++ = auxHeader->KeyIndex;
            break;
        default:
            break;
    }
    return ptr;
}

uint8_t *mac_generic_packet_write(struct protocol_interface_rf_mac_setup *rf_ptr, uint8_t *ptr, const mac_pre_build_frame_t *buffer)
{
    ptr = mac_header_write_fcf_dsn(&buffer->fcf_dsn, ptr);

    if (buffer->fcf_dsn.DstPanPresents) {
        ptr = common_write_16_bit_inverse(buffer->DstPANId, ptr);
    }

    if (buffer->fcf_dsn.DstAddrMode) {
        //Write DST
        ptr = mcps_mac_frame_address_write(ptr, buffer->fcf_dsn.DstAddrMode,  buffer->DstAddr);
    }

    if (buffer->fcf_dsn.SrcPanPresents) {
        ptr = common_write_16_bit_inverse(buffer->SrcPANId, ptr);
    }

    if (buffer->fcf_dsn.SrcAddrMode) {
        ptr = mcps_mac_frame_address_write(ptr, buffer->fcf_dsn.SrcAddrMode, buffer->SrcAddr);
    }

    if (buffer->fcf_dsn.securityEnabled) {
        ptr = mac_security_interface_aux_security_header_write(ptr, &buffer->aux_header);
    }
    uint8_t *ie_start = ptr;
    //Copy Payload and set IE Elemets
    ptr = mac_header_information_elements_write(buffer, ptr);
    if (buffer->mac_payload_length) {
        memcpy(ptr, buffer->mac_payload, buffer->mac_payload_length);
        ptr += buffer->mac_payload_length;
    }
    if (rf_ptr->fhss_api) {
        if (buffer->fcf_dsn.frametype == FC_BEACON_FRAME) {
            dev_driver_tx_buffer_s *tx_buf = &rf_ptr->dev_driver_tx_buffer;
            uint8_t *synch_info = tx_buf->buf + rf_ptr->dev_driver->phy_driver->phy_header_length + tx_buf->len - FHSS_SYNCH_INFO_LENGTH;
            rf_ptr->fhss_api->write_synch_info(rf_ptr->fhss_api, synch_info, FHSS_SYNCH_INFO_LENGTH, FHSS_SYNCH_FRAME, buffer->tx_time);
        } else {
            rf_ptr->fhss_api->write_synch_info(rf_ptr->fhss_api, ie_start, buffer->headerIeLength, FHSS_DATA_FRAME, buffer->tx_time);
        }
    }
    return ptr;
}

static uint8_t *mac_write_ie_vector_list(ns_ie_iovec_t *list, uint16_t length, uint8_t *ptr)
{
    const ns_ie_iovec_t *msg_iov = list;
    for (uint_fast16_t i = 0; i < length; i++, msg_iov++) {
        memcpy(ptr, msg_iov->ieBase, msg_iov->iovLen);
        ptr += msg_iov->iovLen;
    }
    return ptr;
}

static bool mac_parse_header_ie(mac_header_IE_t *header_element, uint8_t *ptr)
{
    uint16_t ie_dummy = common_read_16_bit_inverse(ptr);
    if (ie_dummy & 0x8000) {
        return false;
    }
    header_element->length = (ie_dummy & 0x007f);
    header_element->id = ((ie_dummy & 0x7f80) >> 7);
    header_element->content_ptr = ptr + 2;
    return true;
}

static bool mac_parse_payload_ie(mac_payload_IE_t *payload_element, uint8_t *ptr)
{
    uint16_t ie_dummy = common_read_16_bit_inverse(ptr);
    if (!(ie_dummy & 0x8000)) {
        return false;
    }
    payload_element->length = (ie_dummy & 0x07ff);
    payload_element->id = ((ie_dummy & 0x7800) >> 11);
    payload_element->content_ptr = ptr + 2;
    return true;
}


bool mac_header_information_elements_parse(mac_pre_parsed_frame_t *buffer)
{
    uint8_t *ptr = (mac_header_message_start_pointer(buffer) + buffer->mac_header_length + buffer->security_aux_header_length);

    buffer->headerIePtr = NULL;
    buffer->headerIeLength = 0;
    buffer->payloadsIePtr = NULL;
    buffer->payloadsIeLength = 0;
    buffer->header_ie_length = 0;
    if (!buffer->fcf_dsn.informationElementsPresets) {
        buffer->macPayloadPtr = ptr;
        return true;
    }

    if (buffer->mac_payload_length < 2) {
        return false;
    }

    mac_header_IE_t header_ie;
    buffer->headerIePtr = ptr;

    while (buffer->mac_payload_length >= 2) {

        if (!mac_parse_header_ie(&header_ie, ptr)) {
            return false;
        }

        buffer->mac_payload_length -= 2;
        if (header_ie.length > buffer->mac_payload_length) {
            return false;
        }

        buffer->mac_payload_length -= header_ie.length;

        buffer->header_ie_length  += header_ie.length + 2;
        ptr += (2 + header_ie.length);

        if (header_ie.id == MAC_HEADER_TERMINATION1_IE_ID) {
            break;
        } else if (header_ie.id == MAC_HEADER_TERMINATION2_IE_ID) {
            buffer->macPayloadPtr = ptr;
            return true;
        }
        buffer->headerIeLength += header_ie.length + 2;
    }

    return true;
}


bool mac_payload_information_elements_parse(mac_pre_parsed_frame_t *buffer)
{
    uint8_t *ptr = (mac_header_message_start_pointer(buffer) + buffer->mac_header_length + buffer->security_aux_header_length + buffer->header_ie_length);
    if (!buffer->fcf_dsn.informationElementsPresets) {
        return true;
    }

    if (buffer->mac_payload_length < 2) {
        return false;
    }

    //Parse Payload IE
    buffer->payloadsIePtr = ptr;
    mac_payload_IE_t payload_ie;
    while (buffer->mac_payload_length >= 2) {

        if (!mac_parse_payload_ie(&payload_ie, ptr)) {
            return false;
        }
        buffer->mac_payload_length -= 2;
        if (payload_ie.length > buffer->mac_payload_length) {
            return false;
        }
        buffer->mac_payload_length -= payload_ie.length;

        if (payload_ie.id == MAC_PAYLOAD_TERMINATION_IE_GROUP_ID) {
            break;
        }
        buffer->payloadsIeLength += payload_ie.length + 2;
        buffer->macPayloadPtr += payload_ie.length + 2;
        ptr += (2 + payload_ie.length);

    }
    buffer->macPayloadPtr = ptr;
    return true;
}


static uint8_t *mac_header_ie_terimate(uint8_t *ptr, uint8_t type)
{
    uint16_t ie_dummy = 0;
    ie_dummy |= (type << 7);
    return common_write_16_bit_inverse(ie_dummy, ptr);


}

static uint8_t *mac_payload_ie_terimate(uint8_t *ptr)
{
    uint16_t ie_dummy = 0;
    ie_dummy |= (MAC_PAYLOAD_TERMINATION_IE_GROUP_ID << 11);
    ie_dummy |= (1 << 15);
    return common_write_16_bit_inverse(ie_dummy, ptr);
}


void mac_header_information_elements_preparation(mac_pre_build_frame_t *buffer)
{
    if (buffer->message_builded) {
        return;
    }

    if (buffer->headerIeLength ||  buffer->payloadsIeLength) {
        buffer->fcf_dsn.frameVersion = MAC_FRAME_VERSION_2015;
        buffer->fcf_dsn.informationElementsPresets = true;
        buffer->message_builded = true;
        //Write Header elements
        if (buffer->headerIeLength) {
            buffer->mac_header_length_with_security += buffer->headerIeLength;
            if (!buffer->payloadsIeLength && !buffer->mac_payload_length) {
                //No termination needed
                return;
            }
            //Terminate
            buffer->mac_header_length_with_security += 2;
        } else {
            buffer->mac_header_length_with_security += 2;
        }
    }
}

uint16_t mac_buffer_total_payload_length(mac_pre_build_frame_t *buffer)
{
    return mac_payload_length_calc_with_ie(buffer->mac_payload_length, buffer->payloadsIeLength);
}


static uint8_t *mac_header_information_elements_write(const mac_pre_build_frame_t *buffer, uint8_t *ptr)
{
    if (buffer->fcf_dsn.frameVersion == MAC_FRAME_VERSION_2015 && buffer->fcf_dsn.informationElementsPresets) {
        //Write Header elements
        if (buffer->headerIeLength) {
            ptr = mac_write_ie_vector_list(buffer->ie_elements.headerIeVectorList, buffer->ie_elements.headerIovLength, ptr);

            if (!buffer->payloadsIeLength && !buffer->mac_payload_length) {
                //No termination needed
                return ptr;
            } else if (!buffer->payloadsIeLength && buffer->mac_payload_length) {
                return mac_header_ie_terimate(ptr, MAC_HEADER_TERMINATION2_IE_ID);
            }
        }
        //Add Header Termination
        ptr = mac_header_ie_terimate(ptr, MAC_HEADER_TERMINATION1_IE_ID);

        if (buffer->payloadsIeLength) {
            ptr = mac_write_ie_vector_list(buffer->ie_elements.payloadIeVectorList, buffer->ie_elements.payloadIovLength, ptr);
            if (buffer->mac_payload_length) {
                ptr = mac_payload_ie_terimate(ptr);
            }
        }
    }
    return ptr;
}

