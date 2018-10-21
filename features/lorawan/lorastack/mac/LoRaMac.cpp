#include <stdio.h>
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
#include <stdlib.h>
#include "LoRaMac.h"

#include "mbed-trace/mbed_trace.h"
#define TRACE_GROUP "LMAC"

using namespace events;
using namespace mbed;

/*
 * LoRaWAN spec 6.2: AppKey is AES-128 key
 */
#define APPKEY_KEY_LENGTH                           128

/*!
 * Maximum length of the fOpts field
 */
#define LORA_MAC_COMMAND_MAX_FOPTS_LENGTH           15

/*!
 * LoRaMac duty cycle for the back-off procedure during the first hour.
 */
#define BACKOFF_DC_1_HOUR                           100

/*!
 * LoRaMac duty cycle for the back-off procedure during the next 10 hours.
 */
#define BACKOFF_DC_10_HOURS                         1000

/*!
 * LoRaMac duty cycle for the back-off procedure during the next 24 hours.
 */
#define BACKOFF_DC_24_HOURS                         10000

/*!
 * The frame direction definition for uplink communications.
 */
#define UP_LINK                                     0

/*!
 * The frame direction definition for downlink communications.
 */
#define DOWN_LINK                                   1

#define MHDR_LEN                                    1
#define PORT_FIELD_LEN                              1
#define FHDR_LEN_WITHOUT_FOPTS                      7

#define JOIN_ACCEPT_LEN_WITHOUT_CFLIST              12

static void memcpy_convert_endianess(uint8_t *dst,
                                     const uint8_t *src,
                                     uint16_t size)
{
    dst = dst + (size - 1);
    while (size--) {
        *dst-- = *src++;
    }
}

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
      _can_cancel_tx(true),
      _continuous_rx2_window_open(false),
      _dl_fport_available(true),
      _device_class(CLASS_A),
      _prev_qos_level(LORAWAN_DEFAULT_QOS),
      _demod_ongoing(false)
{
    memset(&_params, 0, sizeof(_params));
    _params.keys.dev_eui = NULL;
    _params.keys.app_eui = NULL;
    _params.keys.app_key = NULL;
    _params.keys.nwk_key = NULL;

    _params.rejoin_forced = false;
    _params.forced_datarate = DR_0;

    memset(_params.keys.nwk_skey, 0, sizeof(_params.keys.nwk_skey));
    memset(_params.keys.app_skey, 0, sizeof(_params.keys.app_skey));
    memset(_params.keys.snwk_sintkey, 0, sizeof(_params.keys.snwk_sintkey));
    memset(_params.keys.nwk_senckey, 0, sizeof(_params.keys.nwk_senckey));
    memset(&_ongoing_tx_msg, 0, sizeof(_ongoing_tx_msg));
    memset(&_params.sys_params, 0, sizeof(_params.sys_params));

    _params.dev_nonce = 0;
    _params.net_id = 0;
    _params.dev_addr = 0;
    _params.tx_buffer_len = 0;
    _params.rx_buffer_len = 0;
    _params.ul_frame_counter = 0;
    _params.dl_frame_counter = 0;
    _params.app_dl_frame_counter = 0;
    _params.is_rx_window_enabled = true;
    _params.adr_ack_counter = 0;
    _params.is_node_ack_requested = false;
    _params.is_srv_ack_requested = false;
    _params.ul_nb_rep_counter = 0;
    _params.timers.mac_init_time = 0;
    _params.max_ack_timeout_retries = 1;
    _params.ack_timeout_retry_counter = 1;
    _params.is_ack_retry_timeout_expired = false;
    _params.timers.tx_toa = 0;

    _params.multicast_channels = NULL;


    _params.sys_params.adr_on = false;
    _params.sys_params.max_duty_cycle = 0;

    _params.join_request_type = JOIN_REQUEST;

    //TODO: RJcount1 must be stored to NVM!
    _params.RJcount0 = 0;
    _params.RJcount1 = 0;

    reset_mcps_confirmation();
    reset_mcps_indication();
}

LoRaMac::~LoRaMac()
{
}

/***************************************************************************
 * Radio event callbacks - delegated to Radio driver                       *
 **************************************************************************/

const loramac_mcps_confirm_t *LoRaMac::get_mcps_confirmation() const
{
    return &_mcps_confirmation;
}

const loramac_mcps_indication_t *LoRaMac::get_mcps_indication() const
{
    return &_mcps_indication;
}

const loramac_mlme_indication_t *LoRaMac::get_mlme_indication() const
{
    return &_mlme_indication;
}

void LoRaMac::post_process_mcps_req()
{
    _params.is_last_tx_join_request = false;
    _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_OK;
    if (_mcps_confirmation.req_type == MCPS_CONFIRMED) {
        // An MCPS request for a CONFIRMED message has received an ack
        // in the downlink message
        if (_mcps_confirmation.ack_received) {
            _params.is_node_ack_requested = false;
            _mcps_confirmation.ack_received = false;
            _mcps_indication.is_ack_recvd = false;
            _params.ul_frame_counter++;
            _params.adr_ack_counter++;
        } else {
            if (_params.server_type == LW1_1) {
                // because network server will not accept un-incremented fcnt
                _params.ul_frame_counter++;
            }
            _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
        }
    } else {
        //UNCONFIRMED or PROPRIETARY
        _params.ul_frame_counter++;
        _params.adr_ack_counter++;
        if (_params.sys_params.nb_trans > 1) {
            _mcps_confirmation.nb_retries = _params.ul_nb_rep_counter;
        }
    }
}

void LoRaMac::post_process_mcps_ind()
{
    _mcps_indication.pending = false;
}

void LoRaMac::post_process_mlme_ind()
{
    _mlme_indication.pending = false;
}

lorawan_time_t LoRaMac::get_current_time(void)
{
    return _lora_time.get_current_time();
}

rx_slot_t LoRaMac::get_current_slot(void)
{
    return _params.rx_slot;
}

/**
 * This part handles incoming frames in response to Radio RX Interrupt
 */
loramac_event_info_status_t LoRaMac::handle_join_accept_frame(const uint8_t *payload, uint16_t size)
{
    uint32_t mic = 0;
    uint32_t mic_rx = 0;
    server_type_t stype = LW1_0_2;
    bool is_cflist_present = false;
    uint8_t *decrypt_key = NULL;

    if (size > JOIN_ACCEPT_LEN_WITHOUT_CFLIST) {
        is_cflist_present = true;
    }

    if (_params.join_request_type == JOIN_REQUEST) {
        decrypt_key = _params.keys.nwk_key;
    } else {
        decrypt_key = _params.keys.js_enckey;
    }

    if (0 != _lora_crypto.decrypt_join_frame(payload + 1, size - 1,
                                             decrypt_key, APPKEY_KEY_LENGTH,
                                             _params.rx_buffer + 1)) {
        return LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
    }
    _params.rx_buffer[0] = payload[0];

    //Store server type to local so that invalid join accept of rejoin request won't affect the orig. type.
    if ( (((_params.rx_buffer[11] >> 7) & 0x01) == 1) && MBED_CONF_LORA_VERSION == LORAWAN_VERSION_1_1) {
        stype = LW1_1;
        tr_debug("LoRaWAN 1.1.x server");
    } else {
        stype = LW1_0_2;
        tr_debug("LoRaWAN 1.0.x server");
        //Server does not support LW 1.1 so we need to unset JS keys
        memcpy(_params.keys.js_intkey, _params.keys.nwk_key, sizeof(_params.keys.nwk_skey));
        memcpy(_params.keys.js_enckey, _params.keys.nwk_key, sizeof(_params.keys.nwk_skey));
    }

    uint8_t payload_start = 1;
    uint8_t mic_start = 0;
    uint8_t args_size = 0;
    uint8_t args[16];
    uint16_t nonce_or_rj_cnt = 0;

    uint8_t *mic_key = _params.keys.js_intkey; //in case of LW1.0.2 js_intkey == nwk_key == app_key

    if (stype == LW1_0_2) {
        mic_start = size - LORAMAC_MFR_LEN;

        memcpy(args, _params.rx_buffer + 1, 6);
        memcpy(args + 6, (uint8_t *) &_params.dev_nonce, 2);
        args_size = 8;
    } else {
        //MIC calculation needs more params, so we move the payload a bit
        memmove(_params.rx_buffer + 11, _params.rx_buffer, size);
        _params.rx_buffer[0] = _params.join_request_type; // JoinReqType
        memcpy_convert_endianess(_params.rx_buffer + 1,  _params.keys.app_eui, 8); // JoinEUI



        switch (_params.join_request_type) {
            case JOIN_REQUEST:
                nonce_or_rj_cnt = _params.dev_nonce;
                break;
            case REJOIN_REQUEST_TYPE0:
            case REJOIN_REQUEST_TYPE2:
                nonce_or_rj_cnt = _params.RJcount0;
                break;
            case REJOIN_REQUEST_TYPE1:
                nonce_or_rj_cnt = _params.RJcount1;
                break;
            default:
                tr_error("Unknown Join Request Type");
                MBED_ASSERT(false);
        }

        _params.rx_buffer[9] = nonce_or_rj_cnt & 0xFF; // DevNonce
        _params.rx_buffer[10] = (nonce_or_rj_cnt >> 8) & 0xFF;

        // MIC is encrypted as part of payload
        mic_start = size + 11 - LORAMAC_MFR_LEN;
        payload_start += 11;

        memcpy(args, _params.rx_buffer + payload_start, 3);
        memcpy_convert_endianess(args + 3, _params.keys.app_eui, 8);
        args[3+8] = nonce_or_rj_cnt & 0xFF;
        args[3+9] = (nonce_or_rj_cnt >> 8) & 0xFF;
        args_size = 13;
    }

    if (_lora_crypto.compute_join_frame_mic(_params.rx_buffer,
                                            mic_start,
                                            mic_key,
                                            APPKEY_KEY_LENGTH,
                                            &mic) != 0) {
        return LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
    }

    mic_rx |= (uint32_t) _params.rx_buffer[mic_start];
    mic_rx |= ((uint32_t) _params.rx_buffer[mic_start + 1] << 8);
    mic_rx |= ((uint32_t) _params.rx_buffer[mic_start + 2] << 16);
    mic_rx |= ((uint32_t) _params.rx_buffer[mic_start + 3] << 24);

    if (mic_rx == mic) {
        _lora_time.stop(_params.timers.rx_window2_timer);
        _params.server_type = stype;
        if (_lora_crypto.compute_skeys_for_join_frame(_params.keys.nwk_key,
                                                      APPKEY_KEY_LENGTH,
                                                      _params.keys.app_key,
                                                      APPKEY_KEY_LENGTH,
                                                      args, args_size,
                                                      _params.keys.nwk_skey,
                                                      _params.keys.app_skey,
                                                      _params.keys.snwk_sintkey,
                                                      _params.keys.nwk_senckey,
                                                      _params.server_type) != 0) {
            return LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
        }

        _params.net_id = (uint32_t) _params.rx_buffer[payload_start + 3];
        _params.net_id |= ((uint32_t) _params.rx_buffer[payload_start + 4] << 8);
        _params.net_id |= ((uint32_t) _params.rx_buffer[payload_start + 5] << 16);

        _params.dev_addr = (uint32_t) _params.rx_buffer[payload_start + 6];
        _params.dev_addr |= ((uint32_t) _params.rx_buffer[payload_start + 7] << 8);
        _params.dev_addr |= ((uint32_t) _params.rx_buffer[payload_start + 8] << 16);
        _params.dev_addr |= ((uint32_t) _params.rx_buffer[payload_start + 9] << 24);

        if (_params.server_type == LW1_0_2 ||
                _params.join_request_type != REJOIN_REQUEST_TYPE2) {
            reset_mac_parameters();
            reset_frame_counters();
            reset_phy_params();
        } else if (_params.server_type == LW1_1 &&
                _params.join_request_type == REJOIN_REQUEST_TYPE2){
            reset_frame_counters();
        }

        _params.sys_params.rx1_dr_offset = (_params.rx_buffer[payload_start + 10] >> 4) & 0x07;
        _params.sys_params.rx2_channel.datarate = _params.rx_buffer[payload_start + 10] & 0x0F;

        _params.sys_params.recv_delay1 = (_params.rx_buffer[payload_start + 11] & 0x0F);

        if (_params.sys_params.recv_delay1 == 0) {
            _params.sys_params.recv_delay1 = 1;
        }

        _params.sys_params.recv_delay1 *= 1000;
        _params.sys_params.recv_delay2 = _params.sys_params.recv_delay1 + 1000;

        // Size of the regular payload is 12. Plus 1 byte MHDR and 4 bytes MIC (== 17)
        //TODO: join request type is needed here also! See LW1.1 lines 1711 -> 1719 (Reset or not)
        // LW1.1 CF_LIST's 16th byte is CFListType!
        if (is_cflist_present) {
            _lora_phy->apply_cf_list(&_params.rx_buffer[payload_start + JOIN_ACCEPT_LEN_WITHOUT_CFLIST],
                                     size - (JOIN_ACCEPT_LEN_WITHOUT_CFLIST + MHDR_LEN + LORAMAC_MFR_LEN));
        } else {
            if (_params.join_request_type != REJOIN_REQUEST_TYPE2) {
                _lora_phy->restore_default_channels();
            }
        }

        _is_nwk_joined = true;

        if (_params.join_request_type == REJOIN_REQUEST_TYPE0 ||
                _params.join_request_type == REJOIN_REQUEST_TYPE2) {
            _params.RJcount0 = 0;
        } else {
            _params.RJcount1 = 0;
        }

    } else {
        return LORAMAC_EVENT_INFO_STATUS_JOIN_FAIL;
    }
    return LORAMAC_EVENT_INFO_STATUS_OK;
}

