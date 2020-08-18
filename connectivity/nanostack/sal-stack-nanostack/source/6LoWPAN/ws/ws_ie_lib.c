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
#include <string.h>
#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "common_functions.h"
#include "mac_common_defines.h"
#include "6LoWPAN/MAC/mac_ie_lib.h"
#include "6LoWPAN/ws/ws_common_defines.h"
#include "6LoWPAN/ws/ws_ie_lib.h"

static uint8_t *ws_wh_header_base_write(uint8_t *ptr, uint16_t length, uint8_t type)
{
    ptr = mac_ie_header_base_write(ptr, MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID, length + 1);
    *ptr++ = type;
    return ptr;
}

static uint16_t ws_channel_plan_length(uint8_t channel_plan)
{
    switch (channel_plan) {
        case 0:
            //Regulator domain and operationg class inline
            return 2;
        case 1:
            //CHo, Channel spasing and number of channel's inline
            return 6;

        default:
            return 0;
    }
}

static uint16_t ws_channel_function_length(uint8_t channel_function, uint16_t hop_channel_count)
{
    switch (channel_function) {
        case 0:
            //Fixed channel inline
            return 2;
        case 1:
        case 2:
            return 0;
        case 3:
            //Hop count + channel hop list
            return (1 + hop_channel_count);
        default:
            return 0;

    }
}

uint16_t ws_wp_nested_hopping_schedule_length(struct ws_hopping_schedule_s *hopping_schedule, bool unicast_schedule)
{
    uint16_t length;
    uint8_t channel_function;
    if (unicast_schedule) {
        length = 4;
        channel_function = hopping_schedule->uc_channel_function;
    } else {
        length = 10;
        channel_function = hopping_schedule->bc_channel_function;
    }

    length += ws_channel_plan_length(hopping_schedule->channel_plan);

    length += ws_channel_function_length(channel_function, 1);

    if (unicast_schedule && hopping_schedule->excluded_channels.excuded_channel_ctrl) {
        if (hopping_schedule->excluded_channels.excuded_channel_ctrl == WS_EXC_CHAN_CTRL_RANGE) {
            length += (hopping_schedule->excluded_channels.excluded_range_length * 4) + 1;
        } else {
            length += hopping_schedule->excluded_channels.channel_mask_bytes_inline;
        }
    }
    return length;
}

uint8_t *ws_wh_utt_write(uint8_t *ptr, uint8_t message_type)
{
    ptr = ws_wh_header_base_write(ptr, 4, WH_IE_UTT_TYPE);
    *ptr++ = message_type;
    memset(ptr, 0, 3);
    ptr += 3;
    return ptr;
}

uint8_t *ws_wh_bt_write(uint8_t *ptr)
{
    ptr = ws_wh_header_base_write(ptr, 5, WH_IE_BT_TYPE);
    memset(ptr, 0, 5);
    ptr += 5;
    return ptr;
}


uint8_t *ws_wh_fc_write(uint8_t *ptr, uint8_t flow_ctrl)
{
    ptr = ws_wh_header_base_write(ptr, 1, WH_IE_FC_TYPE);
    *ptr++ = flow_ctrl;
    return ptr;
}

uint8_t *ws_wh_rsl_write(uint8_t *ptr, uint8_t rsl)
{
    ptr = ws_wh_header_base_write(ptr, 1, WH_IE_RSL_TYPE);
    *ptr++ = rsl;
    return ptr;
}

uint8_t *ws_wh_ea_write(uint8_t *ptr, uint8_t *eui64)
{
    ptr = ws_wh_header_base_write(ptr, 8, WH_IE_EA_TYPE);
    memcpy(ptr, eui64, 8);
    ptr += 8;
    return ptr;
}

uint8_t *ws_wh_vh_write(uint8_t *ptr, uint8_t *vendor_header, uint8_t vendor_header_length)
{
    ptr = ws_wh_header_base_write(ptr, vendor_header_length, WH_IE_VH_TYPE);
    if (vendor_header_length) {
        memcpy(ptr, vendor_header, vendor_header_length);
        ptr += vendor_header_length;
    }
    return ptr;
}

uint8_t *ws_wp_base_write(uint8_t *ptr, uint16_t length)
{
    return mac_ie_payload_base_write(ptr, WS_WP_NESTED_IE, length);
}

