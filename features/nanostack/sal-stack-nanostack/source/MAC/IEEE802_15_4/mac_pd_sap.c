/*
 * Copyright (c) 2014-2018, Arm Limited and affiliates.
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
#include "nsconfig.h"
#include "ns_types.h"
#include "eventOS_event.h"
#include "string.h"
#include "nsdynmemLIB.h"
#include "randLIB.h"
#include "ccmLIB.h"
#include "common_functions.h"
#include "platform/arm_hal_interrupt.h"
#include "mac_api.h"
#include "fhss_api.h"
#include "MAC/IEEE802_15_4/sw_mac_internal.h"
#include "MAC/IEEE802_15_4/mac_pd_sap.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/IEEE802_15_4/mac_header_helper_functions.h"
#include "MAC/IEEE802_15_4/mac_timer.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "MAC/IEEE802_15_4/mac_filter.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/rf_driver_storage.h"

/* Define TX Timeot Period */
// Hardcoded to 1200ms. Should be changed dynamic: (FHSS) channel retries needs longer timeout
#define NWKTX_TIMEOUT_PERIOD (1200*20)
// Measured 3750us with 1280 byte secured packet from calculating TX time to starting CSMA timer on PHY.
// Typically varies from 500us to several milliseconds depending on packet size and the platform.
// MAC should learn and make this dynamic by sending first few packets with predefined CSMA period.
#define MIN_FHSS_CSMA_PERIOD_US    4000

static int8_t mac_data_interface_tx_done_cb(protocol_interface_rf_mac_setup_s *rf_ptr, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry);
static void  mac_sap_cca_fail_cb(protocol_interface_rf_mac_setup_s *rf_ptr);

void mac_csma_param_init(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    rf_mac_setup->macCurrentBE = rf_mac_setup->macMinBE;
}

static void mac_csma_BE_update(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    if (rf_mac_setup->macCurrentBE < rf_mac_setup->macMaxBE) {
        rf_mac_setup->macCurrentBE++;
    }
}

// 8-bit because maxBE is maximum 8 (according to 802.15.4)
static uint8_t mac_csma_random_backoff_get(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    return randLIB_get_random_in_range(0, (1 << rf_mac_setup->macCurrentBE) - 1);
}

static uint16_t mac_csma_backoff_period_convert_to50us(uint8_t random, uint8_t backoff_period_in_10us)
{
    return (random * backoff_period_in_10us) / 5;
}

void mac_csma_backoff_start(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    uint8_t backoff = mac_csma_random_backoff_get(rf_mac_setup);
    uint16_t backoff_slots = mac_csma_backoff_period_convert_to50us(backoff, rf_mac_setup->backoff_period_in_10us);
    if (backoff_slots == 0) {
        backoff_slots = 1;
    }

    timer_mac_start(rf_mac_setup, MAC_TIMER_CCA, backoff_slots);
}


uint32_t mac_csma_backoff_get(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    uint8_t backoff = mac_csma_random_backoff_get(rf_mac_setup);
    uint32_t backoff_in_us;
    //Multiple aUnitBackoffPeriod symbol time
    if (rf_mac_setup->rf_csma_extension_supported) {
        backoff_in_us = backoff * rf_mac_setup->aUnitBackoffPeriod * rf_mac_setup->symbol_time_us;
    } else {
        backoff_in_us = backoff * rf_mac_setup->backoff_period_in_10us * 10;
    }

    if (backoff_in_us == 0) {
        backoff_in_us = 1;
    }
    if (rf_mac_setup->fhss_api) {
        // Synchronization error when backoff time is shorter than allowed.
        // TODO: Make this dynamic.
        if (backoff_in_us < MIN_FHSS_CSMA_PERIOD_US) {
            backoff_in_us += MIN_FHSS_CSMA_PERIOD_US;
        }
    }
    return backoff_in_us;
}


/*
 * \file mac_pd_sap.c
 * \brief Add short description about this file!!!
 *
 */
