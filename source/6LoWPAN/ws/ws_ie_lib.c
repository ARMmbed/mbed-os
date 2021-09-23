/*
 * Copyright (c) 2018-2020, Pelion and affiliates.
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
#include "ws_management_api.h"

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
        case 2:
            //Regulator domain and channel plan ID inline
            return 2;

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

static uint16_t ws_excluded_channel_length(ws_generic_channel_info_t *generic_channel_info)
{
    uint16_t length;
    if (generic_channel_info->excluded_channel_ctrl == WS_EXC_CHAN_CTRL_RANGE) {
        length = (generic_channel_info->excluded_channels.range.excluded_range_length * 4) + 1;
    } else if (generic_channel_info->excluded_channel_ctrl == WS_EXC_CHAN_CTRL_BITMASK) {
        length = generic_channel_info->excluded_channels.mask.channel_mask_bytes_inline;
    } else {
        length = 0;
    }
    return length;
}

static void ws_generic_channel_info_init(struct ws_hopping_schedule_s *hopping_schedule, ws_generic_channel_info_t *generic_channel_info, bool unicast_schedule)
{
    generic_channel_info->channel_plan = hopping_schedule->channel_plan;
    if (unicast_schedule) {
        generic_channel_info->channel_function = hopping_schedule->uc_channel_function;
        generic_channel_info->excluded_channel_ctrl = hopping_schedule->excluded_channels.excuded_channel_ctrl;
        if (generic_channel_info->excluded_channel_ctrl == WS_EXC_CHAN_CTRL_RANGE) {
            generic_channel_info->excluded_channels.range.excluded_range_length = hopping_schedule->excluded_channels.excluded_range_length;
            generic_channel_info->excluded_channels.range.exluded_range = hopping_schedule->excluded_channels.exluded_range;
        } else if (generic_channel_info->excluded_channel_ctrl == WS_EXC_CHAN_CTRL_BITMASK) {
            generic_channel_info->excluded_channels.mask.channel_mask_bytes_inline = hopping_schedule->excluded_channels.channel_mask_bytes_inline;
            generic_channel_info->excluded_channels.mask.excluded_channel_count = hopping_schedule->excluded_channels.excluded_channel_count;
            generic_channel_info->excluded_channels.mask.channel_mask = hopping_schedule->excluded_channels.channel_mask;
        }
    } else {
        generic_channel_info->channel_function = hopping_schedule->bc_channel_function;
        generic_channel_info->excluded_channel_ctrl = WS_EXC_CHAN_CTRL_NONE;
    }
}

static void ws_wp_channel_plan_set(ws_generic_channel_info_t *generic_channel_info, struct ws_hopping_schedule_s *hopping_schedule)
{
    switch (generic_channel_info->channel_plan) {
        case 0:
            //Regulator domain and operationg class inline
            generic_channel_info->plan.zero.regulator_domain = hopping_schedule->regulatory_domain;
            generic_channel_info->plan.zero.operation_class = hopping_schedule->operating_class;
            break;
        case 1:
            //CHo, Channel spasing and number of channel's inline
            generic_channel_info->plan.one.ch0 = hopping_schedule->ch0_freq;
            generic_channel_info->plan.one.channel_spacing = hopping_schedule->channel_spacing;
            generic_channel_info->plan.one.number_of_channel = hopping_schedule->number_of_channels;
            break;
        case 2:
            generic_channel_info->plan.two.regulator_domain = hopping_schedule->regulatory_domain;
            generic_channel_info->plan.two.channel_plan_id = hopping_schedule->channel_plan_id;
            break;
        default:
            break;
    }
}

static void ws_wp_channel_function_set(ws_generic_channel_info_t *generic_channel_info, struct ws_hopping_schedule_s *hopping_schedule, bool unicast_schedule)
{
    switch (generic_channel_info->channel_function) {
        case 0:
            //Fixed channel inline
            if (unicast_schedule) {
                generic_channel_info->function.zero.fixed_channel = hopping_schedule->uc_fixed_channel;
            } else {
                generic_channel_info->function.zero.fixed_channel = hopping_schedule->bc_fixed_channel;
            }
            break;
        case 1:
        case 2:
            //No Inline
            break;
        case 3:
            //TODO add list to possible to set
            //Force 1 channel 0
            generic_channel_info->function.three.channel_hop_count = 1;
            generic_channel_info->function.three.channel_list = NULL;
            break;
        default:
            break;

    }
}

static uint16_t ws_wp_generic_shedule_length_get(ws_generic_channel_info_t *generic_channel_info)
{
    uint16_t length = 1;

    length += ws_channel_plan_length(generic_channel_info->channel_plan);
    uint16_t number_of_channels = 1;
    if (generic_channel_info->channel_plan == 3) {
        number_of_channels = generic_channel_info->function.three.channel_hop_count;
    } else {
        number_of_channels = 1;
    }
    length += ws_channel_function_length(generic_channel_info->channel_function, number_of_channels);

    length += ws_excluded_channel_length(generic_channel_info);

    return length;
}

uint16_t ws_wp_nested_hopping_schedule_length(struct ws_hopping_schedule_s *hopping_schedule, bool unicast_schedule)
{
    ws_generic_channel_info_t generic_channel_info;

    ws_generic_channel_info_init(hopping_schedule, &generic_channel_info, unicast_schedule);
    ws_wp_channel_function_set(&generic_channel_info, hopping_schedule, unicast_schedule);
    uint16_t length;
    if (unicast_schedule) {
        length = 3;
    } else {
        length = 9;
    }
    length += ws_wp_generic_shedule_length_get(&generic_channel_info);
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


uint8_t *ws_wh_fc_write(uint8_t *ptr, ws_fc_ie_t *fc_ie)
{
    ptr = ws_wh_header_base_write(ptr, 2, WH_IE_FC_TYPE);
    *ptr++ = fc_ie->tx_flow_ctrl;
    *ptr++ = fc_ie->rx_flow_ctrl;
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

uint8_t *ws_wh_lutt_write(uint8_t *ptr, uint8_t message_type)
{
    ptr = ws_wh_header_base_write(ptr, ws_wh_lutt_length(), WH_IE_LUTT_TYPE);
    *ptr++ = message_type;
    //Set 0 for next 5 bytes which will be initializwed by FHSS
    memset(ptr, 0, 5); /* Unicast Slot Number 2 bytes, UFSI 3 bytes */
    ptr += 5;
    return ptr;
}

