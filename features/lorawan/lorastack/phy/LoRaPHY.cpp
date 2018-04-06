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

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )


Copyright (c) 2017, Arm Limited and affiliates.

SPDX-License-Identifier: BSD-3-Clause
*/

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "lorawan/lorastack/phy/LoRaPHY.h"

#define BACKOFF_DC_1_HOUR       100
#define BACKOFF_DC_10_HOURS     1000
#define BACKOFF_DC_24_HOURS     10000

#define CHANNELS_IN_MASK  16

LoRaPHY::LoRaPHY(LoRaWANTimeHandler &lora_time)
    : _radio(NULL),
      _lora_time(lora_time)
{
    memset(&phy_params, 0, sizeof(phy_params));
}

LoRaPHY::~LoRaPHY()
{
    _radio = NULL;
}

void LoRaPHY::set_radio_instance(LoRaRadio& radio)
{
    _radio = &radio;
}

void LoRaPHY::put_radio_to_sleep() {
    _radio->lock();
    _radio->sleep();
    _radio->unlock();
}

void LoRaPHY::put_radio_to_standby() {
    _radio->lock();
    _radio->standby();
    _radio->unlock();
}

void LoRaPHY::setup_public_network_mode(bool set)
{
    _radio->lock();
    _radio->set_public_network(set);
    _radio->unlock();
}

void LoRaPHY::setup_rx_window(bool rx_continuous, uint32_t max_rx_window)
{
    _radio->lock();
    if (!rx_continuous) {
        _radio->receive(max_rx_window);
    } else {
        _radio->receive(0); // Continuous mode
    }
    _radio->unlock();
}

// For DevNonce for example
uint32_t LoRaPHY::get_radio_rng()
{
    uint32_t rand;

    _radio->lock();
    rand =_radio->random();
    _radio->unlock();

    return rand;
}

void LoRaPHY::handle_send(uint8_t *buf, uint8_t size)
{
    _radio->lock();
    _radio->send(buf, size);
    _radio->unlock();
}

uint8_t LoRaPHY::request_new_channel(new_channel_req_params_t* params)
{
    if (!phy_params.custom_channelplans_supported) {
        return 0;
    }

    uint8_t status = 0x03;

    if (params->new_channel->frequency == 0) {
        // Remove
        if (remove_channel(params->channel_id) == false) {
            status &= 0xFC;
        }
    } else {

        switch (add_channel(params->new_channel, params->channel_id)) {
            case LORAWAN_STATUS_OK:
            {
                break;
            }
            case LORAWAN_STATUS_FREQUENCY_INVALID:
            {
                status &= 0xFE;
                break;
            }
            case LORAWAN_STATUS_DATARATE_INVALID:
            {
                status &= 0xFD;
                break;
            }
            case LORAWAN_STATUS_FREQ_AND_DR_INVALID:
            {
                status &= 0xFC;
                break;
            }
            default:
            {
                status &= 0xFC;
                break;
            }
        }
    }

    return status;
}

int32_t LoRaPHY::get_random(int32_t min, int32_t max)
{
    return (int32_t) rand() % (max - min + 1) + min;
}

bool LoRaPHY::verify_channel_DR(uint8_t nb_channels, uint16_t* channel_mask,
                                int8_t dr, int8_t min_dr, int8_t max_dr,
                                channel_params_t* channels)
{
    if (val_in_range(dr, min_dr, max_dr) == 0) {
        return false;
    }

    for (uint8_t i = 0; i < phy_params.max_channel_cnt; i++) {
        if (mask_bit_test(channel_mask, i)) {
            // Check datarate validity for enabled channels
            if (val_in_range(dr, (channels[i].dr_range.fields.min & 0x0F),
                                 (channels[i].dr_range.fields.max & 0x0F))) {
                // At least 1 channel has been found we can return OK.
                return true;
            }
        }
    }

    return false;
}

uint8_t LoRaPHY::val_in_range( int8_t value, int8_t min, int8_t max )
{
    if ((value >= min) && (value <= max)) {
        return 1;
    }

    return 0;
}

bool LoRaPHY::disable_channel(uint16_t* channel_mask, uint8_t id,
                              uint8_t max_channels_num)
{
    uint8_t index = id / 16;

    if ((index > phy_params.channels.mask_size) || (id >= max_channels_num)) {
        return false;
    }

    // Deactivate channel
    mask_bit_clear(channel_mask, id);

    return true;
}

uint8_t LoRaPHY::count_bits(uint16_t mask, uint8_t nbBits)
{
    uint8_t nbActiveBits = 0;

    for(uint8_t j = 0; j < nbBits; j++) {
        if (mask_bit_test(&mask, j)) {
            nbActiveBits++;
        }
    }

    return nbActiveBits;
}

uint8_t LoRaPHY::num_active_channels(uint16_t* channel_mask, uint8_t start_idx,
                                     uint8_t stop_idx)
{
    uint8_t nb_channels = 0;

    if (channel_mask == NULL) {
        return 0;
    }

    for (uint8_t i = start_idx; i < stop_idx; i++) {
        nb_channels += count_bits(channel_mask[i], 16);
    }

    return nb_channels;
}

void LoRaPHY::copy_channel_mask(uint16_t* dest_mask, uint16_t* src_mask, uint8_t len)
{
    if ((dest_mask != NULL) && (src_mask != NULL)) {
        for( uint8_t i = 0; i < len; i++ ) {
            dest_mask[i] = src_mask[i];
        }
    }
}

