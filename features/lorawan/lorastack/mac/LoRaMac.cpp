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
#include "LoRaMacCrypto.h"

#if defined(FEATURE_COMMON_PAL)
#include "mbed_trace.h"
#define TRACE_GROUP "LMAC"
#else
#define tr_debug(...) (void(0)) //dummies if feature common pal is not added
#define tr_info(...)  (void(0)) //dummies if feature common pal is not added
#define tr_error(...) (void(0)) //dummies if feature common pal is not added
#endif //defined(FEATURE_COMMON_PAL)

using namespace events;

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
 * Check the MAC layer state every MAC_STATE_CHECK_TIMEOUT in ms.
 */
#define MAC_STATE_CHECK_TIMEOUT                     1000

/*!
 * The maximum number of times the MAC layer tries to get an acknowledge.
 */
#define MAX_ACK_RETRIES                             8

/*!
 * The frame direction definition for uplink communications.
 */
#define UP_LINK                                     0

/*!
 * The frame direction definition for downlink communications.
 */
#define DOWN_LINK                                   1

/**
 * A mask for the network ID.
 */
#define LORAWAN_NETWORK_ID_MASK                     ( uint32_t )0xFE000000


LoRaMac::LoRaMac()
    : _lora_phy(_lora_time), mac_commands(), _is_nwk_joined(false)
{
    //radio_events_t RadioEvents;
    _params.keys.dev_eui = NULL;
    _params.keys.app_eui = NULL;
    _params.keys.app_key = NULL;

    memset(_params.keys.nwk_skey, 0, sizeof(_params.keys.nwk_skey));
    memset(_params.keys.app_skey, 0, sizeof(_params.keys.app_skey));
    memset(&_ongoing_tx_msg, 0, sizeof(_ongoing_tx_msg));

    _params.dev_nonce = 0;
    _params.net_id = 0;
    _params.dev_addr = 0;
    _params.buffer_pkt_len = 0;
    _params.payload_length = 0;
    _params.ul_frame_counter = 0;
    _params.dl_frame_counter = 0;
    _params.is_ul_frame_counter_fixed = false;
    _params.is_rx_window_enabled = true;
    _params.adr_ack_counter = 0;
    _params.is_node_ack_requested = false;
    _params.is_srv_ack_requested = false;
    _params.ul_nb_rep_counter = 0;
    _params.timers.mac_init_time = 0;
    _params.mac_state = LORAMAC_IDLE;
    _params.max_ack_timeout_retries = 1;
    _params.ack_timeout_retry_counter = 1;
    _params.is_ack_retry_timeout_expired = false;
    _params.timers.tx_toa = 0;

    _params.multicast_channels = NULL;

    _params.sys_params.adr_on = false;
    _params.sys_params.max_duty_cycle = 0;

    mac_primitives = NULL;
    ev_queue = NULL;
}

LoRaMac::~LoRaMac()
{
}


/***************************************************************************
 * ISRs - Handlers                                                         *
 **************************************************************************/
void LoRaMac::handle_tx_done(void)
{
    const int ret = ev_queue->call(this, &LoRaMac::on_radio_tx_done);
    MBED_ASSERT(ret != 0);
    (void)ret;
}

void LoRaMac::handle_rx_done(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
    const int ret = ev_queue->call(this, &LoRaMac::on_radio_rx_done, payload, size, rssi, snr);
    MBED_ASSERT(ret != 0);
    (void)ret;
}

void LoRaMac::handle_rx_error(void)
{
    const int ret = ev_queue->call(this, &LoRaMac::on_radio_rx_error);
    MBED_ASSERT(ret != 0);
    (void)ret;
}

void LoRaMac::handle_rx_timeout(void)
{
    const int ret = ev_queue->call(this, &LoRaMac::on_radio_rx_timeout);
    MBED_ASSERT(ret != 0);
    (void)ret;
}

void LoRaMac::handle_tx_timeout(void)
{
    const int ret = ev_queue->call(this, &LoRaMac::on_radio_tx_timeout);
    MBED_ASSERT(ret != 0);
    (void)ret;
}

void LoRaMac::handle_cad_done(bool cad)
{
    //TODO Not implemented yet
    //const int ret = ev_queue->call(this, &LoRaMac::OnRadioCadDone, cad);
    //MBED_ASSERT(ret != 0);
    //(void)ret;
}

void LoRaMac::handle_fhss_change_channel(uint8_t cur_channel)
{
    // TODO Not implemented yet
    //const int ret = ev_queue->call(this, &LoRaMac::OnRadioFHSSChangeChannel, cur_channel);
    //MBED_ASSERT(ret != 0);
    //(void)ret;
}

/***************************************************************************
 * Radio event callbacks - delegated to Radio driver                       *
 **************************************************************************/
void LoRaMac::on_radio_tx_done( void )
{
    lorawan_time_t cur_time = _lora_time.get_current_time( );

    if (_device_class != CLASS_C) {
        _lora_phy.put_radio_to_sleep();
    } else {
        open_continuous_rx2_window();
    }

    if(_params.is_rx_window_enabled == true) {
        _lora_time.start(_params.timers.rx_window1_timer, _params.rx_window1_delay);

        if (_device_class != CLASS_C) {
            _lora_time.start(_params.timers.rx_window2_timer, _params.rx_window2_delay);
        }

        if ((_device_class == CLASS_C ) ||
            (_params.is_node_ack_requested == true)) {
            _lora_time.start(_params.timers.ack_timeout_timer,
                             _params.rx_window2_delay + _lora_phy.get_ack_timeout());
        }
    } else {
        _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_OK;
        _mlme_confirmation.status = LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT;

        if (_params.flags.value == 0) {
            _params.flags.bits.mcps_req = 1;
        }

        _params.flags.bits.mac_done = 1;
    }

    if ((_params.flags.bits.mlme_req == 1) &&
        (_mlme_confirmation.req_type == MLME_JOIN)) {
        _params.is_last_tx_join_request = true;
    } else {
        _params.is_last_tx_join_request = false;
    }

    _params.last_channel_idx = _params.channel;

    _lora_phy.set_last_tx_done(_params.channel, _is_nwk_joined, cur_time);

    _params.timers.aggregated_last_tx_time = cur_time;

    if (_params.is_node_ack_requested == false) {
        _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_OK;
        _params.ul_nb_rep_counter++;
    }
}

void LoRaMac::prepare_rx_done_abort(void)
{
    _params.mac_state |= LORAMAC_RX_ABORT;

    if (_params.is_node_ack_requested) {
        const int ret = ev_queue->call(this, &LoRaMac::on_ack_timeout_timer_event);
        MBED_ASSERT(ret != 0);
        (void)ret;
    }

    _params.flags.bits.mcps_ind = 1;
    _params.flags.bits.mac_done = 1;

    _lora_time.start(_params.timers.mac_state_check_timer, 1);
}