uint8_t *ws_wp_nested_hopping_schedule_write(uint8_t *ptr, struct ws_hopping_schedule_s *hopping_schedule, bool unicast_schedule)
{
    //Calculate length
    uint16_t length = ws_wp_nested_hopping_schedule_length(hopping_schedule, unicast_schedule);
    if (!unicast_schedule) {
        ptr = mac_ie_nested_ie_long_base_write(ptr, WP_PAYLOAD_IE_BS_TYPE, length);
        ptr = common_write_32_bit_inverse(hopping_schedule->fhss_broadcast_interval, ptr);
        ptr = common_write_16_bit_inverse(hopping_schedule->fhss_bsi, ptr);
        *ptr++ = hopping_schedule->fhss_bc_dwell_interval;
    } else {
        ptr = mac_ie_nested_ie_long_base_write(ptr, WP_PAYLOAD_IE_US_TYPE, length);
        *ptr++ =  hopping_schedule->fhss_uc_dwell_interval;
    }

    *ptr++ =  hopping_schedule->clock_drift;
    *ptr++ =  hopping_schedule->timing_accurancy;
    uint8_t channel_info_base = 0;
    channel_info_base = (hopping_schedule->channel_plan);
    if (unicast_schedule) {
        channel_info_base |= (hopping_schedule->uc_channel_function << 3);
        //Set Excluded Channel control part
        channel_info_base |= (hopping_schedule->excluded_channels.excuded_channel_ctrl << 6);
    } else {
        channel_info_base |= (hopping_schedule->bc_channel_function << 3);
    }

    *ptr++ = channel_info_base;

    switch (hopping_schedule->channel_plan) {
        case 0:
            //Regulator domain and operationg class inline
            *ptr++ = hopping_schedule->regulatory_domain;
            *ptr++ = hopping_schedule->operating_class;
            break;
        case 1:
            //CHo, Channel spasing and number of channel's inline
            ptr = common_write_24_bit_inverse(hopping_schedule->ch0_freq * 100, ptr);
            *ptr++ = hopping_schedule->channel_spacing;
            ptr = common_write_16_bit_inverse(hopping_schedule->number_of_channels, ptr);
            break;
        default:
            break;
    }
    uint8_t cf = hopping_schedule->uc_channel_function;
    uint16_t fixed_channel = hopping_schedule->uc_fixed_channel;
    if (!unicast_schedule) {
        cf = hopping_schedule->bc_channel_function;
    }
    switch (cf) {
        case 0:
            //Fixed channel inline
            if (!unicast_schedule) {
                fixed_channel = hopping_schedule->bc_fixed_channel;
            }
            ptr = common_write_16_bit_inverse(fixed_channel, ptr);
            break;
        case 1:
        case 2:
            //No Inline
            break;
        case 3:
            //TODO do this properly
            //Hop count + channel hop list
            *ptr++ = 1;
            *ptr++ = 0;
            break;
        default:
            break;

    }

    if (unicast_schedule && hopping_schedule->excluded_channels.excuded_channel_ctrl) {
        if (hopping_schedule->excluded_channels.excuded_channel_ctrl == WS_EXC_CHAN_CTRL_RANGE) {
            uint8_t range_length = hopping_schedule->excluded_channels.excluded_range_length;
            ws_excluded_channel_range_data_t *range_ptr = hopping_schedule->excluded_channels.exluded_range;
            *ptr++ = range_length;
            while (range_length) {
                ptr = common_write_16_bit_inverse(range_ptr->range_start, ptr);
                ptr = common_write_16_bit_inverse(range_ptr->range_end, ptr);
                range_length--;
                range_ptr++;
            }
        } else if (hopping_schedule->excluded_channels.excuded_channel_ctrl == WS_EXC_CHAN_CTRL_BITMASK) {
            //Set Mask
            uint16_t channel_mask_length = hopping_schedule->excluded_channels.channel_mask_bytes_inline * 8;

            for (uint8_t i = 0; i < 8; i++) {
                uint32_t mask_value = hopping_schedule->excluded_channels.channel_mask[i];
                if (channel_mask_length >= 32) {
                    ptr = common_write_32_bit(mask_value, ptr);
                    channel_mask_length -= 32;
                } else {
                    //Write MSB Bits from mask 24-8 top bits
                    uint8_t move_mask = 0;
                    while (channel_mask_length) {
                        *ptr++ = (uint8_t)(mask_value >> (24 - move_mask));
                        channel_mask_length -= 8;
                        move_mask += 8;
                    }
                }

                if (channel_mask_length == 0) {
                    break;
                }
            }
        }
    }

    return ptr;
}

