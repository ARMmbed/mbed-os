/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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

#include "nsconfig.h"
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_callback_timer.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "randLIB.h"
#include "ccmLIB.h"
#include "mlme.h"
#include "mac_api.h"
#include "fhss_api.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/IEEE802_15_4/mac_header_helper_functions.h"
#include "MAC/IEEE802_15_4/mac_timer.h"
#include "MAC/IEEE802_15_4/mac_security_mib.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "MAC/IEEE802_15_4/mac_filter.h"
#include "MAC/IEEE802_15_4/mac_pd_sap.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/IEEE802_15_4/mac_indirect_data.h"
#include "MAC/rf_driver_storage.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "platform/arm_hal_interrupt.h"
#include "common_functions.h"
#include "mac_api.h"
#include "sw_mac.h"

#include "test_mac_mcps_sap.h"
#include "sw_mac_stub.h"
#include "ccm_security_stub.h"
#include "mac_security_mib_stub.h"
#include "mac_header_helper_functions_stub.h"
#include "nsdynmemLIB_stub.h"
#include "mac_indirect_data_stub.h"
#include "event_stub.h"
#include "mac_filter_stub.h"
#include "mac_mlme_stub.h"
#include "fhss_config_stub.h"
#include "fhss_mac_interface_stub.h"
#include <string.h>

static uint8_t tes_mac_mcps_mac64[8];


static mac_pre_build_frame_t * tes_mac_mcps_temporary_buffer_get(uint16_t length) {
    mac_pre_build_frame_t *buf = malloc(sizeof(mac_pre_build_frame_t));

    memset(buf,0, sizeof(mac_pre_build_frame_t));

    if (length) {
        buf->mac_payload = malloc(length);
        buf->mac_allocated_payload_ptr = true;
        buf->mac_payload_length = length;
    }

    return buf;
}

static void test_mac_buffer_settings(mac_pre_build_frame_t *buf, mac_frame_e frame_type,  uint8_t srcType, uint8_t dstType,bool ack_request, bool intra, bool security, mac_aux_security_header_t *security_params)
{
    buf->fcf_dsn.frametype = frame_type;
    buf->fcf_dsn.SrcAddrMode = srcType;
    buf->fcf_dsn.DstAddrMode = dstType;
    buf->fcf_dsn.intraPan = intra;
    buf->fcf_dsn.securityEnabled = security;
    buf->fcf_dsn.ackRequested = ack_request;
    buf->mac_header_length_with_security = 3;
    switch (buf->fcf_dsn.DstAddrMode) {
        case MAC_ADDR_MODE_64_BIT:
            buf->mac_header_length_with_security += 10;
            break;
        case MAC_ADDR_MODE_16_BIT:
            buf->mac_header_length_with_security += 4;
            break;
    }

    switch (buf->fcf_dsn.SrcAddrMode) {
        case MAC_ADDR_MODE_64_BIT:
            buf->mac_header_length_with_security += 8;
            break;
        case MAC_ADDR_MODE_16_BIT:
            buf->mac_header_length_with_security += 2;
            break;
    }

    if (!intra) {
        buf->mac_header_length_with_security += 2;
    }


    if (security) {
        buf->aux_header = *security_params;
        if (buf->aux_header.securityLevel) {




            switch (buf->aux_header.KeyIdMode) {
                case 1:
                case 5:
                    buf->security_mic_len = 4;
                    break;

                case 2:
                case 6:
                    buf->security_mic_len = 8;
                    break;
                case 3:
                case 7:
                    buf->security_mic_len = 16;
                    break;
                default:

                    buf->security_mic_len = 0;
                    break;
            }
            buf->mac_header_length_with_security += 5; //Level and frame counter

            switch (buf->aux_header.securityLevel) {
                case MAC_KEY_ID_MODE_IMPLICIT:
                    break;
                case MAC_KEY_ID_MODE_IDX:
                    buf->mac_header_length_with_security += 1;
                    break;

                case MAC_KEY_ID_MODE_SRC4_IDX:
                    buf->mac_header_length_with_security += 5;
                    break;
                case MAC_KEY_ID_MODE_SRC8_IDX:
                    buf->mac_header_length_with_security += 9;
                    break;
            }


        } else {
            buf->aux_header.KeyIdMode = 0;
        }
    }
}

static void test_set_security_params(mac_aux_security_header_t *params, uint8_t security_level, uint8_t key_id_mode, uint8_t key_index)
{
    params->KeyIdMode = key_id_mode;
    params->KeyIndex = 1;
    params->frameCounter = 0;
    params->securityLevel = security_level;
}

static int8_t test_phy_rf_virtual_rx(const uint8_t *data_ptr, uint16_t data_len,int8_t driver_id)
{

}

static phy_link_tx_status_e virtual_tx_done_status;
static bool virtual_cb_ok = false;

static int8_t test_phy_rf_tx_done(int8_t driver_id, uint8_t tx_handle, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry)
{
    if (status == virtual_tx_done_status) {
        virtual_cb_ok = true;
    } else {
        virtual_cb_ok = false;
    }

}

static uint16_t expected_virtual_rx_data_length = 0;
static bool virtual_rx_data_length_ok = false;

static int8_t test_phy_rf_rx(const uint8_t *data_ptr, uint16_t data_len, uint8_t link_quality, int8_t dbm, int8_t driver_id)
{
    //Validate proper data length here
    if (expected_virtual_rx_data_length == data_len) {
        virtual_rx_data_length_ok = true;
    } else {
        virtual_rx_data_length_ok = false;
    }
}


static  arm_device_driver_list_s * rf_client_driver_allocate()
{
    arm_device_driver_list_s *driver = malloc(sizeof(arm_device_driver_list_s));
    phy_device_driver_s *dev_driver = malloc(sizeof(phy_device_driver_s));


    memset(driver, 0, sizeof(arm_device_driver_list_s));
    memset(dev_driver, 0, sizeof(phy_device_driver_s));

    driver->phy_driver = dev_driver;


    dev_driver->arm_net_virtual_rx_cb = &test_phy_rf_virtual_rx;
    dev_driver->phy_rx_cb = &test_phy_rf_rx;
    dev_driver->phy_tx_done_cb = &test_phy_rf_tx_done;


    return driver;
}


static protocol_interface_rf_mac_setup_s * test_mac_rf_mac_class_allocate(void) {
    protocol_interface_rf_mac_setup_s *rf_mac_setup = malloc(sizeof(protocol_interface_rf_mac_setup_s));

    memset(rf_mac_setup,0, sizeof(protocol_interface_rf_mac_setup_s));
    rf_mac_setup->dev_driver_tx_buffer.buf = malloc(128);
    rf_mac_setup->dev_driver_tx_buffer.len = 127;


    arm_device_driver_list_s *driver = malloc(sizeof(arm_device_driver_list_s));
    phy_device_driver_s *dev_driver = malloc(sizeof(phy_device_driver_s));


    memset(driver, 0, sizeof(arm_device_driver_list_s));
    memset(dev_driver, 0, sizeof(phy_device_driver_s));

    dev_driver->PHY_MAC = tes_mac_mcps_mac64;
    dev_driver->phy_MTU = 127;
    dev_driver->phy_header_length = 0;
    driver->phy_driver = dev_driver;
    rf_mac_setup->dev_driver = driver;

    return rf_mac_setup;
}

static uint8_t expect_data_confirmation_status = 0;
static bool data_confirm_valid = false;

void test_mcps_data_confirm( const mac_api_t* api, const mcps_data_conf_t *data )
{
    if (data->status == expect_data_confirmation_status) {
        data_confirm_valid = true;
    } else {
        data_confirm_valid = false;
    }
}
static uint16_t expected_data_length = 0;
static bool data_ind_valid = false;

static void test_mcps_data_indication( const mac_api_t* api, const mcps_data_ind_t *data )
{
    if (data->msduLength == expected_data_length) {
        data_ind_valid = true;
    } else {
        data_ind_valid = false;
    }
}

static mlme_primitive expected_mlme_id_cnf = 0;
static bool mlme_confirm_valid = false;

void test_mlme_confirm( const mac_api_t* api, mlme_primitive id, const void *data )
{
    if (id == expected_mlme_id_cnf) {
        mlme_confirm_valid = true;
    } else {
        mlme_confirm_valid = false;
    }
}

static uint8_t expected_purge_status = 0;
static uint8_t expected_purge_handle = 0;
static uint8_t expected_purge_result = false;

void test_purge_confirm( const mac_api_t* api, const mcps_purge_conf_t *data )
{
    if (expected_purge_handle != data->msduHandle) {
        expected_purge_result = false;
    } else if (expected_purge_status != data->status){
        expected_purge_result = false;
    } else {
        expected_purge_result= true;
    }
}

static mlme_primitive expected_mlme_ind_id_cnf = 0;
static uint8_t mlme_ind_status;
static bool mlme_ind_valid = false;


void test_mlme_indication( const mac_api_t* api, mlme_primitive id, const void *data )
{
    mlme_ind_valid = false;
    if (id == expected_mlme_ind_id_cnf) {

        if (id == MLME_COMM_STATUS) {
            mlme_comm_status_t *comm_status = (mlme_comm_status_t*) data;
            if (comm_status->status == mlme_ind_status) {
                mlme_ind_valid = true;
            }
        } else {
            mlme_ind_valid = true;
        }
    }

}

static uint8_t expected_event_type = 0;
static bool valid_pd_sap_confirm_forward = false;

static void handler_func_ptr(arm_event_s *event)
{
    if (event->event_type == expected_event_type) {
        valid_pd_sap_confirm_forward = true;
    } else {
        valid_pd_sap_confirm_forward = false;
    }
}


void test_mac_rf_mac_class_free(protocol_interface_rf_mac_setup_s *rf_ptr) {

    free(rf_ptr->dev_driver_tx_buffer.buf);
    free(rf_ptr->dev_driver->phy_driver);
    free(rf_ptr->dev_driver);
    if (rf_ptr->tun_extension_rf_driver) {
        free(rf_ptr->tun_extension_rf_driver->phy_driver);
        free(rf_ptr->tun_extension_rf_driver);
    }
    free(rf_ptr);
}


bool test_mac_generic_event_trig()
{
    //Init event handler
    event_stub.int8_value = 0;
    int8_t mac_tasklet_id = mac_mcps_sap_tasklet_init();
    mcps_data_req_t data_req;

    ccm_globals_t ccm_ptr;
    mac_api_t mac_api;
    mlme_key_descriptor_t key_description;
    mlme_device_descriptor_t device_description;
    mac_aux_security_header_t secuirity_params;
    mlme_key_device_descriptor_t key_device_description;
    memset(&data_req, 0, sizeof(mcps_data_req_t));

    uint8_t tx_buf[127];
    uint8_t tx_temp[200];
    protocol_interface_rf_mac_setup_s *rf_mac_setup = test_mac_rf_mac_class_allocate();
    memset(tx_buf, 1, 127);

    memset(&key_description, 0, sizeof(mlme_key_descriptor_t));
    memset(&device_description, 0, sizeof(mlme_device_descriptor_t));

    mac_api.data_conf_cb = &test_mcps_data_confirm;
    mac_api.mlme_conf_cb = &test_mlme_confirm;
    mac_api.data_ind_cb = &test_mcps_data_indication;
    mac_api.mlme_ind_cb = &test_mlme_indication;

    rf_mac_setup->macUpState = true;
    sw_mac_stub.mac_api_ptr = &mac_api;

    mac_generic_event_trig(MAC_MLME_EVENT_HANDLER,rf_mac_setup,false);

    mac_generic_event_trig(MAC_MLME_SCAN_CONFIRM_HANDLER,rf_mac_setup,true);

    test_mac_rf_mac_class_free(rf_mac_setup);
    return true;

}

