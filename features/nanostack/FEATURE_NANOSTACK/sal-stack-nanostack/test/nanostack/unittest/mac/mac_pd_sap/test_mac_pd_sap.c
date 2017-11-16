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
#include "inttypes.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/IEEE802_15_4/mac_pd_sap.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/IEEE802_15_4/mac_header_helper_functions.h"
#include "MAC/rf_driver_storage.h"

#include "fhss_api.h"
#include "test_mac_pd_sap.h"
#include "mac_header_helper_functions_stub.h"
#include "mac_mcps_sap_stub.h"
#include "nsdynmemLIB_stub.h"
#include "fhss_mac_interface_stub.h"
#include <string.h>

static int8_t tx_return = -1;

static int8_t test_rf_extension(phy_extension_type_e extension, uint8_t *buf) {
    switch (extension) {
        case PHY_EXTENSION_READ_LAST_ACK_PENDING_STATUS:
            *buf = 1;
            break;
        default:
            break;
    }
    return 0;
}

static int8_t test_rf_tx(uint8_t *data, uint16_t length, uint8_t tx_handle, data_protocol_e protocol) {
    return tx_return;
}
static int8_t test_rf_virtual_tx(const virtual_data_req_t *data_req,int8_t driver_id) {
    return 0;
}


bool test_mac_pd_sap_req()
{
    protocol_interface_rf_mac_setup_s rf_ptr;
    arm_device_driver_list_s driver;
    mac_fcf_sequence_t fcf;
    arm_phy_sap_msg_t phy_message;
    phy_device_driver_s dev_driver;
    mac_pre_build_frame_t temp_buf;


    uint8_t tx_buffer[127];

    memset(&rf_ptr, 0, sizeof(protocol_interface_rf_mac_setup_s));
    memset(&driver, 0, sizeof(arm_device_driver_list_s));
    memset(&dev_driver, 0, sizeof(phy_device_driver_s));
    memset(&phy_message, 0, sizeof(arm_phy_sap_msg_t));
    memset(&fcf, 0, sizeof(mac_fcf_sequence_t));
    memset(&temp_buf, 0, sizeof(mac_pre_build_frame_t));
    rf_ptr.active_pd_data_request = &temp_buf;

    rf_ptr.dev_driver_tx_buffer.buf = tx_buffer;
    rf_ptr.dev_driver_tx_buffer.len = 100;
    dev_driver.extension = &test_rf_extension;
    dev_driver.tx = &test_rf_tx;
    driver.phy_driver = &dev_driver;
    rf_ptr.dev_driver = &driver;
    rf_ptr.macUpState = false;
    if (-1 != mac_pd_sap_req(&rf_ptr) ) {
        return false;
    }
    rf_ptr.macUpState = true;
    temp_buf.fcf_dsn.frametype = MAC_FRAME_BEACON;
    if (0 != mac_pd_sap_req(&rf_ptr) ) {
        return false;
    }
    temp_buf.fcf_dsn.frametype = MAC_FRAME_DATA;
    if (0 != mac_pd_sap_req(&rf_ptr) ) {
        return false;
    }
    return true;
}

bool test_mac_pd_sap_rf_low_level_function_set()
{
    protocol_interface_rf_mac_setup_s rf_ptr;
    arm_device_driver_list_s driver;
    mac_fcf_sequence_t fcf;
    arm_phy_sap_msg_t phy_message;
    phy_device_driver_s dev_driver;

    uint8_t tx_buffer[127];

    memset(&rf_ptr, 0, sizeof(protocol_interface_rf_mac_setup_s));
    memset(&driver, 0, sizeof(arm_device_driver_list_s));
    memset(&dev_driver, 0, sizeof(phy_device_driver_s));
    memset(&phy_message, 0, sizeof(arm_phy_sap_msg_t));
    memset(&fcf, 0, sizeof(mac_fcf_sequence_t));


    rf_ptr.dev_driver_tx_buffer.buf = tx_buffer;
    rf_ptr.dev_driver_tx_buffer.len = 100;
    dev_driver.extension = &test_rf_extension;
    dev_driver.tx = &test_rf_tx;
    driver.phy_driver = &dev_driver;
    rf_ptr.dev_driver = &driver;
    mac_pd_sap_rf_low_level_function_set(&rf_ptr,&driver);
    if (!driver.phy_sap_identifier || !driver.phy_sap_upper_cb) {
        return false;
    }
    return true;
}