void LoRaMac::on_radio_rx_done(uint8_t *payload, uint16_t size, int16_t rssi,
                               int8_t snr)
{
    loramac_mhdr_t mac_hdr;
    loramac_frame_ctrl_t fctrl;

    uint8_t pkt_header_len = 0;
    uint32_t address = 0;
    uint8_t app_payload_start_index = 0;
    uint8_t frame_len = 0;
    uint32_t mic = 0;
    uint32_t mic_rx = 0;

    uint16_t sequence_counter = 0;
    uint16_t sequence_counter_prev = 0;
    uint16_t sequence_counter_diff = 0;
    uint32_t downlink_counter = 0;

    multicast_params_t *cur_multicast_params = NULL;
    uint8_t *nwk_skey = _params.keys.nwk_skey;
    uint8_t *app_skey = _params.keys.app_skey;

    uint8_t multicast = 0;

    bool is_mic_ok = false;

    _mcps_confirmation.ack_received = false;
    _mcps_indication.rssi = rssi;
    _mcps_indication.snr = snr;
    _mcps_indication.rx_slot = _params.rx_slot;
    _mcps_indication.port = 0;
    _mcps_indication.multicast = 0;
    _mcps_indication.fpending_status = 0;
    _mcps_indication.buffer = NULL;
    _mcps_indication.buffer_size = 0;
    _mcps_indication.is_data_recvd = false;
    _mcps_indication.is_ack_recvd = false;
    _mcps_indication.dl_frame_counter = 0;
    _mcps_indication.type = MCPS_UNCONFIRMED;
    _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_OK;

    if (_device_class != CLASS_C) {
        _lora_phy.put_radio_to_sleep();
    }

    _lora_time.stop( _params.timers.rx_window2_timer );

    mac_hdr.value = payload[pkt_header_len++];

    switch (mac_hdr.bits.mtype) {
        case FRAME_TYPE_JOIN_ACCEPT:
            if (_is_nwk_joined) {
                _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                prepare_rx_done_abort();
                return;
            }

            if (0 != decrypt_join_frame(payload + 1, size - 1,
                                        _params.keys.app_key,
                                        _params.payload + 1)) {
                _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
                return;
            }

            _params.payload[0] = mac_hdr.value;

            if (0 != compute_join_frame_mic(_params.payload, size - LORAMAC_MFR_LEN,
                                           _params.keys.app_key, &mic)) {
                _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
                return;
            }

            mic_rx |= (uint32_t) _params.payload[size - LORAMAC_MFR_LEN];
            mic_rx |= ((uint32_t) _params.payload[size - LORAMAC_MFR_LEN + 1] << 8);
            mic_rx |= ((uint32_t) _params.payload[size - LORAMAC_MFR_LEN + 2] << 16);
            mic_rx |= ((uint32_t) _params.payload[size - LORAMAC_MFR_LEN + 3] << 24);

            if (mic_rx == mic) {

                if (0 != compute_skeys_for_join_frame(_params.keys.app_key,
                                                 _params.payload + 1,
                                                 _params.dev_nonce,
                                                 _params.keys.nwk_skey,
                                                 _params.keys.app_skey)) {
                    _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
                    return;
                }

                _params.net_id = (uint32_t) _params.payload[4];
                _params.net_id |= ((uint32_t) _params.payload[5] << 8);
                _params.net_id |= ((uint32_t) _params.payload[6] << 16);

                _params.dev_addr = (uint32_t) _params.payload[7];
                _params.dev_addr |= ((uint32_t) _params.payload[8] << 8);
                _params.dev_addr |= ((uint32_t) _params.payload[9] << 16);
                _params.dev_addr |= ((uint32_t) _params.payload[10] << 24);

                _params.sys_params.rx1_dr_offset = (_params.payload[11] >> 4) & 0x07;
                _params.sys_params.rx2_channel.datarate = _params.payload[11] & 0x0F;

                _params.sys_params.recv_delay1 = (_params.payload[12] & 0x0F);

                if (_params.sys_params.recv_delay1 == 0) {
                    _params.sys_params.recv_delay1 = 1;
                }

                _params.sys_params.recv_delay1 *= 1000;
                _params.sys_params.recv_delay2 = _params.sys_params.recv_delay1 + 1000;

                // Size of the regular payload is 12. Plus 1 byte MHDR and 4 bytes MIC
                _lora_phy.apply_cf_list(&_params.payload[13], size - 17);

                _mlme_confirmation.status = LORAMAC_EVENT_INFO_STATUS_OK;
                _is_nwk_joined = true;
            } else {
                _mlme_confirmation.status = LORAMAC_EVENT_INFO_STATUS_JOIN_FAIL;
            }

            break;

        case FRAME_TYPE_DATA_CONFIRMED_DOWN:
        case FRAME_TYPE_DATA_UNCONFIRMED_DOWN:
            {
                uint8_t value = _lora_phy.get_max_payload(_mcps_indication.rx_datarate, _params.is_repeater_supported);

                if (MAX(0, (int16_t) ((int16_t)size - (int16_t)LORA_MAC_FRMPAYLOAD_OVERHEAD )) > (int32_t)value) {
                    _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                    prepare_rx_done_abort();
                    return;
                }

                address = payload[pkt_header_len++];
                address |= ((uint32_t)payload[pkt_header_len++] << 8);
                address |= ((uint32_t)payload[pkt_header_len++] << 16);
                address |= ((uint32_t)payload[pkt_header_len++] << 24);

                if (address != _params.dev_addr) {

                    cur_multicast_params = _params.multicast_channels;

                    while (cur_multicast_params != NULL) {
                        if (address == cur_multicast_params->address) {
                            multicast = 1;
                            nwk_skey = cur_multicast_params->nwk_skey;
                            app_skey = cur_multicast_params->app_skey;
                            downlink_counter = cur_multicast_params->dl_frame_counter;
                            break;
                        }

                        cur_multicast_params = cur_multicast_params->next;
                    }

                    if (multicast == 0) {
                        // We are not the destination of this frame.
                        _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_ADDRESS_FAIL;
                        prepare_rx_done_abort();
                        return;
                    }
                } else {
                    multicast = 0;
                    nwk_skey = _params.keys.nwk_skey;
                    app_skey = _params.keys.app_skey;
                    downlink_counter = _params.dl_frame_counter;
                }

                fctrl.value = payload[pkt_header_len++];

                sequence_counter = (uint16_t )payload[pkt_header_len++];
                sequence_counter |= (uint16_t)payload[pkt_header_len++] << 8;

                app_payload_start_index = 8 + fctrl.bits.fopts_len;

                mic_rx |= (uint32_t)payload[size - LORAMAC_MFR_LEN];
                mic_rx |= ((uint32_t)payload[size - LORAMAC_MFR_LEN + 1] << 8);
                mic_rx |= ((uint32_t)payload[size - LORAMAC_MFR_LEN + 2] << 16);
                mic_rx |= ((uint32_t)payload[size - LORAMAC_MFR_LEN + 3] << 24);

                sequence_counter_prev = (uint16_t)downlink_counter;
                sequence_counter_diff = (sequence_counter - sequence_counter_prev);

                if (sequence_counter_diff < (1 << 15)) {
                    downlink_counter += sequence_counter_diff;
                    compute_mic(payload, size - LORAMAC_MFR_LEN, nwk_skey,
                                      address, DOWN_LINK, downlink_counter, &mic);
                    if (mic_rx == mic) {
                        is_mic_ok = true;
                    }
                } else {
                    uint32_t  downlink_counter_tmp = downlink_counter + 0x10000 + (int16_t)sequence_counter_diff;
                    compute_mic(payload, size - LORAMAC_MFR_LEN, nwk_skey,
                                      address, DOWN_LINK, downlink_counter_tmp, &mic);

                    if (mic_rx == mic ) {
                        is_mic_ok = true;
                        downlink_counter = downlink_counter_tmp;
                    }
                }

                if (sequence_counter_diff >= _lora_phy.get_maximum_frame_counter_gap()) {
                    _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOSS;
                    _mcps_indication.dl_frame_counter = downlink_counter;
                    prepare_rx_done_abort( );
                    return;
                }

                if (is_mic_ok == true) {
                    _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_OK;
                    _mcps_indication.multicast = multicast;
                    _mcps_indication.fpending_status = fctrl.bits.fpending;
                    _mcps_indication.buffer = NULL;
                    _mcps_indication.buffer_size = 0;
                    _mcps_indication.dl_frame_counter = downlink_counter;

                    _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_OK;

                    _params.adr_ack_counter = 0;
                    mac_commands.clear_repeat_buffer();

                    if (multicast == 1) {
                        _mcps_indication.type = MCPS_MULTICAST;

                        if ((cur_multicast_params->dl_frame_counter == downlink_counter) &&
                            (cur_multicast_params->dl_frame_counter != 0)) {

                            _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED;
                            _mcps_indication.dl_frame_counter = downlink_counter;
                            prepare_rx_done_abort();

                            return;
                        }

                        cur_multicast_params->dl_frame_counter = downlink_counter;

                    } else {

                        if (mac_hdr.bits.mtype == FRAME_TYPE_DATA_CONFIRMED_DOWN) {
                            _params.is_srv_ack_requested = true;
                            _mcps_indication.type = MCPS_CONFIRMED;

                            if ((_params.dl_frame_counter == downlink_counter ) &&
                                (_params.dl_frame_counter != 0)) {
                                // Duplicated confirmed downlink. Skip indication.
                                // In this case, the MAC layer shall accept the MAC commands
                                // which are included in the downlink retransmission.
                                // It should not provide the same frame to the application
                                // layer again. The MAC layer accepts the acknowledgement.
                                _params.flags.bits.mcps_ind_skip = 1;
                            }
                        } else {
                            _params.is_srv_ack_requested = false;
                            _mcps_indication.type = MCPS_UNCONFIRMED;

                            if ((_params.dl_frame_counter == downlink_counter) &&
                                (_params.dl_frame_counter != 0)) {
                                _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED;
                                _mcps_indication.dl_frame_counter = downlink_counter;
                                prepare_rx_done_abort();
                                return;
                            }
                        }
                        _params.dl_frame_counter = downlink_counter;
                    }

                    // This must be done before parsing the payload and the MAC commands.
                    // We need to reset the MacCommandsBufferIndex here, since we need
                    // to take retransmissions and repetitions into account. Error cases
                    // will be handled in function OnMacStateCheckTimerEvent.
                    if (_mcps_confirmation.req_type == MCPS_CONFIRMED) {
                        if (fctrl.bits.ack == 1) {
                            mac_commands.clear_command_buffer();
                            _mcps_confirmation.ack_received = fctrl.bits.ack;
                            _mcps_indication.is_ack_recvd = fctrl.bits.ack;
                        }
                    } else {
                        mac_commands.clear_command_buffer();
                    }

                    if (((size - 4) - app_payload_start_index) > 0) {
                        uint8_t port = payload[app_payload_start_index++];
                        frame_len = (size - 4) - app_payload_start_index;

                        _mcps_indication.port = port;

                        if (port == 0) {
                            if (fctrl.bits.fopts_len == 0) {
                                if (0 != decrypt_payload(payload + app_payload_start_index,
                                                               frame_len,
                                                               nwk_skey,
                                                               address,
                                                               DOWN_LINK,
                                                               downlink_counter,
                                                               _params.payload)) {
                                    _mcps_indication.status =  LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
                                }

                                if (LORAWAN_STATUS_OK != mac_commands.process_mac_commands(
                                            _params.payload, 0, frame_len, snr,
                                            _mlme_confirmation, _params.sys_params, _lora_phy)) {
                                    _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                                } else if (mac_commands.has_sticky_mac_cmd()) {
                                    set_mlme_schedule_ul_indication();
                                    mac_commands.clear_sticky_mac_cmd();
                                }
                            } else {
                                _params.flags.bits.mcps_ind_skip = 1;
                                _mcps_confirmation.ack_received = false;
                                _mcps_indication.is_ack_recvd = false;
                            }
                        } else {
                            if (fctrl.bits.fopts_len > 0) {
                                // Decode Options field MAC commands. Omit the fPort.
                                if (LORAWAN_STATUS_OK != mac_commands.process_mac_commands(
                                            payload, 8, app_payload_start_index - 1, snr,
                                            _mlme_confirmation, _params.sys_params, _lora_phy )) {
                                    _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                                } else if (mac_commands.has_sticky_mac_cmd()) {
                                    set_mlme_schedule_ul_indication();
                                    mac_commands.clear_sticky_mac_cmd();
                                }
                            }

                            if (0 != decrypt_payload(payload + app_payload_start_index,
                                                           frame_len,
                                                           app_skey,
                                                           address,
                                                           DOWN_LINK,
                                                           downlink_counter,
                                                           _params.payload)) {
                                _mcps_indication.status =  LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
                            }

                            _mcps_indication.buffer = _params.payload;
                            _mcps_indication.buffer_size = frame_len;
                            _mcps_indication.is_data_recvd = true;
                        }
                    } else {
                        if (fctrl.bits.fopts_len > 0) {
                            if (LORAWAN_STATUS_OK != mac_commands.process_mac_commands(
                                        payload, 8, app_payload_start_index, snr,
                                        _mlme_confirmation,
                                        _params.sys_params, _lora_phy)) {
                                _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                            } else if (mac_commands.has_sticky_mac_cmd()) {
                                set_mlme_schedule_ul_indication();
                                mac_commands.clear_sticky_mac_cmd();
                            }
                        }
                    }

                    // Provide always an indication, skip the callback to the user application,
                    // in case of a confirmed downlink retransmission.
                    _params.flags.bits.mcps_ind = 1;
                } else {
                    _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_MIC_FAIL;

                    prepare_rx_done_abort( );
                    return;
                }
            }
            break;

        case FRAME_TYPE_PROPRIETARY:
            {
                memcpy(_params.payload, &payload[pkt_header_len], size);

                _mcps_indication.type = MCPS_PROPRIETARY;
                _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_OK;
                _mcps_indication.buffer = _params.payload;
                _mcps_indication.buffer_size = size - pkt_header_len;

                _params.flags.bits.mcps_ind = 1;
                break;
            }
        default:
            _mcps_indication.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
            prepare_rx_done_abort();
            break;
    }

    check_to_disable_ack_timeout(_params.is_node_ack_requested, _device_class, _mcps_confirmation.ack_received,
                                 _params.ack_timeout_retry_counter, _params.max_ack_timeout_retries );

    if(_params.timers.ack_timeout_timer.timer_id == 0) {
        _params.flags.bits.mac_done = 1;

        _lora_time.start(_params.timers.mac_state_check_timer, 1);
    }
}

