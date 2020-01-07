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

Description: LoRa MAC layer implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis ( Semtech ), Gregory Cristian ( Semtech ) and Daniel Jaeckle ( STACKFORCE )

Copyright (c) 2017, Arm Limited and affiliates.

SPDX-License-Identifier: BSD-3-Clause
*/

#include "LoRaMacCommand.h"
#include "LoRaMac.h"
#include "mbed-trace/mbed_trace.h"
#define TRACE_GROUP "LMACC"

using namespace mbed;

/**
 * LoRaMAC max EIRP (dBm) table.
 */
static const uint8_t max_eirp_table[] = { 8, 10, 12, 13, 14, 16, 18, 20, 21, 24, 26, 27, 29, 30, 33, 36 };

LoRaMacCommand::LoRaMacCommand()
{
    sticky_mac_cmd = false;
    mac_cmd_buf_idx = 0;
    mac_cmd_buf_idx_to_repeat = 0;

    memset(mac_cmd_buffer, 0, sizeof(mac_cmd_buffer));
    memset(mac_cmd_buffer_to_repeat, 0, sizeof(mac_cmd_buffer_to_repeat));
}

void LoRaMacCommand::clear_command_buffer()
{
    mac_cmd_buf_idx = 0;
}

uint8_t LoRaMacCommand::get_mac_cmd_length() const
{
    return mac_cmd_buf_idx;
}

uint8_t *LoRaMacCommand::get_mac_commands_buffer()
{
    return mac_cmd_buffer;
}

void LoRaMacCommand::parse_mac_commands_to_repeat()
{
    uint8_t i = 0;
    uint8_t cmd_cnt = 0;

    for (i = 0; i < mac_cmd_buf_idx; i++) {
        switch (mac_cmd_buffer[i]) {
            // STICKY
            case MOTE_MAC_RESET_IND:
            case MOTE_MAC_RX_PARAM_SETUP_ANS:
            case MOTE_MAC_DL_CHANNEL_ANS:
            case MOTE_MAC_REKEY_IND: { // 1 byte payload
                mac_cmd_buffer_to_repeat[cmd_cnt++] = mac_cmd_buffer[i++];
                mac_cmd_buffer_to_repeat[cmd_cnt++] = mac_cmd_buffer[i];
                break;
            }
            case MOTE_MAC_RX_TIMING_SETUP_ANS: { // 0 byte payload
                mac_cmd_buffer_to_repeat[cmd_cnt++] = mac_cmd_buffer[i];
                break;
            }
            // NON-STICKY
            case MOTE_MAC_DEV_STATUS_ANS: { // 2 bytes payload
                i += 2;
                break;
            }
            case MOTE_MAC_LINK_ADR_ANS:
            case MOTE_MAC_NEW_CHANNEL_ANS:
            case MOTE_MAC_REJOIN_PARAM_SETUP_ANS:
            case MOTE_MAC_PING_SLOT_INFO_REQ:
            case MOTE_MAC_PING_SLOT_CHANNEL_ANS: { // 1 byte payload
                i++;
                break;
            }
            case MOTE_MAC_LINK_CHECK_REQ:
            case MOTE_MAC_DUTY_CYCLE_ANS:
            case MOTE_MAC_TX_PARAM_SETUP_ANS:
            case MOTE_MAC_ADR_PARAM_SETUP_ANS:
            case MOTE_MAC_DEVICE_TIME_REQ: { // 0 byte payload
                break;
            }
            default: {
                MBED_ASSERT(false);
            }
        }
    }

    mac_cmd_buf_idx_to_repeat = cmd_cnt;
}

void LoRaMacCommand::clear_repeat_buffer()
{
    mac_cmd_buf_idx_to_repeat = 0;
}

void LoRaMacCommand::copy_repeat_commands_to_buffer()
{
    memcpy(&mac_cmd_buffer[mac_cmd_buf_idx], mac_cmd_buffer_to_repeat, mac_cmd_buf_idx_to_repeat);
    mac_cmd_buf_idx += mac_cmd_buf_idx_to_repeat;
}

uint8_t LoRaMacCommand::get_repeat_commands_length() const
{
    return mac_cmd_buf_idx_to_repeat;
}

