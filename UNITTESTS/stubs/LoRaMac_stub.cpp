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

#include <stdlib.h>
#include "LoRaMac.h"


using namespace events;
using namespace mbed;


LoRaMac::LoRaMac()
    : _lora_time(),
      _lora_phy(NULL),
      _mac_commands(),
      _channel_plan(),
      _lora_crypto(),
      _ev_queue(NULL),
      _mcps_indication(),
      _mcps_confirmation(),
      _mlme_indication(),
      _mlme_confirmation(),
      _is_nwk_joined(false),
      _continuous_rx2_window_open(false),
      _device_class(CLASS_A)
{}

LoRaMac::~LoRaMac()
{
}

const loramac_mcps_confirm_t *LoRaMac::get_mcps_confirmation() const
{
    return NULL;
}

const loramac_mcps_indication_t *LoRaMac::get_mcps_indication() const
{
    return NULL;
}

const loramac_mlme_confirm_t *LoRaMac::get_mlme_confirmation() const
{
    return NULL;
}

const loramac_mlme_indication_t *LoRaMac::get_mlme_indication() const
{
    return NULL;
}

void LoRaMac::post_process_mlme_request()
{
}

void LoRaMac::post_process_mcps_req()
{
}

void LoRaMac::post_process_mcps_ind()
{
}

void LoRaMac::post_process_mlme_ind()
{
}

lorawan_time_t LoRaMac::get_current_time(void)
{
}

rx_slot_t LoRaMac::get_current_slot(void)
{
    return RX_SLOT_WIN_1;
}

void LoRaMac::handle_join_accept_frame(const uint8_t *payload, uint16_t size)
{
}

void LoRaMac::check_frame_size(uint16_t size)
{
}

bool LoRaMac::message_integrity_check(const uint8_t *const payload,
                                      const uint16_t size,
                                      uint8_t *const ptr_pos,
                                      uint32_t address,
                                      uint32_t *downlink_counter,
                                      const uint8_t *nwk_skey)
{
    return true;
}

void LoRaMac::extract_data_and_mac_commands(const uint8_t *payload,
                                            uint16_t size,
                                            uint8_t fopts_len,
                                            uint8_t *nwk_skey,
                                            uint8_t *app_skey,
                                            uint32_t address,
                                            uint32_t downlink_counter,
                                            int16_t rssi,
                                            int8_t snr)
{
}

void LoRaMac::extract_mac_commands_only(const uint8_t *payload,
                                        int8_t snr,
                                        uint8_t fopts_len)
{
}

void LoRaMac::handle_data_frame(const uint8_t *const payload,
                                const uint16_t size,
                                uint8_t ptr_pos,
                                uint8_t msg_type,
                                int16_t rssi,
                                int8_t snr)
{
}

void LoRaMac::set_batterylevel_callback(mbed::Callback<uint8_t(void)> battery_level)
{
}

void LoRaMac::on_radio_tx_done(lorawan_time_t timestamp)
{
}

void LoRaMac::on_radio_rx_done(const uint8_t *const payload, uint16_t size,
                               int16_t rssi, int8_t snr)
{
}

void LoRaMac::on_radio_tx_timeout(void)
{
}

void LoRaMac::on_radio_rx_timeout(bool is_timeout)
{
}

bool LoRaMac::continue_joining_process()
{
    return true;
}

bool LoRaMac::continue_sending_process()
{
    return true;
}

lorawan_status_t LoRaMac::send_join_request()
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMac::handle_retransmission()
{
    return LORAWAN_STATUS_OK;
}

void LoRaMac::on_backoff_timer_expiry(void)
{
}

void LoRaMac::open_rx1_window(void)
{
}

void LoRaMac::open_rx2_window()
{
}

void LoRaMac::on_ack_timeout_timer_event(void)
{
}

bool LoRaMac::validate_payload_length(uint16_t length,
                                      int8_t datarate,
                                      uint8_t fopts_len)
{
    return false;
}

void LoRaMac::set_mlme_schedule_ul_indication(void)
{
}

