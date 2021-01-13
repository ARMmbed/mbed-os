/* mbed Microcontroller Library
 * Copyright (c) 2021 ARM Limited
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

#include "ble_trace_helpers.h"
#define TRACE_GROUP "BLE "

namespace ble {
#if MBED_CONF_MBED_TRACE_ENABLE
const char* dm_callback_event_to_string(uint8_t event) {
    const char* ret = "INVALID";
    switch(event)
    {
        case DM_RESET_CMPL_IND: ret = "DM_RESET_CMPL_IND"; break;
        case DM_ADV_START_IND: ret = "DM_ADV_START_IND"; break;
        case DM_ADV_STOP_IND: ret = "DM_ADV_STOP_IND"; break;
        case DM_ADV_NEW_ADDR_IND: ret = "DM_ADV_NEW_ADDR_IND"; break;
        case DM_SCAN_START_IND: ret = "DM_SCAN_START_IND"; break;
        case DM_SCAN_STOP_IND: ret = "DM_SCAN_STOP_IND"; break;
        case DM_SCAN_REPORT_IND: ret = "DM_SCAN_REPORT_IND"; break;
        case DM_CONN_OPEN_IND: ret = "DM_CONN_OPEN_IND"; break;
        case DM_CONN_CLOSE_IND: ret = "DM_CONN_CLOSE_IND"; break;
        case DM_CONN_UPDATE_IND: ret = "DM_CONN_UPDATE_IND"; break;
        case DM_SEC_PAIR_CMPL_IND: ret = "DM_SEC_PAIR_CMPL_IND"; break;
        case DM_SEC_PAIR_FAIL_IND: ret = "DM_SEC_PAIR_FAIL_IND"; break;
        case DM_SEC_ENCRYPT_IND: ret = "DM_SEC_ENCRYPT_IND"; break;
        case DM_SEC_ENCRYPT_FAIL_IND: ret = "DM_SEC_ENCRYPT_FAIL_IND"; break;
        case DM_SEC_AUTH_REQ_IND: ret = "DM_SEC_AUTH_REQ_IND"; break;
        case DM_SEC_KEY_IND: ret = "DM_SEC_KEY_IND"; break;
        case DM_SEC_LTK_REQ_IND: ret = "DM_SEC_LTK_REQ_IND"; break;
        case DM_SEC_PAIR_IND: ret = "DM_SEC_PAIR_IND"; break;
        case DM_SEC_SLAVE_REQ_IND: ret = "DM_SEC_SLAVE_REQ_IND"; break;
        case DM_SEC_CALC_OOB_IND: ret = "DM_SEC_CALC_OOB_IND"; break;
        case DM_SEC_ECC_KEY_IND: ret = "DM_SEC_ECC_KEY_IND"; break;
        case DM_SEC_COMPARE_IND: ret = "DM_SEC_COMPARE_IND"; break;
        case DM_SEC_KEYPRESS_IND: ret = "DM_SEC_KEYPRESS_IND"; break;
        case DM_PRIV_RESOLVED_ADDR_IND: ret = "DM_PRIV_RESOLVED_ADDR_IND"; break;
        case DM_PRIV_GENERATE_ADDR_IND: ret = "DM_PRIV_GENERATE_ADDR_IND"; break;
        case DM_CONN_READ_RSSI_IND: ret = "DM_CONN_READ_RSSI_IND"; break;
        case DM_PRIV_ADD_DEV_TO_RES_LIST_IND: ret = "DM_PRIV_ADD_DEV_TO_RES_LIST_IND"; break;
        case DM_PRIV_REM_DEV_FROM_RES_LIST_IND: ret = "DM_PRIV_REM_DEV_FROM_RES_LIST_IND"; break;
        case DM_PRIV_CLEAR_RES_LIST_IND: ret = "DM_PRIV_CLEAR_RES_LIST_IND"; break;
        case DM_PRIV_READ_PEER_RES_ADDR_IND: ret = "DM_PRIV_READ_PEER_RES_ADDR_IND"; break;
        case DM_PRIV_READ_LOCAL_RES_ADDR_IND: ret = "DM_PRIV_READ_LOCAL_RES_ADDR_IND"; break;
        case DM_PRIV_SET_ADDR_RES_ENABLE_IND: ret = "DM_PRIV_SET_ADDR_RES_ENABLE_IND"; break;
        case DM_REM_CONN_PARAM_REQ_IND: ret = "DM_REM_CONN_PARAM_REQ_IND"; break;
        case DM_CONN_DATA_LEN_CHANGE_IND: ret = "DM_CONN_DATA_LEN_CHANGE_IND"; break;
        case DM_CONN_WRITE_AUTH_TO_IND: ret = "DM_CONN_WRITE_AUTH_TO_IND"; break;
        case DM_CONN_AUTH_TO_EXPIRED_IND: ret = "DM_CONN_AUTH_TO_EXPIRED_IND"; break;
        case DM_PHY_READ_IND: ret = "DM_PHY_READ_IND"; break;
        case DM_PHY_SET_DEF_IND: ret = "DM_PHY_SET_DEF_IND"; break;
        case DM_PHY_UPDATE_IND: ret = "DM_PHY_UPDATE_IND"; break;
        case DM_ADV_SET_START_IND: ret = "DM_ADV_SET_START_IND"; break;
        case DM_ADV_SET_STOP_IND: ret = "DM_ADV_SET_STOP_IND"; break;
        case DM_SCAN_REQ_RCVD_IND: ret = "DM_SCAN_REQ_RCVD_IND"; break;
        case DM_EXT_SCAN_START_IND: ret = "DM_EXT_SCAN_START_IND"; break;
        case DM_EXT_SCAN_STOP_IND: ret = "DM_EXT_SCAN_STOP_IND"; break;
        case DM_EXT_SCAN_REPORT_IND: ret = "DM_EXT_SCAN_REPORT_IND"; break;
        case DM_PER_ADV_SET_START_IND: ret = "DM_PER_ADV_SET_START_IND"; break;
        case DM_PER_ADV_SET_STOP_IND: ret = "DM_PER_ADV_SET_STOP_IND"; break;
        case DM_PER_ADV_SYNC_EST_IND: ret = "DM_PER_ADV_SYNC_EST_IND"; break;
        case DM_PER_ADV_SYNC_EST_FAIL_IND: ret = "DM_PER_ADV_SYNC_EST_FAIL_IND"; break;
        case DM_PER_ADV_SYNC_LOST_IND: ret = "DM_PER_ADV_SYNC_LOST_IND"; break;
        case DM_PER_ADV_SYNC_TRSF_EST_IND: ret = "DM_PER_ADV_SYNC_TRSF_EST_IND"; break;
        case DM_PER_ADV_SYNC_TRSF_EST_FAIL_IND: ret = "DM_PER_ADV_SYNC_TRSF_EST_FAIL_IND"; break;
        case DM_PER_ADV_SYNC_TRSF_IND: ret = "DM_PER_ADV_SYNC_TRSF_IND"; break;
        case DM_PER_ADV_SET_INFO_TRSF_IND: ret = "DM_PER_ADV_SET_INFO_TRSF_IND"; break;
        case DM_PER_ADV_REPORT_IND: ret = "DM_PER_ADV_REPORT_IND"; break;
        case DM_REMOTE_FEATURES_IND: ret = "DM_REMOTE_FEATURES_IND"; break;
        case DM_READ_REMOTE_VER_INFO_IND: ret = "DM_READ_REMOTE_VER_INFO_IND"; break;
        case DM_CONN_IQ_REPORT_IND: ret = "DM_CONN_IQ_REPORT_IND"; break;
        case DM_CTE_REQ_FAIL_IND: ret = "DM_CTE_REQ_FAIL_IND"; break;
        case DM_CONN_CTE_RX_SAMPLE_START_IND: ret = "DM_CONN_CTE_RX_SAMPLE_START_IND"; break;
        case DM_CONN_CTE_RX_SAMPLE_STOP_IND: ret = "DM_CONN_CTE_RX_SAMPLE_STOP_IND"; break;
        case DM_CONN_CTE_TX_CFG_IND: ret = "DM_CONN_CTE_TX_CFG_IND"; break;
        case DM_CONN_CTE_REQ_START_IND: ret = "DM_CONN_CTE_REQ_START_IND"; break;
        case DM_CONN_CTE_REQ_STOP_IND: ret = "DM_CONN_CTE_REQ_STOP_IND"; break;
        case DM_CONN_CTE_RSP_START_IND: ret = "DM_CONN_CTE_RSP_START_IND"; break;
        case DM_CONN_CTE_RSP_STOP_IND: ret = "DM_CONN_CTE_RSP_STOP_IND"; break;
        case DM_READ_ANTENNA_INFO_IND: ret = "DM_READ_ANTENNA_INFO_IND"; break;
        case DM_CIS_CIG_CONFIG_IND: ret = "DM_CIS_CIG_CONFIG_IND"; break;
        case DM_CIS_CIG_REMOVE_IND: ret = "DM_CIS_CIG_REMOVE_IND"; break;
        case DM_CIS_REQ_IND: ret = "DM_CIS_REQ_IND"; break;
        case DM_CIS_OPEN_IND: ret = "DM_CIS_OPEN_IND"; break;
        case DM_CIS_CLOSE_IND: ret = "DM_CIS_CLOSE_IND"; break;
        case DM_REQ_PEER_SCA_IND: ret = "DM_REQ_PEER_SCA_IND"; break;
        case DM_ISO_DATA_PATH_SETUP_IND: ret = "DM_ISO_DATA_PATH_SETUP_IND"; break;
        case DM_ISO_DATA_PATH_REMOVE_IND: ret = "DM_ISO_DATA_PATH_REMOVE_IND"; break;
        case DM_DATA_PATH_CONFIG_IND: ret = "DM_DATA_PATH_CONFIG_IND"; break;
        case DM_READ_LOCAL_SUP_CODECS_IND: ret = "DM_READ_LOCAL_SUP_CODECS_IND"; break;
        case DM_READ_LOCAL_SUP_CODEC_CAP_IND: ret = "DM_READ_LOCAL_SUP_CODEC_CAP_IND"; break;
        case DM_READ_LOCAL_SUP_CTR_DLY_IND: ret = "DM_READ_LOCAL_SUP_CTR_DLY_IND"; break;
        case DM_BIG_START_IND: ret = "DM_BIG_START_IND"; break;
        case DM_BIG_STOP_IND: ret = "DM_BIG_STOP_IND"; break;
        case DM_BIG_SYNC_EST_IND: ret = "DM_BIG_SYNC_EST_IND"; break;
        case DM_BIG_SYNC_EST_FAIL_IND: ret = "DM_BIG_SYNC_EST_FAIL_IND"; break;
        case DM_BIG_SYNC_LOST_IND: ret = "DM_BIG_SYNC_LOST_IND"; break;
        case DM_BIG_SYNC_STOP_IND: ret = "DM_BIG_SYNC_STOP_IND"; break;
        case DM_BIG_INFO_ADV_REPORT_IND: ret = "DM_BIG_INFO_ADV_REPORT_IND"; break;
        case DM_L2C_CMD_REJ_IND: ret = "DM_L2C_CMD_REJ_IND"; break;
        case DM_ERROR_IND: ret = "DM_ERROR_IND"; break;
        case DM_HW_ERROR_IND: ret = "DM_HW_ERROR_IND"; break;
        case DM_VENDOR_SPEC_IND: ret = "DM_VENDOR_SPEC_IND"; break;
#if MBED_CONF_CORDIO_ROUTE_UNHANDLED_COMMAND_COMPLETE_EVENTS
        case DM_UNHANDLED_CMD_CMPL_EVT_IND: ret = "DM_UNHANDLED_CMD_CMPL_EVT_IND"; break;
#endif
    }
    return ret;
}

void trace_le_supported_features(uint64_t feat) {
    if (feat & HCI_LE_SUP_FEAT_ENCRYPTION) tr_info("ENCRYPTION");
    if (feat & HCI_LE_SUP_FEAT_CONN_PARAM_REQ_PROC) tr_info("CONN_PARAM_REQ_PROC");
    if (feat & HCI_LE_SUP_FEAT_EXT_REJECT_IND) tr_info("EXT_REJECT_IND");
    if (feat & HCI_LE_SUP_FEAT_SLV_INIT_FEAT_EXCH) tr_info("SLV_INIT_FEAT_EXCH");
    if (feat & HCI_LE_SUP_FEAT_LE_PING) tr_info("LE_PING");
    if (feat & HCI_LE_SUP_FEAT_DATA_LEN_EXT) tr_info("DATA_LEN_EXT");
    if (feat & HCI_LE_SUP_FEAT_PRIVACY) tr_info("PRIVACY");
    if (feat & HCI_LE_SUP_FEAT_EXT_SCAN_FILT_POLICY) tr_info("EXT_SCAN_FILT_POLICY");
    if (feat & HCI_LE_SUP_FEAT_LE_2M_PHY) tr_info("LE_2M_PHY");
    if (feat & HCI_LE_SUP_FEAT_STABLE_MOD_IDX_TRANSMITTER) tr_info("STABLE_MOD_IDX_TRANSMITTER");
    if (feat & HCI_LE_SUP_FEAT_STABLE_MOD_IDX_RECEIVER) tr_info("STABLE_MOD_IDX_RECEIVER");
    if (feat & HCI_LE_SUP_FEAT_LE_CODED_PHY) tr_info("LE_CODED_PHY");
    if (feat & HCI_LE_SUP_FEAT_LE_EXT_ADV) tr_info("LE_EXT_ADV");
    if (feat & HCI_LE_SUP_FEAT_LE_PER_ADV) tr_info("LE_PER_ADV");
    if (feat & HCI_LE_SUP_FEAT_CH_SEL_2) tr_info("CH_SEL_2");
    if (feat & HCI_LE_SUP_FEAT_LE_POWER_CLASS_1) tr_info("LE_POWER_CLASS_1");
    if (feat & HCI_LE_SUP_FEAT_MIN_NUN_USED_CHAN) tr_info("MIN_NUN_USED_CHAN");
    if (feat & HCI_LE_SUP_FEAT_CONN_CTE_REQ) tr_info("CONN_CTE_REQ");
    if (feat & HCI_LE_SUP_FEAT_CONN_CTE_RSP) tr_info("CONN_CTE_RSP");
    if (feat & HCI_LE_SUP_FEAT_CONNLESS_CTE_TRANS) tr_info("CONNLESS_CTE_TRANS");
    if (feat & HCI_LE_SUP_FEAT_CONNLESS_CTE_RECV) tr_info("CONNLESS_CTE_RECV");
    if (feat & HCI_LE_SUP_FEAT_ANTENNA_SWITCH_AOD) tr_info("ANTENNA_SWITCH_AOD");
    if (feat & HCI_LE_SUP_FEAT_ANTENNA_SWITCH_AOA) tr_info("ANTENNA_SWITCH_AOA");
    if (feat & HCI_LE_SUP_FEAT_RECV_CTE) tr_info("RECV_CTE");
    if (feat & HCI_LE_SUP_FEAT_PAST_SENDER) tr_info("PAST_SENDER");
    if (feat & HCI_LE_SUP_FEAT_PAST_RECIPIENT) tr_info("PAST_RECIPIENT");
    if (feat & HCI_LE_SUP_FEAT_SCA_UPDATE) tr_info("SCA_UPDATE");
    if (feat & HCI_LE_SUP_FEAT_REMOTE_PUB_KEY_VALIDATION) tr_info("REMOTE_PUB_KEY_VALIDATION");
    if (feat & HCI_LE_SUP_FEAT_CIS_MASTER) tr_info("CIS_MASTER");
    if (feat & HCI_LE_SUP_FEAT_CIS_SLAVE) tr_info("CIS_SLAVE");
    if (feat & HCI_LE_SUP_FEAT_ISO_BROADCASTER) tr_info("ISO_BROADCASTER");
    if (feat & HCI_LE_SUP_FEAT_ISO_SYNC_RECEIVER) tr_info("ISO_SYNC_RECEIVER");
    if (feat & HCI_LE_SUP_FEAT_ISO_HOST_SUPPORT) tr_info("ISO_HOST_SUPPORT");
    if (feat & HCI_LE_SUP_FEAT_POWER_CONTROL_REQUEST) tr_info("POWER_CONTROL_REQUEST");
    if (feat & HCI_LE_SUP_FEAT_POWER_CHANGE_IND) tr_info("POWER_CHANGE_IND");
    if (feat & HCI_LE_SUP_FEAT_PATH_LOSS_MONITOR) tr_info("PATH_LOSS_MONITOR");
}
#endif //MBED_CONF_MBED_TRACE_ENABLE

} // namespace ble
