/**
 *  @file LoRaPHYKR920.cpp
 *
 *  @brief Implements LoRaPHY for Korean 920 MHz band
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

#include "LoRaPHYKR920.h"
#include "lora_phy_ds.h"


/*!
 * Number of default channels
 */
#define KR920_NUMB_DEFAULT_CHANNELS                 3

/*!
 * Number of channels to apply for the CF list
 */
#define KR920_NUMB_CHANNELS_CF_LIST                 5

/*!
 * Minimal datarate that can be used by the node
 */
#define KR920_TX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define KR920_TX_MAX_DATARATE                       DR_5

/*!
 * Minimal datarate that can be used by the node
 */
#define KR920_RX_MIN_DATARATE                       DR_0

/*!
 * Maximal datarate that can be used by the node
 */
#define KR920_RX_MAX_DATARATE                       DR_5

/*!
 * Default datarate used by the node
 */
#define KR920_DEFAULT_DATARATE                      DR_0

/*!
 * Minimal Rx1 receive datarate offset
 */
#define KR920_MIN_RX1_DR_OFFSET                     0

/*!
 * Maximal Rx1 receive datarate offset
 */
#define KR920_MAX_RX1_DR_OFFSET                     5

/*!
 * Default Rx1 receive datarate offset
 */
#define KR920_DEFAULT_RX1_DR_OFFSET                 0

/*!
 * Minimal Tx output power that can be used by the node
 */
#define KR920_MIN_TX_POWER                          TX_POWER_7

/*!
 * Maximal Tx output power that can be used by the node
 */
#define KR920_MAX_TX_POWER                          TX_POWER_0

/*!
 * Default Tx output power used by the node
 */
#define KR920_DEFAULT_TX_POWER                      TX_POWER_0

/*!
 * Default Max EIRP for frequency 920.9 MHz - 921.9 MHz
 */
#define KR920_DEFAULT_MAX_EIRP_LOW                  10.0f

/*!
 * Default Max EIRP for frequency 922.1 MHz - 923.3 MHz
 */
#define KR920_DEFAULT_MAX_EIRP_HIGH                 14.0f

/*!
 * Default antenna gain
 */
#define KR920_DEFAULT_ANTENNA_GAIN                  2.15f

/*!
 * ADR Ack limit
 */
#define KR920_ADR_ACK_LIMIT                         64

/*!
 * ADR Ack delay
 */
#define KR920_ADR_ACK_DELAY                         32

/*!
 * Enabled or disabled the duty cycle
 */
#define KR920_DUTY_CYCLE_ENABLED                    0

/*!
 * Maximum RX window duration
 */
#define KR920_MAX_RX_WINDOW                         4000

/*!
 * Receive delay 1
 */
#define KR920_RECEIVE_DELAY1                        1000

/*!
 * Receive delay 2
 */
#define KR920_RECEIVE_DELAY2                        2000

/*!
 * Join accept delay 1
 */
#define KR920_JOIN_ACCEPT_DELAY1                    5000

/*!
 * Join accept delay 2
 */
#define KR920_JOIN_ACCEPT_DELAY2                    6000

/*!
 * Maximum frame counter gap
 */
#define KR920_MAX_FCNT_GAP                          16384

/*!
 * Ack timeout
 */
#define KR920_ACKTIMEOUT                            2000

/*!
 * Random ack timeout limits
 */
#define KR920_ACK_TIMEOUT_RND                       1000

#if ( KR920_DEFAULT_DATARATE > DR_5 )
#error "A default DR higher than DR_5 may lead to connectivity loss."
#endif

/*!
 * Second reception window channel frequency definition.
 */
#define KR920_RX_WND_2_FREQ                         921900000

/*!
 * Second reception window channel datarate definition.
 */
#define KR920_RX_WND_2_DR                           DR_0

/*!
 * Band 0 definition
 * { DutyCycle, TxMaxPower, LastJoinTxDoneTime, LastTxDoneTime, TimeOff }
 */
static const band_t KR920_BAND0 = { 1 , KR920_MAX_TX_POWER, 0, 0, 0 }; //  100.0 %

/*!
 * LoRaMac default channel 1
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t KR920_LC1 = { 922100000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 };

/*!
 * LoRaMac default channel 2
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t KR920_LC2 = { 922300000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 };

/*!
 * LoRaMac default channel 3
 * Channel = { Frequency [Hz], RX1 Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
 */