uint8_t *ws_wp_nested_vp_write(uint8_t *ptr, uint8_t *vendor_payload, uint16_t vendor_payload_length)
{
    if (vendor_payload_length) {
        ptr = mac_ie_nested_ie_long_base_write(ptr, WP_PAYLOAD_IE_VP_TYPE, vendor_payload_length);
        memcpy(ptr, vendor_payload, vendor_payload_length);
        ptr += vendor_payload_length;
    }
    return ptr;
}

uint8_t *ws_wp_nested_pan_info_write(uint8_t *ptr, struct ws_pan_information_s *pan_congiguration)
{
    if (!pan_congiguration) {
        return mac_ie_nested_ie_short_base_write(ptr, WP_PAYLOAD_IE_PAN_TYPE, 0);
    }
    ptr = mac_ie_nested_ie_short_base_write(ptr, WP_PAYLOAD_IE_PAN_TYPE, 5);
    ptr = common_write_16_bit_inverse(pan_congiguration->pan_size, ptr);
    ptr = common_write_16_bit_inverse(pan_congiguration->routing_cost, ptr);
    uint8_t temp8 = 0;
    temp8 |= (pan_congiguration->use_parent_bs << 0);
    temp8 |= (pan_congiguration->rpl_routing_method << 1);
    temp8 |= pan_congiguration->version << 5;
    *ptr++ = temp8;
    return ptr;
}


uint8_t *ws_wp_nested_netname_write(uint8_t *ptr, uint8_t *network_name, uint8_t network_name_length)
{
    ptr = mac_ie_nested_ie_short_base_write(ptr, WP_PAYLOAD_IE_NETNAME_TYPE, network_name_length);
    if (network_name_length) {
        memcpy(ptr, network_name, network_name_length);
        ptr += network_name_length;
    }
    return ptr;
}

uint8_t *ws_wp_nested_pan_ver_write(uint8_t *ptr, struct ws_pan_information_s *pan_congiguration)
{
    if (!pan_congiguration) {
        return ptr;
    }
    ptr = mac_ie_nested_ie_short_base_write(ptr, WP_PAYLOAD_IE_PAN_VER_TYPE, 2);
    return common_write_16_bit_inverse(pan_congiguration->pan_version, ptr);
}

uint8_t *ws_wp_nested_gtkhash_write(uint8_t *ptr, uint8_t *gtkhash, uint8_t gtkhash_length)
{
    ptr = mac_ie_nested_ie_short_base_write(ptr, WP_PAYLOAD_IE_GTKHASH_TYPE, gtkhash_length);
    if (gtkhash_length) {
        memcpy(ptr, gtkhash, 32);
        ptr += 32;
    }
    return ptr;
}

bool ws_wh_utt_read(uint8_t *data, uint16_t length, struct ws_utt_ie *utt_ie)
{
    mac_header_IE_t utt_ie_data;
    utt_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (4 != mac_ie_header_sub_id_discover(data, length, &utt_ie_data, WH_IE_UTT_TYPE)) {
        // NO UTT header
        return false;
    }
    data = utt_ie_data.content_ptr;
    utt_ie->message_type = *data++;
    utt_ie->ufsi = common_read_24_bit_inverse(data);
    return true;
}

bool ws_wh_bt_read(uint8_t *data, uint16_t length, struct ws_bt_ie *bt_ie)
{
    mac_header_IE_t btt_ie_data;
    btt_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (5 != mac_ie_header_sub_id_discover(data, length, &btt_ie_data, WH_IE_BT_TYPE)) {
        return false;
    }
    data = btt_ie_data.content_ptr;
    bt_ie->broadcast_slot_number = common_read_16_bit_inverse(data);
    bt_ie->broadcast_interval_offset = common_read_24_bit_inverse(data + 2);
    return true;
}

bool ws_wh_rsl_read(uint8_t *data, uint16_t length, int8_t *rsl)
{
    mac_header_IE_t rsl_ie_data;
    rsl_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (1 != mac_ie_header_sub_id_discover(data, length, &rsl_ie_data, WH_IE_RSL_TYPE)) {
        return false;
    }
    *rsl = *rsl_ie_data.content_ptr;

    return true;
}

bool ws_wh_ea_read(uint8_t *data, uint16_t length, uint8_t *eui64)
{
    mac_header_IE_t rsl_ie_data;
    rsl_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (8 != mac_ie_header_sub_id_discover(data, length, &rsl_ie_data, WH_IE_EA_TYPE)) {
        return false;
    }
    memcpy(eui64, rsl_ie_data.content_ptr, 8);

    return true;
}

static uint8_t *ws_channel_plan_zero_read(uint8_t *ptr, ws_channel_plan_zero_t *plan)
{
    plan->regulator_domain = *ptr++;
    plan->operation_class = *ptr++;
    return ptr;
}

