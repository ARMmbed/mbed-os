/*************************************************************************************************/
/*!
 *  \file   hci_defs.h
 *        
 *  \brief  HCI constants and definitions from the Bluetooth specification.
 *
 *          $Date: 2012-06-26 21:53:53 -0700 (Tue, 26 Jun 2012) $
 *          $Revision: 337 $
 *  
 *  Copyright (c) 2009-2016 ARM Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  Licensed under the Permissive Binary License, Version 1.0 (the "License"); you may not use
 *  this file except in compliance with the License.  You may obtain a copy of the License at
 *
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and limitations under the License.
 */
/*************************************************************************************************/
#ifndef HCI_DEFS_H
#define HCI_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/*! Packet definitions */
#define HCI_CMD_HDR_LEN                   3       /*! Command packet header length */
#define HCI_ACL_HDR_LEN                   4       /*! ACL packet header length */
#define HCI_EVT_HDR_LEN                   2       /*! Event packet header length */
#define HCI_EVT_PARAM_MAX_LEN             255     /*! Maximum length of event packet parameters */
#define HCI_PB_FLAG_MASK                  0x1000  /*! ACL packet boundary flag mask */
#define HCI_PB_START                      0x0000  /*! Packet boundary flag, start */
#define HCI_PB_CONTINUE                   0x1000  /*! Packet boundary flag, continue */
#define HCI_HANDLE_MASK                   0x0FFF  /*! Mask for handle bits in ACL packet */
#define HCI_HANDLE_NONE                   0xFFFF  /*! Value for invalid handle */

/*! Packet types */
#define HCI_CMD_TYPE                      1       /*! HCI command packet */
#define HCI_ACL_TYPE                      2       /*! HCI ACL data packet */
#define HCI_EVT_TYPE                      4       /*! HCI event packet */

