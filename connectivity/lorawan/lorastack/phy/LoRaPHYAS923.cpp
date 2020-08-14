/**
 *  @file LoRaPHYAS923.cpp
 *
 *  @brief Implements LoRaPHY for Asia-Pacific 923 MHz band
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

#include "LoRaPHYAS923.h"
#include "lora_phy_ds.h"

/*!
 * Number of default channels
 */
#define AS923_NUMB_DEFAULT_CHANNELS                 2

/*!
 * Number of channels to apply for the CF list
 */
#define AS923_NUMB_CHANNELS_CF_LIST                 5

/*!
 * Minimal datarate that can be used by the node
 */
#define AS923_TX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define AS923_TX_MAX_DATARATE                       DR_7

/*!
 * Minimal datarate that can be used by the node
 */
#define AS923_RX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define AS923_RX_MAX_DATARATE                       DR_7

/*!
 * Default datarate used by the node
 */
#define AS923_DEFAULT_DATARATE                      DR_2

#define AS923_DEFAULT_MAX_DATARATE                  DR_5

/*!
 * The minimum datarate which is used when the
 * dwell time is limited.
 */
#define AS923_DWELL_LIMIT_DATARATE                  DR_2

/*!
 * Minimal Rx1 receive datarate offset
 */
#define AS923_MIN_RX1_DR_OFFSET                     0

/*!
 * Maximal Rx1 receive datarate offset
 */
#define AS923_MAX_RX1_DR_OFFSET                     7

/*!
 * Default Rx1 receive datarate offset
 */
#define AS923_DEFAULT_RX1_DR_OFFSET                 0

/*!
 * Minimal Tx output power that can be used by the node
 */
#define AS923_MIN_TX_POWER                          TX_POWER_7

/*!
 * Maximal Tx output power that can be used by the node
 */
#define AS923_MAX_TX_POWER                          TX_POWER_0

/*!
 * Default Tx output power used by the node
 */
#define AS923_DEFAULT_TX_POWER                      TX_POWER_0

/*!
 * Default Max EIRP
 */
#define AS923_DEFAULT_MAX_EIRP                      16.0f

/*!
 * Default antenna gain
 */
#define AS923_DEFAULT_ANTENNA_GAIN                  2.15f

/*!
 * ADR Ack limit
 */
#define AS923_ADR_ACK_LIMIT                         64

/*!
 * ADR Ack delay
 */
#define AS923_ADR_ACK_DELAY                         32

/*!
 * Enabled or disabled the duty cycle
 */
#define AS923_DUTY_CYCLE_ENABLED                    0

/*!
 * Maximum RX window duration
 */
#define AS923_MAX_RX_WINDOW                         3000

/*!
 * Receive delay 1
 */
#define AS923_RECEIVE_DELAY1                        1000

/*!
 * Receive delay 2
 */
#define AS923_RECEIVE_DELAY2                        2000

/*!
 * Join accept delay 1
 */
#define AS923_JOIN_ACCEPT_DELAY1                    5000

/*!
 * Join accept delay 2
 */
#define AS923_JOIN_ACCEPT_DELAY2                    6000

/*!
 * Maximum frame counter gap
 */
#define AS923_MAX_FCNT_GAP                          16384

/*!
 * Ack timeout
 */
#define AS923_ACKTIMEOUT                            2000

/*!
 * Random ack timeout limits
 */
#define AS923_ACK_TIMEOUT_RND                       1000

#if ( AS923_DEFAULT_DATARATE > DR_5 )
#error "A default DR higher than DR_5 may lead to connectivity loss."
#endif

/*!
 * Second reception window channel frequency definition.
 */
#define AS923_RX_WND_2_FREQ                         923200000

/*!
 * Second reception window channel datarate definition.
 */
#define AS923_RX_WND_2_DR                           DR_2

/*!
 * Band 0 definition
 * { DutyCycle, TxMaxPower, LastJoinTxDoneTime, LastTxDoneTime, TimeOff }
 */
static const band_t AS923_BAND0 = {100, AS923_MAX_TX_POWER, 0, 0, 0, 923000000, 928000000}; //  1.0 %

/*!
 * LoRaMac default channel 1
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t AS923_LC1 = { 923200000, 0, { ((DR_5 << 4) | DR_0) }, 0 };

/*!
 * LoRaMac default channel 2
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t AS923_LC2 = { 923400000, 0, { ((DR_5 << 4) | DR_0) }, 0 };

/*!
 * LoRaMac channels which are allowed for the join procedure
 */
#define AS923_JOIN_CHANNELS                         ( uint16_t )( LC( 1 ) | LC( 2 ) )

/*!
 * RSSI threshold for a free channel [dBm]
 */
#define AS923_RSSI_FREE_TH                          -85

/*!
 * Specifies the time the node performs a carrier sense
 */
#define AS923_CARRIER_SENSE_TIME                    6