uint8_t *ws_wh_lus_write(uint8_t *ptr, struct ws_lus_ie *lus_ptr)
{
    ptr = ws_wh_header_base_write(ptr, ws_wh_lus_length(), WH_IE_LUS_TYPE);
    ptr = common_write_24_bit_inverse(lus_ptr->listen_interval, ptr);
    *ptr++ = lus_ptr->channel_plan_tag;
    return ptr;
}

uint8_t *ws_wh_flus_write(uint8_t *ptr, struct ws_flus_ie *flus_ptr)
{
    ptr = ws_wh_header_base_write(ptr, ws_wh_flus_length(), WH_IE_FLUS_TYPE);
    *ptr++ = flus_ptr->dwell_interval;
    *ptr++ = flus_ptr->channel_plan_tag;
    return ptr;
}

uint8_t *ws_wh_lbt_write(uint8_t *ptr)
{
    ptr = ws_wh_header_base_write(ptr, ws_wh_lbt_length(), WH_IE_LBT_TYPE);
    //Set 0 for next 5 bytes which will be initializwed by FHSS
    memset(ptr, 0, ws_wh_lbt_length()); /* LFN Broadcast Slot Number 2 bytes,  LFN Broadcast Interval Offset 3 bytes */
    ptr += ws_wh_lbt_length();
    return ptr;

}

uint8_t *ws_wh_lbs_write(uint8_t *ptr, struct ws_lbs_ie *lbs_ptr)
{
    ptr = ws_wh_header_base_write(ptr, ws_wh_lbs_length(), WH_IE_LBS_TYPE);
    ptr = common_write_24_bit_inverse(lbs_ptr->broadcast_interval, ptr);
    ptr = common_write_16_bit_inverse(lbs_ptr->broadcast_secheduler_id, ptr);
    *ptr++ = lbs_ptr->channel_plan_tag;
    return ptr;
}

uint16_t ws_wh_nr_length(struct ws_nr_ie *nr_ptr)
{
    uint16_t length;
    if (nr_ptr->node_role == WS_NR_ROLE_LFN) {
        length = 9;
    } else {
        length = 3;
    }
    return length;
}


uint8_t *ws_wh_nr_write(uint8_t *ptr, struct ws_nr_ie *nr_ptr)
{
    ptr = ws_wh_header_base_write(ptr, ws_wh_nr_length(nr_ptr), WH_IE_NR_TYPE);
    *ptr++ = nr_ptr->node_role;
    *ptr++ = nr_ptr->clock_drift;
    *ptr++ = nr_ptr->timing_accurancy;
    if (nr_ptr->node_role == WS_NR_ROLE_LFN) {
        ptr = common_write_24_bit_inverse(nr_ptr->listen_interval_min, ptr);
        ptr = common_write_24_bit_inverse(nr_ptr->listen_interval_max, ptr);
    }
    return ptr;
}

uint8_t *ws_wh_lnd_write(uint8_t *ptr, struct ws_lnd_ie *lnd_ptr)
{
    ptr = ws_wh_header_base_write(ptr, ws_wh_lnd_length(), WH_IE_LND_TYPE);
    *ptr++ = lnd_ptr->response_threshold;
    ptr = common_write_24_bit_inverse(lnd_ptr->response_delay, ptr);
    *ptr++ = lnd_ptr->discovery_slot_time;
    *ptr++ = lnd_ptr->discovery_slots;
    ptr = common_write_16_bit_inverse(lnd_ptr->discovery_first_slot, ptr);
    return ptr;
}

