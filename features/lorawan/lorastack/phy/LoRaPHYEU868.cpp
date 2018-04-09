/**
 *  @file LoRaPHYEU868.cpp
 *
 *  @brief Implements LoRaPHY for European 868 MHz band
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

#include "LoRaPHYEU868.h"
#include "lora_phy_ds.h"

/*!
 * Number of default channels
 */
#define EU868_NUMB_DEFAULT_CHANNELS                 3

/*!
 * Number of channels to apply for the CF list
 */
#define EU868_NUMB_CHANNELS_CF_LIST                 5

/*!
 * Minimal datarate that can be used by the node
 */
#define EU868_TX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define EU868_TX_MAX_DATARATE                       DR_7

/*!
 * Minimal datarate that can be used by the node
 */
#define EU868_RX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define EU868_RX_MAX_DATARATE                       DR_7

/*!
 * Default datarate used by the node
 */
#define EU868_DEFAULT_DATARATE                      DR_0

#define EU868_DEFAULT_MAX_DATARATE                  DR_5

/*!
 * Minimal Rx1 receive datarate offset
 */
#define EU868_MIN_RX1_DR_OFFSET                     0

/*!
 * Maximal Rx1 receive datarate offset
 */
#define EU868_MAX_RX1_DR_OFFSET                     5

/*!
 * Default Rx1 receive datarate offset
 */
#define EU868_DEFAULT_RX1_DR_OFFSET                 0

/*!
 * Minimal Tx output power that can be used by the node
 */
#define EU868_MIN_TX_POWER                          TX_POWER_7

/*!
 * Maximal Tx output power that can be used by the node
 */
#define EU868_MAX_TX_POWER                          TX_POWER_0

/*!
 * Default Tx output power used by the node
 */
#define EU868_DEFAULT_TX_POWER                      TX_POWER_0

/*!
 * Default Max EIRP
 */
#define EU868_DEFAULT_MAX_EIRP                      16.0f

/*!
 * Default antenna gain
 */
#define EU868_DEFAULT_ANTENNA_GAIN                  2.15f

/*!
 * ADR Ack limit
 */
#define EU868_ADR_ACK_LIMIT                         64

/*!
 * ADR Ack delay
 */
#define EU868_ADR_ACK_DELAY                         32

/*!
 * Enabled or disabled the duty cycle
 */
#define EU868_DUTY_CYCLE_ENABLED                    1

/*!
 * Maximum RX window duration
 */
#define EU868_MAX_RX_WINDOW                         3000

/*!
 * Receive delay 1
 */
#define EU868_RECEIVE_DELAY1                        1000

/*!
 * Receive delay 2
 */
#define EU868_RECEIVE_DELAY2                        2000

/*!
 * Join accept delay 1
 */
#define EU868_JOIN_ACCEPT_DELAY1                    5000

/*!
 * Join accept delay 2
 */
#define EU868_JOIN_ACCEPT_DELAY2                    6000

/*!
 * Maximum frame counter gap
 */
#define EU868_MAX_FCNT_GAP                          16384

/*!
 * Ack timeout
 */
#define EU868_ACKTIMEOUT                            2000

/*!
 * Random ack timeout limits
 */
#define EU868_ACK_TIMEOUT_RND     1000

#if ( EU868_DEFAULT_DATARATE > DR_5 )
#error "A default DR higher than DR_5 may lead to connectivity loss."
#endif

/*!
 * Second reception window channel frequency definition.
 */
#define EU868_RX_WND_2_FREQ       869525000

/*!
 * Second reception window channel datarate definition.
 */
#define EU868_RX_WND_2_DR          DR_0

/*!
 * Band 0 definition
 * { DutyCycle, TxMaxPower, LastJoinTxDoneTime, LastTxDoneTime, TimeOff }
 */
static const band_t EU868_BAND0 = {100 , EU868_MAX_TX_POWER, 0, 0, 0,865000000, 868000000}; //  1.0 %
/*!
 * Band 1 definition
 * { DutyCycle, TxMaxPower, LastJoinTxDoneTime, LastTxDoneTime, TimeOff }
 */
