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
#if MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
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
#else // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    static char ret[3] = "00";
    sprintf(ret, "%02x", (int)event);
#endif // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    return ret;
}

void trace_le_supported_features(uint64_t feat)
{
#if MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
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
#else // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    tr_info("%s", tr_array((uint8_t*)&feat, 4));
#endif // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
}

const char* hci_opcode_to_string(uint16_t opcode)
{
#if MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    const char* ret = "UNKNOWN OPCODE";
    switch (opcode) {
        case HCI_OPCODE_NOP: ret = "HCI_OPCODE_NOP"; break;

        case HCI_OPCODE_DISCONNECT: ret = "HCI_OPCODE_DISCONNECT"; break;
        case HCI_OPCODE_READ_REMOTE_VER_INFO: ret = "HCI_OPCODE_READ_REMOTE_VER_INFO"; break;

        case HCI_OPCODE_SET_EVENT_MASK: ret = "HCI_OPCODE_SET_EVENT_MASK"; break;
        case HCI_OPCODE_RESET: ret = "HCI_OPCODE_RESET"; break;
        case HCI_OPCODE_READ_TX_PWR_LVL: ret = "HCI_OPCODE_READ_TX_PWR_LVL"; break;
        case HCI_OPCODE_SET_EVENT_MASK_PAGE2: ret = "HCI_OPCODE_SET_EVENT_MASK_PAGE2"; break;
        case HCI_OPCODE_READ_AUTH_PAYLOAD_TO: ret = "HCI_OPCODE_READ_AUTH_PAYLOAD_TO"; break;
        case HCI_OPCODE_WRITE_AUTH_PAYLOAD_TO: ret = "HCI_OPCODE_WRITE_AUTH_PAYLOAD_TO"; break;
        case HCI_OPCODE_CONFIG_DATA_PATH: ret = "HCI_OPCODE_CONFIG_DATA_PATH"; break;

        case HCI_OPCODE_READ_LOCAL_VER_INFO: ret = "HCI_OPCODE_READ_LOCAL_VER_INFO"; break;
        case HCI_OPCODE_READ_LOCAL_SUP_CMDS: ret = "HCI_OPCODE_READ_LOCAL_SUP_CMDS"; break;
        case HCI_OPCODE_READ_LOCAL_SUP_FEAT: ret = "HCI_OPCODE_READ_LOCAL_SUP_FEAT"; break;
        case HCI_OPCODE_READ_BUF_SIZE: ret = "HCI_OPCODE_READ_BUF_SIZE"; break;
        case HCI_OPCODE_READ_BD_ADDR: ret = "HCI_OPCODE_READ_BD_ADDR"; break;
        case HCI_OPCODE_READ_LOCAL_SUP_CODECS: ret = "HCI_OPCODE_READ_LOCAL_SUP_CODECS"; break;
        case HCI_OPCODE_READ_LOCAL_SUP_CODEC_CAP: ret = "HCI_OPCODE_READ_LOCAL_SUP_CODEC_CAP"; break;
        case HCI_OPCODE_READ_LOCAL_SUP_CONTROLLER_DLY: ret = "HCI_OPCODE_READ_LOCAL_SUP_CONTROLLER_DLY"; break;

        case HCI_OPCODE_READ_RSSI: ret = "HCI_OPCODE_READ_RSSI"; break;

        case HCI_OPCODE_LE_SET_EVENT_MASK: ret = "HCI_OPCODE_LE_SET_EVENT_MASK"; break;
        case HCI_OPCODE_LE_READ_BUF_SIZE: ret = "HCI_OPCODE_LE_READ_BUF_SIZE"; break;
        case HCI_OPCODE_LE_READ_LOCAL_SUP_FEAT: ret = "HCI_OPCODE_LE_READ_LOCAL_SUP_FEAT"; break;
        case HCI_OPCODE_LE_SET_RAND_ADDR: ret = "HCI_OPCODE_LE_SET_RAND_ADDR"; break;
        case HCI_OPCODE_LE_SET_ADV_PARAM: ret = "HCI_OPCODE_LE_SET_ADV_PARAM"; break;
        case HCI_OPCODE_LE_READ_ADV_TX_POWER: ret = "HCI_OPCODE_LE_READ_ADV_TX_POWER"; break;
        case HCI_OPCODE_LE_SET_ADV_DATA: ret = "HCI_OPCODE_LE_SET_ADV_DATA"; break;
        case HCI_OPCODE_LE_SET_SCAN_RESP_DATA: ret = "HCI_OPCODE_LE_SET_SCAN_RESP_DATA"; break;
        case HCI_OPCODE_LE_SET_ADV_ENABLE: ret = "HCI_OPCODE_LE_SET_ADV_ENABLE"; break;
        case HCI_OPCODE_LE_SET_SCAN_PARAM: ret = "HCI_OPCODE_LE_SET_SCAN_PARAM"; break;
        case HCI_OPCODE_LE_SET_SCAN_ENABLE: ret = "HCI_OPCODE_LE_SET_SCAN_ENABLE"; break;
        case HCI_OPCODE_LE_CREATE_CONN: ret = "HCI_OPCODE_LE_CREATE_CONN"; break;
        case HCI_OPCODE_LE_CREATE_CONN_CANCEL: ret = "HCI_OPCODE_LE_CREATE_CONN_CANCEL"; break;
        case HCI_OPCODE_LE_READ_WHITE_LIST_SIZE: ret = "HCI_OPCODE_LE_READ_WHITE_LIST_SIZE"; break;
        case HCI_OPCODE_LE_CLEAR_WHITE_LIST: ret = "HCI_OPCODE_LE_CLEAR_WHITE_LIST"; break;
        case HCI_OPCODE_LE_ADD_DEV_WHITE_LIST: ret = "HCI_OPCODE_LE_ADD_DEV_WHITE_LIST"; break;
        case HCI_OPCODE_LE_REMOVE_DEV_WHITE_LIST: ret = "HCI_OPCODE_LE_REMOVE_DEV_WHITE_LIST"; break;
        case HCI_OPCODE_LE_CONN_UPDATE: ret = "HCI_OPCODE_LE_CONN_UPDATE"; break;
        case HCI_OPCODE_LE_SET_HOST_CHAN_CLASS: ret = "HCI_OPCODE_LE_SET_HOST_CHAN_CLASS"; break;
        case HCI_OPCODE_LE_READ_CHAN_MAP: ret = "HCI_OPCODE_LE_READ_CHAN_MAP"; break;
        case HCI_OPCODE_LE_READ_REMOTE_FEAT: ret = "HCI_OPCODE_LE_READ_REMOTE_FEAT"; break;
        case HCI_OPCODE_LE_ENCRYPT: ret = "HCI_OPCODE_LE_ENCRYPT"; break;
        case HCI_OPCODE_LE_RAND: ret = "HCI_OPCODE_LE_RAND"; break;
        case HCI_OPCODE_LE_START_ENCRYPTION: ret = "HCI_OPCODE_LE_START_ENCRYPTION"; break;
        case HCI_OPCODE_LE_LTK_REQ_REPL: ret = "HCI_OPCODE_LE_LTK_REQ_REPL"; break;
        case HCI_OPCODE_LE_LTK_REQ_NEG_REPL: ret = "HCI_OPCODE_LE_LTK_REQ_NEG_REPL"; break;
        case HCI_OPCODE_LE_READ_SUP_STATES: ret = "HCI_OPCODE_LE_READ_SUP_STATES"; break;
        case HCI_OPCODE_LE_RECEIVER_TEST: ret = "HCI_OPCODE_LE_RECEIVER_TEST"; break;
        case HCI_OPCODE_LE_TRANSMITTER_TEST: ret = "HCI_OPCODE_LE_TRANSMITTER_TEST"; break;
        case HCI_OPCODE_LE_TEST_END: ret = "HCI_OPCODE_LE_TEST_END"; break;
        /* Version 4.1 */
        case HCI_OPCODE_LE_REM_CONN_PARAM_REP: ret = "HCI_OPCODE_LE_REM_CONN_PARAM_REP"; break;
        case HCI_OPCODE_LE_REM_CONN_PARAM_NEG_REP: ret = "HCI_OPCODE_LE_REM_CONN_PARAM_NEG_REP"; break;
        /* Version 4.2 */
        case HCI_OPCODE_LE_SET_DATA_LEN: ret = "HCI_OPCODE_LE_SET_DATA_LEN"; break;
        case HCI_OPCODE_LE_READ_DEF_DATA_LEN: ret = "HCI_OPCODE_LE_READ_DEF_DATA_LEN"; break;
        case HCI_OPCODE_LE_WRITE_DEF_DATA_LEN: ret = "HCI_OPCODE_LE_WRITE_DEF_DATA_LEN"; break;
        case HCI_OPCODE_LE_READ_LOCAL_P256_PUB_KEY: ret = "HCI_OPCODE_LE_READ_LOCAL_P256_PUB_KEY"; break;
        case HCI_OPCODE_LE_GENERATE_DHKEY: ret = "HCI_OPCODE_LE_GENERATE_DHKEY"; break;
        case HCI_OPCODE_LE_ADD_DEV_RES_LIST: ret = "HCI_OPCODE_LE_ADD_DEV_RES_LIST"; break;
        case HCI_OPCODE_LE_REMOVE_DEV_RES_LIST: ret = "HCI_OPCODE_LE_REMOVE_DEV_RES_LIST"; break;
        case HCI_OPCODE_LE_CLEAR_RES_LIST: ret = "HCI_OPCODE_LE_CLEAR_RES_LIST"; break;
        case HCI_OPCODE_LE_READ_RES_LIST_SIZE: ret = "HCI_OPCODE_LE_READ_RES_LIST_SIZE"; break;
        case HCI_OPCODE_LE_READ_PEER_RES_ADDR: ret = "HCI_OPCODE_LE_READ_PEER_RES_ADDR"; break;
        case HCI_OPCODE_LE_READ_LOCAL_RES_ADDR: ret = "HCI_OPCODE_LE_READ_LOCAL_RES_ADDR"; break;
        case HCI_OPCODE_LE_SET_ADDR_RES_ENABLE: ret = "HCI_OPCODE_LE_SET_ADDR_RES_ENABLE"; break;
        case HCI_OPCODE_LE_SET_RES_PRIV_ADDR_TO: ret = "HCI_OPCODE_LE_SET_RES_PRIV_ADDR_TO"; break;
        case HCI_OPCODE_LE_READ_MAX_DATA_LEN: ret = "HCI_OPCODE_LE_READ_MAX_DATA_LEN"; break;
        /* Version 5.0 */
        case HCI_OPCODE_LE_READ_PHY: ret = "HCI_OPCODE_LE_READ_PHY"; break;
        case HCI_OPCODE_LE_SET_DEF_PHY: ret = "HCI_OPCODE_LE_SET_DEF_PHY"; break;
        case HCI_OPCODE_LE_SET_PHY: ret = "HCI_OPCODE_LE_SET_PHY"; break;
        case HCI_OPCODE_LE_ENHANCED_RECEIVER_TEST: ret = "HCI_OPCODE_LE_ENHANCED_RECEIVER_TEST"; break;
        case HCI_OPCODE_LE_ENHANCED_TRANSMITTER_TEST: ret = "HCI_OPCODE_LE_ENHANCED_TRANSMITTER_TEST"; break;
        case HCI_OPCODE_LE_SET_ADV_SET_RAND_ADDR: ret = "HCI_OPCODE_LE_SET_ADV_SET_RAND_ADDR"; break;
        case HCI_OPCODE_LE_SET_EXT_ADV_PARAM: ret = "HCI_OPCODE_LE_SET_EXT_ADV_PARAM"; break;
        case HCI_OPCODE_LE_SET_EXT_ADV_DATA: ret = "HCI_OPCODE_LE_SET_EXT_ADV_DATA"; break;
        case HCI_OPCODE_LE_SET_EXT_SCAN_RESP_DATA: ret = "HCI_OPCODE_LE_SET_EXT_SCAN_RESP_DATA"; break;
        case HCI_OPCODE_LE_SET_EXT_ADV_ENABLE: ret = "HCI_OPCODE_LE_SET_EXT_ADV_ENABLE"; break;
        case HCI_OPCODE_LE_READ_MAX_ADV_DATA_LEN: ret = "HCI_OPCODE_LE_READ_MAX_ADV_DATA_LEN"; break;
        case HCI_OPCODE_LE_READ_NUM_SUP_ADV_SETS: ret = "HCI_OPCODE_LE_READ_NUM_SUP_ADV_SETS"; break;
        case HCI_OPCODE_LE_REMOVE_ADV_SET: ret = "HCI_OPCODE_LE_REMOVE_ADV_SET"; break;
        case HCI_OPCODE_LE_CLEAR_ADV_SETS: ret = "HCI_OPCODE_LE_CLEAR_ADV_SETS"; break;
        case HCI_OPCODE_LE_SET_PER_ADV_PARAM: ret = "HCI_OPCODE_LE_SET_PER_ADV_PARAM"; break;
        case HCI_OPCODE_LE_SET_PER_ADV_DATA: ret = "HCI_OPCODE_LE_SET_PER_ADV_DATA"; break;
        case HCI_OPCODE_LE_SET_PER_ADV_ENABLE: ret = "HCI_OPCODE_LE_SET_PER_ADV_ENABLE"; break;
        case HCI_OPCODE_LE_SET_EXT_SCAN_PARAM: ret = "HCI_OPCODE_LE_SET_EXT_SCAN_PARAM"; break;
        case HCI_OPCODE_LE_SET_EXT_SCAN_ENABLE: ret = "HCI_OPCODE_LE_SET_EXT_SCAN_ENABLE"; break;
        case HCI_OPCODE_LE_EXT_CREATE_CONN: ret = "HCI_OPCODE_LE_EXT_CREATE_CONN"; break;
        case HCI_OPCODE_LE_PER_ADV_CREATE_SYNC: ret = "HCI_OPCODE_LE_PER_ADV_CREATE_SYNC"; break;
        case HCI_OPCODE_LE_PER_ADV_CREATE_SYNC_CANCEL: ret = "HCI_OPCODE_LE_PER_ADV_CREATE_SYNC_CANCEL"; break;
        case HCI_OPCODE_LE_PER_ADV_TERMINATE_SYNC: ret = "HCI_OPCODE_LE_PER_ADV_TERMINATE_SYNC"; break;
        case HCI_OPCODE_LE_ADD_DEV_PER_ADV_LIST: ret = "HCI_OPCODE_LE_ADD_DEV_PER_ADV_LIST"; break;
        case HCI_OPCODE_LE_REMOVE_DEV_PER_ADV_LIST: ret = "HCI_OPCODE_LE_REMOVE_DEV_PER_ADV_LIST"; break;
        case HCI_OPCODE_LE_CLEAR_PER_ADV_LIST: ret = "HCI_OPCODE_LE_CLEAR_PER_ADV_LIST"; break;
        case HCI_OPCODE_LE_READ_PER_ADV_LIST_SIZE: ret = "HCI_OPCODE_LE_READ_PER_ADV_LIST_SIZE"; break;
        case HCI_OPCODE_LE_READ_TX_POWER: ret = "HCI_OPCODE_LE_READ_TX_POWER"; break;
        case HCI_OPCODE_LE_WRITE_RF_PATH_COMP: ret = "HCI_OPCODE_LE_WRITE_RF_PATH_COMP"; break;
        case HCI_OPCODE_LE_READ_RF_PATH_COMP: ret = "HCI_OPCODE_LE_READ_RF_PATH_COMP"; break;
        case HCI_OPCODE_LE_SET_PRIVACY_MODE: ret = "HCI_OPCODE_LE_SET_PRIVACY_MODE"; break;
        /* Version 5.1 */
        case HCI_OPCODE_LE_RECEIVER_TEST_V3: ret = "HCI_OPCODE_LE_RECEIVER_TEST_V3"; break;
        case HCI_OPCODE_LE_TRANSMITTER_TEST_V3: ret = "HCI_OPCODE_LE_TRANSMITTER_TEST_V3"; break;
        case HCI_OPCODE_LE_SET_CONNLESS_CTE_TX_PARAMS: ret = "HCI_OPCODE_LE_SET_CONNLESS_CTE_TX_PARAMS"; break;
        case HCI_OPCODE_LE_SET_CONNLESS_CTE_TX_ENABLE: ret = "HCI_OPCODE_LE_SET_CONNLESS_CTE_TX_ENABLE"; break;
        case HCI_OPCODE_LE_SET_CONNLESS_IQ_SAMP_ENABLE: ret = "HCI_OPCODE_LE_SET_CONNLESS_IQ_SAMP_ENABLE"; break;
        case HCI_OPCODE_LE_SET_CONN_CTE_RX_PARAMS: ret = "HCI_OPCODE_LE_SET_CONN_CTE_RX_PARAMS"; break;
        case HCI_OPCODE_LE_SET_CONN_CTE_TX_PARAMS: ret = "HCI_OPCODE_LE_SET_CONN_CTE_TX_PARAMS"; break;
        case HCI_OPCODE_LE_CONN_CTE_REQ_ENABLE: ret = "HCI_OPCODE_LE_CONN_CTE_REQ_ENABLE"; break;
        case HCI_OPCODE_LE_CONN_CTE_RSP_ENABLE: ret = "HCI_OPCODE_LE_CONN_CTE_RSP_ENABLE"; break;
        case HCI_OPCODE_LE_READ_ANTENNA_INFO: ret = "HCI_OPCODE_LE_READ_ANTENNA_INFO"; break;
        case HCI_OPCODE_LE_SET_PER_ADV_RCV_ENABLE: ret = "HCI_OPCODE_LE_SET_PER_ADV_RCV_ENABLE"; break;
        case HCI_OPCODE_LE_PER_ADV_SYNC_TRANSFER: ret = "HCI_OPCODE_LE_PER_ADV_SYNC_TRANSFER"; break;
        case HCI_OPCODE_LE_PER_ADV_SET_INFO_TRANSFER: ret = "HCI_OPCODE_LE_PER_ADV_SET_INFO_TRANSFER"; break;
        case HCI_OPCODE_LE_SET_PAST_PARAM: ret = "HCI_OPCODE_LE_SET_PAST_PARAM"; break;
        case HCI_OPCODE_LE_SET_DEFAULT_PAST_PARAM: ret = "HCI_OPCODE_LE_SET_DEFAULT_PAST_PARAM"; break;
        case HCI_OPCODE_LE_GENERATE_DHKEY_V2: ret = "HCI_OPCODE_LE_GENERATE_DHKEY_V2"; break;
        case HCI_OPCODE_LE_MODIFY_SLEEP_CLK_ACC: ret = "HCI_OPCODE_LE_MODIFY_SLEEP_CLK_ACC"; break;
        /* Version 5.2 */
        case HCI_OPCODE_LE_READ_BUF_SIZE_V2: ret = "HCI_OPCODE_LE_READ_BUF_SIZE_V2"; break;
        case HCI_OPCODE_LE_READ_ISO_TX_SYNC: ret = "HCI_OPCODE_LE_READ_ISO_TX_SYNC"; break;
        case HCI_OPCODE_LE_SET_CIG_PARAMS: ret = "HCI_OPCODE_LE_SET_CIG_PARAMS"; break;
        case HCI_OPCODE_LE_SET_CIG_PARAMS_TEST: ret = "HCI_OPCODE_LE_SET_CIG_PARAMS_TEST"; break;
        case HCI_OPCODE_LE_CREATE_CIS: ret = "HCI_OPCODE_LE_CREATE_CIS"; break;
        case HCI_OPCODE_LE_REMOVE_CIG: ret = "HCI_OPCODE_LE_REMOVE_CIG"; break;
        case HCI_OPCODE_LE_ACCEPT_CIS_REQ: ret = "HCI_OPCODE_LE_ACCEPT_CIS_REQ"; break;
        case HCI_OPCODE_LE_REJECT_CIS_REQ: ret = "HCI_OPCODE_LE_REJECT_CIS_REQ"; break;
        case HCI_OPCODE_LE_CREATE_BIG: ret = "HCI_OPCODE_LE_CREATE_BIG"; break;
        case HCI_OPCODE_LE_CREATE_BIG_TEST: ret = "HCI_OPCODE_LE_CREATE_BIG_TEST"; break;
        case HCI_OPCODE_LE_TERMINATE_BIG: ret = "HCI_OPCODE_LE_TERMINATE_BIG"; break;
        case HCI_OPCODE_LE_BIG_CREATE_SYNC: ret = "HCI_OPCODE_LE_BIG_CREATE_SYNC"; break;
        case HCI_OPCODE_LE_BIG_TERMINATE_SYNC: ret = "HCI_OPCODE_LE_BIG_TERMINATE_SYNC"; break;
        case HCI_OPCODE_LE_REQUEST_PEER_SCA: ret = "HCI_OPCODE_LE_REQUEST_PEER_SCA"; break;
        case HCI_OPCODE_LE_SETUP_ISO_DATA_PATH: ret = "HCI_OPCODE_LE_SETUP_ISO_DATA_PATH"; break;
        case HCI_OPCODE_LE_REMOVE_ISO_DATA_PATH: ret = "HCI_OPCODE_LE_REMOVE_ISO_DATA_PATH"; break;
        case HCI_OPCODE_LE_ISO_TX_TEST: ret = "HCI_OPCODE_LE_ISO_TX_TEST"; break;
        case HCI_OPCODE_LE_ISO_RX_TEST: ret = "HCI_OPCODE_LE_ISO_RX_TEST"; break;
        case HCI_OPCODE_LE_ISO_READ_TEST_COUNTERS: ret = "HCI_OPCODE_LE_ISO_READ_TEST_COUNTERS"; break;
        case HCI_OPCODE_LE_ISO_TEST_END: ret = "HCI_OPCODE_LE_ISO_TEST_END"; break;
        case HCI_OPCODE_LE_SET_HOST_FEATURE: ret = "HCI_OPCODE_LE_SET_HOST_FEATURE"; break;
        case HCI_OPCODE_LE_READ_ISO_LINK_QUAL: ret = "HCI_OPCODE_LE_READ_ISO_LINK_QUAL"; break;
        case HCI_OPCODE_LE_READ_ENHANCED_TX_POWER: ret = "HCI_OPCODE_LE_READ_ENHANCED_TX_POWER"; break;
        case HCI_OPCODE_LE_READ_REMOTE_TX_POWER: ret = "HCI_OPCODE_LE_READ_REMOTE_TX_POWER"; break;
        case HCI_OPCODE_LE_SET_PATH_LOSS_REPORTING_PARAMS: ret = "HCI_OPCODE_LE_SET_PATH_LOSS_REPORTING_PARAMS"; break;
        case HCI_OPCODE_LE_SET_PATH_LOSS_REPORTING_ENABLE: ret = "HCI_OPCODE_LE_SET_PATH_LOSS_REPORTING_ENABLE"; break;
        case HCI_OPCODE_LE_SET_TX_POWER_REPORT_ENABLE: ret = "HCI_OPCODE_LE_SET_TX_POWER_REPORT_ENABLE"; break;
    }
#else // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    static char ret[3] = "00";
    sprintf(ret, "%02x", (int)opcode);
#endif // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    return ret;
}
#endif //MBED_CONF_MBED_TRACE_ENABLE

} // namespace ble