/*! Error codes */
#define HCI_SUCCESS                       0x00    /*! Success */
#define HCI_ERR_UNKNOWN_CMD               0x01    /*! Unknown HCI command */
#define HCI_ERR_UNKNOWN_HANDLE            0x02    /*! Unknown connection identifier */
#define HCI_ERR_HARDWARE_FAILURE          0x03    /*! Hardware failure */
#define HCI_ERR_PAGE_TIMEOUT              0x04    /*! Page timeout */
#define HCI_ERR_AUTH_FAILURE              0x05    /*! Authentication failure */
#define HCI_ERR_KEY_MISSING               0x06    /*! PIN or key missing */
#define HCI_ERR_MEMORY_EXCEEDED           0x07    /*! Memory capacity exceeded */
#define HCI_ERR_CONN_TIMEOUT              0x08    /*! Connection timeout */
#define HCI_ERR_CONN_LIMIT                0x09    /*! Connection limit exceeded */
#define HCI_ERR_SYNCH_CONN_LIMIT          0x0A    /*! Synchronous connection limit exceeded */
#define HCI_ERR_ACL_CONN_EXISTS           0x0B    /*! ACL connection already exists */
#define HCI_ERR_CMD_DISALLOWED            0x0C    /*! Command disallowed */
#define HCI_ERR_REJ_RESOURCES             0x0D    /*! Connection rejected limited resources */
#define HCI_ERR_REJ_SECURITY              0x0E    /*! Connection rejected security reasons */
#define HCI_ERR_REJ_BD_ADDR               0x0F    /*! Connection rejected unacceptable BD_ADDR */
#define HCI_ERR_ACCEPT_TIMEOUT            0x10    /*! Connection accept timeout exceeded */
#define HCI_ERR_UNSUP_FEAT                0x11    /*! Unsupported feature or parameter value */
#define HCI_ERR_INVALID_PARAM             0x12    /*! Invalid HCI command parameters */
#define HCI_ERR_REMOTE_TERMINATED         0x13    /*! Remote user terminated connection */
#define HCI_ERR_REMOTE_RESOURCES          0x14    /*! Remote device low resources */
#define HCI_ERR_REMOTE_POWER_OFF          0x15    /*! Remote device power off */
#define HCI_ERR_LOCAL_TERMINATED          0x16    /*! Connection terminated by local host */
#define HCI_ERR_REPEATED_ATTEMPTS         0x17    /*! Repeated attempts */
#define HCI_ERR_PAIRING_NOT_ALLOWED       0x18    /*! Pairing not allowed */
#define HCI_ERR_UNKNOWN_LMP_PDU           0x19    /*! Unknown LMP PDU */
#define HCI_ERR_UNSUP_REMOTE_FEAT         0x1A    /*! Unsupported remote feature */
#define HCI_ERR_SCO_OFFSET                0x1B    /*! SCO offset rejected */
#define HCI_ERR_SCO_INTERVAL              0x1C    /*! SCO interval rejected */
#define HCI_ERR_SCO_MODE                  0x1D    /*! SCO air mode rejected */
#define HCI_ERR_LMP_PARAM                 0x1E    /*! Invalid LMP parameters */
#define HCI_ERR_UNSPECIFIED               0x1F    /*! Unspecified error */
#define HCI_ERR_UNSUP_LMP_PARAM           0x20    /*! Unsupported LMP parameter value */
#define HCI_ERR_ROLE_CHANGE               0x21    /*! Role change not allowed */
#define HCI_ERR_LL_RESP_TIMEOUT           0x22    /*! LL response timeout */
#define HCI_ERR_LMP_COLLISION             0x23    /*! LMP error transaction collision */
#define HCI_ERR_LMP_PDU                   0x24    /*! LMP pdu not allowed */
#define HCI_ERR_ENCRYPT_MODE              0x25    /*! Encryption mode not acceptable */
#define HCI_ERR_LINK_KEY                  0x26    /*! Link key can not be changed */
#define HCI_ERR_UNSUP_QOS                 0x27    /*! Requested qos not supported */
#define HCI_ERR_INSTANT_PASSED            0x28    /*! Instant passed */
#define HCI_ERR_UNSUP_UNIT_KEY            0x29    /*! Pairing with unit key not supported */
#define HCI_ERR_TRANSACT_COLLISION        0x2A    /*! Different transaction collision */
#define HCI_ERR_CHANNEL_CLASS             0x2E    /*! Channel classification not supported */
#define HCI_ERR_MEMORY                    0x2F    /*! Insufficient security */
#define HCI_ERR_PARAMETER_RANGE           0x30    /*! Parameter out of mandatory range */
#define HCI_ERR_ROLE_SWITCH_PEND          0x32    /*! Role switch pending */
#define HCI_ERR_RESERVED_SLOT             0x34    /*! Reserved slot violation */
#define HCI_ERR_ROLE_SWITCH               0x35    /*! Role switch failed */
#define HCI_ERR_INQ_TOO_LARGE             0x36    /*! Extended inquiry response too large */
#define HCI_ERR_UNSUP_SSP                 0x37    /*! Secure simple pairing not supported by host */
#define HCI_ERR_HOST_BUSY_PAIRING         0x38    /*! Host busy - pairing */
#define HCI_ERR_NO_CHANNEL                0x39    /*! Connection rejected no suitable channel */
#define HCI_ERR_CONTROLLER_BUSY           0x3A    /*! Controller busy */
#define HCI_ERR_CONN_INTERVAL             0x3B    /*! Unacceptable connection interval */
#define HCI_ERR_ADV_TIMEOUT               0x3C    /*! Directed advertising timeout */
#define HCI_ERR_MIC_FAILURE               0x3D    /*! Connection terminated due to MIC failure */
#define HCI_ERR_CONN_FAIL                 0x3E    /*! Connection failed to be established */
#define HCI_ERR_MAC_CONN_FAIL             0x3F    /*! MAC connection failed */

/*! Command groups */
#define HCI_OGF_NOP                       0x00    /*! No operation */
#define HCI_OGF_LINK_CONTROL              0x01    /*! Link control */
#define HCI_OGF_LINK_POLICY               0x02    /*! Link policy */
#define HCI_OGF_CONTROLLER                0x03    /*! Controller and baseband */
#define HCI_OGF_INFORMATIONAL             0x04    /*! Informational parameters */
#define HCI_OGF_STATUS                    0x05    /*! Status parameters */
#define HCI_OGF_TESTING                   0x06    /*! Testing */
#define HCI_OGF_LE_CONTROLLER             0x08    /*! LE controller */
#define HCI_OGF_VENDOR_SPEC               0x3F    /*! Vendor specific */

/*! NOP command */
#define HCI_OCF_NOP                       0x00

/*! Link control commands */
#define HCI_OCF_DISCONNECT                0x06
#define HCI_OCF_READ_REMOTE_VER_INFO      0x1D

/*! Link policy commands (none used for LE) */

