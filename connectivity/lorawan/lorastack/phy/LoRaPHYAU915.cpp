/**
 *  @file LoRaPHYAU915.cpp
 *
 *  @brief Implements LoRaPHY for Australian 915 MHz band
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

#include <string.h>
#include "LoRaPHYAU915.h"
#include "lora_phy_ds.h"

/*!
 * Minimal datarate that can be used by the node
 */
#define AU915_TX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define AU915_TX_MAX_DATARATE                       DR_6

/*!
 * Minimal datarate that can be used by the node
 */
#define AU915_RX_MIN_DATARATE                       DR_8

/*!
 * Maximal datarate that can be used by the node
 */
#define AU915_RX_MAX_DATARATE                       DR_13

/*!
 * Default datarate used by the node
 */
#define AU915_DEFAULT_DATARATE                      DR_0

/*!
 * Minimal Rx1 receive datarate offset
 */
#define AU915_MIN_RX1_DR_OFFSET                     0

/*!
 * Maximal Rx1 receive datarate offset
 */
#define AU915_MAX_RX1_DR_OFFSET                     6

/*!
 * Default Rx1 receive datarate offset
 */
#define AU915_DEFAULT_RX1_DR_OFFSET                 0

/*!
 * Minimal Tx output power that can be used by the node
 */
#define AU915_MIN_TX_POWER                          TX_POWER_10

/*!
 * Maximal Tx output power that can be used by the node
 */
#define AU915_MAX_TX_POWER                          TX_POWER_0

/*!
 * Default Tx output power used by the node
 */
#define AU915_DEFAULT_TX_POWER                      TX_POWER_0

/*!
 * Default Max EIRP
 */
#define AU915_DEFAULT_MAX_EIRP                      30.0f

/*!
 * Default antenna gain
 */
#define AU915_DEFAULT_ANTENNA_GAIN                  2.15f

/*!
 * ADR Ack limit
 */
#define AU915_ADR_ACK_LIMIT                         64

/*!
 * ADR Ack delay
 */
#define AU915_ADR_ACK_DELAY                         32

/*!
 * Enabled or disabled the duty cycle
 */
#define AU915_DUTY_CYCLE_ENABLED                    0

/*!
 * Maximum RX window duration
 */
#define AU915_MAX_RX_WINDOW                         3000

/*!
 * Receive delay 1
 */
#define AU915_RECEIVE_DELAY1                        1000

/*!
 * Receive delay 2
 */
#define AU915_RECEIVE_DELAY2                        2000

/*!
 * Join accept delay 1
 */
#define AU915_JOIN_ACCEPT_DELAY1                    5000

/*!
 * Join accept delay 2
 */
#define AU915_JOIN_ACCEPT_DELAY2                    6000

/*!
 * Maximum frame counter gap
 */
#define AU915_MAX_FCNT_GAP                          16384

/*!
 * Ack timeout
 */
#define AU915_ACKTIMEOUT                            2000

/*!
 * Random ack timeout limits
 */
#define AU915_ACK_TIMEOUT_RND                       1000

/*!
 * Second reception window channel frequency definition.
 */
#define AU915_RX_WND_2_FREQ                         923300000

/*!
 * Second reception window channel datarate definition.
 */
#define AU915_RX_WND_2_DR                           DR_8

/*!
 * Band 0 definition
 * { DutyCycle, TxMaxPower, LastJoinTxDoneTime, LastTxDoneTime, TimeOff }
 */
static const band_t AU915_BAND0 = {1, AU915_MAX_TX_POWER, 0, 0, 0, 915200000, 927800000}; //  100.0 %

/*!
 * Defines the first channel for RX window 1 for US band
 */
#define AU915_FIRST_RX1_CHANNEL                     ((uint32_t) 923300000)

/*!
 * Defines the last channel for RX window 1 for US band
 */
#define AU915_LAST_RX1_CHANNEL                      ((uint32_t) 927500000)

