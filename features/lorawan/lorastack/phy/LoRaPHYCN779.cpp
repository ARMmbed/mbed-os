/**
 *  @file LoRaPHYCN779.cpp
 *
 *  @brief Implements LoRaPHY for Chinese 779 MHz band
 *
 *  \code
 *   ______                              _
 *  / _____)             _              | |
 * ( (____  _____ ____ _| |_ _____  ____| |__
 *  \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *  _____) ) ____| | | || |_| ____( (___| | | |
 * (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *   (C)2013 Semtech
 *  ___ _____ _   ___ _  _____ ___  ___  ___ ___
 * / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 * \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 * |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 * embedded.connectivity.solutions===============
 *
 * \endcode
 *
 *
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *
 * Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )
 *
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "LoRaPHYCN779.h"
#include "lora_phy_ds.h"

/*!
 * Number of default channels
 */
#define CN779_NUMB_DEFAULT_CHANNELS                 3

/*!
 * Number of channels to apply for the CF list
 */
#define CN779_NUMB_CHANNELS_CF_LIST                 5

/*!
 * Minimal datarate that can be used by the node
 */
#define CN779_TX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define CN779_TX_MAX_DATARATE                       DR_7

/*!
 * Minimal datarate that can be used by the node
 */
#define CN779_RX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define CN779_RX_MAX_DATARATE                       DR_7

#define CN779_DEFAULT_MAX_DATARATE                  DR_5

/*!
 * Default datarate used by the node
 */
#define CN779_DEFAULT_DATARATE                      DR_0

/*!
 * Minimal Rx1 receive datarate offset
 */
#define CN779_MIN_RX1_DR_OFFSET                     0

/*!
 * Maximal Rx1 receive datarate offset
 */
#define CN779_MAX_RX1_DR_OFFSET                     5

/*!
 * Default Rx1 receive datarate offset
 */
#define CN779_DEFAULT_RX1_DR_OFFSET                 0

/*!
 * Minimal Tx output power that can be used by the node
 */
#define CN779_MIN_TX_POWER                          TX_POWER_5

/*!
 * Maximal Tx output power that can be used by the node
 */
#define CN779_MAX_TX_POWER                          TX_POWER_0

/*!
 * Default Tx output power used by the node
 */
#define CN779_DEFAULT_TX_POWER                      TX_POWER_0

/*!
 * Default Max EIRP
 */
#define CN779_DEFAULT_MAX_EIRP                      12.15f

/*!
 * Default antenna gain
 */
#define CN779_DEFAULT_ANTENNA_GAIN                  2.15f

/*!
 * ADR Ack limit
 */
#define CN779_ADR_ACK_LIMIT                         64

/*!
 * ADR Ack delay
 */
#define CN779_ADR_ACK_DELAY                         32

/*!
 * Enabled or disabled the duty cycle
 */
#define CN779_DUTY_CYCLE_ENABLED                    1

/*!
 * Maximum RX window duration
 */
#define CN779_MAX_RX_WINDOW                         3000

/*!
 * Receive delay 1
 */
#define CN779_RECEIVE_DELAY1                        1000

/*!
 * Receive delay 2
 */
#define CN779_RECEIVE_DELAY2                        2000

/*!
 * Join accept delay 1
 */
#define CN779_JOIN_ACCEPT_DELAY1                    5000

/*!
 * Join accept delay 2
 */
#define CN779_JOIN_ACCEPT_DELAY2                    6000

/*!
 * Maximum frame counter gap
 */
#define CN779_MAX_FCNT_GAP                          16384

/*!
 * Ack timeout
 */
#define CN779_ACKTIMEOUT                            2000

/*!
 * Random ack timeout limits
 */
#define CN779_ACK_TIMEOUT_RND                       1000

/*!
 * Verification of default datarate
 */
#if ( CN779_DEFAULT_DATARATE > DR_5 )
#error "A default DR higher than DR_5 may lead to connectivity loss."
#endif

/*!
 * Second reception window channel frequency definition.
 */
#define CN779_RX_WND_2_FREQ                         786000000

/*!
 * Second reception window channel datarate definition.
 */
#define CN779_RX_WND_2_DR                           DR_0

/*!
 * Band 0 definition
 * { DutyCycle, TxMaxPower, LastJoinTxDoneTime, LastTxDoneTime, TimeOff }
 */
static const band_t CN779_BAND0 = {100, CN779_MAX_TX_POWER, 0, 0, 0, 779500000, 786500000}; //  1.0 %

/*!
 * LoRaMac default channel 1
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t CN779_LC1 = {779500000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0};
/*!
 * LoRaMac default channel 2
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t CN779_LC2 = {779700000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0};

/*!
 * LoRaMac default channel 3
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t CN779_LC3 = {779900000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0};

/*!
 * LoRaMac channels which are allowed for the join procedure
 */
#define CN779_JOIN_CHANNELS                         (uint16_t) (LC(1) | LC(2) | LC(3))

/*!
 * Data rates table definition
 */
static const uint8_t datarates_CN779[]  = {12, 11, 10,  9,  8,  7,  7, 50};

/*!
 * Bandwidths table definition in Hz
 */
static const uint32_t bandwidths_CN779[] = {125000, 125000, 125000, 125000, 125000, 125000, 250000, 0};