bool test_mcps_sap_trig_tx()
{
    //Init event handler
    event_stub.int8_value = 0;
    int8_t mac_tasklet_id = mac_mcps_sap_tasklet_init();
    protocol_interface_rf_mac_setup_s *rf_mac_setup = test_mac_rf_mac_class_allocate();

    rf_mac_setup->fhss_api = ns_fhss_create(0, 0);
    mac_pre_build_frame_t *buf = tes_mac_mcps_temporary_buffer_get(0);
    rf_mac_setup->pd_data_request_bc_queue_to_go = buf;

    // Test without MAC setup
    mcps_sap_trig_tx(NULL);
    if (rf_mac_setup->active_pd_data_request == buf) {
        return false;
    }

    // Test when TX trig active
    rf_mac_setup->active_mac_events |= (1 << MAC_SAP_TRIG_TX);
    mcps_sap_trig_tx(rf_mac_setup);
    if (rf_mac_setup->active_pd_data_request == buf) {
        return false;
    }
    rf_mac_setup->active_mac_events &= ~(1 << MAC_SAP_TRIG_TX);

    // FHSS is on broadcast channel and tx conditions are fine
    fhss_mac_interface_stub.channel_bool_value = true;
    fhss_mac_interface_stub.cond_bool_value = true;

    mcps_sap_trig_tx(rf_mac_setup);
    // Active data request should be buf
    if (rf_mac_setup->active_pd_data_request != buf) {
        return false;
    }
    mac_mcps_buffer_queue_free(rf_mac_setup);
    test_mac_rf_mac_class_free(rf_mac_setup);

    return true;
}

bool test_mac_virtual_sap_data_cb() {
    //Init event handler
    event_stub.int8_value = 0;
    int8_t mac_tasklet_id = mac_mcps_sap_tasklet_init();
    mcps_data_req_t data_req;

    ccm_globals_t ccm_ptr;
    mac_api_t mac_api;
    mlme_key_descriptor_t key_description;
    mlme_device_descriptor_t device_description;
    mac_aux_security_header_t secuirity_params;
    mlme_key_device_descriptor_t key_device_description;
    memset(&data_req, 0, sizeof(mcps_data_req_t));

    uint8_t tx_buf[127];
    uint8_t tx_temp[200];
    uint8_t virtual_tx_temp[200];
    protocol_interface_rf_mac_setup_s *rf_mac_setup = test_mac_rf_mac_class_allocate();
    memset(tx_buf, 1, 127);

    memset(&key_description, 0, sizeof(mlme_key_descriptor_t));
    memset(&device_description, 0, sizeof(mlme_device_descriptor_t));

    mac_api.data_conf_cb = &test_mcps_data_confirm;
    mac_api.mlme_conf_cb = &test_mlme_confirm;
    mac_api.data_ind_cb = &test_mcps_data_indication;
    mac_api.mlme_ind_cb = &test_mlme_indication;

    rf_mac_setup->macUpState = true;
    sw_mac_stub.mac_api_ptr = &mac_api;

    rf_mac_setup->tun_extension_rf_driver = rf_client_driver_allocate();

    arm_phy_sap_msg_t phy_message;

    memset(&phy_message, 0 , sizeof(arm_phy_sap_msg_t));

    mac_virtual_sap_data_cb(NULL, NULL);

    phy_message.id = MAC802_PD_SAP_DATA_TX_CONFIRM;
    if (-1 != mac_virtual_sap_data_cb(rf_mac_setup, &phy_message) ) {
        return false;
    }
    rf_mac_setup->macUpState = false;

    //TEST MLME Extension

    phy_message.id = MACTUN_MLME_NAP_EXTENSION;
    uint8_t beacon_payload[18];
    beacon_payload[0] = macBeaconPayload;
    beacon_payload[1] = 0;
    phy_message.message.mlme_request.mlme_ptr = beacon_payload;
    phy_message.message.mlme_request.primitive = MLME_SET;
    phy_message.message.mlme_request.ptr_length = 18;

    mac_mlme_stub.int8_value = -1;
    if (-1 != mac_virtual_sap_data_cb(rf_mac_setup, &phy_message) ) {
        return false;
    }

    mac_mlme_stub.int8_value = 0;
    if (0 != mac_virtual_sap_data_cb(rf_mac_setup, &phy_message) ) {
        return false;
    }

    phy_message.message.mlme_request.primitive = MLME_SCAN;
    phy_message.message.mlme_request.ptr_length = 40;
    if (-1 != mac_virtual_sap_data_cb(rf_mac_setup, &phy_message) ) {
        return false;
    }
    phy_message.message.mlme_request.ptr_length = 47;
    if (0 != mac_virtual_sap_data_cb(rf_mac_setup, &phy_message) ) {
        return false;
    }

    phy_message.message.mlme_request.primitive = MLME_START;
    phy_message.message.mlme_request.ptr_length = 30;
    if (-1 != mac_virtual_sap_data_cb(rf_mac_setup, &phy_message) ) {
        return false;
    }
    phy_message.message.mlme_request.ptr_length = 34;
    if (0 != mac_virtual_sap_data_cb(rf_mac_setup, &phy_message) ) {
        return false;
    }

    // Test unsupported primitive
    phy_message.message.mlme_request.primitive = 65535;
    if (-1 != mac_virtual_sap_data_cb(rf_mac_setup, &phy_message) ) {
        return false;
    }

    //Test Mac down state
    phy_message.id = MACTUN_PD_SAP_NAP_IND;
    if (-1 != mac_virtual_sap_data_cb(rf_mac_setup, &phy_message) ) {
        return false;
    }

    //Test too long
    rf_mac_setup->macUpState = true;
    phy_message.message.generic_data_ind.data_ptr = virtual_tx_temp;
    phy_message.message.generic_data_ind.data_len = 128;
    if (mac_virtual_sap_data_cb(rf_mac_setup, &phy_message) == 0) {
        return false;
    }

    //Test Buffer fail allocation
    nsdynmemlib_stub.returnCounter = 0;

    phy_message.message.generic_data_ind.data_len = 40;
    if (-1 != mac_virtual_sap_data_cb(rf_mac_setup, &phy_message) ) {
        return false;
    }

    //Test no ack request
    nsdynmemlib_stub.returnCounter = 3;
    mac_header_helper_functions_stub.fcf.ackRequested = false;


    virtual_tx_done_status  = PHY_LINK_TX_SUCCESS;
    virtual_cb_ok = false;

    //Waiting tx success from callback
    if (0 != mac_virtual_sap_data_cb(rf_mac_setup, &phy_message) ) {
        return false;
    }

    if (!rf_mac_setup->active_pd_data_request || !virtual_cb_ok) {
        return false;
    }

    //Free current buffer
    mac_mcps_buffer_queue_free(rf_mac_setup);
    //Test with ack
    nsdynmemlib_stub.returnCounter = 3;
    mac_header_helper_functions_stub.fcf.ackRequested = true;
    if (0 != mac_virtual_sap_data_cb(rf_mac_setup, &phy_message) ) {
        return false;
    }

    if (!rf_mac_setup->active_pd_data_request) {
        return false;
    }

    mac_mcps_buffer_queue_free(rf_mac_setup);

    //Test copying MAC address from payload
    uint8_t test_tx_buffer[21] = {0x61, 0xcc, 0x8, 0x55, 0x12, 0x3, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x2, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2};
    uint8_t test_dst_addr_buffer[8] = {0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0x3};
    phy_message.message.generic_data_ind.data_ptr = test_tx_buffer;
    phy_message.message.generic_data_ind.data_len = sizeof(test_tx_buffer);
    nsdynmemlib_stub.returnCounter = 3;
    mac_header_helper_functions_stub.fcf.ackRequested = true;
    mac_header_helper_functions_stub.fcf.DstAddrMode = MAC_ADDR_MODE_64_BIT;
    memcpy(mac_header_helper_functions_stub.dst_address, test_dst_addr_buffer, 8);
    if (0 != mac_virtual_sap_data_cb(rf_mac_setup, &phy_message) ) {
        return false;
    }
    if (!rf_mac_setup->active_pd_data_request) {
        return false;
    }
    // Destination address check
    if (memcmp(rf_mac_setup->active_pd_data_request->DstAddr, test_dst_addr_buffer, 8)) {
        return false;
    }

    mac_mcps_buffer_queue_free(rf_mac_setup);

    test_mac_rf_mac_class_free(rf_mac_setup);
    return true;
}

bool test_mac_mcps_data_confirmation()
{
    //Init event handler
    event_stub.int8_value = 0;
    int8_t mac_tasklet_id = mac_mcps_sap_tasklet_init();
    mcps_data_req_t data_req;

    ccm_globals_t ccm_ptr;
    mac_api_t mac_api;
    mlme_key_descriptor_t key_description;
    mlme_device_descriptor_t device_description;
    mac_aux_security_header_t secuirity_params;
    mlme_key_device_descriptor_t key_device_description;
    memset(&data_req, 0, sizeof(mcps_data_req_t));

    uint8_t tx_buf[127];
    uint8_t tx_temp[200];
    protocol_interface_rf_mac_setup_s *rf_mac_setup = test_mac_rf_mac_class_allocate();
    memset(tx_buf, 1, 127);

    memset(&key_description, 0, sizeof(mlme_key_descriptor_t));
    memset(&device_description, 0, sizeof(mlme_device_descriptor_t));

    mac_api.data_conf_cb = &test_mcps_data_confirm;
    mac_api.mlme_conf_cb = &test_mlme_confirm;
    mac_api.data_ind_cb = &test_mcps_data_indication;
    mac_api.mlme_ind_cb = &test_mlme_indication;

    rf_mac_setup->macUpState = true;
    sw_mac_stub.mac_api_ptr = &mac_api;

    data_req.msdu = tx_buf;
    data_req.msduLength = 10;

    rf_mac_setup->macUpState = true;

    //Empty call
    mcps_sap_pd_confirm(rf_mac_setup);
    //TODO add better validation at callbacks
    //CCA Fail
    rf_mac_setup->mac_tx_result = MAC_CCA_FAIL;
    nsdynmemlib_stub.returnCounter = 3;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    mcps_sap_pd_confirm(rf_mac_setup);

    if (rf_mac_setup->active_pd_data_request) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }


    nsdynmemlib_stub.returnCounter = 3;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    rf_mac_setup->mac_tx_result = MAC_TX_FAIL;
    mcps_sap_pd_confirm(rf_mac_setup);

    if (rf_mac_setup->active_pd_data_request) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    nsdynmemlib_stub.returnCounter = 3;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    rf_mac_setup->mac_tx_result = MAC_TX_DONE;
    mcps_sap_pd_confirm(rf_mac_setup);

    if (rf_mac_setup->active_pd_data_request) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    nsdynmemlib_stub.returnCounter = 3;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    rf_mac_setup->mac_tx_result = MAC_TX_DONE_PENDING;
    mcps_sap_pd_confirm(rf_mac_setup);

    if (rf_mac_setup->active_pd_data_request) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    nsdynmemlib_stub.returnCounter = 3;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    rf_mac_setup->mac_tx_result = MAC_TX_TIMEOUT;
    mcps_sap_pd_confirm(rf_mac_setup);

    if (rf_mac_setup->active_pd_data_request) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    //Change data packet to MAC command: DATA_REQ and test all CONFIRM events

    nsdynmemlib_stub.returnCounter = 3;
    data_req.DstAddrMode = MAC_ADDR_MODE_64_BIT;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    rf_mac_setup->mac_tx_result = MAC_TX_DONE;
    //rf_mac_setup->active_pd_data_request->status = MLME_SUCCESS;
    rf_mac_setup->active_pd_data_request->fcf_dsn.frametype = FC_CMD_FRAME;
    rf_mac_setup->active_pd_data_request->mac_command_id = MAC_DATA_REQ;
    rf_mac_setup->active_pd_data_request->upper_layer_request = false;
    mcps_sap_pd_confirm(rf_mac_setup);

    if (rf_mac_setup->active_pd_data_request) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    //Test data wait state + driver request
    nsdynmemlib_stub.returnCounter = 3;
    rf_mac_setup->dev_driver->phy_driver->phy_header_length = 1;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    rf_mac_setup->mac_tx_result = MAC_TX_DONE_PENDING;
    //rf_mac_setup->active_pd_data_request->status = MLME_SUCCESS;
    rf_mac_setup->active_pd_data_request->fcf_dsn.frametype = FC_CMD_FRAME;
    rf_mac_setup->active_pd_data_request->mac_command_id = MAC_DATA_REQ;
    rf_mac_setup->active_pd_data_request->upper_layer_request = false;

    mcps_sap_pd_confirm(rf_mac_setup);

    if (!rf_mac_setup->active_pd_data_request || rf_mac_setup->mac_mlme_event != ARM_NWK_MAC_MLME_INDIRECT_DATA_POLL) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    //Test Virtual mac extension confirmation + driver request

    rf_mac_setup->tun_extension_rf_driver = rf_client_driver_allocate();

    rf_mac_setup->active_pd_data_request->mac_command_id = 0xff;
    rf_mac_setup->active_pd_data_request->fcf_dsn.frametype = FC_DATA_FRAME;
    rf_mac_setup->active_pd_data_request->fcf_dsn.ackRequested = true;
    rf_mac_setup->mac_tx_result = MAC_TX_FAIL;
    mcps_sap_pd_confirm(rf_mac_setup);

    if (rf_mac_setup->active_pd_data_request) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    // Test FHSS channel retry
    rf_mac_setup->fhss_api = ns_fhss_create(0, 0);
    nsdynmemlib_stub.returnCounter = 3;
    data_req.TxAckReq = true;
    fhss_mac_interface_stub.cond_bool_value = true;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    rf_mac_setup->mac_tx_result = MAC_TX_FAIL;
    mcps_sap_pd_confirm(rf_mac_setup);
    if (!rf_mac_setup->active_pd_data_request) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }
    rf_mac_setup->mac_tx_result = MAC_TX_DONE;
    mcps_sap_pd_confirm(rf_mac_setup);
    if (rf_mac_setup->active_pd_data_request) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }




    test_mac_rf_mac_class_free(rf_mac_setup);
    return true;

}