void LoRaMac::on_radio_tx_timeout( void )
{
    if (_device_class != CLASS_C) {
        _lora_phy.put_radio_to_sleep();
    } else {
        open_continuous_rx2_window();
    }

    _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT;
    _mlme_confirmation.status = LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT;

    _params.flags.bits.mac_done = 1;
}

void LoRaMac::on_radio_rx_error( void )
{
    if (_device_class != CLASS_C) {
        _lora_phy.put_radio_to_sleep();
    }

    if (_params.rx_slot == RX_SLOT_WIN_1) {
        if (_params.is_node_ack_requested == true) {
            _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_RX1_ERROR;
        }

        _mlme_confirmation.status = LORAMAC_EVENT_INFO_STATUS_RX1_ERROR;

        if (_device_class != CLASS_C) {
            if (_lora_time.get_elapsed_time(_params.timers.aggregated_last_tx_time) >= _params.rx_window2_delay) {
                _lora_time.stop(_params.timers.rx_window2_timer);
                _params.flags.bits.mac_done = 1;
            }
        }
    } else {
        if (_params.is_node_ack_requested == true) {
            _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_RX2_ERROR;
        }

        _mlme_confirmation.status = LORAMAC_EVENT_INFO_STATUS_RX2_ERROR;

        if (_device_class != CLASS_C) {
            _params.flags.bits.mac_done = 1;
        }
    }

    if (_device_class == CLASS_C) {
       open_continuous_rx2_window();
    }
}

void LoRaMac::on_radio_rx_timeout(void)
{
    if (_device_class != CLASS_C) {
        _lora_phy.put_radio_to_sleep();
    }

    if (_params.rx_slot == RX_SLOT_WIN_1) {
        if (_params.is_node_ack_requested == true) {
            _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_RX1_TIMEOUT;
        }
        _mlme_confirmation.status = LORAMAC_EVENT_INFO_STATUS_RX1_TIMEOUT;

        if (_device_class != CLASS_C) {
            if (_lora_time.get_elapsed_time(_params.timers.aggregated_last_tx_time ) >= _params.rx_window2_delay) {
                _lora_time.stop(_params.timers.rx_window2_timer);
                _params.flags.bits.mac_done = 1;
            }
        }
    } else {
        if (_params.is_node_ack_requested == true) {
            _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT;
        }

        _mlme_confirmation.status = LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT;

        if (_device_class != CLASS_C) {
            _params.flags.bits.mac_done = 1;
        }
    }

    if (_device_class == CLASS_C) {
       open_continuous_rx2_window();
    }
}

/***************************************************************************
 * Timer event callbacks - deliberated locally                             *
 **************************************************************************/