bool test_mac_pd_sap_data_cb()
{
    protocol_interface_rf_mac_setup_s rf_ptr;
    arm_device_driver_list_s driver;
    mac_fcf_sequence_t fcf;
    arm_phy_sap_msg_t phy_message;
    phy_device_driver_s dev_driver;

    mac_pre_build_frame_t *frame_buf = malloc(sizeof(mac_pre_build_frame_t) + 32);


    uint8_t buffer[127];
    uint8_t tx_buffer[127];

    uint8_t *ptr = buffer;

    memset(buffer, 0, 127);
    memset(&rf_ptr, 0, sizeof(protocol_interface_rf_mac_setup_s));
    memset(&driver, 0, sizeof(arm_device_driver_list_s));
    memset(&phy_message, 0, sizeof(arm_phy_sap_msg_t));
    memset(&fcf, 0, sizeof(mac_fcf_sequence_t));


    rf_ptr.dev_driver_tx_buffer.buf = tx_buffer;
    rf_ptr.dev_driver_tx_buffer.len = 100;
    dev_driver.extension = &test_rf_extension;
    dev_driver.tx = &test_rf_tx;
    driver.phy_driver = &dev_driver;
    rf_ptr.dev_driver = &driver;
    rf_ptr.macMaxCSMABackoffs = 4;
    rf_ptr.macMaxBE = 5;
    rf_ptr.macMinBE = 3;

    int8_t ret = mac_pd_sap_data_cb(NULL, NULL);
    if( ret != -1 ){
        free(frame_buf);
        return false;
    }
    //Test mac dwn state
    rf_ptr.macUpState = false;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -2 ){
        free(frame_buf);
        return false;
    }
    //Test unkow message id

    phy_message.id = MACTUN_PD_SAP_NAP_IND;
    rf_ptr.macUpState = true;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        free(frame_buf);
        return false;
    }
    //Test length < 3
    phy_message.id = MAC15_4_PD_SAP_DATA_IND;
    phy_message.message.generic_data_ind.data_ptr = buffer;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        free(frame_buf);
        return false;
    }

    //Test buffer allocate fail
    mac_mcps_sap_stub.pre_parsed = NULL;
    nsdynmemlib_stub.returnCounter = 0;
    phy_message.message.generic_data_ind.data_len = 32;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -3 ){
        free(frame_buf);
        return false;
    }
    //Test first prosimus mode
    mac_mcps_sap_stub.pre_parsed = frame_buf;
    nsdynmemlib_stub.returnCounter = 2;
    mac_header_helper_functions_stub.fcf = fcf;
    mac_mcps_sap_stub.int8_value = -1;
    rf_ptr.macProminousMode = true;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        free(frame_buf);
        return false;
    }

    mac_mcps_sap_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 1;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != 0 ){
        return false;
    }
    //Test dropped data type (ACK)
    rf_ptr.macProminousMode = false;
    fcf.frametype = FC_ACK_FRAME;
    mac_header_helper_functions_stub.fcf = fcf;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        return false;
    }
    //Test dropped data type (UN SUPPORTED)
    rf_ptr.macProminousMode = false;
    fcf.frametype = MAC_FCF_FRAME_TYPE_MASK;
    mac_header_helper_functions_stub.fcf = fcf;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        return false;
    }

    //Test malformed message
    fcf.frametype = FC_DATA_FRAME;
    fcf.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    phy_message.message.generic_data_ind.data_len = 3;
    mac_header_helper_functions_stub.fcf = fcf;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        return false;
    }
    phy_message.message.generic_data_ind.data_len = 32;
    mac_header_helper_functions_stub.fcf = fcf;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        return false;
    }


    //Test malformed security
    fcf.DstAddrMode = MAC_ADDR_MODE_64_BIT;
    fcf.SrcAddrMode = MAC_ADDR_MODE_64_BIT;
    fcf.securityEnabled = true;
    fcf.intraPan = true;

    mac_header_helper_functions_stub.fcf = fcf;
    ptr += 3 + 10 + 8;
    phy_message.message.generic_data_ind.data_len = 22;
    *ptr = ((MAC_KEY_ID_MODE_IMPLICIT << 3 ) + 1);
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        return false;
    }

    phy_message.message.generic_data_ind.data_len = 22;
    *ptr = ((MAC_KEY_ID_MODE_IDX << 3) + 2);
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        return false;
    }

    phy_message.message.generic_data_ind.data_len = 22;
    *ptr = ((MAC_KEY_ID_MODE_SRC4_IDX << 3) + 3);
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        return false;
    }

    phy_message.message.generic_data_ind.data_len = 22;
    *ptr = ((MAC_KEY_ID_MODE_SRC8_IDX << 3) + 4);
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        return false;
    }

    phy_message.message.generic_data_ind.data_len = 22;
    *ptr = (0);
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != 0 ){
        return false;
    }

    fcf.DstAddrMode = MAC_ADDR_MODE_64_BIT;
    fcf.SrcAddrMode = MAC_ADDR_MODE_16_BIT;

    ptr += 3 + 10 + 2;
    mac_header_helper_functions_stub.fcf = fcf;

    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != 0 ){
        return false;
    }

    fcf.DstAddrMode = MAC_ADDR_MODE_64_BIT;
    fcf.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    fcf.intraPan = false;
    ptr += 3 + 10 + 4;
    mac_header_helper_functions_stub.fcf = fcf;

    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != 0 ){
        return false;
    }

    fcf.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    fcf.SrcAddrMode = MAC_ADDR_MODE_64_BIT;
    ptr += 3 + 10 + 4;
    fcf.intraPan = false;
    mac_header_helper_functions_stub.fcf = fcf;
    *ptr = (4);
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != 0 ){
        return false;
    }
    fcf.DstAddrMode = MAC_ADDR_MODE_NONE;
    fcf.SrcAddrMode = MAC_ADDR_MODE_NONE;
    fcf.frametype = FC_BEACON_FRAME;
    fcf.securityEnabled = false;
    mac_header_helper_functions_stub.fcf = fcf;
    //Test Beacon frame
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        return false;
    }

    fcf.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    fcf.frametype = FC_BEACON_FRAME;
    fcf.securityEnabled = false;
    mac_header_helper_functions_stub.fcf = fcf;
    //Test Beacon frame
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != 0 ){
        return false;
    }

    //Test Command frame
    fcf.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    fcf.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    fcf.frametype = FC_CMD_FRAME;
    fcf.securityEnabled = false;
    fcf.intraPan = true;
    mac_header_helper_functions_stub.fcf = fcf;
    ptr += 3 + 4 + 2;
    *ptr = 1;
    //Test payload length zero
    phy_message.message.generic_data_ind.data_len = 9;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        return false;
    }

    phy_message.message.generic_data_ind.data_len = 10;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != 0 ){
        return false;
    }
    //Test unsupported frame type
    fcf.frametype = MAC_FCF_FRAME_TYPE_MASK;
    mac_header_helper_functions_stub.fcf = fcf;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        return false;
    }

    //Test MAC15_4_PD_SAP_DATA_TX_CONFIRM
    phy_message.id = MAC15_4_PD_SAP_DATA_TX_CONFIRM;
    rf_ptr.macRfRadioTxActive = false;
    phy_message.message.mac15_4_pd_sap_confirm.cca_retry = 1;
    phy_message.message.mac15_4_pd_sap_confirm.tx_retry = 1;
    phy_message.message.mac15_4_pd_sap_confirm.status = PHY_LINK_TX_SUCCESS;

    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        return false;
    }

    rf_ptr.macRfRadioTxActive = true;
    rf_ptr.macTxProcessActive = true;
    rf_ptr.macTxRequestAck = true;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != 0 ){
        return false;
    }

    rf_ptr.macTxProcessActive = true;
    rf_ptr.macRfRadioTxActive = false;
    rf_ptr.macTxRequestAck = false;

    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        return false;
    }

    rf_ptr.macRfRadioTxActive = true;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != 0 ){
        return false;
    }

    rf_ptr.macTxProcessActive = true;
    phy_message.message.mac15_4_pd_sap_confirm.status = PHY_LINK_CCA_FAIL;
    rf_ptr.macRfRadioTxActive = false;
    rf_ptr.mac_cca_retry = 0;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != -1 ){
        return false;
    }

    rf_ptr.macTxProcessActive = true;
    rf_ptr.macRfRadioTxActive = true;
    rf_ptr.mac_cca_retry = 0;

    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != 0 ){
        return false;
    }

    if (rf_ptr.macRfRadioTxActive || !rf_ptr.macTxProcessActive) {
        return false;
    }

    rf_ptr.macRfRadioTxActive = true;
    rf_ptr.mac_cca_retry = MAC_CCA_MAX;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != 0 ){
        return false;
    }
    //Do 3 retry

    rf_ptr.mac_mlme_retry_max = 3;
    rf_ptr.mac_tx_retry = 0;
    phy_message.message.mac15_4_pd_sap_confirm.status = PHY_LINK_TX_FAIL;
    rf_ptr.macTxProcessActive = true;
    rf_ptr.macRfRadioTxActive = true;
    phy_message.message.mac15_4_pd_sap_confirm.tx_retry = 1;
    phy_message.message.mac15_4_pd_sap_confirm.cca_retry = 1;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != 0 ){
        return false;
    }
    if (rf_ptr.macRfRadioTxActive || !rf_ptr.macTxProcessActive) {
        return false;
    }
    //Simulate last retry
    rf_ptr.macRfRadioTxActive = true;
    rf_ptr.macRfRadioTxActive = true;
    rf_ptr.mac_tx_retry = 2;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != 0 ){
        return false;
    }
    if (rf_ptr.macRfRadioTxActive || rf_ptr.macTxProcessActive) {
        return false;
    }
    phy_message.message.mac15_4_pd_sap_confirm.status = PHY_LINK_TX_DONE;
    rf_ptr.macTxProcessActive = true;
    rf_ptr.macRfRadioTxActive = true;

    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != 0 ){
        return false;
    }
    if (rf_ptr.macRfRadioTxActive || rf_ptr.macTxProcessActive) {
        return false;
    }

    phy_message.message.mac15_4_pd_sap_confirm.status = PHY_LINK_TX_DONE_PENDING;
    rf_ptr.macTxProcessActive = true;
    rf_ptr.macRfRadioTxActive = true;

    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if( ret != 0 ){
        return false;
    }
    if (rf_ptr.macRfRadioTxActive || rf_ptr.macTxProcessActive) {
        return false;
    }

    // FHSS TESTS START
    mac_pre_build_frame_t temp_buf;
    rf_ptr.active_pd_data_request = &temp_buf;
    mac_mcps_sap_stub.int8_value = 0;
    nsdynmemlib_stub.returnCounter = 1;
    rf_ptr.fhss_api = ns_fhss_create(0, 0);
    fcf.DstAddrMode = MAC_ADDR_MODE_NONE;
    fcf.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    fcf.frametype = FC_BEACON_FRAME;
    fcf.securityEnabled = false;
    mac_header_helper_functions_stub.fcf = fcf;
    phy_message.id = MAC15_4_PD_SAP_DATA_IND;
    phy_message.message.generic_data_ind.data_ptr = buffer;
    // Test returning timestamp
    fhss_mac_interface_stub.uint32_value = 1234567890;
    ret = mac_pd_sap_data_cb(&rf_ptr, &phy_message);
    if (ret != 0) {
        return false;
    }
    if (mac_mcps_sap_stub.pre_parsed->timestamp != 1234567890){
        return false;
    }
    // Test TX confirm
    phy_message.id = MAC15_4_PD_SAP_DATA_TX_CONFIRM;
    rf_ptr.macTxProcessActive = true;
    rf_ptr.macRfRadioTxActive = true;
    if (mac_pd_sap_data_cb(&rf_ptr, &phy_message) != 0) {
        return false;
    }

    rf_ptr.mac_mlme_retry_max = 3;
    rf_ptr.mac_tx_retry = 0;
    phy_message.message.mac15_4_pd_sap_confirm.status = PHY_LINK_TX_FAIL;
    rf_ptr.macTxProcessActive = true;
    rf_ptr.macRfRadioTxActive = true;
    phy_message.message.mac15_4_pd_sap_confirm.tx_retry = 1;
    phy_message.message.mac15_4_pd_sap_confirm.cca_retry = 1;
    if (mac_pd_sap_data_cb(&rf_ptr, &phy_message) != 0) {
        return false;
    }

    // Test TX done with FHSS
    rf_ptr.fhss_api = ns_fhss_create(0, 0);
    phy_message.id = MAC15_4_PD_SAP_DATA_TX_CONFIRM;
    phy_message.message.mac15_4_pd_sap_confirm.status = PHY_LINK_TX_DONE;
    rf_ptr.macTxProcessActive = true;
    rf_ptr.macRfRadioTxActive = true;
    rf_ptr.mac_tx_result = MAC_TX_DONE;
    if (mac_pd_sap_data_cb(&rf_ptr, &phy_message) != 0) {
        return false;
    }

    free(frame_buf);
    return true;
}