void LoRaPHY::set_last_tx_done(set_band_txdone_params_t* last_tx_params)
{
    if (!last_tx_params) {
        return;
    }

    band_t *band_table = (band_t *) phy_params.bands.table;
    channel_params_t *channel_list = phy_params.channels.channel_list;

    if (last_tx_params->joined == true) {
        band_table[channel_list[last_tx_params->channel].band].last_tx_time = last_tx_params->last_tx_done_time;
        return;
    }

    band_table[channel_list[last_tx_params->channel].band].last_tx_time = last_tx_params->last_tx_done_time;
    band_table[channel_list[last_tx_params->channel].band].last_join_tx_time = last_tx_params->last_tx_done_time;

}

lorawan_time_t LoRaPHY::update_band_timeoff(bool joined, bool duty_cycle,
                                            band_t* bands, uint8_t nb_bands)
{
    lorawan_time_t next_tx_delay = (lorawan_time_t) (-1);

    // Update bands Time OFF
    for (uint8_t i = 0; i < nb_bands; i++) {

        if (joined == false) {
            uint32_t txDoneTime =  MAX(_lora_time.get_elapsed_time(bands[i].last_join_tx_time),
                                        (duty_cycle == true) ?
                                        _lora_time.get_elapsed_time(bands[i].last_tx_time) : 0);

            if (bands[i].off_time <= txDoneTime) {
                bands[i].off_time = 0;
            }

            if (bands[i].off_time != 0) {
                next_tx_delay = MIN( bands[i].off_time - txDoneTime, next_tx_delay );
            }

        } else {
            // if network has been joined
            if (duty_cycle == true) {

                if( bands[i].off_time <= _lora_time.get_elapsed_time(bands[i].last_tx_time)) {
                    bands[i].off_time = 0;
                }

                if(bands[i].off_time != 0 ) {
                    next_tx_delay = MIN(bands[i].off_time - _lora_time.get_elapsed_time(bands[i].last_tx_time),
                                       next_tx_delay);
                }
            } else {
                // if duty cycle is not on
                next_tx_delay = 0;
                bands[i].off_time = 0;
            }
        }
    }

    return next_tx_delay;
}

uint8_t LoRaPHY::parse_link_ADR_req(uint8_t* payload, link_adr_params_t* params)
{
    uint8_t ret_index = 0;

    if (payload[0] == SRV_MAC_LINK_ADR_REQ) {

        // Parse datarate and tx power
        params->datarate = payload[1];
        params->tx_power = params->datarate & 0x0F;
        params->datarate = (params->datarate >> 4) & 0x0F;

        // Parse ChMask
        params->channel_mask = (uint16_t) payload[2];
        params->channel_mask |= (uint16_t) payload[3] << 8;

        // Parse ChMaskCtrl and nbRep
        params->nb_rep = payload[4];
        params->ch_mask_ctrl = ( params->nb_rep >> 4 ) & 0x07;
        params->nb_rep &= 0x0F;

        // LinkAdrReq has 4 bytes length + 1 byte CMD
        ret_index = 5;
    }

    return ret_index;
}

uint8_t LoRaPHY::verify_link_ADR_req(verify_adr_params_t* verify_params,
                                     int8_t* dr, int8_t* tx_pow, uint8_t* nb_rep)
{
    uint8_t status = verify_params->status;
    int8_t datarate = verify_params->datarate;
    int8_t tx_power = verify_params->tx_power;
    int8_t nb_repetitions = verify_params->nb_rep;

    // Handle the case when ADR is off.
    if (verify_params->adr_enabled == false) {
        // When ADR is off, we are allowed to change the channels mask and the NbRep,
        // if the datarate and the TX power of the LinkAdrReq are set to 0x0F.
        if ((verify_params->datarate != 0x0F) || (verify_params->tx_power != 0x0F)) {
            status = 0;
            nb_repetitions = verify_params->current_nb_rep;
        }

        // Get the current datarate and tx power
        datarate = verify_params->current_datarate;
        tx_power = verify_params->current_tx_power;
    }

    if (status != 0) {
        // Verify channel datarate
        if (verify_channel_DR(phy_params.max_channel_cnt, verify_params->channel_mask,
                              datarate, phy_params.min_tx_datarate,
                              phy_params.max_tx_datarate, phy_params.channels.channel_list)
                == false) {
            status &= 0xFD; // Datarate KO
        }

        // Verify tx power
        if (val_in_range(tx_power, phy_params.max_tx_power,
                         phy_params.min_tx_power) == 0) {
            // Verify if the maximum TX power is exceeded
            if (phy_params.max_tx_power > tx_power) {
                // Apply maximum TX power. Accept TX power.
                tx_power = phy_params.max_tx_power;
            } else {
                status &= 0xFB; // TxPower KO
            }
        }
    }

    // If the status is ok, verify the NbRep
    if (status == 0x07 && nb_repetitions == 0) {
        // Restore the default value according to the LoRaWAN specification
        nb_repetitions = 1;
    }

    // Apply changes
    *dr = datarate;
    *tx_pow = tx_power;
    *nb_rep = nb_repetitions;

    return status;
}

double LoRaPHY::compute_symb_timeout_lora(uint8_t phy_dr, uint32_t bandwidth)
{
    return ((double)(1 << phy_dr) / (double) bandwidth) * 1000;
}

double LoRaPHY::compute_symb_timeout_fsk(uint8_t phy_dr)
{
    return (8.0 / (double) phy_dr); // 1 symbol equals 1 byte
}

void LoRaPHY::get_rx_window_params(double t_symb, uint8_t min_rx_symb,
                                   uint32_t rx_error, uint32_t wakeup_time,
                                   uint32_t* window_timeout, int32_t* window_offset)
{
    // Computed number of symbols
    *window_timeout = MAX ((uint32_t) ceil(((2 * min_rx_symb - 8) * t_symb + 2 * rx_error) / t_symb), min_rx_symb );
    *window_offset = (int32_t) ceil((4.0 * t_symb) - ((*window_timeout * t_symb) / 2.0 ) - wakeup_time);
}