/*! Controller and baseband commands */
#define HCI_OCF_SET_EVENT_MASK            0x01
#define HCI_OCF_RESET                     0x03
#define HCI_OCF_READ_TX_PWR_LVL           0x2D
#define HCI_OCF_SET_CONTROLLER_TO_HOST_FC 0x31
#define HCI_OCF_HOST_BUFFER_SIZE          0x33
#define HCI_OCF_HOST_NUM_CMPL_PKTS        0x35

/*! Informational commands */
#define HCI_OCF_READ_LOCAL_VER_INFO       0x01
#define HCI_OCF_READ_LOCAL_SUP_CMDS       0x02
#define HCI_OCF_READ_LOCAL_SUP_FEAT       0x03
#define HCI_OCF_READ_BUF_SIZE             0x05
#define HCI_OCF_READ_BD_ADDR              0x09

/*! Status commands */
#define HCI_OCF_READ_RSSI                 0x05

/*! LE controller commands */
#define HCI_OCF_LE_SET_EVENT_MASK         0x01
#define HCI_OCF_LE_READ_BUF_SIZE          0x02
#define HCI_OCF_LE_READ_LOCAL_SUP_FEAT    0x03
#define HCI_OCF_LE_SET_RAND_ADDR          0x05
#define HCI_OCF_LE_SET_ADV_PARAM          0x06
#define HCI_OCF_LE_READ_ADV_TX_POWER      0x07
#define HCI_OCF_LE_SET_ADV_DATA           0x08
#define HCI_OCF_LE_SET_SCAN_RESP_DATA     0x09
#define HCI_OCF_LE_SET_ADV_ENABLE         0x0A
#define HCI_OCF_LE_SET_SCAN_PARAM         0x0B
#define HCI_OCF_LE_SET_SCAN_ENABLE        0x0C
#define HCI_OCF_LE_CREATE_CONN            0x0D
#define HCI_OCF_LE_CREATE_CONN_CANCEL     0x0E
#define HCI_OCF_LE_READ_WHITE_LIST_SIZE   0x0F
#define HCI_OCF_LE_CLEAR_WHITE_LIST       0x10
#define HCI_OCF_LE_ADD_DEV_WHITE_LIST     0x11
#define HCI_OCF_LE_REMOVE_DEV_WHITE_LIST  0x12
#define HCI_OCF_LE_CONN_UPDATE            0x13
#define HCI_OCF_LE_SET_HOST_CHAN_CLASS    0x14
#define HCI_OCF_LE_READ_CHAN_MAP          0x15
#define HCI_OCF_LE_READ_REMOTE_FEAT       0x16
#define HCI_OCF_LE_ENCRYPT                0x17
#define HCI_OCF_LE_RAND                   0x18
#define HCI_OCF_LE_START_ENCRYPTION       0x19
#define HCI_OCF_LE_LTK_REQ_REPL           0x1A
#define HCI_OCF_LE_LTK_REQ_NEG_REPL       0x1B
#define HCI_OCF_LE_READ_SUP_STATES        0x1C
#define HCI_OCF_LE_RECEIVER_TEST          0x1D
#define HCI_OCF_LE_TRANSMITTER_TEST       0x1E
#define HCI_OCF_LE_TEST_END               0x1F

/*! Opcode manipulation macros */
#define HCI_OPCODE(ogf, ocf)              (((ogf) << 10) + (ocf))
#define HCI_OGF(opcode)                   ((opcode) >> 10)
#define HCI_OCF(opcode)                   ((opcode) & 0x03FF)

/*! Command opcodes */
#define HCI_OPCODE_NOP                       HCI_OPCODE(HCI_OGF_NOP, HCI_OCF_NOP)

#define HCI_OPCODE_DISCONNECT                HCI_OPCODE(HCI_OGF_LINK_CONTROL, HCI_OCF_DISCONNECT)
#define HCI_OPCODE_READ_REMOTE_VER_INFO      HCI_OPCODE(HCI_OGF_LINK_CONTROL, HCI_OCF_READ_REMOTE_VER_INFO)

