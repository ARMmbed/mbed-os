/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
#include "ns_trace.h"
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


#define TRACE_GROUP "mPDs"

/* Define TX Timeot Period */
// Hardcoded to 1200ms. Should be changed dynamic: (FHSS) channel retries needs longer timeout
#define NWKTX_TIMEOUT_PERIOD (1200*20)

static int8_t mac_data_interface_tx_done_cb(protocol_interface_rf_mac_setup_s *rf_ptr, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry);

static void mac_csma_param_init(protocol_interface_rf_mac_setup_s *rf_mac_setup)
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

static void mac_csma_backoff_start(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    uint8_t backoff = mac_csma_random_backoff_get(rf_mac_setup);
    uint16_t backoff_slots = mac_csma_backoff_period_convert_to50us(backoff, rf_mac_setup->backoff_period_in_10us);
    if (backoff_slots == 0) {
        backoff_slots = 1;
    }

    timer_mac_start(rf_mac_setup, MAC_TIMER_CCA, backoff_slots);
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

static void mac_tx_done_state_set(protocol_interface_rf_mac_setup_s *rf_ptr, mac_event_t event )
{
    rf_ptr->mac_tx_result = event;

    if(event == MAC_TX_DONE  || event == MAC_TX_DONE_PENDING) {

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

static int8_t mac_plme_cca_req(protocol_interface_rf_mac_setup_s *rf_ptr) {
    dev_driver_tx_buffer_s *tx_buf = &rf_ptr->dev_driver_tx_buffer;
    phy_device_driver_s *dev_driver = rf_ptr->dev_driver->phy_driver;


    if (dev_driver->arm_net_virtual_tx_cb) {
        if (dev_driver->tx(tx_buf->buf, tx_buf->len, 1, PHY_LAYER_PAYLOAD) == 0) {
            rf_ptr->macRfRadioTxActive = true;
            timer_mac_start(rf_ptr, MAC_TX_TIMEOUT, NWKTX_TIMEOUT_PERIOD);  /*Start Timeout timer for virtual packet loss*/
        } else {
            mac_data_interface_tx_to_cb(rf_ptr);
        }
        return 0;
    }

    if (dev_driver->tx(tx_buf->buf, tx_buf->len, 1, PHY_LAYER_PAYLOAD) == 0) {
        rf_ptr->macRfRadioTxActive = true;
        return 0;
    }
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
 * Run Mac data interface state Machine for mac timer.
 *
 */
void mac_pd_sap_state_machine(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    if (rf_mac_setup->macUpState && rf_mac_setup->macTxProcessActive) {

        if (rf_mac_setup->mac_tx_result == MAC_TIMER_CCA) {
            if (rf_mac_setup->fhss_api) {
                uint8_t *synch_info = NULL;
                mac_pre_build_frame_t *active_buf = rf_mac_setup->active_pd_data_request;
                if (!active_buf) {
                    return;
                }
                if (active_buf->fcf_dsn.frametype == FC_BEACON_FRAME) {
                    // FHSS synchronization info is written in the end of transmitted (Beacon) buffer
                    dev_driver_tx_buffer_s *tx_buf = &rf_mac_setup->dev_driver_tx_buffer;
                    synch_info = tx_buf->buf + rf_mac_setup->dev_driver->phy_driver->phy_header_length + tx_buf->len - FHSS_SYNCH_INFO_LENGTH;
                }
                // Change to destination channel and write synchronization info to Beacon frames here
                int tx_handle_retval = rf_mac_setup->fhss_api->tx_handle(rf_mac_setup->fhss_api, !mac_is_ack_request_set(active_buf),
                        active_buf->DstAddr, mac_convert_frame_type_to_fhss(active_buf->fcf_dsn.frametype),
                        synch_info, active_buf->mac_payload_length, rf_mac_setup->dev_driver->phy_driver->phy_header_length,
                        rf_mac_setup->dev_driver->phy_driver->phy_tail_length);
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
                }
            }
            if (mac_plme_cca_req(rf_mac_setup) != 0) {
                mac_csma_backoff_start(rf_mac_setup);
            }
        } else if (rf_mac_setup->mac_tx_result == MAC_TX_TIMEOUT) {
            mac_data_interface_tx_to_cb(rf_mac_setup);
        } else if (rf_mac_setup->mac_tx_result == MAC_TIMER_ACK) {
            mac_data_interface_tx_done_cb(rf_mac_setup,PHY_LINK_TX_FAIL, 0, 0 );
        }
    }
}

static void  mac_sap_cca_fail_cb(protocol_interface_rf_mac_setup_s *rf_ptr) {
    rf_ptr->macRfRadioTxActive = false;
    if (rf_ptr->mac_cca_retry > rf_ptr->macMaxCSMABackoffs) {
        //Send MAC_CCA_FAIL
        mac_tx_done_state_set(rf_ptr, MAC_CCA_FAIL);
    } else {
        timer_mac_stop(rf_ptr);
        mac_csma_BE_update(rf_ptr);
        mac_csma_backoff_start(rf_ptr);
    }
}

static uint16_t mac_get_retry_period(protocol_interface_rf_mac_setup_s *rf_ptr)
{
    if (rf_ptr->fhss_api) {
        return rf_ptr->fhss_api->get_retry_period(rf_ptr->fhss_api, rf_ptr->active_pd_data_request->DstAddr, rf_ptr->dev_driver->phy_driver->phy_MTU);
    }
    uint8_t backoff_length = mac_csma_random_backoff_get(rf_ptr);
    uint16_t backoff_slots = mac_csma_backoff_period_convert_to50us(backoff_length, rf_ptr->backoff_period_in_10us);
    if (backoff_slots == 0) {
        backoff_slots = 1;
    }
    return backoff_slots;
}

static void mac_sap_no_ack_cb(protocol_interface_rf_mac_setup_s *rf_ptr) {
    rf_ptr->macRfRadioTxActive = false;
    if (rf_ptr->mac_tx_retry < rf_ptr->mac_mlme_retry_max) {
        rf_ptr->mac_cca_retry = 0;
        rf_ptr->mac_tx_retry++; //Update retry counter
        mac_csma_param_init(rf_ptr);
        rf_ptr->mac_tx_status.retry++;
        /*Send retry using random interval*/
        timer_mac_start(rf_ptr, MAC_TIMER_CCA, mac_get_retry_period(rf_ptr));
    } else {
        //Send TX Fail event
        // rf_mac_setup->ip_tx_active->bad_channel = rf_mac_setup->mac_channel;
        mac_tx_done_state_set(rf_ptr, MAC_TX_FAIL);
    }
}


static int8_t mac_data_interface_tx_done_cb(protocol_interface_rf_mac_setup_s *rf_ptr, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry)
{

    if (!rf_ptr->macRfRadioTxActive) {
        return -1;
    }
    bool waiting_ack = false;
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
    }
    if (rf_ptr->fhss_api) {
        bool tx_is_done = false;
        if (rf_ptr->mac_tx_result == MAC_TX_DONE) {
            tx_is_done = true;
        }
        rf_ptr->fhss_api->data_tx_done(rf_ptr->fhss_api, waiting_ack, tx_is_done, rf_ptr->active_pd_data_request->msduHandle);
    }
    return 0;
}

int8_t mac_pd_sap_data_cb(void *identifier, arm_phy_sap_msg_t *message)
{
    protocol_interface_rf_mac_setup_s *rf_ptr = (protocol_interface_rf_mac_setup_s*)identifier;

    if (!rf_ptr || !message ) {
        return -1;
    }

    if (!rf_ptr->macUpState) {
        return -2;
    }

    if (message->id == MAC15_4_PD_SAP_DATA_IND) {
        const uint8_t *ptr;

        arm_pd_sap_generic_ind_t *pd_data_ind = &(message->message.generic_data_ind);

        if (pd_data_ind->data_len < 3 ) {
            return -1;
        }
        ptr = pd_data_ind->data_ptr;

        mac_pre_parsed_frame_t *buffer = mcps_sap_pre_parsed_frame_buffer_get(pd_data_ind->data_ptr, pd_data_ind->data_len);

        if (!buffer) {
            tr_error("pd_ind buffer get fail %u", pd_data_ind->data_len);
            sw_mac_stats_update(rf_ptr, STAT_MAC_RX_DROP, 0);
            return -3;
        }
        if (rf_ptr->fhss_api) {
            buffer->timestamp = rf_ptr->fhss_api->read_timestamp(rf_ptr->fhss_api);
        }
        buffer->ack_pendinfg_status = mac_data_interface_read_last_ack_pending_status(rf_ptr);
        mac_header_parse_fcf_dsn(&buffer->fcf_dsn, ptr);

        int16_t length = pd_data_ind->data_len;
        ptr += 3;
        // Upward direction functions assume no headroom and are trusting that removed bytes are still valid.
        // see mac.c:655

        /* Set default flags */
        buffer->dbm = pd_data_ind->dbm;
        buffer->LQI = pd_data_ind->link_quality;
        buffer->mac_class_ptr = rf_ptr;
        buffer->mac_header_length = 3;

        if (!rf_ptr->macProminousMode) {

            if (buffer->fcf_dsn.frametype > FC_CMD_FRAME || buffer->fcf_dsn.frametype == FC_ACK_FRAME) {
                goto ERROR_HANDLER;
            }
            //Verify Length after address field
            switch (buffer->fcf_dsn.DstAddrMode) {
                case MAC_ADDR_MODE_64_BIT:
                    buffer->mac_header_length += 10;
                    break;
                case MAC_ADDR_MODE_16_BIT:
                    buffer->mac_header_length += 4;
                    break;
                case MAC_ADDR_MODE_NONE:
                    break;
            }

            switch (buffer->fcf_dsn.SrcAddrMode) {
                case MAC_ADDR_MODE_64_BIT:
                    buffer->mac_header_length += 8;
                    if (!buffer->fcf_dsn.intraPan) {
                        buffer->mac_header_length += 2;
                    }
                    break;
                case MAC_ADDR_MODE_16_BIT:
                    buffer->mac_header_length += 2;
                    if (!buffer->fcf_dsn.intraPan) {
                        buffer->mac_header_length += 2;
                    }
                    break;
                case MAC_ADDR_MODE_NONE:
                    break;
            }

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

                switch (security_level) {
                    case 1:
                    case 5:
                        mic_len = 4;
                        break;
                    case 2:
                    case 6:
                        mic_len = 8;
                        break;
                    case 3:
                    case 7:
                        mic_len = 16;
                        break;
                    default:
                        mic_len = 0;
                        break;
                }

                length -= mic_len;

                //Verify that data length is not negative
                if (length < 0) {
                    goto ERROR_HANDLER;
                }

                buffer->mac_payload_length -= (buffer->security_aux_header_length + mic_len);
            }

            switch (buffer->fcf_dsn.frametype) {
                case FC_DATA_FRAME:
                    if (buffer->fcf_dsn.SrcAddrMode == MAC_ADDR_MODE_NONE || buffer->fcf_dsn.DstAddrMode == MAC_ADDR_MODE_NONE) {
                        goto ERROR_HANDLER;
                    }
                    break;
                case FC_BEACON_FRAME:
                    if (buffer->fcf_dsn.SrcAddrMode == MAC_ADDR_MODE_NONE || buffer->fcf_dsn.DstAddrMode != MAC_ADDR_MODE_NONE) {
                        goto ERROR_HANDLER;
                    }
                    break;
                default:
                    if (length == 0) {
                        goto ERROR_HANDLER;
                    }
                    break;
            }

        }

        if (mcps_sap_pd_ind(buffer) == 0) {
            return 0;
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
    arm_device_driver_list_s *driver_ptr = (arm_device_driver_list_s*)driver;
    driver_ptr->phy_sap_identifier = (protocol_interface_rf_mac_setup_s *)mac_ptr;
    driver_ptr->phy_sap_upper_cb = mac_pd_sap_data_cb;
}

