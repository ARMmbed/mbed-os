/**
 *  @file LoRaPHYEU433.cpp
 *
 *  @brief Implements LoRaPHY for European 433 MHz band
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

#include "LoRaPHYEU433.h"
#include "lora_phy_ds.h"

/*!
 * Number of default channels
 */
#define EU433_NUMB_DEFAULT_CHANNELS                 3

/*!
 * Number of channels to apply for the CF list
 */
#define EU433_NUMB_CHANNELS_CF_LIST                 5

/*!
 * Minimal datarate that can be used by the node
 */
#define EU433_TX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define EU433_TX_MAX_DATARATE                       DR_7

/*!
 * Minimal datarate that can be used by the node
 */
#define EU433_RX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define EU433_RX_MAX_DATARATE                       DR_7

/*!
 * Default datarate used by the node
 */
#define EU433_DEFAULT_DATARATE                      DR_0

#define EU433_DEFAULT_MAX_DATARATE                  DR_5

/*!
 * Minimal Rx1 receive datarate offset
 */
#define EU433_MIN_RX1_DR_OFFSET                     0

/*!
 * Maximal Rx1 receive datarate offset
 */
#define EU433_MAX_RX1_DR_OFFSET                     5

/*!
 * Default Rx1 receive datarate offset
 */
#define EU433_DEFAULT_RX1_DR_OFFSET                 0

/*!
 * Minimal Tx output power that can be used by the node
 */
#define EU433_MIN_TX_POWER                          TX_POWER_5

/*!
 * Maximal Tx output power that can be used by the node
 */
#define EU433_MAX_TX_POWER                          TX_POWER_0

/*!
 * Default Tx output power used by the node
 */
#define EU433_DEFAULT_TX_POWER                      TX_POWER_0

/*!
 * Default Max EIRP
 */
#define EU433_DEFAULT_MAX_EIRP                      12.15f

/*!
 * Default antenna gain
 */
#define EU433_DEFAULT_ANTENNA_GAIN                  2.15f

/*!
 * ADR Ack limit
 */
#define EU433_ADR_ACK_LIMIT                         64

/*!
 * ADR Ack delay
 */
#define EU433_ADR_ACK_DELAY                         32

/*!
 * Enabled or disabled the duty cycle
 */
#define EU433_DUTY_CYCLE_ENABLED                    1

/*!
 * Maximum RX window duration
 */
#define EU433_MAX_RX_WINDOW                         3000

/*!
 * Receive delay 1
 */
#define EU433_RECEIVE_DELAY1                        1000

/*!
 * Receive delay 2
 */
#define EU433_RECEIVE_DELAY2                        2000

/*!
 * Join accept delay 1
 */
#define EU433_JOIN_ACCEPT_DELAY1                    5000

/*!
 * Join accept delay 2
 */
#define EU433_JOIN_ACCEPT_DELAY2                    6000

/*!
 * Maximum frame counter gap
 */
#define EU433_MAX_FCNT_GAP                          16384

/*!
 * Ack timeout
 */
#define EU433_ACKTIMEOUT                            2000

/*!
 * Random ack timeout limits
 */
#define EU433_ACK_TIMEOUT_RND                       1000

/*!
 * Verification of default datarate
 */
#if ( EU433_DEFAULT_DATARATE > DR_5 )
#error "A default DR higher than DR_5 may lead to connectivity loss."
#endif

/*!
 * Second reception window channel frequency definition.
 */
#define EU433_RX_WND_2_FREQ                         434665000

/*!
 * Second reception window channel datarate definition.
 */
#define EU433_RX_WND_2_DR                           DR_0

/*!
 * Band 0 definition
 * { DutyCycle, TxMaxPower, LastJoinTxDoneTime, LastTxDoneTime, TimeOff }
 */
static const band_t EU433_BAND0 = {100, EU433_MAX_TX_POWER, 0, 0, 0, 433175000, 434665000}; //  1.0 %

/*!
 * LoRaMac default channel 1
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t EU433_LC1 = {433175000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0};

/*!
 * LoRaMac default channel 2
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t EU433_LC2 = {433375000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0};

/*!
 * LoRaMac default channel 3
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t EU433_LC3 = {433575000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0};

/*!
 * LoRaMac channels which are allowed for the join procedure
 */
#define EU433_JOIN_CHANNELS                         (uint16_t) (LC(1) | LC(2) | LC(3))

/*!
 * Data rates table definition
 */
static const uint8_t datarates_EU433[] = {12, 11, 10,  9,  8,  7,  7, 50};

/*!
 * Bandwidths table definition in Hz
 */
static const uint32_t bandwidths_EU433[] = {125000, 125000, 125000, 125000, 125000, 125000, 250000, 0};

/*!
 * Maximum payload with respect to the datarate index. Cannot operate with repeater.
 */
static const uint8_t max_payloads_EU433[] = {51, 51, 51, 115, 242, 242, 242, 242};

/*!
 * Maximum payload with respect to the datarate index. Can operate with repeater.
 */