int8_t LoRaPHY::compute_tx_power(int8_t tx_power_idx, float max_eirp,
                                 float antenna_gain)
{
    int8_t phy_tx_power = 0;

    phy_tx_power = (int8_t) floor((max_eirp - (tx_power_idx * 2U)) - antenna_gain);

    return phy_tx_power;
}


int8_t LoRaPHY::get_next_lower_dr(int8_t dr, int8_t min_dr)
{
    uint8_t next_lower_dr = 0;

    if (dr == min_dr) {
        next_lower_dr = min_dr;
    } else {
        next_lower_dr = dr - 1;
    }

    return next_lower_dr;
}

uint8_t LoRaPHY::get_bandwidth(uint8_t dr)
{
    uint32_t *bandwidths = (uint32_t *) phy_params.bandwidths.table;

    switch(bandwidths[dr]) {
        default:
        case 125000:
            return 0;
        case 250000:
            return 1;
        case 500000:
            return 2;
    }
}

uint8_t LoRaPHY::enabled_channel_count(bool joined, uint8_t datarate,
                                       const uint16_t *channel_mask,
                                       uint8_t *channel_indices,
                                       uint8_t *delayTx)
{
    uint8_t count = 0;
    uint8_t delay_transmission = 0;

    for (uint8_t i = 0; i < phy_params.max_channel_cnt; i++) {
        if (mask_bit_test(channel_mask, i)) {

            if (val_in_range(datarate, phy_params.channels.channel_list[i].dr_range.fields.min,
                             phy_params.channels.channel_list[i].dr_range.fields.max ) == 0) {
                // data rate range invalid for this channel
                continue;
            }

            band_t *band_table = (band_t *) phy_params.bands.table;
            if (band_table[phy_params.channels.channel_list[i].band].off_time > 0) {
                // Check if the band is available for transmission
                delay_transmission++;
                continue;
            }

            // otherwise count the channel as enabled
            channel_indices[count++] = i;
        }
    }

    *delayTx = delay_transmission;

    return count;
}

phy_param_t LoRaPHY::get_phy_params(get_phy_params_t* getPhy)
{
    phy_param_t phyParam = { 0 };

    switch (getPhy->attribute) {
        case PHY_MIN_RX_DR: {
            if (phy_params.dl_dwell_time_setting == 0) {
                phyParam.value = phy_params.min_rx_datarate;
            } else {
                phyParam.value = phy_params.dwell_limit_datarate;
            }
            break;
        }
        case PHY_MIN_TX_DR: {
            if (phy_params.ul_dwell_time_setting == 0) {
                phyParam.value = phy_params.min_tx_datarate;
            } else {
                phyParam.value = phy_params.dwell_limit_datarate;
            }
            break;
        }
        case PHY_DEF_TX_DR: {
            phyParam.value = phy_params.default_datarate;
            break;
        }
        case PHY_NEXT_LOWER_TX_DR: {
            if (phy_params.ul_dwell_time_setting == 0) {
                phyParam.value = get_next_lower_dr(getPhy->datarate,
                                                   phy_params.min_tx_datarate);
            } else {
                phyParam.value = get_next_lower_dr(
                        getPhy->datarate, phy_params.dwell_limit_datarate);
            }
            break;
        }
        case PHY_DEF_TX_POWER: {
            phyParam.value = phy_params.default_tx_power;
            break;
        }
        case PHY_MAX_PAYLOAD: {
            uint8_t *payload_table = (uint8_t *) phy_params.payloads.table;
            phyParam.value = payload_table[getPhy->datarate];
            break;
        }
        case PHY_MAX_PAYLOAD_REPEATER: {
            uint8_t *payload_table = (uint8_t *) phy_params.payloads_with_repeater.table;
            phyParam.value = payload_table[getPhy->datarate];
            break;
        }
        case PHY_DUTY_CYCLE: {
            phyParam.value = phy_params.duty_cycle_enabled;
            break;
        }
        case PHY_MAX_RX_WINDOW: {
            phyParam.value = phy_params.max_rx_window;
            break;
        }
        case PHY_RECEIVE_DELAY1: {
            phyParam.value = phy_params.recv_delay1;
            break;
        }
        case PHY_RECEIVE_DELAY2: {
            phyParam.value = phy_params.recv_delay2;
            break;
        }
        case PHY_JOIN_ACCEPT_DELAY1: {
            phyParam.value = phy_params.join_accept_delay1;
            break;
        }
        case PHY_JOIN_ACCEPT_DELAY2: {
            phyParam.value = phy_params.join_accept_delay2;
            break;
        }
        case PHY_MAX_FCNT_GAP: {
            phyParam.value = phy_params.max_fcnt_gap;
            break;
        }
        case PHY_ACK_TIMEOUT: {
            uint16_t ack_timeout = phy_params.ack_timeout;
            uint16_t ack_timeout_rnd = phy_params.ack_timeout_rnd;
            phyParam.value = (ack_timeout
                    + get_random(-ack_timeout_rnd, ack_timeout_rnd));
            break;
        }
        case PHY_DEF_DR1_OFFSET: {
            phyParam.value = phy_params.default_rx1_dr_offset;
            break;
        }
        case PHY_DEF_RX2_FREQUENCY: {
            phyParam.value = phy_params.rx_window2_frequency;
            break;
        }
        case PHY_DEF_RX2_DR: {
            phyParam.value = phy_params.rx_window2_datarate;
            break;
        }
        case PHY_CHANNEL_MASK: {
            phyParam.channel_mask = phy_params.channels.mask;
            break;
        }
        case PHY_DEFAULT_CHANNEL_MASK: {
            phyParam.channel_mask = phy_params.channels.default_mask;
            break;
        }
        case PHY_MAX_NB_CHANNELS: {
            phyParam.value = phy_params.max_channel_cnt;
            break;
        }
        case PHY_CHANNELS: {
            phyParam.channel_params = phy_params.channels.channel_list;
            break;
        }
        case PHY_CUSTOM_CHANNEL_PLAN_SUPPORT:
            // 0 if custom channel plans are not supported (in LoRaWAN terms
            // the regions who do not support custom channels are called as
            // regions with dynamic channel plans)
            phyParam.value = (uint32_t) phy_params.custom_channelplans_supported;
            break;
        case PHY_DEF_UPLINK_DWELL_TIME: {
            phyParam.value = phy_params.ul_dwell_time_setting;
            break;
        }
        case PHY_DEF_DOWNLINK_DWELL_TIME: {
            phyParam.value = phy_params.dl_dwell_time_setting;
            break;
        }
        case PHY_DEF_MAX_EIRP: {
            phyParam.f_value = phy_params.default_max_eirp;
            break;
        }
        case PHY_DEF_ANTENNA_GAIN: {
            phyParam.f_value = phy_params.default_antenna_gain;
            break;
        }
        case PHY_NB_JOIN_TRIALS:
        case PHY_DEF_NB_JOIN_TRIALS: {
            phyParam.value = MBED_CONF_LORA_NB_TRIALS;
            break;
        }
        default: {
            break;
        }
    }

    return phyParam;
}