/*!
 * Data rates table definition
 */
static const uint8_t datarates_AS923[]  = {12, 11, 10,  9,  8,  7, 7, 50};

/*!
 * Bandwidths table definition in Hz
 */
static const uint32_t bandwidths_AS923[] = {125000, 125000, 125000, 125000, 125000, 125000, 250000, 0};

#if (MBED_CONF_LORA_DWELL_TIME == 0)
static  const uint8_t max_payload_table[] = {51, 51, 51, 115, 242, 242, 242, 242};
static const uint8_t max_payload_table_with_repeater[] = {51, 51, 51, 115, 222, 222, 222, 222};
#else
// this is the default, i.e.,
static const uint8_t max_payload_table[] = {0, 0, 11, 53, 125, 242, 242, 242};
static const uint8_t max_payload_table_with_repeater[] =  {0, 0, 11, 53, 125, 242, 242, 242};
#endif

/*!
 * Effective datarate offsets for receive window 1.
 */
static const int8_t rx1_dr_offset_AS923[] = {0, 1, 2, 3, 4, 5, -1, -2};

LoRaPHYAS923::LoRaPHYAS923()
{
    bands[0] = AS923_BAND0;

    // Default Channels are always enabled in the channel list,
    // rest will be added later
    channels[0] = AS923_LC1;
    channels[0].band = 0;
    channels[1] = AS923_LC2;
    channels[1].band = 0;

    // Initialize the default channel mask
    default_channel_mask[0] = LC(1) + LC(2);

    // Update the channel mask list
    copy_channel_mask(channel_mask, default_channel_mask, AS923_CHANNEL_MASK_SIZE);

    // set default channels
    phy_params.channels.channel_list = channels;
    phy_params.channels.channel_list_size = AS923_MAX_NB_CHANNELS;
    phy_params.channels.mask = channel_mask;
    phy_params.channels.default_mask = default_channel_mask;
    phy_params.channels.mask_size = AS923_CHANNEL_MASK_SIZE;

    // set bands for AS923 spectrum
    phy_params.bands.table = (void *) bands;
    phy_params.bands.size = AS923_MAX_NB_BANDS;

    // set bandwidths available in AS923 spectrum
    phy_params.bandwidths.table = (void *) bandwidths_AS923;
    phy_params.bandwidths.size = 8;

    // set data rates available in AS923 spectrum
    phy_params.datarates.table = (void *) datarates_AS923;
    phy_params.datarates.size = 8;

    // set payload sizes with respect to data rates
    phy_params.payloads.table = (void *) max_payload_table;
    phy_params.payloads.size = 8;
    phy_params.payloads_with_repeater.table = (void *) max_payload_table_with_repeater;
    phy_params.payloads_with_repeater.size = 8;

    // dwell time setting, 400 ms
    phy_params.ul_dwell_time_setting = 1;
    phy_params.dl_dwell_time_setting = 1;
    phy_params.dwell_limit_datarate = AS923_DWELL_LIMIT_DATARATE;

    phy_params.duty_cycle_enabled = AS923_DUTY_CYCLE_ENABLED;
    phy_params.accept_tx_param_setup_req = true;
    phy_params.fsk_supported = true;
    phy_params.cflist_supported = true;
    phy_params.dl_channel_req_supported = true;
    phy_params.custom_channelplans_supported = true;
    phy_params.default_channel_cnt = AS923_NUMB_DEFAULT_CHANNELS;
    phy_params.max_channel_cnt = AS923_MAX_NB_CHANNELS;
    phy_params.cflist_channel_cnt = AS923_NUMB_CHANNELS_CF_LIST;
    phy_params.min_tx_datarate = AS923_TX_MIN_DATARATE;
    phy_params.max_tx_datarate = AS923_TX_MAX_DATARATE;
    phy_params.min_rx_datarate = AS923_RX_MIN_DATARATE;
    phy_params.max_rx_datarate = AS923_RX_MAX_DATARATE;
    phy_params.default_datarate = AS923_DEFAULT_DATARATE;
    phy_params.default_max_datarate = AS923_DEFAULT_MAX_DATARATE;
    phy_params.min_rx1_dr_offset = AS923_MIN_RX1_DR_OFFSET;
    phy_params.max_rx1_dr_offset = AS923_MAX_RX1_DR_OFFSET;
    phy_params.default_rx1_dr_offset = AS923_DEFAULT_RX1_DR_OFFSET;
    phy_params.min_tx_power = AS923_MIN_TX_POWER;
    phy_params.max_tx_power = AS923_MAX_TX_POWER;
    phy_params.default_tx_power = AS923_DEFAULT_TX_POWER;
    phy_params.default_max_eirp = AS923_DEFAULT_MAX_EIRP;
    phy_params.default_antenna_gain = AS923_DEFAULT_ANTENNA_GAIN;
    phy_params.adr_ack_limit = AS923_ADR_ACK_LIMIT;
    phy_params.adr_ack_delay = AS923_ADR_ACK_DELAY;
    phy_params.max_rx_window = AS923_MAX_RX_WINDOW;
    phy_params.recv_delay1 = AS923_RECEIVE_DELAY1;
    phy_params.recv_delay2 = AS923_RECEIVE_DELAY2;
    phy_params.join_channel_mask = AS923_JOIN_CHANNELS;
    phy_params.join_accept_delay1 = AS923_JOIN_ACCEPT_DELAY1;
    phy_params.join_accept_delay2 = AS923_JOIN_ACCEPT_DELAY2;
    phy_params.max_fcnt_gap = AS923_MAX_FCNT_GAP;
    phy_params.ack_timeout = AS923_ACKTIMEOUT;
    phy_params.ack_timeout_rnd = AS923_ACK_TIMEOUT_RND;
    phy_params.rx_window2_datarate = AS923_RX_WND_2_DR;
    phy_params.rx_window2_frequency = AS923_RX_WND_2_FREQ;
}