#define HCI_OPCODE_SET_EVENT_MASK            HCI_OPCODE(HCI_OGF_CONTROLLER, HCI_OCF_SET_EVENT_MASK)
#define HCI_OPCODE_RESET                     HCI_OPCODE(HCI_OGF_CONTROLLER, HCI_OCF_RESET)
#define HCI_OPCODE_READ_TX_PWR_LVL           HCI_OPCODE(HCI_OGF_CONTROLLER, HCI_OCF_READ_TX_PWR_LVL)
#define HCI_OPCODE_SET_CONTROLLER_TO_HOST_FC HCI_OPCODE(HCI_OGF_CONTROLLER, HCI_OCF_SET_CONTROLLER_TO_HOST_FC)
#define HCI_OPCODE_HOST_BUFFER_SIZE          HCI_OPCODE(HCI_OGF_CONTROLLER, HCI_OCF_HOST_BUFFER_SIZE)
#define HCI_OPCODE_HOST_NUM_CMPL_PKTS        HCI_OPCODE(HCI_OGF_CONTROLLER, HCI_OCF_HOST_NUM_CMPL_PKTS)

#define HCI_OPCODE_READ_LOCAL_VER_INFO       HCI_OPCODE(HCI_OGF_INFORMATIONAL, HCI_OCF_READ_LOCAL_VER_INFO)
#define HCI_OPCODE_READ_LOCAL_SUP_CMDS       HCI_OPCODE(HCI_OGF_INFORMATIONAL, HCI_OCF_READ_LOCAL_SUP_CMDS)
#define HCI_OPCODE_READ_LOCAL_SUP_FEAT       HCI_OPCODE(HCI_OGF_INFORMATIONAL, HCI_OCF_READ_LOCAL_SUP_FEAT)
#define HCI_OPCODE_READ_BUF_SIZE             HCI_OPCODE(HCI_OGF_INFORMATIONAL, HCI_OCF_READ_BUF_SIZE)
#define HCI_OPCODE_READ_BD_ADDR              HCI_OPCODE(HCI_OGF_INFORMATIONAL, HCI_OCF_READ_BD_ADDR)

#define HCI_OPCODE_READ_RSSI                 HCI_OPCODE(HCI_OGF_STATUS, HCI_OCF_READ_RSSI)

#define HCI_OPCODE_LE_SET_EVENT_MASK         HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_EVENT_MASK)
#define HCI_OPCODE_LE_READ_BUF_SIZE          HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_BUF_SIZE)
#define HCI_OPCODE_LE_READ_LOCAL_SUP_FEAT    HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_LOCAL_SUP_FEAT)
#define HCI_OPCODE_LE_SET_RAND_ADDR          HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_RAND_ADDR)
#define HCI_OPCODE_LE_SET_ADV_PARAM          HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_ADV_PARAM)
#define HCI_OPCODE_LE_READ_ADV_TX_POWER      HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_ADV_TX_POWER)
#define HCI_OPCODE_LE_SET_ADV_DATA           HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_ADV_DATA)
#define HCI_OPCODE_LE_SET_SCAN_RESP_DATA     HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_SCAN_RESP_DATA)
#define HCI_OPCODE_LE_SET_ADV_ENABLE         HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_ADV_ENABLE)
#define HCI_OPCODE_LE_SET_SCAN_PARAM         HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_SCAN_PARAM)
#define HCI_OPCODE_LE_SET_SCAN_ENABLE        HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_SCAN_ENABLE)
#define HCI_OPCODE_LE_CREATE_CONN            HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_CREATE_CONN)
#define HCI_OPCODE_LE_CREATE_CONN_CANCEL     HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_CREATE_CONN_CANCEL)
#define HCI_OPCODE_LE_READ_WHITE_LIST_SIZE   HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_WHITE_LIST_SIZE)
#define HCI_OPCODE_LE_CLEAR_WHITE_LIST       HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_CLEAR_WHITE_LIST)
#define HCI_OPCODE_LE_ADD_DEV_WHITE_LIST     HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_ADD_DEV_WHITE_LIST)
#define HCI_OPCODE_LE_REMOVE_DEV_WHITE_LIST  HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_REMOVE_DEV_WHITE_LIST)
#define HCI_OPCODE_LE_CONN_UPDATE            HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_CONN_UPDATE)
#define HCI_OPCODE_LE_SET_HOST_CHAN_CLASS    HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_HOST_CHAN_CLASS)
#define HCI_OPCODE_LE_READ_CHAN_MAP          HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_CHAN_MAP)
#define HCI_OPCODE_LE_READ_REMOTE_FEAT       HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_REMOTE_FEAT)
#define HCI_OPCODE_LE_ENCRYPT                HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_ENCRYPT)
#define HCI_OPCODE_LE_RAND                   HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_RAND)
#define HCI_OPCODE_LE_START_ENCRYPTION       HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_START_ENCRYPTION)
#define HCI_OPCODE_LE_LTK_REQ_REPL           HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_LTK_REQ_REPL)
#define HCI_OPCODE_LE_LTK_REQ_NEG_REPL       HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_LTK_REQ_NEG_REPL)
#define HCI_OPCODE_LE_READ_SUP_STATES        HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_SUP_STATES)
#define HCI_OPCODE_LE_RECEIVER_TEST          HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_RECEIVER_TEST)
#define HCI_OPCODE_LE_TRANSMITTER_TEST       HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_TRANSMITTER_TEST)
#define HCI_OPCODE_LE_TEST_END               HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_TEST_END)