void LoRaMac::on_mac_state_check_timer_event(void)
{
    bool tx_timeout = false;

    _lora_time.stop(_params.timers.mac_state_check_timer);

    if (_params.flags.bits.mac_done == 1) {

        if ((_params.mac_state & LORAMAC_RX_ABORT) == LORAMAC_RX_ABORT) {
            _params.mac_state &= ~LORAMAC_RX_ABORT;
            _params.mac_state &= ~LORAMAC_TX_RUNNING;
        }

        if ((_params.flags.bits.mlme_req == 1) || (_params.flags.bits.mcps_req == 1)) {

            if ((_mcps_confirmation.status == LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT) ||
                (_mlme_confirmation.status == LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT)) {
                _params.mac_state &= ~LORAMAC_TX_RUNNING;
                mac_commands.clear_command_buffer();
                _mcps_confirmation.nb_retries = _params.ack_timeout_retry_counter;
                _mcps_confirmation.ack_received = false;
                _mcps_confirmation.tx_toa = 0;
                tx_timeout = true;
            }
        }

        if ((_params.is_node_ack_requested == false) && (tx_timeout == false)) {
            if ((_params.flags.bits.mlme_req == 1) || ((_params.flags.bits.mcps_req == 1))) {
                if ((_params.flags.bits.mlme_req == 1) && (_mlme_confirmation.req_type == MLME_JOIN)) {
                    _mlme_confirmation.nb_retries = _params.join_request_trial_counter;

                    if (_mlme_confirmation.status == LORAMAC_EVENT_INFO_STATUS_OK) {
                        // Node joined successfully
                        _params.ul_frame_counter = 0;
                        _params.ul_nb_rep_counter = 0;
                        _params.mac_state &= ~LORAMAC_TX_RUNNING;
                    } else {
                        if (_params.join_request_trial_counter >= _params.max_join_request_trials) {
                            _params.mac_state &= ~LORAMAC_TX_RUNNING;
                        } else {
                            _params.flags.bits.mac_done = 0;
                            // Schedule a retry
                            const int ret = ev_queue->call(this, &LoRaMac::on_tx_delayed_timer_event);
                            MBED_ASSERT(ret != 0);
                            (void)ret;

                        }
                    }
                } else {
                    if ((_params.ul_nb_rep_counter >= _params.sys_params.retry_num) ||
                        (_params.flags.bits.mcps_ind == 1)) {
                        if (_params.flags.bits.mcps_ind == 0) {
                            // Maximum repetitions without downlink.
                            // Only process the case when the MAC did not receive a downlink.
                            mac_commands.clear_command_buffer();
                            _params.adr_ack_counter++;
                        }

                        _params.ul_nb_rep_counter = 0;

                        if (_params.is_ul_frame_counter_fixed == false) {
                            _params.ul_frame_counter++;
                        }

                        _params.mac_state &= ~LORAMAC_TX_RUNNING;
                    } else {
                        _params.flags.bits.mac_done = 0;
                        // Schedule a retry
                        const int ret = ev_queue->call(this, &LoRaMac::on_tx_delayed_timer_event);
                        MBED_ASSERT(ret != 0);
                        (void)ret;
                    }
                }
            }
        }

        if (_params.flags.bits.mcps_ind == 1) {
            if ((_mcps_confirmation.ack_received == true) ||
                (_params.ack_timeout_retry_counter > _params.max_ack_timeout_retries)) {
                _params.is_ack_retry_timeout_expired = false;
                _params.is_node_ack_requested = false;
                if (_params.is_ul_frame_counter_fixed == false) {
                    _params.ul_frame_counter++;
                }
                _mcps_confirmation.nb_retries = _params.ack_timeout_retry_counter;

                _params.mac_state &= ~LORAMAC_TX_RUNNING;
            }
        }

        if ((_params.is_ack_retry_timeout_expired == true) &&
            ((_params.mac_state & LORAMAC_TX_DELAYED) == 0)) {

            // Retransmissions procedure for confirmed uplinks
            _params.is_ack_retry_timeout_expired = false;
            if ((_params.ack_timeout_retry_counter < _params.max_ack_timeout_retries) &&
                (_params.ack_timeout_retry_counter <= MAX_ACK_RETRIES)) {

                _params.ack_timeout_retry_counter++;

                if ((_params.ack_timeout_retry_counter % 2) == 1) {
                    _params.sys_params.channel_data_rate = _lora_phy.get_next_lower_tx_datarate(
                                                           _params.sys_params.channel_data_rate);
                }

                // Try to send the frame again
                if (schedule_tx() == LORAWAN_STATUS_OK) {
                    _params.flags.bits.mac_done = 0;
                } else {
                    // The DR is not applicable for the payload size
                    _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_TX_DR_PAYLOAD_SIZE_ERROR;

                    mac_commands.clear_command_buffer();
                    _params.mac_state &= ~LORAMAC_TX_RUNNING;
                    _params.is_node_ack_requested = false;
                    _mcps_confirmation.ack_received = false;
                    _mcps_confirmation.nb_retries = _params.ack_timeout_retry_counter;
                    _mcps_confirmation.data_rate = _params.sys_params.channel_data_rate;

                    if (_params.is_ul_frame_counter_fixed == false) {
                        _params.ul_frame_counter++;
                    }
                }
            } else {
                _lora_phy.restore_default_channels();

                _params.mac_state &= ~LORAMAC_TX_RUNNING;

                mac_commands.clear_command_buffer();
                _params.is_node_ack_requested = false;
                _mcps_confirmation.ack_received = false;
                _mcps_confirmation.nb_retries = _params.ack_timeout_retry_counter;

                if (_params.is_ul_frame_counter_fixed == false) {
                    _params.ul_frame_counter++;
                }
            }
        }
    }

    // Handle reception for Class B and Class C
    if ((_params.mac_state & LORAMAC_RX) == LORAMAC_RX) {
        _params.mac_state &= ~LORAMAC_RX;
    }

    if (_params.mac_state == LORAMAC_IDLE) {
        if (_params.flags.bits.mcps_req == 1) {
            _params.flags.bits.mcps_req = 0;
            mac_primitives->mcps_confirm(&_mcps_confirmation);
        }

        if (_params.flags.bits.mlme_req == 1) {
            _params.flags.bits.mlme_req = 0;
            mac_primitives->mlme_confirm(&_mlme_confirmation);
        }

        if (mac_commands.is_sticky_mac_command_pending() == true) {
            set_mlme_schedule_ul_indication();
        }

        _params.flags.bits.mac_done = 0;
    } else {
        _lora_time.start(_params.timers.mac_state_check_timer, MAC_STATE_CHECK_TIMEOUT);
    }

    if (_params.flags.bits.mcps_ind == 1) {
        _params.flags.bits.mcps_ind = 0;

        if (_device_class == CLASS_C) {
            open_continuous_rx2_window();
        }

        if (_params.flags.bits.mcps_ind_skip == 0) {
            mac_primitives->mcps_indication(&_mcps_indication);
        }

        _params.flags.bits.mcps_ind_skip = 0;
    }

    if (_params.flags.bits.mlme_ind == 1) {
        _params.flags.bits.mlme_ind = 0;
        mac_primitives->mlme_indication(&_mlme_indication);
    }
}

void LoRaMac::on_tx_delayed_timer_event(void)
{
    loramac_mhdr_t mac_hdr;
    loramac_frame_ctrl_t fctrl;

    lorawan_status_t status = LORAWAN_STATUS_OK;

    _lora_time.stop(_params.timers.tx_delayed_timer);
    _params.mac_state &= ~LORAMAC_TX_DELAYED;

    if ((_params.flags.bits.mlme_req == 1 ) &&
        (_mlme_confirmation.req_type == MLME_JOIN)) {

        reset_mac_parameters();

        _params.sys_params.channel_data_rate = _lora_phy.get_alternate_DR(_params.join_request_trial_counter + 1);

        mac_hdr.value = 0;
        mac_hdr.bits.mtype = FRAME_TYPE_JOIN_REQ;

        fctrl.value = 0;
        fctrl.bits.adr = _params.sys_params.adr_on;

        /* In case of join request retransmissions, the stack must prepare
         * the frame again, because the network server keeps track of the random
         * LoRaMacDevNonce values to prevent reply attacks. */
        status = prepare_frame(&mac_hdr, &fctrl, 0, NULL, 0);
    }

    if (status == LORAWAN_STATUS_OK) {
        schedule_tx();
    } else {
        tr_error("Delayed TX: PrepareFrame returned error %d", status);
    }
}

void LoRaMac::on_rx_window1_timer_event(void)
{
    _lora_time.stop(_params.timers.rx_window1_timer);
    _params.rx_slot = RX_SLOT_WIN_1;

    _params.rx_window1_config.channel = _params.channel;
    _params.rx_window1_config.dr_offset = _params.sys_params.rx1_dr_offset;
    _params.rx_window1_config.dl_dwell_time = _params.sys_params.downlink_dwell_time;
    _params.rx_window1_config.is_repeater_supported = _params.is_repeater_supported;
    _params.rx_window1_config.is_rx_continuous = false;
    _params.rx_window1_config.rx_slot = _params.rx_slot;

    if (_device_class == CLASS_C) {
        _lora_phy.put_radio_to_standby();
    }

    _lora_phy.rx_config(&_params.rx_window1_config,
                        (int8_t*) &_mcps_indication.rx_datarate);

    _lora_phy.setup_rx_window(_params.rx_window1_config.is_rx_continuous,
                              _params.sys_params.max_rx_win_time);
}

void LoRaMac::on_rx_window2_timer_event(void)
{
    _lora_time.stop(_params.timers.rx_window2_timer);

    _params.rx_window2_config.channel = _params.channel;
    _params.rx_window2_config.frequency = _params.sys_params.rx2_channel.frequency;
    _params.rx_window2_config.dl_dwell_time = _params.sys_params.downlink_dwell_time;
    _params.rx_window2_config.is_repeater_supported = _params.is_repeater_supported;
    _params.rx_window2_config.rx_slot = RX_SLOT_WIN_2;

    _params.rx_window2_config.is_rx_continuous = true;

    if (_device_class != CLASS_C) {
        _params.rx_window2_config.is_rx_continuous = false;
    }

    if (_lora_phy.rx_config(&_params.rx_window2_config,
                            (int8_t*) &_mcps_indication.rx_datarate) == true) {

        _lora_phy.setup_rx_window(_params.rx_window2_config.is_rx_continuous,
                                  _params.sys_params.max_rx_win_time);

        _params.rx_slot = RX_SLOT_WIN_2;
    }
}

void LoRaMac::check_to_disable_ack_timeout(bool node_ack_requested,
                                           device_class_t dev_class,
                                           bool ack_received,
                                           uint8_t ack_timeout_retries_counter,
                                           uint8_t ack_timeout_retries)
{
    // There are three cases where we need to stop the AckTimeoutTimer:
    if( node_ack_requested == false ) {
        if( dev_class == CLASS_C ) {
            // FIRST CASE
            // We have performed an unconfirmed uplink in class c mode
            // and have received a downlink in RX1 or RX2.
            _lora_time.stop(_params.timers.ack_timeout_timer);
        }
    } else {
        if( ack_received == 1 ) {
            // SECOND CASE
            // We received an ACK for previously sent confirmable message
            _lora_time.stop(_params.timers.ack_timeout_timer);
        } else {
            // THIRD CASE
            // Max number of retries exceeded for confirmable message
            if( ack_timeout_retries_counter > ack_timeout_retries ) {
                _lora_time.stop(_params.timers.ack_timeout_timer);
            }
        }
    }
}

void LoRaMac::on_ack_timeout_timer_event(void)
{
    _lora_time.stop(_params.timers.ack_timeout_timer);

    if (_params.is_node_ack_requested == true) {
        _params.is_ack_retry_timeout_expired = true;
        _params.mac_state &= ~LORAMAC_ACK_REQ;
    }
    if (_device_class == CLASS_C) {
        _params.flags.bits.mac_done = 1;
    }
}

bool LoRaMac::validate_payload_length(uint8_t length, int8_t datarate,
                                      uint8_t fopts_len)
{
    uint16_t max_value = 0;
    uint16_t payloadSize = 0;

    max_value = _lora_phy.get_max_payload(datarate, _params.is_repeater_supported);

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
    _params.flags.bits.mlme_ind = 1;
}

