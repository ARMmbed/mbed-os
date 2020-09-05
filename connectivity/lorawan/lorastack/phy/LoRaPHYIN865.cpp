/**
 *  @file LoRaPHYIN865.cpp
 *
 *  @brief Implements LoRaPHY for Indian 865 MHz band
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

#include "LoRaPHYIN865.h"
#include "lora_phy_ds.h"


/*!
 * Number of default channels
 */
#define IN865_NUMB_DEFAULT_CHANNELS                 3

/*!
 * Number of channels to apply for the CF list
 */
#define IN865_NUMB_CHANNELS_CF_LIST                 5

/*!
 * Minimal datarate that can be used by the node
 */
#define IN865_TX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define IN865_TX_MAX_DATARATE                       DR_7

/*!
 * Minimal datarate that can be used by the node
 */
#define IN865_RX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define IN865_RX_MAX_DATARATE                       DR_7

/*!
 * Default datarate used by the node
 */
#define IN865_DEFAULT_DATARATE                      DR_0
#define IN865_DEFAULT_MAX_DATARATE                  DR_5

/*!
 * Minimal Rx1 receive datarate offset
 */
#define IN865_MIN_RX1_DR_OFFSET                     0

/*!
 * Maximal Rx1 receive datarate offset
 */
#define IN865_MAX_RX1_DR_OFFSET                     7

/*!
 * Default Rx1 receive datarate offset
 */
#define IN865_DEFAULT_RX1_DR_OFFSET                 0

/*!
 * Minimal Tx output power that can be used by the node
 */
#define IN865_MIN_TX_POWER                          TX_POWER_10

/*!
 * Maximal Tx output power that can be used by the node
 */
#define IN865_MAX_TX_POWER                          TX_POWER_0

/*!
 * Default Tx output power used by the node
 */
#define IN865_DEFAULT_TX_POWER                      TX_POWER_0

/*!
 * Default Max EIRP
 */
#define IN865_DEFAULT_MAX_EIRP                      30.0f

/*!
 * Default antenna gain
 */
#define IN865_DEFAULT_ANTENNA_GAIN                  2.15f

/*!
 * ADR Ack limit
 */
#define IN865_ADR_ACK_LIMIT                         64

/*!
 * ADR Ack delay
 */
#define IN865_ADR_ACK_DELAY                         32

/*!
 * Enabled or disabled the duty cycle
 */
#define IN865_DUTY_CYCLE_ENABLED                    1

/*!
 * Maximum RX window duration
 */
#define IN865_MAX_RX_WINDOW                         3000

/*!
 * Receive delay 1
 */
#define IN865_RECEIVE_DELAY1                        1000

/*!
 * Receive delay 2
 */
#define IN865_RECEIVE_DELAY2                        2000

/*!
 * Join accept delay 1
 */
#define IN865_JOIN_ACCEPT_DELAY1                    5000

/*!
 * Join accept delay 2
 */
#define IN865_JOIN_ACCEPT_DELAY2                    6000

/*!
 * Maximum frame counter gap
 */
#define IN865_MAX_FCNT_GAP                          16384

/*!
 * Ack timeout
 */
#define IN865_ACKTIMEOUT                            2000

/*!
 * Random ack timeout limits
 */
#define IN865_ACK_TIMEOUT_RND                       1000

#if ( IN865_DEFAULT_DATARATE > DR_5 )
#error "A default DR higher than DR_5 may lead to connectivity loss."
#endif

/*!
 * Second reception window channel frequency definition.
 */
#define IN865_RX_WND_2_FREQ                         866550000

/*!
 * Second reception window channel datarate definition.
 */
#define IN865_RX_WND_2_DR                           DR_2

/*!
 * Band 0 definition
 * { DutyCycle, TxMaxPower, LastJoinTxDoneTime, LastTxDoneTime, TimeOff }
 */
static const band_t IN865_BAND0 = { 1, IN865_MAX_TX_POWER, 0, 0, 0, 865000000, 867000000 };  //  100.0 %

/*!
 * LoRaMac default channel 1
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t IN865_LC1 = { 865062500, 0, { ((DR_5 << 4) | DR_0) }, 0 };

/*!
 * LoRaMac default channel 2
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t IN865_LC2 = { 865402500, 0, { ((DR_5 << 4) | DR_0) }, 0 };

/*!
 * LoRaMac default channel 3
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t IN865_LC3 = { 865985000, 0, { ((DR_5 << 4) | DR_0) }, 0 };

/*!
 * LoRaMac channels which are allowed for the join procedure
 */
#define IN865_JOIN_CHANNELS                         ( uint16_t )( LC( 1 ) | LC( 2 ) | LC( 3 ) )

/*!
 * Data rates table definition
 */
static const uint8_t datarates_IN865[]  = { 12, 11, 10,  9,  8,  7,  0, 50 };

/*!
 * Bandwidths table definition in Hz
 */
static const uint32_t bandwidths_IN865[] = { 125000, 125000, 125000, 125000, 125000, 125000, 250000, 0 };

/*!
 * Maximum payload with respect to the datarate index. Cannot operate with repeater.
 */
static const uint8_t max_payloads_IN865[] = { 51, 51, 51, 115, 242, 242, 242, 242 };

/*!
 * Maximum payload with respect to the datarate index. Can operate with repeater.
 */
