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

#if defined(FEATURE_COMMON_PAL)
#include "mbed_trace.h"
#define TRACE_GROUP "LMACC"
#else
#define tr_debug(...) (void(0)) //dummies if feature common pal is not added
#define tr_info(...)  (void(0)) //dummies if feature common pal is not added
#define tr_error(...) (void(0)) //dummies if feature common pal is not added
#endif //defined(FEATURE_COMMON_PAL)

/**
 * LoRaMAC max EIRP (dBm) table.
 */
static const uint8_t max_eirp_table[] = { 8, 10, 12, 13, 14, 16, 18, 20, 21, 24, 26, 27, 29, 30, 33, 36 };


LoRaMacCommand::LoRaMacCommand(LoRaMac& lora_mac)
    : _lora_mac(lora_mac)
{
    mac_cmd_in_next_tx = false;
    mac_cmd_buf_idx = 0;
    mac_cmd_buf_idx_to_repeat = 0;

    memset(mac_cmd_buffer, 0, sizeof(mac_cmd_buffer));
    memset(mac_cmd_buffer_to_repeat, 0, sizeof(mac_cmd_buffer_to_repeat));
}

LoRaMacCommand::~LoRaMacCommand()
{
}

lorawan_status_t LoRaMacCommand::add_mac_command(uint8_t cmd, uint8_t p1,
                                                 uint8_t p2)
{
    lorawan_status_t status = LORAWAN_STATUS_BUSY;
    // The maximum buffer length must take MAC commands to re-send into account.
    const uint8_t bufLen = LORA_MAC_COMMAND_MAX_LENGTH
            - mac_cmd_buf_idx_to_repeat;

    switch (cmd) {
        case MOTE_MAC_LINK_CHECK_REQ:
            if (mac_cmd_buf_idx < bufLen) {
                mac_cmd_buffer[mac_cmd_buf_idx++] = cmd;
                // No payload for this command
                status = LORAWAN_STATUS_OK;
            }
            break;
        case MOTE_MAC_LINK_ADR_ANS:
            if (mac_cmd_buf_idx < (bufLen - 1)) {
                mac_cmd_buffer[mac_cmd_buf_idx++] = cmd;
                // Margin
                mac_cmd_buffer[mac_cmd_buf_idx++] = p1;
                status = LORAWAN_STATUS_OK;
            }
            break;
        case MOTE_MAC_DUTY_CYCLE_ANS:
            if (mac_cmd_buf_idx < bufLen) {
                mac_cmd_buffer[mac_cmd_buf_idx++] = cmd;
                // No payload for this answer
                status = LORAWAN_STATUS_OK;
            }
            break;
        case MOTE_MAC_RX_PARAM_SETUP_ANS:
            if (mac_cmd_buf_idx < (bufLen - 1)) {
                mac_cmd_buffer[mac_cmd_buf_idx++] = cmd;
                // Status: Datarate ACK, Channel ACK
                mac_cmd_buffer[mac_cmd_buf_idx++] = p1;
                // This is a sticky MAC command answer. Setup indication
                _lora_mac.set_mlme_schedule_ul_indication();
                status = LORAWAN_STATUS_OK;
            }
            break;
        case MOTE_MAC_DEV_STATUS_ANS:
            if (mac_cmd_buf_idx < (bufLen - 2)) {
                mac_cmd_buffer[mac_cmd_buf_idx++] = cmd;
                // 1st byte Battery
                // 2nd byte Margin
                mac_cmd_buffer[mac_cmd_buf_idx++] = p1;
                mac_cmd_buffer[mac_cmd_buf_idx++] = p2;
                status = LORAWAN_STATUS_OK;
            }
            break;
        case MOTE_MAC_NEW_CHANNEL_ANS:
            if (mac_cmd_buf_idx < (bufLen - 1)) {
                mac_cmd_buffer[mac_cmd_buf_idx++] = cmd;
                // Status: Datarate range OK, Channel frequency OK
                mac_cmd_buffer[mac_cmd_buf_idx++] = p1;
                status = LORAWAN_STATUS_OK;
            }
            break;
        case MOTE_MAC_RX_TIMING_SETUP_ANS:
            if (mac_cmd_buf_idx < bufLen) {
                mac_cmd_buffer[mac_cmd_buf_idx++] = cmd;
                // No payload for this answer
                // This is a sticky MAC command answer. Setup indication
                _lora_mac.set_mlme_schedule_ul_indication();
                status = LORAWAN_STATUS_OK;
            }
            break;
        case MOTE_MAC_TX_PARAM_SETUP_ANS:
            if (mac_cmd_buf_idx < bufLen) {
                mac_cmd_buffer[mac_cmd_buf_idx++] = cmd;
                // No payload for this answer
                status = LORAWAN_STATUS_OK;
            }
            break;
        case MOTE_MAC_DL_CHANNEL_ANS:
            if (mac_cmd_buf_idx < bufLen) {
                mac_cmd_buffer[mac_cmd_buf_idx++] = cmd;
                // Status: Uplink frequency exists, Channel frequency OK
                mac_cmd_buffer[mac_cmd_buf_idx++] = p1;
                // This is a sticky MAC command answer. Setup indication
                _lora_mac.set_mlme_schedule_ul_indication();
                status = LORAWAN_STATUS_OK;
            }
            break;
        default:
            return LORAWAN_STATUS_SERVICE_UNKNOWN;
    }
    if (status == LORAWAN_STATUS_OK) {
        mac_cmd_in_next_tx = true;
    }
    return status;
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
            case MOTE_MAC_DL_CHANNEL_ANS:
            case MOTE_MAC_RX_PARAM_SETUP_ANS: { // 1 byte payload
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
            case MOTE_MAC_NEW_CHANNEL_ANS: { // 1 byte payload
                i++;
                break;
            }
            case MOTE_MAC_TX_PARAM_SETUP_ANS:
            case MOTE_MAC_DUTY_CYCLE_ANS:
            case MOTE_MAC_LINK_CHECK_REQ: { // 0 byte payload
                break;
            }
            default:
                break;
        }
    }

    if (cmd_cnt > 0) {
        mac_cmd_in_next_tx = true;
    } else {
        mac_cmd_in_next_tx = false;
    }
}