static const band_t EU868_BAND1 = {100 , EU868_MAX_TX_POWER, 0, 0, 0, 868100000, 868600000}; //  1.0 %

/*!
 * Band 2 definition
 * Band = { DutyCycle, TxMaxPower, LastJoinTxDoneTime, LastTxDoneTime, TimeOff }
 */
static const band_t EU868_BAND2 = {1000, EU868_MAX_TX_POWER, 0, 0, 0, 868700000, 869200000}; //  0.1 %

/*!
 * Band 3 definition
 * Band = { DutyCycle, TxMaxPower, LastJoinTxDoneTime, LastTxDoneTime, TimeOff }
 */
static const band_t EU868_BAND3 = {10  , EU868_MAX_TX_POWER, 0, 0, 0, 869400000, 869650000}; // 10.0 %

/*!
 * Band 4 definition
 * Band = { DutyCycle, TxMaxPower, LastJoinTxDoneTime, LastTxDoneTime, TimeOff }
 */
static const band_t EU868_BAND4 = {100 , EU868_MAX_TX_POWER, 0, 0, 0, 869700000, 870000000}; //  1.0 %

/*!
 * Band 5 definition - It's actually a sub part of Band 2
 * Band = { DutyCycle, TxMaxPower, LastJoinTxDoneTime, LastTxDoneTime, TimeOff }
 */
static const band_t EU868_BAND5 = {1000, EU868_MAX_TX_POWER, 0, 0, 0, 863000000, 865000000}; //  0.1 %

/*!
 * LoRaMac default channel 1
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t EU868_LC1 = {868100000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1};

/*!
 * LoRaMac default channel 2
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t EU868_LC2 = {868300000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1};

/*!
 * LoRaMac default channel 3
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t EU868_LC3 = {868500000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 1};

/*!
 * LoRaMac channels which are allowed for the join procedure
 */
#define EU868_JOIN_CHANNELS        (uint16_t)(LC(1) | LC(2) | LC(3))

/*!
 * Data rates table definition
 */
static const uint8_t datarates_EU868[]  = {12, 11, 10,  9,  8,  7,  7, 50};

/*!
 * Bandwidths table definition in Hz
 */
static const uint32_t bandwidths_EU868[] = {125000, 125000, 125000, 125000, 125000, 125000, 250000, 0};

/*!
 * Maximum payload with respect to the datarate index. Cannot operate with repeater.
 */
static const uint8_t max_payloads_EU868[] = {51, 51, 51, 115, 242, 242, 242, 242};

/*!
 * Maximum payload with respect to the datarate index. Can operate with repeater.
 */
static const uint8_t max_payloads_repeater_EU868[] = {51, 51, 51, 115, 222, 222, 222, 222};