static uint8_t *ws_channel_plan_one_read(uint8_t *ptr, ws_channel_plan_one_t *plan)
{
    plan->ch0 = common_read_24_bit_inverse(ptr);
    plan->ch0 /= 100;
    ptr += 3;
    plan->channel_spacing = *ptr++;
    plan->number_of_channel = common_read_16_bit_inverse(ptr);
    ptr += 2;
    return ptr;
}

static uint8_t *ws_channel_function_zero_read(uint8_t *ptr, ws_channel_function_zero_t *plan)
{
    plan->fixed_channel = common_read_16_bit_inverse(ptr);
    return ptr + 2;
}

static uint8_t *ws_channel_function_three_read(uint8_t *ptr, ws_channel_function_three_t *plan)
{
    plan->channel_hop_count = *ptr++;
    plan->channel_list = ptr++;
    return ptr;
}

bool ws_wp_nested_us_read(uint8_t *data, uint16_t length, struct ws_us_ie *us_ie)
{
    mac_nested_payload_IE_t nested_payload_ie;
    nested_payload_ie.id = WP_PAYLOAD_IE_US_TYPE;
    nested_payload_ie.type_long = true;
    if (mac_ie_nested_discover(data, length, &nested_payload_ie) < 4) {
        return false;
    }

    data = nested_payload_ie.content_ptr;
    us_ie->dwell_interval = *data++;
    us_ie->clock_drift = *data++;
    us_ie->timing_accurancy = *data++;
    us_ie->channel_plan = (*data & 3);
    us_ie->channel_function = (*data & 0x38) >> 3;
    us_ie->excluded_channel_ctrl = (*data & 0xc0) >> 6;
    data++;
    uint16_t info_length = 0;
    nested_payload_ie.length -= 4;
    info_length = ws_channel_plan_length(us_ie->channel_plan);
    if (nested_payload_ie.length < info_length) {
        return false;
    }

    nested_payload_ie.length -= info_length;
    switch (us_ie->channel_plan) {
        case 0:
            data = ws_channel_plan_zero_read(data, &us_ie->plan.zero);
            break;

        case 1:
            data = ws_channel_plan_one_read(data, &us_ie->plan.one);
            break;
        default:
            return false;

    }

    info_length = ws_channel_function_length(us_ie->channel_function, 0);

    if (nested_payload_ie.length < info_length) {
        return false;
    }
    nested_payload_ie.length -= info_length;


    switch (us_ie->channel_function) {
        case 0:
            data = ws_channel_function_zero_read(data, &us_ie->function.zero);
            break;

        case 1:
        case 2:
            break;

        case 3:

            data = ws_channel_function_three_read(data, &us_ie->function.three);
            info_length = us_ie->function.three.channel_hop_count;
            if (nested_payload_ie.length < info_length) {
                return false;
            }
            nested_payload_ie.length -= info_length;
            data += info_length;
            break;
        default:
            return false;

    }

    switch (us_ie->excluded_channel_ctrl) {
        case WS_EXC_CHAN_CTRL_NONE:

            break;
        case WS_EXC_CHAN_CTRL_RANGE:
            us_ie->excluded_channels.range.number_of_range = *data;
            if (nested_payload_ie.length < (us_ie->excluded_channels.range.number_of_range * 4) + 1) {
                return false;
            }
            //Set Range start after validation
            us_ie->excluded_channels.range.range_start = data + 1;
            break;

        case WS_EXC_CHAN_CTRL_BITMASK:
            if (us_ie->channel_plan == 1) {
                us_ie->excluded_channels.mask.mask_len_inline = ((us_ie->plan.one.number_of_channel + 7) / 8);
                if (us_ie->excluded_channels.mask.mask_len_inline != nested_payload_ie.length) {
                    //Channel mask length is not correct
                    return false;
                }
            } else {
                us_ie->excluded_channels.mask.mask_len_inline = nested_payload_ie.length;
            }

            us_ie->excluded_channels.mask.channel_mask = data;
            break;
        default:
            return false;
    }

    return true;
}
bool ws_wp_nested_bs_read(uint8_t *data, uint16_t length, struct ws_bs_ie *bs_ie)
{
    mac_nested_payload_IE_t nested_payload_ie;
    nested_payload_ie.id = WP_PAYLOAD_IE_BS_TYPE;
    nested_payload_ie.type_long = true;
    if (mac_ie_nested_discover(data, length, &nested_payload_ie) < 10) {
        return false;
    }
    data = nested_payload_ie.content_ptr;
    bs_ie->broadcast_interval = common_read_32_bit_inverse(data);
    bs_ie->broadcast_schedule_identifier = common_read_16_bit_inverse(data + 4);
    data += 6;
    bs_ie->dwell_interval = *data++;
    bs_ie->clock_drift = *data++;
    bs_ie->timing_accurancy = *data++;

    bs_ie->channel_plan = (*data & 3);
    bs_ie->channel_function = (*data & 0x38) >> 3;
    bs_ie->excluded_channel_ctrl = (*data & 0xc0) >> 6;
    data++;
    nested_payload_ie.length -= 10;
    uint16_t info_length = 0;

    info_length = ws_channel_plan_length(bs_ie->channel_plan);
    if (nested_payload_ie.length < info_length) {
        return false;
    }
    nested_payload_ie.length -= info_length;
    switch (bs_ie->channel_plan) {
        case 0:
            data = ws_channel_plan_zero_read(data, &bs_ie->plan.zero);
            break;

        case 1:
            data = ws_channel_plan_one_read(data, &bs_ie->plan.one);
            break;
        default:
            return false;

    }

    info_length = ws_channel_function_length(bs_ie->channel_function, 0);
    if (nested_payload_ie.length < info_length) {
        return false;
    }
    nested_payload_ie.length -= info_length;

    switch (bs_ie->channel_function) {
        case 0:
            data = ws_channel_function_zero_read(data, &bs_ie->function.zero);
            break;

        case 1:
        case 2:
            break;

        case 3:
            data = ws_channel_function_three_read(data, &bs_ie->function.three);
            info_length = bs_ie->function.three.channel_hop_count;
            if (nested_payload_ie.length < info_length) {
                return false;
            }
            nested_payload_ie.length -= info_length;
            data += info_length;
            break;
        default:
            return false;

    }

    return true;
}