void LoRaPHY::restore_default_channels()
{
    // Restore channels default mask
    for (uint8_t i=0; i < phy_params.channels.mask_size; i++) {
        phy_params.channels.mask[i] |= phy_params.channels.default_mask[i];
    }
}

bool LoRaPHY::verify(verification_params_t* verify, phy_attributes_t phy_attribute)
{
    switch(phy_attribute) {
        case PHY_TX_DR:
        {
            if (phy_params.ul_dwell_time_setting == 0) {
                return val_in_range(verify->datarate,
                                    phy_params.min_tx_datarate,
                                    phy_params.max_tx_datarate);
            } else {
                return val_in_range(verify->datarate,
                                    phy_params.dwell_limit_datarate,
                                    phy_params.max_tx_datarate);
            }

        }
        case PHY_DEF_TX_DR:
        {
            return val_in_range(verify->datarate,
                                phy_params.default_datarate,
                                phy_params.default_max_datarate);
        }
        case PHY_RX_DR:
        {
            if (phy_params.dl_dwell_time_setting == 0) {
                return val_in_range(verify->datarate,
                                    phy_params.min_rx_datarate,
                                    phy_params.max_rx_datarate);
            } else {
                return val_in_range(verify->datarate,
                                    phy_params.dwell_limit_datarate,
                                    phy_params.max_rx_datarate);
            }
        }
        case PHY_DEF_TX_POWER:
        case PHY_TX_POWER:
        {
            // Remark: switched min and max!
            return val_in_range(verify->tx_power, phy_params.max_tx_power,
                                phy_params.min_tx_power);
        }
        case PHY_DUTY_CYCLE:
        {
            if (verify->duty_cycle == phy_params.duty_cycle_enabled) {
                return true;
            }

            return false;
        }
        case PHY_NB_JOIN_TRIALS:
        {
            if (verify->nb_join_trials < MBED_CONF_LORA_NB_TRIALS) {
                return false;
            }
            break;
        }
        default:
            return false;
    }

    return true;
}

void LoRaPHY::apply_cf_list(cflist_params_t* cf_list)
{
    // if the underlying PHY doesn't support CF-List, ignore the request
    if (!phy_params.cflist_supported) {
        return;
    }

    channel_params_t new_channel;

    // Setup default datarate range
    new_channel.dr_range.value = (phy_params.default_max_datarate << 4)
                                  | phy_params.default_datarate;

    // Size of the optional CF list
    if (cf_list->size != 16) {
        return;
    }

    // Last byte is RFU, don't take it into account
    // NOTE: Currently the PHY layers supported by LoRaWAN who accept a CF-List
    // define first 2 or 3 channels as default channels. this function is
    // written keeping that in mind. If there would be a PHY in the future that
    // accepts CF-list but have haphazard allocation of default channels, we
    // should override this function in the implementation of that particular
    // PHY.
    for (uint8_t i = 0, channel_id = phy_params.default_channel_cnt;
         channel_id < phy_params.max_channel_cnt; i+=3, channel_id++) {
        if (channel_id < (phy_params.cflist_channel_cnt + phy_params.default_channel_cnt)) {
            // Channel frequency
            new_channel.frequency = (uint32_t) cf_list->payload[i];
            new_channel.frequency |= ((uint32_t) cf_list->payload[i + 1] << 8);
            new_channel.frequency |= ((uint32_t) cf_list->payload[i + 2] << 16);
            new_channel.frequency *= 100;

            // Initialize alternative frequency to 0
            new_channel.rx1_frequency = 0;
        } else {
            new_channel.frequency = 0;
            new_channel.dr_range.value = 0;
            new_channel.rx1_frequency = 0;
        }

        if (new_channel.frequency != 0) {
            // Try to add channel
            add_channel(&new_channel, channel_id);
        } else {
            remove_channel(channel_id);
        }
    }
}


