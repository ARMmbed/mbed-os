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

LoRaMacStatus_t LoRaMacChannelPlan::set_plan(const lora_channelplan_t& plan)
{
    ChannelAddParams_t channelAdd;
    ChannelParams_t mac_layer_ch_params;
    LoRaMacStatus_t status;

    GetPhyParams_t get_phy;
    PhyParam_t phy_param;
    uint8_t max_num_channels;

    // Check first how many channels the selected PHY layer supports
    get_phy.Attribute = PHY_MAX_NB_CHANNELS;
    phy_param = _lora_phy->get_phy_params(&get_phy);
    max_num_channels = (uint8_t) phy_param.Value;

    // check if user is setting more channels than supported
    if (plan.nb_channels > max_num_channels) {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    for (uint8_t i = 0; i < plan.nb_channels; i++) {
        mac_layer_ch_params.Band = plan.channels[i].ch_param.band;
        mac_layer_ch_params.DrRange.Fields.Max =
                plan.channels[i].ch_param.dr_range.lora_mac_fields_s.max;
        mac_layer_ch_params.DrRange.Fields.Min =
                plan.channels[i].ch_param.dr_range.lora_mac_fields_s.min;
        mac_layer_ch_params.DrRange.Value =
                plan.channels[i].ch_param.dr_range.value;
        mac_layer_ch_params.Frequency =
                plan.channels[i].ch_param.frequency;
        mac_layer_ch_params.Rx1Frequency =
                plan.channels[i].ch_param.rx1_frequency;

        channelAdd.ChannelId = plan.channels[i].id;
        channelAdd.NewChannel = &mac_layer_ch_params;

        status = _lora_phy->add_channel(&channelAdd);

        if (status != LORAMAC_STATUS_OK) {
            return status;
        }
    }

    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t LoRaMacChannelPlan::get_plan(lora_channelplan_t& plan,
                                             lora_mac_protocol_params *params)
{
    if (params == NULL) {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    MibRequestConfirm_t mib_params;
    LoRaMacStatus_t status;

    GetPhyParams_t get_phy;
    PhyParam_t phy_param;
    uint8_t max_num_channels;
    uint16_t *channel_masks;
    uint8_t count = 0;

    // Check first how many channels the selected PHY layer supports
    get_phy.Attribute = PHY_MAX_NB_CHANNELS;
    phy_param = _lora_phy->get_phy_params(&get_phy);
    max_num_channels = (uint8_t) phy_param.Value;

    // Now check the Default channel mask
    get_phy.Attribute = PHY_CHANNELS_MASK;
    phy_param = _lora_phy->get_phy_params(&get_phy);
    channel_masks = phy_param.ChannelsMask;

    // Request Mib to get channels
    memset(&mib_params, 0, sizeof(mib_params));
    mib_params.Type = MIB_CHANNELS;

    status = _mib->get_request(&mib_params, params);

    if (status != LORAMAC_STATUS_OK) {
        return status;
    }

    for (uint8_t i = 0; i < max_num_channels; i++) {
        // skip the channels which are not enabled
        if ((channel_masks[0] & (1U << i)) == 0) {
            continue;
        }

        // otherwise add them to the channel_plan struct
        plan.channels[count].id = i;
        plan.channels[count].ch_param.frequency = mib_params.Param.ChannelList[i].Frequency;
        plan.channels[count].ch_param.dr_range.value = mib_params.Param.ChannelList[i].DrRange.Value;
        plan.channels[count].ch_param.dr_range.lora_mac_fields_s.min = mib_params.Param.ChannelList[i].DrRange.Fields.Min;
        plan.channels[count].ch_param.dr_range.lora_mac_fields_s.max = mib_params.Param.ChannelList[i].DrRange.Fields.Max;
        plan.channels[count].ch_param.band = mib_params.Param.ChannelList[i].Band;
        plan.channels[count].ch_param.rx1_frequency = mib_params.Param.ChannelList[i].Rx1Frequency;
        count++;
    }

    plan.nb_channels = count;

    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t LoRaMacChannelPlan::remove_plan()
{
    LoRaMacStatus_t status = LORAMAC_STATUS_OK;

    GetPhyParams_t get_phy;
    PhyParam_t phy_param;
    uint8_t max_num_channels;
    uint16_t *channel_masks;
    uint16_t *default_channel_masks;

    // Check first how many channels the selected PHY layer supports
    get_phy.Attribute = PHY_MAX_NB_CHANNELS;
    phy_param = _lora_phy->get_phy_params(&get_phy);
    max_num_channels = (uint8_t) phy_param.Value;

    // Now check the channel mask for enabled channels
    get_phy.Attribute = PHY_CHANNELS_MASK;
    phy_param = _lora_phy->get_phy_params(&get_phy);
    channel_masks = phy_param.ChannelsMask;

    // Now check the channel mask for default channels
    get_phy.Attribute = PHY_CHANNELS_DEFAULT_MASK;
    phy_param = _lora_phy->get_phy_params(&get_phy);
    default_channel_masks = phy_param.ChannelsMask;

    for (uint8_t i = 0; i < max_num_channels; i++) {
        // skip any default channels
        if ((default_channel_masks[0] & (1U<<i)) != 0) {
            continue;
        }

        // skip any channels which are not currently enabled
        if ((channel_masks[0] & (1U<<i)) == 0) {
            continue;
        }

        status = remove_single_channel(i);

        if (status != LORAMAC_STATUS_OK) {
            return status;
        }
    }

    return status;
}

LoRaMacStatus_t LoRaMacChannelPlan::remove_single_channel(uint8_t channel_id)
{
    GetPhyParams_t get_phy;
    PhyParam_t phy_param;
    uint8_t max_num_channels;
    uint16_t *channel_masks;
    ChannelRemoveParams_t channelRemove;

    // Check first how many channels the selected PHY layer supports
    get_phy.Attribute = PHY_MAX_NB_CHANNELS;
    phy_param = _lora_phy->get_phy_params(&get_phy);
    max_num_channels = (uint8_t) phy_param.Value;

    // According to specification channel IDs start from 0 and last valid
    // channel ID is N-1 where N=MAX_NUM_CHANNELS.
    // So any ID which is larger or equal to the Max number of channels is invalid
    if (channel_id >= max_num_channels) {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    // Now check the Default channel mask
    get_phy.Attribute = PHY_CHANNELS_DEFAULT_MASK;
    phy_param = _lora_phy->get_phy_params(&get_phy);
    channel_masks = phy_param.ChannelsMask;

    // check if the channel ID give belongs to a default channel
    // Mostly the default channels are in the first mask if the region
    // have multiple channel masks for various sub-bands. So we check the first
    // mask only and return an error code if user sent a default channel id
    if ((channel_masks[0] & (1U << channel_id)) != 0) {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    channelRemove.ChannelId = channel_id;

    if(_lora_phy->remove_channel(&channelRemove) == false)
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    _lora_phy->put_radio_to_sleep();

    return LORAMAC_STATUS_OK;
}