bool test_mac_mcps_data_indication()
{
    //Init event handler
    event_stub.int8_value = 0;
    int8_t mac_tasklet_id = mac_mcps_sap_tasklet_init();


    ccm_globals_t ccm_ptr;
    mac_api_t mac_api;
    mlme_key_descriptor_t key_description;
    mlme_device_descriptor_t device_description;
    mac_aux_security_header_t secuirity_params;
    mlme_key_device_descriptor_t key_device_description;
    mcps_data_req_t data_req;
    memset(&data_req, 0, sizeof(mcps_data_req_t));

    uint8_t tx_buf[127];
    uint8_t tx_temp[200];
    protocol_interface_rf_mac_setup_s *rf_mac_setup = test_mac_rf_mac_class_allocate();
    memset(tx_buf, 1, 127);

    memset(&key_description, 0, sizeof(mlme_key_descriptor_t));
    memset(&device_description, 0, sizeof(mlme_device_descriptor_t));

    mac_api.data_conf_cb = &test_mcps_data_confirm;
    mac_api.mlme_conf_cb = &test_mlme_confirm;
    mac_api.data_ind_cb = &test_mcps_data_indication;
    mac_api.mlme_ind_cb = &test_mlme_indication;

    rf_mac_setup->macUpState = true;
    rf_mac_setup->pan_id = 0x0101;
    mac_header_helper_functions_stub.pan_src = 0x0101;
    sw_mac_stub.mac_api_ptr = &mac_api;

    nsdynmemlib_stub.returnCounter = 1;
    mac_pre_parsed_frame_t *buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 100);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.securityEnabled = false;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 100-9;
    buffer->security_aux_header_length = 0;
    buffer->mac_class_ptr = NULL;
    //First Buffer push
    mcps_sap_pd_ind(buffer);

    //Drop by mac filter
    nsdynmemlib_stub.returnCounter = 1;
    mac_filter_stub_def.value_int8 = 1;
    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 100);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.securityEnabled = false;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 100-9;
    buffer->security_aux_header_length = 0;
    buffer->mac_class_ptr = rf_mac_setup;
    mcps_sap_pd_ind(buffer);

    //Drop by unknow SW MAC class
    mac_filter_stub_def.value_int8 = 0;
    sw_mac_stub.mac_api_ptr = NULL;

    nsdynmemlib_stub.returnCounter = 1;
    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 100);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.securityEnabled = false;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 100-9;
    buffer->security_aux_header_length = 0;
    buffer->mac_class_ptr = rf_mac_setup;
    mcps_sap_pd_ind(buffer);

    nsdynmemlib_stub.returnCounter = 1;
    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 100);
    buffer->fcf_dsn.frametype = FC_CMD_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.securityEnabled = false;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 100-9;
    buffer->security_aux_header_length = 0;
    buffer->mac_class_ptr = rf_mac_setup;
    mcps_sap_pd_ind(buffer);
