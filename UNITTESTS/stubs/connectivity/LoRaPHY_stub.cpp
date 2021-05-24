/*
 * Copyright (c) , Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "LoRaPHY.h"
#include "LoRaPHY_stub.h"

LoRaRadio *LoRaPHY_stub::radio = NULL;
uint32_t LoRaPHY_stub::uint32_value = 0;
uint16_t LoRaPHY_stub::uint16_value = 0;
uint8_t LoRaPHY_stub::uint8_value = 0;
int8_t LoRaPHY_stub::int8_value = 0;
int LoRaPHY_stub::int_value = 0;
double LoRaPHY_stub::double_value = 0;
lorawan_status_t LoRaPHY_stub::lorawan_status_value = LORAWAN_STATUS_OK;
channel_params_t *LoRaPHY_stub::channel_params_ptr = NULL;
uint8_t LoRaPHY_stub::bool_counter = 0;
bool LoRaPHY_stub::bool_table[20] = {};
uint8_t LoRaPHY_stub::linkAdrNbBytesParsed = 0;
uint8_t LoRaPHY_stub::ch_mask_value = 0;
uint8_t LoRaPHY_stub::adr_parse_count = 0;

LoRaPHY::LoRaPHY()
    : _radio(LoRaPHY_stub::radio)
{
}

LoRaPHY::~LoRaPHY()
{
}

void LoRaPHY::initialize(LoRaWANTimeHandler *lora_time)
{
}

bool LoRaPHY::mask_bit_test(const uint16_t *mask, unsigned bit)
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

void LoRaPHY::mask_bit_set(uint16_t *mask, unsigned bit)
{
}

void LoRaPHY::mask_bit_clear(uint16_t *mask, unsigned bit)
{
}

void LoRaPHY::set_radio_instance(LoRaRadio &radio)
{
}

void LoRaPHY::put_radio_to_sleep()
{
}

void LoRaPHY::put_radio_to_standby()
{
}

void LoRaPHY::setup_public_network_mode(bool set)
{
}

void LoRaPHY::handle_receive(void)
{
}

uint32_t LoRaPHY::get_radio_rng()
{
    return LoRaPHY_stub::uint32_value;
}

void LoRaPHY::handle_send(uint8_t *buf, uint8_t size)
{
}

uint8_t LoRaPHY::request_new_channel(int8_t channel_id, channel_params_t *new_channel)
{
    return LoRaPHY_stub::uint8_value;
}

int32_t LoRaPHY::get_random(int32_t min, int32_t max)
{
    return LoRaPHY_stub::uint32_value;
}

bool LoRaPHY::verify_channel_DR(uint16_t *channel_mask, int8_t dr)
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

bool LoRaPHY::val_in_range(int8_t value, int8_t min, int8_t max)
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

bool LoRaPHY::disable_channel(uint16_t *channel_mask, uint8_t id,
                              uint8_t max_channels_num)
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

uint8_t LoRaPHY::count_bits(uint16_t mask, uint8_t nbBits)
{
    return LoRaPHY_stub::uint8_value;
}

uint8_t LoRaPHY::num_active_channels(uint16_t *channel_mask, uint8_t start_idx,
                                     uint8_t stop_idx)
{
    return LoRaPHY_stub::uint8_value;
}

void LoRaPHY::copy_channel_mask(uint16_t *dest_mask, uint16_t *src_mask, uint8_t len)
{
    if ((dest_mask != NULL) && (src_mask != NULL)) {
        for (uint8_t i = 0; i < len; i++) {
            dest_mask[i] = src_mask[i];
        }
    }
}

void LoRaPHY::set_last_tx_done(uint8_t channel, bool joined, lorawan_time_t last_tx_done_time)
{
}

lorawan_time_t LoRaPHY::update_band_timeoff(bool joined, bool duty_cycle,
                                            band_t *bands, uint8_t nb_bands)
{
    return LoRaPHY_stub::uint32_value;
}

uint8_t LoRaPHY::parse_link_ADR_req(const uint8_t *payload,
                                    uint8_t payload_size,
                                    link_adr_params_t *params)
{
    params->ch_mask_ctrl = LoRaPHY_stub::ch_mask_value;
    params->channel_mask = 0;
    params->datarate = 0;

    if (LoRaPHY_stub::adr_parse_count) {
        return --LoRaPHY_stub::adr_parse_count;
    }

    return LoRaPHY_stub::uint8_value;
}

uint8_t LoRaPHY::verify_link_ADR_req(verify_adr_params_t *verify_params,
                                     int8_t *dr, int8_t *tx_pow, uint8_t *nb_rep)
{
    return LoRaPHY_stub::uint8_value;
}

void LoRaPHY::get_rx_window_params(float t_symbol, uint8_t min_rx_symbols,
                                   float rx_error, float wakeup_time,
                                   uint32_t *window_length, uint32_t *window_length_ms,
                                   int32_t *window_offset,
                                   uint8_t phy_dr)
{
}

int8_t LoRaPHY::compute_tx_power(int8_t tx_power_idx, float max_eirp,
                                 float antenna_gain)
{
    return LoRaPHY_stub::int8_value;
}


int8_t LoRaPHY::get_next_lower_dr(int8_t dr, int8_t min_dr)
{
    return LoRaPHY_stub::int8_value;
}

uint8_t LoRaPHY::get_bandwidth(uint8_t dr)
{
    return LoRaPHY_stub::uint8_value;
}

uint8_t LoRaPHY::enabled_channel_count(uint8_t datarate,
                                       const uint16_t *channel_mask,
                                       uint8_t *channel_indices,
                                       uint8_t *delayTx)
{
    return LoRaPHY_stub::uint8_value;
}

bool LoRaPHY::is_datarate_supported(const int8_t datarate) const
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

void LoRaPHY::reset_to_default_values(loramac_protocol_params *params, bool init)
{
}

int8_t LoRaPHY::get_next_lower_tx_datarate(int8_t datarate)
{
    return LoRaPHY_stub::int8_value;
}

uint8_t LoRaPHY::get_minimum_rx_datarate()
{
    return LoRaPHY_stub::uint8_value;
}

uint8_t LoRaPHY::get_minimum_tx_datarate()
{
    return LoRaPHY_stub::uint8_value;
}

uint8_t LoRaPHY::get_default_tx_datarate()
{
    return LoRaPHY_stub::uint8_value;
}

uint8_t  LoRaPHY::get_default_max_tx_datarate()
{
    return LoRaPHY_stub::uint8_value;
}

uint8_t LoRaPHY::get_default_tx_power()
{
    return LoRaPHY_stub::uint8_value;
}

uint8_t LoRaPHY::get_max_payload(uint8_t datarate, bool use_repeater)
{
    return LoRaPHY_stub::uint8_value;
}

uint16_t LoRaPHY::get_maximum_frame_counter_gap()
{
    return LoRaPHY_stub::uint16_value;
}

uint32_t LoRaPHY::get_ack_timeout()
{
    return LoRaPHY_stub::uint32_value;
}

uint32_t LoRaPHY::get_default_rx2_frequency()
{
    return LoRaPHY_stub::uint32_value;
}

uint8_t LoRaPHY::get_default_rx2_datarate()
{
    return phy_params.rx_window2_datarate;
}

uint16_t *LoRaPHY::get_channel_mask(bool get_default)
{
    return &LoRaPHY_stub::uint16_value;
}

uint8_t LoRaPHY::get_max_nb_channels()
{
    return LoRaPHY_stub::uint8_value;
}

channel_params_t *LoRaPHY::get_phy_channels()
{
    return LoRaPHY_stub::channel_params_ptr;
}

bool LoRaPHY::is_custom_channel_plan_supported()
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

void LoRaPHY::restore_default_channels()
{
}

bool LoRaPHY::verify_rx_datarate(uint8_t datarate)
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

bool LoRaPHY::verify_tx_datarate(uint8_t datarate, bool use_default)
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

bool LoRaPHY::verify_tx_power(uint8_t tx_power)
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

bool LoRaPHY::verify_duty_cycle(bool cycle)
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

bool LoRaPHY::verify_nb_join_trials(uint8_t nb_join_trials)
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

void LoRaPHY::apply_cf_list(const uint8_t *payload, uint8_t size)
{
}


bool LoRaPHY::get_next_ADR(bool restore_channel_mask, int8_t &dr_out,
                           int8_t &tx_power_out, uint32_t &adr_ack_cnt)
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

void LoRaPHY::compute_rx_win_params(int8_t datarate, uint8_t min_rx_symbols,
                                    uint32_t rx_error,
                                    rx_config_params_t *rx_conf_params)
{
}

bool LoRaPHY::rx_config(rx_config_params_t *rx_conf)
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

bool LoRaPHY::tx_config(tx_config_params_t *tx_conf, int8_t *tx_power,
                        lorawan_time_t *tx_toa)
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

uint8_t LoRaPHY::link_ADR_request(adr_req_params_t *link_adr_req,
                                  int8_t *dr_out, int8_t *tx_power_out,
                                  uint8_t *nb_rep_out, uint8_t *nb_bytes_processed)
{
    *nb_bytes_processed = LoRaPHY_stub::linkAdrNbBytesParsed;
    return LoRaPHY_stub::uint8_value;
}

uint8_t LoRaPHY::accept_rx_param_setup_req(rx_param_setup_req_t *params)
{
    return LoRaPHY_stub::uint8_value;
}

bool LoRaPHY::accept_tx_param_setup_req(uint8_t ul_dwell_time, uint8_t dl_dwell_time)
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

int LoRaPHY::lookup_band_for_frequency(uint32_t freq) const
{
    return LoRaPHY_stub::int_value;
}

bool LoRaPHY::verify_frequency_for_band(uint32_t freq, uint8_t band) const
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

uint8_t LoRaPHY::dl_channel_request(uint8_t channel_id, uint32_t rx1_frequency)
{
    return LoRaPHY_stub::uint8_value;
}

int8_t LoRaPHY::get_alternate_DR(uint8_t nb_trials)
{
    return LoRaPHY_stub::int8_value;
}

void LoRaPHY::calculate_backoff(bool joined, bool last_tx_was_join_req, bool dc_enabled, uint8_t channel,
                                lorawan_time_t elapsed_time, lorawan_time_t tx_toa)
{
}

lorawan_status_t LoRaPHY::set_next_channel(channel_selection_params_t *params,
                                           uint8_t *channel, lorawan_time_t *time,
                                           lorawan_time_t *aggregate_timeoff)
{
    *time = 100;
    return LoRaPHY_stub::lorawan_status_value;
}

lorawan_status_t LoRaPHY::add_channel(const channel_params_t *new_channel,
                                      uint8_t id)
{
    return LoRaPHY_stub::lorawan_status_value;
}

bool LoRaPHY::remove_channel(uint8_t channel_id)
{
    return LoRaPHY_stub::bool_table[LoRaPHY_stub::bool_counter++];
}

void LoRaPHY::set_tx_cont_mode(cw_mode_params_t *params, uint32_t given_frequency)
{
}

uint8_t LoRaPHY::apply_DR_offset(int8_t dr, int8_t dr_offset)
{
    return LoRaPHY_stub::uint8_value;
}

uint32_t LoRaPHY::get_rx_time_on_air(uint8_t modem, uint16_t pkt_len)
{
    return LoRaPHY_stub::uint32_value;
}