/*!
 * Defines the step width of the channels for RX window 1
 */
#define AU915_STEPWIDTH_RX1_CHANNEL                 ((uint32_t) 600000)

/*!
 * Data rates table definition
 */
static const uint8_t datarates_AU915[] = {12, 11, 10, 9, 8, 7, 8, 0, 12, 11, 10, 9, 8, 7, 0, 0};

/*!
 * Bandwidths table definition in Hz
 */
static const uint32_t bandwidths_AU915[] = { 125000, 125000, 125000, 125000,
                                             125000, 125000, 500000, 0, 500000, 500000, 500000, 500000, 500000, 500000,
                                             0, 0
                                           };

/*!
 * Up/Down link data rates offset definition
 */
static const int8_t datarate_offsets_AU915[7][6] = { {
        DR_8, DR_8, DR_8, DR_8,
        DR_8, DR_8
    }, // DR_0
    { DR_9, DR_8, DR_8, DR_8, DR_8, DR_8 }, // DR_1
    { DR_10, DR_9, DR_8, DR_8, DR_8, DR_8 }, // DR_2
    { DR_11, DR_10, DR_9, DR_8, DR_8, DR_8 }, // DR_3
    { DR_12, DR_11, DR_10, DR_9, DR_8, DR_8 }, // DR_4
    { DR_13, DR_12, DR_11, DR_10, DR_9, DR_8 }, // DR_5
    { DR_13, DR_13, DR_12, DR_11, DR_10, DR_9 }, // DR_6
};

/*!
 * Maximum payload with respect to the datarate index. Cannot operate with repeater.
 */
static const uint8_t max_payload_AU915[] = { 51, 51, 51, 115, 242, 242,
                                             242, 0, 53, 129, 242, 242, 242, 242, 0, 0
                                           };

/*!
 * Maximum payload with respect to the datarate index. Can operate with repeater.
 */
static const uint8_t max_payload_with_repeater_AU915[] = { 51, 51, 51, 115,
                                                           222, 222, 222, 0, 33, 109, 222, 222, 222, 222, 0, 0
                                                         };

static const uint16_t fsb_mask[] = MBED_CONF_LORA_FSB_MASK;

static const uint16_t full_channel_mask [] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x00FF};