bool ws_wp_nested_pan_read(uint8_t *data, uint16_t length, struct ws_pan_information_s *pan_congiguration)
{
    mac_nested_payload_IE_t nested_payload_ie;
    nested_payload_ie.id = WP_PAYLOAD_IE_PAN_TYPE;
    nested_payload_ie.type_long = false;
    if (mac_ie_nested_discover(data, length, &nested_payload_ie) !=  5) {
        return false;
    }

    pan_congiguration->pan_size = common_read_16_bit_inverse(nested_payload_ie.content_ptr);
    pan_congiguration->routing_cost = common_read_16_bit_inverse(nested_payload_ie.content_ptr + 2);
    pan_congiguration->use_parent_bs = (nested_payload_ie.content_ptr[4] & 0x01) == 0x01;
    pan_congiguration->rpl_routing_method = (nested_payload_ie.content_ptr[4] & 0x02) == 0x02;
    pan_congiguration->version = (nested_payload_ie.content_ptr[4] & 0xe0) >> 5;

    return true;
}

bool ws_wp_nested_pan_version_read(uint8_t *data, uint16_t length, uint16_t *pan_version)
{
    mac_nested_payload_IE_t nested_payload_ie;
    nested_payload_ie.id = WP_PAYLOAD_IE_PAN_VER_TYPE;
    nested_payload_ie.type_long = false;
    if (mac_ie_nested_discover(data, length, &nested_payload_ie) !=  2) {
        return false;
    }
    *pan_version = common_read_16_bit_inverse(nested_payload_ie.content_ptr);

    return true;
}

uint8_t *ws_wp_nested_gtkhash_read(uint8_t *data, uint16_t length)
{
    mac_nested_payload_IE_t nested_payload_ie;
    nested_payload_ie.id = WP_PAYLOAD_IE_GTKHASH_TYPE;
    nested_payload_ie.type_long = false;
    if (mac_ie_nested_discover(data, length, &nested_payload_ie) !=  32) {
        return NULL;
    }

    return nested_payload_ie.content_ptr;
}


bool ws_wp_nested_network_name_read(uint8_t *data, uint16_t length, ws_wp_network_name_t *network_name)
{
    mac_nested_payload_IE_t nested_payload_ie;
    nested_payload_ie.id = WP_PAYLOAD_IE_NETNAME_TYPE;
    nested_payload_ie.type_long = false;

    if (0 == mac_ie_nested_discover(data, length, &nested_payload_ie)) {
        return false;
    } else if (nested_payload_ie.length > 32) {
        //Too long name
        return false;
    }
    network_name->network_name = nested_payload_ie.content_ptr;
    network_name->network_name_length = nested_payload_ie.length;
    return true;
}

