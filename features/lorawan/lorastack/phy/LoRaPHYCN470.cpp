/**
 *  @file LoRaPHYCN470.cpp
 *
 *  @brief Implements LoRaPHY for Chinese 470 MHz band
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

#include "LoRaPHYCN470.h"
#include "lora_phy_ds.h"

/*!
 * Minimal datarate that can be used by the node
 */
#define CN470_TX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define CN470_TX_MAX_DATARATE                       DR_5

/*!
 * Minimal datarate that can be used by the node
 */
#define CN470_RX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define CN470_RX_MAX_DATARATE                       DR_5

/*!
 * Default datarate used by the node
 */
#define CN470_DEFAULT_DATARATE                      DR_0

/*!
 * Minimal Rx1 receive datarate offset
 */
#define CN470_MIN_RX1_DR_OFFSET                     0

/*!
 * Maximal Rx1 receive datarate offset
 */
#define CN470_MAX_RX1_DR_OFFSET                     3

/*!
 * Default Rx1 receive datarate offset
 */
#define CN470_DEFAULT_RX1_DR_OFFSET                 0

/*!
 * Minimal Tx output power that can be used by the node
 */
#define CN470_MIN_TX_POWER                        TX_POWER_7

/*!
 * Maximal Tx output power that can be used by the node
 */
#define CN470_MAX_TX_POWER                        TX_POWER_0

/*!
 * Default Tx output power used by the node
 */
#define CN470_DEFAULT_TX_POWER                    TX_POWER_0

/*!
 * Default Max EIRP
 */
#define CN470_DEFAULT_MAX_EIRP                      19.15f

/*!
 * Default antenna gain
 */
#define CN470_DEFAULT_ANTENNA_GAIN                  2.15f

/*!
 * ADR Ack limit
 */
#define CN470_ADR_ACK_LIMIT                         64

/*!
 * ADR Ack delay
 */
#define CN470_ADR_ACK_DELAY                         32

/*!
 * Enabled or disabled the duty cycle
 */
#define CN470_DUTY_CYCLE_ENABLED                    0

/*!
 * Maximum RX window duration
 */
#define CN470_MAX_RX_WINDOW                         3000

/*!
 * Receive delay 1
 */
#define CN470_RECEIVE_DELAY1                        1000

/*!
 * Receive delay 2
 */
#define CN470_RECEIVE_DELAY2                        2000

/*!
 * Join accept delay 1
 */
#define CN470_JOIN_ACCEPT_DELAY1                    5000

/*!
 * Join accept delay 2
 */
#define CN470_JOIN_ACCEPT_DELAY2                    6000

/*!
 * Maximum frame counter gap
 */
#define CN470_MAX_FCNT_GAP                          16384

/*!
 * Ack timeout
 */
#define CN470_ACKTIMEOUT                            2000

/*!
 * Random ack timeout limits
 */
#define CN470_ACK_TIMEOUT_RND                       1000

/*!
 * Second reception window channel frequency definition.
 */
#define CN470_RX_WND_2_FREQ                         505300000

/*!
 * Second reception window channel datarate definition.
 */
#define CN470_RX_WND_2_DR                           DR_0

/*!
 * Band 0 definition
 * { DutyCycle, TxMaxPower, LastJoinTxDoneTime, LastTxDoneTime, TimeOff }
 */
static const band_t CN470_BAND0 = {1, CN470_MAX_TX_POWER, 0, 0, 0}; //  100.0 %

/*!
 * Defines the first channel for RX window 1 for CN470 band
 */
#define CN470_FIRST_RX1_CHANNEL                     ((uint32_t) 500300000)

/*!
 * Defines the last channel for RX window 1 for CN470 band
 */
#define CN470_LAST_RX1_CHANNEL                      ((uint32_t) 509700000)

/*!
 * Defines the step width of the channels for RX window 1
 */
#define CN470_STEPWIDTH_RX1_CHANNEL                 ((uint32_t) 200000)

/*!
 * Data rates table definition
 */
static const uint8_t datarates_CN470[]  = {12, 11, 10,  9,  8,  7};

/*!
 * Bandwidths table definition in Hz
 */
static const uint32_t bandwidths_CN470[] = {125000, 125000, 125000, 125000, 125000, 125000};

/*!
 * Maximum payload with respect to the datarate index. Cannot operate with repeater.
 */
static const uint8_t max_payloads_CN470[] = {51, 51, 51, 115, 222, 222};

/*!
 * Maximum payload with respect to the datarate index. Can operate with repeater.
 */