static const channel_params_t KR920_LC3 = { 922500000, 0, { ( ( DR_5 << 4 ) | DR_0 ) }, 0 };

/*!
 * LoRaMac channels which are allowed for the join procedure
 */
#define KR920_JOIN_CHANNELS                         ( uint16_t )( LC( 1 ) | LC( 2 ) | LC( 3 ) )

/*!
 * RSSI threshold for a free channel [dBm]
 */
#define KR920_RSSI_FREE_TH                          -65

/*!
 * Specifies the time the node performs a carrier sense
 */
#define KR920_CARRIER_SENSE_TIME                    6

/*!
 * Data rates table definition
 */
static const uint8_t datarates_KR920[]  = { 12, 11, 10,  9,  8,  7 };

/*!
 * Bandwidths table definition in Hz
 */
static const uint32_t bandwidths_KR920[] = { 125000, 125000, 125000, 125000, 125000, 125000 };

/*!
 * Maximum payload with respect to the datarate index. Can operate with and without a repeater.
 */
static const uint8_t max_payloads_KR920[] = { 51, 51, 51, 115, 242, 242 };

/*!
 * Maximum payload with respect to the datarate index. Can operate with repeater.
 */
static const uint8_t max_payloads_with_repeater_KR920[] = { 51, 51, 51, 115, 222, 222 };

LoRaPHYKR920::LoRaPHYKR920(LoRaWANTimeHandler &lora_time)
    : LoRaPHY(lora_time)
{
    bands[0] = KR920_BAND0;

    // Channels
    channels[0] = KR920_LC1;
    channels[0].band = 0;
    channels[1] = KR920_LC2;
    channels[1].band = 0;
    channels[2] = KR920_LC3;
    channels[2].band = 0;

    // Initialize the channels default mask
    default_channel_mask[0] = LC( 1 ) + LC( 2 ) + LC( 3 );
    // Update the channels mask
    copy_channel_mask(channel_mask, default_channel_mask, KR920_CHANNEL_MASK_SIZE);

    // set default channels
    phy_params.channels.channel_list = channels;
    phy_params.channels.channel_list_size = KR920_MAX_NB_CHANNELS;
    phy_params.channels.mask = channel_mask;
    phy_params.channels.default_mask = default_channel_mask;
    phy_params.channels.mask_size = KR920_CHANNEL_MASK_SIZE;

    // set bands for KR920 spectrum
    phy_params.bands.table = (void *) bands;
    phy_params.bands.size = KR920_MAX_NB_BANDS;

    // set bandwidths available in KR920 spectrum
    phy_params.bandwidths.table = (void *) bandwidths_KR920;
    phy_params.bandwidths.size = 6;

    // set data rates available in KR920 spectrum
    phy_params.datarates.table = (void *) datarates_KR920;
    phy_params.datarates.size = 6;

    // set payload sizes with respect to data rates
    phy_params.payloads.table = (void *) max_payloads_KR920;
    phy_params.payloads.size = 6;
    phy_params.payloads_with_repeater.table = (void *) max_payloads_with_repeater_KR920;
    phy_params.payloads_with_repeater.size = 6;

    // dwell time setting
    phy_params.ul_dwell_time_setting = 0;
    phy_params.dl_dwell_time_setting = 0;

    // set initial and default parameters
    phy_params.duty_cycle_enabled = KR920_DUTY_CYCLE_ENABLED;
    phy_params.accept_tx_param_setup_req = false;
    phy_params.fsk_supported = false;
    phy_params.cflist_supported = true;
    phy_params.dl_channel_req_supported = true;
    phy_params.custom_channelplans_supported = true;
    phy_params.default_channel_cnt = KR920_NUMB_DEFAULT_CHANNELS;
    phy_params.max_channel_cnt = KR920_MAX_NB_CHANNELS;
    phy_params.cflist_channel_cnt = KR920_NUMB_CHANNELS_CF_LIST;
    phy_params.min_tx_datarate = KR920_TX_MIN_DATARATE;
    phy_params.max_tx_datarate = KR920_TX_MAX_DATARATE;
    phy_params.min_rx_datarate = KR920_RX_MIN_DATARATE;
    phy_params.max_rx_datarate = KR920_RX_MAX_DATARATE;
    phy_params.default_datarate = KR920_DEFAULT_DATARATE;
    phy_params.default_max_datarate = KR920_TX_MAX_DATARATE;
    phy_params.min_rx1_dr_offset = KR920_MIN_RX1_DR_OFFSET;
    phy_params.max_rx1_dr_offset = KR920_MAX_RX1_DR_OFFSET;
    phy_params.default_rx1_dr_offset = KR920_DEFAULT_RX1_DR_OFFSET;
    phy_params.min_tx_power = KR920_MIN_TX_POWER;
    phy_params.max_tx_power = KR920_MAX_TX_POWER;
    phy_params.default_tx_power = KR920_DEFAULT_TX_POWER;
    phy_params.default_max_eirp = KR920_DEFAULT_MAX_EIRP_HIGH;
    phy_params.default_antenna_gain = KR920_DEFAULT_ANTENNA_GAIN;
    phy_params.adr_ack_limit = KR920_ADR_ACK_LIMIT;
    phy_params.adr_ack_delay = KR920_ADR_ACK_DELAY;
    phy_params.max_rx_window = KR920_MAX_RX_WINDOW;
    phy_params.recv_delay1 = KR920_RECEIVE_DELAY1;
    phy_params.recv_delay2 = KR920_RECEIVE_DELAY2;
    phy_params.join_channel_mask = KR920_JOIN_CHANNELS;
    phy_params.join_accept_delay1 = KR920_JOIN_ACCEPT_DELAY1;
    phy_params.join_accept_delay2 = KR920_JOIN_ACCEPT_DELAY2;
    phy_params.max_fcnt_gap = KR920_MAX_FCNT_GAP;
    phy_params.ack_timeout = KR920_ACKTIMEOUT;
    phy_params.ack_timeout_rnd = KR920_ACK_TIMEOUT_RND;
    phy_params.rx_window2_datarate = KR920_RX_WND_2_DR;
    phy_params.rx_window2_frequency = KR920_RX_WND_2_FREQ;
}

