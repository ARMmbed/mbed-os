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

#include "lorastack/mac/LoRaMac.h"
#include "lorastack/mac/LoRaMacMib.h"

LoRaMacMib::LoRaMacMib()
: _lora_phy(NULL)
{
}

LoRaMacMib::~LoRaMacMib()
{
}

void LoRaMacMib::activate_mib_subsystem(LoRaPHY *phy)
{
    _lora_phy = phy;
}

lorawan_status_t LoRaMacMib::set_request(loramac_mib_req_confirm_t *mibSet,
                                        loramac_protocol_params *params)
{
    if (mibSet == NULL || _lora_phy == NULL) {
         return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    lorawan_status_t status = LORAWAN_STATUS_OK;

    switch (mibSet->type) {
        case MIB_DEVICE_CLASS: {
            params->dev_class = mibSet->param.dev_class;
            switch (params->dev_class) {
                case CLASS_A: {
                    // Set the radio into sleep to setup a defined state
                    _lora_phy->put_radio_to_sleep();
                    break;
                }
                case CLASS_B: {
                    status = LORAWAN_STATUS_UNSUPPORTED;
                    break;
                }
                case CLASS_C: {
                    // Set the is_node_ack_requested indicator to default
                    params->is_node_ack_requested = false;
                    // Set the radio into sleep mode in case we are still in RX mode
                    _lora_phy->put_radio_to_sleep();
                    // Compute Rx2 windows parameters in case the RX2 datarate has changed
                    _lora_phy->compute_rx_win_params(
                            params->sys_params.rx2_channel.datarate,
                            params->sys_params.min_rx_symb,
                            params->sys_params.max_sys_rx_error,
                            &params->rx_window2_config);
                    break;
                }
            }
            break;
        }
        case MIB_NETWORK_JOINED: {
            params->is_nwk_joined = mibSet->param.is_nwk_joined;
            break;
        }
        case MIB_ADR: {
            params->sys_params.adr_on = mibSet->param.is_adr_enable;
            break;
        }
        case MIB_NET_ID: {
            params->net_id = mibSet->param.net_id;
            break;
        }
        case MIB_DEV_ADDR: {
            params->dev_addr = mibSet->param.dev_addr;
            break;
        }
        case MIB_NWK_SKEY: {
            if (mibSet->param.nwk_skey != NULL) {
                memcpy(params->keys.nwk_skey, mibSet->param.nwk_skey,
                       sizeof(params->keys.nwk_skey));
            } else {
                status = LORAWAN_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_APP_SKEY: {
            if (mibSet->param.app_skey != NULL) {
                memcpy(params->keys.app_skey, mibSet->param.app_skey,
                       sizeof(params->keys.app_skey));
            } else {
                status = LORAWAN_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_PUBLIC_NETWORK: {
            params->is_nwk_public = mibSet->param.enable_public_nwk;
            _lora_phy->setup_public_network_mode(params->is_nwk_public);
            break;
        }
        case MIB_REPEATER_SUPPORT: {
            params->is_repeater_supported = mibSet->param.enable_repeater_support;
            break;
        }
        case MIB_RX2_CHANNEL: {
            if (_lora_phy->verify_rx_datarate(mibSet->param.rx2_channel.datarate) == true) {
                params->sys_params.rx2_channel = mibSet->param.rx2_channel;

                if ((params->dev_class == CLASS_C)
                        && (params->is_nwk_joined == true)) {
                    // We can only compute the RX window parameters directly, if we are already
                    // in class c mode and joined. We cannot setup an RX window in case of any other
                    // class type.
                    // Set the radio into sleep mode in case we are still in RX mode
                    _lora_phy->put_radio_to_sleep();
                    // Compute Rx2 windows parameters
                    _lora_phy->compute_rx_win_params(
                            params->sys_params.rx2_channel.datarate,
                            params->sys_params.min_rx_symb,
                            params->sys_params.max_sys_rx_error,
                            &params->rx_window2_config);
                }
            } else {
                status = LORAWAN_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_RX2_DEFAULT_CHANNEL: {
            if (_lora_phy->verify_rx_datarate(mibSet->param.rx2_channel.datarate) == true) {
                params->sys_params.rx2_channel = mibSet->param.default_rx2_channel;
            } else {
                status = LORAWAN_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_CHANNELS_DEFAULT_MASK:
        case MIB_CHANNELS_MASK: {
            // channel masks must not be tempered with.
            // They should be manipulated only on request with certain
            // APIs like add_channel() and remove_channel()
            // You should be able to get these MIB parameters, not set
            status = LORAWAN_STATUS_SERVICE_UNKNOWN;
            break;
        }
        case MIB_CHANNELS_NB_REP: {
            if ((mibSet->param.channel_nb_rep >= 1)
                    && (mibSet->param.channel_nb_rep <= 15)) {
                params->sys_params.retry_num = mibSet->param.channel_nb_rep;
            } else {
                status = LORAWAN_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_MAX_RX_WINDOW_DURATION: {
            params->sys_params.max_rx_win_time = mibSet->param.max_rx_window;
            break;
        }
        case MIB_RECEIVE_DELAY_1: {
            params->sys_params.recv_delay1 = mibSet->param.recv_delay1;
            break;
        }
        case MIB_RECEIVE_DELAY_2: {
            params->sys_params.recv_delay2 = mibSet->param.recv_delay2;
            break;
        }
        case MIB_JOIN_ACCEPT_DELAY_1: {
            params->sys_params.join_accept_delay1 = mibSet->param.join_accept_delay1;
            break;
        }
        case MIB_JOIN_ACCEPT_DELAY_2: {
            params->sys_params.join_accept_delay2 = mibSet->param.join_accept_delay2;
            break;
        }
        case MIB_CHANNELS_DEFAULT_DATARATE: {
            if (_lora_phy->verify_tx_datarate(mibSet->param.default_channel_data_rate, true)) {
                params->sys_params.channel_data_rate = mibSet->param.default_channel_data_rate;
            } else {
                status = LORAWAN_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_CHANNELS_DATARATE: {
            if (_lora_phy->verify_tx_datarate(mibSet->param.channel_data_rate, false) == true) {
                params->sys_params.channel_data_rate = mibSet->param.channel_data_rate;
            } else {
                status = LORAWAN_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_CHANNELS_DEFAULT_TX_POWER: {
            if (_lora_phy->verify_tx_power(mibSet->param.default_channel_tx_pwr)) {
                params->sys_params.channel_tx_power = mibSet->param.default_channel_tx_pwr;
            } else {
                status = LORAWAN_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_CHANNELS_TX_POWER: {
            if (_lora_phy->verify_tx_power(mibSet->param.channel_tx_pwr)) {
                params->sys_params.channel_tx_power = mibSet->param.channel_tx_pwr;
            } else {
                status = LORAWAN_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_UPLINK_COUNTER: {
            params->ul_frame_counter = mibSet->param.ul_frame_counter;
            break;
        }
        case MIB_DOWNLINK_COUNTER: {
            params->dl_frame_counter = mibSet->param.dl_frame_counter;
            break;
        }
        case MIB_SYSTEM_MAX_RX_ERROR: {
            params->sys_params.max_sys_rx_error = mibSet->param.max_rx_sys_error;
            break;
        }
        case MIB_MIN_RX_SYMBOLS: {
            params->sys_params.min_rx_symb = mibSet->param.min_rx_symb;
            break;
        }
        case MIB_ANTENNA_GAIN: {
            params->sys_params.antenna_gain = mibSet->param.antenna_gain;
            break;
        }
        case MIB_DEFAULT_ANTENNA_GAIN:
        {
            params->sys_params.antenna_gain = mibSet->param.default_antenna_gain;
            break;
        }
        default:
            status = LORAWAN_STATUS_SERVICE_UNKNOWN;
            break;
    }

    return status;
}

lorawan_status_t LoRaMacMib::get_request(loramac_mib_req_confirm_t *mibGet,
                                        loramac_protocol_params *params)
{
    lorawan_status_t status = LORAWAN_STATUS_OK;
    rx2_channel_params rx2_channel;

    if( mibGet == NULL )
    {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    switch( mibGet->type )
    {
        case MIB_DEVICE_CLASS:
        {
            mibGet->param.dev_class = params->dev_class;
            break;
        }
        case MIB_NETWORK_JOINED:
        {
            mibGet->param.is_nwk_joined = params->is_nwk_joined;
            break;
        }
        case MIB_ADR:
        {
            mibGet->param.is_adr_enable = params->sys_params.adr_on;
            break;
        }
        case MIB_NET_ID:
        {
            mibGet->param.net_id = params->net_id;
            break;
        }
        case MIB_DEV_ADDR:
        {
            mibGet->param.dev_addr = params->dev_addr;
            break;
        }
        case MIB_NWK_SKEY:
        {
            mibGet->param.nwk_skey =params->keys.nwk_skey;
            break;
        }
        case MIB_APP_SKEY:
        {
            mibGet->param.app_skey = params->keys.app_skey;
            break;
        }
        case MIB_PUBLIC_NETWORK:
        {
            mibGet->param.enable_public_nwk = params->is_nwk_public;
            break;
        }
        case MIB_REPEATER_SUPPORT:
        {
            mibGet->param.enable_repeater_support = params->is_repeater_supported;
            break;
        }
        case MIB_CHANNELS:
        {
            mibGet->param.channel_list = _lora_phy->get_phy_channels();
            break;
        }
        case MIB_RX2_CHANNEL:
        {
            mibGet->param.rx2_channel = params->sys_params.rx2_channel;
            break;
        }
        case MIB_RX2_DEFAULT_CHANNEL:
        {
            rx2_channel.datarate = _lora_phy->get_default_rx2_datarate();
            rx2_channel.frequency = _lora_phy->get_default_rx2_frequency();
            mibGet->param.rx2_channel = rx2_channel;
            break;
        }
        case MIB_CHANNELS_DEFAULT_MASK:
        {
            mibGet->param.default_channel_mask = _lora_phy->get_channel_mask(true);
            break;
        }
        case MIB_CHANNELS_MASK:
        {
            mibGet->param.channel_mask = _lora_phy->get_channel_mask(false);
            break;
        }
        case MIB_CHANNELS_NB_REP:
        {
            mibGet->param.channel_nb_rep = params->sys_params.retry_num;
            break;
        }
        case MIB_MAX_RX_WINDOW_DURATION:
        {
            mibGet->param.max_rx_window = params->sys_params.max_rx_win_time;
            break;
        }
        case MIB_RECEIVE_DELAY_1:
        {
            mibGet->param.recv_delay1 = params->sys_params.recv_delay1;
            break;
        }
        case MIB_RECEIVE_DELAY_2:
        {
            mibGet->param.recv_delay2 = params->sys_params.recv_delay2;
            break;
        }
        case MIB_JOIN_ACCEPT_DELAY_1:
        {
            mibGet->param.join_accept_delay1 = params->sys_params.join_accept_delay1;
            break;
        }
        case MIB_JOIN_ACCEPT_DELAY_2:
        {
            mibGet->param.join_accept_delay2 = params->sys_params.join_accept_delay2;
            break;
        }
        case MIB_CHANNELS_DEFAULT_DATARATE:
        {
            mibGet->param.default_channel_data_rate = _lora_phy->get_default_tx_datarate();
            break;
        }
        case MIB_CHANNELS_DATARATE:
        {
            mibGet->param.channel_data_rate = params->sys_params.channel_data_rate;
            break;
        }
        case MIB_CHANNELS_DEFAULT_TX_POWER:
        {
            mibGet->param.default_channel_tx_pwr = _lora_phy->get_default_tx_power();
            break;
        }
        case MIB_CHANNELS_TX_POWER:
        {
            mibGet->param.channel_tx_pwr = params->sys_params.channel_tx_power;
            break;
        }
        case MIB_UPLINK_COUNTER:
        {
            mibGet->param.ul_frame_counter = params->ul_frame_counter;
            break;
        }
        case MIB_DOWNLINK_COUNTER:
        {
            mibGet->param.dl_frame_counter = params->dl_frame_counter;
            break;
        }
        case MIB_MULTICAST_CHANNEL:
        {
            mibGet->param.multicast_list = params->multicast_channels;
            break;
        }
        case MIB_SYSTEM_MAX_RX_ERROR:
        {
            mibGet->param.max_rx_sys_error = params->sys_params.max_sys_rx_error;
            break;
        }
        case MIB_MIN_RX_SYMBOLS:
        {
            mibGet->param.min_rx_symb = params->sys_params.min_rx_symb;
            break;
        }
        case MIB_ANTENNA_GAIN:
        {
            mibGet->param.antenna_gain = params->sys_params.antenna_gain;
            break;
        }
        case MIB_DEFAULT_ANTENNA_GAIN:
        {
            mibGet->param.default_antenna_gain = params->sys_params.antenna_gain;
            break;
        }
        default:
            status = LORAWAN_STATUS_SERVICE_UNKNOWN;
            break;
    }

    return status;
}