static bool mac_data_interface_read_last_ack_pending_status(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    uint8_t pending = 1;
    phy_device_driver_s *dev_driver = rf_mac_setup->dev_driver->phy_driver;
    if (dev_driver->extension) {
        dev_driver->extension(PHY_EXTENSION_READ_LAST_ACK_PENDING_STATUS, &pending);
    }


    return pending;

}

static void mac_tx_done_state_set(protocol_interface_rf_mac_setup_s *rf_ptr, mac_event_t event)
{
    rf_ptr->mac_tx_result = event;

    if (event == MAC_TX_DONE  || event == MAC_TX_DONE_PENDING) {

    } else {
        rf_ptr->macTxRequestAck = false;
    }
    rf_ptr->macRfRadioTxActive = false;
    rf_ptr->macTxProcessActive = false;
    mcps_sap_pd_confirm(rf_ptr);
}

static void mac_data_interface_tx_to_cb(protocol_interface_rf_mac_setup_s *rf_ptr)
{
    rf_ptr->macRfRadioTxActive = false;
    mac_tx_done_state_set(rf_ptr, MAC_TX_TIMEOUT);
}

int8_t mac_plme_cca_req(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    dev_driver_tx_buffer_s *tx_buf = &rf_mac_setup->dev_driver_tx_buffer;
    phy_device_driver_s *dev_driver = rf_mac_setup->dev_driver->phy_driver;

    rf_mac_setup->macRfRadioTxActive = true;
    if (dev_driver->arm_net_virtual_tx_cb) {
        if (dev_driver->tx(tx_buf->buf, tx_buf->len, 1, PHY_LAYER_PAYLOAD) == 0) {
            timer_mac_start(rf_mac_setup, MAC_TX_TIMEOUT, NWKTX_TIMEOUT_PERIOD);  /*Start Timeout timer for virtual packet loss*/
        } else {
            rf_mac_setup->macRfRadioTxActive = false;
            mac_data_interface_tx_to_cb(rf_mac_setup);
        }
        return 0;
    }

    uint8_t *buffer;
    uint16_t length;
    if (rf_mac_setup->mac_ack_tx_active) {
        buffer = tx_buf->enhanced_ack_buf;
        length = tx_buf->ack_len;
    } else {
        buffer = tx_buf->buf;
        length = tx_buf->len;
    }
    if (dev_driver->tx(buffer, length, 1, PHY_LAYER_PAYLOAD) == 0) {
        return 0;
    }

    rf_mac_setup->macRfRadioTxActive = false;
    return -1;
}

/**
 * Send a buffer to the MAC.
 * Used by the protocol core.
 *
 * \param buf pointer to buffer
 *
 */
int8_t mac_pd_sap_req(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    if (!rf_mac_setup || !rf_mac_setup->macUpState || !rf_mac_setup->active_pd_data_request) {
        return -1;
    }

    rf_mac_setup->mac_cca_retry = 0;
    rf_mac_setup->mac_tx_retry = 0;
    rf_mac_setup->macTxProcessActive = true;
    mac_csma_param_init(rf_mac_setup);
    mac_csma_backoff_start(rf_mac_setup);
    return 0;
}


/**
 * Set PHY TX time.
 *
 * \param rf_mac_setup pointer to MAC.
 * \param tx_time TX timestamp to be set.
 *
 */
void mac_pd_sap_set_phy_tx_time(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint32_t tx_time, bool cca_enabled)
{
    // With TX time set to zero, PHY sends immediately
    if (!tx_time) {
        tx_time++;
    }
    phy_csma_params_t csma_params;
    csma_params.backoff_time = tx_time;
    csma_params.cca_enabled = cca_enabled;
    rf_mac_setup->dev_driver->phy_driver->extension(PHY_EXTENSION_SET_CSMA_PARAMETERS, (uint8_t *) &csma_params);
}

/**
 * Get PHY RX time.
 *
 * \param rf_mac_setup pointer to MAC
 * \return Timestamp of last PHY reception
 *
 */
static uint32_t mac_pd_sap_get_phy_rx_time(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    uint8_t rx_time_buffer[4];
    rf_mac_setup->dev_driver->phy_driver->extension(PHY_EXTENSION_READ_RX_TIME, rx_time_buffer);
    return common_read_32_bit(rx_time_buffer);
}