bool LoRaPHY::get_next_ADR(bool restore_channel_mask, int8_t& dr_out,
                           int8_t& tx_power_out, uint32_t& adr_ack_cnt)
{
    bool set_adr_ack_bit = false;

    get_phy_params_t get_phy;
    phy_param_t phy_param;
    uint16_t ack_limit_plus_delay = phy_params.adr_ack_limit + phy_params.adr_ack_delay;

    if (dr_out == phy_params.min_tx_datarate) {
        adr_ack_cnt = 0;
        return set_adr_ack_bit;
    }

    if (adr_ack_cnt < phy_params.adr_ack_limit) {
        return set_adr_ack_bit;
    }

    // ADR ack counter is larger than ADR-ACK-LIMIT
    set_adr_ack_bit = true;
    tx_power_out = phy_params.max_tx_power;

    if (adr_ack_cnt >= ack_limit_plus_delay) {
        if ((adr_ack_cnt % phy_params.adr_ack_delay) == 1) {
            // Decrease the datarate
            get_phy.attribute = PHY_NEXT_LOWER_TX_DR;
            get_phy.datarate = dr_out;
            phy_param = get_phy_params(&get_phy);
            dr_out = phy_param.value;

            if (dr_out == phy_params.min_tx_datarate) {
                // We must set adrAckReq to false as soon as we reach the lowest datarate
                set_adr_ack_bit = false;
                if (restore_channel_mask) {
                    // Re-enable default channels
                    restore_default_channels();
                }
            }
        }
    }

    return set_adr_ack_bit;
}

void LoRaPHY::compute_rx_win_params(int8_t datarate, uint8_t min_rx_symbols,
                                    uint32_t rx_error,
                                    rx_config_params_t *rx_conf_params)
{
    double t_symbol = 0.0;

    // Get the datarate, perform a boundary check
    rx_conf_params->datarate = MIN( datarate, phy_params.max_rx_datarate);

    rx_conf_params->bandwidth = get_bandwidth(rx_conf_params->datarate);

    if (phy_params.fsk_supported && rx_conf_params->datarate == phy_params.max_rx_datarate) {
        // FSK
        t_symbol = compute_symb_timeout_fsk(((uint8_t *)phy_params.datarates.table)[rx_conf_params->datarate]);
    } else {
        // LoRa
        t_symbol = compute_symb_timeout_lora(((uint8_t *)phy_params.datarates.table)[rx_conf_params->datarate],
                                            ((uint32_t *)phy_params.bandwidths.table)[rx_conf_params->datarate]);
    }

    get_rx_window_params(t_symbol, min_rx_symbols, rx_error, RADIO_WAKEUP_TIME,
                         &rx_conf_params->window_timeout, &rx_conf_params->window_offset);
}

bool LoRaPHY::rx_config(rx_config_params_t* rx_conf, int8_t* datarate)
{
    radio_modems_t modem;
    uint8_t dr = rx_conf->datarate;
    uint8_t max_payload = 0;
    uint8_t phy_dr = 0;
    uint32_t frequency = rx_conf->frequency;

    _radio->lock();

    if (_radio->get_status() != RF_IDLE) {
        _radio->unlock();
        return false;
    }

    _radio->unlock();

    if (rx_conf->rx_slot == RX_SLOT_WIN_1) {
        // Apply window 1 frequency
        frequency = phy_params.channels.channel_list[rx_conf->channel].frequency;
        // Apply the alternative RX 1 window frequency, if it is available
        if (phy_params.channels.channel_list[rx_conf->channel].rx1_frequency != 0) {
            frequency = phy_params.channels.channel_list[rx_conf->channel].rx1_frequency;
        }
    }

    // Read the physical datarate from the datarates table
    uint8_t *datarate_table = (uint8_t *) phy_params.datarates.table;
    uint8_t *payload_table = (uint8_t *) phy_params.payloads.table;
    uint8_t *payload_with_repeater_table = (uint8_t *) phy_params.payloads_with_repeater.table;

    phy_dr = datarate_table[dr];

    _radio->lock();

    _radio->set_channel(frequency);

    // Radio configuration
    if (dr == DR_7 && phy_params.fsk_supported) {
        modem = MODEM_FSK;
        _radio->set_rx_config(modem, 50000, phy_dr * 1000, 0, 83333, 5,
                              rx_conf->window_timeout, false, 0, true, 0, 0,
                              false, rx_conf->is_rx_continuous);
    } else {
        modem = MODEM_LORA;
        _radio->set_rx_config(modem, rx_conf->bandwidth, phy_dr, 1, 0, 8,
                              rx_conf->window_timeout, false, 0, false, 0, 0,
                              true, rx_conf->is_rx_continuous);
    }

    if (rx_conf->is_repeater_supported) {
        max_payload = payload_with_repeater_table[dr];
    } else {
        max_payload = payload_table[dr];
    }

    _radio->set_max_payload_length(modem, max_payload + LORA_MAC_FRMPAYLOAD_OVERHEAD);

    _radio->unlock();

    *datarate = phy_dr;

    return true;
}