/*! Command parameter lengths */
#define HCI_LEN_NOP                       0

#define HCI_LEN_DISCONNECT                3
#define HCI_LEN_READ_REMOTE_VER_INFO      2

#define HCI_LEN_SET_EVENT_MASK            8
#define HCI_LEN_RESET                     0
#define HCI_LEN_READ_TX_PWR_LVL           3
#define HCI_LEN_SET_CONTROLLER_TO_HOST_FC 1
#define HCI_LEN_HOST_BUFFER_SIZE          8
#define HCI_LEN_HOST_NUM_CMPL_PKTS        1

#define HCI_LEN_READ_LOCAL_VER_INFO       0
#define HCI_LEN_READ_LOCAL_SUP_CMDS       0
#define HCI_LEN_READ_LOCAL_SUP_FEAT       0
#define HCI_LEN_READ_BUF_SIZE             0
#define HCI_LEN_READ_BD_ADDR              0

#define HCI_LEN_READ_RSSI                 2

#define HCI_LEN_LE_SET_EVENT_MASK         8
#define HCI_LEN_LE_READ_BUF_SIZE          0
#define HCI_LEN_LE_READ_LOCAL_SUP_FEAT    0
#define HCI_LEN_LE_SET_RAND_ADDR          6
#define HCI_LEN_LE_SET_ADV_PARAM          15
#define HCI_LEN_LE_READ_ADV_TX_POWER      0
#define HCI_LEN_LE_SET_ADV_DATA           32
#define HCI_LEN_LE_SET_SCAN_RESP_DATA     32
#define HCI_LEN_LE_SET_ADV_ENABLE         1
#define HCI_LEN_LE_SET_SCAN_PARAM         7
#define HCI_LEN_LE_SET_SCAN_ENABLE        2
#define HCI_LEN_LE_CREATE_CONN            25
#define HCI_LEN_LE_CREATE_CONN_CANCEL     0
#define HCI_LEN_LE_READ_WHITE_LIST_SIZE   0
#define HCI_LEN_LE_CLEAR_WHITE_LIST       0
#define HCI_LEN_LE_ADD_DEV_WHITE_LIST     7
#define HCI_LEN_LE_REMOVE_DEV_WHITE_LIST  7
#define HCI_LEN_LE_CONN_UPDATE            14
#define HCI_LEN_LE_SET_HOST_CHAN_CLASS    5
#define HCI_LEN_LE_READ_CHAN_MAP          2
#define HCI_LEN_LE_READ_REMOTE_FEAT       2
#define HCI_LEN_LE_ENCRYPT                32
#define HCI_LEN_LE_RAND                   0
#define HCI_LEN_LE_START_ENCRYPTION       28
#define HCI_LEN_LE_LTK_REQ_REPL           18
#define HCI_LEN_LE_LTK_REQ_NEG_REPL       2
#define HCI_LEN_LE_READ_SUP_STATES        0
#define HCI_LEN_LE_RECEIVER_TEST          1
#define HCI_LEN_LE_TRANSMITTER_TEST       3
#define HCI_LEN_LE_TEST_END               0

/*! Events */
#define HCI_DISCONNECT_CMPL_EVT           0x05
#define HCI_ENC_CHANGE_EVT                0x08
#define HCI_READ_REMOTE_VER_INFO_CMPL_EVT 0x0C
#define HCI_CMD_CMPL_EVT                  0x0E
#define HCI_CMD_STATUS_EVT                0x0F
#define HCI_HW_ERROR_EVT                  0x10
#define HCI_NUM_CMPL_PKTS_EVT             0x13
#define HCI_DATA_BUF_OVERFLOW_EVT         0x1A
#define HCI_ENC_KEY_REFRESH_CMPL_EVT      0x30
#define HCI_LE_META_EVT                   0x3E
#define HCI_VENDOR_SPEC_EVT               0xFF