void LoRaMac::check_frame_size(uint16_t size)
{
    uint8_t value = _lora_phy->get_max_payload(_mcps_indication.rx_datarate,
                                               _params.is_repeater_supported);

    if (MAX(0, (int16_t)((int16_t)size - (int16_t)LORA_MAC_FRMPAYLOAD_OVERHEAD))
            > (int32_t) value) {
        tr_error("Invalid frame size");
    }
}

bool LoRaMac::message_integrity_check(const uint8_t *const payload,
                                      const uint16_t size,
                                      uint8_t *const ptr_pos, uint16_t confFCnt,
                                      uint32_t address,
                                      uint32_t *downlink_counter,
                                      const uint8_t *nwk_skey)
{
    uint32_t mic = 0;
    uint32_t mic_rx = 0;
    uint32_t args = confFCnt;

    uint16_t sequence_counter = 0;
    uint16_t sequence_counter_prev = 0;
    uint16_t sequence_counter_diff = 0;

    sequence_counter = (uint16_t) payload[(*ptr_pos)++];
    sequence_counter |= (uint16_t) payload[(*ptr_pos)++] << 8;

    mic_rx |= (uint32_t) payload[size - LORAMAC_MFR_LEN];
    mic_rx |= ((uint32_t) payload[size - LORAMAC_MFR_LEN + 1] << 8);
    mic_rx |= ((uint32_t) payload[size - LORAMAC_MFR_LEN + 2] << 16);
    mic_rx |= ((uint32_t) payload[size - LORAMAC_MFR_LEN + 3] << 24);

    sequence_counter_prev = (uint16_t) * downlink_counter;
    sequence_counter_diff = sequence_counter - sequence_counter_prev;
    *downlink_counter += sequence_counter_diff;
    if (sequence_counter < sequence_counter_prev) {
        *downlink_counter += 0x10000;
    }

    // sizeof nwk_skey must be the same as _params.keys.nwk_skey,
    _lora_crypto.compute_mic(payload, size - LORAMAC_MFR_LEN,
                             nwk_skey, sizeof(_params.keys.nwk_skey) * 8, args,
                             address, DOWN_LINK, *downlink_counter, &mic);

    if (mic_rx != mic) {
        _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_MIC_FAIL;
        return false;
    }

    if (sequence_counter_diff >= _lora_phy->get_maximum_frame_counter_gap()) {
        _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOST;
        _mcps_indication.dl_frame_counter = *downlink_counter;
        return false;
    }

    return true;
}

void LoRaMac::extract_data_and_mac_commands(const uint8_t *payload,
                                            uint16_t size,
                                            uint8_t fopts_len,
                                            uint8_t *nwk_skey,
                                            uint8_t *app_skey,
                                            uint32_t address,
                                            uint32_t downlink_counter,
                                            seq_counter_type_t cnt_type,
                                            int16_t rssi,
                                            int8_t snr,
                                            Callback<void(loramac_mlme_confirm_t&)> confirm_handler)
{
    uint8_t frame_len = 0;
    uint8_t payload_start_index = FHDR_LEN_WITHOUT_FOPTS + PORT_FIELD_LEN + fopts_len;
    uint8_t port = payload[payload_start_index++];
    frame_len = (size - 4) - payload_start_index;

    _mcps_indication.port = port;

    // special handling of control port 0
    if (port == 0) {
        if (fopts_len == 0) {
            if (_lora_crypto.decrypt_payload(payload + payload_start_index,
                                             frame_len,
                                             _params.keys.nwk_senckey,
                                             sizeof(_params.keys.nwk_senckey) * 8,
                                             address,
                                             DOWN_LINK,
                                             downlink_counter,
                                             cnt_type,
                                             FRMPAYLOAD,
                                             _params.rx_buffer, _params.server_type) != 0) {
                _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
            }

            if (_mac_commands.process_mac_commands(_params.rx_buffer, 0, frame_len,
                                                   snr, _params.sys_params, *_lora_phy,
                                                   confirm_handler)
                    != LORAWAN_STATUS_OK) {
                _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                return;
            }

            if (_mac_commands.has_sticky_mac_cmd()) {
                set_mlme_schedule_ul_indication();
                _mac_commands.clear_sticky_mac_cmd();
            }

            return;
        }

        _mcps_indication.pending = false;
        _mcps_confirmation.ack_received = false;
        _mcps_indication.is_ack_recvd = false;

        return;
    }

    if(!extract_mac_commands_only(payload, size, snr, fopts_len, confirm_handler)) {
        return;
    }

    // sizeof app_skey must be the same as _params.keys.app_skey
    if (_lora_crypto.decrypt_payload(payload + payload_start_index,
                                     frame_len,
                                     app_skey,
                                     sizeof(_params.keys.app_skey) * 8,
                                     address,
                                     DOWN_LINK,
                                     downlink_counter,
                                     cnt_type,
                                     FRMPAYLOAD,
                                     _params.rx_buffer,
                                     _params.server_type) != 0) {
        _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
    } else {
        _mcps_indication.buffer = _params.rx_buffer;
        _mcps_indication.buffer_size = frame_len;
        _mcps_indication.is_data_recvd = true;
    }
}

bool LoRaMac::extract_mac_commands_only(const uint8_t *payload,
                                        uint16_t size,
                                        int8_t snr,
                                        uint8_t fopts_len,
                                        Callback<void(loramac_mlme_confirm_t&)> confirm_handler)
{
    if (fopts_len > 0) {
        uint8_t buffer[15] = {0};

        unsigned pld_idx = MHDR_LEN + FHDR_LEN_WITHOUT_FOPTS;

        if (_params.server_type == LW1_1) {
            if (0 != _lora_crypto.decrypt_payload(payload + pld_idx, fopts_len,
                                                  _params.keys.nwk_senckey, sizeof(_params.keys.nwk_senckey) * 8,
                                                  _params.dev_addr, DOWN_LINK,
                                                  _params.dl_frame_counter,
                                                  NFCNT_DOWN,
                                                  FOPTS,
                                                  buffer,
                                                  _params.server_type)) {
                _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
                return false;
            }

        } else {
            memcpy(buffer, payload + pld_idx, fopts_len);
        }

        if (_mac_commands.process_mac_commands(buffer, 0, fopts_len,
                                               snr, _params.sys_params,
                                               *_lora_phy, confirm_handler)

                != LORAWAN_STATUS_OK) {
            _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
            return false;
        }

        if (_mac_commands.has_sticky_mac_cmd()) {
            set_mlme_schedule_ul_indication();
            _mac_commands.clear_sticky_mac_cmd();
        }
    }
    return true;
}