/**
 * Run Mac data interface state Machine for mac timer.
 *
 */
void mac_pd_sap_state_machine(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{

    if (rf_mac_setup->macUpState && rf_mac_setup->macTxProcessActive) {

        if (rf_mac_setup->mac_tx_result == MAC_TIMER_CCA) {

            if (rf_mac_setup->rf_csma_extension_supported) {
                mac_sap_cca_fail_cb(rf_mac_setup);
                return;
            }

            if (rf_mac_setup->fhss_api) {
                uint8_t *synch_info = NULL;
                mac_pre_build_frame_t *active_buf = rf_mac_setup->active_pd_data_request;
                if (!active_buf) {
                    return;
                }
                bool cca_enabled;
                if (active_buf->fcf_dsn.frametype == MAC_FRAME_ACK) {
                    cca_enabled = false;
                } else {
                    cca_enabled = true;
                }

                mac_pd_sap_set_phy_tx_time(rf_mac_setup, active_buf->tx_time, cca_enabled);
                if (active_buf->fcf_dsn.frametype == FC_BEACON_FRAME) {
                    // FHSS synchronization info is written in the end of transmitted (Beacon) buffer
                    dev_driver_tx_buffer_s *tx_buf = &rf_mac_setup->dev_driver_tx_buffer;
                    synch_info = tx_buf->buf + rf_mac_setup->dev_driver->phy_driver->phy_header_length + tx_buf->len - FHSS_SYNCH_INFO_LENGTH;
                    rf_mac_setup->fhss_api->write_synch_info(rf_mac_setup->fhss_api, synch_info, 0, FHSS_SYNCH_FRAME, 0);
                }
                // Change to destination channel and write synchronization info to Beacon frames here
                int tx_handle_retval = rf_mac_setup->fhss_api->tx_handle(rf_mac_setup->fhss_api, !mac_is_ack_request_set(active_buf),
                                                                         active_buf->DstAddr, mac_convert_frame_type_to_fhss(active_buf->fcf_dsn.frametype),
                                                                         active_buf->mac_payload_length, rf_mac_setup->dev_driver->phy_driver->phy_header_length,
                                                                         rf_mac_setup->dev_driver->phy_driver->phy_tail_length, active_buf->tx_time);
                // When FHSS TX handle returns -1, transmission of the packet is currently not allowed -> restart CCA timer
                if (tx_handle_retval == -1) {
                    timer_mac_start(rf_mac_setup, MAC_TIMER_CCA, randLIB_get_random_in_range(20, 400) + 1);
                    return;
                }
                // When FHSS TX handle returns -3, we are trying to transmit broadcast packet on unicast channel -> push back
                // to queue by using CCA fail event
                if (tx_handle_retval == -3) {
                    mac_tx_done_state_set(rf_mac_setup, MAC_CCA_FAIL);
                    return;
                } else if (tx_handle_retval == -2) {
                    mac_tx_done_state_set(rf_mac_setup, MAC_UNKNOWN_DESTINATION);
                    return;
                }
            }
            if (mac_plme_cca_req(rf_mac_setup) != 0) {
                mac_csma_backoff_start(rf_mac_setup);
            }
        } else if (rf_mac_setup->mac_tx_result == MAC_TX_TIMEOUT) {
            mac_data_interface_tx_to_cb(rf_mac_setup);
        } else if (rf_mac_setup->mac_tx_result == MAC_TIMER_ACK) {
            mac_data_interface_tx_done_cb(rf_mac_setup, PHY_LINK_TX_FAIL, 0, 0);
        }
    }
}

static void  mac_sap_cca_fail_cb(protocol_interface_rf_mac_setup_s *rf_ptr)
{
    if (rf_ptr->mac_ack_tx_active) {
        if (rf_ptr->active_pd_data_request) {
            mac_csma_backoff_start(rf_ptr);
        }
    } else {

        rf_ptr->macRfRadioTxActive = false;
        if (rf_ptr->mac_cca_retry > rf_ptr->macMaxCSMABackoffs || (rf_ptr->active_pd_data_request && rf_ptr->active_pd_data_request->asynch_request)) {
            //Send MAC_CCA_FAIL
            mac_tx_done_state_set(rf_ptr, MAC_CCA_FAIL);
        } else {
            timer_mac_stop(rf_ptr);
            mac_csma_BE_update(rf_ptr);
            if (mcps_pd_data_rebuild(rf_ptr, rf_ptr->active_pd_data_request)) {
                mac_tx_done_state_set(rf_ptr, MAC_CCA_FAIL);
            }
        }
    }
}

//static uint16_t mac_get_retry_period(protocol_interface_rf_mac_setup_s *rf_ptr)
//{
//    if (rf_ptr->fhss_api && rf_ptr->fhss_api->get_retry_period) {
//        return rf_ptr->fhss_api->get_retry_period(rf_ptr->fhss_api, rf_ptr->active_pd_data_request->DstAddr, rf_ptr->dev_driver->phy_driver->phy_MTU);
//    }
//    uint8_t backoff_length = mac_csma_random_backoff_get(rf_ptr);
//    uint16_t backoff_slots = mac_csma_backoff_period_convert_to50us(backoff_length, rf_ptr->backoff_period_in_10us);
//    if (backoff_slots == 0) {
//        backoff_slots = 1;
//    }
//    return backoff_slots;
//}

static void mac_sap_no_ack_cb(protocol_interface_rf_mac_setup_s *rf_ptr)
{
    rf_ptr->macRfRadioTxActive = false;
    if (rf_ptr->mac_tx_retry < rf_ptr->mac_mlme_retry_max) {
        rf_ptr->mac_cca_retry = 0;
        rf_ptr->mac_tx_retry++; //Update retry counter
        mac_csma_param_init(rf_ptr);
        rf_ptr->mac_tx_status.retry++;
        /*Send retry using random interval*/
        if (mcps_pd_data_rebuild(rf_ptr, rf_ptr->active_pd_data_request)) {
            mac_tx_done_state_set(rf_ptr, MAC_CCA_FAIL);
        }

    } else {
        //Send TX Fail event
        // rf_mac_setup->ip_tx_active->bad_channel = rf_mac_setup->mac_channel;
        mac_tx_done_state_set(rf_ptr, MAC_TX_FAIL);
    }
}

static bool mac_data_counter_too_small(uint32_t current_counter, uint32_t packet_counter)
{
    if ((current_counter - packet_counter) >= 2) {
        return true;
    }
    return false;
}


static bool mac_data_asynch_channel_switch(protocol_interface_rf_mac_setup_s *rf_ptr, mac_pre_build_frame_t *active_buf)
{
    if (!active_buf || !active_buf->asynch_request) {
        return false;
    }
    active_buf->asynch_channel = rf_ptr->mac_channel; //Store Original channel
    uint16_t channel = mlme_scan_analyze_next_channel(&active_buf->asynch_channel_list, true);
    if (channel <= 0xff) {
        mac_mlme_rf_channel_change(rf_ptr, channel);

    }
    return true;
}

static int8_t mac_data_interface_tx_done_cb(protocol_interface_rf_mac_setup_s *rf_ptr, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry)
{

    if (!rf_ptr->macRfRadioTxActive) {
        return -1;
    }

    if (status == PHY_LINK_CCA_PREPARE) {

        if (rf_ptr->mac_ack_tx_active) {
            return 0;
        }

        if (mac_data_asynch_channel_switch(rf_ptr, rf_ptr->active_pd_data_request)) {
            return 0;
        }

        if (rf_ptr->fhss_api) {
            mac_pre_build_frame_t *active_buf = rf_ptr->active_pd_data_request;
            if (!active_buf) {
                return -1;
            }

            if (active_buf->fcf_dsn.frametype == FC_BEACON_FRAME) {
                // FHSS synchronization info is written in the end of transmitted (Beacon) buffer
                dev_driver_tx_buffer_s *tx_buf = &rf_ptr->dev_driver_tx_buffer;
                uint8_t *synch_info = tx_buf->buf + rf_ptr->dev_driver->phy_driver->phy_header_length + tx_buf->len - FHSS_SYNCH_INFO_LENGTH;
                rf_ptr->fhss_api->write_synch_info(rf_ptr->fhss_api, synch_info, 0, FHSS_SYNCH_FRAME, 0);
            }

            // Change to destination channel and write synchronization info to Beacon frames here
            int tx_handle_retval = rf_ptr->fhss_api->tx_handle(rf_ptr->fhss_api, !mac_is_ack_request_set(active_buf),
                                                               active_buf->DstAddr, mac_convert_frame_type_to_fhss(active_buf->fcf_dsn.frametype),
                                                               active_buf->mac_payload_length, rf_ptr->dev_driver->phy_driver->phy_header_length,
                                                               rf_ptr->dev_driver->phy_driver->phy_tail_length, active_buf->tx_time);
            // When FHSS TX handle returns -1, transmission of the packet is currently not allowed -> restart CCA timer
            if (tx_handle_retval == -1) {
                mac_sap_cca_fail_cb(rf_ptr);
                return -2;
            }
            // When FHSS TX handle returns -3, we are trying to transmit broadcast packet on unicast channel -> push back
            // to queue by using CCA fail event
            if (tx_handle_retval == -3) {
                mac_tx_done_state_set(rf_ptr, MAC_CCA_FAIL);
                return -3;
            } else if (tx_handle_retval == -2) {
                mac_tx_done_state_set(rf_ptr, MAC_UNKNOWN_DESTINATION);
                return -2;
            }
        }

        return 0;
    }

    //
    bool waiting_ack = false;


    if (rf_ptr->mac_ack_tx_active) {
        rf_ptr->mac_ack_tx_active = false;
        if (rf_ptr->fhss_api) {
            //SET tx completed false because ack isnot never queued
            rf_ptr->fhss_api->data_tx_done(rf_ptr->fhss_api, false, false, 0xff);
        }
        if (rf_ptr->active_pd_data_request) {

            if (rf_ptr->active_pd_data_request->fcf_dsn.securityEnabled) {
                uint32_t current_counter = mac_mlme_framecounter_get(rf_ptr);
                if (mac_data_counter_too_small(current_counter, rf_ptr->active_pd_data_request->aux_header.frameCounter)) {
                    rf_ptr->active_pd_data_request->aux_header.frameCounter = current_counter;
                    mac_mlme_framecounter_increment(rf_ptr);
                }
            }
            //GEN TX failure
            mac_sap_cca_fail_cb(rf_ptr);
        }
        return 0;
    } else {
        // Do not update CCA count when Ack is received, it was already updated with PHY_LINK_TX_SUCCESS event
        if ((status != PHY_LINK_TX_DONE) && (status != PHY_LINK_TX_DONE_PENDING)) {
            /* For PHY_LINK_TX_SUCCESS and PHY_LINK_CCA_FAIL cca_retry must always be > 0.
             * PHY_LINK_TX_FAIL either happened during transmission or when waiting Ack -> we must use the CCA count given by PHY.
             */
            if ((cca_retry == 0) && (status != PHY_LINK_TX_FAIL)) {
                cca_retry = 1;
            }
            rf_ptr->mac_tx_status.cca_cnt += cca_retry;
            rf_ptr->mac_cca_retry += cca_retry;
        }
        rf_ptr->mac_tx_status.retry += tx_retry;
        rf_ptr->mac_tx_retry += tx_retry;
        timer_mac_stop(rf_ptr);
    }

    switch (status) {
        case PHY_LINK_TX_SUCCESS:
            if (rf_ptr->macTxRequestAck) {
                timer_mac_start(rf_ptr, MAC_TIMER_ACK, rf_ptr->mac_ack_wait_duration); /*wait for ACK 1 ms*/
                waiting_ack = true;
            } else {
                //TODO CHECK this is MAC_TX_ PERMIT OK
                mac_tx_done_state_set(rf_ptr, MAC_TX_DONE);
            }
            break;

        case PHY_LINK_CCA_FAIL:
            mac_sap_cca_fail_cb(rf_ptr);
            break;

        case PHY_LINK_TX_FAIL:
            mac_sap_no_ack_cb(rf_ptr);
            break;

        case PHY_LINK_TX_DONE:
            //mac_tx_result
            mac_tx_done_state_set(rf_ptr, MAC_TX_DONE);
            break;

        case PHY_LINK_TX_DONE_PENDING:
            mac_tx_done_state_set(rf_ptr, MAC_TX_DONE_PENDING);
            break;

        default:
            break;
    }
    if (rf_ptr->fhss_api) {
        bool tx_is_done = false;
        if (rf_ptr->mac_tx_result == MAC_TX_DONE) {
            tx_is_done = true;
        }
        if (rf_ptr->active_pd_data_request->asynch_request == false) {
            rf_ptr->fhss_api->data_tx_done(rf_ptr->fhss_api, waiting_ack, tx_is_done, rf_ptr->active_pd_data_request->msduHandle);
        }
    }
    return 0;
}


static int8_t mac_data_interface_tx_done_by_ack_cb(protocol_interface_rf_mac_setup_s *rf_ptr, mac_pre_parsed_frame_t *buf)
{

    if (!rf_ptr->macRfRadioTxActive || !rf_ptr->active_pd_data_request || rf_ptr->active_pd_data_request->fcf_dsn.DSN != buf->fcf_dsn.DSN) {
        return -1;
    }

    timer_mac_stop(rf_ptr);
    if (buf->fcf_dsn.framePending) {
        rf_ptr->mac_tx_result = MAC_TX_DONE_PENDING;
    } else {
        rf_ptr->mac_tx_result = MAC_TX_DONE;
    }
    rf_ptr->macRfRadioTxActive = false;
    rf_ptr->macTxProcessActive = false;
    mcps_sap_pd_ack(buf);

    if (rf_ptr->fhss_api) {
        rf_ptr->fhss_api->data_tx_done(rf_ptr->fhss_api, false, true, rf_ptr->active_pd_data_request->msduHandle);
    }
    return 0;
}

static bool mac_pd_sap_ack_validation(protocol_interface_rf_mac_setup_s *rf_ptr, mac_fcf_sequence_t *fcf_dsn, const uint8_t *data_ptr)
{
    if (!rf_ptr->active_pd_data_request || !rf_ptr->active_pd_data_request->fcf_dsn.ackRequested) {
        return false; //No active Data request anymore or no ACK request for current TX
    }

    if (fcf_dsn->frameVersion != rf_ptr->active_pd_data_request->fcf_dsn.frameVersion) {
        return false;
    }

    if (fcf_dsn->frameVersion == MAC_FRAME_VERSION_2015) {

        //Validate ACK SRC address mode and address to Active TX dst address
        if (rf_ptr->active_pd_data_request->fcf_dsn.DstAddrMode != fcf_dsn->SrcAddrMode) {
            return false;
        }

        if (fcf_dsn->SrcAddrMode) {
            uint8_t srcAddress[8];
            uint8_t address_length = mac_address_length(fcf_dsn->SrcAddrMode);
            mac_header_get_src_address(fcf_dsn, data_ptr, srcAddress);
            if (memcmp(srcAddress, rf_ptr->active_pd_data_request->DstAddr, address_length)) {
                return false;
            }
        }

        //Validate ACK DST address mode and address to Active TX src address
        if (rf_ptr->active_pd_data_request->fcf_dsn.SrcAddrMode != fcf_dsn->DstAddrMode) {
            return false;
        }

        if (fcf_dsn->DstAddrMode) {
            uint8_t dstAddress[8];
            uint8_t address_length = mac_address_length(fcf_dsn->DstAddrMode);
            mac_header_get_dst_address(fcf_dsn, data_ptr, dstAddress);
            if (memcmp(dstAddress, rf_ptr->active_pd_data_request->SrcAddr, address_length)) {
                return false;
            }
        }

        if (rf_ptr->active_pd_data_request->fcf_dsn.sequenceNumberSuppress != fcf_dsn->sequenceNumberSuppress) {
            return false; //sequence number validation not correct
        }

        if (!fcf_dsn->sequenceNumberSuppress && (rf_ptr->active_pd_data_request->fcf_dsn.DSN != fcf_dsn->DSN)) {
            return false;
        }
        return true;
    }

    if (rf_ptr->active_pd_data_request->fcf_dsn.DSN != fcf_dsn->DSN) {
        return false;
    }
    return true;
}

int8_t mac_pd_sap_data_cb(void *identifier, arm_phy_sap_msg_t *message)
{
    protocol_interface_rf_mac_setup_s *rf_ptr = (protocol_interface_rf_mac_setup_s *)identifier;
    mac_pre_parsed_frame_t *buffer = NULL;

    if (!rf_ptr || !message) {
        return -1;
    }

    if (!rf_ptr->macUpState) {
        return -2;
    }

    if (message->id == MAC15_4_PD_SAP_DATA_IND) {
        const uint8_t *ptr;
        arm_pd_sap_generic_ind_t *pd_data_ind = &(message->message.generic_data_ind);

        if (pd_data_ind->data_len < 3) {
            return -1;
        }
        ptr = pd_data_ind->data_ptr;

        uint32_t time_stamp = 0;
        if (rf_ptr->rf_csma_extension_supported) {
            time_stamp = mac_pd_sap_get_phy_rx_time(rf_ptr);
        }
        mac_fcf_sequence_t fcf_read;
        ptr = mac_header_parse_fcf_dsn(&fcf_read, ptr);
        //Check PanID presents at header
        fcf_read.DstPanPresents = mac_dst_panid_present(&fcf_read);
        fcf_read.SrcPanPresents = mac_src_panid_present(&fcf_read);
        int16_t length = pd_data_ind->data_len;
        if (!rf_ptr->macProminousMode) {

            //Unsupported Frame
            if (fcf_read.frametype > FC_CMD_FRAME || (fcf_read.frametype == FC_ACK_FRAME && fcf_read.frameVersion != MAC_FRAME_VERSION_2015)) {
                goto ERROR_HANDLER;
            }

            switch (fcf_read.frametype) {
                case FC_DATA_FRAME:
                    if (fcf_read.SrcAddrMode == MAC_ADDR_MODE_NONE) {
                        return -1;
                    } else if (fcf_read.DstAddrMode == MAC_ADDR_MODE_NONE && fcf_read.frameVersion != MAC_FRAME_VERSION_2015) {
                        return -1;
                    }
                    break;
                case FC_BEACON_FRAME:
                    if (fcf_read.SrcAddrMode == MAC_ADDR_MODE_NONE || fcf_read.DstAddrMode != MAC_ADDR_MODE_NONE) {
                        return -1;
                    }
                    break;
                case FC_ACK_FRAME:
                    //Validate here that we are waiting ack
                    if (fcf_read.ackRequested) {
                        return -1;
                    }

                    //Validate ACK
                    if (!mac_pd_sap_ack_validation(rf_ptr, &fcf_read, pd_data_ind->data_ptr)) {
                        return -1;
                    }
                    break;

                default:
                    break;
            }

            //Generate ACK when Extension is enabled and ACK is requested
            if (rf_ptr->mac_extension_enabled && fcf_read.ackRequested && fcf_read.frameVersion == MAC_FRAME_VERSION_2015) {
                //SEND ACK here
                if (rf_ptr->mac_ack_tx_active) {
                    return -1;
                }

                mcps_ack_data_payload_t ack_payload;
                mac_api_t *mac_api = get_sw_mac_api(rf_ptr);
                mac_api->enhanced_ack_data_req_cb(mac_api, &ack_payload, pd_data_ind->dbm, pd_data_ind->link_quality);
                //Calculate Delta time

                if (mcps_generic_ack_build(rf_ptr, &fcf_read, pd_data_ind->data_ptr, &ack_payload, time_stamp) != 0) {
                    return -1;
                }
            }
        }

        buffer = mcps_sap_pre_parsed_frame_buffer_get(pd_data_ind->data_ptr, pd_data_ind->data_len);

        if (!buffer) {
            sw_mac_stats_update(rf_ptr, STAT_MAC_RX_DROP, 0);
            return -3;
        }

        //Copy Pre Parsed values
        buffer->fcf_dsn = fcf_read;
        buffer->timestamp = time_stamp;

        buffer->ack_pendinfg_status = mac_data_interface_read_last_ack_pending_status(rf_ptr);


        // Upward direction functions assume no headroom and are trusting that removed bytes are still valid.
        // see mac.c:655

        /* Set default flags */
        buffer->dbm = pd_data_ind->dbm;
        buffer->LQI = pd_data_ind->link_quality;
        buffer->mac_class_ptr = rf_ptr;
        //Dnamic calculation for FCF + SEQ parse
        buffer->mac_header_length = ptr - pd_data_ind->data_ptr;

        if (!rf_ptr->macProminousMode) {

            if (buffer->fcf_dsn.frametype > FC_CMD_FRAME) {
                goto ERROR_HANDLER;
            }

            buffer->mac_header_length += mac_header_address_length(&buffer->fcf_dsn);

            length -= buffer->mac_header_length;

            if (length < 0) {
                goto ERROR_HANDLER;
            }

            buffer->mac_payload_length = (buffer->frameLength - buffer->mac_header_length);

            if (buffer->fcf_dsn.securityEnabled) {
                //Read KEYID Mode
                uint8_t key_id_mode, security_level, mic_len;
                uint8_t *security_ptr = &buffer->buf[buffer->mac_header_length];
                uint8_t auxBaseHeader = *security_ptr;
                key_id_mode = (auxBaseHeader >> 3) & 3;
                security_level = auxBaseHeader & 7;

                switch (key_id_mode) {
                    case MAC_KEY_ID_MODE_IMPLICIT:
                        if (security_level) {
                            buffer->security_aux_header_length = 5;
                        } else {
                            buffer->security_aux_header_length = 1;
                        }
                        break;
                    case MAC_KEY_ID_MODE_IDX:
                        buffer->security_aux_header_length = 6;
                        break;
                    case MAC_KEY_ID_MODE_SRC4_IDX:
                        buffer->security_aux_header_length = 10;
                        break;
                    default:
                        buffer->security_aux_header_length = 14;
                        break;
                }

                length -= buffer->security_aux_header_length;
                mic_len = mac_security_mic_length_get(security_level);

                length -= mic_len;

                //Verify that data length is not negative
                if (length < 0) {
                    goto ERROR_HANDLER;
                }

                buffer->mac_payload_length -= (buffer->security_aux_header_length + mic_len);
            }


        }
        //Do not accept commend frame with length 0
        if (fcf_read.frametype == FC_CMD_FRAME && length == 0) {
            goto ERROR_HANDLER;
        }

        //Parse IE Elements
        if (!mac_header_information_elements_parse(buffer)) {
            goto ERROR_HANDLER;
        }

        if (!rf_ptr->macProminousMode && buffer->fcf_dsn.frametype == FC_ACK_FRAME) {
            if (mac_data_interface_tx_done_by_ack_cb(rf_ptr, buffer)) {
                mcps_sap_pre_parsed_frame_buffer_free(buffer);
            }
            return 0;

        } else {

            if (mcps_sap_pd_ind(buffer) == 0) {
                return 0;
            }
        }
ERROR_HANDLER:
        mcps_sap_pre_parsed_frame_buffer_free(buffer);
        return -1;


    } else if (message->id == MAC15_4_PD_SAP_DATA_TX_CONFIRM) {
        arm_pd_sap_15_4_confirm_with_params_t *pd_data_cnf = &(message->message.mac15_4_pd_sap_confirm);
        return mac_data_interface_tx_done_cb(rf_ptr, pd_data_cnf->status, pd_data_cnf->cca_retry, pd_data_cnf->tx_retry);
    }

    return -1;
}

void mac_pd_sap_rf_low_level_function_set(void *mac_ptr, void *driver)
{
    arm_device_driver_list_s *driver_ptr = (arm_device_driver_list_s *)driver;
    driver_ptr->phy_sap_identifier = (protocol_interface_rf_mac_setup_s *)mac_ptr;
    driver_ptr->phy_sap_upper_cb = mac_pd_sap_data_cb;
}