/*! LE Subevents */
#define HCI_LE_CONN_CMPL_EVT              0x01
#define HCI_LE_ADV_REPORT_EVT             0x02
#define HCI_LE_CONN_UPDATE_CMPL_EVT       0x03
#define HCI_LE_READ_REMOTE_FEAT_CMPL_EVT  0x04
#define HCI_LE_LTK_REQ_EVT                0x05

/*! Event parameter lengths */
#define HCI_LEN_DISCONNECT_CMPL           4
#define HCI_LEN_ENC_CHANGE                5
#define HCI_LEN_LE_CONN_CMPL              19
#define HCI_LEN_LE_CONN_UPDATE_CMPL       9
#define HCI_LEN_LE_READ_REMOTE_FEAT_CMPL  12
#define HCI_LEN_LE_LTK_REQ                13

/*! Supported commands */
#define HCI_SUP_DISCONNECT                0x20      /*! Byte 0 */
#define HCI_SUP_READ_REMOTE_VER_INFO      0x80      /*! Byte 2 */
#define HCI_SUP_SET_EVENT_MASK            0x40      /*! Byte 5 */
#define HCI_SUP_RESET                     0x80      /*! Byte 5 */
#define HCI_SUP_READ_TX_PWR_LVL           0x04      /*! Byte 10 */
#define HCI_SUP_SET_CONTROLLER_TO_HOST_FC 0x20      /*! Byte 10 */
#define HCI_SUP_HOST_BUFFER_SIZE          0x40      /*! Byte 10 */
#define HCI_SUP_HOST_NUM_CMPL_PKTS        0x80      /*! Byte 10 */
#define HCI_SUP_READ_LOCAL_VER_INFO       0x08      /*! Byte 14 */
#define HCI_SUP_READ_LOCAL_SUP_FEAT       0x20      /*! Byte 14 */
#define HCI_SUP_READ_BD_ADDR              0x02      /*! Byte 15 */
#define HCI_SUP_READ_RSSI                 0x20      /*! Byte 15 */
#define HCI_SUP_LE_SET_EVENT_MASK         0x01      /*! Byte 25 */
#define HCI_SUP_LE_READ_BUF_SIZE          0x02      /*! Byte 25 */
#define HCI_SUP_LE_READ_LOCAL_SUP_FEAT    0x04      /*! Byte 25 */
#define HCI_SUP_LE_SET_RAND_ADDR          0x10      /*! Byte 25 */
#define HCI_SUP_LE_SET_ADV_PARAM          0x20      /*! Byte 25 */
#define HCI_SUP_LE_READ_ADV_TX_POWER      0x40      /*! Byte 25 */
#define HCI_SUP_LE_SET_ADV_DATA           0x80      /*! Byte 25 */
#define HCI_SUP_LE_SET_SCAN_RESP_DATA     0x01      /*! Byte 26 */
#define HCI_SUP_LE_SET_ADV_ENABLE         0x02      /*! Byte 26 */
#define HCI_SUP_LE_SET_SCAN_PARAM         0x04      /*! Byte 26 */
#define HCI_SUP_LE_SET_SCAN_ENABLE        0x08      /*! Byte 26 */
#define HCI_SUP_LE_CREATE_CONN            0x10      /*! Byte 26 */
#define HCI_SUP_LE_CREATE_CONN_CANCEL     0x20      /*! Byte 26 */
#define HCI_SUP_LE_READ_WHITE_LIST_SIZE   0x40      /*! Byte 26 */
#define HCI_SUP_LE_CLEAR_WHITE_LIST       0x80      /*! Byte 26 */
#define HCI_SUP_LE_ADD_DEV_WHITE_LIST     0x01      /*! Byte 27 */
#define HCI_SUP_LE_REMOVE_DEV_WHITE_LIST  0x02      /*! Byte 27 */
#define HCI_SUP_LE_CONN_UPDATE            0x04      /*! Byte 27 */
#define HCI_SUP_LE_SET_HOST_CHAN_CLASS    0x08      /*! Byte 27 */
#define HCI_SUP_LE_READ_CHAN_MAP          0x10      /*! Byte 27 */
#define HCI_SUP_LE_READ_REMOTE_FEAT       0x20      /*! Byte 27 */
#define HCI_SUP_LE_ENCRYPT                0x40      /*! Byte 27 */
#define HCI_SUP_LE_RAND                   0x80      /*! Byte 27 */
#define HCI_SUP_LE_START_ENCRYPTION       0x01      /*! Byte 28 */
#define HCI_SUP_LE_LTK_REQ_REPL           0x02      /*! Byte 28 */
#define HCI_SUP_LE_LTK_REQ_NEG_REPL       0x04      /*! Byte 28 */
#define HCI_SUP_LE_READ_SUP_STATES        0x08      /*! Byte 28 */
#define HCI_SUP_LE_RECEIVER_TEST          0x10      /*! Byte 28 */
#define HCI_SUP_LE_TRANSMITTER_TEST       0x20      /*! Byte 28 */
#define HCI_SUP_LE_TEST_END               0x40      /*! Byte 28 */