LoRaPHYAU915::LoRaPHYAU915()
{
    bands[0] = AU915_BAND0;

    // Activate Channels
    // 125 kHz channels Upstream only
    for (uint8_t i = 0; i < AU915_MAX_NB_CHANNELS - 8; i++) {
        channels[i].frequency = 915200000 + i * 200000;
        channels[i].dr_range.value = (DR_5 << 4) | DR_0;
        channels[i].band = 0;
    }
    // 500 kHz channels
    // Upstream and downstream both
    for (uint8_t i = AU915_MAX_NB_CHANNELS - 8; i < AU915_MAX_NB_CHANNELS; i++) {
        channels[i].frequency = 915900000 + (i - (AU915_MAX_NB_CHANNELS - 8)) * 1600000;
        channels[i].dr_range.value = (DR_6 << 4) | DR_6;
        channels[i].band = 0;
    }

    // Initialize channels default mask
    // All channels are default channels here
    // Join request needs to alternate between 125 KHz and 500 KHz channels
    // randomly. Fill in the default channel mask depending upon the given
    // fsb_mask
    fill_channel_mask_with_fsb(full_channel_mask, fsb_mask,
                               default_channel_mask, AU915_CHANNEL_MASK_SIZE);

    memset(channel_mask, 0, sizeof(channel_mask));
    memset(current_channel_mask, 0, sizeof(current_channel_mask));

    // Copy channels default mask
    copy_channel_mask(channel_mask, default_channel_mask, AU915_CHANNEL_MASK_SIZE);

    // Copy into current channels mask
    // This mask is used to keep track of the channels which were used in
    // previous transmissions as the AU915 band doesn't allow concurrent
    // transmission on the same channel
    copy_channel_mask(current_channel_mask, channel_mask, AU915_CHANNEL_MASK_SIZE);

    // set default channels
    phy_params.channels.channel_list = channels;
    phy_params.channels.channel_list_size = AU915_MAX_NB_CHANNELS;
    phy_params.channels.mask = channel_mask;
    phy_params.channels.default_mask = default_channel_mask;
    phy_params.channels.mask_size = AU915_CHANNEL_MASK_SIZE;

    // set bands for AU915 spectrum
    phy_params.bands.table = (void *) bands;
    phy_params.bands.size = AU915_MAX_NB_BANDS;

    // set bandwidths available in AU915 spectrum
    phy_params.bandwidths.table = (void *) bandwidths_AU915;
    phy_params.bandwidths.size = 16;

    // set data rates available in AU915 spectrum
    phy_params.datarates.table = (void *) datarates_AU915;
    phy_params.datarates.size = 16;

    // set payload sizes with respect to data rates
    phy_params.payloads.table = (void *) max_payload_AU915;
    phy_params.payloads.size = 16;
    phy_params.payloads_with_repeater.table = (void *) max_payload_with_repeater_AU915;
    phy_params.payloads_with_repeater.size = 16;

    // dwell time setting
    phy_params.ul_dwell_time_setting = 0;
    phy_params.dl_dwell_time_setting = 0;
    phy_params.dwell_limit_datarate = AU915_DEFAULT_DATARATE;

    phy_params.duty_cycle_enabled = AU915_DUTY_CYCLE_ENABLED;
    phy_params.accept_tx_param_setup_req = false;
    phy_params.custom_channelplans_supported = false;
    phy_params.cflist_supported = false;
    phy_params.fsk_supported = false;

    phy_params.default_channel_cnt = AU915_MAX_NB_CHANNELS;
    phy_params.max_channel_cnt = AU915_MAX_NB_CHANNELS;
    phy_params.cflist_channel_cnt = 0;
    phy_params.min_tx_datarate = AU915_TX_MIN_DATARATE;
    phy_params.max_tx_datarate = AU915_TX_MAX_DATARATE;
    phy_params.min_rx_datarate = AU915_RX_MIN_DATARATE;
    phy_params.max_rx_datarate = AU915_RX_MAX_DATARATE;
    phy_params.default_datarate = AU915_DEFAULT_DATARATE;
    phy_params.default_max_datarate = AU915_TX_MAX_DATARATE;
    phy_params.min_rx1_dr_offset = AU915_MIN_RX1_DR_OFFSET;
    phy_params.max_rx1_dr_offset = AU915_MAX_RX1_DR_OFFSET;
    phy_params.default_rx1_dr_offset = AU915_DEFAULT_RX1_DR_OFFSET;
    phy_params.min_tx_power = AU915_MIN_TX_POWER;
    phy_params.max_tx_power = AU915_MAX_TX_POWER;
    phy_params.default_tx_power = AU915_DEFAULT_TX_POWER;
    phy_params.default_max_eirp = AU915_DEFAULT_MAX_EIRP;
    phy_params.default_antenna_gain = AU915_DEFAULT_ANTENNA_GAIN;
    phy_params.adr_ack_limit = AU915_ADR_ACK_LIMIT;
    phy_params.adr_ack_delay = AU915_ADR_ACK_DELAY;
    phy_params.max_rx_window = AU915_MAX_RX_WINDOW;
    phy_params.recv_delay1 = AU915_RECEIVE_DELAY1;
    phy_params.recv_delay2 = AU915_RECEIVE_DELAY2;

    phy_params.join_accept_delay1 = AU915_JOIN_ACCEPT_DELAY1;
    phy_params.join_accept_delay2 = AU915_JOIN_ACCEPT_DELAY2;
    phy_params.max_fcnt_gap = AU915_MAX_FCNT_GAP;
    phy_params.ack_timeout = AU915_ACKTIMEOUT;
    phy_params.ack_timeout_rnd = AU915_ACK_TIMEOUT_RND;
    phy_params.rx_window2_datarate = AU915_RX_WND_2_DR;
    phy_params.rx_window2_frequency = AU915_RX_WND_2_FREQ;
}