static const uint8_t max_payloads_with_repeater_CN470[] = {51, 51, 51, 115, 222, 222};


LoRaPHYCN470::LoRaPHYCN470()
{
    static const uint16_t fsb_mask[] = MBED_CONF_LORA_FSB_MASK_CHINA;

    bands[0] = CN470_BAND0;

    // Channels
    // 125 kHz channels
    for( uint8_t i = 0; i < CN470_MAX_NB_CHANNELS; i++ )
    {
        channels[i].frequency = 470300000 + i * 200000;
        channels[i].dr_range.value = ( DR_5 << 4 ) | DR_0;
        channels[i].band = 0;
    }

    // Initialize the channels default mask
    for (uint8_t i = 0; i < CN470_CHANNEL_MASK_SIZE; i++) {
        default_channel_mask[i] = 0xFFFF & fsb_mask[i];
    }

    // Update the channels mask
    copy_channel_mask(channel_mask, default_channel_mask, CN470_CHANNEL_MASK_SIZE);

    // set default channels
    phy_params.channels.channel_list = channels;
    phy_params.channels.channel_list_size = CN470_MAX_NB_CHANNELS;
    phy_params.channels.mask = channel_mask;
    phy_params.channels.default_mask = default_channel_mask;
    phy_params.channels.mask_size = CN470_CHANNEL_MASK_SIZE;

    // set bands for CN470 spectrum
    phy_params.bands.table = (void *) bands;
    phy_params.bands.size = CN470_MAX_NB_BANDS;

    // set bandwidths available in CN470 spectrum
    phy_params.bandwidths.table = (void *) bandwidths_CN470;
    phy_params.bandwidths.size = 6;

    // set data rates available in CN470 spectrum
    phy_params.datarates.table = (void *) datarates_CN470;
    phy_params.datarates.size = 6;

    // set payload sizes with respect to data rates
    phy_params.payloads.table = (void *) max_payloads_CN470;
    phy_params.payloads.size = 6;
    phy_params.payloads_with_repeater.table = (void *)max_payloads_with_repeater_CN470;
    phy_params.payloads_with_repeater.size = 6;

    // dwell time setting
    phy_params.ul_dwell_time_setting = 0;
    phy_params.dl_dwell_time_setting = 0;

    // set initial and default parameters
    phy_params.duty_cycle_enabled = CN470_DUTY_CYCLE_ENABLED;

    phy_params.accept_tx_param_setup_req = false;
    phy_params.fsk_supported = false;
    phy_params.cflist_supported = false;
    phy_params.dl_channel_req_supported = false;
    phy_params.custom_channelplans_supported = false;

    phy_params.default_channel_cnt = CN470_MAX_NB_CHANNELS;
    phy_params.max_channel_cnt = CN470_MAX_NB_CHANNELS;
    phy_params.cflist_channel_cnt = 0;
    phy_params.min_tx_datarate = CN470_TX_MIN_DATARATE;
    phy_params.max_tx_datarate = CN470_TX_MAX_DATARATE;
    phy_params.min_rx_datarate = CN470_RX_MIN_DATARATE;
    phy_params.max_rx_datarate = CN470_RX_MAX_DATARATE;
    phy_params.default_datarate = CN470_DEFAULT_DATARATE;
    phy_params.default_max_datarate = CN470_TX_MAX_DATARATE;
    phy_params.min_rx1_dr_offset = CN470_MIN_RX1_DR_OFFSET;
    phy_params.max_rx1_dr_offset = CN470_MAX_RX1_DR_OFFSET;
    phy_params.default_rx1_dr_offset = CN470_DEFAULT_RX1_DR_OFFSET;
    phy_params.min_tx_power = CN470_MIN_TX_POWER;
    phy_params.max_tx_power = CN470_MAX_TX_POWER;
    phy_params.default_tx_power = CN470_DEFAULT_TX_POWER;
    phy_params.default_max_eirp = CN470_DEFAULT_MAX_EIRP;
    phy_params.default_antenna_gain = CN470_DEFAULT_ANTENNA_GAIN;
    phy_params.adr_ack_limit = CN470_ADR_ACK_LIMIT;
    phy_params.adr_ack_delay = CN470_ADR_ACK_DELAY;
    phy_params.max_rx_window = CN470_MAX_RX_WINDOW;
    phy_params.recv_delay1 = CN470_RECEIVE_DELAY1;
    phy_params.recv_delay2 = CN470_RECEIVE_DELAY2;

    phy_params.join_accept_delay1 = CN470_JOIN_ACCEPT_DELAY1;
    phy_params.join_accept_delay2 = CN470_JOIN_ACCEPT_DELAY2;
    phy_params.max_fcnt_gap = CN470_MAX_FCNT_GAP;
    phy_params.ack_timeout = CN470_ACKTIMEOUT;
    phy_params.ack_timeout_rnd = CN470_ACK_TIMEOUT_RND;
    phy_params.rx_window2_datarate = CN470_RX_WND_2_DR;
    phy_params.rx_window2_frequency = CN470_RX_WND_2_FREQ;
}