void LoRaMac::handle_data_frame(const uint8_t *const payload,
                                const uint16_t size,
                                uint8_t ptr_pos,
                                uint8_t msg_type,
                                int16_t rssi,
                                int8_t snr,
                                Callback<void(loramac_mlme_confirm_t&)> confirm_handler)
{
    check_frame_size(size);
    bool is_multicast = false;
    loramac_frame_ctrl_t fctrl;
    multicast_params_t *cur_multicast_params;
    uint32_t address = 0;
    uint32_t downlink_counter = 0;
    seq_counter_type_t cnt_type = NFCNT_DOWN;
    uint8_t *nwk_skey = _params.keys.nwk_skey;
    uint8_t *mic_key  = _params.keys.nwk_skey;
    uint8_t *app_skey = _params.keys.app_skey;
    uint16_t fport = 0;
    uint16_t confFCnt = 0;

    // always assume in the beginning that a downlink fport field is included
    _dl_fport_available = true;

    address = payload[ptr_pos++];
    address |= ((uint32_t) payload[ptr_pos++] << 8);
    address |= ((uint32_t) payload[ptr_pos++] << 16);
    address |= ((uint32_t) payload[ptr_pos++] << 24);

    fctrl.value = payload[ptr_pos++];

    int check_frm_len = size - (fctrl.bits.fopts_len + LORA_MAC_FRMPAYLOAD_OVERHEAD);

    if (check_frm_len < 0) {
        _dl_fport_available = false;
    }

    if (_dl_fport_available) {
        fport = payload[FHDR_LEN_WITHOUT_FOPTS + fctrl.bits.fopts_len];
    }

    if (address != _params.dev_addr) {
        // check if Multicast is destined for us
        cur_multicast_params = _params.multicast_channels;

        while (cur_multicast_params != NULL) {
            if (address == cur_multicast_params->address) {
                is_multicast = true;
                nwk_skey = cur_multicast_params->nwk_skey;
                mic_key = cur_multicast_params->nwk_skey;
                app_skey = cur_multicast_params->app_skey;
                downlink_counter = cur_multicast_params->dl_frame_counter;
                break;
            }

            cur_multicast_params = cur_multicast_params->next;
        }

        if (!is_multicast) {
            // We are not the destination of this frame.
            _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_ADDRESS_FAIL;
            _mcps_indication.pending = false;
            return;
        }
    } else {
        is_multicast = false;
        nwk_skey = _params.keys.nwk_skey;
        mic_key = _params.keys.snwk_sintkey;
        app_skey = _params.keys.app_skey;
        downlink_counter = _params.dl_frame_counter;
    }

    if (_params.server_type == LW1_1) {
        if (_params.is_node_ack_requested && fctrl.bits.ack) {
            confFCnt = _mcps_confirmation.ul_frame_counter;
        }
        if (!is_multicast) {
            if (_dl_fport_available && fport != 0) {
                downlink_counter = _params.app_dl_frame_counter;
                cnt_type = AFCNT_DOWN;
            } else {
                nwk_skey = _params.keys.nwk_senckey;
                cnt_type = NFCNT_DOWN;
            }
        }
    }

    //perform MIC check
    if (!message_integrity_check(payload, size, &ptr_pos, confFCnt, address,
                                 &downlink_counter, mic_key)) {
        tr_error("MIC failed");
        _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_MIC_FAIL;
        _mcps_indication.pending = false;
        return;
    }

    _mcps_confirmation.ack_received = false;
    _mcps_indication.is_ack_recvd = false;
    _mcps_indication.pending = true;
    _mcps_indication.is_data_recvd = false;
    _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_OK;
    _mcps_indication.multicast = is_multicast;
    _mcps_indication.fpending_status = fctrl.bits.fpending;
    _mcps_indication.buffer = NULL;
    _mcps_indication.buffer_size = 0;
    _mcps_indication.dl_frame_counter = downlink_counter;
    _mcps_indication.rssi = rssi;
    _mcps_indication.snr = snr;

    _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_OK;

    _params.adr_ack_counter = 0;
    _mac_commands.clear_repeat_buffer();
    _mac_commands.clear_command_buffer();

    if (is_multicast) {
        _mcps_indication.type = MCPS_MULTICAST;

        // Discard if its a repeated message
        if ((cur_multicast_params->dl_frame_counter == downlink_counter)
                && (cur_multicast_params->dl_frame_counter != 0)) {
            _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED;
            _mcps_indication.dl_frame_counter = downlink_counter;
            _mcps_indication.pending = false;

            return;
        }

        cur_multicast_params->dl_frame_counter = downlink_counter;

    } else {
        if (msg_type == FRAME_TYPE_DATA_CONFIRMED_DOWN) {
            _params.is_srv_ack_requested = true;
            _params.counterForAck = downlink_counter;
            _mcps_indication.type = MCPS_CONFIRMED;

            if ((_params.dl_frame_counter == downlink_counter)
                    && (_params.dl_frame_counter != 0)) {
                // Duplicated confirmed downlink. Skip indication.
                // In this case, the MAC layer shall accept the MAC commands
                // which are included in the downlink retransmission.
                // It should not provide the same frame to the application
                // layer again. The MAC layer accepts the acknowledgement.
                tr_debug("Discarding duplicate frame");
                _mcps_indication.pending = false;
                _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED;

                return;
            }
        } else if (msg_type == FRAME_TYPE_DATA_UNCONFIRMED_DOWN) {
            _params.is_srv_ack_requested = false;
            _mcps_indication.type = MCPS_UNCONFIRMED;

            if ((_params.dl_frame_counter == downlink_counter)
                    && (_params.dl_frame_counter != 0)) {
                tr_debug("Discarding duplicate frame");
                _mcps_indication.pending = false;
                _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED;

                return;
            }
        }
        _params.dl_frame_counter = downlink_counter;
    }

    // message is intended for us and MIC have passed, stop RX2 Window
    // Spec: 3.3.4 Receiver Activity during the receive windows
    if (get_current_slot() == RX_SLOT_WIN_1) {
        _lora_time.stop(_params.timers.rx_window2_timer);
    } else {
        _lora_time.stop(_params.timers.rx_window1_timer);
        _lora_time.stop(_params.timers.rx_window2_timer);
    }

    if (_device_class == CLASS_C) {
        _lora_time.stop(_rx2_closure_timer_for_class_c);
    }

    if (_params.is_node_ack_requested && fctrl.bits.ack) {
        _mcps_confirmation.ack_received = fctrl.bits.ack;
        _mcps_indication.is_ack_recvd = fctrl.bits.ack;
    }

    if (check_frm_len > 0) {
        extract_data_and_mac_commands(payload, size, fctrl.bits.fopts_len,
                                      nwk_skey, app_skey, address,
                                      downlink_counter, cnt_type, rssi, snr,
                                      confirm_handler);
    } else {
        extract_mac_commands_only(payload, size, snr, fctrl.bits.fopts_len, confirm_handler);
    }

    // Handle proprietary messages.
    if (msg_type == FRAME_TYPE_PROPRIETARY) {
        memcpy(_params.rx_buffer, &payload[ptr_pos], size);

        _mcps_indication.type = MCPS_PROPRIETARY;
        _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_OK;
        _mcps_indication.buffer = _params.rx_buffer;
        _mcps_indication.buffer_size = size - ptr_pos;
    }

    // only stop ack timer, if the ack is actually received
    if (_mcps_confirmation.ack_received) {
        _lora_time.stop(_params.timers.ack_timeout_timer);
    }
}

void LoRaMac::set_batterylevel_callback(mbed::Callback<uint8_t(void)> battery_level)
{
    _mac_commands.set_batterylevel_callback(battery_level);
}

void LoRaMac::on_radio_tx_done(lorawan_time_t timestamp)
{
    if (_device_class == CLASS_C) {
        // this will open a continuous RX2 window until time==RECV_DELAY1
        open_rx2_window();
    } else {
        _lora_phy->put_radio_to_sleep();
    }

    if ((_mcps_confirmation.req_type == MCPS_UNCONFIRMED)
            && (_params.sys_params.nb_trans > 1)) {
        //MBED_ASSERT(_params.ul_nb_rep_counter <= _params.sys_params.nb_trans);
        _params.ul_nb_rep_counter++;
    }

    if (_params.is_rx_window_enabled == true) {
        lorawan_time_t time_diff = _lora_time.get_current_time() - timestamp;
        // start timer after which rx1_window will get opened
        _lora_time.start(_params.timers.rx_window1_timer,
                         _params.rx_window1_delay - time_diff);

        // start timer after which rx2_window will get opened
        _lora_time.start(_params.timers.rx_window2_timer,
                         _params.rx_window2_delay - time_diff);

        // If class C and an Unconfirmed messgae is outgoing,
        // this will start a timer which will invoke rx2 would be
        // closure handler
        if (get_device_class() == CLASS_C) {
            _lora_time.start(_rx2_closure_timer_for_class_c,
                             (_params.rx_window2_delay - time_diff) +
                             _params.rx_window2_config.window_timeout);
        }

        // start timer after which ack wait will timeout (for Confirmed messages)
        if (_params.is_node_ack_requested) {
            _lora_time.start(_params.timers.ack_timeout_timer,
                             (_params.rx_window2_delay - time_diff) +
                             _lora_phy->get_ack_timeout());
        }
    } else {
        _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_OK;
    }

    _params.last_channel_idx = _params.channel;

    _lora_phy->set_last_tx_done(_params.channel, _is_nwk_joined, timestamp);

    _params.timers.aggregated_last_tx_time = timestamp;

    _mac_commands.clear_command_buffer();
}

void LoRaMac::on_radio_rx_done(const uint8_t *const payload, uint16_t size,
                               int16_t rssi, int8_t snr,
                               Callback<void(loramac_mlme_confirm_t&)> confirm_handler)
{
    _demod_ongoing = false;
    if (_device_class == CLASS_C && !_continuous_rx2_window_open) {
        _lora_time.stop(_rx2_closure_timer_for_class_c);
        open_rx2_window();
    } else if (_device_class != CLASS_C) {
        _lora_time.stop(_params.timers.rx_window1_timer);
        _lora_phy->put_radio_to_sleep();
    }

    loramac_mhdr_t mac_hdr;
    loramac_mlme_confirm_t mlme;
    uint8_t pos = 0;
    mac_hdr.value = payload[pos++];

    switch (mac_hdr.bits.mtype) {

        case FRAME_TYPE_JOIN_ACCEPT:

            if (nwk_joined()) {
                //TODO: this might need more logic as with rejoin this will happen more often
                _params.RJcount0 = 0;
                return;
            } else {
                loramac_event_info_status_t ret = handle_join_accept_frame(payload, size);
                mlme.type = MLME_JOIN_ACCEPT;
                mlme.status = ret;
                confirm_handler(mlme);
            }

            break;

        case FRAME_TYPE_DATA_UNCONFIRMED_DOWN:
        case FRAME_TYPE_DATA_CONFIRMED_DOWN:
        case FRAME_TYPE_PROPRIETARY:

            handle_data_frame(payload, size, pos, mac_hdr.bits.mtype, rssi, snr, confirm_handler);

            break;

        default:
            break;
    }
}

void LoRaMac::on_radio_tx_timeout(void)
{
    _lora_time.stop(_params.timers.rx_window1_timer);
    _lora_time.stop(_params.timers.rx_window2_timer);
    _lora_time.stop(_rx2_closure_timer_for_class_c);
    _lora_time.stop(_params.timers.ack_timeout_timer);

    if (_device_class == CLASS_C) {
        open_rx2_window();
    } else {
        _lora_phy->put_radio_to_sleep();
    }

    _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT;

    _mac_commands.clear_command_buffer();

    if (_mcps_confirmation.req_type == MCPS_CONFIRMED) {
        _mcps_confirmation.nb_retries = _params.ack_timeout_retry_counter;
    } else {
        _mcps_confirmation.nb_retries = _params.ul_nb_rep_counter;
    }

    _mcps_confirmation.ack_received = false;
    _mcps_confirmation.tx_toa = 0;
}