//
    //Drop by data confirm ind allocate fail
    sw_mac_stub.mac_api_ptr = &mac_api;
    nsdynmemlib_stub.returnCounter = 1;
    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 100);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.securityEnabled = false;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 100-9;
    buffer->security_aux_header_length = 0;
    buffer->mac_class_ptr = rf_mac_setup;
    mcps_sap_pd_ind(buffer);

    //Drp by rx on idle
    //Drop when not waiting data
    nsdynmemlib_stub.returnCounter = 2;
    rf_mac_setup->macWaitingData = false;
    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 100);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.securityEnabled = false;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 100-9;
    buffer->security_aux_header_length = 0;
    buffer->mac_class_ptr = rf_mac_setup;
    mcps_sap_pd_ind(buffer);

    //Drop by brodacst destination
    nsdynmemlib_stub.returnCounter = 2;
    rf_mac_setup->macWaitingData = true;
    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 100);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = false;
    buffer->fcf_dsn.securityEnabled = false;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 100-9;
    buffer->security_aux_header_length = 0;
    buffer->mac_class_ptr = rf_mac_setup;
    //Set Bradcast address
    mac_header_helper_functions_stub.dst_address[0] = 0xff;
    mac_header_helper_functions_stub.dst_address[1] = 0xff;
    mcps_sap_pd_ind(buffer);

    //Drop Empty data packet
    mac_mlme_stub.pollConfirm = 0xfc;
    nsdynmemlib_stub.returnCounter = 2;
    rf_mac_setup->macWaitingData = true;
    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 9);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = true;
    buffer->fcf_dsn.securityEnabled = false;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 0;
    buffer->security_aux_header_length = 0;
    buffer->mac_class_ptr = rf_mac_setup;
    //Set Bradcast address
    mac_header_helper_functions_stub.dst_address[0] = 0;
    mac_header_helper_functions_stub.dst_address[1] = 0;
    mcps_sap_pd_ind(buffer);

    if (mac_mlme_stub.pollConfirm != MLME_NO_DATA) {
        return false;
    }


    expected_data_length = 20;
    data_ind_valid = false;

    mac_mlme_stub.pollConfirm = 0xfc;
    nsdynmemlib_stub.returnCounter = 2;
    rf_mac_setup->macWaitingData = true;
    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 29);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = true;
    buffer->fcf_dsn.securityEnabled = false;
    rf_mac_setup->macDataPollReq = true;
    rf_mac_setup->macRxDataAtPoll = false;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 20;
    buffer->security_aux_header_length = 6;
    buffer->mac_class_ptr = rf_mac_setup;
    //Set Bradcast address
    mac_header_helper_functions_stub.dst_address[0] = 0;
    mac_header_helper_functions_stub.dst_address[1] = 0;
    mcps_sap_pd_ind(buffer);

    if (rf_mac_setup->mac_mlme_event != ARM_NWK_MAC_MLME_INDIRECT_DATA_POLL_AFTER_DATA || !rf_mac_setup->macRxDataAtPoll) {
        return false;
    }

    //Test security data packet

    mac_header_helper_functions_stub.security_header.securityLevel = 5; //securityLevel
    mac_header_helper_functions_stub.security_header.KeyIdMode = MAC_KEY_ID_MODE_IDX;
    mac_header_helper_functions_stub.security_header.KeyIndex = 1;
    mac_header_helper_functions_stub.security_header.frameCounter = 0;
    mac_header_helper_functions_stub.uint8_ptr = tx_buf + 35; // Set Mic pointer

    rf_mac_setup->macCapRxOnIdle = true;
    rf_mac_setup->macWaitingData = false;

    //Unsupported security should call here
    nsdynmemlib_stub.returnCounter = 2;
    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 39);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = true;
    buffer->fcf_dsn.securityEnabled = true;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 20;
    buffer->security_aux_header_length = 6;
    buffer->mac_class_ptr = rf_mac_setup;

    //Shuold generate MLME_UNSUPPORTED_SECURITY COMM Status call back call
    expected_mlme_ind_id_cnf = MLME_COMM_STATUS;
    mlme_ind_status = MLME_UNSUPPORTED_SECURITY;
    mlme_ind_valid = false;

    mcps_sap_pd_ind(buffer);

    if (!mlme_ind_valid) {
        return false;
    }

    //Test MLME_UNSUPPORTED_LEGACY
    mac_header_helper_functions_stub.security_header.securityLevel = 0;
    nsdynmemlib_stub.returnCounter = 2;
    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 39);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = true;
    buffer->fcf_dsn.securityEnabled = true;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 20;
    buffer->security_aux_header_length = 6;
    buffer->mac_class_ptr = rf_mac_setup;

    //Shuold generate MLME_UNSUPPORTED_SECURITY COMM Status call back call
    expected_mlme_ind_id_cnf = MLME_COMM_STATUS;
    mlme_ind_status = MLME_UNSUPPORTED_LEGACY;
    mlme_ind_valid = false;

    mcps_sap_pd_ind(buffer);

    if (!mlme_ind_valid) {
        return false;
    }

    //Enable security at MAC
    mac_header_helper_functions_stub.security_header.securityLevel = 5;
    rf_mac_setup->mac_security_enabled = true;
    //Generate MLME_COUNTER_ERROR
    mac_header_helper_functions_stub.security_header.frameCounter = 0xffffffff;

    nsdynmemlib_stub.returnCounter = 2;
    rf_mac_setup->mac_security_enabled = true;
    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 39);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = true;
    buffer->fcf_dsn.securityEnabled = true;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 20;
    buffer->security_aux_header_length = 6;
    buffer->mac_class_ptr = rf_mac_setup;

    //Shuold generate MLME_UNSUPPORTED_SECURITY COMM Status call back call
    expected_mlme_ind_id_cnf = MLME_COMM_STATUS;
    mlme_ind_status = MLME_COUNTER_ERROR;
    mlme_ind_valid = false;

    mcps_sap_pd_ind(buffer);

    if (!mlme_ind_valid) {
        return false;
    }

    //Generate MLME_UNAVAILABLE_KEY
    mac_header_helper_functions_stub.security_header.frameCounter = 0;
    mac_security_mib_stub.key_ptr = NULL;

    nsdynmemlib_stub.returnCounter = 2;

    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 39);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = true;
    buffer->fcf_dsn.securityEnabled = true;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 20;
    buffer->security_aux_header_length = 6;
    buffer->mac_class_ptr = rf_mac_setup;

    //Shuold generate MLME_UNSUPPORTED_SECURITY COMM Status call back call
    expected_mlme_ind_id_cnf = MLME_COMM_STATUS;
    mlme_ind_status = MLME_UNAVAILABLE_KEY;
    mlme_ind_valid = false;

    mcps_sap_pd_ind(buffer);

    if (!mlme_ind_valid) {
        return false;
    }

    //MLME_UNSUPPORTED_SECURITY
    mac_security_mib_stub.key_ptr = &key_description;

    mac_security_mib_stub.device_ptr = NULL;

    nsdynmemlib_stub.returnCounter = 2;

    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 39);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = true;
    buffer->fcf_dsn.securityEnabled = true;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 20;
    buffer->security_aux_header_length = 6;
    buffer->mac_class_ptr = rf_mac_setup;

    //Shuold generate MLME_UNSUPPORTED_SECURITY COMM Status call back call
    expected_mlme_ind_id_cnf = MLME_COMM_STATUS;
    mlme_ind_status = MLME_UNSUPPORTED_SECURITY;
    mlme_ind_valid = false;

    mcps_sap_pd_ind(buffer);

    if (!mlme_ind_valid) {
        return false;
    }

    //Test drop blacklisted device
    //MLME_UNAVAILABLE_KEY
    key_device_description.Blacklisted = true;
    mac_security_mib_stub.device_ptr = &device_description;
    mac_security_mib_stub.key_dev_ptr = &key_device_description;

    nsdynmemlib_stub.returnCounter = 2;

    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 39);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = true;
    buffer->fcf_dsn.securityEnabled = true;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 20;
    buffer->security_aux_header_length = 6;
    buffer->mac_class_ptr = rf_mac_setup;

    //Shuold generate MLME_UNSUPPORTED_SECURITY COMM Status call back call
    expected_mlme_ind_id_cnf = MLME_COMM_STATUS;
    mlme_ind_status = MLME_UNAVAILABLE_KEY;
    mlme_ind_valid = false;

    mcps_sap_pd_ind(buffer);

    if (!mlme_ind_valid) {
        return false;
    }

    //Test drop REPLay attack
    //MLME_COUNTER_ERROR
    key_device_description.Blacklisted = false;
    device_description.FrameCounter = 1;

    nsdynmemlib_stub.returnCounter = 2;

    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 39);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = true;
    buffer->fcf_dsn.securityEnabled = true;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 20;
    buffer->security_aux_header_length = 6;
    buffer->mac_class_ptr = rf_mac_setup;

    //Shuold generate MLME_UNSUPPORTED_SECURITY COMM Status call back call
    expected_mlme_ind_id_cnf = MLME_COMM_STATUS;
    mlme_ind_status = MLME_COUNTER_ERROR;
    mlme_ind_valid = false;

    mcps_sap_pd_ind(buffer);

    if (!mlme_ind_valid) {
        return false;
    }

    //Test ccm busy or init fail
    //MLME_UNSUPPORTED_SECURITY
    ccm_security_stub_def.ccm_struct_ptr = NULL;
    device_description.FrameCounter = 1;
    mac_header_helper_functions_stub.security_header.frameCounter = 1;
    nsdynmemlib_stub.returnCounter = 2;

    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 39);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = true;
    buffer->fcf_dsn.securityEnabled = true;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 20;
    buffer->security_aux_header_length = 6;
    buffer->mac_class_ptr = rf_mac_setup;

    //Shuold generate MLME_UNSUPPORTED_SECURITY COMM Status call back call
    expected_mlme_ind_id_cnf = MLME_COMM_STATUS;
    mlme_ind_status = MLME_UNSUPPORTED_SECURITY;
    mlme_ind_valid = false;

    mcps_sap_pd_ind(buffer);

    if (!mlme_ind_valid) {
        return false;
    }

    //Generate Enrypt fail
    //MLME_SECURITY_FAIL
    ccm_security_stub_def.ccm_struct_ptr = &ccm_ptr;
    ccm_security_stub_def.value_int8 = -1;

    nsdynmemlib_stub.returnCounter = 2;

    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 39);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = true;
    buffer->fcf_dsn.securityEnabled = true;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 20;
    buffer->security_aux_header_length = 6;
    buffer->mac_class_ptr = rf_mac_setup;

    //Shuold generate MLME_UNSUPPORTED_SECURITY COMM Status call back call
    expected_mlme_ind_id_cnf = MLME_COMM_STATUS;
    mlme_ind_status = MLME_SECURITY_FAIL;
    mlme_ind_valid = false;

    mcps_sap_pd_ind(buffer);

    if (!mlme_ind_valid) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;

    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 16);
    buffer->fcf_dsn.frametype = FC_CMD_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = true;
    buffer->fcf_dsn.securityEnabled = true;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 1;
    buffer->security_aux_header_length = 6;
    buffer->mac_class_ptr = rf_mac_setup;

    //Shuold generate MLME_UNSUPPORTED_SECURITY COMM Status call back call
    expected_mlme_ind_id_cnf = MLME_COMM_STATUS;
    mlme_ind_status = MLME_SECURITY_FAIL;
    mlme_ind_valid = false;

    mcps_sap_pd_ind(buffer);

    if (!mlme_ind_valid) {
        return false;
    }



    //sucesfully CCM decrypt with key desc
    ccm_security_stub_def.value_int8 = 0;
    nsdynmemlib_stub.returnCounter = 2;

    ccm_ptr.mic_len = 4;

    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 39);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = true;
    buffer->fcf_dsn.securityEnabled = true;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 20;
    buffer->security_aux_header_length = 6;
    buffer->mac_class_ptr = rf_mac_setup;

    expected_data_length = 20;
    data_ind_valid = false;

    mcps_sap_pd_ind(buffer);

    if (!data_ind_valid || device_description.FrameCounter != (mac_header_helper_functions_stub.security_header.frameCounter +1)) {
        return false;
    }

    //sucesfully CCM decrypt with key desc
    ccm_security_stub_def.value_int8 = 0;
    nsdynmemlib_stub.returnCounter = 2;
    device_description.FrameCounter--;

    ccm_ptr.mic_len = 0;
    mac_header_helper_functions_stub.security_header.securityLevel = 4;

    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 35);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = true;
    buffer->fcf_dsn.securityEnabled = true;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 20;
    buffer->security_aux_header_length = 6;
    buffer->mac_class_ptr = rf_mac_setup;

    expected_data_length = 20;
    data_ind_valid = false;

    mcps_sap_pd_ind(buffer);

    if (!data_ind_valid || device_description.FrameCounter != (mac_header_helper_functions_stub.security_header.frameCounter +1)) {
        return false;
    }

    //Update key device and key description tables
    ccm_ptr.mic_len = 4;
    mac_header_helper_functions_stub.security_header.securityLevel = 5;

    key_device_description.Blacklisted = false;

    //SET init values which should not be same after
    device_description.FrameCounter = 100;
    key_device_description.DeviceDescriptorHandle = 100;

    mac_security_mib_stub.device_ptr = &device_description;
    mac_security_mib_stub.key_dev_update_ptr = &key_device_description; //For update new key stub response
    mac_security_mib_stub.key_dev_ptr = NULL; //Un used key for this device

    mac_security_mib_stub.uint8_value = 4;

    ccm_security_stub_def.value_int8 = 0;
    nsdynmemlib_stub.returnCounter = 2;

    ccm_ptr.mic_len = 4;

    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 39);
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = true;
    buffer->fcf_dsn.securityEnabled = true;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 20;
    buffer->security_aux_header_length = 6;
    buffer->mac_class_ptr = rf_mac_setup;

    expected_data_length = 20;
    data_ind_valid = false;

    mcps_sap_pd_ind(buffer);

    if (!data_ind_valid || key_device_description.DeviceDescriptorHandle  != 4 || device_description.FrameCounter != (mac_header_helper_functions_stub.security_header.frameCounter +1)) {
        return false;
    }



    //Test Data Request with security and trig dummy data packet
    indirect_data_stub_def.value_uint8 = 0;
    ccm_security_stub_def.value_int8 = 0;
    nsdynmemlib_stub.returnCounter = 1;
    mac_header_helper_functions_stub.uint8_value = MAC_DATA_REQ;



    device_description.FrameCounter--;
    nsdynmemlib_stub.returnCounter = 1;
    buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 20);
    buffer->fcf_dsn.frametype = MAC_FRAME_CMD;
    buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    buffer->fcf_dsn.intraPan = true;
    buffer->fcf_dsn.ackRequested = true;
    buffer->fcf_dsn.securityEnabled = true;
    buffer->mac_header_length = 9;
    buffer->mac_payload_length = 1;
    buffer->security_aux_header_length = 6;
    buffer->mac_class_ptr = rf_mac_setup;

    //Test here first empty data packet allocae fail
     mcps_sap_pd_ind(buffer);

     device_description.FrameCounter--;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 20);
     buffer->fcf_dsn.frametype = MAC_FRAME_CMD;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.intraPan = true;
     buffer->fcf_dsn.ackRequested = true;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 9;
     buffer->mac_payload_length = 1;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;

     //Test here that data request is pushed to queue
     mcps_sap_pd_ind(buffer);

     if (!rf_mac_setup->active_pd_data_request) {
         return false;
     }

     mac_mcps_buffer_queue_free(rf_mac_setup);

     //Test Beacon REQ
     //MAC_BEACON_REQ

     mac_header_helper_functions_stub.uint8_value = MAC_BEACON_REQ;
     device_description.FrameCounter--;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 18);
     buffer->fcf_dsn.frametype = MAC_FRAME_CMD;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.intraPan = false;
     buffer->fcf_dsn.ackRequested = true;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 7;
     buffer->mac_payload_length = 1;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;


     //Test Firts handle when not cordinator
     rf_mac_setup->macCapCordinator = false;
     mcps_sap_pd_ind(buffer);

     device_description.FrameCounter--;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 18);
     buffer->fcf_dsn.frametype = MAC_FRAME_CMD;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.intraPan = false;
     buffer->fcf_dsn.ackRequested = true;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 7;
     buffer->mac_payload_length = 1;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;


     //Test Now drop by unsupported address
     rf_mac_setup->macCapCordinator = true;
     mcps_sap_pd_ind(buffer);

     device_description.FrameCounter--;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 18);
     buffer->fcf_dsn.frametype = MAC_FRAME_CMD;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.intraPan = false;
     buffer->fcf_dsn.ackRequested = true;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 7;
     buffer->mac_payload_length = 1;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;


     //Test succesfully request
     mcps_sap_pd_ind(buffer);

     //test unknow command MAC_COORD_REALIGN

     device_description.FrameCounter--;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 18);
     buffer->fcf_dsn.frametype = MAC_FRAME_CMD;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.intraPan = false;
     buffer->fcf_dsn.ackRequested = true;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 7;
     buffer->mac_payload_length = 1;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;
     mac_header_helper_functions_stub.uint8_value = MAC_COORD_REALIGN;

     mcps_sap_pd_ind(buffer);

     //Test RX beacon filter for dig´ffrenrent pan:
     mac_header_helper_functions_stub.pan_src = 0x2222;
     device_description.FrameCounter--;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 21);
     buffer->fcf_dsn.frametype = MAC_FRAME_BEACON;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.intraPan = false;
     buffer->fcf_dsn.ackRequested = false;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 7;
     buffer->mac_payload_length = 4;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;

     //NO GTS or Pending
     tx_buf[0] = 0xff;
     tx_buf[1] = 0xff;
     tx_buf[2] = 0;
     tx_buf[3] = 0;
     mac_header_helper_functions_stub.uint8_ptr = tx_buf;
     mac_header_helper_functions_stub.uint16_value = 4;
     mcps_sap_pd_ind(buffer);

     if (mac_mlme_stub.beacon_ind.beacon_data_length != 0) {
         return false;
     }
     mac_header_helper_functions_stub.pan_src = 0x0101;

     //RX beacon without payload
     device_description.FrameCounter--;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 21);
     buffer->fcf_dsn.frametype = MAC_FRAME_BEACON;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.intraPan = false;
     buffer->fcf_dsn.ackRequested = false;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 7;
     buffer->mac_payload_length = 4;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;

     //NO GTS or Pending
     tx_buf[0] = 0xff;
     tx_buf[1] = 0xff;
     tx_buf[2] = 0;
     tx_buf[3] = 0;
     mac_header_helper_functions_stub.uint8_ptr = tx_buf;
     mac_header_helper_functions_stub.uint16_value = 4;
     mcps_sap_pd_ind(buffer);

     if (mac_mlme_stub.beacon_ind.beacon_data_length != 0) {
         return false;
     }

     device_description.FrameCounter--;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 25);
     buffer->fcf_dsn.frametype = MAC_FRAME_BEACON;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.intraPan = false;
     buffer->fcf_dsn.ackRequested = false;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 7;
     buffer->mac_payload_length = 8;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;

     //NO GTS or Pending
     tx_buf[0] = 0xff;
     tx_buf[1] = 0xff;
     tx_buf[2] = 0;
     tx_buf[3] = 0;
     mac_header_helper_functions_stub.uint8_ptr = tx_buf;
     mac_header_helper_functions_stub.uint16_value = 8;
     mcps_sap_pd_ind(buffer);

     if (mac_mlme_stub.beacon_ind.beacon_data_length != 4) {
         return false;
     }

     mac_mlme_stub.beacon_ind.beacon_data_length = 0;

     device_description.FrameCounter--;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 25);
     buffer->fcf_dsn.frametype = MAC_FRAME_BEACON;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.intraPan = false;
     buffer->fcf_dsn.ackRequested = false;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 7;
     buffer->mac_payload_length = 8;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;

     //NO GTS or Pending
     tx_buf[0] = 0xff;
     tx_buf[1] = 0xff;
     tx_buf[2] = 2;//GTS length should be 3
     tx_buf[3] = 0;
     mac_header_helper_functions_stub.uint8_ptr = tx_buf;
     mac_header_helper_functions_stub.uint16_value = 8;
     mcps_sap_pd_ind(buffer);

     if (mac_mlme_stub.beacon_ind.beacon_data_length != 0) {
         return false;
     }

     mac_mlme_stub.beacon_ind.beacon_data_length = 0;

     device_description.FrameCounter--;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 28);
     buffer->fcf_dsn.frametype = MAC_FRAME_BEACON;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.intraPan = false;
     buffer->fcf_dsn.ackRequested = false;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 7;
     buffer->mac_payload_length = 11;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;

     //NO GTS or Pending
     tx_buf[0] = 0xff;
     tx_buf[1] = 0xff;
     tx_buf[2] = 2;//GTS length should be 3
     tx_buf[3] = 0;
     tx_buf[4] = 0;
     tx_buf[5] = 0;
     tx_buf[6] = 0;
     tx_buf[7] = 0;
     tx_buf[8] = 0;
     tx_buf[9] = 0;
     mac_header_helper_functions_stub.uint8_ptr = tx_buf;
     mac_header_helper_functions_stub.uint16_value = 11;
     mcps_sap_pd_ind(buffer);

     if (mac_mlme_stub.beacon_ind.beacon_data_length != 1) {
         return false;
     }

     mac_mlme_stub.beacon_ind.beacon_data_length = 0;

     device_description.FrameCounter--;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 30);
     buffer->fcf_dsn.frametype = MAC_FRAME_BEACON;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.intraPan = false;
     buffer->fcf_dsn.ackRequested = false;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 7;
     buffer->mac_payload_length = 13;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;

     //NO GTS or Pending
     tx_buf[0] = 0xff;
     tx_buf[1] = 0xff;
     tx_buf[2] = 2;//GTS length should be 3
     tx_buf[3] = 0;
     tx_buf[4] = 0;
     tx_buf[5] = 0;
     tx_buf[6] = 0;
     tx_buf[7] = 0;
     tx_buf[8] = 0;
     tx_buf[9] = 1;
     mac_header_helper_functions_stub.uint8_ptr = tx_buf;
     mac_header_helper_functions_stub.uint16_value = 13;
     mcps_sap_pd_ind(buffer);

     if (mac_mlme_stub.beacon_ind.beacon_data_length != 1) {
         return false;
     }

     mac_mlme_stub.beacon_ind.beacon_data_length = 0;

     device_description.FrameCounter--;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 30);
     buffer->fcf_dsn.frametype = MAC_FRAME_BEACON;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.intraPan = false;
     buffer->fcf_dsn.ackRequested = false;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 7;
     buffer->mac_payload_length = 13;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;

     //NO GTS or Pending
     tx_buf[0] = 0xff;
     tx_buf[1] = 0xff;
     tx_buf[2] = 2;//GTS length should be 3
     tx_buf[3] = 0;
     tx_buf[4] = 0;
     tx_buf[5] = 0;
     tx_buf[6] = 0;
     tx_buf[7] = 0;
     tx_buf[8] = 0;
     tx_buf[9] = 2;
     mac_header_helper_functions_stub.uint8_ptr = tx_buf;
     mac_header_helper_functions_stub.uint16_value = 13;
     mcps_sap_pd_ind(buffer);

     if (mac_mlme_stub.beacon_ind.beacon_data_length != 0) {
         return false;
     }

     device_description.FrameCounter--;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 30);
     buffer->fcf_dsn.frametype = MAC_FRAME_BEACON;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.intraPan = false;
     buffer->fcf_dsn.ackRequested = false;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 7;
     buffer->mac_payload_length = 13;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;

     //NO GTS or Pending
     tx_buf[0] = 0xff;
     tx_buf[1] = 0xff;
     tx_buf[2] = 2;//GTS length should be 3
     tx_buf[3] = 0;
     tx_buf[4] = 0;
     tx_buf[5] = 0;
     tx_buf[6] = 0;
     tx_buf[7] = 0;
     tx_buf[8] = 0;
     tx_buf[9] = 0x35;
     mac_header_helper_functions_stub.uint8_ptr = tx_buf;
     mac_header_helper_functions_stub.uint16_value = 13;
     mcps_sap_pd_ind(buffer);

     if (mac_mlme_stub.beacon_ind.beacon_data_length != 0) {
         return false;
     }

     //Test Unknow frame

     device_description.FrameCounter--;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 30);
     buffer->fcf_dsn.frametype = MAC_ILLEGAL_LEGACY;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.intraPan = false;
     buffer->fcf_dsn.ackRequested = false;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 7;
     buffer->mac_payload_length = 13;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;
     mcps_sap_pd_ind(buffer);

     //Test virtual MAC data forward
     //Allocate driver
     rf_mac_setup->tun_extension_rf_driver = rf_client_driver_allocate();

     ccm_security_stub_def.value_int8 = 0;
     nsdynmemlib_stub.returnCounter = 2;

     ccm_ptr.mic_len = 4;

     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 39);
     buffer->fcf_dsn.frametype = FC_DATA_FRAME;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.intraPan = true;
     buffer->fcf_dsn.ackRequested = true;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 9;
     buffer->mac_payload_length = 20;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;

     expected_virtual_rx_data_length = 39;
     virtual_rx_data_length_ok = false;

     mcps_sap_pd_ind(buffer);
     if (!virtual_rx_data_length_ok) {
         return false;
     }

     //Validate that length is 39

     //Test Beacon request
     mac_header_helper_functions_stub.uint8_value = MAC_BEACON_REQ;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 18);
     buffer->fcf_dsn.frametype = MAC_FRAME_CMD;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.intraPan = false;
     buffer->fcf_dsn.ackRequested = true;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 7;
     buffer->mac_payload_length = 1;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;
     mcps_sap_pd_ind(buffer);


     //Test Data request
     mac_header_helper_functions_stub.uint8_value = MAC_DATA_REQ;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 18);
     buffer->fcf_dsn.frametype = MAC_FRAME_CMD;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.intraPan = false;
     buffer->fcf_dsn.ackRequested = true;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 7;
     buffer->mac_payload_length = 1;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;

     expected_virtual_rx_data_length = 18;
     virtual_rx_data_length_ok = false;

     mcps_sap_pd_ind(buffer);
     if (!virtual_rx_data_length_ok) {
         return false;
     }

     // Test calling FHSS receive synch request
     rf_mac_setup->fhss_api = ns_fhss_create(0, 0);
     int start_value = fhss_mac_interface_stub.int8_value;
     mac_header_helper_functions_stub.uint8_value = MAC_BEACON_REQ;
     nsdynmemlib_stub.returnCounter = 2;
     buffer =  mcps_sap_pre_parsed_frame_buffer_get(tx_buf, 18);
     buffer->fcf_dsn.frametype = MAC_FRAME_CMD;
     buffer->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
     buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_NONE;
     buffer->fcf_dsn.intraPan = false;
     buffer->fcf_dsn.ackRequested = true;
     buffer->fcf_dsn.securityEnabled = true;
     buffer->mac_header_length = 7;
     buffer->mac_payload_length = 1;
     buffer->security_aux_header_length = 6;
     buffer->mac_class_ptr = rf_mac_setup;
     mcps_sap_pd_ind(buffer);
     if (start_value != (fhss_mac_interface_stub.int8_value - 1)) {
    	 return false;
     }

     test_mac_rf_mac_class_free(rf_mac_setup);
     return true;
}