/*! Event mask */
#define HCI_EVT_MASK_DISCONNECT_CMPL            0x10   /*! Byte 0 */
#define HCI_EVT_MASK_ENC_CHANGE                 0x80   /*! Byte 0 */
#define HCI_EVT_MASK_READ_REMOTE_VER_INFO_CMPL  0x08   /*! Byte 1 */
#define HCI_EVT_MASK_HW_ERROR                   0x80   /*! Byte 1 */
#define HCI_EVT_MASK_DATA_BUF_OVERFLOW          0x02   /*! Byte 3 */
#define HCI_EVT_MASK_ENC_KEY_REFRESH_CMPL       0x80   /*! Byte 5 */
#define HCI_EVT_MASK_LE_META                    0x20   /*! Byte 7 */

/*! LE event mask */
#define HCI_EVT_MASK_LE_CONN_CMPL_EVT              0x01   /*! Byte 0 */
#define HCI_EVT_MASK_LE_ADV_REPORT_EVT             0x02   /*! Byte 0 */
#define HCI_EVT_MASK_LE_CONN_UPDATE_CMPL_EVT       0x04   /*! Byte 0 */
#define HCI_EVT_MASK_LE_READ_REMOTE_FEAT_CMPL_EVT  0x08   /*! Byte 0 */
#define HCI_EVT_MASK_LE_LTK_REQ_EVT                0x10   /*! Byte 0 */

/*! LE supported features */
#define HCI_LE_SUP_FEAT_ENCRYPTION        0x01

/*! Advertising command parameters */
#define HCI_ADV_MIN_INTERVAL              0x0020    /*! Minimum advertising interval */
#define HCI_ADV_NONCONN_MIN_INTERVAL      0x00A0    /*! Minimum nonconnectable adv. interval */
#define HCI_ADV_MAX_INTERVAL              0x4000    /*! Maximum advertising interval */
#define HCI_ADV_TYPE_CONN_UNDIRECT        0x00      /*! Connectable undirected advertising */
#define HCI_ADV_TYPE_CONN_DIRECT          0x01      /*! Connectable directed advertising */
#define HCI_ADV_TYPE_DISC_UNDIRECT        0x02      /*! Discoverable undirected advertising */
#define HCI_ADV_TYPE_NONCONN_UNDIRECT     0x03      /*! Nonconnectable undirected advertising */
#define HCI_ADV_CHAN_37                   0x01      /*! Advertising channel 37 */
#define HCI_ADV_CHAN_38                   0x02      /*! Advertising channel 38 */
#define HCI_ADV_CHAN_39                   0x04      /*! Advertising channel 39 */
#define HCI_ADV_FILT_NONE                 0x00      /*! No scan request or connection filtering */
#define HCI_ADV_FILT_SCAN                 0x01      /*! White list filters scan requests */
#define HCI_ADV_FILT_CONN                 0x02      /*! White list filters connections */
#define HCI_ADV_FILT_ALL                  0x03      /*! White list filters scan req. and conn. */

/*! Scan command parameters */
#define HCI_SCAN_TYPE_PASSIVE             0         /*! Passive scan */
#define HCI_SCAN_TYPE_ACTIVE              1         /*! Active scan */
#define HCI_SCAN_INTERVAL_MIN             0x0004    /*! Minimum scan interval */
#define HCI_SCAN_INTERVAL_MAX             0x4000    /*! Maximum scan interval */
#define HCI_SCAN_INTERVAL_DEFAULT         0x0010    /*! Default scan interval */
#define HCI_SCAN_WINDOW_MIN               0x0004    /*! Minimum scan window */
#define HCI_SCAN_WINDOW_MAX               0x4000    /*! Maximum scan window */
#define HCI_SCAN_WINDOW_DEFAULT           0x0010    /*! Default scan window */