LoRaPHYCN470::~LoRaPHYCN470()
{
}

lorawan_status_t LoRaPHYCN470::set_next_channel(channel_selection_params_t *params,
                                                uint8_t *channel, lorawan_time_t *time,
                                                lorawan_time_t *aggregate_timeoff)
{
    uint8_t channel_count = 0;
    uint8_t delay_tx = 0;

    uint8_t enabled_channels[CN470_MAX_NB_CHANNELS] = {0};

    lorawan_time_t next_tx_delay = 0;
    band_t *band_table = (band_t *) phy_params.bands.table;

    if (num_active_channels(phy_params.channels.mask, 0,
                            phy_params.channels.mask_size) == 0) {

        // Reactivate default channels
        copy_channel_mask(phy_params.channels.mask,
                          phy_params.channels.default_mask,
                          phy_params.channels.mask_size);
    }

    if (params->aggregate_timeoff
            <= _lora_time->get_elapsed_time(params->last_aggregate_tx_time)) {
        // Reset Aggregated time off
        *aggregate_timeoff = 0;

        // Update bands Time OFF
        next_tx_delay = update_band_timeoff(params->joined,
                                            params->dc_enabled,
                                            band_table, phy_params.bands.size);

        // Search how many channels are enabled
        channel_count = enabled_channel_count(params->current_datarate,
                                              phy_params.channels.mask,
                                              enabled_channels, &delay_tx);
    } else {
        delay_tx++;
        next_tx_delay = params->aggregate_timeoff -
                        _lora_time->get_elapsed_time(params->last_aggregate_tx_time);
    }

    if (channel_count > 0) {
        // We found a valid channel
        *channel = enabled_channels[get_random(0, channel_count - 1)];
        *time = 0;
        return LORAWAN_STATUS_OK;
    }

    if (delay_tx > 0) {
        // Delay transmission due to AggregatedTimeOff or to a band time off
        *time = next_tx_delay;
        return LORAWAN_STATUS_DUTYCYCLE_RESTRICTED;
    }

    *time = 0;
    return LORAWAN_STATUS_NO_CHANNEL_FOUND;
}

bool LoRaPHYCN470::rx_config(rx_config_params_t* config)
{
    int8_t dr = config->datarate;
    uint8_t max_payload = 0;
    int8_t phy_dr = 0;
    uint32_t frequency = config->frequency;

    _radio->lock();

    if (_radio->get_status() != RF_IDLE) {
        _radio->unlock();
        return false;
    }

    _radio->unlock();

    if( config->rx_slot == RX_SLOT_WIN_1 )
    {
        // Apply window 1 frequency
        frequency = CN470_FIRST_RX1_CHANNEL + (config->channel % 48) * CN470_STEPWIDTH_RX1_CHANNEL;
    }

    // Read the physical datarate from the datarates table
    phy_dr = datarates_CN470[dr];

    _radio->lock();

    _radio->set_channel(frequency);

    // Radio configuration
    _radio->set_rx_config(MODEM_LORA, config->bandwidth, phy_dr, 1, 0,
                          MBED_CONF_LORA_DOWNLINK_PREAMBLE_LENGTH,
                          config->window_timeout, false, 0, false, 0, 0, true,
                          config->is_rx_continuous);

    _radio->unlock();

    if (config->is_repeater_supported == true) {
        max_payload = max_payloads_with_repeater_CN470[dr];
    } else {
        max_payload = max_payloads_CN470[dr];
    }

    _radio->lock();
    _radio->set_max_payload_length(MODEM_LORA, max_payload + LORA_MAC_FRMPAYLOAD_OVERHEAD);
    _radio->unlock();

    return true;
}