bool LoRaPHY::tx_config(tx_config_params_t* tx_conf, int8_t* tx_power,
                        lorawan_time_t* tx_toa)
{
    radio_modems_t modem;
    int8_t phy_dr = ((uint8_t *)phy_params.datarates.table)[tx_conf->datarate];
    channel_params_t *list = phy_params.channels.channel_list;
    uint8_t band_idx = list[tx_conf->channel].band;
    band_t *bands = (band_t *)phy_params.bands.table;

    // limit TX power if set to too much
    if (tx_conf->tx_power > bands[band_idx].max_tx_pwr) {
        tx_conf->tx_power = bands[band_idx].max_tx_pwr;
    }

    uint8_t bandwidth = get_bandwidth(tx_conf->datarate);
    int8_t phy_tx_power = 0;

    // Calculate physical TX power
    phy_tx_power = compute_tx_power(tx_conf->tx_power, tx_conf->max_eirp,
                                    tx_conf->antenna_gain);

    _radio->lock();

    // Setup the radio frequency
    _radio->set_channel(list[tx_conf->channel].frequency);

    if( tx_conf->datarate == phy_params.max_tx_datarate ) {
        // High Speed FSK channel
        modem = MODEM_FSK;
        _radio->set_tx_config(modem, phy_tx_power, 25000, bandwidth,
                              phy_dr * 1000, 0, 5, false, true, 0, 0, false,
                              3000);
    } else {
        modem = MODEM_LORA;
        _radio->set_tx_config(modem, phy_tx_power, 0, bandwidth, phy_dr, 1, 8,
                              false, true, 0, 0, false, 3000 );
    }

    // Setup maximum payload lenght of the radio driver
    _radio->set_max_payload_length( modem, tx_conf->pkt_len);
    // Get the time-on-air of the next tx frame
    *tx_toa = _radio->time_on_air(modem, tx_conf->pkt_len);

    _radio->unlock();

    *tx_power = tx_conf->tx_power;

    return true;
}

uint8_t LoRaPHY::link_ADR_request(adr_req_params_t* link_adr_req,
                                  int8_t* dr_out, int8_t* tx_power_out,
                                  uint8_t* nb_rep_out, uint8_t* nb_bytes_processed)
{
    uint8_t status = 0x07;
    link_adr_params_t adr_settings;
    uint8_t next_index = 0;
    uint8_t bytes_processed = 0;

    // rather than dynamically allocating memory, we choose to set
    // a channel mask list size of unity here as we know that all
    // the PHY layer implementations who have more than 16 channels, i.e.,
    // have channel mask list size more than unity, override this method.
    uint16_t temp_channel_mask[1] = {0};

    verify_adr_params_t verify_params;

    while (bytes_processed < link_adr_req->payload_size) {
        // Get ADR request parameters
        next_index = parse_link_ADR_req(&(link_adr_req->payload[bytes_processed]),
                                       &adr_settings);

        if (next_index == 0) {
            break; // break loop, since no more request has been found
        }

        // Update bytes processed
        bytes_processed += next_index;

        // Revert status, as we only check the last ADR request for the channel mask KO
        status = 0x07;

        // Setup temporary channels mask
        temp_channel_mask[0] = adr_settings.channel_mask;

        // Verify channels mask
        if (adr_settings.ch_mask_ctrl == 0 && temp_channel_mask[0] == 0) {
            status &= 0xFE; // Channel mask KO
        }

        // channel mask applies to first 16 channels
        if (adr_settings.ch_mask_ctrl == 0 ||
            adr_settings.ch_mask_ctrl == 6) {

            for (uint8_t i = 0; i < phy_params.max_channel_cnt; i++) {

                // turn on all channels if channel mask control is 6
                if (adr_settings.ch_mask_ctrl == 6) {
                    if (phy_params.channels.channel_list[i].frequency != 0) {
                        mask_bit_set(temp_channel_mask, i);
                    }

                    continue;
                }

                // if channel mask control is 0, we test the bits and
                // frequencies and change the status if we find a discrepancy
                if ((mask_bit_test(temp_channel_mask, i)) &&
                    (phy_params.channels.channel_list[i].frequency == 0)) {
                    // Trying to enable an undefined channel
                    status &= 0xFE; // Channel mask KO
                }
            }
        } else {
            // Channel mask control applies to RFUs
            status &= 0xFE; // Channel mask KO
        }
    }

    verify_params.status = status;

    verify_params.adr_enabled = link_adr_req->adr_enabled;
    verify_params.current_datarate = link_adr_req->current_datarate;
    verify_params.current_tx_power = link_adr_req->current_tx_power;
    verify_params.current_nb_rep = link_adr_req->current_nb_rep;

    verify_params.datarate = adr_settings.datarate;
    verify_params.tx_power = adr_settings.tx_power;
    verify_params.nb_rep = adr_settings.nb_rep;


    verify_params.channel_mask = temp_channel_mask;

    // Verify the parameters and update, if necessary
    status = verify_link_ADR_req(&verify_params, &adr_settings.datarate,
                                 &adr_settings.tx_power, &adr_settings.nb_rep);

    // Update channelsMask if everything is correct
    if (status == 0x07) {
        // Set the channels mask to a default value
        memset(phy_params.channels.mask, 0,
               sizeof(uint16_t)*phy_params.channels.mask_size);

        // Update the channels mask
        copy_channel_mask(phy_params.channels.mask, temp_channel_mask,
                          phy_params.channels.mask_size);
    }

    // Update status variables
    *dr_out = adr_settings.datarate;
    *tx_power_out = adr_settings.tx_power;
    *nb_rep_out = adr_settings.nb_rep;
    *nb_bytes_processed = bytes_processed;

    return status;
}

uint8_t LoRaPHY::accept_rx_param_setup_req(rx_param_setup_req_t* params)
{
    uint8_t status = 0x07;

    // Verify radio frequency
    if (_radio->check_rf_frequency(params->frequency) == false) {
        status &= 0xFE; // Channel frequency KO
    }

    // Verify datarate
    if (val_in_range(params->datarate, phy_params.min_rx_datarate,
                     phy_params.max_rx_datarate) == 0) {
        status &= 0xFD; // Datarate KO
    }

    // Verify datarate offset
    if (val_in_range(params->dr_offset, phy_params.min_rx1_dr_offset,
                     phy_params.max_rx1_dr_offset) == 0) {
        status &= 0xFB; // Rx1DrOffset range KO
    }

    return status;
}