void LoRaMac::on_radio_rx_timeout(bool is_timeout)
{
    _demod_ongoing = false;
    if (_device_class == CLASS_A) {
        _lora_phy->put_radio_to_sleep();
    }

    if (_params.rx_slot == RX_SLOT_WIN_1) {
        if (_params.is_node_ack_requested == true) {
            _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_RX1_ERROR;
        }
        if (_device_class != CLASS_C) {
            if (_lora_time.get_elapsed_time(_params.timers.aggregated_last_tx_time) >= _params.rx_window2_delay) {
                _lora_time.stop(_params.timers.rx_window2_timer);
            }
        }
    } else {
        if (_params.is_node_ack_requested == true) {
            _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_RX2_ERROR;
        }
    }
}

bool LoRaMac::continue_joining_process()
{
    if (_params.join_request_trial_counter >= _params.max_join_request_trials) {
        return false;
    }

    // Schedule a retry
    if (handle_retransmission() != LORAWAN_STATUS_CONNECT_IN_PROGRESS) {
        return false;
    }

    return true;
}

bool LoRaMac::continue_sending_process()
{
    if (_params.ack_timeout_retry_counter >= _params.max_ack_timeout_retries) {
        _lora_time.stop(_params.timers.ack_timeout_timer);
        return false;
    }

    // retransmission will be handled in on_ack_timeout() whence the ACK timeout
    // gets fired
    return true;
}

lorawan_status_t LoRaMac::send_join_request()
{
    lorawan_status_t status = LORAWAN_STATUS_OK;
    loramac_mhdr_t mac_hdr;
    loramac_frame_ctrl_t fctrl;

    _params.sys_params.channel_data_rate =
        _lora_phy->get_alternate_DR(_params.join_request_trial_counter + 1);

    mac_hdr.value = 0;
    if (_params.join_request_type == JOIN_REQUEST) {
        mac_hdr.bits.mtype = FRAME_TYPE_JOIN_REQ;
        _params.is_last_tx_join_request = true;
    } else {
        mac_hdr.bits.mtype = FRAME_TYPE_REJOIN_REQUEST;
        _params.is_last_tx_join_request = false;
    }

    fctrl.value = 0;
    fctrl.bits.adr = _params.sys_params.adr_on;

    /* In case of join request retransmissions, the stack must prepare
     * the frame again, because the network server keeps track of the random
     * LoRaMacDevNonce values to prevent replay attacks. */
    status = prepare_frame(&mac_hdr, &fctrl, 0, NULL, 0);

    if (status == LORAWAN_STATUS_OK) {
        if (schedule_tx() == LORAWAN_STATUS_OK) {
            status = LORAWAN_STATUS_CONNECT_IN_PROGRESS;
        }
    } else {
        tr_error("Couldn't send a JoinRequest: error %d", status);
    }

    return status;
}

/**
 * This function handles retransmission of failed or unacknowledged
 * outgoing traffic
 */
lorawan_status_t LoRaMac::handle_retransmission()
{
    //TODO: Rejoin requests are not retransmitted (except in case of ForceRejoinReq), in most of the cases server won't respond
    if (!nwk_joined() && _params.is_last_tx_join_request) {
        return send_join_request();
    }

    return schedule_tx();
}

/**
 * This function is called when the backoff_timer gets fired.
 * It is used for re-scheduling an unsent packet in the pipe. This packet
 * can be a Join Request or any other data packet.
 */
void LoRaMac::on_backoff_timer_expiry(void)
{
    Lock lock(*this);

    _lora_time.stop(_params.timers.backoff_timer);

    if ((schedule_tx() != LORAWAN_STATUS_OK) && nwk_joined()) {
        _scheduling_failure_handler.call();
    }
}

void LoRaMac::open_rx1_window(void)
{
    Lock lock(*this);
    _demod_ongoing = true;
    _continuous_rx2_window_open = false;
    _lora_time.stop(_params.timers.rx_window1_timer);
    _params.rx_slot = RX_SLOT_WIN_1;

    _params.rx_window1_config.channel = _params.channel;
    _params.rx_window1_config.dr_offset = _params.sys_params.rx1_dr_offset;
    _params.rx_window1_config.dl_dwell_time = _params.sys_params.downlink_dwell_time;
    _params.rx_window1_config.is_repeater_supported = _params.is_repeater_supported;
    _params.rx_window1_config.is_rx_continuous = false;
    _params.rx_window1_config.rx_slot = _params.rx_slot;

    if (_device_class == CLASS_C) {
        _lora_phy->put_radio_to_standby();
    }

    _mcps_indication.rx_datarate = _params.rx_window1_config.datarate;

    _lora_phy->rx_config(&_params.rx_window1_config);
    _lora_phy->handle_receive();

    tr_debug("RX1 slot open, Freq = %lu", _params.rx_window1_config.frequency);
}

void LoRaMac::open_rx2_window()
{
    if (_demod_ongoing) {
        tr_info("RX1 Demodulation ongoing, skip RX2 window opening");
        return;
    }
    Lock lock(*this);
    _continuous_rx2_window_open = true;
    _lora_time.stop(_params.timers.rx_window2_timer);

    _params.rx_window2_config.channel = _params.channel;
    _params.rx_window2_config.frequency = _params.sys_params.rx2_channel.frequency;
    _params.rx_window2_config.dl_dwell_time = _params.sys_params.downlink_dwell_time;
    _params.rx_window2_config.is_repeater_supported = _params.is_repeater_supported;

    if (get_device_class() == CLASS_C) {
        _params.rx_window2_config.is_rx_continuous = true;
    } else {
        _params.rx_window2_config.is_rx_continuous = false;
    }

    _params.rx_window2_config.rx_slot = _params.rx_window2_config.is_rx_continuous ?
                                        RX_SLOT_WIN_CLASS_C : RX_SLOT_WIN_2;

    _mcps_indication.rx_datarate = _params.rx_window2_config.datarate;

    _lora_phy->rx_config(&_params.rx_window2_config);
    _lora_phy->handle_receive();
    _params.rx_slot = _params.rx_window2_config.rx_slot;

    tr_debug("RX2 slot open, Freq = %lu", _params.rx_window2_config.frequency);
}

void LoRaMac::on_ack_timeout_timer_event(void)
{
    Lock lock(*this);

    if (_params.ack_timeout_retry_counter > _params.max_ack_timeout_retries) {
        return;
    }

    tr_debug("ACK_TIMEOUT Elapses, Retrying ...");
    _lora_time.stop(_params.timers.ack_timeout_timer);

    // reduce data rate on every 2nd attempt if and only if the
    // ADR is on
    if ((_params.ack_timeout_retry_counter % 2)
            && (_params.sys_params.adr_on)) {
        tr_debug("Trading datarate for range");
        _params.sys_params.channel_data_rate = _lora_phy->get_next_lower_tx_datarate(_params.sys_params.channel_data_rate);
    }

    _mcps_confirmation.nb_retries = _params.ack_timeout_retry_counter;


    // Schedule a retry
    lorawan_status_t status = handle_retransmission();

    if (status == LORAWAN_STATUS_NO_CHANNEL_FOUND ||
        status == LORAWAN_STATUS_NO_FREE_CHANNEL_FOUND) {
        // In a case when enabled channels are not found, PHY layer
        // resorts to default channels. Next attempt should go forward as the
        // default channels are always available if there is a base station in the
        // vicinity. Otherwise something is wrong with the stack, we should assert
        // here
        _mac_commands.clear_command_buffer();
        _params.is_node_ack_requested = false;
        _mcps_confirmation.ack_received = false;
        _mcps_confirmation.nb_retries = _params.ack_timeout_retry_counter;

        // For the next attempt we need to make sure that we do not incur length error
        // which would mean that the datarate changed during retransmissions and
        // the original packet doesn't fit into allowed payload buffer anymore.
        status = handle_retransmission();

        if (status == LORAWAN_STATUS_LENGTH_ERROR) {
            _scheduling_failure_handler.call();
            return;
        }

        // if we did not incur a length error and still the status is not OK,
        // it is a critical failure
        status = handle_retransmission();
        MBED_ASSERT(status == LORAWAN_STATUS_OK);
        (void) status;
    } else if (status != LORAWAN_STATUS_OK) {
        _scheduling_failure_handler.call();
        return;
    }

    _params.ack_timeout_retry_counter++;
}

bool LoRaMac::validate_payload_length(uint16_t length,
                                      int8_t datarate,
                                      uint8_t fopts_len)
{
    uint16_t max_value = 0;
    uint16_t payloadSize = 0;

    max_value = _lora_phy->get_max_payload(datarate, _params.is_repeater_supported);

    // Calculate the resulting payload size
    payloadSize = (length + fopts_len);

    // Validation of the application payload size
    if ((payloadSize <= max_value) &&
            (payloadSize <= LORAMAC_PHY_MAXPAYLOAD)) {
        return true;
    }
    return false;
}

void LoRaMac::set_mlme_schedule_ul_indication(void)
{
    _mlme_indication.indication_type = MLME_SCHEDULE_UPLINK;
    _mlme_indication.pending = true;
}

// This is not actual transmission. It just schedules a message in response
// to MCPS request
lorawan_status_t LoRaMac::send(loramac_mhdr_t *machdr, const uint8_t fport,
                               const void *fbuffer, uint16_t fbuffer_size)
{
    loramac_frame_ctrl_t fctrl;

    fctrl.value = 0;
    fctrl.bits.fopts_len = 0;
    fctrl.bits.fpending = 0;
    fctrl.bits.ack = false;
    fctrl.bits.adr_ack_req = false;
    fctrl.bits.adr = _params.sys_params.adr_on;

    lorawan_status_t status = prepare_frame(machdr, &fctrl, fport, fbuffer,
                                            fbuffer_size);

    if (status != LORAWAN_STATUS_OK) {
        return status;
    }

    // Reset confirm parameters
    _mcps_confirmation.nb_retries = 0;
    _mcps_confirmation.ack_received = false;
    _mcps_confirmation.ul_frame_counter = _params.ul_frame_counter;

    status = schedule_tx();

    return status;
}

int LoRaMac::get_backoff_timer_event_id(void)
{
    return _params.timers.backoff_timer.timer_id;
}

lorawan_status_t LoRaMac::clear_tx_pipe(void)
{
    if (!_can_cancel_tx) {
        return LORAWAN_STATUS_BUSY;
    }

    // check if the event is not already queued
    const int id = get_backoff_timer_event_id();

    if (id == 0) {
        // No queued send request
        return LORAWAN_STATUS_NO_OP;
    }

    if (_ev_queue->time_left(id) > 0) {
        _lora_time.stop(_params.timers.backoff_timer);
        _lora_time.stop(_params.timers.ack_timeout_timer);
        memset(_params.tx_buffer, 0, sizeof _params.tx_buffer);
        _params.tx_buffer_len = 0;
        reset_ongoing_tx(true);
        tr_debug("Sending Cancelled");
        return LORAWAN_STATUS_OK;
    } else {
        // Event is already being dispatched so it cannot be cancelled
        return LORAWAN_STATUS_BUSY;
    }
}