bool test_mac_pd_sap_state_machine()
{
    //Testing MAC timer event handler
    protocol_interface_rf_mac_setup_s rf_ptr;
    arm_device_driver_list_s driver;
    mac_fcf_sequence_t fcf;
    arm_phy_sap_msg_t phy_message;
    phy_device_driver_s dev_driver;

    uint8_t tx_buffer[127];

    memset(&rf_ptr, 0, sizeof(protocol_interface_rf_mac_setup_s));
    memset(&driver, 0, sizeof(arm_device_driver_list_s));
    memset(&dev_driver, 0, sizeof(phy_device_driver_s));
    memset(&phy_message, 0, sizeof(arm_phy_sap_msg_t));
    memset(&fcf, 0, sizeof(mac_fcf_sequence_t));


    rf_ptr.dev_driver_tx_buffer.buf = tx_buffer;
    rf_ptr.dev_driver_tx_buffer.len = 100;
    dev_driver.extension = &test_rf_extension;
    dev_driver.tx = &test_rf_tx;
    driver.phy_driver = &dev_driver;
    rf_ptr.dev_driver = &driver;

    //Test CCA timer

    rf_ptr.macTxProcessActive = true;
    rf_ptr.macUpState = true;
    rf_ptr.mac_tx_result = MAC_TIMER_CCA;

    tx_return = -1;
    mac_pd_sap_state_machine(&rf_ptr);
    if (rf_ptr.macRfRadioTxActive ) {
        return false;
    }


    rf_ptr.mac_tx_result = MAC_TIMER_CCA;
    tx_return = 0;
    mac_pd_sap_state_machine(&rf_ptr);
    if (!rf_ptr.macRfRadioTxActive ) {
        return false;
    }

    //Test Virtual callback usecase
    rf_ptr.mac_tx_result = MAC_TIMER_CCA;
    rf_ptr.macTxProcessActive = true;
    dev_driver.arm_net_virtual_tx_cb = &test_rf_virtual_tx;
    tx_return = -1;
    rf_ptr.macRfRadioTxActive = false;
    mac_pd_sap_state_machine(&rf_ptr);
    if (rf_ptr.macRfRadioTxActive && rf_ptr.macTxProcessActive) {
        return false;
    }

    tx_return = 0;
    rf_ptr.mac_tx_result = MAC_TIMER_CCA;
    rf_ptr.macTxProcessActive = true;
    mac_pd_sap_state_machine(&rf_ptr);
    if (!rf_ptr.macRfRadioTxActive || !rf_ptr.macTxProcessActive) {
        return false;
    }

    dev_driver.arm_net_virtual_tx_cb = NULL;

    //End of CCA timer
    rf_ptr.macTxProcessActive = true;
    rf_ptr.macRfRadioTxActive = true;
    rf_ptr.macUpState = true;
    rf_ptr.mac_tx_result = MAC_TIMER_ACK;
    rf_ptr.mac_mlme_retry_max = 3;
    rf_ptr.mac_tx_retry = 0;
    mac_pd_sap_state_machine(&rf_ptr);

    if (!rf_ptr.macTxProcessActive) {
        return false;
    }

    rf_ptr.macRfRadioTxActive = true;
    rf_ptr.mac_tx_retry = 2;
    mac_pd_sap_state_machine(&rf_ptr);
    if (!rf_ptr.macTxProcessActive || rf_ptr.mac_tx_retry != 3) {
        return false;
    }
    rf_ptr.macRfRadioTxActive = true;
    mac_pd_sap_state_machine(&rf_ptr);
    if (rf_ptr.macTxProcessActive ) {
        return false;
    }

    //Test Timeout
    rf_ptr.mac_tx_result = MAC_TX_TIMEOUT;
    rf_ptr.macTxProcessActive = true;
    rf_ptr.macRfRadioTxActive = true;
    rf_ptr.macUpState = true;
    mac_pd_sap_state_machine(&rf_ptr);
    if (rf_ptr.macTxProcessActive || rf_ptr.macRfRadioTxActive) {
        return false;
    }

    // FHSS TESTS START
    mac_pre_build_frame_t temp_buf;
    rf_ptr.fhss_api = ns_fhss_create(0, 0);
    rf_ptr.mac_tx_result = MAC_TIMER_CCA;
    rf_ptr.macTxProcessActive = true;
    rf_ptr.active_pd_data_request = &temp_buf;
    mac_pd_sap_state_machine(&rf_ptr);
    if (rf_ptr.macRfRadioTxActive == false) {
        return false;
    }

    // Test TX handle fail
    rf_ptr.mac_tx_result = MAC_TIMER_CCA;
    fhss_mac_interface_stub.handle_value = -1;
    mac_pd_sap_state_machine(&rf_ptr);
    if (!rf_ptr.macRfRadioTxActive ) {
        return false;
    }
    // Test synch frame
    rf_ptr.mac_tx_result = MAC_TIMER_CCA;
    fhss_mac_interface_stub.handle_value = 0;
    rf_ptr.active_pd_data_request->fcf_dsn.frametype = FC_BEACON_FRAME;
    mac_pd_sap_state_machine(&rf_ptr);
    if (!rf_ptr.macRfRadioTxActive ) {
        return false;
    }
    // Test without active request
    rf_ptr.mac_tx_result = MAC_TIMER_CCA;
    rf_ptr.active_pd_data_request = NULL;
    mac_pd_sap_state_machine(&rf_ptr);

    return true;
}