LoRaPHYAU915::~LoRaPHYAU915()
{
}

bool LoRaPHYAU915::rx_config(rx_config_params_t *params)
{
    int8_t dr = params->datarate;
    uint8_t max_payload = 0;
    int8_t phy_dr = 0;
    uint32_t frequency = params->frequency;

    if (_radio->get_status() != RF_IDLE) {
        return false;
    }

    if (params->rx_slot == RX_SLOT_WIN_1) {
        // Apply window 1 frequency
        frequency = AU915_FIRST_RX1_CHANNEL
                    + (params->channel % 8) * AU915_STEPWIDTH_RX1_CHANNEL;
        // Caller may print the frequency to log so update it to match actual frequency
        params->frequency = frequency;
    }

    // Read the physical datarate from the datarates table
    phy_dr = datarates_AU915[dr];

    _radio->lock();

    _radio->set_channel(frequency);

    // Radio configuration
    _radio->set_rx_config(MODEM_LORA, params->bandwidth, phy_dr, 1, 0, 8,
                          params->window_timeout, false, 0, false, 0, 0, true,
                          params->is_rx_continuous);

    if (params->is_repeater_supported == true) {
        max_payload = max_payload_with_repeater_AU915[dr];
    } else {
        max_payload = max_payload_AU915[dr];
    }
    _radio->set_max_payload_length(MODEM_LORA,
                                   max_payload + LORA_MAC_FRMPAYLOAD_OVERHEAD);

    _radio->unlock();

    return true;
}

bool LoRaPHYAU915::tx_config(tx_config_params_t *params, int8_t *tx_power,
                             lorawan_time_t *tx_toa)
{
    int8_t phy_dr = datarates_AU915[params->datarate];

    if (params->tx_power > bands[channels[params->channel].band].max_tx_pwr) {
        params->tx_power = bands[channels[params->channel].band].max_tx_pwr;
    }

    uint32_t bandwidth = get_bandwidth(params->datarate);
    int8_t phy_tx_power = 0;

    // Calculate physical TX power
    phy_tx_power = compute_tx_power(params->tx_power, params->max_eirp,
                                    params->antenna_gain);

    // setting up radio tx configurations

    _radio->lock();

    _radio->set_channel(channels[params->channel].frequency);

    _radio->set_tx_config(MODEM_LORA, phy_tx_power, 0, bandwidth, phy_dr, 1, 8,
                          false, true, 0, 0, false, 3000);

    // Setup maximum payload lenght of the radio driver
    _radio->set_max_payload_length(MODEM_LORA, params->pkt_len);

    *tx_toa = _radio->time_on_air(MODEM_LORA, params->pkt_len);

    _radio->unlock();

    *tx_power = params->tx_power;

    return true;
}