lorawan_status_t LoRaMac::schedule_tx()
{
    channel_selection_params_t next_channel;
    lorawan_time_t backoff_time = 0;
    uint8_t fopts_len = 0;

    if (_params.sys_params.max_duty_cycle == 255) {
        return LORAWAN_STATUS_DEVICE_OFF;
    }

    if (_params.sys_params.max_duty_cycle == 0) {
        _params.timers.aggregated_timeoff = 0;
    }

    if (MBED_CONF_LORA_DUTY_CYCLE_ON && _lora_phy->verify_duty_cycle(true)) {
        _params.is_dutycycle_on = true;
    } else {
        _params.is_dutycycle_on = false;
    }

    calculate_backOff(_params.last_channel_idx);

    next_channel.aggregate_timeoff = _params.timers.aggregated_timeoff;
    next_channel.current_datarate = _params.sys_params.channel_data_rate;
    next_channel.dc_enabled = _params.is_dutycycle_on;
    next_channel.joined = _is_nwk_joined;
    next_channel.last_aggregate_tx_time = _params.timers.aggregated_last_tx_time;

    lorawan_status_t status = _lora_phy->set_next_channel(&next_channel,
                                                          &_params.channel,
                                                          &backoff_time,
                                                          &_params.timers.aggregated_timeoff);

    switch (status) {
        case LORAWAN_STATUS_NO_CHANNEL_FOUND:
        case LORAWAN_STATUS_NO_FREE_CHANNEL_FOUND:
            _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
            return status;
        case LORAWAN_STATUS_DUTYCYCLE_RESTRICTED:
            if (backoff_time != 0) {
                tr_debug("DC enforced: Transmitting in %lu ms", backoff_time);
                _can_cancel_tx = true;
                _lora_time.start(_params.timers.backoff_timer, backoff_time);
            }
            return LORAWAN_STATUS_OK;
        default:
            break;
    }


    uint8_t rx1_dr = _lora_phy->apply_DR_offset(_params.sys_params.channel_data_rate,
                                                _params.sys_params.rx1_dr_offset);

    bool process_mic = true;
    loramac_mhdr_t mac_hdr;
    mac_hdr.value = _params.tx_buffer[0];
    if (mac_hdr.bits.mtype == FRAME_TYPE_JOIN_REQ || mac_hdr.bits.mtype == FRAME_TYPE_REJOIN_REQUEST) {
        // JOIN and REJOIN frames already has own MIC
        process_mic = false;
    }

    if (process_mic) {
        status = calculate_userdata_mic();
        if (status != LORAWAN_STATUS_OK) {
            return status;
        }
    }

    tr_debug("TX: Channel=%d, TX DR=%d, RX1 DR=%d",
             _params.channel, _params.sys_params.channel_data_rate, rx1_dr);

    _lora_phy->compute_rx_win_params(rx1_dr, MBED_CONF_LORA_DOWNLINK_PREAMBLE_LENGTH,
                                     MBED_CONF_LORA_MAX_SYS_RX_ERROR,
                                     &_params.rx_window1_config);

    _lora_phy->compute_rx_win_params(_params.sys_params.rx2_channel.datarate,
                                     MBED_CONF_LORA_DOWNLINK_PREAMBLE_LENGTH,
                                     MBED_CONF_LORA_MAX_SYS_RX_ERROR,
                                     &_params.rx_window2_config);

    if (!_is_nwk_joined) {
        _params.rx_window1_delay = _params.sys_params.join_accept_delay1
                                   + _params.rx_window1_config.window_offset;
        _params.rx_window2_delay = _params.sys_params.join_accept_delay2
                                   + _params.rx_window2_config.window_offset;
    } else {

        // if the outgoing message is a proprietary message, it doesn't include any
        // standard message formatting except port and MHDR.
        if (_ongoing_tx_msg.type == MCPS_PROPRIETARY) {
            fopts_len = 0;
        } else {
            fopts_len = _mac_commands.get_mac_cmd_length() + _mac_commands.get_repeat_commands_length();
        }

        // A check was performed for validity of FRMPayload in ::prepare_ongoing_tx() API.
        // However, owing to the asynch nature of the send() API, we should check the
        // validity again, as datarate may have changed since we last attempted to transmit.
        if (validate_payload_length(_ongoing_tx_msg.f_buffer_size,
                                    _params.sys_params.channel_data_rate,
                                    fopts_len) == false) {
            tr_error("Allowed FRMPayload = %d, FRMPayload = %d, MAC commands pending = %d",
                     _lora_phy->get_max_payload(_params.sys_params.channel_data_rate,
                                                _params.is_repeater_supported),
                     _ongoing_tx_msg.f_buffer_size, fopts_len);
            return LORAWAN_STATUS_LENGTH_ERROR;
        }
        _params.rx_window1_delay = _params.sys_params.recv_delay1
                                   + _params.rx_window1_config.window_offset;
        _params.rx_window2_delay = _params.sys_params.recv_delay2
                                   + _params.rx_window2_config.window_offset;
    }

    // handle the ack to the server here so that if the sending was cancelled
    // by the user in the backoff period, we would still ack the previous frame.
    if (_params.is_srv_ack_requested) {
        _params.is_srv_ack_requested = false;
    }

    _can_cancel_tx = false;
    status = send_frame_on_channel(_params.channel);

    // If MIC was calculated, remove it from buffer after sending
    // so it can be recalculated and added to the buffer in case
    // of retransmission.
    if (process_mic) {
        _params.tx_buffer_len -= LORAMAC_MFR_LEN;
    }
    return status;
}

void LoRaMac::calculate_backOff(uint8_t channel)
{
    lorawan_time_t elapsed_time = _lora_time.get_elapsed_time(_params.timers.mac_init_time);
    _lora_phy->calculate_backoff(_is_nwk_joined, _params.is_last_tx_join_request, _params.is_dutycycle_on,
                                 channel, elapsed_time, _params.timers.tx_toa);

    // Update aggregated time-off. This must be an assignment and no incremental
    // update as we do only calculate the time-off based on the last transmission
    _params.timers.aggregated_timeoff = (_params.timers.tx_toa * _params.sys_params.aggregated_duty_cycle
                                         - _params.timers.tx_toa);
}

void LoRaMac::reset_frame_counters(void)
{
    _params.ul_frame_counter = 0;
    _params.dl_frame_counter = 0;
}

void LoRaMac::reset_phy_params(void)
{
    _lora_phy->reset_to_default_values(&_params, false);
}

void LoRaMac::reset_mac_parameters(void)
{
    _is_nwk_joined = false;

    _params.adr_ack_counter = 0;

    _params.ul_nb_rep_counter = 0;

    _params.max_ack_timeout_retries = 1;
    _params.ack_timeout_retry_counter = 1;
    _params.is_ack_retry_timeout_expired = false;

    _params.sys_params.max_duty_cycle = 0;
    _params.sys_params.aggregated_duty_cycle = 1;

    _mac_commands.clear_command_buffer();
    _mac_commands.clear_repeat_buffer();

    _params.is_rx_window_enabled = true;

    _params.is_node_ack_requested = false;
    _params.is_srv_ack_requested = false;

    multicast_params_t *cur = _params.multicast_channels;
    while (cur != NULL) {
        cur->dl_frame_counter = 0;
        cur = cur->next;
    }
    _params.channel = 0;
    _params.last_channel_idx = _params.channel;

    _demod_ongoing = false;
}

uint8_t LoRaMac::get_default_tx_datarate()
{
    return _lora_phy->get_default_tx_datarate();
}

void LoRaMac::enable_adaptive_datarate(bool adr_enabled)
{
    _params.sys_params.adr_on = adr_enabled;
}