LoRaPHYKR920::~LoRaPHYKR920()
{
}

int8_t LoRaPHYKR920::get_max_eirp(uint32_t freq)
{
    if (freq >= 922100000) {// Limit to 14dBm
        return KR920_DEFAULT_MAX_EIRP_HIGH;
    }

    // Limit to 10dBm
    return KR920_DEFAULT_MAX_EIRP_LOW;
}


bool LoRaPHYKR920::verify_frequency_for_band(uint32_t freq, uint8_t band) const
{
    uint32_t tmp_freq = freq;

    _radio->lock();

    if (_radio->check_rf_frequency(tmp_freq) == false) {
        _radio->unlock();
        return false;
    }

    _radio->unlock();

    // Verify if the frequency is valid. The frequency must be in a specified
    // range and can be set to specific values.
    if ((tmp_freq >= 920900000) && (tmp_freq <=923300000)) {
        // Range ok, check for specific value
        tmp_freq -= 920900000;
        if ((tmp_freq % 200000) == 0) {
            return true;
        }
    }

    return false;
}

bool LoRaPHYKR920::tx_config(tx_config_params_t* config, int8_t* tx_power,
                             lorawan_time_t* tx_toa)
{
    int8_t phy_dr = datarates_KR920[config->datarate];

    if (config->tx_power > bands[channels[config->channel].band].max_tx_pwr) {
        config->tx_power = bands[channels[config->channel].band].max_tx_pwr;
    }

    uint32_t bandwidth = get_bandwidth(config->datarate);
    float max_eirp = get_max_eirp(channels[config->channel].frequency);
    int8_t phy_tx_power = 0;

    // Take the minimum between the max_eirp and txConfig->MaxEirp.
    // The value of txConfig->MaxEirp could have changed during runtime, e.g. due to a MAC command.
    max_eirp = MIN(config->max_eirp, max_eirp);

    // Calculate physical TX power
    phy_tx_power = compute_tx_power(config->tx_power, max_eirp, config->antenna_gain);

    // Setup the radio frequency
    _radio->lock();

    _radio->set_channel(channels[config->channel].frequency);

    _radio->set_tx_config(MODEM_LORA, phy_tx_power, 0, bandwidth, phy_dr, 1, 8,
                          false, true, 0, 0, false, 3000 );

    // Setup maximum payload lenght of the radio driver
    _radio->set_max_payload_length(MODEM_LORA, config->pkt_len);
    // Get the time-on-air of the next tx frame
    *tx_toa =_radio->time_on_air(MODEM_LORA, config->pkt_len);

    _radio->unlock();

    *tx_power = config->tx_power;
    return true;
}