uint8_t LoRaPHYAU915::link_ADR_request(adr_req_params_t *params,
                                       int8_t *dr_out, int8_t *tx_power_out,
                                       uint8_t *nb_rep_out,
                                       uint8_t *nb_bytes_parsed)
{
    uint8_t status = 0x07;
    link_adr_params_t adr_settings = {};
    uint8_t next_index = 0;
    uint8_t bytes_processed = 0;
    uint16_t temp_channel_masks[AU915_CHANNEL_MASK_SIZE] = { 0, 0, 0, 0, 0};

    verify_adr_params_t verify_params;

    // Initialize local copy of channels mask
    copy_channel_mask(temp_channel_masks, channel_mask, AU915_CHANNEL_MASK_SIZE);

    while (bytes_processed < params->payload_size &&
            params->payload[bytes_processed] == SRV_MAC_LINK_ADR_REQ) {
        next_index = parse_link_ADR_req(&(params->payload[bytes_processed]),
                                        params->payload_size,
                                        &adr_settings);

        if (next_index == 0) {
            bytes_processed = 0;
            // break loop, malformed packet
            break;
        }

        // Update bytes processed
        bytes_processed += next_index;

        // Revert status, as we only check the last ADR request for the channel mask KO
        status = 0x07;

        if (adr_settings.ch_mask_ctrl == 6) {
            // Enable all 125 kHz channels
            fill_channel_mask_with_value(temp_channel_masks, 0xFFFF,
                                         AU915_CHANNEL_MASK_SIZE - 1);

            // Apply chMask to channels 64 to 71
            temp_channel_masks[4] = adr_settings.channel_mask;
        } else if (adr_settings.ch_mask_ctrl == 7) {
            // Disable all 125 kHz channels
            fill_channel_mask_with_value(temp_channel_masks, 0x0000,
                                         AU915_CHANNEL_MASK_SIZE - 1);

            // Apply chMask to channels 64 to 71
            temp_channel_masks[4] = adr_settings.channel_mask;
        } else if (adr_settings.ch_mask_ctrl == 5) {
            // RFU
            status &= 0xFE; // Channel mask KO
        } else {
            temp_channel_masks[adr_settings.ch_mask_ctrl] = adr_settings.channel_mask;
        }
    }

    if (bytes_processed == 0) {
        *nb_bytes_parsed = 0;
        return status;
    }

    // FCC 15.247 paragraph F mandates to hop on at least 2 125 kHz channels
    if ((adr_settings.datarate < DR_6)
            && (num_active_channels(temp_channel_masks, 0, 4) < 2)) {
        status &= 0xFE; // Channel mask KO
    }

    verify_params.status = status;
    verify_params.adr_enabled = params->adr_enabled;
    verify_params.datarate = adr_settings.datarate;
    verify_params.tx_power = adr_settings.tx_power;
    verify_params.nb_rep = adr_settings.nb_rep;
    verify_params.current_datarate = params->current_datarate;
    verify_params.current_tx_power = params->current_tx_power;
    verify_params.current_nb_rep = params->current_nb_trans;
    verify_params.channel_mask = temp_channel_masks;


    // Verify the parameters and update, if necessary
    status = verify_link_ADR_req(&verify_params, &adr_settings.datarate,
                                 &adr_settings.tx_power, &adr_settings.nb_rep);

    // Update cchannel mask if everything is correct
    if (status == 0x07) {
        // Copy Mask
        copy_channel_mask(channel_mask, temp_channel_masks, AU915_CHANNEL_MASK_SIZE);

        intersect_channel_mask(channel_mask, current_channel_mask,
                               AU915_CHANNEL_MASK_SIZE);
    }

    // Update status variables
    *dr_out = adr_settings.datarate;
    *tx_power_out = adr_settings.tx_power;
    *nb_rep_out = adr_settings.nb_rep;
    *nb_bytes_parsed = bytes_processed;

    return status;
}

uint8_t LoRaPHYAU915::accept_rx_param_setup_req(rx_param_setup_req_t *params)
{
    uint8_t status = 0x07;
    uint32_t freq = params->frequency;

    // Verify radio frequency
    _radio->lock();

    if ((_radio->check_rf_frequency(freq) == false)
            || (freq < AU915_FIRST_RX1_CHANNEL)
            || (freq > AU915_LAST_RX1_CHANNEL)
            || (((freq - (uint32_t) AU915_FIRST_RX1_CHANNEL)
                 % (uint32_t) AU915_STEPWIDTH_RX1_CHANNEL) != 0)) {
        status &= 0xFE; // Channel frequency KO
    }

    _radio->unlock();

    // Verify datarate
    if (val_in_range(params->datarate, AU915_RX_MIN_DATARATE, AU915_RX_MAX_DATARATE) == 0) {
        status &= 0xFD; // Datarate KO
    }

    if ((params->datarate == DR_7) || (params->datarate > DR_13)) {
        status &= 0xFD; // Datarate KO
    }

    // Verify datarate offset
    if (val_in_range(params->dr_offset, AU915_MIN_RX1_DR_OFFSET, AU915_MAX_RX1_DR_OFFSET) == 0) {
        status &= 0xFB; // Rx1DrOffset range KO
    }

    return status;
}

