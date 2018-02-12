/**
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech
 ___ _____ _   ___ _  _____ ___  ___  ___ ___
/ __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
\__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
|___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
embedded.connectivity.solutions===============

Description: LoRaWAN stack layer that controls both MAC and PHY underneath

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )


Copyright (c) 2017, Arm Limited and affiliates.

SPDX-License-Identifier: BSD-3-Clause
*/

#include "lorastack/mac/LoRaMacChannelPlan.h"

LoRaMacChannelPlan::LoRaMacChannelPlan() : _lora_phy(NULL), _mib(NULL)
{
}

LoRaMacChannelPlan::~LoRaMacChannelPlan()
{
}

void LoRaMacChannelPlan::activate_channelplan_subsystem(LoRaPHY *phy, LoRaMacMib *mib)
{
    _lora_phy = phy;
    _mib = mib;
}

lorawan_status_t LoRaMacChannelPlan::set_plan(const lorawan_channelplan_t& plan)
{
    channel_params_t mac_layer_ch_params;
    lorawan_status_t status;

    get_phy_params_t get_phy;
    phy_param_t phy_param;
    uint8_t max_num_channels;

    // Check if the PHY layer supports custom channel plans or not.
    get_phy.attribute = PHY_CUSTOM_CHANNEL_PLAN_SUPPORT;
    phy_param = _lora_phy->get_phy_params(&get_phy);

    if (!phy_param.value) {
        return LORAWAN_STATUS_SERVICE_UNKNOWN;
    }

    // Check first how many channels the selected PHY layer supports
    get_phy.attribute = PHY_MAX_NB_CHANNELS;
    phy_param = _lora_phy->get_phy_params(&get_phy);
    max_num_channels = (uint8_t) phy_param.value;

    // check if user is setting more channels than supported
    if (plan.nb_channels > max_num_channels) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    for (uint8_t i = 0; i < plan.nb_channels; i++) {

        mac_layer_ch_params.band = plan.channels[i].ch_param.band;

        mac_layer_ch_params.dr_range.fields.max = plan.channels[i].ch_param.dr_range.fields.max;
        mac_layer_ch_params.dr_range.fields.min = plan.channels[i].ch_param.dr_range.fields.min;
        mac_layer_ch_params.dr_range.value = plan.channels[i].ch_param.dr_range.value;
        mac_layer_ch_params.frequency = plan.channels[i].ch_param.frequency;
        mac_layer_ch_params.rx1_frequency = plan.channels[i].ch_param.rx1_frequency;

        status = _lora_phy->add_channel(&mac_layer_ch_params, plan.channels[i].id);

        if (status != LORAWAN_STATUS_OK) {
            return status;
        }
    }

    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacChannelPlan::get_plan(lorawan_channelplan_t& plan,
                                             loramac_protocol_params *params)
{
    if (params == NULL) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    loramac_mib_req_confirm_t mib_confirm;
    lorawan_status_t status;

    get_phy_params_t get_phy;
    phy_param_t phy_param;
    uint8_t max_num_channels;
    uint16_t *channel_mask;
    uint8_t count = 0;

    // Check if the PHY layer supports custom channel plans or not.
    get_phy.attribute = PHY_CUSTOM_CHANNEL_PLAN_SUPPORT;
    phy_param = _lora_phy->get_phy_params(&get_phy);

    if (!phy_param.value) {
        return LORAWAN_STATUS_SERVICE_UNKNOWN;
    }

    // Check first how many channels the selected PHY layer supports
    get_phy.attribute = PHY_MAX_NB_CHANNELS;
    phy_param = _lora_phy->get_phy_params(&get_phy);
    max_num_channels = (uint8_t) phy_param.value;

    // Now check the Default channel mask
    get_phy.attribute = PHY_CHANNEL_MASK;
    phy_param = _lora_phy->get_phy_params(&get_phy);
    channel_mask = phy_param.channel_mask;

    // Request Mib to get channels
    memset(&mib_confirm, 0, sizeof(mib_confirm));
    mib_confirm.type = MIB_CHANNELS;

    status = _mib->get_request(&mib_confirm, params);

    if (status != LORAWAN_STATUS_OK) {
        return status;
    }

    for (uint8_t i = 0; i < max_num_channels; i++) {
        // skip the channels which are not enabled
        if (_lora_phy->mask_bit_test(channel_mask, i) == 0) {
            continue;
        }

        // otherwise add them to the channel_plan struct
        plan.channels[count].id = i;
        plan.channels[count].ch_param.frequency = mib_confirm.param.channel_list[i].frequency;
        plan.channels[count].ch_param.dr_range.value = mib_confirm.param.channel_list[i].dr_range.value;
        plan.channels[count].ch_param.dr_range.fields.min = mib_confirm.param.channel_list[i].dr_range.fields.min;
        plan.channels[count].ch_param.dr_range.fields.max = mib_confirm.param.channel_list[i].dr_range.fields.max;
        plan.channels[count].ch_param.band = mib_confirm.param.channel_list[i].band;
        plan.channels[count].ch_param.rx1_frequency = mib_confirm.param.channel_list[i].rx1_frequency;
        count++;
    }

    plan.nb_channels = count;

    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMacChannelPlan::remove_plan()
{
    lorawan_status_t status = LORAWAN_STATUS_OK;

    get_phy_params_t get_phy;
    phy_param_t phy_param;
    uint8_t max_num_channels;
    uint16_t *channel_mask;
    uint16_t *default_channel_mask;

    // Check if the PHY layer supports custom channel plans or not.
    get_phy.attribute = PHY_CUSTOM_CHANNEL_PLAN_SUPPORT;
    phy_param = _lora_phy->get_phy_params(&get_phy);

    if (!phy_param.value) {
        return LORAWAN_STATUS_SERVICE_UNKNOWN;
    }

    // Check first how many channels the selected PHY layer supports
    get_phy.attribute = PHY_MAX_NB_CHANNELS;
    phy_param = _lora_phy->get_phy_params(&get_phy);
    max_num_channels = (uint8_t) phy_param.value;

    // Now check the channel mask for enabled channels
    get_phy.attribute = PHY_CHANNEL_MASK;
    phy_param = _lora_phy->get_phy_params(&get_phy);
    channel_mask = phy_param.channel_mask;

    // Now check the channel mask for default channels
    get_phy.attribute = PHY_DEFAULT_CHANNEL_MASK;
    phy_param = _lora_phy->get_phy_params(&get_phy);
    default_channel_mask = phy_param.channel_mask;

    for (uint8_t i = 0; i < max_num_channels; i++) {
        // skip any default channels
        if (_lora_phy->mask_bit_test(default_channel_mask, i) != 0) {
            continue;
        }

        // skip any channels which are not currently enabled
        if (_lora_phy->mask_bit_test(channel_mask, i) == 0) {
            continue;
        }

        status = remove_single_channel(i);

        if (status != LORAWAN_STATUS_OK) {
            return status;
        }
    }

    return status;
}

lorawan_status_t LoRaMacChannelPlan::remove_single_channel(uint8_t channel_id)
{
    get_phy_params_t get_phy;
    phy_param_t phy_param;
    uint8_t max_num_channels;

    // Check if the PHY layer supports custom channel plans or not.
    get_phy.attribute = PHY_CUSTOM_CHANNEL_PLAN_SUPPORT;
    phy_param = _lora_phy->get_phy_params(&get_phy);

    if (!phy_param.value) {
        return LORAWAN_STATUS_SERVICE_UNKNOWN;
    }

    // Check first how many channels the selected PHY layer supports
    get_phy.attribute = PHY_MAX_NB_CHANNELS;
    phy_param = _lora_phy->get_phy_params(&get_phy);
    max_num_channels = (uint8_t) phy_param.value;

    // According to specification channel IDs start from 0 and last valid
    // channel ID is N-1 where N=MAX_NUM_CHANNELS.
    // So any ID which is larger or equal to the Max number of channels is invalid
    if (channel_id >= max_num_channels) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    if (_lora_phy->remove_channel(channel_id) == false) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    _lora_phy->put_radio_to_sleep();

    return LORAWAN_STATUS_OK;
}