LoRaPHYAS923::~LoRaPHYAS923()
{
}

int8_t LoRaPHYAS923::get_alternate_DR(uint8_t nb_trials)
{
    // Only AS923_DWELL_LIMIT_DATARATE is supported
    return AS923_DWELL_LIMIT_DATARATE;
}

lorawan_status_t LoRaPHYAS923::set_next_channel(channel_selection_params_t *next_channel_prams,
                                                uint8_t *channel, lorawan_time_t *time,
                                                lorawan_time_t *aggregate_timeoff)
{
    uint8_t next_channel_idx = 0;
    uint8_t nb_enabled_channels = 0;
    uint8_t delay_tx = 0;
    uint8_t enabled_channels[AS923_MAX_NB_CHANNELS] = { 0 };
    lorawan_time_t next_tx_delay = 0;

    if (num_active_channels(channel_mask, 0, 1) == 0) {
        // Reactivate default channels
        channel_mask[0] |= LC(1) + LC(2);
    }

    if (next_channel_prams->aggregate_timeoff <= _lora_time->get_elapsed_time(next_channel_prams->last_aggregate_tx_time)) {
        // Reset Aggregated time off
        *aggregate_timeoff = 0;

        // Update bands Time OFF
        next_tx_delay = update_band_timeoff(next_channel_prams->joined,
                                            next_channel_prams->dc_enabled,
                                            bands, AS923_MAX_NB_BANDS);

        // Search how many channels are enabled
        nb_enabled_channels = enabled_channel_count(next_channel_prams->current_datarate,
                                                    channel_mask,
                                                    enabled_channels, &delay_tx);
    }  else {
        delay_tx++;
        next_tx_delay = next_channel_prams->aggregate_timeoff - _lora_time->get_elapsed_time(next_channel_prams->last_aggregate_tx_time);
    }

    if (nb_enabled_channels > 0) {

        _radio->lock();

        for (uint8_t  i = 0, j = get_random(0, nb_enabled_channels - 1); i < AS923_MAX_NB_CHANNELS; i++) {
            next_channel_idx = enabled_channels[j];
            j = (j + 1) % nb_enabled_channels;

            // Perform carrier sense for AS923_CARRIER_SENSE_TIME
            // If the channel is free, we can stop the LBT mechanism

            if (_radio->perform_carrier_sense(MODEM_LORA,
                                              channels[next_channel_idx].frequency,
                                              AS923_RSSI_FREE_TH,
                                              AS923_CARRIER_SENSE_TIME) == true) {
                // Free channel found
                _radio->unlock();
                *channel = next_channel_idx;
                *time = 0;
                return LORAWAN_STATUS_OK;
            }
        }
        _radio->unlock();
        return LORAWAN_STATUS_NO_FREE_CHANNEL_FOUND;
    } else {

        if (delay_tx > 0) {
            // Delay transmission due to AggregatedTimeOff or to a band time off
            *time = next_tx_delay;
            return LORAWAN_STATUS_DUTYCYCLE_RESTRICTED;
        }

        // Datarate not supported by any channel, restore defaults
        channel_mask[0] |= LC(1) + LC(2);
        *time = 0;
        return LORAWAN_STATUS_NO_CHANNEL_FOUND;
    }
}

uint8_t LoRaPHYAS923::apply_DR_offset(int8_t dr, int8_t dr_offset)
{
    // Initialize minDr for a downlink dwell time configuration of 0
    int8_t min_dr = DR_0;

    // Update the minDR for a downlink dwell time configuration of 1
    if (phy_params.dl_dwell_time_setting == 1) {
        min_dr = AS923_DWELL_LIMIT_DATARATE;
    }

    // Apply offset formula
    return MIN(DR_5, MAX(min_dr, dr - rx1_dr_offset_AS923[dr_offset]));
}