void LoRaMacCommand::clear_sticky_mac_cmd()
{
    sticky_mac_cmd = false;
}

bool LoRaMacCommand::has_sticky_mac_cmd() const
{
    return sticky_mac_cmd;
}

lorawan_status_t LoRaMacCommand::process_mac_commands(const uint8_t *payload, uint8_t mac_index,
                                                      uint8_t commands_size, uint8_t snr,
                                                      lora_mac_system_params_t &mac_sys_params,
                                                      LoRaPHY &lora_phy,
                                                      Callback<void(loramac_mlme_confirm_t &)> confirm_handler,
                                                      rx_slot_t rx_slot)
{
    uint8_t status = 0;
    lorawan_status_t ret_value = LORAWAN_STATUS_OK;

    while (mac_index < commands_size) {
        // Decode Frame MAC commands
        switch (payload[mac_index++]) {
            case SRV_MAC_RESET_CONF: {
                loramac_mlme_confirm_t mlme_conf;
                mlme_conf.type = MLME_RESET;
                mlme_conf.status = LORAMAC_EVENT_INFO_STATUS_OK;
                mlme_conf.version = payload[mac_index++] & 0x0F;
                confirm_handler(mlme_conf);
            }
            break;
            case SRV_MAC_LINK_CHECK_ANS: {
                loramac_mlme_confirm_t mlme_conf;
                mlme_conf.type = MLME_LINK_CHECK;
                mlme_conf.status = LORAMAC_EVENT_INFO_STATUS_OK;
                mlme_conf.demod_margin = payload[mac_index++];
                mlme_conf.nb_gateways = payload[mac_index++];
                confirm_handler(mlme_conf);
            }
            break;
            case SRV_MAC_LINK_ADR_REQ: {
                adr_req_params_t link_adr_req;
                int8_t link_adr_dr = DR_0;
                int8_t link_adr_txpower = TX_POWER_0;
                uint8_t link_adr_nbtrans = 0;
                uint8_t link_adr_nb_bytes_parsed = 0;

                // Fill parameter structure
                link_adr_req.payload = &payload[mac_index - 1];
                link_adr_req.payload_size = commands_size - (mac_index - 1);
                link_adr_req.adr_enabled = mac_sys_params.adr_on;
                link_adr_req.ul_dwell_time = mac_sys_params.uplink_dwell_time;
                link_adr_req.current_datarate = mac_sys_params.channel_data_rate;
                link_adr_req.current_tx_power = mac_sys_params.channel_tx_power;
                link_adr_req.current_nb_trans = mac_sys_params.nb_trans;

                // Process the ADR requests
                status = lora_phy.link_ADR_request(&link_adr_req,
                                                   &link_adr_dr,
                                                   &link_adr_txpower,
                                                   &link_adr_nbtrans,
                                                   &link_adr_nb_bytes_parsed);

                // If nothing was consumed, we have a malformed packet at our hand
                // we bin everything and return. link_adr_nb_bytes_parsed being 0 is
                // a magic identifier letting us know that there are payload inconsistencies
                if (link_adr_nb_bytes_parsed == 0) {
                    return LORAWAN_STATUS_UNSUPPORTED;
                }

                if ((status & 0x07) == 0x07) {
                    mac_sys_params.channel_data_rate = link_adr_dr;
                    mac_sys_params.channel_tx_power = link_adr_txpower;
                    mac_sys_params.nb_trans = link_adr_nbtrans;
                }

                // Add the answers to the buffer
                for (uint8_t i = 0; i < (link_adr_nb_bytes_parsed / 5); i++) {
                    ret_value = add_link_adr_ans(status);
                }
                // Update MAC index
                mac_index += link_adr_nb_bytes_parsed - 1;
            }
            break;
            case SRV_MAC_DUTY_CYCLE_REQ:
                mac_sys_params.max_duty_cycle = payload[mac_index++];
                mac_sys_params.aggregated_duty_cycle = 1 << mac_sys_params.max_duty_cycle;
                ret_value = add_duty_cycle_ans();
                break;
            case SRV_MAC_RX_PARAM_SETUP_REQ: {
                rx_param_setup_req_t rxParamSetupReq;

                rxParamSetupReq.dr_offset = (payload[mac_index] >> 4) & 0x07;
                rxParamSetupReq.datarate = payload[mac_index++] & 0x0F;

                rxParamSetupReq.frequency = (uint32_t) payload[mac_index++];
                rxParamSetupReq.frequency |= (uint32_t) payload[mac_index++] << 8;
                rxParamSetupReq.frequency |= (uint32_t) payload[mac_index++] << 16;
                rxParamSetupReq.frequency *= 100;

                // Perform request on region
                status = lora_phy.accept_rx_param_setup_req(&rxParamSetupReq);

                if ((status & 0x07) == 0x07) {
                    mac_sys_params.rx2_channel.datarate = rxParamSetupReq.datarate;
                    mac_sys_params.rx2_channel.frequency = rxParamSetupReq.frequency;
                    mac_sys_params.rx1_dr_offset = rxParamSetupReq.dr_offset;
                }
                ret_value = add_rx_param_setup_ans(status);
            }
            break;
            case SRV_MAC_DEV_STATUS_REQ: {
                uint8_t battery_level = BAT_LEVEL_NO_MEASURE;
                if (_battery_level_cb) {
                    battery_level = _battery_level_cb();
                }
                ret_value = add_dev_status_ans(battery_level, snr & 0x3F);
            }
            break;
            case SRV_MAC_NEW_CHANNEL_REQ: {
                channel_params_t chParam;
                int8_t channel_id = payload[mac_index++];

                chParam.frequency = (uint32_t) payload[mac_index++];
                chParam.frequency |= (uint32_t) payload[mac_index++] << 8;
                chParam.frequency |= (uint32_t) payload[mac_index++] << 16;
                chParam.frequency *= 100;
                chParam.rx1_frequency = 0;
                chParam.dr_range.value = payload[mac_index++];

                status = lora_phy.request_new_channel(channel_id, &chParam);

                ret_value = add_new_channel_ans(status);
            }
            break;
            case SRV_MAC_RX_TIMING_SETUP_REQ: {
                uint8_t delay = payload[mac_index++] & 0x0F;

                if (delay == 0) {
                    delay++;
                }
                mac_sys_params.recv_delay1 = delay * 1000;
                mac_sys_params.recv_delay2 = mac_sys_params.recv_delay1 + 1000;
                ret_value = add_rx_timing_setup_ans();
            }
            break;
            case SRV_MAC_TX_PARAM_SETUP_REQ: {
                uint8_t eirpDwellTime = payload[mac_index++];
                uint8_t ul_dwell_time;
                uint8_t dl_dwell_time;
                uint8_t max_eirp;

                ul_dwell_time = 0;
                dl_dwell_time = 0;

                if ((eirpDwellTime & 0x20) == 0x20) {
                    dl_dwell_time = 1;
                }
                if ((eirpDwellTime & 0x10) == 0x10) {
                    ul_dwell_time = 1;
                }
                max_eirp = eirpDwellTime & 0x0F;

                // Check the status for correctness
                if (lora_phy.accept_tx_param_setup_req(ul_dwell_time, dl_dwell_time)) {
                    // Accept command
                    mac_sys_params.uplink_dwell_time = ul_dwell_time;
                    mac_sys_params.downlink_dwell_time = dl_dwell_time;
                    mac_sys_params.max_eirp = max_eirp_table[max_eirp];
                    // Add command response
                    ret_value = add_tx_param_setup_ans();
                }
            }
            break;
            case SRV_MAC_DL_CHANNEL_REQ: {
                uint8_t channel_id = payload[mac_index++];
                uint32_t rx1_frequency;

                rx1_frequency = (uint32_t) payload[mac_index++];
                rx1_frequency |= (uint32_t) payload[mac_index++] << 8;
                rx1_frequency |= (uint32_t) payload[mac_index++] << 16;
                rx1_frequency *= 100;
                status = lora_phy.dl_channel_request(channel_id, rx1_frequency);

                ret_value = add_dl_channel_ans(status);
            }
            break;
            case SRV_MAC_REKEY_CONF: {
                loramac_mlme_confirm_t mlme_conf;
                mlme_conf.type = MLME_REKEY;
                mlme_conf.status = LORAMAC_EVENT_INFO_STATUS_OK;
                mlme_conf.version = payload[mac_index++] & 0x0F;
                confirm_handler(mlme_conf);
            }
            break;
            case SRV_MAC_ADR_PARAM_SETUP_REQ: {
                uint16_t limit = 1;
                uint16_t delay = 1;
                uint8_t adrParam = payload[mac_index++];
                delay = delay << (adrParam & 0x0f);
                limit = limit << (adrParam >> 4 & 0x0f);
                lora_phy.set_adr_ack_limit(limit);
                lora_phy.set_adr_ack_delay(delay);
                ret_value = add_adr_param_setup_ans();
            }
            break;
            case SRV_MAC_DEVICE_TIME_ANS: {
                lorawan_gps_time_t gps_time;
                gps_time = (uint32_t) payload[mac_index++];
                gps_time |= (uint32_t) payload[mac_index++] << 8;
                gps_time |= (uint32_t) payload[mac_index++] << 16;
                gps_time |= (uint32_t) payload[mac_index++] << 24;
                gps_time = (gps_time * 1000) + (payload[mac_index++] * 1000 >> 8);
                if (_time_sync_cb) {
                    _time_sync_cb(gps_time);
                }
            }
            break;
            case SRV_MAC_FORCE_REJOIN_REQ: {
                loramac_mlme_confirm_t mlme_conf;
                uint8_t data = payload[mac_index++];
                uint8_t max_retries = data & 0x07;
                uint8_t period = (data >> 3) & 0x07;
                data = payload[mac_index++];
                uint8_t datarate = data & 0x0F;
                uint8_t rejoin_type = (data >> 4) & 0x07;

                mlme_conf.status = LORAMAC_EVENT_INFO_STATUS_OK;
                mlme_conf.type = MLME_FORCE_REJOIN;
                mlme_conf.max_retries = max_retries;
                mlme_conf.period = period;
                mlme_conf.datarate = datarate;
                mlme_conf.rejoin_type = rejoin_type;
                confirm_handler(mlme_conf);

            }
            break;
            case SRV_MAC_REJOIN_PARAM_SETUP_REQ: {
                uint8_t data = payload[mac_index++];
                uint8_t count = (data & 0x0F) + 4;
                uint8_t time = ((data >> 4) & 0x0F) + 10;

                uint32_t max_count = 1 << count;
                uint32_t max_time = 1 << time;

                uint8_t time_available = lora_phy.update_rejoin_params(max_time, max_count);
                ret_value = add_rejoin_param_setup_ans(time_available);
            }
            break;
            case SRV_MAC_DEVICE_MODE_CONF: {
                loramac_mlme_confirm_t mlme_conf;
                uint8_t classType = payload[mac_index++];
                mlme_conf.classType = classType;
                mlme_conf.status = LORAMAC_EVENT_INFO_STATUS_OK;
                mlme_conf.type = MLME_DEVICE_MODE;
                confirm_handler(mlme_conf);
            }
            break;
            case SRV_MAC_PING_SLOT_INFO_ANS: {
                loramac_mlme_confirm_t mlme_conf;
                mlme_conf.status = LORAMAC_EVENT_INFO_STATUS_OK;
                mlme_conf.type = MLME_PING_SLOT_INFO;
                confirm_handler(mlme_conf);
            }
            break;
            case SRV_MAC_PING_SLOT_CHANNEL_REQ: {
                uint32_t frequency;
                uint8_t  datarate;
                /* This command can only be sent in a class A receive window.
                    * If received inside a class B ping-slot, the command SHALL NOT
                    * be processed (14.3)*/
                if ((rx_slot != RX_SLOT_WIN_UNICAST_PING_SLOT) && (rx_slot != RX_SLOT_WIN_MULTICAST_PING_SLOT)) {
                    frequency = (uint32_t) payload[mac_index++];
                    frequency |= (uint32_t) payload[mac_index++] << 8;
                    frequency |= (uint32_t) payload[mac_index++] << 16;
                    frequency *= 100;
                    datarate = payload[mac_index++];
                    status = lora_phy.accept_ping_slot_channel_req(frequency, datarate);
                    ret_value = add_ping_slot_channel_ans(status);
                }
            }
            break;
            case SRV_MAC_BEACON_FREQ_REQ: {
                uint32_t frequency;

                frequency = (uint32_t) payload[mac_index++];
                frequency |= (uint32_t) payload[mac_index++] << 8;
                frequency |= (uint32_t) payload[mac_index++] << 16;
                frequency *= 100;
                status = lora_phy.accept_beacon_frequency_request(frequency);
                ret_value = add_beacon_freq_ans(status);
            }
            break;
            default:
                // Unknown command. ABORT MAC commands processing
                tr_error("Invalid MAC command (0x%X)!", payload[mac_index]);
                return LORAWAN_STATUS_UNSUPPORTED;
        }
    }
    return ret_value;
}