bool test_mac_mcps_sap_tasklet_init()
{
    return true;
}

bool test_mcps_sap_pre_parsed_frame_buffer_free()
{
    mcps_sap_pre_parsed_frame_buffer_free(NULL);
    mac_pre_parsed_frame_t *buf = malloc(sizeof(mac_pre_parsed_frame_t) + 100);
    mcps_sap_pre_parsed_frame_buffer_free(buf);
    return true;
}

bool test_mcps_sap_prebuild_frame_buffer_get()
{
    mac_pre_build_frame_t * buffer;
    nsdynmemlib_stub.returnCounter = 0;
    buffer = mcps_sap_prebuild_frame_buffer_get(0);
    if (buffer) {
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    buffer = mcps_sap_prebuild_frame_buffer_get(0);
    if (!buffer || buffer->mac_allocated_payload_ptr) {
        return false;
    }
    mcps_sap_prebuild_frame_buffer_free(buffer);
    nsdynmemlib_stub.returnCounter = 1;
    buffer = mcps_sap_prebuild_frame_buffer_get(100);
    if (buffer) {
        return false;
    }
    nsdynmemlib_stub.returnCounter = 2;
    buffer = mcps_sap_prebuild_frame_buffer_get(100);
    if (!buffer || !buffer->mac_allocated_payload_ptr || buffer->mac_payload_length != 100) {
        return false;
    }
    mcps_sap_prebuild_frame_buffer_free(buffer);
    return true;
}

bool test_mcps_sap_prebuild_frame_buffer_free()
{
    mcps_sap_prebuild_frame_buffer_free(NULL);

    mac_pre_build_frame_t *buf_1 = tes_mac_mcps_temporary_buffer_get(10);
    mac_pre_build_frame_t *buf_2 = tes_mac_mcps_temporary_buffer_get(0);

    mcps_sap_prebuild_frame_buffer_free(buf_1);
    mcps_sap_prebuild_frame_buffer_free(buf_2);

    return true;
}

bool test_mcps_sap_pd_req_queue_write()
{
    ccm_globals_t ccm_ptr;
    mac_api_t mac_api;
    mlme_key_descriptor_t key_description;
    mac_aux_security_header_t secuirity_params;
    mcps_data_req_t data_req;
    memset(&data_req, 0, sizeof(mcps_data_req_t));

    uint8_t tx_buf[127];

    uint8_t tx_temp[200];
    protocol_interface_rf_mac_setup_s *rf_mac_setup = test_mac_rf_mac_class_allocate();
    memset(tx_buf, 1, 127);

    memset(&key_description, 0, sizeof(mlme_key_descriptor_t));

    mac_api.data_conf_cb = &test_mcps_data_confirm;
    mac_api.mlme_conf_cb = &test_mlme_confirm;
    mac_api.data_ind_cb = &test_mcps_data_indication;

    rf_mac_setup->macUpState = true;
    sw_mac_stub.mac_api_ptr = &mac_api;
    mac_header_helper_functions_stub.uint8_ptr = tx_temp;

    mcps_sap_pd_req_queue_write(NULL, NULL);
    nsdynmemlib_stub.returnCounter = 2;
    mac_pre_build_frame_t *buffer = mcps_sap_prebuild_frame_buffer_get(80);
    test_mac_buffer_settings(buffer, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, false, &secuirity_params);
    mcps_sap_pd_req_queue_write(rf_mac_setup, buffer);

    if (!rf_mac_setup->active_pd_data_request || rf_mac_setup->active_pd_data_request != buffer) {
        return false;
    }
    nsdynmemlib_stub.returnCounter = 2;
    buffer = mcps_sap_prebuild_frame_buffer_get(80);
    test_mac_buffer_settings(buffer, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, false, &secuirity_params);
    mcps_sap_pd_req_queue_write(rf_mac_setup, buffer);

    if (rf_mac_setup->pd_data_request_queue_to_go != buffer) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    buffer = mcps_sap_prebuild_frame_buffer_get(80);
    test_mac_buffer_settings(buffer, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, false, &secuirity_params);
    mcps_sap_pd_req_queue_write(rf_mac_setup, buffer);

    if (rf_mac_setup->pd_data_request_queue_to_go->next != buffer) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    buffer = mcps_sap_prebuild_frame_buffer_get(80);
    test_mac_buffer_settings(buffer, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, false, &secuirity_params);
    buffer->priority = 2;

    mcps_sap_pd_req_queue_write(rf_mac_setup, buffer);
    if (rf_mac_setup->pd_data_request_queue_to_go != buffer) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    buffer = mcps_sap_prebuild_frame_buffer_get(80);
    test_mac_buffer_settings(buffer, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, false, &secuirity_params);
    buffer->priority = 1;
    mcps_sap_pd_req_queue_write(rf_mac_setup, buffer);

    if (rf_mac_setup->pd_data_request_queue_to_go->next != buffer) {
        return false;
    }
    mac_mcps_buffer_queue_free(rf_mac_setup);

    // FHSS TESTS START
    rf_mac_setup->fhss_api = ns_fhss_create(0, 0);

    nsdynmemlib_stub.returnCounter = 2;
    buffer = mcps_sap_prebuild_frame_buffer_get(80);
    test_mac_buffer_settings(buffer, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, false, &secuirity_params);

    // Buffer has broadcast destination and TX conditions are fine
    buffer->fcf_dsn.ackRequested = false;
    fhss_mac_interface_stub.cond_bool_value = true;
    mcps_sap_pd_req_queue_write(rf_mac_setup, buffer);
    // Should be sent immediately
    if (rf_mac_setup->active_pd_data_request != buffer) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    buffer = mcps_sap_prebuild_frame_buffer_get(80);
    test_mac_buffer_settings(buffer, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, false, &secuirity_params);

    // Buffer has broadcast destination and TX conditions are bad
    buffer->fcf_dsn.ackRequested = false;
    fhss_mac_interface_stub.cond_bool_value = false;
    fhss_mac_interface_stub.queue_bool_value = true;
    mcps_sap_pd_req_queue_write(rf_mac_setup, buffer);
    // Should be first in BC queue, one buffer in a bc queue
    if ((rf_mac_setup->pd_data_request_bc_queue_to_go != buffer) || (rf_mac_setup->broadcast_queue_size != 1)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    buffer = mcps_sap_prebuild_frame_buffer_get(80);
    test_mac_buffer_settings(buffer, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, false, &secuirity_params);

    // Buffer has broadcast destination and TX conditions are bad
    buffer->fcf_dsn.ackRequested = false;
    fhss_mac_interface_stub.cond_bool_value = false;
    mcps_sap_pd_req_queue_write(rf_mac_setup, buffer);
    // Should be second in BC queue, two buffers in a bc queue
    if ((rf_mac_setup->pd_data_request_bc_queue_to_go->next != buffer) || (rf_mac_setup->broadcast_queue_size != 2)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    buffer = mcps_sap_prebuild_frame_buffer_get(80);
    test_mac_buffer_settings(buffer, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, false, &secuirity_params);

    // Buffer has broadcast destination and TX conditions are bad
    buffer->fcf_dsn.ackRequested = false;
    fhss_mac_interface_stub.cond_bool_value = false;
    // Higher priority for this buffer
    buffer->priority = 2;
    mcps_sap_pd_req_queue_write(rf_mac_setup, buffer);
    // Should be first in a BC queue since priority is highest, three buffers in a bc queue
    if ((rf_mac_setup->pd_data_request_bc_queue_to_go != buffer) || (rf_mac_setup->broadcast_queue_size != 3)) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 2;
    buffer = mcps_sap_prebuild_frame_buffer_get(80);
    test_mac_buffer_settings(buffer, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, false, &secuirity_params);

    // Buffer has unicast destination and TX conditions are bad
    buffer->fcf_dsn.ackRequested = true;
    fhss_mac_interface_stub.cond_bool_value = false;
    fhss_mac_interface_stub.queue_bool_value = false;
    mcps_sap_pd_req_queue_write(rf_mac_setup, buffer);
    // Should be first in a UC queue, 1 buffer in a uc queue
    if ((rf_mac_setup->pd_data_request_queue_to_go != buffer) || (rf_mac_setup->unicast_queue_size != 1)) {
        return false;
    }

    mac_mcps_buffer_queue_free(rf_mac_setup);

    nsdynmemlib_stub.returnCounter = 2;
    buffer = mcps_sap_prebuild_frame_buffer_get(80);
    test_mac_buffer_settings(buffer, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, false, &secuirity_params);

    // Buffer has broadcast destination and TX conditions are bad
    buffer->fcf_dsn.ackRequested = false;
    fhss_mac_interface_stub.cond_bool_value = false;
    // Beacon request should go to unicast queue
    buffer->fcf_dsn.frametype = FC_CMD_FRAME;
    mcps_sap_pd_req_queue_write(rf_mac_setup, buffer);
    // Goes to first in a BC queue using push_to_queue label
    if (rf_mac_setup->pd_data_request_queue_to_go != buffer) {
        return false;
    }

    mac_mcps_buffer_queue_free(rf_mac_setup);

    nsdynmemlib_stub.returnCounter = 2;
    buffer = mcps_sap_prebuild_frame_buffer_get(80);
    test_mac_buffer_settings(buffer, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, false, &secuirity_params);

    // Test queueing when broadcast disable
    buffer->fcf_dsn.ackRequested = false;
    rf_mac_setup->macBroadcastDisabled = true;
    fhss_mac_interface_stub.queue_bool_value = true;
    mcps_sap_pd_req_queue_write(rf_mac_setup, buffer);
    // Should be first in a BC queue, 1 buffer in a BC queue
    if ((rf_mac_setup->pd_data_request_bc_queue_to_go != buffer) || (rf_mac_setup->broadcast_queue_size != 1)) {
    	return false;
    }

    mac_mcps_buffer_queue_free(rf_mac_setup);

    test_mac_rf_mac_class_free(rf_mac_setup);
    return true;
}

bool test_mcps_sap_pre_parsed_frame_buffer_get()
{
    uint8_t buffer[8];
    for (int i= 0; i< 8; i++) {
        buffer[i] = i;
    }
    nsdynmemlib_stub.returnCounter = 0;
    mac_pre_parsed_frame_t *buf = mcps_sap_pre_parsed_frame_buffer_get(buffer, 8);
    if (buf) {
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    buf = mcps_sap_pre_parsed_frame_buffer_get(buffer, 8);
    if (!buf || buf->frameLength != 8 ) {
        return false;
    }

    if (memcmp(mac_header_message_start_pointer(buf),buffer , 8) != 0) {
        return false;
    }

    ns_dyn_mem_free(buf);
    return true;
}

bool test_mcps_sap_pd_ind()
{
    mac_pre_parsed_frame_t buffer;
    //Firts test without init
    mcps_sap_pd_ind(NULL);
    mcps_sap_pd_ind(&buffer);

    return true;
}



bool test_mcps_sap_pd_confirm()
{
    protocol_interface_rf_mac_setup_s *rf_mac_setup = test_mac_rf_mac_class_allocate();
    //Firts test without init
    mcps_sap_pd_confirm(NULL);
    mcps_sap_pd_confirm(rf_mac_setup);
    test_mac_rf_mac_class_free(rf_mac_setup);
    return true;
}

bool test_mcps_sap_data_req_handler()
{
    ccm_globals_t ccm_ptr;
    mac_api_t mac_api;
    mlme_key_descriptor_t key_description;
    mlme_device_descriptor_t device_description;
    mac_aux_security_header_t secuirity_params;
    mcps_data_req_t data_req;
    memset(&data_req, 0, sizeof(mcps_data_req_t));

    uint8_t tx_buf[127];
    uint8_t msdu_handle = 0;

    uint8_t tx_temp[200];
    protocol_interface_rf_mac_setup_s *rf_mac_setup = test_mac_rf_mac_class_allocate();
    memset(tx_buf, 1, 127);

    memset(&key_description, 0, sizeof(mlme_key_descriptor_t));
    memset(&device_description, 0, sizeof(mlme_device_descriptor_t));

    mac_api.data_conf_cb = &test_mcps_data_confirm;
    mac_api.mlme_conf_cb = &test_mlme_confirm;
    mac_api.data_ind_cb = &test_mcps_data_indication;
    mac_api.purge_conf_cb = &test_purge_confirm;


    sw_mac_stub.mac_api_ptr = &mac_api;

    data_req.msdu = tx_buf;

    //Test unsupported security case with security enabled request
    //Wait MLME_UNSUPPORTED_SECURITY confirmation status

    rf_mac_setup->mac_security_enabled = false;

    data_req.Key.SecurityLevel = 5;
    data_req.Key.KeyIndex = 1;
    data_req.Key.KeyIdMode = MAC_KEY_ID_MODE_IDX;
    data_req.msduHandle = msdu_handle++;
    expect_data_confirmation_status = MLME_UNSUPPORTED_SECURITY;
    data_confirm_valid = false;

    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    if (rf_mac_setup->active_pd_data_request || !data_confirm_valid) {
        return false;
    }


    //Test Packet Too long case
    //wait MLME_FRAME_TOO_LONG confirmation status
    data_req.Key.SecurityLevel = 0;
    data_req.msduLength = 117;
    expect_data_confirmation_status = MLME_FRAME_TOO_LONG;
    data_confirm_valid = false;
    data_req.msduHandle = msdu_handle++;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    if (rf_mac_setup->active_pd_data_request || !data_confirm_valid) {
        return false;
    }

    //Test Buffer allocate fail
    // Wait MLME_TRANSACTION_OVERFLOW confirmation status
    data_req.msduLength = 116;
    data_req.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    data_req.DstPANId = 1;
    data_req.InDirectTx = false;
    data_req.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    expect_data_confirmation_status = MLME_TRANSACTION_OVERFLOW;
    data_confirm_valid = false;
    nsdynmemlib_stub.returnCounter = 0;
    data_req.msduHandle = msdu_handle++;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    if (rf_mac_setup->active_pd_data_request || !data_confirm_valid) {
        return false;
    }

    //Test Radio shutdown case
    //wait MLME_TRX_OFF confirmation status
    expect_data_confirmation_status = MLME_TRX_OFF;
    data_confirm_valid = false;
    nsdynmemlib_stub.returnCounter = 1;
    rf_mac_setup->macUpState = false;
    data_req.msduHandle = msdu_handle++;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    if (rf_mac_setup->active_pd_data_request || !data_confirm_valid) {
        return false;
    }


    // Test Direct data push fail by security fail
    // Wait MLME_SECURITY_FAIL
    data_req.Key.SecurityLevel = 5;
    data_req.msduLength = 85;
    rf_mac_setup->macUpState = true;
    rf_mac_setup->mac_security_enabled = true;
    mac_security_mib_stub.key_ptr = NULL;
    mac_security_mib_stub.device_ptr = NULL;
    mac_header_helper_functions_stub.security_header.securityLevel = 5; //securityLevel
    mac_header_helper_functions_stub.security_header.KeyIdMode = MAC_KEY_ID_MODE_IDX;
    mac_header_helper_functions_stub.security_header.KeyIndex = 1;


    expect_data_confirmation_status = MLME_UNAVAILABLE_KEY;
    data_confirm_valid = false;
    nsdynmemlib_stub.returnCounter = 1;
    data_req.msduHandle = msdu_handle++;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    if (rf_mac_setup->active_pd_data_request || !data_confirm_valid) {
        return false;
    }


    //Test with unknow neighbor
    mac_security_mib_stub.key_ptr = &key_description;

    expect_data_confirmation_status = MLME_UNAVAILABLE_KEY;
    data_confirm_valid = false;
    nsdynmemlib_stub.returnCounter = 1;
    data_req.TxAckReq = true;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    data_req.msduHandle = msdu_handle++;
    if (rf_mac_setup->active_pd_data_request || !data_confirm_valid) {
        return false;
    }

    //Test Direct TX with succesfully to active push

    mac_security_mib_stub.device_ptr = &device_description;

    ccm_security_stub_def.ccm_struct_ptr = &ccm_ptr;

    mac_header_helper_functions_stub.uint8_ptr = tx_temp;

    nsdynmemlib_stub.returnCounter = 1;
    data_req.msduHandle = msdu_handle++;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    if (!rf_mac_setup->active_pd_data_request) {
        return false;
    }
    data_req.msduLength = 75;
    mac_header_helper_functions_stub.security_header.securityLevel = 6; //securityLevel
    mac_header_helper_functions_stub.security_header.KeyIdMode = MAC_KEY_ID_MODE_SRC4_IDX;
    data_req.DstAddrMode = MAC_ADDR_MODE_64_BIT;
    data_req.SrcAddrMode = MAC_ADDR_MODE_NONE;
    nsdynmemlib_stub.returnCounter = 1;
    data_req.msduHandle = msdu_handle++;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    if (!rf_mac_setup->active_pd_data_request || !rf_mac_setup->pd_data_request_queue_to_go) {
        return false;
    }

    data_req.msduLength = 40;
    mac_header_helper_functions_stub.security_header.securityLevel = 7; //securityLevel
    mac_header_helper_functions_stub.security_header.KeyIdMode = MAC_KEY_ID_MODE_SRC8_IDX;
    data_req.DstAddrMode = MAC_ADDR_MODE_64_BIT;
    data_req.SrcAddrMode = MAC_ADDR_MODE_NONE;
    rf_mac_setup->shortAdressValid = true;
    nsdynmemlib_stub.returnCounter = 1;
    data_req.msduHandle = msdu_handle++;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    if (!rf_mac_setup->active_pd_data_request || !rf_mac_setup->pd_data_request_queue_to_go || !rf_mac_setup->pd_data_request_queue_to_go->next) {
        return false;
    }

    data_req.msduLength = 40;
    mac_header_helper_functions_stub.security_header.securityLevel = 4; //securityLevel
    mac_header_helper_functions_stub.security_header.KeyIdMode = MAC_KEY_ID_MODE_IMPLICIT;
    data_req.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    data_req.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    data_req.InDirectTx = true;
    data_req.DstPANId = 0;
    data_req.TxAckReq = true;
    rf_mac_setup->shortAdressValid = true;
    indirect_data_stub_def.indirect_buf = NULL;
    nsdynmemlib_stub.returnCounter = 1;
    data_req.msduHandle = msdu_handle++;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    if (!rf_mac_setup->active_pd_data_request || !rf_mac_setup->pd_data_request_queue_to_go || !rf_mac_setup->pd_data_request_queue_to_go->next || !indirect_data_stub_def.indirect_buf) {
        return false;
    }
    if (!rf_mac_setup->indirect_pd_data_request_queue) {
        return false;
    }
    //Use confirmable for release indirect packet
    mcps_purge_t purge;
    purge.msduHandle = data_req.msduHandle;

    expected_purge_status = MLME_SUCCESS;
    expected_purge_handle = data_req.msduHandle;
    expected_purge_result = false;

    mcps_sap_purge_reg_handler(rf_mac_setup, &purge);
    if (!expected_purge_result) {
        return false;
    }
    expected_purge_result = false;
    expected_purge_status = MLME_INVALID_HANDLE;
    mcps_sap_purge_reg_handler(rf_mac_setup, &purge);
    if (!expected_purge_result) {
        return false;
    }

    indirect_data_stub_def.indirect_buf = NULL;


    mac_mcps_buffer_queue_free(rf_mac_setup);

    data_req.msduLength = 75;
    mac_header_helper_functions_stub.security_header.securityLevel = 6; //securityLevel
    mac_header_helper_functions_stub.security_header.KeyIdMode = MAC_KEY_ID_MODE_SRC4_IDX;
    data_req.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    data_req.SrcAddrMode = MAC_ADDR_MODE_NONE;
    data_req.TxAckReq = false;
    data_req.InDirectTx = false;
    data_req.DstAddr[0] = 0xff;
    data_req.DstAddr[1] = 0xff;
    nsdynmemlib_stub.returnCounter = 1;
    data_req.msduHandle = msdu_handle++;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    if (!rf_mac_setup->active_pd_data_request ) {
        return false;
    }


    nsdynmemlib_stub.returnCounter = 1;
    data_req.msduHandle = msdu_handle++;
    mcps_sap_data_req_handler(rf_mac_setup, &data_req);
    if (!rf_mac_setup->active_pd_data_request || !rf_mac_setup->pd_data_request_queue_to_go) {
        return false;
    }

    purge.msduHandle = data_req.msduHandle;

    mcps_sap_purge_reg_handler(rf_mac_setup, &purge);

    mac_mcps_buffer_queue_free(rf_mac_setup);


    //Test indirect data request with security
    //mcps_sap_data_req_handler();

    //Test indirect data request without security
    //mcps_sap_data_req_handler();

    test_mac_rf_mac_class_free(rf_mac_setup);
    return true;
}

bool test_mac_mcps_trig_buffer_from_queue()
{

    ccm_globals_t ccm_ptr;
    mac_api_t mac_api;
    mlme_key_descriptor_t key_description;
    mlme_device_descriptor_t device_description;
    mac_aux_security_header_t secuirity_params;
    uint8_t tx_buf[127];
    protocol_interface_rf_mac_setup_s *rf_mac_setup = test_mac_rf_mac_class_allocate();
    memset(tx_buf, 1, 127);

    memset(&key_description, 0, sizeof(mlme_key_descriptor_t));
    memset(&device_description, 0, sizeof(mlme_device_descriptor_t));

    mac_api.data_conf_cb = &test_mcps_data_confirm;
    mac_api.mlme_conf_cb = &test_mlme_confirm;
    mac_api.data_ind_cb = &test_mcps_data_indication;

    //NULL pointer test
    mac_mcps_trig_buffer_from_queue(NULL);


    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (rf_mac_setup->active_pd_data_request) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }
    //allocate test buffer

    mac_pre_build_frame_t *buf = tes_mac_mcps_temporary_buffer_get(0);
    rf_mac_setup->pd_data_request_queue_to_go = buf;
    mac_mcps_trig_buffer_from_queue(rf_mac_setup);

    if (!rf_mac_setup->active_pd_data_request || rf_mac_setup->pd_data_request_queue_to_go) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    mac_mcps_buffer_queue_free(rf_mac_setup);

    rf_mac_setup->dev_driver->phy_driver->phy_header_length = 1;
    rf_mac_setup->pd_data_request_queue_to_go = tes_mac_mcps_temporary_buffer_get(0);
    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (!rf_mac_setup->active_pd_data_request || rf_mac_setup->pd_data_request_queue_to_go) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    mac_mcps_buffer_queue_free(rf_mac_setup);
    rf_mac_setup->dev_driver->phy_driver->phy_header_length = 0;
    //Test Data Frame types
    buf = tes_mac_mcps_temporary_buffer_get(20);
    rf_mac_setup->pd_data_request_queue_to_go = buf;
    // No security and with acks

    //Send Beacon

    test_mac_buffer_settings(buf, MAC_FRAME_BEACON, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, false, &secuirity_params);
    mac_header_helper_functions_stub.uint8_ptr = rf_mac_setup->dev_driver_tx_buffer.buf + buf->mac_header_length_with_security;

    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (!rf_mac_setup->active_pd_data_request || rf_mac_setup->pd_data_request_queue_to_go) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }
    mac_mcps_buffer_queue_free(rf_mac_setup);

    buf = tes_mac_mcps_temporary_buffer_get(20);

    rf_mac_setup->pd_data_request_queue_to_go = buf;
    test_mac_buffer_settings(buf, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_NONE, false, false, false, &secuirity_params);
    mac_header_helper_functions_stub.uint8_ptr = rf_mac_setup->dev_driver_tx_buffer.buf + buf->mac_header_length_with_security;

    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (!rf_mac_setup->active_pd_data_request || rf_mac_setup->pd_data_request_queue_to_go) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }
    mac_mcps_buffer_queue_free(rf_mac_setup);

    //Send too long packet

    //Set SW MAC stub class

    expect_data_confirmation_status = MLME_FRAME_TOO_LONG;
    data_confirm_valid = false;
    sw_mac_stub.mac_api_ptr = &mac_api;
    buf = tes_mac_mcps_temporary_buffer_get(0);
    buf->mac_payload = tx_buf;
    buf->mac_payload_length = 110;
    buf->upper_layer_request = true;
    rf_mac_setup->pd_data_request_queue_to_go = buf;
    test_mac_buffer_settings(buf, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, false, &secuirity_params);
    mac_header_helper_functions_stub.uint8_ptr = rf_mac_setup->dev_driver_tx_buffer.buf + buf->mac_header_length_with_security;

    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (rf_mac_setup->active_pd_data_request || !data_confirm_valid) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    //First unknow key
    expect_data_confirmation_status = MLME_UNAVAILABLE_KEY;
    data_confirm_valid = false;
    sw_mac_stub.mac_api_ptr = &mac_api;
    mac_security_mib_stub.key_ptr = NULL;
    mac_security_mib_stub.device_ptr = NULL;
    buf = tes_mac_mcps_temporary_buffer_get(0);
    buf->mac_payload = tx_buf;
    buf->mac_payload_length = 85;
    buf->upper_layer_request = true;
    rf_mac_setup->pd_data_request_queue_to_go = buf;
    test_set_security_params(&secuirity_params, 4, MAC_KEY_ID_MODE_IDX, 1);
    test_mac_buffer_settings(buf, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, true, &secuirity_params);
    mac_header_helper_functions_stub.uint8_ptr = rf_mac_setup->dev_driver_tx_buffer.buf + buf->mac_header_length_with_security;

    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (rf_mac_setup->active_pd_data_request || !data_confirm_valid) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    //Test security counter overflow
    expect_data_confirmation_status = MLME_COUNTER_ERROR;
    data_confirm_valid = false;

    mac_security_mib_stub.key_ptr = &key_description;
    mac_security_mib_stub.device_ptr = &device_description;



    rf_mac_setup->security_frame_counter = 0xffffffff;
    buf = tes_mac_mcps_temporary_buffer_get(0);
    buf->mac_payload = tx_buf;
    buf->mac_payload_length = 85;
    buf->upper_layer_request = true;
    rf_mac_setup->pd_data_request_queue_to_go = buf;
    test_set_security_params(&secuirity_params, 4, MAC_KEY_ID_MODE_IDX, 1);
    test_mac_buffer_settings(buf, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, true, &secuirity_params);
    mac_header_helper_functions_stub.uint8_ptr = rf_mac_setup->dev_driver_tx_buffer.buf + buf->mac_header_length_with_security;

    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (rf_mac_setup->active_pd_data_request || !data_confirm_valid) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    //Test busy CCM process
    expect_data_confirmation_status = MLME_SECURITY_FAIL;
    data_confirm_valid = false;
    ccm_security_stub_def.ccm_struct_ptr = NULL;
    rf_mac_setup->security_frame_counter = 0;
    buf = tes_mac_mcps_temporary_buffer_get(0);
    buf->mac_payload = tx_buf;
    buf->mac_payload_length = 85;
    buf->upper_layer_request = true;
    rf_mac_setup->pd_data_request_queue_to_go = buf;
    test_set_security_params(&secuirity_params, 4, MAC_KEY_ID_MODE_IDX, 1);
    test_mac_buffer_settings(buf, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, true, &secuirity_params);
    mac_header_helper_functions_stub.uint8_ptr = rf_mac_setup->dev_driver_tx_buffer.buf + buf->mac_header_length_with_security;

    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (rf_mac_setup->active_pd_data_request || !data_confirm_valid) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    //First Succesfully builded Data packet with security
    ccm_security_stub_def.ccm_struct_ptr = &ccm_ptr;
    rf_mac_setup->security_frame_counter = 0;
    buf = tes_mac_mcps_temporary_buffer_get(0);
    buf->mac_payload = tx_buf;
    buf->mac_payload_length = 50;
    buf->upper_layer_request = true;
    rf_mac_setup->pd_data_request_queue_to_go = buf;
    test_set_security_params(&secuirity_params, 5, MAC_KEY_ID_MODE_IDX, 1);
    test_mac_buffer_settings(buf, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, true, &secuirity_params);
    mac_header_helper_functions_stub.uint8_ptr = rf_mac_setup->dev_driver_tx_buffer.buf + buf->mac_header_length_with_security;

    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (!rf_mac_setup->active_pd_data_request) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    mac_mcps_buffer_queue_free(rf_mac_setup);

    buf = tes_mac_mcps_temporary_buffer_get(0);
    buf->mac_payload = tx_buf;
    buf->mac_payload_length = 50;
    buf->upper_layer_request = true;
    rf_mac_setup->pd_data_request_queue_to_go = buf;
    test_set_security_params(&secuirity_params, 6, MAC_KEY_ID_MODE_SRC8_IDX, 1);
    test_mac_buffer_settings(buf, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, true, &secuirity_params);
    mac_header_helper_functions_stub.uint8_ptr = rf_mac_setup->dev_driver_tx_buffer.buf + buf->mac_header_length_with_security;

    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (!rf_mac_setup->active_pd_data_request) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    mac_mcps_buffer_queue_free(rf_mac_setup);

    buf = tes_mac_mcps_temporary_buffer_get(0);
    buf->mac_payload = tx_buf;
    buf->mac_payload_length = 50;
    buf->upper_layer_request = true;
    rf_mac_setup->pd_data_request_queue_to_go = buf;
    test_set_security_params(&secuirity_params, 7, MAC_KEY_ID_MODE_SRC4_IDX, 1);
    test_mac_buffer_settings(buf, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, true, &secuirity_params);
    mac_header_helper_functions_stub.uint8_ptr = rf_mac_setup->dev_driver_tx_buffer.buf + buf->mac_header_length_with_security;

    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (!rf_mac_setup->active_pd_data_request) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    mac_mcps_buffer_queue_free(rf_mac_setup);


    //Test without callback Negative confirmation handle
    ccm_security_stub_def.ccm_struct_ptr = NULL;
    expected_mlme_id_cnf = MLME_POLL;
    buf = tes_mac_mcps_temporary_buffer_get(0);
    rf_mac_setup->pd_data_request_queue_to_go = buf;
    test_set_security_params(&secuirity_params, 7, MAC_KEY_ID_MODE_SRC4_IDX, 1);
    test_mac_buffer_settings(buf, MAC_FRAME_CMD, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, true, &secuirity_params);
    buf->mac_command_id = DATA_REQUEST;

    mac_header_helper_functions_stub.uint8_ptr = rf_mac_setup->dev_driver_tx_buffer.buf + buf->mac_header_length_with_security;

    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (rf_mac_setup->active_pd_data_request || !mlme_confirm_valid) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }


    buf = tes_mac_mcps_temporary_buffer_get(0);
    rf_mac_setup->pd_data_request_queue_to_go = buf;
    test_set_security_params(&secuirity_params, 7, MAC_KEY_ID_MODE_SRC4_IDX, 1);
    test_mac_buffer_settings(buf, MAC_FRAME_CMD, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, true, &secuirity_params);
    buf->mac_command_id = MAC_BEACON_REQ;

    mac_header_helper_functions_stub.uint8_ptr = rf_mac_setup->dev_driver_tx_buffer.buf + buf->mac_header_length_with_security;

    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (rf_mac_setup->active_pd_data_request) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }




    //Test without callback
    buf = tes_mac_mcps_temporary_buffer_get(0);
    buf->mac_payload = tx_buf;
    buf->mac_payload_length = 110;
    buf->upper_layer_request = false;
    rf_mac_setup->pd_data_request_queue_to_go = buf;

    test_mac_buffer_settings(buf, FC_DATA_FRAME, MAC_ADDR_MODE_64_BIT,MAC_ADDR_MODE_64_BIT, true, true, true, &secuirity_params);

    test_set_security_params(&secuirity_params, 7, MAC_KEY_ID_MODE_IMPLICIT, 1);
    mac_header_helper_functions_stub.uint8_ptr = rf_mac_setup->dev_driver_tx_buffer.buf + buf->mac_header_length_with_security;

    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (rf_mac_setup->active_pd_data_request) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }
    mac_mcps_buffer_queue_free(rf_mac_setup);

	// FHSS TESTS START
    rf_mac_setup->fhss_api = ns_fhss_create(0, 0);
    buf = tes_mac_mcps_temporary_buffer_get(0);
    rf_mac_setup->pd_data_request_bc_queue_to_go = buf;
    rf_mac_setup->broadcast_queue_size = 1;

    // FHSS is not on broadcast channel and tx conditions are fine
    fhss_mac_interface_stub.channel_bool_value = false;
    fhss_mac_interface_stub.cond_bool_value = true;
    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (!rf_mac_setup->pd_data_request_bc_queue_to_go) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    // FHSS is on broadcast channel and tx conditions are bad
    fhss_mac_interface_stub.channel_bool_value = true;
    fhss_mac_interface_stub.cond_bool_value = false;
    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (!rf_mac_setup->pd_data_request_bc_queue_to_go) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }

    // FHSS is on broadcast channel and tx conditions are fine
    fhss_mac_interface_stub.channel_bool_value = true;
    fhss_mac_interface_stub.cond_bool_value = true;
    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    if (!rf_mac_setup->active_pd_data_request || rf_mac_setup->pd_data_request_bc_queue_to_go || rf_mac_setup->broadcast_queue_size) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }
    mac_mcps_buffer_queue_free(rf_mac_setup);

    // Test taking second packet from unicast queue
    rf_mac_setup->fhss_api = ns_fhss_create(0, 0);
    buf = tes_mac_mcps_temporary_buffer_get(0);
    mac_pre_build_frame_t *buf2 = tes_mac_mcps_temporary_buffer_get(0);
    // First packet is Beacon request and sending it is currently not allowed
    buf->fcf_dsn.frametype = FC_CMD_FRAME;
    rf_mac_setup->pd_data_request_queue_to_go = buf;
    rf_mac_setup->pd_data_request_queue_to_go->next = buf2;
    rf_mac_setup->unicast_queue_size = 2;
    fhss_mac_interface_stub.channel_bool_value = false;
    fhss_mac_interface_stub.cond_bool_value = true;
    // Should trig second packet from queue
    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
    // First packet should still be the Beacon request
    if (rf_mac_setup->pd_data_request_queue_to_go != buf) {
        test_mac_rf_mac_class_free(rf_mac_setup);
        return false;
    }
    mac_mcps_buffer_queue_free(rf_mac_setup);
	// Free test setup struct
    test_mac_rf_mac_class_free(rf_mac_setup);
    return true;
}



