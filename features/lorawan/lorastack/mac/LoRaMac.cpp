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


LoRaMac::LoRaMac(LoRaWANTimeHandler &lora_time)
    : mac_commands(*this), _lora_time(lora_time)
{
    lora_phy = NULL;
    //radio_events_t RadioEvents;
    _params.keys.dev_eui = NULL;
    _params.keys.app_eui = NULL;
    _params.keys.app_key = NULL;

    memset(_params.keys.nwk_skey, 0, sizeof(_params.keys.nwk_skey));
    memset(_params.keys.app_skey, 0, sizeof(_params.keys.app_skey));

    _params.dev_nonce = 0;
    _params.net_id = 0;
    _params.dev_addr = 0;
    _params.buffer_pkt_len = 0;
    _params.payload_length = 0;
    _params.ul_frame_counter = 0;
    _params.dl_frame_counter = 0;
    _params.is_ul_frame_counter_fixed = false;
    _params.is_rx_window_enabled = true;
    _params.is_nwk_joined = false;
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

void LoRaMac::handle_mac_state_check_timer_event(void)
{
    const int ret = ev_queue->call(this, &LoRaMac::on_mac_state_check_timer_event);
    MBED_ASSERT(ret != 0);
    (void)ret;
}

void LoRaMac::handle_delayed_tx_timer_event(void)
{
    const int ret = ev_queue->call(this, &LoRaMac::on_tx_delayed_timer_event);
    MBED_ASSERT(ret != 0);
    (void)ret;
}

void LoRaMac::handle_ack_timeout()
{
    const int ret = ev_queue->call(this, &LoRaMac::on_ack_timeout_timer_event);
    MBED_ASSERT(ret != 0);
    (void)ret;
}

void LoRaMac::handle_rx1_timer_event(void)
{
    const int ret = ev_queue->call(this, &LoRaMac::on_rx_window1_timer_event);
    MBED_ASSERT(ret != 0);
    (void)ret;
}

void LoRaMac::handle_rx2_timer_event(void)
{
    const int ret = ev_queue->call(this, &LoRaMac::on_rx_window2_timer_event);
    MBED_ASSERT(ret != 0);
    (void)ret;
}

/***************************************************************************
 * Radio event callbacks - delegated to Radio driver                       *
 **************************************************************************/
void LoRaMac::on_radio_tx_done( void )
{
    get_phy_params_t get_phy;
    phy_param_t phy_param;
    set_band_txdone_params_t tx_done_params;
    lorawan_time_t cur_time = _lora_time.get_current_time( );
    loramac_mlme_confirm_t mlme_confirm = mlme.get_confirmation();

    if (_params.dev_class != CLASS_C) {
        lora_phy->put_radio_to_sleep();
    } else {
        open_continuous_rx2_window();
    }

    // Setup timers
    if(_params.is_rx_window_enabled == true) {
        _lora_time.start(_params.timers.rx_window1_timer, _params.rx_window1_delay);

        if (_params.dev_class != CLASS_C) {
            _lora_time.start(_params.timers.rx_window2_timer, _params.rx_window2_delay);
        }

        if ((_params.dev_class == CLASS_C ) ||
            (_params.is_node_ack_requested == true)) {
            get_phy.attribute = PHY_ACK_TIMEOUT;
            phy_param = lora_phy->get_phy_params(&get_phy);
            _lora_time.start(_params.timers.ack_timeout_timer,
                             _params.rx_window2_delay + phy_param.value);
        }
    } else {
        mcps.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_OK;
        mlme_confirm.status = LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT;

        if (_params.flags.value == 0) {
            _params.flags.bits.mcps_req = 1;
        }

        _params.flags.bits.mac_done = 1;
    }

    // Verify if the last uplink was a join request
    if ((_params.flags.bits.mlme_req == 1) &&
        (mlme_confirm.req_type == MLME_JOIN)) {
        _params.is_last_tx_join_request = true;
    } else {
        _params.is_last_tx_join_request = false;
    }

    // Store last Tx channel
    _params.last_channel_idx = _params.channel;

    // Update last tx done time for the current channel
    tx_done_params.channel = _params.channel;
    tx_done_params.joined = _params.is_nwk_joined;
    tx_done_params.last_tx_done_time = cur_time;
    lora_phy->set_last_tx_done(&tx_done_params);

    // Update Aggregated last tx done time
    _params.timers.aggregated_last_tx_time = cur_time;

    if (_params.is_node_ack_requested == false) {
        mcps.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_OK;
        _params.ul_nb_rep_counter++;
    }
}

void LoRaMac::prepare_rx_done_abort(void)
{
    _params.mac_state |= LORAMAC_RX_ABORT;

    if (_params.is_node_ack_requested) {
        handle_ack_timeout();
    }

    _params.flags.bits.mcps_ind = 1;
    _params.flags.bits.mac_done = 1;

    // Trigger MAC state check event timer as soon as possible
    _lora_time.start(_params.timers.mac_state_check_timer, 1);
}

void LoRaMac::on_radio_rx_done(uint8_t *payload, uint16_t size, int16_t rssi,
                               int8_t snr)
{
    loramac_mhdr_t mac_hdr;
    loramac_frame_ctrl_t fctrl;
    cflist_params_t cflist;
    get_phy_params_t get_phy;
    phy_param_t phy_param;
    bool skip_indication = false;

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

    mcps.get_confirmation().ack_received = false;
    mcps.get_indication().rssi = rssi;
    mcps.get_indication().snr = snr;
    mcps.get_indication().rx_slot = _params.rx_slot;
    mcps.get_indication().port = 0;
    mcps.get_indication().multicast = 0;
    mcps.get_indication().fpending_status = 0;
    mcps.get_indication().buffer = NULL;
    mcps.get_indication().buffer_size = 0;
    mcps.get_indication().is_data_recvd = false;
    mcps.get_indication().is_ack_recvd = false;
    mcps.get_indication().dl_frame_counter = 0;
    mcps.get_indication().type = MCPS_UNCONFIRMED;

    lora_phy->put_radio_to_sleep();

    _lora_time.stop( _params.timers.rx_window2_timer );

    mac_hdr.value = payload[pkt_header_len++];

    switch (mac_hdr.bits.mtype) {

        case FRAME_TYPE_JOIN_ACCEPT:
            if (_params.is_nwk_joined == true) {
                mcps.get_indication().status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                prepare_rx_done_abort();
                return;
            }

            if (0 != decrypt_join_frame(payload + 1, size - 1,
                                        _params.keys.app_key,
                                        _params.payload + 1)) {
                mcps.get_indication().status = LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
                return;
            }

            _params.payload[0] = mac_hdr.value;

            if (0 != compute_join_frame_mic(_params.payload, size - LORAMAC_MFR_LEN,
                                           _params.keys.app_key, &mic)) {
                mcps.get_indication().status = LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
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
                    mcps.get_indication().status = LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
                    return;
                }

                _params.net_id = (uint32_t) _params.payload[4];
                _params.net_id |= ((uint32_t) _params.payload[5] << 8);
                _params.net_id |= ((uint32_t) _params.payload[6] << 16);

                _params.dev_addr = (uint32_t) _params.payload[7];
                _params.dev_addr |= ((uint32_t) _params.payload[8] << 8);
                _params.dev_addr |= ((uint32_t) _params.payload[9] << 16);
                _params.dev_addr |= ((uint32_t) _params.payload[10] << 24);

                // DLSettings
                _params.sys_params.rx1_dr_offset = (_params.payload[11] >> 4) & 0x07;
                _params.sys_params.rx2_channel.datarate = _params.payload[11] & 0x0F;

                // RxDelay
                _params.sys_params.recv_delay1 = (_params.payload[12] & 0x0F);

                if (_params.sys_params.recv_delay1 == 0) {
                    _params.sys_params.recv_delay1 = 1;
                }

                _params.sys_params.recv_delay1 *= 1000;
                _params.sys_params.recv_delay2 = _params.sys_params.recv_delay1 + 1000;

                // Apply CF list
                cflist.payload = &_params.payload[13];
                // Size of the regular payload is 12. Plus 1 byte MHDR and 4 bytes MIC
                cflist.size = size - 17;

                lora_phy->apply_cf_list(&cflist);

                mlme.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_OK;
                _params.is_nwk_joined = true;
            } else {
                mlme.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_JOIN_FAIL;
            }

            break;

        case FRAME_TYPE_DATA_CONFIRMED_DOWN:
        case FRAME_TYPE_DATA_UNCONFIRMED_DOWN:
            {
                // Check if the received payload size is valid
                get_phy.datarate = mcps.get_indication().rx_datarate;
                get_phy.attribute = PHY_MAX_PAYLOAD;

                // Get the maximum payload length
                if (_params.is_repeater_supported == true) {
                    get_phy.attribute = PHY_MAX_PAYLOAD_REPEATER;
                }

                phy_param = lora_phy->get_phy_params(&get_phy);

                if (MAX(0, (int16_t) ((int16_t)size - (int16_t)LORA_MAC_FRMPAYLOAD_OVERHEAD )) > (int32_t)phy_param.value) {
                    mcps.get_indication().status = LORAMAC_EVENT_INFO_STATUS_ERROR;
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
                        mcps.get_indication().status = LORAMAC_EVENT_INFO_STATUS_ADDRESS_FAIL;
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
                    // check for sequence roll-over
                    uint32_t  downlink_counter_tmp = downlink_counter + 0x10000 + (int16_t)sequence_counter_diff;
                    compute_mic(payload, size - LORAMAC_MFR_LEN, nwk_skey,
                                      address, DOWN_LINK, downlink_counter_tmp, &mic);

                    if (mic_rx == mic ) {
                        is_mic_ok = true;
                        downlink_counter = downlink_counter_tmp;
                    }
                }

                // Check for a the maximum allowed counter difference
                get_phy.attribute = PHY_MAX_FCNT_GAP;
                phy_param = lora_phy->get_phy_params(&get_phy);

                if (sequence_counter_diff >= phy_param.value) {
                    mcps.get_indication().status = LORAMAC_EVENT_INFO_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOSS;
                    mcps.get_indication().dl_frame_counter = downlink_counter;
                    prepare_rx_done_abort( );
                    return;
                }

                if (is_mic_ok == true) {
                    mcps.get_indication().status = LORAMAC_EVENT_INFO_STATUS_OK;
                    mcps.get_indication().multicast = multicast;
                    mcps.get_indication().fpending_status = fctrl.bits.fpending;
                    mcps.get_indication().buffer = NULL;
                    mcps.get_indication().buffer_size = 0;
                    mcps.get_indication().dl_frame_counter = downlink_counter;

                    mcps.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_OK;

                    _params.adr_ack_counter = 0;
                    mac_commands.clear_repeat_buffer();

                    // Update 32 bits downlink counter
                    if (multicast == 1) {
                        mcps.get_indication().type = MCPS_MULTICAST;

                        if ((cur_multicast_params->dl_frame_counter == downlink_counter) &&
                            (cur_multicast_params->dl_frame_counter != 0)) {

                            mcps.get_indication().status = LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED;
                            mcps.get_indication().dl_frame_counter = downlink_counter;
                            prepare_rx_done_abort();

                            return;
                        }

                        cur_multicast_params->dl_frame_counter = downlink_counter;

                    } else {

                        if (mac_hdr.bits.mtype == FRAME_TYPE_DATA_CONFIRMED_DOWN) {
                            _params.is_srv_ack_requested = true;
                            mcps.get_indication().type = MCPS_CONFIRMED;

                            if ((_params.dl_frame_counter == downlink_counter ) &&
                                (_params.dl_frame_counter != 0)) {
                                // Duplicated confirmed downlink. Skip indication.
                                // In this case, the MAC layer shall accept the MAC commands
                                // which are included in the downlink retransmission.
                                // It should not provide the same frame to the application
                                // layer again.
                                skip_indication = true;
                            }
                        } else {
                            _params.is_srv_ack_requested = false;
                            mcps.get_indication().type = MCPS_UNCONFIRMED;

                            if ((_params.dl_frame_counter == downlink_counter) &&
                                (_params.dl_frame_counter != 0)) {
                                mcps.get_indication().status = LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED;
                                mcps.get_indication().dl_frame_counter = downlink_counter;
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
                    if (mcps.get_confirmation().req_type == MCPS_CONFIRMED) {
                        if (fctrl.bits.ack == 1) {
                            // Reset MacCommandsBufferIndex when we have received an ACK.
                            mac_commands.clear_command_buffer();
                        }
                    } else {
                        // Reset the variable if we have received any valid frame.
                        mac_commands.clear_command_buffer();
                    }

                    // Process payload and MAC commands
                    if (((size - 4) - app_payload_start_index) > 0) {
                        uint8_t port = payload[app_payload_start_index++];
                        frame_len = (size - 4) - app_payload_start_index;

                        mcps.get_indication().port = port;

                        if (port == 0) {
                            // Only allow frames which do not have fOpts
                            if (fctrl.bits.fopts_len == 0) {
                                if (0 != decrypt_payload(payload + app_payload_start_index,
                                                               frame_len,
                                                               nwk_skey,
                                                               address,
                                                               DOWN_LINK,
                                                               downlink_counter,
                                                               _params.payload)) {
                                    mcps.get_indication().status =  LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
                                }

                                // Decode frame payload MAC commands
                                if (mac_commands.process_mac_commands(_params.payload, 0, frame_len, snr,
                                                                    mlme.get_confirmation(),
                                                                    _params.sys_params, *lora_phy) != LORAWAN_STATUS_OK) {
                                    mcps.get_indication().status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                                }
                            } else {
                                skip_indication = true;
                            }
                        } else {
                            if (fctrl.bits.fopts_len > 0) {
                                // Decode Options field MAC commands. Omit the fPort.
                                if (mac_commands.process_mac_commands(payload, 8, app_payload_start_index - 1, snr,
                                                                    mlme.get_confirmation(),
                                                                    _params.sys_params, *lora_phy ) != LORAWAN_STATUS_OK) {
                                    mcps.get_indication().status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                                }
                            }

                            if (0 != decrypt_payload(payload + app_payload_start_index,
                                                           frame_len,
                                                           app_skey,
                                                           address,
                                                           DOWN_LINK,
                                                           downlink_counter,
                                                           _params.payload)) {
                                mcps.get_indication().status =  LORAMAC_EVENT_INFO_STATUS_CRYPTO_FAIL;
                            }

                            if (skip_indication == false) {
                                mcps.get_indication().buffer = _params.payload;
                                mcps.get_indication().buffer_size = frame_len;
                                mcps.get_indication().is_data_recvd = true;
                            }
                        }
                    } else {
                        if (fctrl.bits.fopts_len > 0) {
                            // Decode Options field MAC commands
                            if (mac_commands.process_mac_commands(payload, 8, app_payload_start_index, snr,
                                                                mlme.get_confirmation(),
                                                                _params.sys_params, *lora_phy) != LORAWAN_STATUS_OK) {
                                mcps.get_indication().status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                            }
                        }
                    }

                    if (skip_indication == false) {
                        // Check if the frame is an acknowledgement
                        if (fctrl.bits.ack == 1) {
                            mcps.get_confirmation().ack_received = true;
                            mcps.get_indication().is_ack_recvd = true;

                            // Stop the AckTimeout timer as no more retransmissions
                            // are needed.
                            _lora_time.stop(_params.timers.ack_timeout_timer);
                        } else {
                            mcps.get_confirmation().ack_received = false;

                            if (_params.ack_timeout_retry_counter > _params.max_ack_timeout_retries) {
                                // Stop the AckTimeout timer as no more retransmissions
                                // are needed.
                                _lora_time.stop( _params.timers.ack_timeout_timer );
                            }
                        }
                    }
                    // Provide always an indication, skip the callback to the user application,
                    // in case of a confirmed downlink retransmission.
                    _params.flags.bits.mcps_ind = 1;
                    _params.flags.bits.mcps_ind_skip = skip_indication;
                } else {
                    mcps.get_indication().status = LORAMAC_EVENT_INFO_STATUS_MIC_FAIL;

                    prepare_rx_done_abort( );
                    return;
                }
            }

            break;

        case FRAME_TYPE_PROPRIETARY:
            {
                memcpy(_params.payload, &payload[pkt_header_len], size);

                mcps.get_indication().type = MCPS_PROPRIETARY;
                mcps.get_indication().status = LORAMAC_EVENT_INFO_STATUS_OK;
                mcps.get_indication().buffer = _params.payload;
                mcps.get_indication().buffer_size = size - pkt_header_len;

                _params.flags.bits.mcps_ind = 1;
                break;
            }
        default:
            mcps.get_indication().status = LORAMAC_EVENT_INFO_STATUS_ERROR;
            prepare_rx_done_abort();
            break;
    }
    _params.flags.bits.mac_done = 1;

    _lora_time.start(_params.timers.mac_state_check_timer, 1);
}

void LoRaMac::on_radio_tx_timeout( void )
{
    if (_params.dev_class != CLASS_C) {
        lora_phy->put_radio_to_sleep();
    } else {
        open_continuous_rx2_window();
    }

    mcps.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT;

    mlme.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT;

    _params.flags.bits.mac_done = 1;
}

void LoRaMac::on_radio_rx_error( void )
{
    if (_params.dev_class != CLASS_C) {
        lora_phy->put_radio_to_sleep();
    } else {
        open_continuous_rx2_window();
    }

    if (_params.rx_slot == RX_SLOT_WIN_1) {
        if (_params.is_node_ack_requested == true) {
            mcps.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_RX1_ERROR;
        }

        mlme.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_RX1_ERROR;

        if (_lora_time.get_elapsed_time(_params.timers.aggregated_last_tx_time) >= _params.rx_window2_delay) {
            _lora_time.stop(_params.timers.rx_window2_timer);
            _params.flags.bits.mac_done = 1;
        }

    } else {

        if (_params.is_node_ack_requested == true) {
            mcps.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_RX2_ERROR;
        }

        mlme.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_RX2_ERROR;

        _params.flags.bits.mac_done = 1;
    }
}

void LoRaMac::on_radio_rx_timeout(void)
{
    if (_params.dev_class != CLASS_C) {
        lora_phy->put_radio_to_sleep();
    } else {
        open_continuous_rx2_window();
    }

    if (_params.rx_slot == RX_SLOT_WIN_1) {
        if (_params.is_node_ack_requested == true) {
            mcps.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_RX1_TIMEOUT;
        }
        mlme.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_RX1_TIMEOUT;

        if (_lora_time.get_elapsed_time(_params.timers.aggregated_last_tx_time ) >= _params.rx_window2_delay) {
            _lora_time.stop(_params.timers.rx_window2_timer);
            _params.flags.bits.mac_done = 1;
        }

    } else {

        if (_params.is_node_ack_requested == true) {
            mcps.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT;
        }

        mlme.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT;

        if (_params.dev_class != CLASS_C) {
            _params.flags.bits.mac_done = 1;
        }
    }
}

/***************************************************************************
 * Timer event callbacks - deliberated locally                             *
 **************************************************************************/
void LoRaMac::on_mac_state_check_timer_event(void)
{
    get_phy_params_t get_phy;
    phy_param_t phy_param;
    bool tx_timeout = false;

    _lora_time.stop(_params.timers.mac_state_check_timer);

    if (_params.flags.bits.mac_done == 1) {

        if ((_params.mac_state & LORAMAC_RX_ABORT) == LORAMAC_RX_ABORT) {
            _params.mac_state &= ~LORAMAC_RX_ABORT;
            _params.mac_state &= ~LORAMAC_TX_RUNNING;
        }

        if ((_params.flags.bits.mlme_req == 1) || (_params.flags.bits.mcps_req == 1)) {

            if ((mcps.get_confirmation().status == LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT) ||
                ( mlme.get_confirmation().status == LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT)) {
                // Stop transmit cycle due to tx timeout.
                _params.mac_state &= ~LORAMAC_TX_RUNNING;
                mac_commands.clear_command_buffer();
                mcps.get_confirmation().nb_retries = _params.ack_timeout_retry_counter;
                mcps.get_confirmation().ack_received = false;
                mcps.get_confirmation().tx_toa = 0;
                tx_timeout = true;
            }
        }

        if ((_params.is_node_ack_requested == false) && (tx_timeout == false)) {
            if ((_params.flags.bits.mlme_req == 1) || ((_params.flags.bits.mcps_req == 1))) {
                if ((_params.flags.bits.mlme_req == 1) && (mlme.get_confirmation().req_type == MLME_JOIN)) {
                    // Procedure for the join request
                    mlme.get_confirmation().nb_retries = _params.join_request_trial_counter;

                    if (mlme.get_confirmation().status == LORAMAC_EVENT_INFO_STATUS_OK) {
                        // Node joined successfully
                        _params.ul_frame_counter = 0;
                        _params.ul_nb_rep_counter = 0;
                        _params.mac_state &= ~LORAMAC_TX_RUNNING;
                    } else {
                        if (_params.join_request_trial_counter >= _params.max_join_request_trials) {
                            _params.mac_state &= ~LORAMAC_TX_RUNNING;
                        } else {
                            _params.flags.bits.mac_done = 0;
                            // Sends the same frame again
                            handle_delayed_tx_timer_event();
                        }
                    }
                } else {
                    // Procedure for all other frames
                    if ((_params.ul_nb_rep_counter >= _params.sys_params.retry_num) ||
                        (_params.flags.bits.mcps_ind == 1)) {
                        if (_params.flags.bits.mcps_ind == 0) {
                            // Maximum repetitions without downlink. Reset MacCommandsBufferIndex. Increase ADR Ack counter.
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
                        // Sends the same frame again
                        handle_delayed_tx_timer_event();
                    }
                }
            }
        }

        if (_params.flags.bits.mcps_ind == 1) {
            // Procedure if we received a frame
            if ((mcps.get_confirmation().ack_received == true) ||
                (_params.ack_timeout_retry_counter > _params.max_ack_timeout_retries)) {
                _params.is_ack_retry_timeout_expired = false;
                _params.is_node_ack_requested = false;
                if (_params.is_ul_frame_counter_fixed == false) {
                    _params.ul_frame_counter++;
                }
                mcps.get_confirmation().nb_retries = _params.ack_timeout_retry_counter;

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
                    get_phy.attribute = PHY_NEXT_LOWER_TX_DR;
                    get_phy.datarate = _params.sys_params.channel_data_rate;
                    phy_param = lora_phy->get_phy_params( &get_phy );
                    _params.sys_params.channel_data_rate = phy_param.value;
                }

                // Try to send the frame again
                if (schedule_tx() == LORAWAN_STATUS_OK) {
                    _params.flags.bits.mac_done = 0;
                } else {
                    // The DR is not applicable for the payload size
                    mcps.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_TX_DR_PAYLOAD_SIZE_ERROR;

                    mac_commands.clear_command_buffer();
                    _params.mac_state &= ~LORAMAC_TX_RUNNING;
                    _params.is_node_ack_requested = false;
                    mcps.get_confirmation().ack_received = false;
                    mcps.get_confirmation().nb_retries = _params.ack_timeout_retry_counter;
                    mcps.get_confirmation().data_rate = _params.sys_params.channel_data_rate;

                    if (_params.is_ul_frame_counter_fixed == false) {
                        _params.ul_frame_counter++;
                    }
                }
            } else {
                lora_phy->restore_default_channels();

                _params.mac_state &= ~LORAMAC_TX_RUNNING;

                mac_commands.clear_command_buffer();
                _params.is_node_ack_requested = false;
                mcps.get_confirmation().ack_received = false;
                mcps.get_confirmation().nb_retries = _params.ack_timeout_retry_counter;

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
            mac_primitives->mcps_confirm(&mcps.get_confirmation());
        }

        if (_params.flags.bits.mlme_req == 1) {
            _params.flags.bits.mlme_req = 0;
            mac_primitives->mlme_confirm(&mlme.get_confirmation());
        }

        // Verify if sticky MAC commands are pending or not
        if (mac_commands.is_sticky_mac_command_pending() == true) {
            // Setup MLME indication
            set_mlme_schedule_ul_indication();
        }

        // Procedure done. Reset variables.
        _params.flags.bits.mac_done = 0;
    } else {
        // Operation not finished restart timer
        _lora_time.start(_params.timers.mac_state_check_timer, MAC_STATE_CHECK_TIMEOUT);
    }

    // Handle MCPS indication
    if (_params.flags.bits.mcps_ind == 1) {
        _params.flags.bits.mcps_ind = 0;

        if (_params.dev_class== CLASS_C) {
            // Activate RX2 window for Class C
            open_continuous_rx2_window();
        }

        if (_params.flags.bits.mcps_ind_skip == 0) {
            mac_primitives->mcps_indication(&mcps.get_indication());
        }

        _params.flags.bits.mcps_ind_skip = 0;
    }

    // Handle MLME indication
    if (_params.flags.bits.mlme_ind == 1) {
        _params.flags.bits.mlme_ind = 0;
        mac_primitives->mlme_indication(&mlme.get_indication());
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
        (mlme.get_confirmation().req_type == MLME_JOIN)) {

        reset_mac_parameters();

        _params.sys_params.channel_data_rate = lora_phy->get_alternate_DR(_params.join_request_trial_counter + 1);

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

    if (_params.dev_class == CLASS_C) {
        lora_phy->put_radio_to_standby();
    }

    lora_phy->rx_config(&_params.rx_window1_config,
                        (int8_t*) &mcps.get_indication().rx_datarate);

    rx_window_setup(_params.rx_window1_config.is_rx_continuous,
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

    if (_params.dev_class != CLASS_C) {
        _params.rx_window2_config.is_rx_continuous = false;
    } else {
        // Setup continuous listening for class c
        _params.rx_window2_config.is_rx_continuous = true;
    }

    if (lora_phy->rx_config(&_params.rx_window2_config,
                            (int8_t*) &mcps.get_indication().rx_datarate) == true) {

        rx_window_setup(_params.rx_window2_config.is_rx_continuous,
                        _params.sys_params.max_rx_win_time);

        _params.rx_slot = RX_SLOT_WIN_2;
    }
}

void LoRaMac::on_ack_timeout_timer_event(void)
{
    _lora_time.stop(_params.timers.ack_timeout_timer);

    if (_params.is_node_ack_requested == true) {
        _params.is_ack_retry_timeout_expired = true;
        _params.mac_state &= ~LORAMAC_ACK_REQ;
    }
    if (_params.dev_class == CLASS_C) {
        _params.flags.bits.mac_done = 1;
    }
}

void LoRaMac::rx_window_setup(bool rx_continuous, uint32_t max_rx_window_time)
{
    lora_phy->setup_rx_window(rx_continuous, max_rx_window_time);
}

bool LoRaMac::validate_payload_length(uint8_t length, int8_t datarate,
                                      uint8_t fopts_len)
{
    get_phy_params_t get_phy;
    phy_param_t phy_param;
    uint16_t max_value = 0;
    uint16_t payloadSize = 0;

    // Setup PHY request
    get_phy.datarate = datarate;
    get_phy.attribute = PHY_MAX_PAYLOAD;

    // Get the maximum payload length
    if (_params.is_repeater_supported == true) {
        get_phy.attribute = PHY_MAX_PAYLOAD_REPEATER;
    }
    phy_param = lora_phy->get_phy_params(&get_phy);
    max_value = phy_param.value;

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
    mlme.get_indication().indication_type = MLME_SCHEDULE_UPLINK;
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

    // Prepare the frame
    lorawan_status_t status = prepare_frame(machdr, &fctrl, fport, fbuffer,
                                            fbuffer_size);

    // Validate status
    if (status != LORAWAN_STATUS_OK) {
        return status;
    }

    // Reset confirm parameters
    mcps.get_confirmation().nb_retries = 0;
    mcps.get_confirmation().ack_received = false;
    mcps.get_confirmation().ul_frame_counter = _params.ul_frame_counter;

    status = schedule_tx();

    return status;
}

lorawan_status_t LoRaMac::schedule_tx(void)
{
    lorawan_time_t dutyCycleTimeOff = 0;
    channel_selection_params_t nextChan;
    get_phy_params_t getPhy;
    phy_param_t phyParam;

    // Check if the device is off
    if (_params.sys_params.max_duty_cycle == 255) {
        return LORAWAN_STATUS_DEVICE_OFF;
    }

    if (_params.sys_params.max_duty_cycle == 0) {
        _params.timers.aggregated_timeoff = 0;
    }

    // Update Backoff
    calculate_backOff(_params.last_channel_idx);

    nextChan.aggregate_timeoff = _params.timers.aggregated_timeoff;
    nextChan.current_datarate = _params.sys_params.channel_data_rate;
    _params.is_dutycycle_on = MBED_CONF_LORA_DUTY_CYCLE_ON;
    nextChan.dc_enabled = _params.is_dutycycle_on;
    nextChan.joined = _params.is_nwk_joined;
    nextChan.last_aggregate_tx_time = _params.timers.aggregated_last_tx_time;

    // Select channel
    while (lora_phy->set_next_channel(&nextChan, &_params.channel,
                                      &dutyCycleTimeOff,
                                      &_params.timers.aggregated_timeoff) == false) {
        // Set the default datarate
        getPhy.attribute = PHY_DEF_TX_DR;
        phyParam = lora_phy->get_phy_params(&getPhy);
        _params.sys_params.channel_data_rate = phyParam.value;
        // Update datarate in the function parameters
        nextChan.current_datarate = _params.sys_params.channel_data_rate;
    }

    tr_debug("Next Channel Idx=%d, DR=%d", _params.channel, nextChan.current_datarate);

    // Compute Rx1 windows parameters
    uint8_t dr_offset = lora_phy->apply_DR_offset(_params.sys_params.channel_data_rate,
                                                  _params.sys_params.rx1_dr_offset);

    lora_phy->compute_rx_win_params(dr_offset, _params.sys_params.min_rx_symb,
                                    _params.sys_params.max_sys_rx_error,
                                    &_params.rx_window1_config);

    // Compute Rx2 windows parameters
    lora_phy->compute_rx_win_params(_params.sys_params.rx2_channel.datarate,
                                    _params.sys_params.min_rx_symb,
                                    _params.sys_params.max_sys_rx_error,
                                    &_params.rx_window2_config);

    if (_params.is_nwk_joined == false) {
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

    // Schedule transmission of frame
    if (dutyCycleTimeOff == 0) {
        // Try to send now
        return send_frame_on_channel(_params.channel);
    } else {
        // Send later - prepare timer
        _params.mac_state |= LORAMAC_TX_DELAYED;
        tr_debug("Next Transmission in %lu ms", dutyCycleTimeOff);

        _lora_time.start(_params.timers.tx_delayed_timer, dutyCycleTimeOff);

        return LORAWAN_STATUS_OK;
    }
}

void LoRaMac::calculate_backOff(uint8_t channel)
{
    backoff_params_t backoff_params;

    backoff_params.joined = _params.is_nwk_joined;
    _params.is_dutycycle_on = MBED_CONF_LORA_DUTY_CYCLE_ON;
    backoff_params.dc_enabled = _params.is_dutycycle_on;
    backoff_params.channel = channel;
    backoff_params.elapsed_time = _lora_time.get_elapsed_time(_params.timers.mac_init_time);
    backoff_params.tx_toa = _params.timers.tx_toa;
    backoff_params.last_tx_was_join_req = _params.is_last_tx_join_request;

    // Update regional back-off
    lora_phy->calculate_backoff(&backoff_params);

    // Update aggregated time-off
    _params.timers.aggregated_timeoff = _params.timers.aggregated_timeoff
            + (_params.timers.tx_toa * _params.sys_params.aggregated_duty_cycle
            - _params.timers.tx_toa);
}

void LoRaMac::reset_mac_parameters(void)
{
    get_phy_params_t get_phy;
    phy_param_t phy_param;

    _params.is_nwk_joined = false;

    // Counters
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

    get_phy.attribute = PHY_DEF_TX_POWER;
    phy_param = lora_phy->get_phy_params(&get_phy);

    _params.sys_params.channel_tx_power = phy_param.value;

    get_phy.attribute = PHY_DEF_TX_DR;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.channel_data_rate = phy_param.value;

    get_phy.attribute = PHY_DEF_DR1_OFFSET;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.rx1_dr_offset = phy_param.value;

    get_phy.attribute = PHY_DEF_RX2_FREQUENCY;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.rx2_channel.frequency = phy_param.value;
    get_phy.attribute = PHY_DEF_RX2_DR;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.rx2_channel.datarate = phy_param.value;

    get_phy.attribute = PHY_DEF_UPLINK_DWELL_TIME;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.uplink_dwell_time = phy_param.value;

    get_phy.attribute = PHY_DEF_MAX_EIRP;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.max_eirp = phy_param.f_value;

    get_phy.attribute = PHY_DEF_ANTENNA_GAIN;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.antenna_gain = phy_param.f_value;

    _params.is_node_ack_requested = false;
    _params.is_srv_ack_requested = false;

    // Reset Multicast downlink counters
    multicast_params_t *cur = _params.multicast_channels;
    while (cur != NULL) {
        cur->dl_frame_counter = 0;
        cur = cur->next;
    }

    // Initialize channel index.
    _params.channel = 0;
    _params.last_channel_idx = _params.channel;
}

bool LoRaMac::is_fPort_allowed (uint8_t fPort)
{
    if ((fPort == 0) || (fPort > 224)) {
        return false;
    }
    return true;
}

void LoRaMac::open_continuous_rx2_window (void)
{
    handle_rx2_timer_event();
    _params.rx_slot = RX_SLOT_WIN_CLASS_C;
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

            _params.dev_nonce = lora_phy->get_radio_rng();

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
            if (_params.is_nwk_joined == false) {
                // No network has been joined yet
                return LORAWAN_STATUS_NO_NETWORK_JOINED;
            }

            if (_params.sys_params.adr_on) {
                if (lora_phy->get_next_ADR(true,
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

            // Copy the MAC commands which must be re-send into the MAC command buffer
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

            // Store MAC commands which must be re-send in case the device does not receive a downlink anymore
            mac_commands.parse_mac_commands_to_repeat();

            if ((payload != NULL) && (_params.payload_length > 0)) {
                _params.buffer[pkt_header_len++] = frame_port;

                if (frame_port == 0) {
                    // Reset buffer index as the mac commands are being sent on port 0
                    mac_commands.clear_command_buffer();
                    if (0 != encrypt_payload((uint8_t*) payload, _params.payload_length,
                                                   _params.keys.nwk_skey, _params.dev_addr,
                                                   UP_LINK,
                                                   _params.ul_frame_counter,
                                                   &_params.buffer[pkt_header_len])) {
                        status = LORAWAN_STATUS_CRYPTO_FAIL;
                    }
                } else {
                    if (0 != encrypt_payload((uint8_t*) payload, _params.payload_length,
                                                   _params.keys.app_skey, _params.dev_addr,
                                                   UP_LINK,
                                                   _params.ul_frame_counter,
                                                   &_params.buffer[pkt_header_len])) {
                        status = LORAWAN_STATUS_CRYPTO_FAIL;
                    }
                }
            }

            _params.buffer_pkt_len = pkt_header_len + _params.payload_length;

            if (0 != compute_mic(_params.buffer, _params.buffer_pkt_len,
                                       _params.keys.nwk_skey,
                                       _params.dev_addr,
                                       UP_LINK,
                                       _params.ul_frame_counter, &mic)) {
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

    lora_phy->tx_config(&tx_config, &tx_power, &_params.timers.tx_toa);

    mlme.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_ERROR;

    mcps.get_confirmation().status = LORAMAC_EVENT_INFO_STATUS_ERROR;
    mcps.get_confirmation().data_rate = _params.sys_params.channel_data_rate;
    mcps.get_confirmation().tx_power = tx_power;

    // Store the time on air
    mcps.get_confirmation().tx_toa = _params.timers.tx_toa;
    mlme.get_confirmation().tx_toa = _params.timers.tx_toa;

    // Starts the MAC layer status check timer
    _lora_time.start(_params.timers.mac_state_check_timer,
                     MAC_STATE_CHECK_TIMEOUT);

    if (_params.is_nwk_joined == false) {
        _params.join_request_trial_counter++;
    }

    // Send now
    lora_phy->handle_send(_params.buffer, _params.buffer_pkt_len);

    _params.mac_state |= LORAMAC_TX_RUNNING;

    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMac::set_tx_continuous_wave(uint16_t timeout)
{
    cw_mode_params_t continuous_wave;

    continuous_wave.channel = _params.channel;
    continuous_wave.datarate = _params.sys_params.channel_data_rate;
    continuous_wave.tx_power = _params.sys_params.channel_tx_power;
    continuous_wave.max_eirp = _params.sys_params.max_eirp;
    continuous_wave.antenna_gain = _params.sys_params.antenna_gain;
    continuous_wave.timeout = timeout;

    lora_phy->set_tx_cont_mode(&continuous_wave);

    // Starts the MAC layer status check timer
    _lora_time.start(_params.timers.mac_state_check_timer,
                     MAC_STATE_CHECK_TIMEOUT);

    _params.mac_state |= LORAMAC_TX_RUNNING;

    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMac::set_tx_continuous_wave1(uint16_t timeout,
                                                  uint32_t frequency,
                                                  uint8_t power)
{
    cw_mode_params_t continuous_wave;

    continuous_wave.channel = 0;
    continuous_wave.datarate = 0;
    continuous_wave.tx_power = power;
    continuous_wave.max_eirp = 0;
    continuous_wave.antenna_gain = 0;
    continuous_wave.timeout = timeout;

    lora_phy->set_tx_cont_mode(&continuous_wave);

    // Starts the MAC layer status check timer
    _lora_time.start(_params.timers.mac_state_check_timer,
                     MAC_STATE_CHECK_TIMEOUT);

    _params.mac_state |= LORAMAC_TX_RUNNING;

    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMac::initialize(loramac_primitives_t *primitives,
                                     LoRaPHY *phy, EventQueue *queue)
{
    get_phy_params_t get_phy;
    phy_param_t phy_param;

    //store event queue pointer
    ev_queue = queue;

    if (!primitives) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    lora_phy = phy;

    // Activate MLME subsystem
    mlme.activate_mlme_subsystem(this, lora_phy, &mac_commands);

    // Activate MCPS subsystem
    mcps.activate_mcps_subsystem(this, lora_phy);

    // Activate MIB subsystem
    mib.activate_mib_subsystem(this, lora_phy);

    // Activate channel planning subsystem
    channel_plan.activate_channelplan_subsystem(lora_phy, &mib);

    mac_primitives = primitives;

    _params.flags.value = 0;

    _params.dev_class = CLASS_A;
    _params.mac_state = LORAMAC_IDLE;

    _params.join_request_trial_counter = 0;
    _params.max_join_request_trials = 1;
    _params.is_repeater_supported = false;

    // Reset duty cycle times
    _params.timers.aggregated_last_tx_time = 0;
    _params.timers.aggregated_timeoff = 0;

    // Reset to defaults
    get_phy.attribute = PHY_DUTY_CYCLE;
    phy_param = lora_phy->get_phy_params(&get_phy);
    // load default at this moment. Later can be changed using json
    _params.is_dutycycle_on = (bool) phy_param.value;

    get_phy.attribute = PHY_DEF_TX_POWER;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.channel_tx_power = phy_param.value;

    get_phy.attribute = PHY_DEF_TX_DR;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.channel_data_rate = phy_param.value;

    get_phy.attribute = PHY_MAX_RX_WINDOW;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.max_rx_win_time = phy_param.value;

    get_phy.attribute = PHY_RECEIVE_DELAY1;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.recv_delay1 = phy_param.value;

    get_phy.attribute = PHY_RECEIVE_DELAY2;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.recv_delay2 = phy_param.value;

    get_phy.attribute = PHY_JOIN_ACCEPT_DELAY1;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.join_accept_delay1 = phy_param.value;

    get_phy.attribute = PHY_JOIN_ACCEPT_DELAY2;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.join_accept_delay2 = phy_param.value;

    get_phy.attribute = PHY_DEF_DR1_OFFSET;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.rx1_dr_offset = phy_param.value;

    get_phy.attribute = PHY_DEF_RX2_FREQUENCY;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.rx2_channel.frequency = phy_param.value;

    get_phy.attribute = PHY_DEF_RX2_DR;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.rx2_channel.datarate = phy_param.value;

    get_phy.attribute = PHY_DEF_UPLINK_DWELL_TIME;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.uplink_dwell_time = phy_param.value;

    get_phy.attribute = PHY_DEF_DOWNLINK_DWELL_TIME;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.downlink_dwell_time = phy_param.value;

    get_phy.attribute = PHY_DEF_MAX_EIRP;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.max_eirp = phy_param.f_value;

    get_phy.attribute = PHY_DEF_ANTENNA_GAIN;
    phy_param = lora_phy->get_phy_params(&get_phy);
    _params.sys_params.antenna_gain = phy_param.f_value;

    // Init parameters which are not set in function ResetMacParameters
    _params.sys_params.max_sys_rx_error = 10;
    _params.sys_params.min_rx_symb = 6;
    _params.sys_params.retry_num = 1;

    reset_mac_parameters();

    // Random seed initialization
    srand(lora_phy->get_radio_rng());

    _params.is_nwk_public = MBED_CONF_LORA_PUBLIC_NETWORK;
    lora_phy->setup_public_network_mode(_params.is_nwk_public);
    lora_phy->put_radio_to_sleep();

    // Initialize timers
    _lora_time.init(_params.timers.mac_state_check_timer,
                    mbed::callback(this, &LoRaMac::handle_mac_state_check_timer_event));
    _lora_time.init(_params.timers.tx_delayed_timer,
                    mbed::callback(this, &LoRaMac::handle_delayed_tx_timer_event));
    _lora_time.init(_params.timers.rx_window1_timer,
                    mbed::callback(this, &LoRaMac::handle_rx1_timer_event));
    _lora_time.init(_params.timers.rx_window2_timer,
                    mbed::callback(this, &LoRaMac::handle_rx2_timer_event));
    _lora_time.init(_params.timers.ack_timeout_timer,
                    mbed::callback(this, &LoRaMac::handle_ack_timeout));

    // Store the current initialization time
    _params.timers.mac_init_time = _lora_time.get_current_time();

    return LORAWAN_STATUS_OK;
}

void LoRaMac::disconnect()
{
    // Cancel all timers
    _lora_time.stop(_params.timers.mac_state_check_timer);
    _lora_time.stop(_params.timers.tx_delayed_timer);
    _lora_time.stop(_params.timers.rx_window1_timer);
    _lora_time.stop(_params.timers.rx_window2_timer);
    _lora_time.stop(_params.timers.ack_timeout_timer);

    // Put radio to sleep
    lora_phy->put_radio_to_sleep();

    // Reset internal state
    _params.is_nwk_joined = false;
    _params.is_ack_retry_timeout_expired = false;
    _params.is_rx_window_enabled = true;
    _params.is_node_ack_requested = false;
    _params.is_srv_ack_requested = false;
    _params.flags.value = 0;
    _params.mac_state = 0;

    // Clear MAC commands
    mac_commands.clear_command_buffer();
    mac_commands.clear_repeat_buffer();
    mac_commands.clear_mac_commands_in_next_tx();

    // Set internal state to idle.
    _params.mac_state = LORAMAC_IDLE;
}

lorawan_status_t LoRaMac::query_tx_possible(uint8_t size,
                                            loramac_tx_info_t* tx_info)
{
    get_phy_params_t get_phy;
    phy_param_t phy_param;

    uint8_t fopt_len = mac_commands.get_mac_cmd_length()
            + mac_commands.get_repeat_commands_length();

    if (tx_info == NULL) {
        return LORAWAN_STATUS_PARAMETER_INVALID;
    }

    // if applicaion has turned on ADR, we want to opt it out
    if (_params.sys_params.adr_on) {
        lora_phy->get_next_ADR(false, _params.sys_params.channel_data_rate,
                               _params.sys_params.channel_tx_power,
                               _params.adr_ack_counter);
    }

    // Setup PHY request
    get_phy.datarate = _params.sys_params.channel_data_rate;
    get_phy.attribute = PHY_MAX_PAYLOAD;

    // Change request in case repeater is supported
    if (_params.is_repeater_supported == true) {
        get_phy.attribute = PHY_MAX_PAYLOAD_REPEATER;
    }
    phy_param = lora_phy->get_phy_params(&get_phy);
    tx_info->current_payload_size = phy_param.value;

    // Verify if the fOpts fit into the maximum payload
    if (tx_info->current_payload_size >= fopt_len) {
        tx_info->max_possible_payload_size = tx_info->current_payload_size - fopt_len;
    } else {
        tx_info->max_possible_payload_size = tx_info->current_payload_size;
        // The fOpts don't fit into the maximum payload. Omit the MAC commands to
        // ensure that another uplink is possible.
        fopt_len = 0;
        mac_commands.clear_command_buffer();
        mac_commands.clear_repeat_buffer();
    }

    // Verify if the fOpts and the payload fit into the maximum payload
    if (validate_payload_length(size, _params.sys_params.channel_data_rate,
                                fopt_len) == false) {
        return LORAWAN_STATUS_LENGTH_ERROR;
    }
    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMac::add_channel_plan(const lorawan_channelplan_t& plan)
{
    // Validate if the MAC is in a correct state
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
    return channel_plan.get_plan(plan, &_params);
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

    // Reset downlink counter
    channel_param->dl_frame_counter = 0;

    if (_params.multicast_channels == NULL) {
        // New node is the fist element
        _params.multicast_channels = channel_param;
    } else {
        multicast_params_t *cur = _params.multicast_channels;

        // Search the last node in the list
        while (cur->next != NULL) {
            cur = cur->next;
        }
        // This function always finds the last node
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
            // First element
            _params.multicast_channels = channel_param->next;
        } else {
            multicast_params_t *cur = _params.multicast_channels;

            // Search the node in the list
            while (cur->next && cur->next != channel_param) {
                cur = cur->next;
            }
            // If we found the node, remove it
            if (cur->next) {
                cur->next = channel_param->next;
            }
        }
        channel_param->next = NULL;
    }

    return LORAWAN_STATUS_OK;
}

lorawan_status_t LoRaMac::mlme_request( loramac_mlme_req_t *mlmeRequest )
{
    return mlme.set_request(mlmeRequest, &_params);
}

lorawan_status_t LoRaMac::mcps_request( loramac_mcps_req_t *mcpsRequest )
{
    if (_params.mac_state != LORAMAC_IDLE) {
        return LORAWAN_STATUS_BUSY;
    }

    return mcps.set_request(mcpsRequest, &_params);
}

lorawan_status_t LoRaMac::mib_get_request_confirm( loramac_mib_req_confirm_t *mibGet )
{
    return mib.get_request(mibGet, &_params);
}

lorawan_status_t LoRaMac::mib_set_request_confirm( loramac_mib_req_confirm_t *mibSet )
{
    return mib.set_request(mibSet, &_params);
}

radio_events_t *LoRaMac::get_phy_event_handlers()
{
    radio_events.tx_done = mbed::callback(this, &LoRaMac::handle_tx_done);
    radio_events.rx_done = mbed::callback(this, &LoRaMac::handle_rx_done);
    radio_events.rx_error = mbed::callback(this, &LoRaMac::handle_rx_error);
    radio_events.tx_timeout = mbed::callback(this, &LoRaMac::handle_tx_timeout);
    radio_events.rx_timeout = mbed::callback(this, &LoRaMac::handle_rx_timeout);

    return &radio_events;
}

#if defined(LORAWAN_COMPLIANCE_TEST)
/***************************************************************************
 * Compliance testing                                                      *
 **************************************************************************/

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
    VerifyParams_t verify;

    verify.DutyCycle = enable;

    if(lora_phy->verify(&verify, PHY_DUTY_CYCLE) == true)
    {
        _params.is_dutycycle_on = enable;
    }
}

void LoRaMac::LoRaMacTestSetChannel( uint8_t channel )
{
    _params.channel = channel;
}
#endif