int32_t LoRaMacCommand::cmd_buffer_remaining() const
{
    // The maximum buffer length must take MAC commands to re-send into account.
    return sizeof(mac_cmd_buffer) - mac_cmd_buf_idx_to_repeat - mac_cmd_buf_idx;
}

void LoRaMacCommand::set_batterylevel_callback(mbed::Callback<uint8_t(void)> battery_level)
{
    _battery_level_cb = battery_level;
}

lorawan_status_t LoRaMacCommand::add_link_check_req()
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;
    if (cmd_buffer_remaining() > 0) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_LINK_CHECK_REQ;
        // No payload for this command
        ret = LORAWAN_STATUS_OK;
    }
    return ret;
}

lorawan_status_t LoRaMacCommand::add_reset_ind(uint8_t version)
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;
    if (cmd_buffer_remaining() > 0) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_RESET_IND;
        mac_cmd_buffer[mac_cmd_buf_idx++] = version & 0x0F;
        ret = LORAWAN_STATUS_OK;
    }
    return ret;
}

lorawan_status_t LoRaMacCommand::add_rekey_ind(uint8_t version)
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;
    if (cmd_buffer_remaining() > 0) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_REKEY_IND;
        mac_cmd_buffer[mac_cmd_buf_idx++] = version & 0x0F;
        // This is a sticky MAC command answer. Setup indication
        sticky_mac_cmd = true;
        ret = LORAWAN_STATUS_OK;
    }
    return ret;
}