/*! Connection command parameters */
#define HCI_CONN_INTERVAL_MIN             0x0006    /*! Minimum connection interval */
#define HCI_CONN_INTERVAL_MAX             0x0C80    /*! Maximum connection interval */
#define HCI_CONN_LATENCY_MAX              0x01F3    /*! Maximum connection latency */
#define HCI_SUP_TIMEOUT_MIN               0x000A    /*! Minimum supervision timeout */
#define HCI_SUP_TIMEOUT_MAX               0x0C80    /*! Maximum supervision timeout */

/*! Connection event parameters */
#define HCI_ROLE_MASTER                   0         /*! Role is master */
#define HCI_ROLE_SLAVE                    1         /*! Role is slave */
#define HCI_CLOCK_500PPM                  0x00      /*! 500 ppm clock accuracy */
#define HCI_CLOCK_250PPM                  0x01      /*! 250 ppm clock accuracy */
#define HCI_CLOCK_150PPM                  0x02      /*! 150 ppm clock accuracy */
#define HCI_CLOCK_100PPM                  0x03      /*! 100 ppm clock accuracy */
#define HCI_CLOCK_75PPM                   0x04      /*! 75 ppm clock accuracy */
#define HCI_CLOCK_50PPM                   0x05      /*! 50 ppm clock accuracy */
#define HCI_CLOCK_30PPM                   0x06      /*! 30 ppm clock accuracy */
#define HCI_CLOCK_20PPM                   0x07      /*! 20 ppm clock accuracy */

/*! Advertising report event parameters */
#define HCI_ADV_CONN_UNDIRECT             0x00      /*! Connectable undirected advertising */
#define HCI_ADV_CONN_DIRECT               0x01      /*! Connectable directed advertising */
#define HCI_ADV_DISC_UNDIRECT             0x02      /*! Discoverable undirected advertising */
#define HCI_ADV_NONCONN_UNDIRECT          0x03      /*! Non-connectable undirected advertising */
#define HCI_ADV_SCAN_RESPONSE             0x04      /*! Scan response */

/*! Misc command parameters */
#define HCI_READ_TX_PWR_CURRENT           0         /*! Read current tx power */
#define HCI_READ_TX_PWR_MAX               1         /*! Read maximum tx power */
#define HCI_TX_PWR_MIN                    -30       /*! Minimum tx power dBm */
#define HCI_TX_PWR_MAX                    20        /*! Maximum tx power dBm */
#define HCI_VERSION                       6         /*! HCI specification version */
#define HCI_RSSI_MIN                      -127      /*! Minimum RSSI dBm */
#define HCI_RSSI_MAX                      20        /*! Maximum RSSI dBm */
#define HCI_ADDR_TYPE_PUBLIC              0         /*! Public device address */
#define HCI_ADDR_TYPE_RANDOM              1         /*! Random device address */
#define HCI_FILT_NONE                     0         /*! No white list filtering */
#define HCI_FILT_WHITE_LIST               1         /*! White list filtering */
#define HCI_ROLE_MASTER                   0         /*! Role is master */
#define HCI_ROLE_SLAVE                    1         /*! Role is slave */

/*! Parameter lengths */
#define HCI_EVT_MASK_LEN                  8         /*! Length of event mask byte array */
#define HCI_LE_EVT_MASK_LEN               8         /*! Length of LE event mask byte array */
#define HCI_FEAT_LEN                      8         /*! Length of features byte array */
#define HCI_ADV_DATA_LEN                  31        /*! Length of advertising data */
#define HCI_SCAN_DATA_LEN                 31        /*! Length of scan response data */
#define HCI_CHAN_MAP_LEN                  5         /*! Length of channel map byte array */
#define HCI_KEY_LEN                       16        /*! Length of encryption key */
#define HCI_ENCRYPT_DATA_LEN              16        /*! Length of data used in encryption */
#define HCI_RAND_LEN                      8         /*! Length of random number */
#define HCI_LE_STATES_LEN                 8         /*! Length of LE states byte array */

/*! Wicentric company ID */
#define HCI_ID_WICENTRIC                  0x005F

#ifdef __cplusplus
};
#endif

#endif /* HCI_DEFS_H */