lorawan_status_t LoRaPHYKR920::set_next_channel(channel_selection_params_t* params,
                                                uint8_t* channel, lorawan_time_t* time,
                                                lorawan_time_t* aggregate_timeoff)
{
    uint8_t next_channel_idx = 0;
    uint8_t nb_enabled_channels = 0;
    uint8_t delay_tx = 0;
    uint8_t enabled_channels[KR920_MAX_NB_CHANNELS] = {0};
    lorawan_time_t nextTxDelay = 0;

    if (num_active_channels(channel_mask, 0, 1) == 0) {
        // Reactivate default channels
        channel_mask[0] |= LC(1) + LC(2) + LC(3);
    }

    if (params->aggregate_timeoff <= _lora_time.get_elapsed_time(params->last_aggregate_tx_time)) {
        // Reset Aggregated time off
        *aggregate_timeoff = 0;

        // Update bands Time OFF
        nextTxDelay = update_band_timeoff(params->joined, params->dc_enabled,
                                          bands, KR920_MAX_NB_BANDS);

        // Search how many channels are enabled
        nb_enabled_channels = enabled_channel_count(params->joined, params->current_datarate,
                                                     channel_mask,
                                                     enabled_channels, &delay_tx);
    } else {
        delay_tx++;
        nextTxDelay = params->aggregate_timeoff - _lora_time.get_elapsed_time(params->last_aggregate_tx_time);
    }

    if (nb_enabled_channels > 0) {

        for (uint8_t  i = 0, j = get_random(0, nb_enabled_channels - 1);
                      i < KR920_MAX_NB_CHANNELS; i++) {
            next_channel_idx = enabled_channels[j];
            j = ( j + 1 ) % nb_enabled_channels;

            // Perform carrier sense for KR920_CARRIER_SENSE_TIME
            // If the channel is free, we can stop the LBT mechanism
            _radio->lock();

            if (_radio->perform_carrier_sense(MODEM_LORA,
                                              channels[next_channel_idx].frequency,
                                              KR920_RSSI_FREE_TH,
                                              KR920_CARRIER_SENSE_TIME ) == true) {
                // Free channel found
                *channel = next_channel_idx;
                *time = 0;
                _radio->unlock();
                return LORAWAN_STATUS_OK;
            }

            _radio->unlock();
        }

        return LORAWAN_STATUS_NO_FREE_CHANNEL_FOUND;

    } else {

        if (delay_tx > 0) {
            // Delay transmission due to AggregatedTimeOff or to a band time off
            *time = nextTxDelay;
            return LORAWAN_STATUS_DUTYCYCLE_RESTRICTED;
        }

        // Datarate not supported by any channel, restore defaults
        channel_mask[0] |= LC(1) + LC(2) + LC(3);
        *time = 0;
        return LORAWAN_STATUS_NO_CHANNEL_FOUND;
    }
}

void LoRaPHYKR920::set_tx_cont_mode(cw_mode_params_t* params, uint32_t given_frequency)
{
    (void)given_frequency;

    if (params->tx_power > bands[channels[params->channel].band].max_tx_pwr) {
        params->tx_power = bands[channels[params->channel].band].max_tx_pwr;
    }

    float max_eirp = get_max_eirp(channels[params->channel].frequency);
    int8_t phy_tx_power = 0;
    uint32_t frequency = channels[params->channel].frequency;

    // Take the minimum between the max_eirp and params->max_eirp.
    // The value of params->max_eirp could have changed during runtime,
    // e.g. due to a MAC command.
    max_eirp = MIN (params->max_eirp, max_eirp);

    // Calculate physical TX power
    phy_tx_power = compute_tx_power(params->tx_power, max_eirp, params->antenna_gain);

    _radio->lock();
    _radio->set_tx_continuous_wave(frequency, phy_tx_power, params->timeout);
    _radio->unlock();
}