lorawan_status_t LoRaMacCommand::add_device_mode_indication(uint8_t classType)
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;
    if (cmd_buffer_remaining() > 0) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_DEVICE_MODE_IND;
        mac_cmd_buffer[mac_cmd_buf_idx++] = classType;
        ret = LORAWAN_STATUS_OK;
    }
    return ret;
}

lorawan_status_t LoRaMacCommand::add_device_time_req(mbed::Callback<void(lorawan_gps_time_t gps_time)> notify)
{
    //App developer must know if the server version is 1.0.3 (or greater)
    //1.0.2 does not support this MAC command and our stack does not support pre 1.0.2 versions
    if (LORAWAN_VERSION_1_0_2 == MBED_CONF_LORA_VERSION) {
        return LORAWAN_STATUS_UNSUPPORTED;
    }

    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;
    if (cmd_buffer_remaining() > 0) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_DEVICE_TIME_REQ;
        ret = LORAWAN_STATUS_OK;
    }

    _time_sync_cb = notify;

    return ret;
}

lorawan_status_t LoRaMacCommand::add_link_adr_ans(uint8_t status)
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;
    if (cmd_buffer_remaining() > 1) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_LINK_ADR_ANS;
        mac_cmd_buffer[mac_cmd_buf_idx++] = status;
        ret = LORAWAN_STATUS_OK;
    }
    return ret;
}