static const uint8_t max_payloads_with_repeater[] = { 51, 51, 51, 115, 222, 222, 222, 222 };

/*!
 * Effective datarate offsets for receive window 1.
 */
static const int8_t rx1_dr_offset_IN865[] = { 0, 1, 2, 3, 4, 5, -1, -2 };

LoRaPHYIN865::LoRaPHYIN865()
{
    bands[0] = IN865_BAND0;

    // Default Channels are always enabled, rest will be added later
    channels[0] = IN865_LC1;
    channels[0].band = 0;
    channels[1] = IN865_LC2;
    channels[1].band = 0;
    channels[2] = IN865_LC3;
    channels[2].band = 0;

    // Initialize the channels default mask
    default_channel_mask[0] = LC(1) + LC(2) + LC(3);
    // Update the channels mask
    copy_channel_mask(channel_mask, default_channel_mask, 1);

    // set default channels
    phy_params.channels.channel_list = channels;
    phy_params.channels.channel_list_size = IN865_MAX_NB_CHANNELS;
    phy_params.channels.mask = channel_mask;
    phy_params.channels.default_mask = default_channel_mask;
    phy_params.channels.mask_size = IN865_CHANNEL_MASK_SIZE;

    // set bands for IN865 spectrum
    phy_params.bands.table = (void *) bands;
    phy_params.bands.size = IN865_MAX_NB_BANDS;

    // set bandwidths available in IN865 spectrum
    phy_params.bandwidths.table = (void *) bandwidths_IN865;
    phy_params.bandwidths.size = 8;

    // set data rates available in IN865 spectrum
    phy_params.datarates.table = (void *) datarates_IN865;
    phy_params.datarates.size = 8;

    // set payload sizes with respect to data rates
    phy_params.payloads.table = (void *) max_payloads_IN865;
    phy_params.payloads.size = 8;
    phy_params.payloads_with_repeater.table = (void *) max_payloads_with_repeater;
    phy_params.payloads_with_repeater.size = 8;

    // dwell time setting
    phy_params.ul_dwell_time_setting = 0;
    phy_params.dl_dwell_time_setting = 0;

    // set initial and default parameters
    phy_params.duty_cycle_enabled = IN865_DUTY_CYCLE_ENABLED;
    phy_params.accept_tx_param_setup_req = false;
    phy_params.fsk_supported = true;
    phy_params.cflist_supported = true;
    phy_params.dl_channel_req_supported = true;
    phy_params.custom_channelplans_supported = true;
    phy_params.default_channel_cnt = IN865_NUMB_DEFAULT_CHANNELS;
    phy_params.max_channel_cnt = IN865_MAX_NB_CHANNELS;
    phy_params.cflist_channel_cnt = IN865_NUMB_CHANNELS_CF_LIST;
    phy_params.min_tx_datarate = IN865_TX_MIN_DATARATE;
    phy_params.max_tx_datarate = IN865_TX_MAX_DATARATE;
    phy_params.min_rx_datarate = IN865_RX_MIN_DATARATE;
    phy_params.max_rx_datarate = IN865_RX_MAX_DATARATE;
    phy_params.default_datarate = IN865_DEFAULT_DATARATE;
    phy_params.default_max_datarate = IN865_DEFAULT_MAX_DATARATE;
    phy_params.min_rx1_dr_offset = IN865_MIN_RX1_DR_OFFSET;
    phy_params.max_rx1_dr_offset = IN865_MAX_RX1_DR_OFFSET;
    phy_params.default_rx1_dr_offset = IN865_DEFAULT_RX1_DR_OFFSET;
    phy_params.min_tx_power = IN865_MIN_TX_POWER;
    phy_params.max_tx_power = IN865_MAX_TX_POWER;
    phy_params.default_tx_power = IN865_DEFAULT_TX_POWER;
    phy_params.default_max_eirp = IN865_DEFAULT_MAX_EIRP;
    phy_params.default_antenna_gain = IN865_DEFAULT_ANTENNA_GAIN;
    phy_params.adr_ack_limit = IN865_ADR_ACK_LIMIT;
    phy_params.adr_ack_delay = IN865_ADR_ACK_DELAY;
    phy_params.max_rx_window = IN865_MAX_RX_WINDOW;
    phy_params.recv_delay1 = IN865_RECEIVE_DELAY1;
    phy_params.recv_delay2 = IN865_RECEIVE_DELAY2;
    phy_params.join_channel_mask = IN865_JOIN_CHANNELS;
    phy_params.join_accept_delay1 = IN865_JOIN_ACCEPT_DELAY1;
    phy_params.join_accept_delay2 = IN865_JOIN_ACCEPT_DELAY2;
    phy_params.max_fcnt_gap = IN865_MAX_FCNT_GAP;
    phy_params.ack_timeout = IN865_ACKTIMEOUT;
    phy_params.ack_timeout_rnd = IN865_ACK_TIMEOUT_RND;
    phy_params.rx_window2_datarate = IN865_RX_WND_2_DR;
    phy_params.rx_window2_frequency = IN865_RX_WND_2_FREQ;
}

LoRaPHYIN865::~LoRaPHYIN865()
{
}

uint8_t LoRaPHYIN865::apply_DR_offset(int8_t dr, int8_t dr_offset)
{
    // Apply offset formula
    return MIN(DR_5, MAX(DR_0, dr - rx1_dr_offset_IN865[dr_offset]));
}