bool LoRaPHYCN470::tx_config(tx_config_params_t* config, int8_t* tx_power,
                             lorawan_time_t* tx_toa)
{
    int8_t phy_dr = datarates_CN470[config->datarate];

    if (config->tx_power > bands[channels[config->channel].band].max_tx_pwr) {
        config->tx_power = bands[channels[config->channel].band].max_tx_pwr;
    }

    int8_t phy_tx_power = 0;

    // Calculate physical TX power
    phy_tx_power = compute_tx_power(config->tx_power, config->max_eirp,
                                    config->antenna_gain);

    // acquire lock to radio
    _radio->lock();

    _radio->set_channel(channels[config->channel].frequency);

   _radio->set_tx_config(MODEM_LORA, phy_tx_power, 0, 0, phy_dr, 1,
                         MBED_CONF_LORA_UPLINK_PREAMBLE_LENGTH, false, true,
                         0, 0, false, 3000);
    // Setup maximum payload lenght of the radio driver
    _radio->set_max_payload_length(MODEM_LORA, config->pkt_len);

    // Get the time-on-air of the next tx frame
    *tx_toa = _radio->time_on_air(MODEM_LORA, config->pkt_len);

    // release lock to radio
    _radio->unlock();

    *tx_power = config->tx_power;

    return true;
}

uint8_t LoRaPHYCN470::link_ADR_request(adr_req_params_t* params,
                                       int8_t* dr_out, int8_t* tx_power_out,
                                       uint8_t* nb_rep_out,
                                       uint8_t* nb_bytes_parsed)
{
    uint8_t status = 0x07;
    link_adr_params_t adr_settings;
    uint8_t next_index = 0;
    uint8_t bytes_processed = 0;
    uint16_t temp_channel_masks[CN470_CHANNEL_MASK_SIZE] = {0, 0, 0, 0, 0, 0};

    verify_adr_params_t verify_params;

    // Initialize local copy of channels mask
    copy_channel_mask(temp_channel_masks, channel_mask, CN470_CHANNEL_MASK_SIZE);

    while(bytes_processed < params->payload_size) {

        // Get ADR request parameters
        next_index = parse_link_ADR_req(&(params->payload[bytes_processed]), &adr_settings);

        if (next_index == 0) {
            break; // break loop, since no more request has been found
        }

        // Update bytes processed
        bytes_processed += next_index;

        // Revert status, as we only check the last ADR request for the channel mask KO
        status = 0x07;

        if (adr_settings.ch_mask_ctrl == 6) {

            // Enable all 125 kHz channels
            for (uint8_t i = 0; i < CN470_CHANNEL_MASK_SIZE; i++) {
                temp_channel_masks[i] = 0xFFFF;
            }

        } else if( adr_settings.ch_mask_ctrl == 7 ) {

            status &= 0xFE; // Channel mask KO

        } else {

            for (uint8_t i = 0; i < 16; i++) {

                if (((adr_settings.channel_mask & (1 << i)) != 0 ) &&
                    (channels[adr_settings.ch_mask_ctrl * 16 + i].frequency == 0)) {
                    // Trying to enable an undefined channel
                    status &= 0xFE; // Channel mask KO
                }
            }

            temp_channel_masks[adr_settings.ch_mask_ctrl] = adr_settings.channel_mask;
        }
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

    // Update channelsMask if everything is correct
    if (status == 0x07) {
        // Copy Mask
        copy_channel_mask(channel_mask, temp_channel_masks, CN470_CHANNEL_MASK_SIZE);
    }

    // Update status variables
    *dr_out = adr_settings.datarate;
    *tx_power_out = adr_settings.tx_power;
    *nb_rep_out = adr_settings.nb_rep;
    *nb_bytes_parsed = bytes_processed;

    return status;
}

uint8_t LoRaPHYCN470::accept_rx_param_setup_req(rx_param_setup_req_t* params)
{
    uint8_t status = 0x07;
    uint32_t freq = params->frequency;

    // acquire radio lock
    _radio->lock();

    if ((_radio->check_rf_frequency(freq) == false)
            || (freq < CN470_FIRST_RX1_CHANNEL)
            || (freq > CN470_LAST_RX1_CHANNEL)
            || (((freq - (uint32_t) CN470_FIRST_RX1_CHANNEL) % (uint32_t) CN470_STEPWIDTH_RX1_CHANNEL) != 0)) {

        status &= 0xFE; // Channel frequency KO
    }

    // release radio lock
    _radio->unlock();

    // Verify datarate
    if (val_in_range(params->datarate, CN470_RX_MIN_DATARATE, CN470_RX_MAX_DATARATE) == 0) {
        status &= 0xFD; // Datarate KO
    }

    // Verify datarate offset
    if (val_in_range(params->dr_offset, CN470_MIN_RX1_DR_OFFSET, CN470_MAX_RX1_DR_OFFSET) == 0) {
        status &= 0xFB; // Rx1DrOffset range KO
    }

    return status;
}