lorawan_status_t LoRaMacCommand::add_duty_cycle_ans()
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;
    if (cmd_buffer_remaining() > 0) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_DUTY_CYCLE_ANS;
        // No payload for this answer
        ret = LORAWAN_STATUS_OK;
    }
    return ret;
}

lorawan_status_t LoRaMacCommand::add_rx_param_setup_ans(uint8_t status)
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;
    if (cmd_buffer_remaining() > 1) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_RX_PARAM_SETUP_ANS;
        // Status: Datarate ACK, Channel ACK
        mac_cmd_buffer[mac_cmd_buf_idx++] = status;
        // This is a sticky MAC command answer. Setup indication
        sticky_mac_cmd = true;
        ret = LORAWAN_STATUS_OK;
    }
    return ret;
}

lorawan_status_t LoRaMacCommand::add_dev_status_ans(uint8_t battery, uint8_t margin)
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;
    if (cmd_buffer_remaining() > 2) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_DEV_STATUS_ANS;
        // 1st byte Battery
        // 2nd byte Margin
        mac_cmd_buffer[mac_cmd_buf_idx++] = battery;
        mac_cmd_buffer[mac_cmd_buf_idx++] = margin;
        ret = LORAWAN_STATUS_OK;
    }
    return ret;
}

lorawan_status_t LoRaMacCommand::add_new_channel_ans(uint8_t status)
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;
    if (cmd_buffer_remaining() > 1) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_NEW_CHANNEL_ANS;
        // Status: Datarate range OK, Channel frequency OK
        mac_cmd_buffer[mac_cmd_buf_idx++] = status;
        ret = LORAWAN_STATUS_OK;
    }
    return ret;
}