bool test_mac_mcps_buffer_queue_free()
{
    protocol_interface_rf_mac_setup_s *rf_mac_setup = test_mac_rf_mac_class_allocate();

    //Allocate 5 buffer

    //Allocate 1 active
    rf_mac_setup->active_pd_data_request = tes_mac_mcps_temporary_buffer_get(2);
    //allocate 2 pending
    rf_mac_setup->pd_data_request_queue_to_go = tes_mac_mcps_temporary_buffer_get(0);
    rf_mac_setup->pd_data_request_queue_to_go->next = tes_mac_mcps_temporary_buffer_get(20);
    //allocate 2 buffers in broadcast queue
    rf_mac_setup->pd_data_request_bc_queue_to_go = tes_mac_mcps_temporary_buffer_get(0);
    rf_mac_setup->pd_data_request_bc_queue_to_go->next = tes_mac_mcps_temporary_buffer_get(20);
    //allocate 2 indirect pending
    rf_mac_setup->indirect_pd_data_request_queue = tes_mac_mcps_temporary_buffer_get(0);
    rf_mac_setup->indirect_pd_data_request_queue->next = tes_mac_mcps_temporary_buffer_get(20);

    mac_mcps_buffer_queue_free(rf_mac_setup);
    test_mac_rf_mac_class_free(rf_mac_setup);
    return true;
}