void LoRaMacCommand::clear_repeat_buffer()
{
    mac_cmd_buf_idx_to_repeat = 0;
}

void LoRaMacCommand::copy_repeat_commands_to_buffer()
{
    // Copy the MAC commands which must be re-send into the MAC command buffer
    memcpy(&mac_cmd_buffer[mac_cmd_buf_idx], mac_cmd_buffer_to_repeat, mac_cmd_buf_idx_to_repeat);
    mac_cmd_buf_idx += mac_cmd_buf_idx_to_repeat;
}

uint8_t LoRaMacCommand::get_repeat_commands_length() const
{
    return mac_cmd_buf_idx_to_repeat;
}

void LoRaMacCommand::clear_mac_commands_in_next_tx()
{
    mac_cmd_in_next_tx = false;
}

bool LoRaMacCommand::is_mac_command_in_next_tx() const
{
    return mac_cmd_in_next_tx;
}

lorawan_status_t LoRaMacCommand::process_mac_commands(uint8_t *payload,
                                                      uint8_t mac_index,
                                                      uint8_t commands_size,
                                                      uint8_t snr,
                                                      loramac_mlme_confirm_t& mlme_conf,
                                                      lora_mac_system_params_t &mac_sys_params,
                                                      LoRaPHY &lora_phy)
{
    uint8_t status = 0;
    lorawan_status_t ret_value = LORAWAN_STATUS_OK;

    while (mac_index < commands_size) {
        // Decode Frame MAC commands
        switch (payload[mac_index++]) {
            case SRV_MAC_LINK_CHECK_ANS:
                mlme_conf.status = LORAMAC_EVENT_INFO_STATUS_OK;
                mlme_conf.demod_margin = payload[mac_index++];
                mlme_conf.nb_gateways = payload[mac_index++];
                break;
            case SRV_MAC_LINK_ADR_REQ: {
                adr_req_params_t linkAdrReq;
                int8_t linkAdrDatarate = DR_0;
                int8_t linkAdrTxPower = TX_POWER_0;
                uint8_t linkAdrNbRep = 0;
                uint8_t linkAdrNbBytesParsed = 0;

                // Fill parameter structure
                linkAdrReq.payload = &payload[mac_index - 1];
                linkAdrReq.payload_size = commands_size - (mac_index - 1);
                linkAdrReq.adr_enabled = mac_sys_params.adr_on;
                linkAdrReq.ul_dwell_time = mac_sys_params.uplink_dwell_time;
                linkAdrReq.current_datarate = mac_sys_params.channel_data_rate;
                linkAdrReq.current_tx_power = mac_sys_params.channel_tx_power;
                linkAdrReq.current_nb_rep = mac_sys_params.retry_num;

                // Process the ADR requests
                status = lora_phy.link_ADR_request(&linkAdrReq,
                                                   &linkAdrDatarate,
                                                   &linkAdrTxPower,
                                                   &linkAdrNbRep,
                                                   &linkAdrNbBytesParsed);

                if ((status & 0x07) == 0x07) {
                    mac_sys_params.channel_data_rate = linkAdrDatarate;
                    mac_sys_params.channel_tx_power = linkAdrTxPower;
                    mac_sys_params.retry_num = linkAdrNbRep;
                }

                // Add the answers to the buffer
                for (uint8_t i = 0; i < (linkAdrNbBytesParsed / 5); i++) {
                    ret_value = add_mac_command(MOTE_MAC_LINK_ADR_ANS, status, 0);
                }
                // Update MAC index
                mac_index += linkAdrNbBytesParsed - 1;
            }
                break;
            case SRV_MAC_DUTY_CYCLE_REQ:
                mac_sys_params.max_duty_cycle = payload[mac_index++];
                mac_sys_params.aggregated_duty_cycle = 1 << mac_sys_params.max_duty_cycle;
                ret_value = add_mac_command(MOTE_MAC_DUTY_CYCLE_ANS, 0, 0);
                break;
            case SRV_MAC_RX_PARAM_SETUP_REQ: {
                rx_param_setup_req_t rxParamSetupReq;

                rxParamSetupReq.dr_offset = (payload[mac_index] >> 4) & 0x07;
                rxParamSetupReq.datarate = payload[mac_index] & 0x0F;
                mac_index++;

                rxParamSetupReq.frequency = (uint32_t) payload[mac_index++];
                rxParamSetupReq.frequency |= (uint32_t) payload[mac_index++]
                        << 8;
                rxParamSetupReq.frequency |= (uint32_t) payload[mac_index++]
                        << 16;
                rxParamSetupReq.frequency *= 100;

                // Perform request on region
                status = lora_phy.accept_rx_param_setup_req(&rxParamSetupReq);

                if ((status & 0x07) == 0x07) {
                    mac_sys_params.rx2_channel.datarate =
                            rxParamSetupReq.datarate;
                    mac_sys_params.rx2_channel.frequency =
                            rxParamSetupReq.frequency;
                    mac_sys_params.rx1_dr_offset = rxParamSetupReq.dr_offset;
                }
                ret_value = add_mac_command(MOTE_MAC_RX_PARAM_SETUP_ANS, status,
                                            0);
            }
                break;
            case SRV_MAC_DEV_STATUS_REQ: {
                uint8_t batteryLevel = BAT_LEVEL_NO_MEASURE;
                // we don't have a mechanism at the moment to measure
                // battery levels
                ret_value = add_mac_command(MOTE_MAC_DEV_STATUS_ANS,
                                            batteryLevel, snr);
                break;
            }
            case SRV_MAC_NEW_CHANNEL_REQ: {
                new_channel_req_params_t newChannelReq;
                channel_params_t chParam;

                newChannelReq.channel_id = payload[mac_index++];
                newChannelReq.new_channel = &chParam;

                chParam.frequency = (uint32_t) payload[mac_index++];
                chParam.frequency |= (uint32_t) payload[mac_index++] << 8;
                chParam.frequency |= (uint32_t) payload[mac_index++] << 16;
                chParam.frequency *= 100;
                chParam.rx1_frequency = 0;
                chParam.dr_range.value = payload[mac_index++];

                status = lora_phy.request_new_channel(&newChannelReq);

                ret_value = add_mac_command(MOTE_MAC_NEW_CHANNEL_ANS, status, 0);
            }
                break;
            case SRV_MAC_RX_TIMING_SETUP_REQ: {
                uint8_t delay = payload[mac_index++] & 0x0F;

                if (delay == 0) {
                    delay++;
                }
                mac_sys_params.recv_delay1 = delay * 1000;
                mac_sys_params.recv_delay2 = mac_sys_params.recv_delay1 + 1000;
                ret_value = add_mac_command(MOTE_MAC_RX_TIMING_SETUP_ANS, 0, 0);
            }
                break;
            case SRV_MAC_TX_PARAM_SETUP_REQ: {
                tx_param_setup_req_t txParamSetupReq;
                uint8_t eirpDwellTime = payload[mac_index++];

                txParamSetupReq.ul_dwell_time = 0;
                txParamSetupReq.dl_dwell_time = 0;

                if ((eirpDwellTime & 0x20) == 0x20) {
                    txParamSetupReq.dl_dwell_time = 1;
                }
                if ((eirpDwellTime & 0x10) == 0x10) {
                    txParamSetupReq.ul_dwell_time = 1;
                }
                txParamSetupReq.max_eirp = eirpDwellTime & 0x0F;

                // Check the status for correctness
                if (lora_phy.accept_tx_param_setup_req(&txParamSetupReq)) {
                    // Accept command
                    mac_sys_params.uplink_dwell_time =
                            txParamSetupReq.ul_dwell_time;
                    mac_sys_params.downlink_dwell_time =
                            txParamSetupReq.dl_dwell_time;
                    mac_sys_params.max_eirp =
                            max_eirp_table[txParamSetupReq.max_eirp];
                    // Add command response
                    ret_value = add_mac_command(MOTE_MAC_TX_PARAM_SETUP_ANS, 0, 0);
                }
            }
                break;
            case SRV_MAC_DL_CHANNEL_REQ: {
                dl_channel_req_params_t dlChannelReq;

                dlChannelReq.channel_id = payload[mac_index++];
                dlChannelReq.rx1_frequency = (uint32_t) payload[mac_index++];
                dlChannelReq.rx1_frequency |= (uint32_t) payload[mac_index++] << 8;
                dlChannelReq.rx1_frequency |= (uint32_t) payload[mac_index++] << 16;
                dlChannelReq.rx1_frequency *= 100;

                status = lora_phy.dl_channel_request(&dlChannelReq);

                ret_value = add_mac_command(MOTE_MAC_DL_CHANNEL_ANS, status, 0);
            }
                break;
            default:
                // Unknown command. ABORT MAC commands processing
                ret_value = LORAWAN_STATUS_UNSUPPORTED;
        }
    }
    return ret_value;
}

bool LoRaMacCommand::is_sticky_mac_command_pending()
{
    if (mac_cmd_buf_idx_to_repeat > 0) {
        // Sticky MAC commands pending
        return true;
    }
    return false;
}