// This is not actual transmission. It just schedules a message in response
// to MCPS request
lorawan_status_t LoRaMac::send(loramac_mhdr_t *machdr, uint8_t fport,
                               void *fbuffer, uint16_t fbuffer_size)
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

lorawan_status_t LoRaMac::schedule_tx(void)
{
    lorawan_time_t dutyCycleTimeOff = 0;
    channel_selection_params_t nextChan;

    if (_params.sys_params.max_duty_cycle == 255) {
        return LORAWAN_STATUS_DEVICE_OFF;
    }

    if (_params.sys_params.max_duty_cycle == 0) {
        _params.timers.aggregated_timeoff = 0;
    }

    calculate_backOff(_params.last_channel_idx);

    nextChan.aggregate_timeoff = _params.timers.aggregated_timeoff;
    nextChan.current_datarate = _params.sys_params.channel_data_rate;
    _params.is_dutycycle_on = MBED_CONF_LORA_DUTY_CYCLE_ON;
    nextChan.dc_enabled = _params.is_dutycycle_on;
    nextChan.joined = _is_nwk_joined;
    nextChan.last_aggregate_tx_time = _params.timers.aggregated_last_tx_time;

    lorawan_status_t status = _lora_phy.set_next_channel(&nextChan,
                                                         &_params.channel,
                                                         &dutyCycleTimeOff,
                                                         &_params.timers.aggregated_timeoff);
    switch (status) {
        case LORAWAN_STATUS_NO_CHANNEL_FOUND:
        case LORAWAN_STATUS_NO_FREE_CHANNEL_FOUND:
            return status;
        case LORAWAN_STATUS_DUTYCYCLE_RESTRICTED:
            if (dutyCycleTimeOff != 0) {
                tr_debug("Next Transmission in %lu ms", dutyCycleTimeOff);
                _params.mac_state |= LORAMAC_TX_DELAYED;
                _lora_time.start(_params.timers.tx_delayed_timer, dutyCycleTimeOff);
            }
            return LORAWAN_STATUS_OK;
        default:
            break;
    }
    tr_debug("Next Channel Idx=%d, DR=%d", _params.channel, nextChan.current_datarate);

    uint8_t dr_offset = _lora_phy.apply_DR_offset(_params.sys_params.channel_data_rate,
                                                  _params.sys_params.rx1_dr_offset);

    _lora_phy.compute_rx_win_params(dr_offset, _params.sys_params.min_rx_symb,
                                    _params.sys_params.max_sys_rx_error,
                                    &_params.rx_window1_config);

    _lora_phy.compute_rx_win_params(_params.sys_params.rx2_channel.datarate,
                                    _params.sys_params.min_rx_symb,
                                    _params.sys_params.max_sys_rx_error,
                                    &_params.rx_window2_config);

    if (!_is_nwk_joined) {
        _params.rx_window1_delay = _params.sys_params.join_accept_delay1
                + _params.rx_window1_config.window_offset;
        _params.rx_window2_delay = _params.sys_params.join_accept_delay2
                + _params.rx_window2_config.window_offset;
    } else {
        if (validate_payload_length(_params.payload_length,
                                    _params.sys_params.channel_data_rate,
                                    mac_commands.get_mac_cmd_length()) == false) {
            return LORAWAN_STATUS_LENGTH_ERROR;
        }
        _params.rx_window1_delay = _params.sys_params.recv_delay1
                + _params.rx_window1_config.window_offset;
        _params.rx_window2_delay = _params.sys_params.recv_delay2
                + _params.rx_window2_config.window_offset;
    }

    return send_frame_on_channel(_params.channel);
}

void LoRaMac::calculate_backOff(uint8_t channel)
{
    lorawan_time_t elapsed_time = _lora_time.get_elapsed_time(_params.timers.mac_init_time);

    _params.is_dutycycle_on = MBED_CONF_LORA_DUTY_CYCLE_ON;

    _lora_phy.calculate_backoff(_is_nwk_joined, _params.is_last_tx_join_request, _params.is_dutycycle_on,
                                channel, elapsed_time, _params.timers.tx_toa);

    // Update aggregated time-off. This must be an assignment and no incremental
    // update as we do only calculate the time-off based on the last transmission
    _params.timers.aggregated_timeoff = (_params.timers.tx_toa * _params.sys_params.aggregated_duty_cycle
                                         - _params.timers.tx_toa);
}

void LoRaMac::reset_mac_parameters(void)
{
    _is_nwk_joined = false;

    _params.ul_frame_counter = 0;
    _params.dl_frame_counter = 0;
    _params.adr_ack_counter = 0;

    _params.ul_nb_rep_counter = 0;

    _params.max_ack_timeout_retries = 1;
    _params.ack_timeout_retry_counter = 1;
    _params.is_ack_retry_timeout_expired = false;

    _params.sys_params.max_duty_cycle = 0;
    _params.sys_params.aggregated_duty_cycle = 1;

    mac_commands.clear_command_buffer();
    mac_commands.clear_repeat_buffer();
    mac_commands.clear_mac_commands_in_next_tx();

    _params.is_rx_window_enabled = true;

    _lora_phy.reset_to_default_values(&_params, false);

    _params.is_node_ack_requested = false;
    _params.is_srv_ack_requested = false;

    multicast_params_t *cur = _params.multicast_channels;
    while (cur != NULL) {
        cur->dl_frame_counter = 0;
        cur = cur->next;
    }
    _params.channel = 0;
    _params.last_channel_idx = _params.channel;
}

void LoRaMac::open_continuous_rx2_window (void)
{
    const int ret = ev_queue->call(this, &LoRaMac::on_rx_window2_timer_event);
    MBED_ASSERT(ret != 0);
    (void)ret;

    _params.rx_slot = RX_SLOT_WIN_CLASS_C;
}

