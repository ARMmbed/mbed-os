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
#include "pal/AttServerMessage.h"
#include "att_api.h"
#define TRACE_GROUP "BLE "

namespace ble {
#if MBED_CONF_MBED_TRACE_ENABLE

const char *ble_error_to_string(ble_error_t error_code)
{
#if MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    const char *ret = "INVALID ENUM VALUE";
    switch(error_code)
    {
        case BLE_ERROR_NONE: ret = "BLE_ERROR_NONE"; break;
        case BLE_ERROR_BUFFER_OVERFLOW: ret = "BLE_ERROR_BUFFER_OVERFLOW"; break;
        case BLE_ERROR_NOT_IMPLEMENTED: ret = "BLE_ERROR_NOT_IMPLEMENTED"; break;
        case BLE_ERROR_PARAM_OUT_OF_RANGE: ret = "BLE_ERROR_PARAM_OUT_OF_RANGE"; break;
        case BLE_ERROR_INVALID_PARAM: ret = "BLE_ERROR_INVALID_PARAM"; break;
        case BLE_STACK_BUSY: ret = "BLE_STACK_BUSY"; break;
        case BLE_ERROR_INVALID_STATE: ret = "BLE_ERROR_INVALID_STATE"; break;
        case BLE_ERROR_NO_MEM: ret = "BLE_ERROR_NO_MEM"; break;
        case BLE_ERROR_OPERATION_NOT_PERMITTED: ret = "BLE_ERROR_OPERATION_NOT_PERMITTED"; break;
        case BLE_ERROR_INITIALIZATION_INCOMPLETE: ret = "BLE_ERROR_INITIALIZATION_INCOMPLETE"; break;
        case BLE_ERROR_ALREADY_INITIALIZED: ret = "BLE_ERROR_ALREADY_INITIALIZED"; break;
        case BLE_ERROR_UNSPECIFIED: ret = "BLE_ERROR_UNSPECIFIED"; break;
        case BLE_ERROR_INTERNAL_STACK_FAILURE: ret = "BLE_ERROR_INTERNAL_STACK_FAILURE"; break;
        case BLE_ERROR_NOT_FOUND: ret = "BLE_ERROR_NOT_FOUND"; break;
    }
#else // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    static char ret[3] = "00";
    sprintf(ret, "%02x", (int)error_code);
#endif // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    return ret;
}

const char *attribute_error_code_to_string(uint8_t error_code)
{
#if MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    const char *ret = "INVALID ENUM VALUE";
    switch(error_code)
    {
        case AttErrorResponse::INVALID_HANDLE: ret = "INVALID_HANDLE"; break;
        case AttErrorResponse::READ_NOT_PERMITTED: ret = "READ_NOT_PERMITTED"; break;
        case AttErrorResponse::WRITE_NOT_PERMITTED: ret = "WRITE_NOT_PERMITTED"; break;
        case AttErrorResponse::INVALID_PDU: ret = "INVALID_PDU"; break;
        case AttErrorResponse::INSUFFICIENT_AUTHENTICATION: ret = "INSUFFICIENT_AUTHENTICATION"; break;
        case AttErrorResponse::REQUEST_NOT_SUPPORTED: ret = "REQUEST_NOT_SUPPORTED"; break;
        case AttErrorResponse::INVALID_OFFSET: ret = "INVALID_OFFSET"; break;
        case AttErrorResponse::INSUFFICIENT_AUTHORIZATION: ret = "INSUFFICIENT_AUTHORIZATION"; break;
        case AttErrorResponse::PREPARE_QUEUE_FULL: ret = "PREPARE_QUEUE_FULL"; break;
        case AttErrorResponse::ATTRIBUTE_NOT_FOUND: ret = "ATTRIBUTE_NOT_FOUND"; break;
        case AttErrorResponse::ATTRIBUTE_NOT_LONG: ret = "ATTRIBUTE_NOT_LONG"; break;
        case AttErrorResponse::INSUFFICIENT_ENCRYPTION_KEY_SIZE: ret = "INSUFFICIENT_ENCRYPTION_KEY_SIZE"; break;
        case AttErrorResponse::INVALID_ATTRIBUTE_VALUE_LENGTH: ret = "INVALID_ATTRIBUTE_VALUE_LENGTH"; break;
        case AttErrorResponse::UNLIKELY_ERROR: ret = "UNLIKELY_ERROR"; break;
        case AttErrorResponse::INSUFFICIENT_ENCRYPTION: ret = "INSUFFICIENT_ENCRYPTION"; break;
        case AttErrorResponse::UNSUPPORTED_GROUP_TYPE: ret = "UNSUPPORTED_GROUP_TYPE"; break;
        case AttErrorResponse::INSUFFICIENT_RESOURCES: ret = "INSUFFICIENT_RESOURCES"; break;
        case AttErrorResponse::WRITE_REQUEST_REJECTED: ret = "WRITE_REQUEST_REJECTED"; break;
        case AttErrorResponse::CLIENT_CHARACTERISTIC_CONFIGURATION_DESCRIPTOR_IMPROPERLY_CONFIGURED:
            ret = "CLIENT_CHARACTERISTIC_CONFIGURATION_DESCRIPTOR_IMPROPERLY_CONFIGURED"; break;
        case AttErrorResponse::PROCEDURE_ALREADY_IN_PROGRESS: ret = "PROCEDURE_ALREADY_IN_PROGRESS"; break;
        case AttErrorResponse::OUT_OF_RANGE: ret = "OUT_OF_RANGE"; break;
    }
#else // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    static char ret[3] = "00";
    sprintf(ret, "%02x", (int)error_code);
#endif // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    return ret;
}

const char *att_client_callback_event_to_string(uint8_t code)
{
#if MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    const char *ret = "INVALID EVENT";
    switch(code)
    {
        case ATTC_FIND_INFO_RSP: ret = "ATTC_FIND_INFO_RSP"; break;
        case ATTC_FIND_BY_TYPE_VALUE_RSP: ret = "ATTC_FIND_BY_TYPE_VALUE_RSP"; break;
        case ATTC_READ_BY_TYPE_RSP: ret = "ATTC_READ_BY_TYPE_RSP"; break;
        case ATTC_READ_RSP: ret = "ATTC_READ_RSP"; break;
        case ATTC_READ_LONG_RSP: ret = "ATTC_READ_LONG_RSP"; break;
        case ATTC_READ_MULTIPLE_RSP: ret = "ATTC_READ_MULTIPLE_RSP"; break;
        case ATTC_READ_BY_GROUP_TYPE_RSP: ret = "ATTC_READ_BY_GROUP_TYPE_RSP"; break;
        case ATTC_WRITE_RSP: ret = "ATTC_WRITE_RSP"; break;
        case ATTC_WRITE_CMD_RSP: ret = "ATTC_WRITE_CMD_RSP"; break;
        case ATTC_PREPARE_WRITE_RSP: ret = "ATTC_PREPARE_WRITE_RSP"; break;
        case ATTC_EXECUTE_WRITE_RSP: ret = "ATTC_EXECUTE_WRITE_RSP"; break;
        case ATTC_HANDLE_VALUE_NTF: ret = "ATTC_HANDLE_VALUE_NTF"; break;
        case ATTC_HANDLE_VALUE_IND: ret = "ATTC_HANDLE_VALUE_IND"; break;
        case ATTC_READ_MULT_VAR_RSP: ret = "ATTC_READ_MULT_VAR_RSP"; break;
        case ATTC_MULT_VALUE_NTF: ret = "ATTC_MULT_VALUE_NTF"; break;
        case ATTS_HANDLE_VALUE_CNF: ret = "ATTS_HANDLE_VALUE_CNF"; break;
        case ATTS_MULT_VALUE_CNF: ret = "ATTS_MULT_VALUE_CNF"; break;
        case ATTS_CCC_STATE_IND: ret = "ATTS_CCC_STATE_IND"; break;
        case ATTS_DB_HASH_CALC_CMPL_IND: ret = "ATTS_DB_HASH_CALC_CMPL_IND"; break;
        case ATT_MTU_UPDATE_IND: ret = "ATT_MTU_UPDATE_IND"; break;
        case ATT_EATT_CONN_CMPL_IND: ret = "ATT_EATT_CONN_CMPL_IND"; break;
        case ATT_EATT_RECONFIG_CMPL_IND: ret = "ATT_EATT_RECONFIG_CMPL_IND"; break;
    }
#else // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    static char ret[3] = "00";
    sprintf(ret, "%02x", (int)code);
#endif // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    return ret;
}

const char *att_error_code_to_string(uint8_t error_code)
{
#if MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    const char *ret = "INVALID EVENT";
    switch(error_code)
    {
        case ATT_SUCCESS: ret = "ATT_SUCCESS"; break;
        case ATT_ERR_HANDLE: ret = "ATT_ERR_HANDLE"; break;
        case ATT_ERR_READ: ret = "ATT_ERR_READ"; break;
        case ATT_ERR_WRITE: ret = "ATT_ERR_WRITE"; break;
        case ATT_ERR_INVALID_PDU: ret = "ATT_ERR_INVALID_PDU"; break;
        case ATT_ERR_AUTH: ret = "ATT_ERR_AUTH"; break;
        case ATT_ERR_NOT_SUP: ret = "ATT_ERR_NOT_SUP"; break;
        case ATT_ERR_OFFSET: ret = "ATT_ERR_OFFSET"; break;
        case ATT_ERR_AUTHOR: ret = "ATT_ERR_AUTHOR"; break;
        case ATT_ERR_QUEUE_FULL: ret = "ATT_ERR_QUEUE_FULL"; break;
        case ATT_ERR_NOT_FOUND: ret = "ATT_ERR_NOT_FOUND"; break;
        case ATT_ERR_NOT_LONG: ret = "ATT_ERR_NOT_LONG"; break;
        case ATT_ERR_KEY_SIZE: ret = "ATT_ERR_KEY_SIZE"; break;
        case ATT_ERR_LENGTH: ret = "ATT_ERR_LENGTH"; break;
        case ATT_ERR_UNLIKELY: ret = "ATT_ERR_UNLIKELY"; break;
        case ATT_ERR_ENC: ret = "ATT_ERR_ENC"; break;
        case ATT_ERR_GROUP_TYPE: ret = "ATT_ERR_GROUP_TYPE"; break;
        case ATT_ERR_RESOURCES: ret = "ATT_ERR_RESOURCES"; break;
        case ATT_ERR_DATABASE_OUT_OF_SYNC: ret = "ATT_ERR_DATABASE_OUT_OF_SYNC"; break;
        case ATT_ERR_VALUE_NOT_ALLOWED: ret = "ATT_ERR_VALUE_NOT_ALLOWED"; break;
        case ATT_ERR_WRITE_REJ: ret = "ATT_ERR_WRITE_REJ"; break;
        case ATT_ERR_CCCD: ret = "ATT_ERR_CCCD"; break;
        case ATT_ERR_IN_PROGRESS: ret = "ATT_ERR_IN_PROGRESS"; break;
        case ATT_ERR_RANGE: ret = "ATT_ERR_RANGE"; break;
    }
#else // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    static char ret[3] = "00";
    sprintf(ret, "%02x", (int)error_code);
#endif // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    return ret;
}

const char *attribute_opcode_to_string(uint8_t error_code)
{
#if MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    const char *ret = "INVALID OPCODE";
    switch(error_code)
    {
        case AttributeOpcode::ERROR_RESPONSE: ret = "ERROR_RESPONSE"; break;
        case AttributeOpcode::EXCHANGE_MTU_REQUEST: ret = "EXCHANGE_MTU_REQUEST"; break;
        case AttributeOpcode::EXCHANGE_MTU_RESPONSE: ret = "EXCHANGE_MTU_RESPONSE"; break;
        case AttributeOpcode::FIND_INFORMATION_REQUEST: ret = "FIND_INFORMATION_REQUEST"; break;
        case AttributeOpcode::FIND_INFORMATION_RESPONSE: ret = "FIND_INFORMATION_RESPONSE"; break;
        case AttributeOpcode::FIND_BY_TYPE_VALUE_REQUEST: ret = "FIND_BY_TYPE_VALUE_REQUEST"; break;
        case AttributeOpcode::FIND_BY_VALUE_TYPE_RESPONSE: ret = "FIND_BY_VALUE_TYPE_RESPONSE"; break;
        case AttributeOpcode::READ_BY_TYPE_REQUEST: ret = "READ_BY_TYPE_REQUEST"; break;
        case AttributeOpcode::READ_BY_TYPE_RESPONSE: ret = "READ_BY_TYPE_RESPONSE"; break;
        case AttributeOpcode::READ_REQUEST: ret = "READ_REQUEST"; break;
        case AttributeOpcode::READ_RESPONSE: ret = "READ_RESPONSE"; break;
        case AttributeOpcode::READ_BLOB_REQUEST: ret = "READ_BLOB_REQUEST"; break;
        case AttributeOpcode::READ_BLOB_RESPONSE: ret = "READ_BLOB_RESPONSE"; break;
        case AttributeOpcode::READ_MULTIPLE_REQUEST: ret = "READ_MULTIPLE_REQUEST"; break;
        case AttributeOpcode::READ_MULTIPLE_RESPONSE: ret = "READ_MULTIPLE_RESPONSE"; break;
        case AttributeOpcode::READ_BY_GROUP_TYPE_REQUEST: ret = "READ_BY_GROUP_TYPE_REQUEST"; break;
        case AttributeOpcode::READ_BY_GROUP_TYPE_RESPONSE: ret = "READ_BY_GROUP_TYPE_RESPONSE"; break;
        case AttributeOpcode::WRITE_REQUEST: ret = "WRITE_REQUEST"; break;
        case AttributeOpcode::WRITE_RESPONSE: ret = "WRITE_RESPONSE"; break;
        case AttributeOpcode::WRITE_COMMAND: ret = "WRITE_COMMAND"; break;
        case AttributeOpcode::SIGNED_WRITE_COMMAND: ret = "SIGNED_WRITE_COMMAND"; break;
        case AttributeOpcode::PREPARE_WRITE_REQUEST: ret = "PREPARE_WRITE_REQUEST"; break;
        case AttributeOpcode::PREPARE_WRITE_RESPONSE: ret = "PREPARE_WRITE_RESPONSE"; break;
        case AttributeOpcode::EXECUTE_WRITE_REQUEST: ret = "EXECUTE_WRITE_REQUEST"; break;
        case AttributeOpcode::EXECUTE_WRITE_RESPONSE: ret = "EXECUTE_WRITE_RESPONSE"; break;
        case AttributeOpcode::HANDLE_VALUE_NOTIFICATION: ret = "HANDLE_VALUE_NOTIFICATION"; break;
        case AttributeOpcode::HANDLE_VALUE_INDICATION: ret = "HANDLE_VALUE_INDICATION"; break;
    }
#else // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    static char ret[3] = "00";
    sprintf(ret, "%02x", (int)error_code);
#endif // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    return ret;
}

const char *dm_callback_event_to_string(uint8_t event) {
#if MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    const char *ret = "INVALID";
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

const char *hci_opcode_to_string(uint16_t opcode)
{
#if MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    const char *ret = "UNKNOWN OPCODE";
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
    static char ret[5] = "0000";
    sprintf(ret, "%04x", opcode);
#endif // MBED_CONF_BLE_TRACE_HUMAN_READABLE_ENUMS
    return ret;
}
#endif //MBED_CONF_MBED_TRACE_ENABLE

const char *to_string(const UUID& uuid)
{
    static constexpr const  size_t  ROW_COUNT = 4;
    static constexpr const char *HEX = "0123456789ABCDEF";
    static constexpr const size_t HYPHENS_DELIMITER_COUNT = 4;

    /*
     * For each hex digit, we require 2 bytes. So, we need at most 2 * 16 = 32 bytes for the hex digits.
     * In the case of 128-bit UUIDs, we also need space for 4 hyphen delimiters.
     * An additional byte is required at the end of each array to store the null terminator.
     */
    static char string_buffer[ROW_COUNT][2 * UUID::LENGTH_OF_LONG_UUID + HYPHENS_DELIMITER_COUNT + 1];
    static uint8_t idx = 0;

    ++idx;

    if (idx == ROW_COUNT) {
        idx= 0;
    }

    const uint8_t  length = uuid.getLen();
    const char *reader  = (const char *) uuid.getBaseUUID() + length - 1;
    char* writer  = string_buffer[idx];

    if (length == UUID::LENGTH_OF_LONG_UUID) {
        constexpr uint8_t format[] = {4, 2, 2, 2, 6};
        for ( uint8_t char_count: format) {
            for (size_t i = 0; i < char_count; ++i) {
                *writer++ = HEX[(*reader >> 4) & 0xF];
                *writer++ = HEX[(*reader--)    & 0xF];
            }
            *writer++ = '-';
        }
        *--writer = 0;
    } else {
        for (size_t i = 0; i < length; ++i) {
            *writer++ = HEX[(*reader >> 4) & 0xF];
            *writer++ = HEX[(*reader--)    & 0xF];
        }
        *writer = 0;
    }

    return string_buffer[idx];
}

const char *to_string(bool v)
{
    if (v) {
        return "true";
    } else {
        return "false";
    }
}

const char *to_string(ble::SecurityManager::SecurityIOCapabilities_t capabilities)
{
    switch(capabilities) {
        case ble::SecurityManager::IO_CAPS_DISPLAY_ONLY:
            return "IO_CAPS_DISPLAY_ONLY";
        case ble::SecurityManager::IO_CAPS_DISPLAY_YESNO:
            return "IO_CAPS_DISPLAY_YESNO";
        case ble::SecurityManager::IO_CAPS_KEYBOARD_DISPLAY:
            return "IO_CAPS_KEYBOARD_DISPLAY";
        case ble::SecurityManager::IO_CAPS_KEYBOARD_ONLY:
            return "IO_CAPS_KEYBOARD_ONLY";
        case ble::SecurityManager::IO_CAPS_NONE:
            return "IO_CAPS_NONE";
        default:
            return "unknown";
    }
}

const char *to_string(ble::io_capability_t capabilities)
{
    switch (capabilities.value()) {
        case ble::io_capability_t::DISPLAY_ONLY:
            return "DISPLAY_ONLY";
        case ble::io_capability_t::DISPLAY_YES_NO:
            return "IO_CAPS_DISPLAY_DISPLAY_YES_NOYESNO";
        case ble::io_capability_t::KEYBOARD_ONLY:
            return "KEYBOARD_ONLY";
        case ble::io_capability_t::NO_INPUT_NO_OUTPUT:
            return "NO_INPUT_NO_OUTPUT";
        case ble::io_capability_t::KEYBOARD_DISPLAY:
            return "KEYBOARD_DISPLAY";
        default:
            return "unknown";
    }
}

const char *to_string(ble::SecurityManager::SecurityMode_t security_mode)
{
    switch (security_mode) {
        case ble::SecurityManager::SECURITY_MODE_NO_ACCESS:
            return "SECURITY_MODE_NO_ACCESS";
        case ble::SecurityManager::SECURITY_MODE_ENCRYPTION_OPEN_LINK:
            return "SECURITY_MODE_ENCRYPTION_OPEN_LINK";
        case ble::SecurityManager::SECURITY_MODE_ENCRYPTION_NO_MITM:
            return "SECURITY_MODE_ENCRYPTION_NO_MITM";
        case ble::SecurityManager::SECURITY_MODE_ENCRYPTION_WITH_MITM:
            return "SECURITY_MODE_ENCRYPTION_WITH_MITM";
        case ble::SecurityManager::SECURITY_MODE_SIGNED_NO_MITM:
            return "SECURITY_MODE_SIGNED_NO_MITM";
        case ble::SecurityManager::SECURITY_MODE_SIGNED_WITH_MITM:
            return "SECURITY_MODE_SIGNED_WITH_MITM";
        default:
            return "Unknown";
    }
}

const char *passkey_str(const uint8_t* passkey)
{
    if (!passkey) {
        return "0";
    } else {
        return (mbed_trace_array)(passkey, 6);
    }
}

const char *to_string(const ble::address_t& address)
{
    return (mbed_trace_array)(address.data(), address.size());
}

const char *to_string(const ble::link_encryption_t encryption)
{
    using link_encryption_t = ble::link_encryption_t;

    switch (encryption.value()) {
        case link_encryption_t::NOT_ENCRYPTED:
            return "NOT_ENCRYPTED";
        case link_encryption_t::ENCRYPTION_IN_PROGRESS:
            return "ENCRYPTION_IN_PROGRESS";
        case link_encryption_t::ENCRYPTED:
            return "ENCRYPTED";
        case link_encryption_t::ENCRYPTED_WITH_MITM:
            return "ENCRYPTED_WITH_MITM";
        case link_encryption_t::ENCRYPTED_WITH_SC_AND_MITM:
            return "ENCRYPTED_WITH_SC_AND_MITM";
        default:
            return "Unknown";
    }
}

const char *to_string(Keypress_t keypress)
{
    switch (keypress) {
        case KEYPRESS_STARTED:
            return "KEYPRESS_STARTED";
        case KEYPRESS_ENTERED:
            return "KEYPRESS_ENTERED";
        case KEYPRESS_ERASED:
            return "KEYPRESS_ERASED";
        case KEYPRESS_CLEARED:
            return "KEYPRESS_CLEARED";
        case KEYPRESS_COMPLETED:
            return "KEYPRESS_COMPLETED";
        default:
            return "Unknown";
    }
}

const char *to_string(ble::pairing_failure_t reason)
{
    switch (reason.value()) {
        case ble::pairing_failure_t::PASSKEY_ENTRY_FAILED:
            return "PASSKEY_ENTRY_FAILED";
        case ble::pairing_failure_t::OOB_NOT_AVAILABLE:
            return "OOB_NOT_AVAILABLE";
        case ble::pairing_failure_t::AUTHENTICATION_REQUIREMENTS:
            return "AUTHENTICATION_REQUIREMENTS";
        case ble::pairing_failure_t::CONFIRM_VALUE_FAILED:
            return "CONFIRM_VALUE_FAILED";
        case ble::pairing_failure_t::PAIRING_NOT_SUPPORTED:
            return "PAIRING_NOT_SUPPORTED";
        case ble::pairing_failure_t::ENCRYPTION_KEY_SIZE:
            return "ENCRYPTION_KEY_SIZE";
        case ble::pairing_failure_t::COMMAND_NOT_SUPPORTED:
            return "COMMAND_NOT_SUPPORTED";
        case ble::pairing_failure_t::UNSPECIFIED_REASON:
            return "UNSPECIFIED_REASON";
        case ble::pairing_failure_t::REPEATED_ATTEMPTS:
            return "REPEATED_ATTEMPTS";
        case ble::pairing_failure_t::INVALID_PARAMETERS:
            return "INVALID_PARAMETERS";
        case ble::pairing_failure_t::DHKEY_CHECK_FAILED:
            return "DHKEY_CHECK_FAILED";
        case ble::pairing_failure_t::NUMERIC_COMPARISON_FAILED:
            return "NUMERIC_COMPARISON_FAILED";
        case ble::pairing_failure_t::BR_EDR_PAIRING_IN_PROGRESS:
            return "BR_EDR_PAIRING_IN_PROGRESS";
        case ble::pairing_failure_t::CROSS_TRANSPORT_KEY_DERIVATION_OR_GENERATION_NOT_ALLOWED:
            return "CROSS_TRANSPORT_KEY_DERIVATION_OR_GENERATION_NOT_ALLOWED";
        default:
            return "Unknown";
    }
}

const char *to_string(target_peer_address_type_t type)
{
    if (type == target_peer_address_type_t::PUBLIC) {
        return "PUBLIC";
    } else {
        return "RANDOM";
    }
}

const char *to_string(privacy_mode_t mode)
{
    if (mode == privacy_mode_t::NETWORK) {
        return "NETWORK";
    } else {
        return "DEVICE";
    }
}

const char *to_string(ble::advertising_type_t advType)
{
    switch (advType.value()) {
        case ble::advertising_type_t::CONNECTABLE_UNDIRECTED:
            return "CONNECTABLE_UNDIRECTED";
        case ble::advertising_type_t::CONNECTABLE_DIRECTED:
            return "CONNECTABLE_DIRECTED";
        case ble::advertising_type_t::SCANNABLE_UNDIRECTED:
            return "SCANNABLE_UNDIRECTED";
        case ble::advertising_type_t::NON_CONNECTABLE_UNDIRECTED:
            return "NON_CONNECTABLE_UNDIRECTED";
        case ble::advertising_type_t::CONNECTABLE_DIRECTED_LOW_DUTY:
            return "CONNECTABLE_DIRECTED_LOW_DUTY";
        case ble::advertising_type_t::CONNECTABLE_NON_SCANNABLE_UNDIRECTED:
            return "CONNECTABLE_NON_SCANNABLE_UNDIRECTED";
        default:
            return "unknown";
    }
}

const char *to_string(ble::own_address_type_t addressType)
{
    switch (addressType.value()) {
        case ble::own_address_type_t::PUBLIC:
            return "PUBLIC";
        case ble::own_address_type_t::RANDOM:
            return "RANDOM";
        case ble::own_address_type_t::RESOLVABLE_PRIVATE_ADDRESS_PUBLIC_FALLBACK:
            return "RESOLVABLE_PRIVATE_ADDRESS_PUBLIC_FALLBACK";
        case ble::own_address_type_t::RESOLVABLE_PRIVATE_ADDRESS_RANDOM_FALLBACK:
            return "RESOLVABLE_PRIVATE_ADDRESS_RANDOM_FALLBACK";
        default:
            return "unknown";
    }
}

const char *to_string(ble::advertising_filter_policy_t policy)
{
    switch (policy.value()) {
        case ble::advertising_filter_policy_t::NO_FILTER:
            return "NO_FILTER";
        case ble::advertising_filter_policy_t::FILTER_SCAN_REQUESTS:
            return "FILTER_SCAN_REQUESTS";
        case ble::advertising_filter_policy_t::FILTER_CONNECTION_REQUEST:
            return "FILTER_CONNECTION_REQUEST";
        case ble::advertising_filter_policy_t::FILTER_SCAN_AND_CONNECTION_REQUESTS:
            return "FILTER_SCAN_AND_CONNECTION_REQUESTS";
        default:
            return "unknown";
    }
}

const char *to_string(ble::phy_t phy)
{
    switch (phy.value()) {
        case ble::phy_t::NONE:
            return "NONE";
        case ble::phy_t::LE_1M:
            return "LE_1M";
        case ble::phy_t::LE_2M:
            return "LE_2M";
        case ble::phy_t::LE_CODED:
            return "LE_CODED";
        default:
            return "unknown";
    }
}

const char *to_string(ble::scanning_filter_policy_t policy)
{
    switch (policy.value()) {
        case ble::scanning_filter_policy_t::NO_FILTER:
            return "NO_FILTER";
        case ble::scanning_filter_policy_t::FILTER_ADVERTISING:
            return "FILTER_ADVERTISING";
        case ble::scanning_filter_policy_t::NO_FILTER_INCLUDE_UNRESOLVABLE_DIRECTED:
            return "NO_FILTER_INCLUDE_UNRESOLVABLE_DIRECTED";
        case ble::scanning_filter_policy_t::FILTER_ADVERTISING_INCLUDE_UNRESOLVABLE_DIRECTED:
            return "FILTER_ADVERTISING_INCLUDE_UNRESOLVABLE_DIRECTED";
        default:
            return "unknown";
    }
}

const char *to_string(ble::duplicates_filter_t filtering)
{
    switch (filtering.value()) {
        case ble::duplicates_filter_t::ENABLE:
            return "ENABLE";
        case ble::duplicates_filter_t::DISABLE:
            return "DISABLE";
        case ble::duplicates_filter_t::PERIODIC_RESET:
            return "PERIODIC_RESET";
        default:
            return "unknown";
    }
}

const char *to_string(ble::peer_address_type_t addressType)
{
    switch (addressType.value()) {
        case ble::peer_address_type_t::PUBLIC:
            return "PUBLIC";
        case ble::peer_address_type_t::RANDOM:
            return "RANDOM";
        case ble::peer_address_type_t::PUBLIC_IDENTITY:
            return "PUBLIC_IDENTITY";
        case ble::peer_address_type_t::RANDOM_STATIC_IDENTITY:
            return "RANDOM_STATIC_IDENTITY";
        case ble::peer_address_type_t::ANONYMOUS:
            return "ANONYMOUS";
        default:
            return "unknown";
    }
}

const char *to_string(ble::initiator_filter_policy_t policy)
{
    switch(policy.value()) {
        case ble::initiator_filter_policy_t::NO_FILTER:
            return "NO_FILTER";
        case ble::initiator_filter_policy_t::USE_WHITE_LIST:
            return "USE_WHITE_LIST";
        default:
            return "unknown";
    }
}

const char *to_string(ble::coded_symbol_per_bit_t sym)
{
    switch (sym.value()) {
        case ble::coded_symbol_per_bit_t::UNDEFINED:
            return "UNDEFINED";
        case ble::coded_symbol_per_bit_t::S2:
            return "S2";
        case ble::coded_symbol_per_bit_t::S8:
            return "S8";
        default:
            return "unknown";
    }
}

const char *to_string(ble::local_disconnection_reason_t reason)
{
    switch (reason.value()) {
        case ble::local_disconnection_reason_t::AUTHENTICATION_FAILURE:
            return "AUTHENTICATION_FAILURE";
        case ble::local_disconnection_reason_t::USER_TERMINATION:
            return "USER_TERMINATION";
        case ble::local_disconnection_reason_t::LOW_RESOURCES:
            return "LOW_RESOURCES";
        case ble::local_disconnection_reason_t::POWER_OFF:
            return "POWER OFF";
        case ble::local_disconnection_reason_t::UNSUPPORTED_REMOTE_FEATURE:
            return "UNSUPPORTED_REMOTE_FEATURE";
        case ble::local_disconnection_reason_t::PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED:
            return "PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED";
        case ble::local_disconnection_reason_t::UNACCEPTABLE_CONNECTION_PARAMETERS:
            return "UNACCEPTABLE_CONNECTION_PARAMETERS";
        default:
            return "unknown";
    }
}

const char *to_string(ble::advertising_channel_map_t map)
{
    switch (map.value()) {
        case ble::advertising_channel_map_t::ADVERTISING_CHANNEL_37:
            return "[001]";
        case ble::advertising_channel_map_t::ADVERTISING_CHANNEL_38:
            return "[010]";
        case ble::advertising_channel_map_t::ADVERTISING_CHANNEL_37_AND_38:
            return "[011]";
        case ble::advertising_channel_map_t::ADVERTISING_CHANNEL_39:
            return "[100]";
        case ble::advertising_channel_map_t::ADVERTISING_CHANNEL_37_AND_39:
            return "[101]";
        case ble::advertising_channel_map_t::ADVERTISING_CHANNEL_38_AND_39:
            return "[110]";
        case ble::advertising_channel_map_t::ALL_ADVERTISING_CHANNELS:
            return "[111]";
        default:
            return "unknown";
    }
}

const char *to_string(ble::connection_peer_address_type_t connection_peer_address_type)
{
    switch (connection_peer_address_type.value()) {
        case ble::connection_peer_address_type_t::PUBLIC_ADDRESS:
            return "PUBLIC_ADDRESS";
        case ble::connection_peer_address_type_t::RANDOM_ADDRESS:
            return "RANDOM_ADDRESS";
        case ble::connection_peer_address_type_t::PUBLIC_IDENTITY_ADDRESS:
            return "PUBLIC_IDENTITY_ADDRESS";
        case ble::connection_peer_address_type_t::RANDOM_IDENTITY_ADDRESS:
            return "RANDOM_IDENTITY_ADDRESS";
        default:
            return "unknown";
    }
}

const char *to_string(ble::whitelist_address_type_t whitelist_address_type)
{
    if (whitelist_address_type.value() == ble::whitelist_address_type_t::PUBLIC_DEVICE_ADDRESS) {
        return "PUBLIC_DEVICE_ADDRESS";
    } else {
        return "RANDOM_DEVICE_ADDRESS";
    }
}

const char *to_string(ble::hci_error_code_t error)
{
    switch (error.value()) {
        case ble::hci_error_code_t::SUCCESS:
            return "SUCCESS";
        case ble::hci_error_code_t::UNKNOWN_HCI_COMMAND:
            return "UNKNOWN_HCI_COMMAND";
        case ble::hci_error_code_t::UNKNOWN_CONNECTION_IDENTIFIER:
            return "UNKNOWN_CONNECTION_IDENTIFIER";
        case ble::hci_error_code_t::HARDWARE_FAILLURE:
            return "HARDWARE_FAILLURE";
        case ble::hci_error_code_t::PAGE_TIMEOUT:
            return "PAGE_TIMEOUT";
        case ble::hci_error_code_t::AUTHENTICATION_FAILLURE:
            return "AUTHENTICATION_FAILLURE";
        case ble::hci_error_code_t::PIN_OR_KEY_MISSING:
            return "PIN_OR_KEY_MISSING";
        case ble::hci_error_code_t::MEMORY_CAPACITY_EXCEEDED:
            return "MEMORY_CAPACITY_EXCEEDED";
        case ble::hci_error_code_t::CONNECTION_TIMEOUT:
            return "CONNECTION_TIMEOUT";
        case ble::hci_error_code_t::CONNECTION_LIMIT_EXCEEDED:
            return "CONNECTION_LIMIT_EXCEEDED";
        case ble::hci_error_code_t::SYNCHRONOUS_CONNECTION_LIMIT_TO_A_DEVICE_EXCEEDED:
            return "SYNCHRONOUS_CONNECTION_LIMIT_TO_A_DEVICE_EXCEEDED";
        case ble::hci_error_code_t::CONNECTION_ALREADY_EXIST:
            return "CONNECTION_ALREADY_EXIST";
        case ble::hci_error_code_t::COMMAND_DISALLOWED:
            return "COMMAND_DISALLOWED";
        case ble::hci_error_code_t::CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES:
            return "CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES";
        case ble::hci_error_code_t::CONNECTION_REJECTED_DUE_TO_SECURITY_REASONS:
            return "CONNECTION_REJECTED_DUE_TO_SECURITY_REASONS";
        case ble::hci_error_code_t::CONNECTION_REJECTED_DUE_TO_UNACCEPTABLE_BD_ADDR:
            return "CONNECTION_REJECTED_DUE_TO_UNACCEPTABLE_BD_ADDR";
        case ble::hci_error_code_t::CONNECTION_ACCEPT_TIMEOUT_EXCEEDED:
            return "CONNECTION_ACCEPT_TIMEOUT_EXCEEDED";
        case ble::hci_error_code_t::UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE:
            return "UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE";
        case ble::hci_error_code_t::INVALID_HCI_COMMAND_PARAMETERS:
            return "INVALID_HCI_COMMAND_PARAMETERS";
        case ble::hci_error_code_t::REMOTE_USER_TERMINATED_CONNECTION:
            return "REMOTE_USER_TERMINATED_CONNECTION";
        case ble::hci_error_code_t::REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES:
            return "REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES";
        case ble::hci_error_code_t::REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF:
            return "REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF";
        case ble::hci_error_code_t::CONNECTION_TERMINATED_BY_LOCAL_HOST:
            return "CONNECTION_TERMINATED_BY_LOCAL_HOST";
        case ble::hci_error_code_t::REPEATED_ATTEMPTS:
            return "REPEATED_ATTEMPTS";
        case ble::hci_error_code_t::PAIRING_NOT_ALLOWED:
            return "PAIRING_NOT_ALLOWED";
        case ble::hci_error_code_t::UNKNOWN_LMP_PDU:
            return "UNKNOWN_LMP_PDU";
        case ble::hci_error_code_t::UNSUPPORTED_REMOTE_FEATURE | ble::hci_error_code_t::UNSUPPORTED_LMP_FEATURE:
            return "UNSUPPORTED_REMOTE_FEATURE/UNSUPPORTED_LMP_FEATURE";
        case ble::hci_error_code_t::SCO_OFFSET_REJECTED:
            return "SCO_OFFSET_REJECTED";
        case ble::hci_error_code_t::SCO_INTERVAL_REJECTED:
            return "SCO_INTERVAL_REJECTED";
        case ble::hci_error_code_t::SCO_AIR_MODE_REJECTED:
            return "SCO_AIR_MODE_REJECTED";
        case ble::hci_error_code_t::INVALID_LMP_PARAMETERS | ble::hci_error_code_t::INVALID_LL_PARAMETERS:
            return "INVALID_LMP_PARAMETERS/INVALID_LL_PARAMETERS";
        case ble::hci_error_code_t::UNSPECIFIED_ERROR:
            return "UNSPECIFIED_ERROR";
        case ble::hci_error_code_t::UNSUPPORTED_LMP_PARAMETER_VALUE | ble::hci_error_code_t::UNSUPPORTED_LL_PARAMETER_VALUE:
            return "UNSUPPORTED_LMP_PARAMETER_VALUE/UNSUPPORTED_LL_PARAMETER_VALUE";
        case ble::hci_error_code_t::ROLE_CHANGE_NOT_ALLOWED:
            return "ROLE_CHANGE_NOT_ALLOWED";
        case ble::hci_error_code_t::LMP_RESPONSE_TIMEOUT | ble::hci_error_code_t::LL_RESPONSE_TIMEOUT:
            return "LMP_RESPONSE_TIMEOUT/LL_RESPONSE_TIMEOUT";
        case ble::hci_error_code_t::LMP_ERROR_TRANSACTION_COLLISION | ble::hci_error_code_t::LL_PROCEDURE_COLLISION:
            return "LMP_ERROR_TRANSACTION_COLLISION/LL_PROCEDURE_COLLISION";
        case ble::hci_error_code_t::LMP_PDU_NOT_ALLOWED:
            return "LMP_PDU_NOT_ALLOWED";
        case ble::hci_error_code_t::ENCRYPTION_MODE_NOT_ACCEPTABLE:
            return "ENCRYPTION_MODE_NOT_ACCEPTABLE";
        case ble::hci_error_code_t::LINK_KEY_CANNOT_BE_CHANGED:
            return "LINK_KEY_CANNOT_BE_CHANGED";
        case ble::hci_error_code_t::REQUESTED_QOS_NOT_SUPPORTED:
            return "REQUESTED_QOS_NOT_SUPPORTED";
        case ble::hci_error_code_t::INSTANT_PASSED:
            return "INSTANT_PASSED";
        case ble::hci_error_code_t::PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED:
            return "PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED";
        case ble::hci_error_code_t::DIFFERENT_TRANSACTION_COLLISION:
            return "DIFFERENT_TRANSACTION_COLLISION";
        case ble::hci_error_code_t::RESERVED_FOR_FUTURE_USE:
            return "RESERVED_FOR_FUTURE_USE";
        case ble::hci_error_code_t::QOS_UNACCEPTABLE_PARAMETER:
            return "QOS_UNACCEPTABLE_PARAMETER";
        case ble::hci_error_code_t::QOS_REJECTED:
            return "QOS_REJECTED";
        case ble::hci_error_code_t::CHANNEL_CLASSIFICATION_NOT_SUPPORTED:
            return "CHANNEL_CLASSIFICATION_NOT_SUPPORTED";
        case ble::hci_error_code_t::INSUFFICIENT_SECURITY:
            return "INSUFFICIENT_SECURITY";
        case ble::hci_error_code_t::PARAMETER_OUT_OF_MANDATORY_RANGE:
            return "PARAMETER_OUT_OF_MANDATORY_RANGE";
        case ble::hci_error_code_t::ROLE_SWITCH_PENDING:
            return "ROLE_SWITCH_PENDING";
        case ble::hci_error_code_t::RESERVED_SLOT_VIOLATION:
            return "RESERVED_SLOT_VIOLATION";
        case ble::hci_error_code_t::ROLE_SWITCH_FAILED:
            return "ROLE_SWITCH_FAILED";
        case ble::hci_error_code_t::EXTENDED_INQUIRY_RESPONSE_TOO_LARGE:
            return "EXTENDED_INQUIRY_RESPONSE_TOO_LARGE";
        case ble::hci_error_code_t::SECURE_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST:
            return "SECURE_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST";
        case ble::hci_error_code_t::HOST_BUSY_PAIRING:
            return "HOST_BUSY_PAIRING";
        case ble::hci_error_code_t::CONNECTION_REJECTED_DUE_TO_NO_SUITABLE_CHANNEL_FOUND:
            return "CONNECTION_REJECTED_DUE_TO_NO_SUITABLE_CHANNEL_FOUND";
        case ble::hci_error_code_t::CONTROLLER_BUSY:
            return "CONTROLLER_BUSY";
        case ble::hci_error_code_t::UNACCEPTABLE_CONNECTION_PARAMETERS:
            return "UNACCEPTABLE_CONNECTION_PARAMETERS";
        case ble::hci_error_code_t::ADVERTISING_TIMEOUT:
            return "ADVERTISING_TIMEOUT";
        case ble::hci_error_code_t::CONNECTION_TERMINATED_DUE_TO_MIC_FAILURE:
            return "CONNECTION_TERMINATED_DUE_TO_MIC_FAILURE";
        case ble::hci_error_code_t::CONNECTION_FAILED_TO_BE_ESTABLISHED:
            return "CONNECTION_FAILED_TO_BE_ESTABLISHED";
        case ble::hci_error_code_t::MAC_CONNECTION_FAILED:
            return "MAC_CONNECTION_FAILED";
        case ble::hci_error_code_t::COARSE_CLOCK_ADJUSTMENT_REJECTED_BUT_WILL_TRY_TO_ADJUST_USING_CLOCK_DRAGGING:
            return "COARSE_CLOCK_ADJUSTMENT_REJECTED_BUT_WILL_TRY_TO_ADJUST_USING_CLOCK_DRAGGING";
        case ble::hci_error_code_t::TYPE0_SUBMAP_NOT_DEFINED:
            return "TYPE0_SUBMAP_NOT_DEFINED";
        case ble::hci_error_code_t::UNKNOWN_ADVERTISING_IDENTIFIER:
            return "UNKNOWN_ADVERTISING_IDENTIFIER";
        case ble::hci_error_code_t::LIMIT_REACHED:
            return "LIMIT_REACHED";
        case ble::hci_error_code_t::OPERATION_CANCELLED_BY_HOST:
            return "OPERATION_CANCELLED_BY_HOST";
        default:
            return "unknown";
    }
}

const char *to_string(ble::advertising_fragment_description_t fragment_description)
{
    switch (fragment_description.value()) {
        case ble::advertising_fragment_description_t::INTERMEDIATE_FRAGMENT:
            return "INTERMEDIATE_FRAGMENT";
        case ble::advertising_fragment_description_t::FIRST_FRAGMENT:
            return "FIRST_FRAGMENT";
        case ble::advertising_fragment_description_t::LAST_FRAGMENT:
            return "LAST_FRAGMENT";
        case ble::advertising_fragment_description_t::COMPLETE_FRAGMENT:
            return "COMPLETE_FRAGMENT";
        case ble::advertising_fragment_description_t::UNCHANGED_DATA:
            return "UNCHANGED_DATA";
        default:
            return "unknown";
    }
}

const char *to_string(ble::peripheral_privacy_configuration_t::resolution_strategy_t resolution_strategy)
{
    switch (resolution_strategy) {
        case ble::peripheral_privacy_configuration_t::resolution_strategy_t::DO_NOT_RESOLVE:
            return "DO_NOT_RESOLVE";
        case ble::peripheral_privacy_configuration_t::resolution_strategy_t::REJECT_NON_RESOLVED_ADDRESS:
            return "REJECT_NON_RESOLVED_ADDRESS";
        case ble::peripheral_privacy_configuration_t::resolution_strategy_t::PERFORM_PAIRING_PROCEDURE:
            return "PERFORM_PAIRING_PROCEDURE";
        case ble::peripheral_privacy_configuration_t::resolution_strategy_t::PERFORM_AUTHENTICATION_PROCEDURE:
            return "PERFORM_AUTHENTICATION_PROCEDURE";
        default:
            return "unknown";
    }
}

const char *to_string(ble::GapEventType event_type)
{
    switch (event_type.value()) {
        case ble::GapEventType::type::UNEXPECTED_ERROR:
            return "UNEXPECTED_ERROR";
        case ble::GapEventType::type::CONNECTION_COMPLETE:
            return "CONNECTION_COMPLETE";
        case ble::GapEventType::type::ADVERTISING_REPORT:
            return "ADVERTISING_REPORT";
        case ble::GapEventType::type::CONNECTION_UPDATE:
            return "CONNECTION_UPDATE";
        case ble::GapEventType::type::REMOTE_CONNECTION_PARAMETER_REQUEST:
            return "REMOTE_CONNECTION_PARAMETER_REQUEST";
        case ble::GapEventType::type::DISCONNECTION_COMPLETE:
            return "DISCONNECTION_COMPLETE";
        default:
            return "unknown";
    }
}

const char *to_string(ble::received_advertising_type_t type)
{
    switch (type.value()) {
        case ble::received_advertising_type_t::ADV_IND:
            return "ADV_IND";
        case ble::received_advertising_type_t::ADV_DIRECT_IND:
            return "ADV_DIRECT_IND";
        case ble::received_advertising_type_t::ADV_SCAN_IND:
            return "ADV_SCAN_IND";
        case ble::received_advertising_type_t::ADV_NONCONN_IND:
            return "ADV_NONCONN_IND";
        case ble::received_advertising_type_t::SCAN_RESPONSE:
            return "SCAN_RESPONSE";
        default:
            return "unknown";
    }
}

const char *to_string(ble::connection_role_t role)
{
    if (role.value() == ble::connection_role_t::CENTRAL) {
        return "CENTRAL";
    } else {
        return "PERIPHERAL";
    }
}

const char *to_string(ble::direct_address_type_t direct_address_type)
{
    switch (direct_address_type.value()) {
        case ble::direct_address_type_t::PUBLIC_ADDRESS:
            return "PUBLIC_ADDRESS";
        case ble::direct_address_type_t::RANDOM_ADDRESS:
            return "RANDOM_ADDRESS";
        case ble::direct_address_type_t::PUBLIC_IDENTITY_ADDRESS:
            return "PUBLIC_IDENTITY_ADDRESS";
        case ble::direct_address_type_t::RANDOM_IDENTITY_ADDRESS:
            return "RANDOM_IDENTITY_ADDRESS";
        case ble::direct_address_type_t::RANDOM_DEVICE_ADDRESS:
            return "RANDOM_DEVICE_ADDRESS";
        default:
            return "unknown";
    }
}

const char *to_string(ble::advertising_data_status_t data_status) {
    switch (data_status.value()) {
        case ble::advertising_data_status_t::COMPLETE:
            return "COMPLETE";
        case ble::advertising_data_status_t::INCOMPLETE_MORE_DATA:
            return "INCOMPLETE_MORE_DATA";
        case ble::advertising_data_status_t::INCOMPLETE_DATA_TRUNCATED:
            return "INCOMPLETE_DATA_TRUNCATED";
        default:
            return "unknown";
    }
}
const char *to_string(ble::att_security_requirement_t security_requirement)
{
    switch (security_requirement.value()) {
        case ble::att_security_requirement_t::NONE:
            return "NONE";
        case ble::att_security_requirement_t::UNAUTHENTICATED:
            return "UNAUTHENTICATED";
        case ble::att_security_requirement_t::AUTHENTICATED:
            return "AUTHENTICATED";
        case ble::att_security_requirement_t::SC_AUTHENTICATED:
            return "SC_AUTHENTICATED";
        default:
            return "unknown";
    }
}

const char *to_string(GattAuthCallbackReply_t authorization_reply)
{
    switch (authorization_reply) {
        case AUTH_CALLBACK_REPLY_SUCCESS:
            return "SUCCESS";
        case AUTH_CALLBACK_REPLY_ATTERR_INVALID_HANDLE:
            return "INVALID_HANDLE";
        case AUTH_CALLBACK_REPLY_ATTERR_READ_NOT_PERMITTED:
            return "READ_NOT_PERMITTED";
        case AUTH_CALLBACK_REPLY_ATTERR_WRITE_NOT_PERMITTED:
            return "WRITE_NOT_PERMITTED";
        case AUTH_CALLBACK_REPLY_ATTERR_INVALID_PDU:
            return "INVALID_PDU";
        case AUTH_CALLBACK_REPLY_ATTERR_INSUFFICIENT_AUTHENTICATION:
            return "INSUFFICIENT_AUTHENTICATION";
        case AUTH_CALLBACK_REPLY_ATTERR_REQUEST_NOT_SUPPORTED:
            return "REQUEST_NOT_SUPPORTED";
        case AUTH_CALLBACK_REPLY_ATTERR_INVALID_OFFSET:
            return "INVALID_OFFSET";
        case AUTH_CALLBACK_REPLY_ATTERR_INSUFFICIENT_AUTHORIZATION:
            return "INSUFFICIENT_AUTHORIZATION";
        case AUTH_CALLBACK_REPLY_ATTERR_PREPARE_QUEUE_FULL:
            return "PREPARE_QUEUE_FULL";
        case AUTH_CALLBACK_REPLY_ATTERR_ATTRIBUTE_NOT_FOUND:
            return "ATTRIBUTE_NOT_FOUND";
        case AUTH_CALLBACK_REPLY_ATTERR_ATTRIBUTE_NOT_LONG:
            return "ATTRIBUTE_NOT_LONG";
        case AUTH_CALLBACK_REPLY_ATTERR_INSUFFICIENT_ENCRYPTION_KEY_SIZE:
            return "INSUFFICIENT_ENCRYPTION_KEY_SIZE";
        case AUTH_CALLBACK_REPLY_ATTERR_INVALID_ATTRIBUTE_VALUE_LENGTH:
            return "INVALID_ATTRIBUTE_VALUE_LENGTH";
        case AUTH_CALLBACK_REPLY_ATTERR_UNLIKELY_ERROR:
            return "UNLIKELY_ERROR";
        case AUTH_CALLBACK_REPLY_ATTERR_INSUFFICIENT_ENCRYPTION:
            return "INSUFFICIENT_ENCRYPTION";
        case AUTH_CALLBACK_REPLY_ATTERR_UNSUPPORTED_GROUP_TYPE:
            return "UNSUPPORTED_GROUP_TYPE";
        case AUTH_CALLBACK_REPLY_ATTERR_INSUFFICIENT_RESOURCES:
            return "INSUFFICIENT_RESOURCES";
        case AUTH_CALLBACK_REPLY_ATTERR_WRITE_REQUEST_REJECTED:
            return "WRITE_REQUEST_REJECTED";
        case AUTH_CALLBACK_REPLY_ATTERR_CLIENT_CHARACTERISTIC_CONFIGURATION_DESCRIPTOR_IMPROPERLY_CONFIGURED:
            return "CLIENT_CHARACTERISTIC_CONFIGURATION_DESCRIPTOR_IMPROPERLY_CONFIGURED";
        case AUTH_CALLBACK_REPLY_ATTERR_PROCEDURE_ALREADY_IN_PROGRESS:
            return "PROCEDURE_ALREADY_IN_PROGRESS";
        case AUTH_CALLBACK_REPLY_ATTERR_OUT_OF_RANGE:
            return "OUT_OF_RANGE";
        default:
            return "unknown";
    }
}


} // namespace ble