bool LoRaPHY::accept_tx_param_setup_req(tx_param_setup_req_t *params)
{
    if (phy_params.accept_tx_param_setup_req) {
        phy_params.ul_dwell_time_setting = params->ul_dwell_time;
        phy_params.dl_dwell_time_setting = params->dl_dwell_time;
    }

    return phy_params.accept_tx_param_setup_req;
}

bool LoRaPHY::verify_frequency(uint32_t freq)
{
    band_t *bands_table = (band_t *)phy_params.bands.table;

    // check all sub bands (if there are sub-bands) to check if the given
    // frequency falls into any of the frequency ranges

    for (uint8_t i=0; i<phy_params.bands.size; i++) {
        if (freq <= bands_table[i].higher_band_freq
                 && freq >= bands_table[i].lower_band_freq) {
            return true;
        }
    }

    return false;
}

uint8_t LoRaPHY::dl_channel_request(dl_channel_req_params_t* params)
{
    if (!phy_params.dl_channel_req_supported) {
        return 0;
    }

    uint8_t status = 0x03;

    // Verify if the frequency is supported
    if (verify_frequency(params->rx1_frequency) == false) {
        status &= 0xFE;
    }

    // Verify if an uplink frequency exists
    if (phy_params.channels.channel_list[params->channel_id].frequency == 0) {
        status &= 0xFD;
    }

    // Apply Rx1 frequency, if the status is OK
    if (status == 0x03) {
        phy_params.channels.channel_list[params->channel_id].rx1_frequency = params->rx1_frequency;
    }

    return status;
}

/**
 * Alternate datarate algorithm for join requests.
 *  - We check from the PHY and take note of total
 *    number of data rates available upto the default data rate for
 *    default channels.
 *
 *  - Application sets a total number of re-trials for a Join Request, i.e.,
 *    MBED_CONF_LORA_NB_TRIALS. So MAC layer will send us a counter
 *    nb_trials < MBED_CONF_LORA_NB_TRIALS which is the current number of trial.
 *
 *  - We roll over total available datarates and pick one according to the
 *    number of trial sequentially.
 *
 *  - We always start from the Default Data rate and and set the next lower
 *    data rate for every iteration.
 *
 *  - MAC layer will stop when maximum number of re-trials, i.e.,
 *    MBED_CONF_LORA_NB_TRIALS is achieved.
 *
 * So essentially MBED_CONF_LORA_NB_TRIALS should be a multiple of range of
 * data rates available. For example, in EU868 band, default max. data rate is
 * DR_5 and min. data rate is DR_0, so total data rates available are 6.
 *
 * Hence MBED_CONF_LORA_NB_TRIALS should be a multiple of 6. Setting,
 * MBED_CONF_LORA_NB_TRIALS = 6 would mean that every data rate will be tried
 * exactly once starting from the largest and finishing at the smallest.
 *
 * PHY layers which do not have datarates scheme similar to EU band will ofcourse
 * override this method.
 */
int8_t LoRaPHY::get_alternate_DR(uint8_t nb_trials)
{
    int8_t datarate = 0;
    uint8_t total_nb_datrates = (phy_params.default_max_datarate - phy_params.min_tx_datarate) + 1;

    uint8_t res = nb_trials % total_nb_datrates;

    if (res == 0) {
        datarate = phy_params.min_tx_datarate;
    } else if (res == 1) {
        datarate = phy_params.default_max_datarate;
    } else {
        datarate = (phy_params.default_max_datarate - res) + 1;
    }

    return datarate;
}

void LoRaPHY::calculate_backoff(backoff_params_t* calc_backoff)
{
    band_t *band_table = (band_t *) phy_params.bands.table;
    channel_params_t *channel_list = phy_params.channels.channel_list;

    uint8_t band_idx = channel_list[calc_backoff->channel].band;
    uint16_t duty_cycle = band_table[band_idx].duty_cycle;
    uint16_t join_duty_cycle = 0;

    // Reset time-off to initial value.
    band_table[band_idx].off_time = 0;

    if (calc_backoff->joined == false) {
        // Get the join duty cycle
        if (calc_backoff->elapsed_time < 3600000) {
            join_duty_cycle = BACKOFF_DC_1_HOUR;
        } else if (calc_backoff->elapsed_time < (3600000 + 36000000)) {
            join_duty_cycle = BACKOFF_DC_10_HOURS;
        } else {
            join_duty_cycle = BACKOFF_DC_24_HOURS;
        }

        // Apply the most restricting duty cycle
        duty_cycle = MAX(duty_cycle, join_duty_cycle);
    }

    // No back-off if the last frame was not a join request and when the
    // duty cycle is not enabled
    if (calc_backoff->dc_enabled == false &&
        calc_backoff->last_tx_was_join_req == false) {
        band_table[band_idx].off_time = 0;
    } else {
        // Apply band time-off.
        band_table[band_idx].off_time = calc_backoff->tx_toa * duty_cycle - calc_backoff->tx_toa;
    }
}