/*!
 * Maximum payload with respect to the datarate index. Cannot operate with repeater.
 */
static const uint8_t max_payloads_CN779[] = {51, 51, 51, 115, 242, 242, 242, 242};

/*!
 * Maximum payload with respect to the datarate index. Can operate with repeater.
 */
static const uint8_t max_payloads_with_repeater_CN779[] = {51, 51, 51, 115, 222, 222, 222, 222};


LoRaPHYCN779::LoRaPHYCN779(LoRaWANTimeHandler &lora_time)
        : LoRaPHY(lora_time)
{
    bands[0] = CN779_BAND0;

    // Channels
    channels[0] = CN779_LC1;
    channels[0].band = 0;
    channels[1] = CN779_LC2;
    channels[1].band = 0;
    channels[2] = CN779_LC3;
    channels[2].band = 0;

    // Initialize the channels default mask
    default_channel_mask[0] = LC(1) + LC(2) + LC(3);
    // Update the channels mask
    copy_channel_mask(channel_mask, default_channel_mask, CN779_CHANNEL_MASK_SIZE);

    // set default channels
    phy_params.channels.channel_list = channels;
    phy_params.channels.channel_list_size = CN779_MAX_NB_CHANNELS;
    phy_params.channels.mask = channel_mask;
    phy_params.channels.default_mask = default_channel_mask;
    phy_params.channels.mask_size = CN779_CHANNEL_MASK_SIZE;

    // set bands for CN779 spectrum
    phy_params.bands.table = bands;
    phy_params.bands.size = CN779_MAX_NB_BANDS;

    // set bandwidths available in CN779 spectrum
    phy_params.bandwidths.table = (void *) bandwidths_CN779;
    phy_params.bandwidths.size = 8;

    // set data rates available in CN779 spectrum
    phy_params.datarates.table = (void *) datarates_CN779;
    phy_params.datarates.size = 8;

    // set payload sizes with respect to data rates
    phy_params.payloads.table = (void *) max_payloads_CN779;
    phy_params.payloads.size = 8;
    phy_params.payloads_with_repeater.table = (void *) max_payloads_with_repeater_CN779;
    phy_params.payloads_with_repeater.size = 8;

    // dwell time setting
    phy_params.ul_dwell_time_setting = 0;
    phy_params.dl_dwell_time_setting = 0;

    // set initial and default parameters
    phy_params.duty_cycle_enabled = CN779_DUTY_CYCLE_ENABLED;
    phy_params.accept_tx_param_setup_req = false;
    phy_params.fsk_supported = true;
    phy_params.cflist_supported = true;
    phy_params.dl_channel_req_supported = true;
    phy_params.custom_channelplans_supported = true;
    phy_params.default_channel_cnt = CN779_NUMB_DEFAULT_CHANNELS;
    phy_params.max_channel_cnt = CN779_MAX_NB_CHANNELS;
    phy_params.cflist_channel_cnt = CN779_NUMB_CHANNELS_CF_LIST;
    phy_params.min_tx_datarate = CN779_TX_MIN_DATARATE;
    phy_params.max_tx_datarate = CN779_TX_MAX_DATARATE;
    phy_params.min_rx_datarate = CN779_RX_MIN_DATARATE;
    phy_params.max_rx_datarate = CN779_RX_MAX_DATARATE;
    phy_params.default_datarate = CN779_DEFAULT_DATARATE;
    phy_params.default_max_datarate = CN779_DEFAULT_MAX_DATARATE;
    phy_params.min_rx1_dr_offset = CN779_MIN_RX1_DR_OFFSET;
    phy_params.max_rx1_dr_offset = CN779_MAX_RX1_DR_OFFSET;
    phy_params.default_rx1_dr_offset = CN779_DEFAULT_RX1_DR_OFFSET;
    phy_params.min_tx_power = CN779_MIN_TX_POWER;
    phy_params.max_tx_power = CN779_MAX_TX_POWER;
    phy_params.default_tx_power = CN779_DEFAULT_TX_POWER;
    phy_params.default_max_eirp = CN779_DEFAULT_MAX_EIRP;
    phy_params.default_antenna_gain = CN779_DEFAULT_ANTENNA_GAIN;
    phy_params.adr_ack_limit = CN779_ADR_ACK_LIMIT;
    phy_params.adr_ack_delay = CN779_ADR_ACK_DELAY;
    phy_params.max_rx_window = CN779_MAX_RX_WINDOW;
    phy_params.recv_delay1 = CN779_RECEIVE_DELAY1;
    phy_params.recv_delay2 = CN779_RECEIVE_DELAY2;
    phy_params.join_channel_mask = CN779_JOIN_CHANNELS;
    phy_params.join_accept_delay1 = CN779_JOIN_ACCEPT_DELAY1;
    phy_params.join_accept_delay2 = CN779_JOIN_ACCEPT_DELAY2;
    phy_params.max_fcnt_gap = CN779_MAX_FCNT_GAP;
    phy_params.ack_timeout = CN779_ACKTIMEOUT;
    phy_params.ack_timeout_rnd = CN779_ACK_TIMEOUT_RND;
    phy_params.rx_window2_datarate = CN779_RX_WND_2_DR;
    phy_params.rx_window2_frequency = CN779_RX_WND_2_FREQ;
}

LoRaPHYCN779::~LoRaPHYCN779()
{
}