uint8_t LoRaMac::get_default_tx_datarate()
{
    return _lora_phy.get_default_tx_datarate();
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

    if (_lora_phy.verify_tx_datarate(data_rate, false) == true) {
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

int16_t LoRaMac::prepare_ongoing_tx(uint8_t port, const uint8_t* data,
                                    uint16_t length, uint8_t flags, uint8_t num_retries)
{
    _ongoing_tx_msg.port = port;

    uint8_t max_possible_size = get_max_possible_tx_size(length);

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

    // Handles all unconfirmed messages, including proprietary and multicast
    if ((flags & MSG_FLAG_MASK) == MSG_UNCONFIRMED_FLAG
            || (flags & MSG_FLAG_MASK) == MSG_UNCONFIRMED_MULTICAST
            || (flags & MSG_FLAG_MASK) == MSG_UNCONFIRMED_PROPRIETARY) {

         _ongoing_tx_msg.type = MCPS_UNCONFIRMED;
         _ongoing_tx_msg.fport = port;
         _ongoing_tx_msg.nb_trials = 1;
    }

    // Handles all confirmed messages, including proprietary and multicast
    if ((flags & MSG_FLAG_MASK) == MSG_CONFIRMED_FLAG
            || (flags & MSG_FLAG_MASK) == MSG_CONFIRMED_MULTICAST
            || (flags & MSG_FLAG_MASK) == MSG_CONFIRMED_PROPRIETARY) {

        _ongoing_tx_msg.type = MCPS_CONFIRMED;
        _ongoing_tx_msg.fport = port;
        _ongoing_tx_msg.nb_trials = num_retries;
    }

    tr_info("RTS = %u bytes, PEND = %u", _ongoing_tx_msg.f_buffer_size, _ongoing_tx_msg.pending_size);
    return _ongoing_tx_msg.f_buffer_size;
}

lorawan_status_t LoRaMac::send_ongoing_tx()
{
    lorawan_status_t status;

    if (_params.mac_state != LORAMAC_IDLE) {
        return LORAWAN_STATUS_BUSY;
    }

    int8_t datarate = _params.sys_params.channel_data_rate;

    // TODO: The comment is different than the code???
    // Apply the minimum possible datarate.
    // Some regions have limitations for the minimum datarate.
    datarate = MAX(datarate, (int8_t)_lora_phy.get_minimum_tx_datarate());

    loramac_mhdr_t machdr;
    machdr.value = 0;

    reset_mcps_confirmation();

    _params.ack_timeout_retry_counter = 1;
    _params.max_ack_timeout_retries = 1;

    if (MCPS_UNCONFIRMED == _ongoing_tx_msg.type) {
        machdr.bits.mtype = FRAME_TYPE_DATA_UNCONFIRMED_UP;
    } else if (_ongoing_tx_msg.type == MCPS_CONFIRMED) {
        machdr.bits.mtype = FRAME_TYPE_DATA_CONFIRMED_UP;
        _params.max_ack_timeout_retries = _ongoing_tx_msg.nb_trials;
    } else if ( _ongoing_tx_msg.type == MCPS_PROPRIETARY) {
        //TODO: Is this dead code currently??? Nobody sets this type
        machdr.bits.mtype = FRAME_TYPE_PROPRIETARY;
    } else {
        return LORAWAN_STATUS_SERVICE_UNKNOWN;
    }

    if (_params.sys_params.adr_on == false) {
        if (_lora_phy.verify_tx_datarate(datarate, false) == true) {
            _params.sys_params.channel_data_rate = datarate;
        } else {
            return LORAWAN_STATUS_PARAMETER_INVALID;
        }
    }

    status = send(&machdr, _ongoing_tx_msg.fport, _ongoing_tx_msg.f_buffer,
                           _ongoing_tx_msg.f_buffer_size);
    if (status == LORAWAN_STATUS_OK) {
        _mcps_confirmation.req_type = _ongoing_tx_msg.type;
        _params.flags.bits.mcps_req = 1;
    } else {
        _params.is_node_ack_requested = false;
    }

    return status;
}

device_class_t LoRaMac::get_device_class() const
{
    return _device_class;
}

void LoRaMac::set_device_class(const device_class_t& device_class)
{
    _device_class = device_class;

    if (CLASS_A == _device_class) {
        _lora_phy.put_radio_to_sleep();
    } else if (CLASS_C == _device_class) {
        _params.is_node_ack_requested = false;
        _lora_phy.put_radio_to_sleep();
        _lora_phy.compute_rx_win_params(
                    _params.sys_params.rx2_channel.datarate,
                    _params.sys_params.min_rx_symb,
                    _params.sys_params.max_sys_rx_error,
                    &_params.rx_window2_config);
    }
    if (CLASS_C == _device_class) {
        open_continuous_rx2_window();
    }
}

void LoRaMac::setup_link_check_request()
{
    reset_mlme_confirmation();

    _mlme_confirmation.req_type = MLME_LINK_CHECK;
    _params.flags.bits.mlme_req = 1;
    mac_commands.add_mac_command(MOTE_MAC_LINK_CHECK_REQ, 0, 0);
}

lorawan_status_t LoRaMac::prepare_join(const lorawan_connect_t *params, bool is_otaa)
{
    if (params) {
        if (is_otaa) {
            if ((params->connection_u.otaa.dev_eui == NULL) ||
                (params->connection_u.otaa.app_eui == NULL) ||
                (params->connection_u.otaa.app_key == NULL) ||
                (params->connection_u.otaa.nb_trials == 0)) {
                return LORAWAN_STATUS_PARAMETER_INVALID;
            }
            _params.keys.dev_eui = params->connection_u.otaa.dev_eui;
            _params.keys.app_eui = params->connection_u.otaa.app_eui;
            _params.keys.app_key = params->connection_u.otaa.app_key;
            _params.max_join_request_trials = params->connection_u.otaa.nb_trials;

            if (!_lora_phy.verify_nb_join_trials(params->connection_u.otaa.nb_trials)) {
                // Value not supported, get default
                _params.max_join_request_trials = MBED_CONF_LORA_NB_TRIALS;
            }
            // Reset variable JoinRequestTrials
            _params.join_request_trial_counter = 0;

            reset_mac_parameters();

            _params.sys_params.channel_data_rate =
                    _lora_phy.get_alternate_DR(_params.join_request_trial_counter + 1);
        } else {
            _params.net_id = params->connection_u.abp.nwk_id;
            _params.dev_addr = params->connection_u.abp.dev_addr;

            memcpy(_params.keys.nwk_skey, params->connection_u.abp.nwk_skey,
                   sizeof(_params.keys.nwk_skey));

            memcpy(_params.keys.app_skey, params->connection_u.abp.app_skey,
                   sizeof(_params.keys.app_skey));
        }
    } else {
#if MBED_CONF_LORA_OVER_THE_AIR_ACTIVATION
    const static uint8_t dev_eui[] = MBED_CONF_LORA_DEVICE_EUI;
    const static uint8_t app_eui[] = MBED_CONF_LORA_APPLICATION_EUI;
    const static uint8_t app_key[] = MBED_CONF_LORA_APPLICATION_KEY;

    _params.keys.app_eui = const_cast<uint8_t *>(app_eui);
    _params.keys.dev_eui = const_cast<uint8_t *>(dev_eui);
    _params.keys.app_key = const_cast<uint8_t *>(app_key);
    _params.max_join_request_trials = MBED_CONF_LORA_NB_TRIALS;

    // Reset variable JoinRequestTrials
    _params.join_request_trial_counter = 0;

    reset_mac_parameters();

    _params.sys_params.channel_data_rate =
            _lora_phy.get_alternate_DR(_params.join_request_trial_counter + 1);

#else
    const static uint8_t nwk_skey[] = MBED_CONF_LORA_NWKSKEY;
    const static uint8_t app_skey[] = MBED_CONF_LORA_APPSKEY;

    _params.net_id = (MBED_CONF_LORA_DEVICE_ADDRESS & LORAWAN_NETWORK_ID_MASK);
    _params.dev_addr = MBED_CONF_LORA_DEVICE_ADDRESS;

    memcpy(_params.keys.nwk_skey, nwk_skey,
           sizeof(_params.keys.nwk_skey));

    memcpy(_params.keys.app_skey, app_skey,
           sizeof(_params.keys.app_skey));
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

    if (LORAMAC_IDLE != _params.mac_state) {
        return LORAWAN_STATUS_BUSY;
    }

    reset_mlme_confirmation();

    _mlme_confirmation.req_type = MLME_JOIN;
    _params.flags.bits.mlme_req = 1;

    loramac_mhdr_t machdr;
    machdr.value = 0;
    machdr.bits.mtype = FRAME_TYPE_JOIN_REQ;
    return send(&machdr, 0, NULL, 0);
}

static void memcpy_convert_endianess(uint8_t *dst, const uint8_t *src,
                                     uint16_t size)
{
    dst = dst + (size - 1);
    while (size--) {
        *dst-- = *src++;
    }
}

lorawan_status_t LoRaMac::prepare_frame(loramac_mhdr_t *machdr,
                                        loramac_frame_ctrl_t *fctrl,
                                        uint8_t fport, void *fbuffer,
                                        uint16_t fbuffer_size)
{
    uint16_t i;
    uint8_t pkt_header_len = 0;
    uint32_t mic = 0;
    const void* payload = fbuffer;
    uint8_t frame_port = fport;
    lorawan_status_t status = LORAWAN_STATUS_OK;

    _params.buffer_pkt_len = 0;

    _params.is_node_ack_requested = false;

    if (fbuffer == NULL) {
        fbuffer_size = 0;
    }

    _params.payload_length = fbuffer_size;

    _params.buffer[pkt_header_len++] = machdr->value;

    switch (machdr->bits.mtype) {

        case FRAME_TYPE_JOIN_REQ:

            _params.buffer_pkt_len = pkt_header_len;
            memcpy_convert_endianess(_params.buffer + _params.buffer_pkt_len,
                                     _params.keys.app_eui, 8);
            _params.buffer_pkt_len += 8;
            memcpy_convert_endianess(_params.buffer + _params.buffer_pkt_len,
                                     _params.keys.dev_eui, 8);
            _params.buffer_pkt_len += 8;

            _params.dev_nonce = _lora_phy.get_radio_rng();

            _params.buffer[_params.buffer_pkt_len++] = _params.dev_nonce & 0xFF;
            _params.buffer[_params.buffer_pkt_len++] = (_params.dev_nonce >> 8) & 0xFF;

            if (0 != compute_join_frame_mic(_params.buffer,
                                           _params.buffer_pkt_len & 0xFF,
                                           _params.keys.app_key, &mic)) {
                return LORAWAN_STATUS_CRYPTO_FAIL;
            }

            _params.buffer[_params.buffer_pkt_len++] = mic & 0xFF;
            _params.buffer[_params.buffer_pkt_len++] = (mic >> 8) & 0xFF;
            _params.buffer[_params.buffer_pkt_len++] = (mic >> 16) & 0xFF;
            _params.buffer[_params.buffer_pkt_len++] = (mic >> 24) & 0xFF;

            break;
        case FRAME_TYPE_DATA_CONFIRMED_UP:
            _params.is_node_ack_requested = true;
            //Intentional fallthrough
        case FRAME_TYPE_DATA_UNCONFIRMED_UP: {
            if (!_is_nwk_joined) {
                return LORAWAN_STATUS_NO_NETWORK_JOINED;
            }

            if (_params.sys_params.adr_on) {
                if (_lora_phy.get_next_ADR(true,
                                           _params.sys_params.channel_data_rate,
                                           _params.sys_params.channel_tx_power,
                                           _params.adr_ack_counter)) {
                    fctrl->bits.adr_ack_req = 1;
                }
            }

            if (_params.is_srv_ack_requested == true) {
                _params.is_srv_ack_requested = false;
                fctrl->bits.ack = 1;
            }

            _params.buffer[pkt_header_len++] = (_params.dev_addr) & 0xFF;
            _params.buffer[pkt_header_len++] = (_params.dev_addr >> 8) & 0xFF;
            _params.buffer[pkt_header_len++] = (_params.dev_addr >> 16) & 0xFF;
            _params.buffer[pkt_header_len++] = (_params.dev_addr >> 24) & 0xFF;

            _params.buffer[pkt_header_len++] = fctrl->value;

            _params.buffer[pkt_header_len++] = _params.ul_frame_counter & 0xFF;
            _params.buffer[pkt_header_len++] = (_params.ul_frame_counter >> 8)
                    & 0xFF;

            mac_commands.copy_repeat_commands_to_buffer();

            const uint8_t mac_commands_len = mac_commands.get_mac_cmd_length();

            if ((payload != NULL) && (_params.payload_length > 0)) {
                if (mac_commands.is_mac_command_in_next_tx() == true) {
                    if (mac_commands_len <= LORA_MAC_COMMAND_MAX_FOPTS_LENGTH) {
                        fctrl->bits.fopts_len += mac_commands_len;

                        // Update FCtrl field with new value of OptionsLength
                        _params.buffer[0x05] = fctrl->value;

                        const uint8_t *buffer =
                                mac_commands.get_mac_commands_buffer();
                        for (i = 0; i < mac_commands_len; i++) {
                            _params.buffer[pkt_header_len++] = buffer[i];
                        }
                    } else {
                        _params.payload_length = mac_commands_len;
                        payload = mac_commands.get_mac_commands_buffer();
                        frame_port = 0;
                    }
                }
            } else {
                if ((mac_commands_len > 0)
                        && (mac_commands.is_mac_command_in_next_tx() == true)) {
                    _params.payload_length = mac_commands_len;
                    payload = mac_commands.get_mac_commands_buffer();
                    frame_port = 0;
                }
            }

            mac_commands.parse_mac_commands_to_repeat();

            if ((payload != NULL) && (_params.payload_length > 0)) {
                _params.buffer[pkt_header_len++] = frame_port;

                uint8_t *key = _params.keys.app_skey;
                if (frame_port == 0) {
                    mac_commands.clear_command_buffer();
                    key = _params.keys.nwk_skey;
                }
                if (0 != encrypt_payload((uint8_t*) payload, _params.payload_length,
                                         key, _params.dev_addr, UP_LINK,
                                         _params.ul_frame_counter,
                                         &_params.buffer[pkt_header_len])) {
                    status = LORAWAN_STATUS_CRYPTO_FAIL;
                }
            }

            _params.buffer_pkt_len = pkt_header_len + _params.payload_length;

            if (0 != compute_mic(_params.buffer, _params.buffer_pkt_len,
                                 _params.keys.nwk_skey, _params.dev_addr,
                                 UP_LINK, _params.ul_frame_counter, &mic)) {
                status = LORAWAN_STATUS_CRYPTO_FAIL;
            }

            _params.buffer[_params.buffer_pkt_len + 0] = mic & 0xFF;
            _params.buffer[_params.buffer_pkt_len + 1] = (mic >> 8) & 0xFF;
            _params.buffer[_params.buffer_pkt_len + 2] = (mic >> 16) & 0xFF;
            _params.buffer[_params.buffer_pkt_len + 3] = (mic >> 24) & 0xFF;

            _params.buffer_pkt_len += LORAMAC_MFR_LEN;
        }
            break;
        case FRAME_TYPE_PROPRIETARY:
            if ((fbuffer != NULL) && (_params.payload_length > 0)) {
                memcpy(_params.buffer + pkt_header_len, (uint8_t*) fbuffer,
                       _params.payload_length);
                _params.buffer_pkt_len = pkt_header_len + _params.payload_length;
            }
            break;
        default:
            status = LORAWAN_STATUS_SERVICE_UNKNOWN;
    }

    return status;
}

lorawan_status_t LoRaMac::send_frame_on_channel(uint8_t channel)
{
    tx_config_params_t tx_config;
    int8_t tx_power = 0;

    tx_config.channel = channel;
    tx_config.datarate = _params.sys_params.channel_data_rate;
    tx_config.tx_power = _params.sys_params.channel_tx_power;
    tx_config.max_eirp = _params.sys_params.max_eirp;
    tx_config.antenna_gain = _params.sys_params.antenna_gain;
    tx_config.pkt_len = _params.buffer_pkt_len;

    _lora_phy.tx_config(&tx_config, &tx_power, &_params.timers.tx_toa);

    _mlme_confirmation.status = LORAMAC_EVENT_INFO_STATUS_ERROR;

    _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
    _mcps_confirmation.data_rate = _params.sys_params.channel_data_rate;
    _mcps_confirmation.tx_power = tx_power;
    _mcps_confirmation.channel = channel;

    _mcps_confirmation.tx_toa = _params.timers.tx_toa;
    _mlme_confirmation.tx_toa = _params.timers.tx_toa;

    _lora_time.start(_params.timers.mac_state_check_timer,
                     MAC_STATE_CHECK_TIMEOUT);

    if (!_is_nwk_joined) {
        _params.join_request_trial_counter++;
    }

    _lora_phy.handle_send(_params.buffer, _params.buffer_pkt_len);

    _params.mac_state |= LORAMAC_TX_RUNNING;

    return LORAWAN_STATUS_OK;
}

void LoRaMac::reset_mcps_confirmation()
{
    memset((uint8_t*) &_mcps_confirmation, 0, sizeof(_mcps_confirmation));
    _mcps_confirmation.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
}

void LoRaMac::reset_mlme_confirmation()
{
    memset((uint8_t*) &_mlme_confirmation, 0, sizeof(_mlme_confirmation));
    _mlme_confirmation.status = LORAMAC_EVENT_INFO_STATUS_ERROR;
}

void LoRaMac::set_tx_continuous_wave(uint8_t channel, int8_t datarate, int8_t tx_power,
                                     float max_eirp, float antenna_gain, uint16_t timeout)
{
    cw_mode_params_t continuous_wave;

    continuous_wave.channel = channel;
    continuous_wave.datarate = datarate;
    continuous_wave.tx_power = tx_power;
    continuous_wave.max_eirp = max_eirp;
    continuous_wave.antenna_gain = antenna_gain;
    continuous_wave.timeout = timeout;

    _lora_phy.set_tx_cont_mode(&continuous_wave);
}

lorawan_status_t LoRaMac::initialize(loramac_primitives_t *primitives,
                                     EventQueue *queue)
{
    _lora_time.activate_timer_subsystem(queue);

    ev_queue = queue;

    if (!primitives) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    channel_plan.activate_channelplan_subsystem(&_lora_phy);

    mac_primitives = primitives;

    _params.flags.value = 0;

    _device_class = CLASS_A;
    _params.mac_state = LORAMAC_IDLE;

    _params.join_request_trial_counter = 0;
    _params.max_join_request_trials = 1;
    _params.is_repeater_supported = false;

    _params.timers.aggregated_last_tx_time = 0;
    _params.timers.aggregated_timeoff = 0;

    _lora_phy.reset_to_default_values(&_params, true);

    _params.sys_params.max_sys_rx_error = 10;
    _params.sys_params.min_rx_symb = 6;
    _params.sys_params.retry_num = 1;

    reset_mac_parameters();

    srand(_lora_phy.get_radio_rng());

    _params.is_nwk_public = MBED_CONF_LORA_PUBLIC_NETWORK;
    _lora_phy.setup_public_network_mode(_params.is_nwk_public);
    _lora_phy.put_radio_to_sleep();

    _lora_time.init(_params.timers.mac_state_check_timer,
                    mbed::callback(this, &LoRaMac::on_mac_state_check_timer_event));
    _lora_time.init(_params.timers.tx_delayed_timer,
                    mbed::callback(this, &LoRaMac::on_tx_delayed_timer_event));
    _lora_time.init(_params.timers.rx_window1_timer,
                    mbed::callback(this, &LoRaMac::on_rx_window1_timer_event));
    _lora_time.init(_params.timers.rx_window2_timer,
                    mbed::callback(this, &LoRaMac::on_rx_window2_timer_event));
    _lora_time.init(_params.timers.ack_timeout_timer,
                    mbed::callback(this, &LoRaMac::on_ack_timeout_timer_event));

    _params.timers.mac_init_time = _lora_time.get_current_time();

    _params.sys_params.adr_on = MBED_CONF_LORA_ADR_ON;

    _params.is_nwk_public = MBED_CONF_LORA_PUBLIC_NETWORK;
    _lora_phy.setup_public_network_mode(MBED_CONF_LORA_PUBLIC_NETWORK);

    return LORAWAN_STATUS_OK;
}

void LoRaMac::disconnect()
{
    _lora_time.stop(_params.timers.mac_state_check_timer);
    _lora_time.stop(_params.timers.tx_delayed_timer);
    _lora_time.stop(_params.timers.rx_window1_timer);
    _lora_time.stop(_params.timers.rx_window2_timer);
    _lora_time.stop(_params.timers.ack_timeout_timer);

    _lora_phy.put_radio_to_sleep();

    _is_nwk_joined = false;
    _params.is_ack_retry_timeout_expired = false;
    _params.is_rx_window_enabled = true;
    _params.is_node_ack_requested = false;
    _params.is_srv_ack_requested = false;
    _params.flags.value = 0;
    _params.mac_state = 0;

    mac_commands.clear_command_buffer();
    mac_commands.clear_repeat_buffer();
    mac_commands.clear_mac_commands_in_next_tx();

    _params.mac_state = LORAMAC_IDLE;
}

uint8_t LoRaMac::get_max_possible_tx_size(uint8_t size)
{
    uint8_t max_possible_payload_size = 0;
    uint8_t current_payload_size = 0;
    uint8_t fopt_len = mac_commands.get_mac_cmd_length()
            + mac_commands.get_repeat_commands_length();

    if (_params.sys_params.adr_on) {
        _lora_phy.get_next_ADR(false, _params.sys_params.channel_data_rate,
                               _params.sys_params.channel_tx_power,
                               _params.adr_ack_counter);
    }

    current_payload_size = _lora_phy.get_max_payload(_params.sys_params.channel_data_rate, _params.is_repeater_supported);

    if (current_payload_size >= fopt_len) {
        max_possible_payload_size = current_payload_size - fopt_len;
    } else {
        max_possible_payload_size = current_payload_size;
        fopt_len = 0;
        mac_commands.clear_command_buffer();
        mac_commands.clear_repeat_buffer();
    }

    if (validate_payload_length(size, _params.sys_params.channel_data_rate,
                                fopt_len) == false) {
        return max_possible_payload_size;
    }
    return current_payload_size;
}

bool LoRaMac::nwk_joined()
{
    return _is_nwk_joined;
}

void LoRaMac::set_nwk_joined(bool joined)
{
    _is_nwk_joined = joined;
}

lorawan_status_t LoRaMac::add_channel_plan(const lorawan_channelplan_t& plan)
{
    if ((_params.mac_state & LORAMAC_TX_RUNNING) == LORAMAC_TX_RUNNING) {
        if ((_params.mac_state & LORAMAC_TX_CONFIG) != LORAMAC_TX_CONFIG) {
            return LORAWAN_STATUS_BUSY;
        }
    }

    return channel_plan.set_plan(plan);
}

lorawan_status_t LoRaMac::remove_channel_plan()
{
    if ((_params.mac_state & LORAMAC_TX_RUNNING) == LORAMAC_TX_RUNNING) {
        if ((_params.mac_state & LORAMAC_TX_CONFIG) != LORAMAC_TX_CONFIG) {
            return LORAWAN_STATUS_BUSY;
        }
    }

    return channel_plan.remove_plan();
}

lorawan_status_t LoRaMac::get_channel_plan(lorawan_channelplan_t& plan)
{
    return channel_plan.get_plan(plan, _lora_phy.get_phy_channels());
}

lorawan_status_t LoRaMac::remove_single_channel(uint8_t id)
{
    if ((_params.mac_state & LORAMAC_TX_RUNNING) == LORAMAC_TX_RUNNING) {
        if ((_params.mac_state & LORAMAC_TX_CONFIG) != LORAMAC_TX_CONFIG) {
            return LORAWAN_STATUS_BUSY;
        }
    }

    return channel_plan.remove_single_channel(id);
}

lorawan_status_t LoRaMac::multicast_channel_link(multicast_params_t *channel_param)
{
    if (channel_param == NULL) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }
    if ((_params.mac_state & LORAMAC_TX_RUNNING) == LORAMAC_TX_RUNNING) {
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

lorawan_status_t LoRaMac::multicast_channel_unlink(
        multicast_params_t *channel_param)
{
    if (channel_param == NULL) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }
    if ((_params.mac_state & LORAMAC_TX_RUNNING) == LORAMAC_TX_RUNNING) {
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

void LoRaMac::bind_radio_driver(LoRaRadio& radio)
{
    radio_events.tx_done = mbed::callback(this, &LoRaMac::handle_tx_done);
    radio_events.rx_done = mbed::callback(this, &LoRaMac::handle_rx_done);
    radio_events.rx_error = mbed::callback(this, &LoRaMac::handle_rx_error);
    radio_events.tx_timeout = mbed::callback(this, &LoRaMac::handle_tx_timeout);
    radio_events.rx_timeout = mbed::callback(this, &LoRaMac::handle_rx_timeout);

    _lora_phy.set_radio_instance(radio);
    radio.lock();
    radio.init_radio(&radio_events);
    radio.unlock();
}

#if defined(LORAWAN_COMPLIANCE_TEST)
/***************************************************************************
 * Compliance testing                                                      *
 **************************************************************************/


lorawan_status_t LoRaMac::mlme_request( loramac_mlme_req_t *mlmeRequest )
{
    if (LORAMAC_IDLE != _params.mac_state) {
        return LORAWAN_STATUS_BUSY;
    }

    reset_mlme_confirmation();

    _mlme_confirmation.req_type = mlmeRequest->type;
    _params.flags.bits.mlme_req = 1;

    lorawan_status_t status = LORAWAN_STATUS_SERVICE_UNKNOWN;

    if (MLME_TXCW == mlmeRequest->type) {
        set_tx_continuous_wave(_params.channel, _params.sys_params.channel_data_rate, _params.sys_params.channel_tx_power,
                                    _params.sys_params.max_eirp, _params.sys_params.antenna_gain, mlmeRequest->cw_tx_mode.timeout);
        _lora_time.start(_params.timers.mac_state_check_timer,
                         MAC_STATE_CHECK_TIMEOUT);

        _params.mac_state |= LORAMAC_TX_RUNNING;
        status = LORAWAN_STATUS_OK;
    } else if (MLME_TXCW_1 == mlmeRequest->type) {
        set_tx_continuous_wave(0, 0, mlmeRequest->cw_tx_mode.power, 0, 0, mlmeRequest->cw_tx_mode.timeout);
        _lora_time.start(_params.timers.mac_state_check_timer,
                         MAC_STATE_CHECK_TIMEOUT);

        _params.mac_state |= LORAMAC_TX_RUNNING;
        status = LORAWAN_STATUS_OK;
    }

    if (status != LORAWAN_STATUS_OK) {
        _params.is_node_ack_requested = false;
        _params.flags.bits.mlme_req = 0;
    }


    return status;
}

lorawan_status_t LoRaMac::test_request( loramac_compliance_test_req_t *mcpsRequest )
{
    if (_params.mac_state != LORAMAC_IDLE) {
        return LORAWAN_STATUS_BUSY;
    }

    loramac_mhdr_t machdr;
    int8_t datarate = mcpsRequest->data_rate;
    // TODO: The comment is different than the code???
    // Apply the minimum possible datarate.
    // Some regions have limitations for the minimum datarate.
    datarate = MAX(datarate, (int8_t)_lora_phy.get_minimum_tx_datarate());

    machdr.value = 0;

    reset_mcps_confirmation();

    _params.ack_timeout_retry_counter = 1;
    _params.max_ack_timeout_retries = 1;

    switch (mcpsRequest->type) {
        case MCPS_UNCONFIRMED: {
            machdr.bits.mtype = FRAME_TYPE_DATA_UNCONFIRMED_UP;
            break;
        }
        case MCPS_CONFIRMED: {
            machdr.bits.mtype = FRAME_TYPE_DATA_CONFIRMED_UP;
            _params.max_ack_timeout_retries = mcpsRequest->nb_trials;
            break;
        }
        case MCPS_PROPRIETARY: {
            machdr.bits.mtype = FRAME_TYPE_PROPRIETARY;
            break;
        }
        default:
            return LORAWAN_STATUS_PARAMETER_INVALID;
    }

//    Filter fPorts
//    TODO: Does not work with PROPRIETARY messages
//    if( IsFPortAllowed( mcpsRequest->fport ) == false ) {
//        return LORAWAN_STATUS_PARAMETER_INVALID;
//    }

    if (_params.sys_params.adr_on == false) {
        if (_lora_phy.verify_tx_datarate(datarate, false) == true) {
            _params.sys_params.channel_data_rate = datarate;
        } else {
            return LORAWAN_STATUS_PARAMETER_INVALID;
        }
    }

    lorawan_status_t status = send(&machdr, mcpsRequest->fport, mcpsRequest->f_buffer,
                                   mcpsRequest->f_buffer_size);
    if (status == LORAWAN_STATUS_OK) {
        _mcps_confirmation.req_type = mcpsRequest->type;
        _params.flags.bits.mcps_req = 1;
    } else {
        _params.is_node_ack_requested = false;
    }

    return status;
}

lorawan_status_t LoRaMac::LoRaMacSetTxTimer( uint32_t TxDutyCycleTime )
{
    _lora_time.start(tx_next_packet_timer, TxDutyCycleTime);
    return LORAWAN_STATUS_OK;
}

 lorawan_status_t LoRaMac::LoRaMacStopTxTimer( )
{
    _lora_time.stop(tx_next_packet_timer);
    return LORAWAN_STATUS_OK;
}

void LoRaMac::LoRaMacTestRxWindowsOn( bool enable )
{
    _params.is_rx_window_enabled = enable;
}

void LoRaMac::LoRaMacTestSetMic( uint16_t txPacketCounter )
{
    _params.ul_frame_counter = txPacketCounter;
    _params.is_ul_frame_counter_fixed = true;
}

void LoRaMac::LoRaMacTestSetDutyCycleOn( bool enable )
{
    if(_lora_phy.verify_duty_cycle(enable) == true)
    {
        _params.is_dutycycle_on = enable;
    }
}

void LoRaMac::LoRaMacTestSetChannel( uint8_t channel )
{
    _params.channel = channel;
}
#endif