uint8_t *ws_wh_lto_write(uint8_t *ptr, struct ws_lto_ie *lto_ptr)
{
    ptr = ws_wh_header_base_write(ptr, ws_wh_lto_length(), WH_IE_LTO_TYPE);
    ptr = common_write_24_bit_inverse(lto_ptr->offset, ptr);
    ptr = common_write_24_bit_inverse(lto_ptr->adjusted_listening_interval, ptr);
    return ptr;
}


uint8_t *ws_wh_panid_write(uint8_t *ptr, uint16_t pana_id)
{
    ptr = ws_wh_header_base_write(ptr, ws_wh_panid_length(), WH_IE_PANID_TYPE);
    //TODO need to be check byte order
    ptr = common_write_16_bit_inverse(pana_id, ptr);
    return ptr;
}



uint8_t *ws_wp_base_write(uint8_t *ptr, uint16_t length)
{
    return mac_ie_payload_base_write(ptr, WS_WP_NESTED_IE, length);
}

static uint8_t ws_wp_channel_info_base_get(ws_generic_channel_info_t *generic_channel_info)
{
    uint8_t channel_info_base = 0;
    channel_info_base = generic_channel_info->channel_plan;
    channel_info_base |= (generic_channel_info->channel_function << 3);
    //Set Excluded Channel control part
    channel_info_base |= (generic_channel_info->excluded_channel_ctrl << 6);

    return channel_info_base;
}

static uint8_t *ws_wp_channel_plan_write(uint8_t *ptr, ws_generic_channel_info_t *generic_channel_info)
{
    switch (generic_channel_info->channel_plan) {
        case 0:
            //Regulator domain and operationg class inline
            *ptr++ = generic_channel_info->plan.zero.regulator_domain;
            *ptr++ = generic_channel_info->plan.zero.operation_class;
            break;
        case 1:
            //CHo, Channel spasing and number of channel's inline
            ptr = common_write_24_bit_inverse(generic_channel_info->plan.one.ch0 * 100, ptr);
            *ptr++ = generic_channel_info->plan.one.channel_spacing;
            ptr = common_write_16_bit_inverse(generic_channel_info->plan.one.number_of_channel, ptr);
            break;
        case 2:
            *ptr++ = generic_channel_info->plan.two.regulator_domain;
            *ptr++ = generic_channel_info->plan.two.channel_plan_id;
            break;
        default:
            break;
    }
    return ptr;
}

static uint8_t *ws_wp_channel_function_write(uint8_t *ptr, ws_generic_channel_info_t *generic_channel_info)
{
    switch (generic_channel_info->channel_function) {
        case 0:
            //Fixed channel inline
            ptr = common_write_16_bit_inverse(generic_channel_info->function.zero.fixed_channel, ptr);
            break;
        case 1:
        case 2:
            //No Inline
            break;
        case 3:
            //TODO do this properly
            //Hop count + channel hop list
            if (generic_channel_info->function.three.channel_list && generic_channel_info->function.three.channel_hop_count) {
                *ptr++ = generic_channel_info->function.three.channel_hop_count;
                memcpy(ptr, generic_channel_info->function.three.channel_list, generic_channel_info->function.three.channel_hop_count);
                ptr += generic_channel_info->function.three.channel_hop_count;
            } else {
                *ptr++ = 1;
                *ptr++ = 0;
            }

            break;
        default:
            break;

    }
    return ptr;
}