lorawan_status_t LoRaMac::set_channel_data_rate(uint8_t data_rate)
{
    if (_params.sys_params.adr_on) {
        tr_error("Cannot set data rate. Please turn off ADR first.");
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    if (_lora_phy->verify_tx_datarate(data_rate, false) == true) {
        _params.sys_params.channel_data_rate = data_rate;
    } else {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    return LORAWAN_STATUS_OK;
}

bool LoRaMac::tx_ongoing()
{
    return _ongoing_tx_msg.tx_ongoing;
}

void LoRaMac::set_tx_ongoing(bool ongoing)
{
    _can_cancel_tx = true;
    _ongoing_tx_msg.tx_ongoing = ongoing;
}

void LoRaMac::reset_ongoing_tx(bool reset_pending)
{
    _ongoing_tx_msg.tx_ongoing = false;
    memset(_ongoing_tx_msg.f_buffer, 0, MBED_CONF_LORA_TX_MAX_SIZE);
    _ongoing_tx_msg.f_buffer_size = 0;
    if (reset_pending) {
        _ongoing_tx_msg.pending_size = 0;
    }
}

int16_t LoRaMac::prepare_ongoing_tx(const uint8_t port,
                                    const uint8_t *const data,
                                    uint16_t length,
                                    uint8_t flags,
                                    uint8_t num_retries)
{
    _ongoing_tx_msg.port = port;
    uint8_t max_possible_size = 0;
    uint8_t fopts_len = _mac_commands.get_mac_cmd_length()
                        + _mac_commands.get_repeat_commands_length();

    // Handles unconfirmed messages
    if (flags & MSG_UNCONFIRMED_FLAG) {
        _ongoing_tx_msg.type = MCPS_UNCONFIRMED;
        _ongoing_tx_msg.fport = port;
        _ongoing_tx_msg.nb_trials = 1;
    }

    // Handles confirmed messages
    if (flags & MSG_CONFIRMED_FLAG) {
        _ongoing_tx_msg.type = MCPS_CONFIRMED;
        _ongoing_tx_msg.fport = port;
        _ongoing_tx_msg.nb_trials = num_retries;
    }

    // Handles proprietary messages
    if (flags & MSG_PROPRIETARY_FLAG) {
        _ongoing_tx_msg.type = MCPS_PROPRIETARY;
        _ongoing_tx_msg.fport = port;
        _ongoing_tx_msg.nb_trials = _params.sys_params.nb_trans > 0 ? _params.sys_params.nb_trans : 1;
        // a proprietary frame only includes an MHDR field which contains MTYPE field.
        // Everything else is at the discretion of the implementer
        fopts_len = 0;
    }

    max_possible_size = get_max_possible_tx_size(fopts_len);

    if (max_possible_size > MBED_CONF_LORA_TX_MAX_SIZE) {
        max_possible_size = MBED_CONF_LORA_TX_MAX_SIZE;
    }

    if (max_possible_size < length) {
        tr_info("Cannot transmit %d bytes. Possible TX Size is %d bytes",
                length, max_possible_size);

        _ongoing_tx_msg.pending_size = length - max_possible_size;
        _ongoing_tx_msg.f_buffer_size = max_possible_size;
        memcpy(_ongoing_tx_msg.f_buffer, data, _ongoing_tx_msg.f_buffer_size);
    } else {
        _ongoing_tx_msg.f_buffer_size = length;
        _ongoing_tx_msg.pending_size = 0;
        if (length > 0) {
            memcpy(_ongoing_tx_msg.f_buffer, data, length);
        }
    }

    tr_info("RTS = %u bytes, PEND = %u, Port: %u",
            _ongoing_tx_msg.f_buffer_size, _ongoing_tx_msg.pending_size,
            _ongoing_tx_msg.fport);

    return _ongoing_tx_msg.f_buffer_size;
}

lorawan_status_t LoRaMac::send_ongoing_tx()
{
    lorawan_status_t status;
    _params.is_last_tx_join_request = false;
    int8_t datarate = _params.sys_params.channel_data_rate;

    // This prohibits the data rate going below the minimum value.
    datarate = MAX(datarate, (int8_t)_lora_phy->get_minimum_tx_datarate());

    loramac_mhdr_t machdr;
    machdr.value = 0;

    reset_mcps_confirmation();

    _params.ack_timeout_retry_counter = 1;
    _params.max_ack_timeout_retries = 1;

    if (MCPS_UNCONFIRMED == _ongoing_tx_msg.type) {
        machdr.bits.mtype = FRAME_TYPE_DATA_UNCONFIRMED_UP;
    } else if (_ongoing_tx_msg.type == MCPS_CONFIRMED) {
        machdr.bits.mtype = FRAME_TYPE_DATA_CONFIRMED_UP;
        if (_params.server_type == LW1_1) {
            _params.max_ack_timeout_retries = _params.sys_params.nb_trans;
        } else {
            _params.max_ack_timeout_retries = _ongoing_tx_msg.nb_trials;
        }
    } else if (_ongoing_tx_msg.type == MCPS_PROPRIETARY) {
        machdr.bits.mtype = FRAME_TYPE_PROPRIETARY;
    } else {
        return LORAWAN_STATUS_SERVICE_UNKNOWN;
    }

    if (_params.sys_params.adr_on == false) {
        if (_lora_phy->verify_tx_datarate(datarate, false) == true) {
            _params.sys_params.channel_data_rate = datarate;
        } else {
            return LORAWAN_STATUS_PARAMETER_INVALID;
        }
    }

    status = send(&machdr, _ongoing_tx_msg.fport, _ongoing_tx_msg.f_buffer,
                  _ongoing_tx_msg.f_buffer_size);
    if (status == LORAWAN_STATUS_OK) {
        _mcps_confirmation.req_type = _ongoing_tx_msg.type;
    }

    return status;
}

device_class_t LoRaMac::get_device_class() const
{
    return _device_class;
}

void LoRaMac::set_device_class(const device_class_t &device_class,
                               mbed::Callback<void(void)>rx2_would_be_closure_handler)
{
    _device_class = device_class;
    _rx2_would_be_closure_for_class_c = rx2_would_be_closure_handler;

    _lora_time.init(_rx2_closure_timer_for_class_c, _rx2_would_be_closure_for_class_c);

    if (CLASS_A == _device_class) {
        tr_debug("Changing device class to -> CLASS_A");
        _lora_phy->put_radio_to_sleep();
    } else if (CLASS_C == _device_class) {
        tr_debug("Changing device class to -> CLASS_C");
        _params.is_node_ack_requested = false;
        _lora_phy->put_radio_to_sleep();
        _lora_phy->compute_rx_win_params(_params.sys_params.rx2_channel.datarate,
                                         MBED_CONF_LORA_DOWNLINK_PREAMBLE_LENGTH,
                                         MBED_CONF_LORA_MAX_SYS_RX_ERROR,
                                         &_params.rx_window2_config);
    }

    if (CLASS_C == _device_class) {
        tr_debug("Changing device class to -> CLASS_C");
        open_rx2_window();
    }
}

void LoRaMac::setup_link_check_request()
{
    _mac_commands.add_link_check_req();
}

void LoRaMac::setup_reset_indication()
{
    _mac_commands.add_reset_ind(1);
}

void LoRaMac::setup_rekey_indication()
{
    _mac_commands.add_rekey_ind(1);
}

void LoRaMac::setup_device_mode_indication(uint8_t classType)
{
    _mac_commands.add_device_mode_indication(classType);
}

lorawan_status_t LoRaMac::prepare_join(const lorawan_connect_t *params, bool is_otaa)
{
    if (params) {
        if (is_otaa) {
            if ((params->connection_u.otaa.dev_eui == NULL)
                    || (params->connection_u.otaa.app_eui == NULL)
                    || (params->connection_u.otaa.app_key == NULL)
                    || (params->connection_u.otaa.nwk_key == NULL )
                    || (params->connection_u.otaa.nb_trials == 0)) {
                return LORAWAN_STATUS_PARAMETER_INVALID;
            }
            _params.keys.dev_eui = params->connection_u.otaa.dev_eui;
            _params.keys.app_eui = params->connection_u.otaa.app_eui;
            _params.keys.app_key = params->connection_u.otaa.app_key;
            _params.keys.nwk_key = params->connection_u.otaa.nwk_key;

            if (MBED_CONF_LORA_VERSION == LORAWAN_VERSION_1_0_2) {
                _params.keys.nwk_key = params->connection_u.otaa.app_key;
            }

            uint8_t converted_eui[8];
            memcpy_convert_endianess(converted_eui, _params.keys.dev_eui, 8);

            if (0 != _lora_crypto.compute_join_server_keys(_params.keys.nwk_key, APPKEY_KEY_LENGTH, converted_eui,
                                                           _params.keys.js_intkey, _params.keys.js_enckey)) {
                return LORAWAN_STATUS_CRYPTO_FAIL;
            }

            _params.max_join_request_trials = params->connection_u.otaa.nb_trials;

            if (!_lora_phy->verify_nb_join_trials(params->connection_u.otaa.nb_trials)) {
                _params.max_join_request_trials = MBED_CONF_LORA_NB_TRIALS;
            }

            _params.join_request_trial_counter = 0;

            reset_mac_parameters();

            reset_frame_counters();

            reset_phy_params();

            _params.sys_params.channel_data_rate =
                _lora_phy->get_alternate_DR(_params.join_request_trial_counter + 1);
        } else {
            if ((params->connection_u.abp.dev_addr == 0)
                    || (params->connection_u.abp.nwk_id == 0)
                    || (params->connection_u.abp.nwk_skey == NULL)
                    || (params->connection_u.abp.app_skey == NULL)) {
                return LORAWAN_STATUS_PARAMETER_INVALID;
            }

            if (MBED_CONF_LORA_VERSION == LORAWAN_VERSION_1_1
                && ((params->connection_u.abp.snwk_sintkey == NULL)
                || (params->connection_u.abp.nwk_senckey == NULL))) {
                return LORAWAN_STATUS_PARAMETER_INVALID;
            }

            _params.net_id = params->connection_u.abp.nwk_id;
            _params.dev_addr = params->connection_u.abp.dev_addr;

            memcpy(_params.keys.nwk_skey, params->connection_u.abp.nwk_skey,
                   sizeof(_params.keys.nwk_skey));

            memcpy(_params.keys.app_skey, params->connection_u.abp.app_skey,
                   sizeof(_params.keys.app_skey));

            if (MBED_CONF_LORA_VERSION == LORAWAN_VERSION_1_1) {
                memcpy(_params.keys.snwk_sintkey, params->connection_u.abp.snwk_sintkey,
                       sizeof(_params.keys.snwk_sintkey));

                memcpy(_params.keys.nwk_senckey, params->connection_u.abp.nwk_senckey,
                       sizeof(_params.keys.nwk_senckey));

                _params.server_type = LW1_1;
            } else {
                memcpy(_params.keys.snwk_sintkey, params->connection_u.abp.nwk_skey,
                       sizeof(_params.keys.snwk_sintkey));

                memcpy(_params.keys.nwk_senckey, params->connection_u.abp.nwk_skey,
                       sizeof(_params.keys.nwk_senckey));

                _params.server_type = LW1_0_2;
            }
        }
    } else {
#if MBED_CONF_LORA_OVER_THE_AIR_ACTIVATION
        const static uint8_t dev_eui[] = MBED_CONF_LORA_DEVICE_EUI;
        const static uint8_t app_eui[] = MBED_CONF_LORA_APPLICATION_EUI;
        const static uint8_t app_key[] = MBED_CONF_LORA_APPLICATION_KEY;
        const static uint8_t nwk_key[] = MBED_CONF_LORA_NETWORK_KEY;

        _params.keys.app_eui = const_cast<uint8_t *>(app_eui);
        _params.keys.dev_eui = const_cast<uint8_t *>(dev_eui);
        _params.keys.app_key = const_cast<uint8_t *>(app_key);
        _params.keys.nwk_key = const_cast<uint8_t *>(nwk_key);
        _params.max_join_request_trials = MBED_CONF_LORA_NB_TRIALS;

        if( MBED_CONF_LORA_VERSION == LORAWAN_VERSION_1_0_2 ) {
            _params.keys.nwk_key = _params.keys.app_key;
        }

        uint8_t converted_eui[8];
        memcpy_convert_endianess(converted_eui, _params.keys.dev_eui, 8);

        if (0 != _lora_crypto.compute_join_server_keys(_params.keys.nwk_key, APPKEY_KEY_LENGTH, converted_eui,
                                                       _params.keys.js_intkey, _params.keys.js_enckey)) {
            return LORAWAN_STATUS_CRYPTO_FAIL;
        }

        // Reset variable JoinRequestTrials
        _params.join_request_trial_counter = 0;

        reset_mac_parameters();
        reset_frame_counters();
        reset_phy_params();

        _params.sys_params.channel_data_rate =
            _lora_phy->get_alternate_DR(_params.join_request_trial_counter + 1);

#else
        const static uint8_t nwk_skey[] = MBED_CONF_LORA_NWKSKEY;
        const static uint8_t app_skey[] = MBED_CONF_LORA_APPSKEY;
        const static uint8_t snwk_sintkey[] = MBED_CONF_LORA_SNWKSINTKEY;
        const static uint8_t nwk_senckey[] = MBED_CONF_LORA_NWKSENCKEY;

        _params.net_id = (MBED_CONF_LORA_DEVICE_ADDRESS & LORAWAN_NETWORK_ID_MASK) >> 25;
        _params.dev_addr = MBED_CONF_LORA_DEVICE_ADDRESS;

        memcpy(_params.keys.nwk_skey, nwk_skey, sizeof(_params.keys.nwk_skey));
        memcpy(_params.keys.app_skey, app_skey, sizeof(_params.keys.app_skey));
        if (MBED_CONF_LORA_VERSION == LORAWAN_VERSION_1_1) {
            memcpy(_params.keys.snwk_sintkey, snwk_sintkey, sizeof(_params.keys.snwk_sintkey));
            memcpy(_params.keys.nwk_senckey, nwk_senckey, sizeof(_params.keys.nwk_senckey));
            _params.server_type = LW1_1;
        } else {
            memcpy(_params.keys.snwk_sintkey, nwk_skey, sizeof(_params.keys.snwk_sintkey));
            memcpy(_params.keys.nwk_senckey, nwk_skey, sizeof(_params.keys.nwk_senckey));
            _params.server_type = LW1_0_2;
        }
#endif
    }

    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMac::join(bool is_otaa)
{
    if (!is_otaa) {
        set_nwk_joined(true);
        return LORAWAN_STATUS_OK;
    }

    _params.join_request_type = JOIN_REQUEST;

    return send_join_request();
}

lorawan_status_t LoRaMac::rejoin(join_req_type_t rejoin_type, bool is_forced, uint8_t datarate)
{
    _params.join_request_type = rejoin_type;
    _params.rejoin_forced = is_forced;
    _params.forced_datarate = datarate;

    return send_join_request();
}

lorawan_status_t LoRaMac::prepare_frame(loramac_mhdr_t *machdr,
                                        loramac_frame_ctrl_t *fctrl,
                                        const uint8_t fport,
                                        const void *fbuffer,
                                        uint16_t fbuffer_size)
{
    uint16_t i;
    uint8_t pkt_header_len = 0;
    uint32_t mic = 0;
    const void *payload = fbuffer;
    uint8_t frame_port = fport;
    lorawan_status_t status = LORAWAN_STATUS_OK;

    _params.tx_buffer_len = 0;

    _params.is_node_ack_requested = false;

    if (fbuffer == NULL) {
        fbuffer_size = 0;
    }

    _params.tx_buffer_len = fbuffer_size;

    _params.tx_buffer[pkt_header_len++] = machdr->value;

    switch (machdr->bits.mtype) {

        case FRAME_TYPE_JOIN_REQ:

            _params.tx_buffer_len = pkt_header_len;
            memcpy_convert_endianess(_params.tx_buffer + _params.tx_buffer_len,
                                     _params.keys.app_eui, 8);
            _params.tx_buffer_len += 8;
            memcpy_convert_endianess(_params.tx_buffer + _params.tx_buffer_len,
                                     _params.keys.dev_eui, 8);
            _params.tx_buffer_len += 8;

            _params.dev_nonce = _lora_phy->get_radio_rng();

            _params.tx_buffer[_params.tx_buffer_len++] = _params.dev_nonce & 0xFF;
            _params.tx_buffer[_params.tx_buffer_len++] = (_params.dev_nonce >> 8) & 0xFF;

            if (0 != _lora_crypto.compute_join_frame_mic(_params.tx_buffer,
                                                         _params.tx_buffer_len & 0xFF,
                                                         _params.keys.nwk_key,
                                                         APPKEY_KEY_LENGTH,
                                                         &mic)) {
                return LORAWAN_STATUS_CRYPTO_FAIL;
            }

            _params.tx_buffer[_params.tx_buffer_len++] = mic & 0xFF;
            _params.tx_buffer[_params.tx_buffer_len++] = (mic >> 8) & 0xFF;
            _params.tx_buffer[_params.tx_buffer_len++] = (mic >> 16) & 0xFF;
            _params.tx_buffer[_params.tx_buffer_len++] = (mic >> 24) & 0xFF;

            break;
        case FRAME_TYPE_REJOIN_REQUEST:
            _params.tx_buffer_len = pkt_header_len;
            _params.tx_buffer[_params.tx_buffer_len++] = (uint8_t)_params.join_request_type;

            if (_params.join_request_type == REJOIN_REQUEST_TYPE0 || _params.join_request_type == REJOIN_REQUEST_TYPE2) {
                _params.tx_buffer[_params.tx_buffer_len++] = _params.net_id & 0xFF;
                _params.tx_buffer[_params.tx_buffer_len++] = (_params.net_id >> 8) & 0xFF;
                _params.tx_buffer[_params.tx_buffer_len++] = (_params.net_id >> 16) & 0xFF;

                memcpy_convert_endianess(_params.tx_buffer + _params.tx_buffer_len,
                                         _params.keys.dev_eui, 8);
                _params.tx_buffer_len += 8;

                _params.tx_buffer[_params.tx_buffer_len++] = _params.RJcount0 & 0xFF;
                _params.tx_buffer[_params.tx_buffer_len++] = (_params.RJcount0 >> 8) & 0xFF;
            } else { //_params.join_request_type == REJOIN_REQUEST_TYPE1
                memcpy_convert_endianess(_params.tx_buffer + _params.tx_buffer_len,
                                         _params.keys.app_eui, 8);
                _params.tx_buffer_len += 8;
                memcpy_convert_endianess(_params.tx_buffer + _params.tx_buffer_len,
                                         _params.keys.dev_eui, 8);
                _params.tx_buffer_len += 8;

                _params.tx_buffer[_params.tx_buffer_len++] = _params.RJcount1 & 0xFF;
                _params.tx_buffer[_params.tx_buffer_len++] = (_params.RJcount1 >> 8) & 0xFF;
            }
            if (0 != _lora_crypto.compute_join_frame_mic(_params.tx_buffer,
                                                         _params.tx_buffer_len & 0xFF,
                                                         _params.keys.snwk_sintkey,
                                                         APPKEY_KEY_LENGTH,
                                                         &mic)) {
                return LORAWAN_STATUS_CRYPTO_FAIL;
            }
            _params.tx_buffer[_params.tx_buffer_len++] = mic & 0xFF;
            _params.tx_buffer[_params.tx_buffer_len++] = (mic >> 8) & 0xFF;
            _params.tx_buffer[_params.tx_buffer_len++] = (mic >> 16) & 0xFF;
            _params.tx_buffer[_params.tx_buffer_len++] = (mic >> 24) & 0xFF;

            break;
        case FRAME_TYPE_DATA_CONFIRMED_UP:
            _params.is_node_ack_requested = true;
        //Intentional fallthrough
        case FRAME_TYPE_DATA_UNCONFIRMED_UP: {
            if (!_is_nwk_joined) {
                return LORAWAN_STATUS_NO_NETWORK_JOINED;
            }

            if (_params.sys_params.adr_on) {
                if (_lora_phy->get_next_ADR(true,
                                            _params.sys_params.channel_data_rate,
                                            _params.sys_params.channel_tx_power,
                                            _params.adr_ack_counter)) {
                    fctrl->bits.adr_ack_req = 1;
                }
            }

            if (_params.is_srv_ack_requested == true) {
                tr_debug("Acking to NS");
                fctrl->bits.ack = 1;
            }

            _params.tx_buffer[pkt_header_len++] = (_params.dev_addr) & 0xFF;
            _params.tx_buffer[pkt_header_len++] = (_params.dev_addr >> 8) & 0xFF;
            _params.tx_buffer[pkt_header_len++] = (_params.dev_addr >> 16) & 0xFF;
            _params.tx_buffer[pkt_header_len++] = (_params.dev_addr >> 24) & 0xFF;

            _params.tx_buffer[pkt_header_len++] = fctrl->value;

            _params.tx_buffer[pkt_header_len++] = _params.ul_frame_counter & 0xFF;
            _params.tx_buffer[pkt_header_len++] = (_params.ul_frame_counter >> 8)
                                                  & 0xFF;

            _mac_commands.copy_repeat_commands_to_buffer();

            const uint8_t mac_commands_len = _mac_commands.get_mac_cmd_length();

            if ((payload != NULL) && (_params.tx_buffer_len > 0)) {
                if (mac_commands_len <= LORA_MAC_COMMAND_MAX_FOPTS_LENGTH) {
                    fctrl->bits.fopts_len += mac_commands_len;

                    // Update FCtrl field with new value of OptionsLength
                    _params.tx_buffer[0x05] = fctrl->value;
                    const uint8_t *buffer = _mac_commands.get_mac_commands_buffer();

                    if (_params.server_type == LW1_1) {
                        if (0 != _lora_crypto.encrypt_payload(buffer, mac_commands_len,
                                                              _params.keys.nwk_senckey,
                                                              sizeof(_params.keys.nwk_senckey) * 8,
                                                              _params.dev_addr, UP_LINK,
                                                              _params.ul_frame_counter,
                                                              FCNT_UP,
                                                              FOPTS,
                                                              &_params.tx_buffer[pkt_header_len],
                                                              _params.server_type)) {
                            status = LORAWAN_STATUS_CRYPTO_FAIL;
                        }
                        pkt_header_len += mac_commands_len;
                    } else {
                        for (i = 0; i < mac_commands_len; i++) {
                            _params.tx_buffer[pkt_header_len++] = buffer[i];
                        }
                    }
                } else {
                    _params.tx_buffer_len = mac_commands_len;
                    payload = _mac_commands.get_mac_commands_buffer();
                    frame_port = 0;
                }
            } else {
                _params.tx_buffer_len = mac_commands_len;
                payload = _mac_commands.get_mac_commands_buffer();
                frame_port = 0;
            }

            _mac_commands.parse_mac_commands_to_repeat();

            // We always add Port Field. Spec leaves it optional.
            _params.tx_buffer[pkt_header_len++] = frame_port;

            if ((payload != NULL) && (_params.tx_buffer_len > 0)) {

                uint8_t *key = _params.keys.app_skey;
                uint32_t key_length = sizeof(_params.keys.app_skey) * 8;
                if (frame_port == 0) {
                    key = _params.keys.nwk_senckey;
                    key_length = sizeof(_params.keys.nwk_senckey) * 8;
                }

                if (0 != _lora_crypto.encrypt_payload((uint8_t *) payload,
                                                      _params.tx_buffer_len,
                                                      key, key_length,
                                                      _params.dev_addr, UP_LINK,
                                                      _params.ul_frame_counter,
                                                      FCNT_UP,
                                                      FRMPAYLOAD,
                                                      &_params.tx_buffer[pkt_header_len],
                                                      _params.server_type)) {
                    status = LORAWAN_STATUS_CRYPTO_FAIL;
                }
            }

            _params.tx_buffer_len = pkt_header_len + _params.tx_buffer_len;
        }
        break;
        case FRAME_TYPE_PROPRIETARY:
            if ((fbuffer != NULL) && (_params.tx_buffer_len > 0)) {
                memcpy(_params.tx_buffer + pkt_header_len, (uint8_t *) fbuffer,
                       _params.tx_buffer_len);
                _params.tx_buffer_len = pkt_header_len + _params.tx_buffer_len;
            }
            break;
        default:
            status = LORAWAN_STATUS_SERVICE_UNKNOWN;
    }

    tr_debug("Frame prepared to send at port %u", frame_port);
    return status;
}

lorawan_status_t LoRaMac::send_frame_on_channel(uint8_t channel)
{
    tx_config_params_t tx_config;
    int8_t tx_power = 0;

    tx_config.channel = channel;
    if (_params.rejoin_forced) {
        tx_config.datarate = _params.forced_datarate;
    } else {
        tx_config.datarate = _params.sys_params.channel_data_rate;
    }
    tx_config.tx_power = _params.sys_params.channel_tx_power;
    tx_config.max_eirp = _params.sys_params.max_eirp;
    tx_config.antenna_gain = _params.sys_params.antenna_gain;
    tx_config.pkt_len = _params.tx_buffer_len;

    _lora_phy->tx_config(&tx_config, &tx_power, &_params.timers.tx_toa);

    _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
    _mcps_confirmation.data_rate = _params.sys_params.channel_data_rate;
    _mcps_confirmation.tx_power = tx_power;
    _mcps_confirmation.channel = channel;

    _mcps_confirmation.tx_toa = _params.timers.tx_toa;

    if (!_is_nwk_joined) {
        _params.join_request_trial_counter++;
    }

    _lora_phy->handle_send(_params.tx_buffer, _params.tx_buffer_len);

    return LORAWAN_STATUS_OK;
}

void LoRaMac::reset_mcps_confirmation()
{
    memset((uint8_t *) &_mcps_confirmation, 0, sizeof(_mcps_confirmation));
    _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
}

void LoRaMac::reset_mcps_indication()
{
    memset((uint8_t *) &_mcps_indication, 0, sizeof(_mcps_indication));
    _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
}

LoRaWANTimeHandler *LoRaMac::get_lora_time()
{
    return &_lora_time;
}

lorawan_status_t LoRaMac::initialize(EventQueue *queue,
                                     mbed::Callback<void(void)>scheduling_failure_handler)
{
    _lora_time.activate_timer_subsystem(queue);
    _lora_phy->initialize(&_lora_time);

    _ev_queue = queue;
    _scheduling_failure_handler = scheduling_failure_handler;
    _rx2_closure_timer_for_class_c.callback = NULL;
    _rx2_closure_timer_for_class_c.timer_id = -1;

    _channel_plan.activate_channelplan_subsystem(_lora_phy);

    _device_class = CLASS_A;

    _params.join_request_trial_counter = 0;
    _params.max_join_request_trials = 1;
    _params.is_repeater_supported = false;

    _params.timers.aggregated_last_tx_time = 0;
    _params.timers.aggregated_timeoff = 0;

    _lora_phy->reset_to_default_values(&_params, true);
    _params.sys_params.nb_trans = 1;

    reset_mac_parameters();
    reset_frame_counters();

    srand(_lora_phy->get_radio_rng());

    _params.is_nwk_public = MBED_CONF_LORA_PUBLIC_NETWORK;
    _lora_phy->setup_public_network_mode(_params.is_nwk_public);
    _lora_phy->put_radio_to_sleep();

    _lora_time.init(_params.timers.backoff_timer,
                    mbed::callback(this, &LoRaMac::on_backoff_timer_expiry));
    _lora_time.init(_params.timers.rx_window1_timer,
                    mbed::callback(this, &LoRaMac::open_rx1_window));
    _lora_time.init(_params.timers.rx_window2_timer,
                    mbed::callback(this, &LoRaMac::open_rx2_window));
    _lora_time.init(_params.timers.ack_timeout_timer,
                    mbed::callback(this, &LoRaMac::on_ack_timeout_timer_event));

    _params.timers.mac_init_time = _lora_time.get_current_time();

    _params.sys_params.adr_on = MBED_CONF_LORA_ADR_ON;
    _params.sys_params.channel_data_rate = _lora_phy->get_default_max_tx_datarate();

    return LORAWAN_STATUS_OK;
}

void LoRaMac::disconnect()
{
    _lora_time.stop(_params.timers.backoff_timer);
    _lora_time.stop(_params.timers.rx_window1_timer);
    _lora_time.stop(_params.timers.rx_window2_timer);
    _lora_time.stop(_params.timers.ack_timeout_timer);

    _lora_phy->put_radio_to_sleep();

    _is_nwk_joined = false;
    _params.is_ack_retry_timeout_expired = false;
    _params.is_rx_window_enabled = true;
    _params.is_node_ack_requested = false;
    _params.is_srv_ack_requested = false;

    _mac_commands.clear_command_buffer();
    _mac_commands.clear_repeat_buffer();

    reset_mcps_confirmation();
    reset_mcps_indication();
}

uint8_t LoRaMac::get_max_possible_tx_size(uint8_t fopts_len)
{
    uint8_t max_possible_payload_size = 0;
    uint8_t allowed_frm_payload_size = 0;

    int8_t datarate = _params.sys_params.channel_data_rate;
    int8_t tx_power = _params.sys_params.channel_tx_power;
    uint32_t adr_ack_counter = _params.adr_ack_counter;

    if (_params.sys_params.adr_on) {
        // Just query the information. We do not want to apply them into use
        // at this point.
        _lora_phy->get_next_ADR(false, datarate, tx_power, adr_ack_counter);
    }

    allowed_frm_payload_size = _lora_phy->get_max_payload(datarate,
                                                          _params.is_repeater_supported);

    if (allowed_frm_payload_size >= fopts_len) {
        max_possible_payload_size = allowed_frm_payload_size - fopts_len;
    } else {
        max_possible_payload_size = allowed_frm_payload_size;
        _mac_commands.clear_command_buffer();
        _mac_commands.clear_repeat_buffer();
    }

    return max_possible_payload_size;
}

bool LoRaMac::nwk_joined()
{
    return _is_nwk_joined;
}

void LoRaMac::set_nwk_joined(bool joined)
{
    _is_nwk_joined = joined;
}

lorawan_status_t LoRaMac::add_channel_plan(const lorawan_channelplan_t &plan)
{
    if (tx_ongoing()) {
        return LORAWAN_STATUS_BUSY;
    }

    return _channel_plan.set_plan(plan);
}

lorawan_status_t LoRaMac::remove_channel_plan()
{
    if (tx_ongoing()) {
        return LORAWAN_STATUS_BUSY;
    }

    return _channel_plan.remove_plan();
}

lorawan_status_t LoRaMac::get_channel_plan(lorawan_channelplan_t &plan)
{
    return _channel_plan.get_plan(plan, _lora_phy->get_phy_channels());
}

lorawan_status_t LoRaMac::remove_single_channel(uint8_t id)
{
    if (tx_ongoing()) {
        return LORAWAN_STATUS_BUSY;
    }

    return _channel_plan.remove_single_channel(id);
}

lorawan_status_t LoRaMac::multicast_channel_link(multicast_params_t *channel_param)
{
    if (channel_param == NULL) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }
    if (tx_ongoing()) {
        return LORAWAN_STATUS_BUSY;
    }

    channel_param->dl_frame_counter = 0;

    if (_params.multicast_channels == NULL) {
        _params.multicast_channels = channel_param;
    } else {
        multicast_params_t *cur = _params.multicast_channels;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = channel_param;
    }

    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMac::multicast_channel_unlink(multicast_params_t *channel_param)
{
    if (channel_param == NULL) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    if (tx_ongoing()) {
        return LORAWAN_STATUS_BUSY;
    }

    if (_params.multicast_channels != NULL) {
        if (_params.multicast_channels == channel_param) {
            _params.multicast_channels = channel_param->next;
        } else {
            multicast_params_t *cur = _params.multicast_channels;

            while (cur->next && cur->next != channel_param) {
                cur = cur->next;
            }

            if (cur->next) {
                cur->next = channel_param->next;
            }
        }
        channel_param->next = NULL;
    }

    return LORAWAN_STATUS_OK;
}

void LoRaMac::bind_phy(LoRaPHY &phy)
{
    _lora_phy = &phy;
}

uint8_t LoRaMac::get_QOS_level()
{
    if (_prev_qos_level != _params.sys_params.nb_trans) {
        _prev_qos_level = _params.sys_params.nb_trans;
    }

    return _params.sys_params.nb_trans;
}

uint8_t LoRaMac::get_prev_QOS_level()
{
    return _prev_qos_level;
}

server_type_t LoRaMac::get_server_type()
{
    return _params.server_type;
}

uint8_t LoRaMac::get_current_adr_ack_limit()
{
    return _lora_phy->get_adr_ack_limit();
}

void LoRaMac::get_rejoin_parameters(uint32_t& max_time, uint32_t& max_count)
{
    max_time = _lora_phy->get_rejoin_max_time();
    max_count = _lora_phy->get_rejoin_max_count();
}

lorawan_status_t LoRaMac::calculate_userdata_mic()
{
    lorawan_status_t status = LORAWAN_STATUS_OK;
    uint32_t mic = 0;
    uint32_t mic2 = 0;
    uint32_t args = 0;

    if (0 != _lora_crypto.compute_mic(_params.tx_buffer, _params.tx_buffer_len,
                                      _params.keys.nwk_skey,
                                      sizeof(_params.keys.nwk_skey) * 8,
                                      args, _params.dev_addr,
                                      UP_LINK, _params.ul_frame_counter, &mic)) {
        status = LORAWAN_STATUS_CRYPTO_FAIL;
    }

    if (_params.server_type == LW1_1) {
        if (_params.is_srv_ack_requested) {
            args = _params.counterForAck;
        }
        args |= ((uint8_t) _params.sys_params.channel_data_rate) << 16;
        args |= _params.channel << 24;

        if (0 != _lora_crypto.compute_mic(_params.tx_buffer, _params.tx_buffer_len,
                                          _params.keys.snwk_sintkey,
                                          sizeof(_params.keys.snwk_sintkey) * 8,
                                          args, _params.dev_addr,
                                          UP_LINK, _params.ul_frame_counter, &mic2)) {
            status = LORAWAN_STATUS_CRYPTO_FAIL;
        }

        _params.tx_buffer[_params.tx_buffer_len + 0] = mic2 & 0xFF;
        _params.tx_buffer[_params.tx_buffer_len + 1] = (mic2 >> 8) & 0xFF;
        _params.tx_buffer[_params.tx_buffer_len + 2] = mic & 0xFF;
        _params.tx_buffer[_params.tx_buffer_len + 3] = (mic >> 8) & 0xFF;
    } else {
        _params.tx_buffer[_params.tx_buffer_len + 0] = mic & 0xFF;
        _params.tx_buffer[_params.tx_buffer_len + 1] = (mic >> 8) & 0xFF;
        _params.tx_buffer[_params.tx_buffer_len + 2] = (mic >> 16) & 0xFF;
        _params.tx_buffer[_params.tx_buffer_len + 3] = (mic >> 24) & 0xFF;
    }

    _params.tx_buffer_len += LORAMAC_MFR_LEN;

    return status;
}