lorawan_status_t LoRaMacCommand::add_rx_timing_setup_ans()
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;
    if (cmd_buffer_remaining() > 0) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_RX_TIMING_SETUP_ANS;
        // No payload for this answer
        // This is a sticky MAC command answer. Setup indication
        sticky_mac_cmd = true;
        ret = LORAWAN_STATUS_OK;
    }
    return ret;
}

lorawan_status_t LoRaMacCommand::add_tx_param_setup_ans()
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;
    if (cmd_buffer_remaining() > 0) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_TX_PARAM_SETUP_ANS;
        // No payload for this answer
        ret = LORAWAN_STATUS_OK;
    }
    return ret;
}

lorawan_status_t LoRaMacCommand::add_dl_channel_ans(uint8_t status)
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;
    if (cmd_buffer_remaining() > 1) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_DL_CHANNEL_ANS;
        // Status: Uplink frequency exists, Channel frequency OK
        mac_cmd_buffer[mac_cmd_buf_idx++] = status;
        // This is a sticky MAC command answer. Setup indication
        sticky_mac_cmd = true;
        ret = LORAWAN_STATUS_OK;
    }
    return ret;
}

lorawan_status_t LoRaMacCommand::add_adr_param_setup_ans()
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;
    if (cmd_buffer_remaining() > 0) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_ADR_PARAM_SETUP_ANS;
        ret = LORAWAN_STATUS_OK;
    }
    return ret;
}

lorawan_status_t LoRaMacCommand::add_rejoin_param_setup_ans(uint8_t status)
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;
    if (cmd_buffer_remaining() > 1) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_REJOIN_PARAM_SETUP_ANS;
        mac_cmd_buffer[mac_cmd_buf_idx++] = status & 0x01;
        ret = LORAWAN_STATUS_OK;
    }
    return ret;
}

lorawan_status_t LoRaMacCommand::add_ping_slot_info_req(uint8_t periodicity)
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;

    if ((periodicity & 0x07) == periodicity) {
        if (cmd_buffer_remaining() > 1) {
            mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_PING_SLOT_INFO_REQ;
            mac_cmd_buffer[mac_cmd_buf_idx++] = periodicity;
            ret = LORAWAN_STATUS_OK;
        }
    } else {
        ret = LORAWAN_STATUS_PARAMETER_INVALID;
    }
    return ret;
}

lorawan_status_t LoRaMacCommand::add_ping_slot_channel_ans(uint8_t status)
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;
    if (cmd_buffer_remaining() > 1) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_PING_SLOT_CHANNEL_ANS;
        mac_cmd_buffer[mac_cmd_buf_idx++] = status & 0x03;
        ret = LORAWAN_STATUS_OK;
    }
    return ret;
}

lorawan_status_t LoRaMacCommand::add_beacon_freq_ans(uint8_t status)
{
    lorawan_status_t ret = LORAWAN_STATUS_LENGTH_ERROR;

    if (cmd_buffer_remaining() > 1) {
        mac_cmd_buffer[mac_cmd_buf_idx++] = MOTE_MAC_BEACON_FREQ_ANS;
        mac_cmd_buffer[mac_cmd_buf_idx++] = status & 0x01;
        ret = LORAWAN_STATUS_OK;
    }
    return ret;
}