static uint8_t *ws_wp_nested_excluded_channel_write(uint8_t *ptr, ws_generic_channel_info_t *generic_channel_info)
{
    if (generic_channel_info->excluded_channel_ctrl == WS_EXC_CHAN_CTRL_RANGE) {
        uint8_t range_length = generic_channel_info->excluded_channels.range.excluded_range_length;
        ws_excluded_channel_range_data_t *range_ptr = generic_channel_info->excluded_channels.range.exluded_range;
        *ptr++ = range_length;
        while (range_length) {
            ptr = common_write_16_bit_inverse(range_ptr->range_start, ptr);
            ptr = common_write_16_bit_inverse(range_ptr->range_end, ptr);
            range_length--;
            range_ptr++;
        }
    } else if (generic_channel_info->excluded_channel_ctrl == WS_EXC_CHAN_CTRL_BITMASK) {
        //Set Mask
        uint16_t channel_mask_length = generic_channel_info->excluded_channels.mask.channel_mask_bytes_inline * 8;

        for (uint8_t i = 0; i < 8; i++) {
            uint32_t mask_value = generic_channel_info->excluded_channels.mask.channel_mask[i];
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
    return ptr;
}

uint8_t *ws_wp_nested_hopping_schedule_write(uint8_t *ptr, struct ws_hopping_schedule_s *hopping_schedule, bool unicast_schedule)
{
    //Calculate length
    ws_generic_channel_info_t generic_channel_info;

    ws_generic_channel_info_init(hopping_schedule, &generic_channel_info, unicast_schedule);
    ws_wp_channel_plan_set(&generic_channel_info, hopping_schedule);
    ws_wp_channel_function_set(&generic_channel_info, hopping_schedule, unicast_schedule);

    uint16_t length;
    if (unicast_schedule) {
        length = 3;
    } else {
        length = 9;
    }
    length += ws_wp_generic_shedule_length_get(&generic_channel_info);

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

    // Write a generic part of shedule
    *ptr++ = ws_wp_channel_info_base_get(&generic_channel_info);
    ptr = ws_wp_channel_plan_write(ptr, &generic_channel_info);
    ptr = ws_wp_channel_function_write(ptr, &generic_channel_info);
    ptr = ws_wp_nested_excluded_channel_write(ptr, &generic_channel_info);

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
    /* FAN 1.1 specific write */
    if (pan_congiguration->version > WS_FAN_VERSION_1_0) {
        temp8 |= (pan_congiguration->lfn_window_style << 2);
    }
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

uint16_t ws_wp_nested_pcap_length(uint8_t list_length)
{
    uint16_t lenght = (list_length * 3) + 1;
    return lenght;
}

void ws_ie_lib_phy_cap_list_update(struct ws_phy_cap_info *phy_pap, struct ws_pcap_ie *pcap)
{
    for (int i = 0; i < phy_pap->length_of_list; i++) {
        if (phy_pap->pcap[i].phy_type == pcap->phy_type) {
            phy_pap->pcap[i].operating_mode |= pcap->operating_mode;
            return;
        }
    }

    if (phy_pap->length_of_list == 7) {
        return;
    }

    phy_pap->pcap[phy_pap->length_of_list].phy_type = pcap->phy_type;
    phy_pap->pcap[phy_pap->length_of_list].operating_mode |= pcap->operating_mode;
    phy_pap->length_of_list++;
}


ws_pcap_ie_t ws_ie_lib_generate_phy_cap_from_phy_mode_id(uint8_t phy_mode_id)
{
    ws_pcap_ie_t pcap;
    if (phy_mode_id > 96) {
        pcap.operating_mode = 0;
        return pcap;
    }

    if (phy_mode_id < 16) {
        pcap.phy_type = WS_PHY_TYPE_ID_FSK;
    } else if (phy_mode_id < 32) {
        phy_mode_id -= 16;
        pcap.phy_type = WS_PHY_TYPE_ID_FSK_FEC;
    } else if (phy_mode_id < 48) {
        phy_mode_id -= 32;
        pcap.phy_type = WS_PHY_TYPE_ID_OFDM1;
    } else if (phy_mode_id < 64) {
        phy_mode_id -= 48;
        pcap.phy_type = WS_PHY_TYPE_ID_OFDM2;
    } else if (phy_mode_id < 80) {
        phy_mode_id -= 64;
        pcap.phy_type = WS_PHY_TYPE_ID_OFDM3;
    } else {
        phy_mode_id -= 80;
        pcap.phy_type = WS_PHY_TYPE_ID_OFDM4;
    }
    pcap.operating_mode = 1 << phy_mode_id;
    return pcap;
}

uint8_t  ws_ie_lib_phy_mode_id_get_from_phy_cap(ws_pcap_ie_t *phy_cap)
{
    if (!phy_cap->operating_mode) {
        return 0;
    }

    uint8_t phy_mode_id = 0;
    for (uint8_t i = 0; i < 16; i++) {
        if (phy_cap->operating_mode & (1 << (15 - i))) {
            phy_mode_id = 15 - i;
            break;
        }
    }

    switch (phy_cap->phy_type) {
        case WS_PHY_TYPE_ID_FSK:
            break;
        case WS_PHY_TYPE_ID_FSK_FEC:
            phy_mode_id += 16;
            break;
        case WS_PHY_TYPE_ID_OFDM1:
            phy_mode_id += 32;
            break;
        case WS_PHY_TYPE_ID_OFDM2:
            phy_mode_id += 48;
            break;
        case WS_PHY_TYPE_ID_OFDM3:
            phy_mode_id += 64;
            break;
        case WS_PHY_TYPE_ID_OFDM4:
            phy_mode_id += 80;
            break;
        default:
            break;
    }

    return phy_mode_id;

}

uint8_t *ws_wp_nested_pcap_write(uint8_t *ptr, struct ws_phy_cap_info *pcap_list)
{
    uint16_t lenght = ws_wp_nested_pcap_length(pcap_list->length_of_list);

    ptr = mac_ie_nested_ie_short_base_write(ptr, WP_PAYLOAD_IE_PCAP_TYPE, lenght);
    *ptr++ = pcap_list->length_of_list;
    for (int i = 0; i < pcap_list->length_of_list; i++) {
        *ptr++ = pcap_list->pcap[i].phy_type;
        ptr = common_write_16_bit_inverse(pcap_list->pcap[i].operating_mode, ptr);
    }
    return ptr;
}


uint8_t *ws_wp_nested_lfn_version_write(uint8_t *ptr, struct ws_lfnver_ie *ws_lfnver)
{
    ptr = mac_ie_nested_ie_short_base_write(ptr, WP_PAYLOAD_IE_LFNVER_TYPE, ws_wp_nested_lfn_version_length());
    ptr = common_write_16_bit_inverse(ws_lfnver->lfn_version, ptr);

    return ptr;
}


uint16_t ws_wp_lgtk_hash_length(struct ws_lgtkhash_ie *ws_lgtkhash)
{
    uint16_t length = 1;

    if (ws_lgtkhash->lgtk0) {
        length += 8;
    }

    if (ws_lgtkhash->lgtk1) {
        length += 8;
    }

    if (ws_lgtkhash->lgtk2) {
        length += 8;
    }
    return length;
}

uint8_t *ws_wp_nested_lgtk_hash_write(uint8_t *ptr, struct ws_lgtkhash_ie *ws_lgtkhash)
{
    uint16_t length = ws_wp_lgtk_hash_length(ws_lgtkhash);

    ptr = mac_ie_nested_ie_short_base_write(ptr, WP_PAYLOAD_IE_LGTKHASH_TYPE, length);

    uint8_t temp8 = 0;
    temp8 |= (ws_lgtkhash->lgtk0 << 0);
    temp8 |= (ws_lgtkhash->lgtk1 << 1);
    temp8 |= (ws_lgtkhash->lgtk2 << 2);
    temp8 |= (ws_lgtkhash->active_lgtk_index << 3);

    *ptr++ = temp8;
    if (ws_lgtkhash->lgtk0) {
        memcpy(ptr, ws_lgtkhash->lgtk0_hash, 8);
        ptr += 8;
    }

    if (ws_lgtkhash->lgtk1) {
        memcpy(ptr, ws_lgtkhash->lgtk1_hash, 8);
        ptr += 8;
    }

    if (ws_lgtkhash->lgtk2) {
        memcpy(ptr, ws_lgtkhash->lgtk2_hash, 8);
        ptr += 8;
    }

    return ptr;
}

uint16_t ws_wp_nested_lfn_channel_plan_length(ws_generic_channel_info_t *ws_lcp)
{
    uint16_t length = 1; //Channel Plan Tag

    length += ws_wp_generic_shedule_length_get(ws_lcp);
    return length;
}

uint8_t *ws_wp_nested_lfn_channel_plan_write(uint8_t *ptr, ws_generic_channel_info_t *ws_lcp, uint8_t plan_tag_id)
{
    uint16_t length = ws_wp_nested_lfn_channel_plan_length(ws_lcp);

    ptr = mac_ie_nested_ie_long_base_write(ptr, WP_PAYLOAD_IE_LFN_CHANNEL_PLAN_TYPE, length);
    *ptr++ = plan_tag_id;
    *ptr++ = ws_wp_channel_info_base_get(ws_lcp);
    ptr = ws_wp_channel_plan_write(ptr, ws_lcp);
    ptr = ws_wp_channel_function_write(ptr, ws_lcp);
    ptr = ws_wp_nested_excluded_channel_write(ptr, ws_lcp);
    return ptr;

}



bool ws_wh_utt_read(uint8_t *data, uint16_t length, struct ws_utt_ie *utt_ie)
{
    mac_header_IE_t utt_ie_data;
    utt_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (4 > mac_ie_header_sub_id_discover(data, length, &utt_ie_data, WH_IE_UTT_TYPE)) {
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
    if (5 > mac_ie_header_sub_id_discover(data, length, &btt_ie_data, WH_IE_BT_TYPE)) {
        return false;
    }
    data = btt_ie_data.content_ptr;
    bt_ie->broadcast_slot_number = common_read_16_bit_inverse(data);
    bt_ie->broadcast_interval_offset = common_read_24_bit_inverse(data + 2);
    return true;
}

bool ws_wh_fc_read(uint8_t *data, uint16_t length, struct ws_fc_ie *fc_ie)
{
    mac_header_IE_t fc_ie_data;
    fc_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (2 > mac_ie_header_sub_id_discover(data, length, &fc_ie_data, WH_IE_FC_TYPE)) {
        return false;
    }
    data = fc_ie_data.content_ptr;
    fc_ie->tx_flow_ctrl = *data++;
    fc_ie->rx_flow_ctrl = *data;
    return true;
}

bool ws_wh_rsl_read(uint8_t *data, uint16_t length, int8_t *rsl)
{
    mac_header_IE_t rsl_ie_data;
    rsl_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (1 > mac_ie_header_sub_id_discover(data, length, &rsl_ie_data, WH_IE_RSL_TYPE)) {
        return false;
    }
    *rsl = *rsl_ie_data.content_ptr;

    return true;
}

bool ws_wh_ea_read(uint8_t *data, uint16_t length, uint8_t *eui64)
{
    mac_header_IE_t rsl_ie_data;
    rsl_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (8 > mac_ie_header_sub_id_discover(data, length, &rsl_ie_data, WH_IE_EA_TYPE)) {
        return false;
    }
    memcpy(eui64, rsl_ie_data.content_ptr, 8);

    return true;
}

bool ws_wh_lutt_read(uint8_t *data, uint16_t length, struct ws_lutt_ie *ws_lutt)
{

    mac_header_IE_t lutt_ie_data;
    lutt_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (ws_wh_lutt_length() > mac_ie_header_sub_id_discover(data, length, &lutt_ie_data, WH_IE_LUTT_TYPE)) {
        return false;
    }
    data = lutt_ie_data.content_ptr;
    ws_lutt->message_type = *data++;
    ws_lutt->slot_number = common_read_16_bit_inverse(data);
    ws_lutt->interval_offset = common_read_24_bit_inverse(data + 2);

    return true;
}

bool ws_wh_lus_read(uint8_t *data, uint16_t length, struct ws_lus_ie *lus_ptr)
{
    mac_header_IE_t lus_ie_data;
    lus_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (ws_wh_lus_length() > mac_ie_header_sub_id_discover(data, length, &lus_ie_data, WH_IE_LUS_TYPE)) {
        return false;
    }
    data = lus_ie_data.content_ptr;
    lus_ptr->listen_interval = common_read_24_bit_inverse(data);
    data += 3;
    lus_ptr->channel_plan_tag = *data;

    return true;
}

bool ws_wh_flus_read(uint8_t *data, uint16_t length, struct ws_flus_ie *flus_ptr)
{
    mac_header_IE_t flus_ie_data;
    flus_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (ws_wh_flus_length() > mac_ie_header_sub_id_discover(data, length, &flus_ie_data, WH_IE_FLUS_TYPE)) {
        return false;
    }
    data = flus_ie_data.content_ptr;
    flus_ptr->dwell_interval = *data++;
    flus_ptr->channel_plan_tag = *data;

    return true;
}

bool ws_wh_lbt_read(uint8_t *data, uint16_t length, struct ws_lbt_ie *ws_lbt)
{
    mac_header_IE_t lbt_ie_data;
    lbt_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (ws_wh_lbt_length() > mac_ie_header_sub_id_discover(data, length, &lbt_ie_data, WH_IE_LBT_TYPE)) {
        return false;
    }
    data = lbt_ie_data.content_ptr;
    ws_lbt->slot_number = common_read_16_bit_inverse(data);
    ws_lbt->interval_offset = common_read_24_bit_inverse(data + 2);

    return true;
}

bool ws_wh_lbs_read(uint8_t *data, uint16_t length, struct ws_lbs_ie *lbs_ptr)
{
    mac_header_IE_t lbs_ie_data;
    lbs_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (ws_wh_lbs_length() > mac_ie_header_sub_id_discover(data, length, &lbs_ie_data, WH_IE_LBS_TYPE)) {
        return false;
    }

    data = lbs_ie_data.content_ptr;
    lbs_ptr->broadcast_interval = common_read_24_bit_inverse(data);
    data += 3;
    lbs_ptr->broadcast_secheduler_id = common_read_16_bit_inverse(data);
    data += 2;
    lbs_ptr->channel_plan_tag = *data;

    return true;
}

bool ws_wh_nr_read(uint8_t *data, uint16_t length, struct ws_nr_ie *nr_ptr)
{
    mac_header_IE_t nr_ie_data;
    nr_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (3 > mac_ie_header_sub_id_discover(data, length, &nr_ie_data, WH_IE_NR_TYPE)) {
        return false;
    }
    data = nr_ie_data.content_ptr;
    nr_ptr->node_role = *data++ & 7;
    nr_ptr->clock_drift = *data++;
    nr_ptr->timing_accurancy = *data++;
    switch (nr_ptr->node_role) {
        case WS_NR_ROLE_BR:
            break;
        case WS_NR_ROLE_ROUTER:
            break;
        case WS_NR_ROLE_LFN:
            if (9 > nr_ie_data.length) {
                return false;
            }
            nr_ptr->listen_interval_min = common_read_24_bit_inverse(data);
            nr_ptr->listen_interval_max = common_read_24_bit_inverse(data + 3);
            break;
        default:
            return false;
    }

    return true;
}

bool ws_wh_lnd_read(uint8_t *data, uint16_t length, struct ws_lnd_ie *lnd_ptr)
{
    mac_header_IE_t lnd_ie_data;
    lnd_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (ws_wh_lnd_length() > mac_ie_header_sub_id_discover(data, length, &lnd_ie_data, WH_IE_LND_TYPE)) {
        return false;
    }

    data = lnd_ie_data.content_ptr;
    lnd_ptr->response_threshold = *data++;
    lnd_ptr->response_delay = common_read_24_bit_inverse(data);
    data += 3;
    lnd_ptr->discovery_slot_time = *data++;
    lnd_ptr->discovery_slots = *data++;
    lnd_ptr->discovery_first_slot = common_read_16_bit_inverse(data);

    return true;
}

bool ws_wh_lto_read(uint8_t *data, uint16_t length, struct ws_lto_ie *lto_ptr)
{
    mac_header_IE_t lto_ie_data;
    lto_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (ws_wh_lto_length() > mac_ie_header_sub_id_discover(data, length, &lto_ie_data, WH_IE_LTO_TYPE)) {
        return false;
    }
    data = lto_ie_data.content_ptr;

    lto_ptr->offset = common_read_24_bit_inverse(data);
    lto_ptr->adjusted_listening_interval = common_read_24_bit_inverse(data + 3);

    return true;
}

bool ws_wh_panid_read(uint8_t *data, uint16_t length, struct ws_panid_ie *ws_panid)
{
    mac_header_IE_t panid_ie_data;
    panid_ie_data.id = MAC_HEADER_ASSIGNED_EXTERNAL_ORG_IE_ID;
    if (ws_wh_panid_length() > mac_ie_header_sub_id_discover(data, length, &panid_ie_data, WH_IE_PANID_TYPE)) {
        return false;
    }

    ws_panid->panid = common_read_16_bit_inverse(panid_ie_data.content_ptr);

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

static uint8_t *ws_channel_plan_two_read(uint8_t *ptr, ws_channel_plan_two_t *plan)
{
    plan->regulator_domain = *ptr++;
    plan->channel_plan_id = *ptr++;
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
    if (4 > mac_ie_nested_discover(data, length, &nested_payload_ie)) {
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
        case 2:
            data = ws_channel_plan_two_read(data, &us_ie->plan.two);
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
    if (10 > mac_ie_nested_discover(data, length, &nested_payload_ie)) {
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
        case 2:
            data = ws_channel_plan_two_read(data, &bs_ie->plan.two);
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
    if (5 > mac_ie_nested_discover(data, length, &nested_payload_ie)) {
        return false;
    }

    pan_congiguration->pan_size = common_read_16_bit_inverse(nested_payload_ie.content_ptr);
    pan_congiguration->routing_cost = common_read_16_bit_inverse(nested_payload_ie.content_ptr + 2);
    pan_congiguration->use_parent_bs = (nested_payload_ie.content_ptr[4] & 0x01) == 0x01;
    pan_congiguration->rpl_routing_method = (nested_payload_ie.content_ptr[4] & 0x02) == 0x02;
    pan_congiguration->version = (nested_payload_ie.content_ptr[4] & 0xe0) >> 5;
    if (pan_congiguration->version > WS_FAN_VERSION_1_0) {
        /* FAN 1.1 specific read */
        pan_congiguration->lfn_window_style = (nested_payload_ie.content_ptr[4] & 0x04) == 0x04;
    } else {
        pan_congiguration->lfn_window_style = false; //Set false for FAN 1.0
    }

    return true;
}

bool ws_wp_nested_pan_version_read(uint8_t *data, uint16_t length, uint16_t *pan_version)
{
    mac_nested_payload_IE_t nested_payload_ie;
    nested_payload_ie.id = WP_PAYLOAD_IE_PAN_VER_TYPE;
    nested_payload_ie.type_long = false;
    if (2 > mac_ie_nested_discover(data, length, &nested_payload_ie)) {
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

bool ws_wp_nested_pcap_read(uint8_t *data, uint16_t length, struct ws_phy_cap_info *ws_pcap_list)
{
#ifdef HAVE_WS_VERSION_1_1
    mac_nested_payload_IE_t nested_payload_ie;
    nested_payload_ie.id = WP_PAYLOAD_IE_PCAP_TYPE;
    nested_payload_ie.type_long = false;
    if (4 > mac_ie_nested_discover(data, length, &nested_payload_ie)) {
        return false;
    }
    //Read & Validate length
    data = nested_payload_ie.content_ptr;
    uint8_t length_of_cap = *data++ & 0x07;

    if (nested_payload_ie.length < length_of_cap * 3) {
        return false;
    }
    ws_pcap_list->length_of_list = length_of_cap;

    for (uint8_t i = 0; i < length_of_cap; i++) {
        ws_pcap_list->pcap[i].phy_type = *data++ & 7;
        ws_pcap_list->pcap[i].operating_mode = common_read_16_bit_inverse(data);
        data += 2;
    }


    return true;
#else
    (void) data;
    (void) length;
    (void) ws_pcap_list;
    return false;
#endif
}


bool ws_wp_nested_lfn_version_read(uint8_t *data, uint16_t length, struct ws_lfnver_ie *ws_lfnver)
{
    mac_nested_payload_IE_t nested_payload_ie;
    nested_payload_ie.id = WP_PAYLOAD_IE_LFNVER_TYPE;
    nested_payload_ie.type_long = false;
    if (ws_wp_nested_lfn_version_length() > mac_ie_nested_discover(data, length, &nested_payload_ie)) {
        return false;
    }

    ws_lfnver->lfn_version = common_read_16_bit_inverse(nested_payload_ie.content_ptr);
    return true;
}


bool ws_wp_nested_lgtk_hash_read(uint8_t *data, uint16_t length, struct ws_lgtkhash_ie *ws_lgtkhash)
{
    mac_nested_payload_IE_t nested_payload_ie;
    nested_payload_ie.id = WP_PAYLOAD_IE_LGTKHASH_TYPE;
    nested_payload_ie.type_long = false;
    if (1 > mac_ie_nested_discover(data, length, &nested_payload_ie)) {
        return false;
    }
    data = nested_payload_ie.content_ptr;

    ws_lgtkhash->lgtk0 = (*data & 0x01) == 0x01;
    ws_lgtkhash->lgtk1 = (*data & 0x02) == 0x02;
    ws_lgtkhash->lgtk2 = (*data & 0x04) == 0x04;
    ws_lgtkhash->active_lgtk_index = (*data & 0x18) >> 3;

    if (ws_wp_lgtk_hash_length(ws_lgtkhash) > nested_payload_ie.length) {
        return false;
    }

    data++;
    if (ws_lgtkhash->lgtk0) {
        ws_lgtkhash->lgtk0_hash = data;
        data += 8;
    } else {
        ws_lgtkhash->lgtk0_hash = NULL;
    }

    if (ws_lgtkhash->lgtk1) {
        ws_lgtkhash->lgtk1_hash = data;
        data += 8;
    } else {
        ws_lgtkhash->lgtk1_hash = NULL;
    }

    if (ws_lgtkhash->lgtk2) {
        ws_lgtkhash->lgtk2_hash = data;
    } else {
        ws_lgtkhash->lgtk2_hash = NULL;
    }

    return true;

}



bool ws_wp_nested_lfn_channel_plan_read(uint8_t *data, uint16_t length, ws_generic_channel_info_t *ws_lcp, uint8_t plan_tag_id)
{
    mac_nested_payload_IE_t nested_payload_ie;
    nested_payload_ie.id = WP_PAYLOAD_IE_LFN_CHANNEL_PLAN_TYPE;
    nested_payload_ie.type_long = true;

    if (1 <  mac_ie_nested_tagged_discover(data, length, &nested_payload_ie, plan_tag_id)) {
        return false;
    }
    //Parse Channel Plan, function and excluded channel
    data = nested_payload_ie.content_ptr;
    ws_lcp->channel_plan = (*data & 3);
    ws_lcp->channel_function = (*data & 0x38) >> 3;
    ws_lcp->excluded_channel_ctrl = (*data & 0xc0) >> 6;
    data++;
    nested_payload_ie.length--;

    uint16_t info_length = 0;
    info_length = ws_channel_plan_length(ws_lcp->channel_plan);
    if (nested_payload_ie.length < info_length) {
        return false;
    }

    nested_payload_ie.length -= info_length;
    switch (ws_lcp->channel_plan) {
        case 0:
            data = ws_channel_plan_zero_read(data, &ws_lcp->plan.zero);
            break;

        case 1:
            data = ws_channel_plan_one_read(data, &ws_lcp->plan.one);
            break;
        case 2:
            data = ws_channel_plan_two_read(data, &ws_lcp->plan.two);
            break;
        default:
            return false;

    }

    info_length = ws_channel_function_length(ws_lcp->channel_function, 0);

    if (nested_payload_ie.length < info_length) {
        return false;
    }
    nested_payload_ie.length -= info_length;


    switch (ws_lcp->channel_function) {
        case 0:
            data = ws_channel_function_zero_read(data, &ws_lcp->function.zero);
            break;

        case 1:
        case 2:
            break;

        case 3:

            data = ws_channel_function_three_read(data, &ws_lcp->function.three);
            info_length = ws_lcp->function.three.channel_hop_count;
            if (nested_payload_ie.length < info_length) {
                return false;
            }
            nested_payload_ie.length -= info_length;
            data += info_length;
            break;
        default:
            return false;

    }

    switch (ws_lcp->excluded_channel_ctrl) {
        case WS_EXC_CHAN_CTRL_NONE:

            break;
        case WS_EXC_CHAN_CTRL_RANGE:
            ws_lcp->excluded_channels.range_in.number_of_range = *data;
            if (nested_payload_ie.length < (ws_lcp->excluded_channels.range_in.number_of_range * 4) + 1) {
                return false;
            }
            //Set Range start after validation
            ws_lcp->excluded_channels.range_in.range_start = data + 1;
            break;

        case WS_EXC_CHAN_CTRL_BITMASK:
            if (ws_lcp->channel_plan == 1) {
                ws_lcp->excluded_channels.mask_in.mask_len_inline = ((ws_lcp->plan.one.number_of_channel + 7) / 8);
                if (ws_lcp->excluded_channels.mask_in.mask_len_inline != nested_payload_ie.length) {
                    //Channel mask length is not correct
                    return false;
                }
            } else {
                ws_lcp->excluded_channels.mask_in.mask_len_inline = nested_payload_ie.length;
            }

            ws_lcp->excluded_channels.mask_in.channel_mask = data;
            break;
        default:
            return false;
    }

    return true;

}