static const uint8_t max_payloads_with_repeater_EU433[] = {51, 51, 51, 115, 222, 222, 222, 222};


LoRaPHYEU433::LoRaPHYEU433(LoRaWANTimeHandler &lora_time)
        : LoRaPHY(lora_time)
{
    bands[0] = EU433_BAND0;

    // Channels
    channels[0] = EU433_LC1;
    channels[0].band = 0;
    channels[1] = EU433_LC2;
    channels[1].band = 0;
    channels[2] = EU433_LC3;
    channels[2].band = 0;

    // Initialize the channels default mask
    default_channel_mask[0] = LC(1) + LC(2) + LC(3);
    // Update the channels mask
    copy_channel_mask(channel_mask, default_channel_mask, EU433_CHANNEL_MASK_SIZE);

    // set default channels
    phy_params.channels.channel_list = channels;
    phy_params.channels.channel_list_size = EU433_MAX_NB_CHANNELS;
    phy_params.channels.mask = channel_mask;
    phy_params.channels.default_mask = default_channel_mask;
    phy_params.channels.mask_size = EU433_CHANNEL_MASK_SIZE;

    // set bands for EU433 spectrum
    phy_params.bands.table = bands;
    phy_params.bands.size = EU433_MAX_NB_BANDS;

    // set bandwidths available in EU433 spectrum
    phy_params.bandwidths.table = (void *) bandwidths_EU433;
    phy_params.bandwidths.size = 8;

    // set data rates available in EU433 spectrum
    phy_params.datarates.table = (void *) datarates_EU433;
    phy_params.datarates.size = 8;

    // set payload sizes with respect to data rates
    phy_params.payloads.table = (void *) max_payloads_EU433;
    phy_params.payloads.size = 8;
    phy_params.payloads_with_repeater.table = (void *) max_payloads_with_repeater_EU433;
    phy_params.payloads_with_repeater.size = 8;

    // dwell time setting
    phy_params.ul_dwell_time_setting = 0;
    phy_params.dl_dwell_time_setting = 0;

    // set initial and default parameters
    phy_params.duty_cycle_enabled = EU433_DUTY_CYCLE_ENABLED;
    phy_params.accept_tx_param_setup_req = false;
    phy_params.fsk_supported = true;
    phy_params.cflist_supported = true;
    phy_params.dl_channel_req_supported = true;
    phy_params.custom_channelplans_supported = true;
    phy_params.default_channel_cnt = EU433_NUMB_DEFAULT_CHANNELS;
    phy_params.max_channel_cnt = EU433_MAX_NB_CHANNELS;
    phy_params.cflist_channel_cnt = EU433_NUMB_CHANNELS_CF_LIST;
    phy_params.min_tx_datarate = EU433_TX_MIN_DATARATE;
    phy_params.max_tx_datarate = EU433_TX_MAX_DATARATE;
    phy_params.min_rx_datarate = EU433_RX_MIN_DATARATE;
    phy_params.max_rx_datarate = EU433_RX_MAX_DATARATE;
    phy_params.default_datarate = EU433_DEFAULT_DATARATE;
    phy_params.default_max_datarate = EU433_DEFAULT_MAX_DATARATE;
    phy_params.min_rx1_dr_offset = EU433_MIN_RX1_DR_OFFSET;
    phy_params.max_rx1_dr_offset = EU433_MAX_RX1_DR_OFFSET;
    phy_params.default_rx1_dr_offset = EU433_DEFAULT_RX1_DR_OFFSET;
    phy_params.min_tx_power = EU433_MIN_TX_POWER;
    phy_params.max_tx_power = EU433_MAX_TX_POWER;
    phy_params.default_tx_power = EU433_DEFAULT_TX_POWER;
    phy_params.default_max_eirp = EU433_DEFAULT_MAX_EIRP;
    phy_params.default_antenna_gain = EU433_DEFAULT_ANTENNA_GAIN;
    phy_params.adr_ack_limit = EU433_ADR_ACK_LIMIT;
    phy_params.adr_ack_delay = EU433_ADR_ACK_DELAY;
    phy_params.max_rx_window = EU433_MAX_RX_WINDOW;
    phy_params.recv_delay1 = EU433_RECEIVE_DELAY1;
    phy_params.recv_delay2 = EU433_RECEIVE_DELAY2;
    phy_params.join_channel_mask = EU433_JOIN_CHANNELS;
    phy_params.join_accept_delay1 = EU433_JOIN_ACCEPT_DELAY1;
    phy_params.join_accept_delay2 = EU433_JOIN_ACCEPT_DELAY2;
    phy_params.max_fcnt_gap = EU433_MAX_FCNT_GAP;
    phy_params.ack_timeout = EU433_ACKTIMEOUT;
    phy_params.ack_timeout_rnd = EU433_ACK_TIMEOUT_RND;
    phy_params.rx_window2_datarate = EU433_RX_WND_2_DR;
    phy_params.rx_window2_frequency = EU433_RX_WND_2_FREQ;
}

LoRaPHYEU433::~LoRaPHYEU433()
{
}