int8_t LoRaPHYAU915::get_alternate_DR(uint8_t nb_trials)
{
    int8_t datarate = 0;

    if ((nb_trials & 0x01) == 0x01) {
        datarate = DR_6;
    } else {
        datarate = DR_0;
    }

    return datarate;
}

lorawan_status_t LoRaPHYAU915::set_next_channel(channel_selection_params_t *next_chan_params,
                                                uint8_t *channel, lorawan_time_t *time,
                                                lorawan_time_t *aggregated_timeOff)
{
    uint8_t nb_enabled_channels = 0;
    uint8_t delay_tx = 0;
    uint8_t enabled_channels[AU915_MAX_NB_CHANNELS] = { 0 };
    lorawan_time_t next_tx_delay = 0;

    // Count 125kHz channels
    if (num_active_channels(current_channel_mask, 0, 4) == 0) {
        // Reactivate 125 kHz default channels
        copy_channel_mask(current_channel_mask, channel_mask, 4);
    }

    // Check other channels
    if ((next_chan_params->current_datarate >= DR_6)
            && (current_channel_mask[4] & 0x00FF) == 0) {
        // fall back to 500 kHz default channels
        current_channel_mask[4] = channel_mask[4];
    }

    if (next_chan_params->aggregate_timeoff <= _lora_time->get_elapsed_time(next_chan_params->last_aggregate_tx_time)) {
        // Reset Aggregated time off
        *aggregated_timeOff = 0;

        // Update bands Time OFF
        next_tx_delay = update_band_timeoff(next_chan_params->joined,
                                            next_chan_params->dc_enabled,
                                            bands, AU915_MAX_NB_BANDS);

        // Search how many channels are enabled
        nb_enabled_channels = enabled_channel_count(next_chan_params->current_datarate,
                                                    current_channel_mask,
                                                    enabled_channels, &delay_tx);
    } else {
        delay_tx++;
        next_tx_delay = next_chan_params->aggregate_timeoff - _lora_time->get_elapsed_time(next_chan_params->last_aggregate_tx_time);
    }

    if (nb_enabled_channels > 0) {
        // We found a valid channel
        *channel = enabled_channels[get_random(0, nb_enabled_channels - 1)];
        // Disable the channel in the mask
        disable_channel(current_channel_mask, *channel, AU915_MAX_NB_CHANNELS);

        *time = 0;
        return LORAWAN_STATUS_OK;
    } else {
        if (delay_tx > 0) {
            // Delay transmission due to AggregatedTimeOff or to a band time off
            *time = next_tx_delay;
            return LORAWAN_STATUS_DUTYCYCLE_RESTRICTED;
        }
        // Datarate not supported by any channel
        *time = 0;
        return LORAWAN_STATUS_NO_CHANNEL_FOUND;
    }
}

uint8_t LoRaPHYAU915::apply_DR_offset(int8_t dr, int8_t dr_offset)
{
    return datarate_offsets_AU915[dr][dr_offset];
}

void LoRaPHYAU915::intersect_channel_mask(const uint16_t *source,
                                          uint16_t *destination, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        destination[i] &= source[i];
    }
}

void LoRaPHYAU915::fill_channel_mask_with_fsb(const uint16_t *expectation,
                                              const uint16_t *fsb_mask,
                                              uint16_t *destination,
                                              uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        destination[i] = expectation[i] & fsb_mask[i];
    }

}

void LoRaPHYAU915::fill_channel_mask_with_value(uint16_t *channel_mask,
                                                uint16_t value, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        channel_mask[i] = value;
    }
}