bool LoRaPHY::set_next_channel(channel_selection_params_t* params,
                               uint8_t* channel, lorawan_time_t* time,
                               lorawan_time_t* aggregate_timeoff)
{
    uint8_t channel_count = 0;
    uint8_t delay_tx = 0;

    // Note here that the PHY layer implementations which have more than
    // 16 channels at their disposal, override this function. That's why
    // it is safe to assume that we are dealing with a block of 16 channels
    // i.e., EU like implementations. So rather than dynamically allocating
    // memory we chose to use a magic number of 16
    uint8_t enabled_channels[16];

    memset(enabled_channels, 0xFF, sizeof(uint8_t)*16);

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
            <= _lora_time.get_elapsed_time(params->last_aggregate_tx_time)) {
        // Reset Aggregated time off
        *aggregate_timeoff = 0;

        // Update bands Time OFF
        next_tx_delay = update_band_timeoff(params->joined,
                                          params->dc_enabled,
                                          band_table, phy_params.bands.size);

        // Search how many channels are enabled
        channel_count = enabled_channel_count(params->joined, params->current_datarate,
                                                  phy_params.channels.mask,
                                                  enabled_channels, &delay_tx);
    } else {
        delay_tx++;
        next_tx_delay = params->aggregate_timeoff
                - _lora_time.get_elapsed_time(params->last_aggregate_tx_time);
    }

    if (channel_count > 0) {
        // We found a valid channel
        *channel = enabled_channels[get_random(0, channel_count - 1)];
        *time = 0;
        return true;
    }

    if (delay_tx > 0) {
        // Delay transmission due to AggregatedTimeOff or to a band time off
        *time = next_tx_delay;
        return true;
    }

    // Datarate not supported by any channel, restore defaults
    copy_channel_mask(phy_params.channels.mask,
                      phy_params.channels.default_mask,
                      phy_params.channels.mask_size);
    *time = 0;
    return false;
}

lorawan_status_t LoRaPHY::add_channel(channel_params_t* new_channel, uint8_t id)
{
    bool dr_invalid = false;
    bool freq_invalid = false;

    if (!phy_params.custom_channelplans_supported
            || id >= phy_params.max_channel_cnt) {

        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    // Validate the datarate range
    if (val_in_range(new_channel->dr_range.fields.min,
                     phy_params.min_tx_datarate,
                     phy_params.max_tx_datarate) == 0) {
        dr_invalid = true;
    }

    if (val_in_range(new_channel->dr_range.fields.max, phy_params.min_tx_datarate,
                     phy_params.max_tx_datarate) == 0) {
        dr_invalid = true;
    }

    if (new_channel->dr_range.fields.min > new_channel->dr_range.fields.max) {
        dr_invalid = true;
    }

    // Default channels don't accept all values
    if (id < phy_params.default_channel_cnt) {
        // Validate the datarate range for min: must be DR_0
        if (new_channel->dr_range.fields.min > phy_params.min_tx_datarate) {
            dr_invalid = true;
        }

        // Validate the datarate range for max: must be DR_5 <= Max <= TX_MAX_DATARATE
        if (val_in_range(new_channel->dr_range.fields.max,
                         phy_params.default_max_datarate,
                         phy_params.max_tx_datarate) == 0) {
            dr_invalid = true;
        }

        // We are not allowed to change the frequency
        if (new_channel->frequency != phy_params.channels.channel_list[id].frequency) {
            freq_invalid = true;
        }
    }

    // Check frequency
    if (!freq_invalid) {
        if (verify_frequency(new_channel->frequency) == false) {
            freq_invalid = true;
        }
    }

    // Check status
    if (dr_invalid && freq_invalid) {
        return LORAWAN_STATUS_FREQ_AND_DR_INVALID;
    }

    if (dr_invalid) {
        return LORAWAN_STATUS_DATARATE_INVALID;
    }

    if (freq_invalid) {
        return LORAWAN_STATUS_FREQUENCY_INVALID;
    }

    memcpy(&(phy_params.channels.channel_list[id]), new_channel, sizeof(channel_params_t));

    phy_params.channels.channel_list[id].band = new_channel->band;

    mask_bit_set(phy_params.channels.mask, id);

    return LORAWAN_STATUS_OK;
}

bool LoRaPHY::remove_channel(uint8_t channel_id)
{
    // upper layers are checking if the custom channel planning is supported or
    // not. So we don't need to worry about that
    if (mask_bit_test(phy_params.channels.default_mask, channel_id)) {
        return false;
    }


    // Remove the channel from the list of channels
    const channel_params_t empty_channel = { 0, 0, { 0 }, 0 };
    phy_params.channels.channel_list[channel_id] = empty_channel;

    return disable_channel(phy_params.channels.mask, channel_id,
                           phy_params.max_channel_cnt);
}

void LoRaPHY::set_tx_cont_mode(cw_mode_params_t* params, uint32_t given_frequency)
{
    band_t *bands_table = (band_t *) phy_params.bands.table;
    channel_params_t *channels = phy_params.channels.channel_list;

    if (params->tx_power > bands_table[channels[params->channel].band].max_tx_pwr) {
        params->tx_power = bands_table[channels[params->channel].band].max_tx_pwr;
    }

    int8_t phy_tx_power = 0;
    uint32_t frequency  = 0;

    if (given_frequency == 0) {
       frequency = channels[params->channel].frequency;
    } else {
        frequency = given_frequency;
    }

    // Calculate physical TX power
    if (params->max_eirp > 0 && params->antenna_gain > 0) {
        phy_tx_power = compute_tx_power(params->tx_power, params->max_eirp,
                                    params->antenna_gain );
    } else {
        phy_tx_power = params->tx_power;
    }

    _radio->lock();
    _radio->set_tx_continuous_wave(frequency, phy_tx_power, params->timeout);
    _radio->unlock();
}

uint8_t LoRaPHY::apply_DR_offset(int8_t dr, int8_t dr_offset)
{
    int8_t datarate = dr - dr_offset;

    if (datarate < 0) {
        datarate = phy_params.min_tx_datarate;
    }

    return datarate;
}