// This is not actual transmission. It just schedules a message in response
// to MCPS request
lorawan_status_t LoRaMac::send(loramac_mhdr_t *machdr, const uint8_t fport,
                               const void *fbuffer, uint16_t fbuffer_size)
{
    return LORAWAN_STATUS_OK;
}

int LoRaMac::get_backoff_timer_event_id(void)
{
    return 0;
}

lorawan_status_t LoRaMac::clear_tx_pipe(void)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMac::schedule_tx()
{
    return LORAWAN_STATUS_OK;
}

void LoRaMac::calculate_backOff(uint8_t channel)
{
}

void LoRaMac::reset_mac_parameters(void)
{
}

uint8_t LoRaMac::get_default_tx_datarate()
{
    return 0;
}

void LoRaMac::enable_adaptive_datarate(bool adr_enabled)
{
}

lorawan_status_t LoRaMac::set_channel_data_rate(uint8_t data_rate)
{
    return LORAWAN_STATUS_OK;
}

bool LoRaMac::tx_ongoing()
{
    return false;
}

void LoRaMac::set_tx_ongoing(bool ongoing)
{
}

void LoRaMac::reset_ongoing_tx(bool reset_pending)
{
}

int16_t LoRaMac::prepare_ongoing_tx(const uint8_t port,
                                    const uint8_t *const data,
                                    uint16_t length,
                                    uint8_t flags,
                                    uint8_t num_retries)
{
    return 0;
}

lorawan_status_t LoRaMac::send_ongoing_tx()
{
    return LORAWAN_STATUS_OK;
}

device_class_t LoRaMac::get_device_class() const
{
    return CLASS_A;
}

void LoRaMac::set_device_class(const device_class_t &device_class,
                               mbed::Callback<void(void)>ack_expiry_handler)
{
}

void LoRaMac::setup_link_check_request()
{
}

lorawan_status_t LoRaMac::prepare_join(const lorawan_connect_t *params, bool is_otaa)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMac::join(bool is_otaa)
{
    return LORAWAN_STATUS_OK;
}

static void memcpy_convert_endianess(uint8_t *dst,
                                     const uint8_t *src,
                                     uint16_t size)
{
}

lorawan_status_t LoRaMac::prepare_frame(loramac_mhdr_t *machdr,
                                        loramac_frame_ctrl_t *fctrl,
                                        const uint8_t fport,
                                        const void *fbuffer,
                                        uint16_t fbuffer_size)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMac::send_frame_on_channel(uint8_t channel)
{
    return LORAWAN_STATUS_OK;
}

void LoRaMac::reset_mcps_confirmation()
{
}

void LoRaMac::reset_mlme_confirmation()
{
}

void LoRaMac::reset_mcps_indication()
{
}

void LoRaMac::set_tx_continuous_wave(uint8_t channel, int8_t datarate, int8_t tx_power,
                                     float max_eirp, float antenna_gain, uint16_t timeout)
{
}

lorawan_status_t LoRaMac::initialize(EventQueue *queue,
                                     mbed::Callback<void(void)>scheduling_failure_handler)
{
    return LORAWAN_STATUS_OK;
}

void LoRaMac::disconnect()
{
}

uint8_t LoRaMac::get_max_possible_tx_size(uint8_t fopts_len)
{
    return 0;
}

bool LoRaMac::nwk_joined()
{
    return false;
}

void LoRaMac::set_nwk_joined(bool joined)
{
}

lorawan_status_t LoRaMac::add_channel_plan(const lorawan_channelplan_t &plan)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMac::remove_channel_plan()
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMac::get_channel_plan(lorawan_channelplan_t &plan)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMac::remove_single_channel(uint8_t id)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMac::multicast_channel_link(multicast_params_t *channel_param)
{
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMac::multicast_channel_unlink(multicast_params_t *channel_param)
{
    return LORAWAN_STATUS_OK;
}

void LoRaMac::bind_phy(LoRaPHY &phy)
{
}