LoRaPHYEU868::LoRaPHYEU868(LoRaWANTimeHandler &lora_time)
    : LoRaPHY(lora_time)
{
    bands[0] = EU868_BAND0;
    bands[1] = EU868_BAND1;
    bands[2] = EU868_BAND2;
    bands[3] = EU868_BAND3;
    bands[4] = EU868_BAND4;
    bands[5] = EU868_BAND5;

    // Default Channels are always enabled, rest will be added later
    channels[0] = EU868_LC1;
    channels[1] = EU868_LC2;
    channels[2] = EU868_LC3;

    // Initialize the channels default mask
    default_channel_mask[0] = LC(1) + LC(2) + LC(3);
    // Update the channels mask
    copy_channel_mask(channel_mask, default_channel_mask, 1);

    // set default channels
    phy_params.channels.channel_list = channels;
    phy_params.channels.channel_list_size = EU868_MAX_NB_CHANNELS;
    phy_params.channels.mask = channel_mask;
    phy_params.channels.default_mask = default_channel_mask;
    phy_params.channels.mask_size = EU868_CHANNEL_MASK_SIZE;

    // set bands for EU868 spectrum
    phy_params.bands.table = (void *) bands;
    phy_params.bands.size = EU868_MAX_NB_BANDS;

    // set bandwidths available in EU868 spectrum
    phy_params.bandwidths.table = (void *) bandwidths_EU868;
    phy_params.bandwidths.size = 8;

    // set data rates available in EU868 spectrum
    phy_params.datarates.table = (void *) datarates_EU868;
    phy_params.datarates.size = 8;

    // set payload sizes with respect to data rates
    phy_params.payloads.table = (void *) max_payloads_EU868;
    phy_params.payloads.size = 8;
    phy_params.payloads_with_repeater.table = (void *) max_payloads_repeater_EU868;
    phy_params.payloads_with_repeater.size = 8;

    // dwell time setting
    phy_params.ul_dwell_time_setting = 0;
    phy_params.dl_dwell_time_setting = 0;

    // set initial and default parameters
    phy_params.duty_cycle_enabled = EU868_DUTY_CYCLE_ENABLED;
    phy_params.accept_tx_param_setup_req = true;
    phy_params.fsk_supported = true;
    phy_params.cflist_supported = true;
    phy_params.dl_channel_req_supported = true;
    phy_params.custom_channelplans_supported = true;
    phy_params.default_channel_cnt = EU868_NUMB_DEFAULT_CHANNELS;
    phy_params.max_channel_cnt = EU868_MAX_NB_CHANNELS;
    phy_params.cflist_channel_cnt = EU868_NUMB_CHANNELS_CF_LIST;
    phy_params.min_tx_datarate = EU868_TX_MIN_DATARATE;
    phy_params.max_tx_datarate = EU868_TX_MAX_DATARATE;
    phy_params.min_rx_datarate = EU868_RX_MIN_DATARATE;
    phy_params.max_rx_datarate = EU868_RX_MAX_DATARATE;
    phy_params.default_datarate = EU868_DEFAULT_DATARATE;
    phy_params.default_max_datarate = EU868_DEFAULT_MAX_DATARATE;
    phy_params.min_rx1_dr_offset = EU868_MIN_RX1_DR_OFFSET;
    phy_params.max_rx1_dr_offset = EU868_MAX_RX1_DR_OFFSET;
    phy_params.default_rx1_dr_offset = EU868_DEFAULT_RX1_DR_OFFSET;
    phy_params.min_tx_power = EU868_MIN_TX_POWER;
    phy_params.max_tx_power = EU868_MAX_TX_POWER;
    phy_params.default_tx_power = EU868_DEFAULT_TX_POWER;
    phy_params.default_max_eirp = EU868_DEFAULT_MAX_EIRP;
    phy_params.default_antenna_gain = EU868_DEFAULT_ANTENNA_GAIN;
    phy_params.adr_ack_limit = EU868_ADR_ACK_LIMIT;
    phy_params.adr_ack_delay = EU868_ADR_ACK_DELAY;
    phy_params.max_rx_window = EU868_MAX_RX_WINDOW;
    phy_params.recv_delay1 = EU868_RECEIVE_DELAY1;
    phy_params.recv_delay2 = EU868_RECEIVE_DELAY2;
    phy_params.join_channel_mask = EU868_JOIN_CHANNELS;
    phy_params.join_accept_delay1 = EU868_JOIN_ACCEPT_DELAY1;
    phy_params.join_accept_delay2 = EU868_JOIN_ACCEPT_DELAY2;
    phy_params.max_fcnt_gap = EU868_MAX_FCNT_GAP;
    phy_params.ack_timeout = EU868_ACKTIMEOUT;
    phy_params.ack_timeout_rnd = EU868_ACK_TIMEOUT_RND;
    phy_params.rx_window2_datarate = EU868_RX_WND_2_DR;
    phy_params.rx_window2_frequency = EU868_RX_WND_2_FREQ;
}

LoRaPHYEU868::~LoRaPHYEU868()
{
}

