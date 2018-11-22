/*************************************************************************************************/
/*!
 *  \file   hci_defs.h
 *
 *  \brief  HCI constants and definitions from the Bluetooth specification.
 *
 *  Copyright (c) 2009-2018 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/
#ifndef HCI_DEFS_H
#define HCI_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup STACK_HCI_API
 *  \{ */

/** \name Packet definitions
 *
 */
/**@{*/
#define HCI_CMD_HDR_LEN                              3       /*!< \brief Command packet header length */
#define HCI_ACL_HDR_LEN                              4       /*!< \brief ACL packet header length */
#define HCI_EVT_HDR_LEN                              2       /*!< \brief Event packet header length */
#define HCI_EVT_PARAM_MAX_LEN                        255     /*!< \brief Maximum length of event packet parameters */
#define HCI_ACL_DEFAULT_LEN                          27      /*!< \brief Default maximum ACL packet length */
#define HCI_PB_FLAG_MASK                             0x3000  /*!< \brief ACL packet boundary flag mask */
#define HCI_PB_START_H2C                             0x0000  /*!< \brief Packet boundary flag, start, host-to-controller */
#define HCI_PB_CONTINUE                              0x1000  /*!< \brief Packet boundary flag, continue */
#define HCI_PB_START_C2H                             0x2000  /*!< \brief Packet boundary flag, start, controller-to-host */
#define HCI_HANDLE_MASK                              0x0FFF  /*!< \brief Mask for handle bits in ACL packet */
#define HCI_HANDLE_NONE                              0xFFFF  /*!< \brief Value for invalid handle */
/**@}*/

/** \name Packet types
 *
 */
#define HCI_CMD_TYPE                                 1       /*!< \brief HCI command packet */
#define HCI_ACL_TYPE                                 2       /*!< \brief HCI ACL data packet */
#define HCI_EVT_TYPE                                 4       /*!< \brief HCI event packet */
/**@}*/

/** \name Error codes
 *
 */
/**@{*/
#define HCI_SUCCESS                                  0x00    /*!< \brief Success */
#define HCI_ERR_UNKNOWN_CMD                          0x01    /*!< \brief Unknown HCI command */
#define HCI_ERR_UNKNOWN_HANDLE                       0x02    /*!< \brief Unknown connection identifier */
#define HCI_ERR_HARDWARE_FAILURE                     0x03    /*!< \brief Hardware failure */
#define HCI_ERR_PAGE_TIMEOUT                         0x04    /*!< \brief Page timeout */
#define HCI_ERR_AUTH_FAILURE                         0x05    /*!< \brief Authentication failure */
#define HCI_ERR_KEY_MISSING                          0x06    /*!< \brief PIN or key missing */
#define HCI_ERR_MEMORY_EXCEEDED                      0x07    /*!< \brief Memory capacity exceeded */
#define HCI_ERR_CONN_TIMEOUT                         0x08    /*!< \brief Connection timeout */
#define HCI_ERR_CONN_LIMIT                           0x09    /*!< \brief Connection limit exceeded */
#define HCI_ERR_SYNCH_CONN_LIMIT                     0x0A    /*!< \brief Synchronous connection limit exceeded */
#define HCI_ERR_ACL_CONN_EXISTS                      0x0B    /*!< \brief ACL connection already exists */
#define HCI_ERR_CMD_DISALLOWED                       0x0C    /*!< \brief Command disallowed */
#define HCI_ERR_REJ_RESOURCES                        0x0D    /*!< \brief Connection rejected limited resources */
#define HCI_ERR_REJ_SECURITY                         0x0E    /*!< \brief Connection rejected security reasons */
#define HCI_ERR_REJ_BD_ADDR                          0x0F    /*!< \brief Connection rejected unacceptable BD_ADDR */
#define HCI_ERR_ACCEPT_TIMEOUT                       0x10    /*!< \brief Connection accept timeout exceeded */
#define HCI_ERR_UNSUP_FEAT                           0x11    /*!< \brief Unsupported feature or parameter value */
#define HCI_ERR_INVALID_PARAM                        0x12    /*!< \brief Invalid HCI command parameters */
#define HCI_ERR_REMOTE_TERMINATED                    0x13    /*!< \brief Remote user terminated connection */
#define HCI_ERR_REMOTE_RESOURCES                     0x14    /*!< \brief Remote device low resources */
#define HCI_ERR_REMOTE_POWER_OFF                     0x15    /*!< \brief Remote device power off */
#define HCI_ERR_LOCAL_TERMINATED                     0x16    /*!< \brief Connection terminated by local host */
#define HCI_ERR_REPEATED_ATTEMPTS                    0x17    /*!< \brief Repeated attempts */
#define HCI_ERR_PAIRING_NOT_ALLOWED                  0x18    /*!< \brief Pairing not allowed */
#define HCI_ERR_UNKNOWN_LMP_PDU                      0x19    /*!< \brief Unknown LMP PDU */
#define HCI_ERR_UNSUP_REMOTE_FEAT                    0x1A    /*!< \brief Unsupported remote feature */
#define HCI_ERR_SCO_OFFSET                           0x1B    /*!< \brief SCO offset rejected */
#define HCI_ERR_SCO_INTERVAL                         0x1C    /*!< \brief SCO interval rejected */
#define HCI_ERR_SCO_MODE                             0x1D    /*!< \brief SCO air mode rejected */
#define HCI_ERR_LMP_PARAM                            0x1E    /*!< \brief Invalid LMP parameters */
#define HCI_ERR_UNSPECIFIED                          0x1F    /*!< \brief Unspecified error */
#define HCI_ERR_UNSUP_LMP_PARAM                      0x20    /*!< \brief Unsupported LMP parameter value */
#define HCI_ERR_ROLE_CHANGE                          0x21    /*!< \brief Role change not allowed */
#define HCI_ERR_LL_RESP_TIMEOUT                      0x22    /*!< \brief LL response timeout */
#define HCI_ERR_LMP_COLLISION                        0x23    /*!< \brief LMP error transaction collision */
#define HCI_ERR_LMP_PDU                              0x24    /*!< \brief LMP pdu not allowed */
#define HCI_ERR_ENCRYPT_MODE                         0x25    /*!< \brief Encryption mode not acceptable */
#define HCI_ERR_LINK_KEY                             0x26    /*!< \brief Link key can not be changed */
#define HCI_ERR_UNSUP_QOS                            0x27    /*!< \brief Requested qos not supported */
#define HCI_ERR_INSTANT_PASSED                       0x28    /*!< \brief Instant passed */
#define HCI_ERR_UNSUP_UNIT_KEY                       0x29    /*!< \brief Pairing with unit key not supported */
#define HCI_ERR_TRANSACT_COLLISION                   0x2A    /*!< \brief Different transaction collision */
#define HCI_ERR_CHANNEL_CLASS                        0x2E    /*!< \brief Channel classification not supported */
#define HCI_ERR_MEMORY                               0x2F    /*!< \brief Insufficient security */
#define HCI_ERR_PARAMETER_RANGE                      0x30    /*!< \brief Parameter out of mandatory range */
#define HCI_ERR_ROLE_SWITCH_PEND                     0x32    /*!< \brief Role switch pending */
#define HCI_ERR_RESERVED_SLOT                        0x34    /*!< \brief Reserved slot violation */
#define HCI_ERR_ROLE_SWITCH                          0x35    /*!< \brief Role switch failed */
#define HCI_ERR_INQ_TOO_LARGE                        0x36    /*!< \brief Extended inquiry response too large */
#define HCI_ERR_UNSUP_SSP                            0x37    /*!< \brief Secure simple pairing not supported by host */
#define HCI_ERR_HOST_BUSY_PAIRING                    0x38    /*!< \brief Host busy - pairing */
#define HCI_ERR_NO_CHANNEL                           0x39    /*!< \brief Connection rejected no suitable channel */
#define HCI_ERR_CONTROLLER_BUSY                      0x3A    /*!< \brief Controller busy */
#define HCI_ERR_CONN_INTERVAL                        0x3B    /*!< \brief Unacceptable connection interval */
#define HCI_ERR_ADV_TIMEOUT                          0x3C    /*!< \brief Advertising timeout */
#define HCI_ERR_MIC_FAILURE                          0x3D    /*!< \brief Connection terminated due to MIC failure */
#define HCI_ERR_CONN_FAIL                            0x3E    /*!< \brief Connection failed to be established */
#define HCI_ERR_MAC_CONN_FAIL                        0x3F    /*!< \brief MAC connection failed */
#define HCI_ERR_COARSE_CLK_ADJ_REJ                   0x40    /*!< \brief Coarse clock adjustment rejected */
#define HCI_ERR_TYPE0_SUBMAP_NOT_DEF                 0x41    /*!< \brief Type0 submap not defined */
#define HCI_ERR_UNKNOWN_ADV_ID                       0x42    /*!< \brief Unknown advertising identifier */
#define HCI_ERR_LIMIT_REACHED                        0x43    /*!< \brief Limit reached */
#define HCI_ERR_OP_CANCELLED_BY_HOST                 0x44    /*!< \brief Operation cancelled by host */
/**@}*/

/** \name Command groups
 *
 */
/**@{*/
#define HCI_OGF_NOP                                  0x00    /*!< \brief No operation */
#define HCI_OGF_LINK_CONTROL                         0x01    /*!< \brief Link control */
#define HCI_OGF_LINK_POLICY                          0x02    /*!< \brief Link policy */
#define HCI_OGF_CONTROLLER                           0x03    /*!< \brief Controller and baseband */
#define HCI_OGF_INFORMATIONAL                        0x04    /*!< \brief Informational parameters */
#define HCI_OGF_STATUS                               0x05    /*!< \brief Status parameters */
#define HCI_OGF_TESTING                              0x06    /*!< \brief Testing */
#define HCI_OGF_LE_CONTROLLER                        0x08    /*!< \brief LE controller */
#define HCI_OGF_VENDOR_SPEC                          0x3F    /*!< \brief Vendor specific */
/**@}*/

/** \name NOP command
 *
 */
/**@{*/
#define HCI_OCF_NOP                                  0x00
/**@}*/

/** \name Link control commands
 *
 */
/**@{*/
#define HCI_OCF_DISCONNECT                           0x06
#define HCI_OCF_READ_REMOTE_VER_INFO                 0x1D
/**@}*/

/*! \brief Link policy commands (none used for LE) */

/** \name Controller and baseband commands
 *
 */
/**@{*/
#define HCI_OCF_SET_EVENT_MASK                       0x01
#define HCI_OCF_RESET                                0x03
#define HCI_OCF_READ_TX_PWR_LVL                      0x2D
#define HCI_OCF_SET_CONTROLLER_TO_HOST_FC            0x31
#define HCI_OCF_HOST_BUFFER_SIZE                     0x33
#define HCI_OCF_HOST_NUM_CMPL_PKTS                   0x35
#define HCI_OCF_SET_EVENT_MASK_PAGE2                 0x63
#define HCI_OCF_READ_AUTH_PAYLOAD_TO                 0x7B
#define HCI_OCF_WRITE_AUTH_PAYLOAD_TO                0x7C
/**@}*/

/** \name Informational commands
 *
 */
/**@{*/
#define HCI_OCF_READ_LOCAL_VER_INFO                  0x01
#define HCI_OCF_READ_LOCAL_SUP_CMDS                  0x02
#define HCI_OCF_READ_LOCAL_SUP_FEAT                  0x03
#define HCI_OCF_READ_BUF_SIZE                        0x05
#define HCI_OCF_READ_BD_ADDR                         0x09
/**@}*/

/** \name Status commands
 *
 */
/**@{*/
#define HCI_OCF_READ_RSSI                            0x05
/**@}*/

/** \name LE controller commands
 *
 */
/**@{*/
#define HCI_OCF_LE_SET_EVENT_MASK                    0x01
#define HCI_OCF_LE_READ_BUF_SIZE                     0x02
#define HCI_OCF_LE_READ_LOCAL_SUP_FEAT               0x03
#define HCI_OCF_LE_SET_RAND_ADDR                     0x05
#define HCI_OCF_LE_SET_ADV_PARAM                     0x06
#define HCI_OCF_LE_READ_ADV_TX_POWER                 0x07
#define HCI_OCF_LE_SET_ADV_DATA                      0x08
#define HCI_OCF_LE_SET_SCAN_RESP_DATA                0x09
#define HCI_OCF_LE_SET_ADV_ENABLE                    0x0A
#define HCI_OCF_LE_SET_SCAN_PARAM                    0x0B
#define HCI_OCF_LE_SET_SCAN_ENABLE                   0x0C
#define HCI_OCF_LE_CREATE_CONN                       0x0D
#define HCI_OCF_LE_CREATE_CONN_CANCEL                0x0E
#define HCI_OCF_LE_READ_WHITE_LIST_SIZE              0x0F
#define HCI_OCF_LE_CLEAR_WHITE_LIST                  0x10
#define HCI_OCF_LE_ADD_DEV_WHITE_LIST                0x11
#define HCI_OCF_LE_REMOVE_DEV_WHITE_LIST             0x12
#define HCI_OCF_LE_CONN_UPDATE                       0x13
#define HCI_OCF_LE_SET_HOST_CHAN_CLASS               0x14
#define HCI_OCF_LE_READ_CHAN_MAP                     0x15
#define HCI_OCF_LE_READ_REMOTE_FEAT                  0x16
#define HCI_OCF_LE_ENCRYPT                           0x17
#define HCI_OCF_LE_RAND                              0x18
#define HCI_OCF_LE_START_ENCRYPTION                  0x19
#define HCI_OCF_LE_LTK_REQ_REPL                      0x1A
#define HCI_OCF_LE_LTK_REQ_NEG_REPL                  0x1B
#define HCI_OCF_LE_READ_SUP_STATES                   0x1C
#define HCI_OCF_LE_RECEIVER_TEST                     0x1D
#define HCI_OCF_LE_TRANSMITTER_TEST                  0x1E
#define HCI_OCF_LE_TEST_END                          0x1F
/*! \brief New in version 4.1 */
#define HCI_OCF_LE_REM_CONN_PARAM_REP                0x20
#define HCI_OCF_LE_REM_CONN_PARAM_NEG_REP            0x21
/*! \brief New in version 4.2 */
#define HCI_OCF_LE_SET_DATA_LEN                      0x22
#define HCI_OCF_LE_READ_DEF_DATA_LEN                 0x23
#define HCI_OCF_LE_WRITE_DEF_DATA_LEN                0x24
#define HCI_OCF_LE_READ_LOCAL_P256_PUB_KEY           0x25
#define HCI_OCF_LE_GENERATE_DHKEY                    0x26
#define HCI_OCF_LE_ADD_DEV_RES_LIST                  0x27
#define HCI_OCF_LE_REMOVE_DEV_RES_LIST               0x28
#define HCI_OCF_LE_CLEAR_RES_LIST                    0x29
#define HCI_OCF_LE_READ_RES_LIST_SIZE                0x2A
#define HCI_OCF_LE_READ_PEER_RES_ADDR                0x2B
#define HCI_OCF_LE_READ_LOCAL_RES_ADDR               0x2C
#define HCI_OCF_LE_SET_ADDR_RES_ENABLE               0x2D
#define HCI_OCF_LE_SET_RES_PRIV_ADDR_TO              0x2E
#define HCI_OCF_LE_READ_MAX_DATA_LEN                 0x2F
/*! \brief New in version 5.0 */
#define HCI_OCF_LE_READ_PHY                          0x30
#define HCI_OCF_LE_SET_DEF_PHY                       0x31
#define HCI_OCF_LE_SET_PHY                           0x32
#define HCI_OCF_LE_ENHANCED_RECEIVER_TEST            0x33
#define HCI_OCF_LE_ENHANCED_TRANSMITTER_TEST         0x34
#define HCI_OCF_LE_SET_ADV_SET_RAND_ADDR             0x35
#define HCI_OCF_LE_SET_EXT_ADV_PARAM                 0x36
#define HCI_OCF_LE_SET_EXT_ADV_DATA                  0x37
#define HCI_OCF_LE_SET_EXT_SCAN_RESP_DATA            0x38
#define HCI_OCF_LE_SET_EXT_ADV_ENABLE                0x39
#define HCI_OCF_LE_READ_MAX_ADV_DATA_LEN             0x3A
#define HCI_OCF_LE_READ_NUM_SUP_ADV_SETS             0x3B
#define HCI_OCF_LE_REMOVE_ADV_SET                    0x3C
#define HCI_OCF_LE_CLEAR_ADV_SETS                    0x3D
#define HCI_OCF_LE_SET_PER_ADV_PARAM                 0x3E
#define HCI_OCF_LE_SET_PER_ADV_DATA                  0x3F
#define HCI_OCF_LE_SET_PER_ADV_ENABLE                0x40
#define HCI_OCF_LE_SET_EXT_SCAN_PARAM                0x41
#define HCI_OCF_LE_SET_EXT_SCAN_ENABLE               0x42
#define HCI_OCF_LE_EXT_CREATE_CONN                   0x43
#define HCI_OCF_LE_PER_ADV_CREATE_SYNC               0x44
#define HCI_OCF_LE_PER_ADV_CREATE_SYNC_CANCEL        0x45
#define HCI_OCF_LE_PER_ADV_TERM_SYNC                 0x46
#define HCI_OCF_LE_ADD_DEV_PER_ADV_LIST              0x47
#define HCI_OCF_LE_REMOVE_DEV_PER_ADV_LIST           0x48
#define HCI_OCF_LE_CLEAR_PER_ADV_LIST                0x49
#define HCI_OCF_LE_READ_PER_ADV_LIST_SIZE            0x4A
#define HCI_OCF_LE_READ_TX_POWER                     0x4B
#define HCI_OCF_LE_READ_RF_PATH_COMP                 0x4C
#define HCI_OCF_LE_WRITE_RF_PATH_COMP                0x4D
#define HCI_OCF_LE_SET_PRIVACY_MODE                  0x4E
/**@}*/

/** \name Opcode manipulation macros
 *
 */
/**@{*/
#define HCI_OPCODE(ogf, ocf)                         (((ogf) << 10) + (ocf))
#define HCI_OGF(opcode)                              ((opcode) >> 10)
#define HCI_OCF(opcode)                              ((opcode) & 0x03FF)
/**@}*/

/** \name Command opcodes
 *
 */
/**@{*/
#define HCI_OPCODE_NOP                               HCI_OPCODE(HCI_OGF_NOP, HCI_OCF_NOP)

#define HCI_OPCODE_DISCONNECT                        HCI_OPCODE(HCI_OGF_LINK_CONTROL, HCI_OCF_DISCONNECT)
#define HCI_OPCODE_READ_REMOTE_VER_INFO              HCI_OPCODE(HCI_OGF_LINK_CONTROL, HCI_OCF_READ_REMOTE_VER_INFO)

#define HCI_OPCODE_SET_EVENT_MASK                    HCI_OPCODE(HCI_OGF_CONTROLLER, HCI_OCF_SET_EVENT_MASK)
#define HCI_OPCODE_RESET                             HCI_OPCODE(HCI_OGF_CONTROLLER, HCI_OCF_RESET)
#define HCI_OPCODE_READ_TX_PWR_LVL                   HCI_OPCODE(HCI_OGF_CONTROLLER, HCI_OCF_READ_TX_PWR_LVL)
#define HCI_OPCODE_SET_EVENT_MASK_PAGE2              HCI_OPCODE(HCI_OGF_CONTROLLER, HCI_OCF_SET_EVENT_MASK_PAGE2)
#define HCI_OPCODE_READ_AUTH_PAYLOAD_TO              HCI_OPCODE(HCI_OGF_CONTROLLER, HCI_OCF_READ_AUTH_PAYLOAD_TO)
#define HCI_OPCODE_WRITE_AUTH_PAYLOAD_TO             HCI_OPCODE(HCI_OGF_CONTROLLER, HCI_OCF_WRITE_AUTH_PAYLOAD_TO)

#define HCI_OPCODE_READ_LOCAL_VER_INFO               HCI_OPCODE(HCI_OGF_INFORMATIONAL, HCI_OCF_READ_LOCAL_VER_INFO)
#define HCI_OPCODE_READ_LOCAL_SUP_CMDS               HCI_OPCODE(HCI_OGF_INFORMATIONAL, HCI_OCF_READ_LOCAL_SUP_CMDS)
#define HCI_OPCODE_READ_LOCAL_SUP_FEAT               HCI_OPCODE(HCI_OGF_INFORMATIONAL, HCI_OCF_READ_LOCAL_SUP_FEAT)
#define HCI_OPCODE_READ_BUF_SIZE                     HCI_OPCODE(HCI_OGF_INFORMATIONAL, HCI_OCF_READ_BUF_SIZE)
#define HCI_OPCODE_READ_BD_ADDR                      HCI_OPCODE(HCI_OGF_INFORMATIONAL, HCI_OCF_READ_BD_ADDR)

#define HCI_OPCODE_READ_RSSI                         HCI_OPCODE(HCI_OGF_STATUS, HCI_OCF_READ_RSSI)

#define HCI_OPCODE_LE_SET_EVENT_MASK                 HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_EVENT_MASK)
#define HCI_OPCODE_LE_READ_BUF_SIZE                  HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_BUF_SIZE)
#define HCI_OPCODE_LE_READ_LOCAL_SUP_FEAT            HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_LOCAL_SUP_FEAT)
#define HCI_OPCODE_LE_SET_RAND_ADDR                  HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_RAND_ADDR)
#define HCI_OPCODE_LE_SET_ADV_PARAM                  HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_ADV_PARAM)
#define HCI_OPCODE_LE_READ_ADV_TX_POWER              HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_ADV_TX_POWER)
#define HCI_OPCODE_LE_SET_ADV_DATA                   HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_ADV_DATA)
#define HCI_OPCODE_LE_SET_SCAN_RESP_DATA             HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_SCAN_RESP_DATA)
#define HCI_OPCODE_LE_SET_ADV_ENABLE                 HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_ADV_ENABLE)
#define HCI_OPCODE_LE_SET_SCAN_PARAM                 HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_SCAN_PARAM)
#define HCI_OPCODE_LE_SET_SCAN_ENABLE                HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_SCAN_ENABLE)
#define HCI_OPCODE_LE_CREATE_CONN                    HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_CREATE_CONN)
#define HCI_OPCODE_LE_CREATE_CONN_CANCEL             HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_CREATE_CONN_CANCEL)
#define HCI_OPCODE_LE_READ_WHITE_LIST_SIZE           HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_WHITE_LIST_SIZE)
#define HCI_OPCODE_LE_CLEAR_WHITE_LIST               HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_CLEAR_WHITE_LIST)
#define HCI_OPCODE_LE_ADD_DEV_WHITE_LIST             HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_ADD_DEV_WHITE_LIST)
#define HCI_OPCODE_LE_REMOVE_DEV_WHITE_LIST          HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_REMOVE_DEV_WHITE_LIST)
#define HCI_OPCODE_LE_CONN_UPDATE                    HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_CONN_UPDATE)
#define HCI_OPCODE_LE_SET_HOST_CHAN_CLASS            HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_HOST_CHAN_CLASS)
#define HCI_OPCODE_LE_READ_CHAN_MAP                  HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_CHAN_MAP)
#define HCI_OPCODE_LE_READ_REMOTE_FEAT               HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_REMOTE_FEAT)
#define HCI_OPCODE_LE_ENCRYPT                        HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_ENCRYPT)
#define HCI_OPCODE_LE_RAND                           HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_RAND)
#define HCI_OPCODE_LE_START_ENCRYPTION               HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_START_ENCRYPTION)
#define HCI_OPCODE_LE_LTK_REQ_REPL                   HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_LTK_REQ_REPL)
#define HCI_OPCODE_LE_LTK_REQ_NEG_REPL               HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_LTK_REQ_NEG_REPL)
#define HCI_OPCODE_LE_READ_SUP_STATES                HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_SUP_STATES)
#define HCI_OPCODE_LE_RECEIVER_TEST                  HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_RECEIVER_TEST)
#define HCI_OPCODE_LE_TRANSMITTER_TEST               HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_TRANSMITTER_TEST)
#define HCI_OPCODE_LE_TEST_END                       HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_TEST_END)
/*! \brief New in version 4.1 */
#define HCI_OPCODE_LE_REM_CONN_PARAM_REP             HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_REM_CONN_PARAM_REP)
#define HCI_OPCODE_LE_REM_CONN_PARAM_NEG_REP         HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_REM_CONN_PARAM_NEG_REP)
/*! \brief New in version 4.2 */
#define HCI_OPCODE_LE_SET_DATA_LEN                   HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_DATA_LEN)
#define HCI_OPCODE_LE_READ_DEF_DATA_LEN              HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_DEF_DATA_LEN)
#define HCI_OPCODE_LE_WRITE_DEF_DATA_LEN             HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_WRITE_DEF_DATA_LEN)
#define HCI_OPCODE_LE_READ_LOCAL_P256_PUB_KEY        HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_LOCAL_P256_PUB_KEY)
#define HCI_OPCODE_LE_GENERATE_DHKEY                 HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_GENERATE_DHKEY)
#define HCI_OPCODE_LE_ADD_DEV_RES_LIST               HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_ADD_DEV_RES_LIST)
#define HCI_OPCODE_LE_REMOVE_DEV_RES_LIST            HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_REMOVE_DEV_RES_LIST)
#define HCI_OPCODE_LE_CLEAR_RES_LIST                 HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_CLEAR_RES_LIST)
#define HCI_OPCODE_LE_READ_RES_LIST_SIZE             HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_RES_LIST_SIZE)
#define HCI_OPCODE_LE_READ_PEER_RES_ADDR             HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_PEER_RES_ADDR)
#define HCI_OPCODE_LE_READ_LOCAL_RES_ADDR            HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_LOCAL_RES_ADDR)
#define HCI_OPCODE_LE_SET_ADDR_RES_ENABLE            HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_ADDR_RES_ENABLE)
#define HCI_OPCODE_LE_SET_RES_PRIV_ADDR_TO           HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_RES_PRIV_ADDR_TO)
#define HCI_OPCODE_LE_READ_MAX_DATA_LEN              HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_MAX_DATA_LEN)
/*! \brief New in version 5.0 */
#define HCI_OPCODE_LE_READ_PHY                       HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_PHY)
#define HCI_OPCODE_LE_SET_DEF_PHY                    HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_DEF_PHY)
#define HCI_OPCODE_LE_SET_PHY                        HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_PHY)
#define HCI_OPCODE_LE_ENHANCED_RECEIVER_TEST         HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_ENHANCED_RECEIVER_TEST)
#define HCI_OPCODE_LE_ENHANCED_TRANSMITTER_TEST      HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_ENHANCED_TRANSMITTER_TEST)
#define HCI_OPCODE_LE_SET_ADV_SET_RAND_ADDR          HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_ADV_SET_RAND_ADDR)
#define HCI_OPCODE_LE_SET_EXT_ADV_PARAM              HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_EXT_ADV_PARAM)
#define HCI_OPCODE_LE_SET_EXT_ADV_DATA               HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_EXT_ADV_DATA)
#define HCI_OPCODE_LE_SET_EXT_SCAN_RESP_DATA         HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_EXT_SCAN_RESP_DATA)
#define HCI_OPCODE_LE_SET_EXT_ADV_ENABLE             HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_EXT_ADV_ENABLE)
#define HCI_OPCODE_LE_READ_MAX_ADV_DATA_LEN          HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_MAX_ADV_DATA_LEN)
#define HCI_OPCODE_LE_READ_NUM_SUP_ADV_SETS          HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_NUM_SUP_ADV_SETS)
#define HCI_OPCODE_LE_REMOVE_ADV_SET                 HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_REMOVE_ADV_SET)
#define HCI_OPCODE_LE_CLEAR_ADV_SETS                 HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_CLEAR_ADV_SETS)
#define HCI_OPCODE_LE_SET_PER_ADV_PARAM              HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_PER_ADV_PARAM)
#define HCI_OPCODE_LE_SET_PER_ADV_DATA               HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_PER_ADV_DATA)
#define HCI_OPCODE_LE_SET_PER_ADV_ENABLE             HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_PER_ADV_ENABLE)
#define HCI_OPCODE_LE_SET_EXT_SCAN_PARAM             HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_EXT_SCAN_PARAM)
#define HCI_OPCODE_LE_SET_EXT_SCAN_ENABLE            HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_EXT_SCAN_ENABLE)
#define HCI_OPCODE_LE_EXT_CREATE_CONN                HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_EXT_CREATE_CONN)
#define HCI_OPCODE_LE_PER_ADV_CREATE_SYNC            HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_PER_ADV_CREATE_SYNC)
#define HCI_OPCODE_LE_PER_ADV_CREATE_SYNC_CANCEL     HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_PER_ADV_CREATE_SYNC_CANCEL)
#define HCI_OPCODE_LE_PER_ADV_TERMINATE_SYNC         HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_PER_ADV_TERM_SYNC)
#define HCI_OPCODE_LE_ADD_DEV_PER_ADV_LIST           HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_ADD_DEV_PER_ADV_LIST)
#define HCI_OPCODE_LE_REMOVE_DEV_PER_ADV_LIST        HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_REMOVE_DEV_PER_ADV_LIST)
#define HCI_OPCODE_LE_CLEAR_PER_ADV_LIST             HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_CLEAR_PER_ADV_LIST)
#define HCI_OPCODE_LE_READ_PER_ADV_LIST_SIZE         HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_PER_ADV_LIST_SIZE)
#define HCI_OPCODE_LE_READ_TX_POWER                  HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_TX_POWER)
#define HCI_OPCODE_LE_WRITE_RF_PATH_COMP             HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_WRITE_RF_PATH_COMP)
#define HCI_OPCODE_LE_READ_RF_PATH_COMP              HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_READ_RF_PATH_COMP)
#define HCI_OPCODE_LE_SET_PRIVACY_MODE               HCI_OPCODE(HCI_OGF_LE_CONTROLLER, HCI_OCF_LE_SET_PRIVACY_MODE)
/**@}*/

/** \name ARM Vendor Specific
 *
 */
/**@{*/
#define HCI_OPCODE_LE_VS_ENABLE_READ_FEAT_ON_CONN    ((uint16_t)(0xfff3))
/**@}*/

/** \name Command parameter lengths
 *
 */
/**@{*/
#define HCI_LEN_NOP                                  0

#define HCI_LEN_DISCONNECT                           3
#define HCI_LEN_READ_REMOTE_VER_INFO                 2

#define HCI_LEN_SET_EVENT_MASK                       8
#define HCI_LEN_SET_EVENT_MASK_PAGE2                 8
#define HCI_LEN_RESET                                0
#define HCI_LEN_READ_TX_PWR_LVL                      3
#define HCI_LEN_SET_CONTROLLER_TO_HOST_FC            1
#define HCI_LEN_HOST_BUFFER_SIZE                     8
#define HCI_LEN_HOST_NUM_CMPL_PKTS                   1

#define HCI_LEN_READ_LOCAL_VER_INFO                  0
#define HCI_LEN_READ_LOCAL_SUP_CMDS                  0
#define HCI_LEN_READ_LOCAL_SUP_FEAT                  0
#define HCI_LEN_READ_BUF_SIZE                        0
#define HCI_LEN_READ_BD_ADDR                         0

#define HCI_LEN_READ_RSSI                            2
#define HCI_LEN_READ_AUTH_PAYLOAD_TO                 2
#define HCI_LEN_WRITE_AUTH_PAYLOAD_TO                4

#define HCI_LEN_LE_SET_EVENT_MASK                    8
#define HCI_LEN_LE_READ_BUF_SIZE                     0
#define HCI_LEN_LE_READ_LOCAL_SUP_FEAT               0
#define HCI_LEN_LE_SET_RAND_ADDR                     6
#define HCI_LEN_LE_SET_ADV_PARAM                     15
#define HCI_LEN_LE_READ_ADV_TX_POWER                 0
#define HCI_LEN_LE_SET_ADV_DATA                      32
#define HCI_LEN_LE_SET_SCAN_RESP_DATA                32
#define HCI_LEN_LE_SET_ADV_ENABLE                    1
#define HCI_LEN_LE_SET_SCAN_PARAM                    7
#define HCI_LEN_LE_SET_SCAN_ENABLE                   2
#define HCI_LEN_LE_CREATE_CONN                       25
#define HCI_LEN_LE_CREATE_CONN_CANCEL                0
#define HCI_LEN_LE_READ_WHITE_LIST_SIZE              0
#define HCI_LEN_LE_CLEAR_WHITE_LIST                  0
#define HCI_LEN_LE_ADD_DEV_WHITE_LIST                7
#define HCI_LEN_LE_REMOVE_DEV_WHITE_LIST             7
#define HCI_LEN_LE_CONN_UPDATE                       14
#define HCI_LEN_LE_SET_HOST_CHAN_CLASS               5
#define HCI_LEN_LE_READ_CHAN_MAP                     2
#define HCI_LEN_LE_READ_REMOTE_FEAT                  2
#define HCI_LEN_LE_ENCRYPT                           32
#define HCI_LEN_LE_RAND                              0
#define HCI_LEN_LE_START_ENCRYPTION                  28
#define HCI_LEN_LE_LTK_REQ_REPL                      18
#define HCI_LEN_LE_LTK_REQ_NEG_REPL                  2
#define HCI_LEN_LE_READ_SUP_STATES                   0
#define HCI_LEN_LE_RECEIVER_TEST                     1
#define HCI_LEN_LE_TRANSMITTER_TEST                  3
#define HCI_LEN_LE_TEST_END                          0
/*! \brief New in version 4.1 */
#define HCI_LEN_LE_REM_CONN_PARAM_REP                14
#define HCI_LEN_LE_REM_CONN_PARAM_NEG_REP            3
/*! \brief New in version 4.2 */
#define HCI_LEN_LE_SET_DATA_LEN                      6
#define HCI_LEN_LE_READ_DEF_DATA_LEN                 0
#define HCI_LEN_LE_WRITE_DEF_DATA_LEN                4
#define HCI_LEN_LE_READ_LOCAL_P256_PUB_KEY           0
#define HCI_LEN_LE_GENERATE_DHKEY                    64
#define HCI_LEN_LE_ADD_DEV_RES_LIST                  39
#define HCI_LEN_LE_REMOVE_DEV_RES_LIST               7
#define HCI_LEN_LE_CLEAR_RES_LIST                    0
#define HCI_LEN_LE_READ_RES_LIST_SIZE                0
#define HCI_LEN_LE_READ_PEER_RES_ADDR                7
#define HCI_LEN_LE_READ_LOCAL_RES_ADDR               7
#define HCI_LEN_LE_SET_ADDR_RES_ENABLE               1
#define HCI_LEN_LE_SET_RES_PRIV_ADDR_TO              2
#define HCI_LEN_LE_READ_MAX_DATA_LEN                 0
/*! \brief New in version 5.0 */
#define HCI_LEN_LE_READ_PHY                          2
#define HCI_LEN_LE_SET_DEF_PHY                       3
#define HCI_LEN_LE_SET_PHY                           7
#define HCI_LEN_LE_ENHANCED_RECEIVER_TEST            3
#define HCI_LEN_LE_ENHANCED_TRANSMITTER_TEST         4
#define HCI_LEN_LE_SET_ADV_SET_RAND_ADDR             7
#define HCI_LEN_LE_SET_EXT_ADV_PARAM                 25
#define HCI_LEN_LE_SET_EXT_ADV_DATA(len)             (4 + (len))
#define HCI_LEN_LE_SET_EXT_SCAN_RESP_DATA(len)       (4 + (len))
#define HCI_LEN_LE_EXT_ADV_ENABLE(numSets)           (2 + (4 * (numSets)))
#define HCI_LEN_LE_READ_MAX_ADV_DATA_LEN             0
#define HCI_LEN_LE_READ_NUM_OF_SUP_ADV_SETS          0
#define HCI_LEN_LE_REMOVE_ADV_SET                    1
#define HCI_LEN_LE_CLEAR_ADV_SETS                    0
#define HCI_LEN_LE_SET_PER_ADV_PARAM                 7
#define HCI_LEN_LE_SET_PER_ADV_DATA(len)             (3 + (len))
#define HCI_LEN_LE_SET_PER_ADV_ENABLE                2
#define HCI_LEN_LE_SET_EXT_SCAN_PARAM(numPhys)       (3 + (5 * (numPhys)))
#define HCI_LEN_LE_SET_EXT_SCAN_ENABLE               6
#define HCI_LEN_LE_EXT_CREATE_CONN(numPhys)          (10 + (16 * (numPhys)))
#define HCI_LEN_LE_PER_ADV_CREATE_SYNC               14
#define HCI_LEN_LE_PER_ADV_CREATE_SYNC_CANCEL        0
#define HCI_LEN_LE_PER_ADV_TERMINATE_SYNC            2
#define HCI_LEN_LE_ADD_DEV_PER_ADV_LIST              8
#define HCI_LEN_LE_REMOVE_DEV_PER_ADV_LIST           8
#define HCI_LEN_LE_CLEAR_PER_ADV_LIST                0
#define HCI_LEN_LE_READ_PER_ADV_LIST_SIZE            0
#define HCI_LEN_LE_READ_TX_POWER                     0
#define HCI_LEN_LE_READ_RF_PATH_COMP                 0
#define HCI_LEN_LE_WRITE_RF_PATH_COMP                4
#define HCI_LEN_LE_SET_PRIVACY_MODE                  8
/**@}*/

/** \name Events
 *
 */
/**@{*/
#define HCI_DISCONNECT_CMPL_EVT                      0x05
#define HCI_ENC_CHANGE_EVT                           0x08
#define HCI_READ_REMOTE_VER_INFO_CMPL_EVT            0x0C
#define HCI_CMD_CMPL_EVT                             0x0E
#define HCI_CMD_STATUS_EVT                           0x0F
#define HCI_HW_ERROR_EVT                             0x10
#define HCI_NUM_CMPL_PKTS_EVT                        0x13
#define HCI_DATA_BUF_OVERFLOW_EVT                    0x1A
#define HCI_ENC_KEY_REFRESH_CMPL_EVT                 0x30
#define HCI_LE_META_EVT                              0x3E
#define HCI_AUTH_PAYLOAD_TIMEOUT_EVT                 0x57
#define HCI_VENDOR_SPEC_EVT                          0xFF
/**@}*/

/** \name LE Subevents
 *
 */
/**@{*/
#define HCI_LE_CONN_CMPL_EVT                         0x01
#define HCI_LE_ADV_REPORT_EVT                        0x02
#define HCI_LE_CONN_UPDATE_CMPL_EVT                  0x03
#define HCI_LE_READ_REMOTE_FEAT_CMPL_EVT             0x04
#define HCI_LE_LTK_REQ_EVT                           0x05
/*! \brief New in version 4.1 */
#define HCI_LE_REM_CONN_PARAM_REQ_EVT                0x06
/*! \brief New in version 4.2 */
#define HCI_LE_DATA_LEN_CHANGE_EVT                   0x07
#define HCI_LE_READ_LOCAL_P256_PUB_KEY_CMPL_EVT      0x08
#define HCI_LE_GENERATE_DHKEY_CMPL_EVT               0x09
#define HCI_LE_ENHANCED_CONN_CMPL_EVT                0x0A
#define HCI_LE_DIRECT_ADV_REPORT_EVT                 0x0B
/*! \brief New in version 5.0 */
#define HCI_LE_PHY_UPDATE_CMPL_EVT                   0x0C
#define HCI_LE_EXT_ADV_REPORT_EVT                    0x0D
#define HCI_LE_PER_ADV_SYNC_EST_EVT                  0x0E
#define HCI_LE_PER_ADV_REPORT_EVT                    0x0F
#define HCI_LE_PER_ADV_SYNC_LOST_EVT                 0x10
#define HCI_LE_SCAN_TIMEOUT_EVT                      0x11
#define HCI_LE_ADV_SET_TERM_EVT                      0x12
#define HCI_LE_SCAN_REQ_RCVD_EVT                     0x13
#define HCI_LE_CH_SEL_ALGO_EVT                       0x14
/**@}*/

/** \name Event parameter lengths
 *
 */
/**@{*/
#define HCI_LEN_DISCONNECT_CMPL                      4       /*!< \brief Disconnect event length. */
#define HCI_LEN_READ_REMOTE_VER_INFO_CMPL            8       /*!< \brief Read remove version info complete event length. */
#define HCI_LEN_CMD_CMPL                             3       /*!< \brief Command complete event length. */
#define HCI_LEN_CMD_STATUS                           4       /*!< \brief Command status event length. */
#define HCI_LEN_HW_ERR                               1       /*!< \brief Hardware error event length. */
#define HCI_LEN_NUM_CMPL_PKTS                        5       /*!< \brief Number of completed packets event length. */
#define HCI_LEN_ENC_CHANGE                           4       /*!< \brief Encryption change event length. */
#define HCI_LEN_ENC_KEY_REFRESH_CMPL                 3       /*!< \brief Encryption key refresh complete event length. */
#define HCI_LEN_LE_CONN_CMPL                         19      /*!< \brief Connection complete event length. */
#define HCI_LEN_LE_ADV_RPT_MIN                       12      /*!< \brief Advertising report event minimum length. */
#define HCI_LEN_LE_CONN_UPDATE_CMPL                  10      /*!< \brief Connection update complete event length. */
#define HCI_LEN_LE_READ_REMOTE_FEAT_CMPL             12      /*!< \brief Read remote feature event length. */
#define HCI_LEN_LE_LTK_REQ                           13      /*!< \brief LTK request event length. */
/*! \brief New in version 4.1 */
#define HCI_LEN_LE_REM_CONN_PARAM_REQ                11      /*!< \brief Remote connection parameter event length. */
#define HCI_LEN_LE_DATA_LEN_CHANGE                   11      /*!< \brief Data length change event length. */
#define HCI_LEN_LE_READ_PUB_KEY_CMPL                 66      /*!< \brief Read local P256 public key compete event length. */
#define HCI_LEN_LE_GEN_DHKEY_CMPL                    34      /*!< \brief Generate DH key complete event length. */
#define HCI_LEN_LE_ENHANCED_CONN_CMPL                31      /*!< \brief Enhanced connection complete event length. */
#define HCI_LEN_LE_DIRECT_ADV_REPORT                 18      /*!< \brief Direct advertising report event length. */
#define HCI_LEN_AUTH_PAYLOAD_TIMEOUT                 2       /*!< \brief Authenticated payload timeout event length. */
/*! \brief New in version 5.0 */
#define HCI_LEN_LE_PHY_UPDATE_CMPL                   6       /*!< \brief PHY update complete event length. */
#define HCI_LEN_LE_CH_SEL_ALGO                       4       /*!< \brief Channel selection algorithm event length. */
#define HCI_LEN_LE_PHY_UPDATE_CMPL                   6       /*!< \brief PHY update complete event length. */
#define HCI_LEN_LE_EXT_ADV_REPORT_MIN                26      /*!< \brief Extended advertising report minimum length. */
#define HCI_LEN_LE_PER_ADV_SYNC_EST                  16      /*!< \brief Periodic advertising sync established event length. */
#define HCI_LEN_LE_PER_ADV_REPORT                    8       /*!< \brief Periodic advertising report event length. */
#define HCI_LEN_LE_PER_ADV_SYNC_LOST                 3       /*!< \brief Periodic advertising sync lost event length. */
#define HCI_LEN_LE_SCAN_TIMEOUT                      1       /*!< \brief Scan timeout event length. */
#define HCI_LEN_LE_ADV_SET_TERM                      6       /*!< \brief Advertising set terminated event length. */
#define HCI_LEN_LE_SCAN_REQ_RCVD                     9       /*!< \brief Scan request received event length. */
/**@}*/

/** \name Supported commands
 *
 */
/**@{*/
#define HCI_SUP_DISCONNECT                           0x20    /*!< \brief Byte 0 */
#define HCI_SUP_READ_REMOTE_VER_INFO                 0x80    /*!< \brief Byte 2 */
#define HCI_SUP_SET_EVENT_MASK                       0x40    /*!< \brief Byte 5 */
#define HCI_SUP_RESET                                0x80    /*!< \brief Byte 5 */
#define HCI_SUP_READ_TX_PWR_LVL                      0x04    /*!< \brief Byte 10 */
#define HCI_SUP_READ_LOCAL_VER_INFO                  0x08    /*!< \brief Byte 14 */
#define HCI_SUP_READ_LOCAL_SUP_FEAT                  0x20    /*!< \brief Byte 14 */
#define HCI_SUP_READ_BD_ADDR                         0x02    /*!< \brief Byte 15 */
#define HCI_SUP_READ_RSSI                            0x20    /*!< \brief Byte 15 */
#define HCI_SUP_SET_EVENT_MASK_PAGE2                 0x04    /*!< \brief Byte 22 */
#define HCI_SUP_LE_SET_EVENT_MASK                    0x01    /*!< \brief Byte 25 */
#define HCI_SUP_LE_READ_BUF_SIZE                     0x02    /*!< \brief Byte 25 */
#define HCI_SUP_LE_READ_LOCAL_SUP_FEAT               0x04    /*!< \brief Byte 25 */
#define HCI_SUP_LE_SET_RAND_ADDR                     0x10    /*!< \brief Byte 25 */
#define HCI_SUP_LE_SET_ADV_PARAM                     0x20    /*!< \brief Byte 25 */
#define HCI_SUP_LE_READ_ADV_TX_POWER                 0x40    /*!< \brief Byte 25 */
#define HCI_SUP_LE_SET_ADV_DATA                      0x80    /*!< \brief Byte 25 */
#define HCI_SUP_LE_SET_SCAN_RESP_DATA                0x01    /*!< \brief Byte 26 */
#define HCI_SUP_LE_SET_ADV_ENABLE                    0x02    /*!< \brief Byte 26 */
#define HCI_SUP_LE_SET_SCAN_PARAM                    0x04    /*!< \brief Byte 26 */
#define HCI_SUP_LE_SET_SCAN_ENABLE                   0x08    /*!< \brief Byte 26 */
#define HCI_SUP_LE_CREATE_CONN                       0x10    /*!< \brief Byte 26 */
#define HCI_SUP_LE_CREATE_CONN_CANCEL                0x20    /*!< \brief Byte 26 */
#define HCI_SUP_LE_READ_WHITE_LIST_SIZE              0x40    /*!< \brief Byte 26 */
#define HCI_SUP_LE_CLEAR_WHITE_LIST                  0x80    /*!< \brief Byte 26 */
#define HCI_SUP_LE_ADD_DEV_WHITE_LIST                0x01    /*!< \brief Byte 27 */
#define HCI_SUP_LE_REMOVE_DEV_WHITE_LIST             0x02    /*!< \brief Byte 27 */
#define HCI_SUP_LE_CONN_UPDATE                       0x04    /*!< \brief Byte 27 */
#define HCI_SUP_LE_SET_HOST_CHAN_CLASS               0x08    /*!< \brief Byte 27 */
#define HCI_SUP_LE_READ_CHAN_MAP                     0x10    /*!< \brief Byte 27 */
#define HCI_SUP_LE_READ_REMOTE_FEAT                  0x20    /*!< \brief Byte 27 */
#define HCI_SUP_LE_ENCRYPT                           0x40    /*!< \brief Byte 27 */
#define HCI_SUP_LE_RAND                              0x80    /*!< \brief Byte 27 */
#define HCI_SUP_LE_START_ENCRYPTION                  0x01    /*!< \brief Byte 28 */
#define HCI_SUP_LE_LTK_REQ_REPL                      0x02    /*!< \brief Byte 28 */
#define HCI_SUP_LE_LTK_REQ_NEG_REPL                  0x04    /*!< \brief Byte 28 */
#define HCI_SUP_LE_READ_SUP_STATES                   0x08    /*!< \brief Byte 28 */
#define HCI_SUP_LE_RECEIVER_TEST                     0x10    /*!< \brief Byte 28 */
#define HCI_SUP_LE_TRANSMITTER_TEST                  0x20    /*!< \brief Byte 28 */
#define HCI_SUP_LE_TEST_END                          0x40    /*!< \brief Byte 28 */
#define HCI_SUP_READ_AUTH_PAYLOAD_TO                 0x10    /*!< \brief Byte 32 */
#define HCI_SUP_WRITE_AUTH_PAYLOAD_TO                0x20    /*!< \brief Byte 32 */
/*! \brief New in version 4.1 */
#define HCI_SUP_LE_REM_CONN_PARAM_REQ_REPL           0x10    /*!< \brief Byte 33 */
#define HCI_SUP_LE_REM_CONN_PARAM_REQ_NEG_REPL       0x20    /*!< \brief Byte 33 */
/*! \brief New in version 4.2 */
#define HCI_SUP_LE_SET_DATA_LEN                      0x40    /*!< \brief Byte 33 */
#define HCI_SUP_LE_READ_DEF_DATA_LEN                 0x80    /*!< \brief Byte 33 */
#define HCI_SUP_LE_WRITE_DEF_DATA_LEN                0x01    /*!< \brief Byte 34 */
#define HCI_SUP_LE_READ_LOCAL_P256_PUB_KEY           0x02    /*!< \brief Byte 34 */
#define HCI_SUP_LE_GENERATE_DHKEY                    0x04    /*!< \brief Byte 34 */
#define HCI_SUP_LE_ADD_DEV_RES_LIST_EVT              0x08    /*!< \brief Byte 34 */
#define HCI_SUP_LE_REMOVE_DEV_RES_LIST               0x10    /*!< \brief Byte 34 */
#define HCI_SUP_LE_CLEAR_RES_LIST                    0x20    /*!< \brief Byte 34 */
#define HCI_SUP_LE_READ_RES_LIST_SIZE                0x40    /*!< \brief Byte 34 */
#define HCI_SUP_LE_READ_PEER_RES_ADDR                0x80    /*!< \brief Byte 34 */
#define HCI_SUP_LE_READ_LOCAL_RES_ADDR               0x01    /*!< \brief Byte 35 */
#define HCI_SUP_LE_SET_ADDR_RES_ENABLE               0x02    /*!< \brief Byte 35 */
#define HCI_SUP_LE_SET_RES_PRIV_ADDR_TO              0x04    /*!< \brief Byte 35 */
#define HCI_SUP_LE_READ_MAX_DATA_LEN                 0x08    /*!< \brief Byte 35 */
/*! \brief New in version 5.0 */
#define HCI_SUP_LE_READ_PHY                          0x10    /*!< \brief Byte 35 */
#define HCI_SUP_LE_SET_DEF_PHY                       0x20    /*!< \brief Byte 35 */
#define HCI_SUP_LE_SET_PHY                           0x40    /*!< \brief Byte 35 */
#define HCI_SUP_LE_ENHANCED_RECEIVER_TEST            0x80    /*!< \brief Byte 35 */
#define HCI_SUP_LE_ENHANCED_TRANSMITTER_TEST         0x01    /*!< \brief Byte 36 */
#define HCI_SUP_LE_SET_ADV_SET_RAND_ADDR             0x02    /*!< \brief Byte 36 */
#define HCI_SUP_LE_SET_EXT_ADV_PARAM                 0x04    /*!< \brief Byte 36 */
#define HCI_SUP_LE_SET_EXT_ADV_DATA                  0x08    /*!< \brief Byte 36 */
#define HCI_SUP_LE_SET_EXT_SCAN_RESP_DATA            0x10    /*!< \brief Byte 36 */
#define HCI_SUP_LE_SET_EXT_ADV_ENABLE                0x20    /*!< \brief Byte 36 */
#define HCI_SUP_LE_READ_MAX_ADV_DATA_LEN             0x40    /*!< \brief Byte 36 */
#define HCI_SUP_LE_READ_NUM_OF_SUP_ADV_SETS          0x80    /*!< \brief Byte 36 */
#define HCI_SUP_LE_REMOVE_ADV_SET                    0x01    /*!< \brief Byte 37 */
#define HCI_SUP_LE_CLEAR_ADV_SETS                    0x02    /*!< \brief Byte 37 */
#define HCI_SUP_LE_SET_PER_ADV_PARAM                 0x04    /*!< \brief Byte 37 */
#define HCI_SUP_LE_SET_PER_ADV_DATA                  0x08    /*!< \brief Byte 37 */
#define HCI_SUP_LE_SET_PER_ADV_ENABLE                0x10    /*!< \brief Byte 37 */
#define HCI_SUP_LE_SET_EXT_SCAN_PARAM                0x20    /*!< \brief Byte 37 */
#define HCI_SUP_LE_SET_EXT_SCAN_ENABLE               0x40    /*!< \brief Byte 37 */
#define HCI_SUP_LE_EXT_CREATE_CONN                   0x80    /*!< \brief Byte 37 */
#define HCI_SUP_LE_PER_ADV_CREATE_SYNC               0x01    /*!< \brief Byte 38 */
#define HCI_SUP_LE_PER_ADV_CREATE_SYNC_CANCEL        0x02    /*!< \brief Byte 38 */
#define HCI_SUP_LE_PER_ADV_TERMINATE_SYNC            0x04    /*!< \brief Byte 38 */
#define HCI_SUP_LE_ADD_DEV_PER_ADV_LIST              0x08    /*!< \brief Byte 38 */
#define HCI_SUP_LE_REMOVE_DEV_PER_ADV_LIST           0x10    /*!< \brief Byte 38 */
#define HCI_SUP_LE_CLEAR_PER_ADV_LIST                0x20    /*!< \brief Byte 38 */
#define HCI_SUP_LE_READ_PER_ADV_LIST_SIZE            0x40    /*!< \brief Byte 38 */
#define HCI_SUP_LE_READ_TX_POWER                     0x80    /*!< \brief Byte 38 */
#define HCI_SUP_LE_READ_RF_PATH_COMP                 0x01    /*!< \brief Byte 39 */
#define HCI_SUP_LE_WRITE_RF_PATH_COMP                0x02    /*!< \brief Byte 39 */
#define HCI_SUP_LE_SET_PRIVACY_MODE                  0x04    /*!< \brief Byte 39 */
#define HCI_SUP_LE_SET_MIN_NUM_OF_USED_CH            0x08    /*!< \brief Byte 39 */
/**@}*/

/** \name Event mask
 *
 */
/**@{*/
#define HCI_EVT_MASK_DISCONNECT_CMPL                 0x10    /*!< \brief Byte 0 */
#define HCI_EVT_MASK_ENC_CHANGE                      0x80    /*!< \brief Byte 0 */
#define HCI_EVT_MASK_READ_REMOTE_VER_INFO_CMPL       0x08    /*!< \brief Byte 1 */
#define HCI_EVT_MASK_HW_ERROR                        0x80    /*!< \brief Byte 1 */
#define HCI_EVT_MASK_DATA_BUF_OVERFLOW               0x02    /*!< \brief Byte 3 */
#define HCI_EVT_MASK_ENC_KEY_REFRESH_CMPL            0x80    /*!< \brief Byte 5 */
#define HCI_EVT_MASK_LE_META                         0x20    /*!< \brief Byte 7 */
/**@}*/

/** \name Event mask page 2
 *
 */
/**@{*/
#define HCI_EVT_MASK_AUTH_PAYLOAD_TIMEOUT            0x80    /*!< \brief Byte 2 */
/**@}*/

/** \name LE event mask
 *
 */
/**@{*/
#define HCI_EVT_MASK_LE_CONN_CMPL_EVT                0x01    /*!< \brief Byte 0 */
#define HCI_EVT_MASK_LE_ADV_REPORT_EVT               0x02    /*!< \brief Byte 0 */
#define HCI_EVT_MASK_LE_CONN_UPDATE_CMPL_EVT         0x04    /*!< \brief Byte 0 */
#define HCI_EVT_MASK_LE_READ_REMOTE_FEAT_CMPL_EVT    0x08    /*!< \brief Byte 0 */
#define HCI_EVT_MASK_LE_LTK_REQ_EVT                  0x10    /*!< \brief Byte 0 */
/*! \brief New in version 4.1 */
#define HCI_EVT_MASK_LE_REMOTE_CONN_PARAM_REQ_EVT    0x20    /*!< \brief Byte 0 */
/*! \brief New in version 4.2 */
#define HCI_EVT_MASK_LE_DATA_LEN_CHANGE_EVT          0x40    /*!< \brief Byte 0 */
#define HCI_EVT_MASK_LE_READ_LOCAL_P256_PUB_KEY_CMPL 0x80    /*!< \brief Byte 0 */
#define HCI_EVT_MASK_LE_GENERATE_DHKEY_CMPL          0x01    /*!< \brief Byte 1 */
#define HCI_EVT_MASK_LE_ENHANCED_CONN_CMPL_EVT       0x02    /*!< \brief Byte 1 */
#define HCI_EVT_MASK_LE_DIRECT_ADV_REPORT_EVT        0x04    /*!< \brief Byte 1 */
/*! \brief New in version 5.0 */
#define HCI_EVT_MASK_LE_PHY_UPDATE_CMPL_EVT          0x08    /*!< \brief Byte 1 */
#define HCI_EVT_MASK_LE_EXT_ADV_REPORT_EVT           0x10    /*!< \brief Byte 1 */
#define HCI_EVT_MASK_LE_PER_ADV_SYNC_EST_EVT         0x20    /*!< \brief Byte 1 */
#define HCI_EVT_MASK_LE_PER_ADV_REPORT_EVT           0x40    /*!< \brief Byte 1 */
#define HCI_EVT_MASK_LE_PER_ADV_SYNC_LOST_EVT        0x80    /*!< \brief Byte 1 */
#define HCI_EVT_MASK_LE_SCAN_TIMEOUT_EVT             0x01    /*!< \brief Byte 2 */
#define HCI_EVT_MASK_LE_ADV_SET_TERM_EVT             0x02    /*!< \brief Byte 2 */
#define HCI_EVT_MASK_LE_SCAN_REQ_RCVD_EVT            0x04    /*!< \brief Byte 2 */
#define HCI_EVT_MASK_LE_CH_SEL_ALGO_EVT              0x08    /*!< \brief Byte 2 */
/**@}*/

/** \name LE supported features
 *
 */
/**@{*/
/*! \brief New in version 4.0 */
#define HCI_LE_SUP_FEAT_ENCRYPTION                   0x0001  /*!< \brief Encryption supported */
/*! \brief New in version 4.1 */
#define HCI_LE_SUP_FEAT_CONN_PARAM_REQ_PROC          0x0002  /*!< \brief Connection Parameters Request Procedure supported */
#define HCI_LE_SUP_FEAT_EXT_REJECT_IND               0x0004  /*!< \brief Extended Reject Indication supported */
#define HCI_LE_SUP_FEAT_SLV_INIT_FEAT_EXCH           0x0008  /*!< \brief Slave-Initiated Features Exchange supported */
#define HCI_LE_SUP_FEAT_LE_PING                      0x0010  /*!< \brief LE Ping supported */
/*! \brief New in version 4.2 */
#define HCI_LE_SUP_FEAT_DATA_LEN_EXT                 0x0020  /*!< \brief Data Length Extension supported */
#define HCI_LE_SUP_FEAT_PRIVACY                      0x0040  /*!< \brief LL Privacy supported */
#define HCI_LE_SUP_FEAT_EXT_SCAN_FILT_POLICY         0x0080  /*!< \brief Extended Scan Filter Policy supported */
/*! \brief New in version 5.0 */
#define HCI_LE_SUP_FEAT_LE_2M_PHY                    0x0100  /*!< \brief LE 2M PHY supported */
#define HCI_LE_SUP_FEAT_STABLE_MOD_IDX_TRANSMITTER   0x0200  /*!< \brief Stable Modulation Index - Transmitter supported */
#define HCI_LE_SUP_FEAT_STABLE_MOD_IDX_RECEIVER      0x0400  /*!< \brief Stable Modulation Index - Receiver supported */
#define HCI_LE_SUP_FEAT_LE_CODED_PHY                 0x0800  /*!< \brief LE Coded PHY supported */
#define HCI_LE_SUP_FEAT_LE_EXT_ADV                   0x1000  /*!< \brief LE Extended Advertising supported */
#define HCI_LE_SUP_FEAT_LE_PER_ADV                   0x2000  /*!< \brief LE Periodic Advertising supported */
#define HCI_LE_SUP_FEAT_CH_SEL_2                     0x4000  /*!< \brief Channel Selection Algorithm #2 supported */
#define HCI_LE_SUP_FEAT_LE_POWER_CLASS_1             0x8000  /*!< \brief LE Power Class 1 supported */
/**@}*/

/** \name Advertising command parameters
 *
 */
/**@{*/
#define HCI_ADV_MIN_INTERVAL                         0x0020  /*!< \brief Minimum advertising interval */
#define HCI_ADV_MAX_INTERVAL                         0x4000  /*!< \brief Maximum advertising interval */
#define HCI_ADV_DIRECTED_MAX_DURATION                0x0500  /*!< \brief Maximum high duty cycle connectable directed advertising duration */
#define HCI_ADV_TYPE_CONN_UNDIRECT                   0x00    /*!< \brief Connectable undirected advertising */
#define HCI_ADV_TYPE_CONN_DIRECT                     0x01    /*!< \brief Connectable directed high duty cycle advertising */
#define HCI_ADV_TYPE_DISC_UNDIRECT                   0x02    /*!< \brief Discoverable undirected advertising */
#define HCI_ADV_TYPE_NONCONN_UNDIRECT                0x03    /*!< \brief Nonconnectable undirected advertising */
#define HCI_ADV_TYPE_CONN_DIRECT_LO_DUTY             0x04    /*!< \brief Connectable directed low duty cycle advertising */
#define HCI_ADV_CHAN_37                              0x01    /*!< \brief Advertising channel 37 */
#define HCI_ADV_CHAN_38                              0x02    /*!< \brief Advertising channel 38 */
#define HCI_ADV_CHAN_39                              0x04    /*!< \brief Advertising channel 39 */
#define HCI_ADV_FILT_NONE                            0x00    /*!< \brief No scan request or connection filtering */
#define HCI_ADV_FILT_SCAN                            0x01    /*!< \brief White list filters scan requests */
#define HCI_ADV_FILT_CONN                            0x02    /*!< \brief White list filters connections */
#define HCI_ADV_FILT_ALL                             0x03    /*!< \brief White list filters scan req. and conn. */
/**@}*/

/** \name Scan command parameters
 *
 */
/**@{*/
#define HCI_SCAN_TYPE_PASSIVE                        0       /*!< \brief Passive scan */
#define HCI_SCAN_TYPE_ACTIVE                         1       /*!< \brief Active scan */
#define HCI_SCAN_INTERVAL_MIN                        0x0004  /*!< \brief Minimum scan interval */
#define HCI_SCAN_INTERVAL_MAX                        0x4000  /*!< \brief Maximum scan interval */
#define HCI_SCAN_INTERVAL_DEFAULT                    0x0010  /*!< \brief Default scan interval */
#define HCI_SCAN_WINDOW_MIN                          0x0004  /*!< \brief Minimum scan window */
#define HCI_SCAN_WINDOW_MAX                          0x4000  /*!< \brief Maximum scan window */
#define HCI_SCAN_WINDOW_DEFAULT                      0x0010  /*!< \brief Default scan window */
/**@}*/

/** \name Connection command parameters
 *
 */
/**@{*/
#define HCI_CONN_INTERVAL_MIN                        0x0006  /*!< \brief Minimum connection interval */
#define HCI_CONN_INTERVAL_MAX                        0x0C80  /*!< \brief Maximum connection interval */
#define HCI_CONN_LATENCY_MAX                         0x01F3  /*!< \brief Maximum connection latency */
#define HCI_SUP_TIMEOUT_MIN                          0x000A  /*!< \brief Minimum supervision timeout */
#define HCI_SUP_TIMEOUT_MAX                          0x0C80  /*!< \brief Maximum supervision timeout */
/**@}*/

/** \name Connection event parameters
 *
 */
/**@{*/
#define HCI_ROLE_MASTER                              0       /*!< \brief Role is master */
#define HCI_ROLE_SLAVE                               1       /*!< \brief Role is slave */
#define HCI_CLOCK_500PPM                             0x00    /*!< \brief 500 ppm clock accuracy */
#define HCI_CLOCK_250PPM                             0x01    /*!< \brief 250 ppm clock accuracy */
#define HCI_CLOCK_150PPM                             0x02    /*!< \brief 150 ppm clock accuracy */
#define HCI_CLOCK_100PPM                             0x03    /*!< \brief 100 ppm clock accuracy */
#define HCI_CLOCK_75PPM                              0x04    /*!< \brief 75 ppm clock accuracy */
#define HCI_CLOCK_50PPM                              0x05    /*!< \brief 50 ppm clock accuracy */
#define HCI_CLOCK_30PPM                              0x06    /*!< \brief 30 ppm clock accuracy */
#define HCI_CLOCK_20PPM                              0x07    /*!< \brief 20 ppm clock accuracy */
/**@}*/

/** \name Advertising report event parameters
 *
 */
/**@{*/
#define HCI_ADV_CONN_UNDIRECT                        0x00    /*!< \brief Connectable undirected advertising */
#define HCI_ADV_CONN_DIRECT                          0x01    /*!< \brief Connectable directed advertising */
#define HCI_ADV_DISC_UNDIRECT                        0x02    /*!< \brief Discoverable undirected advertising */
#define HCI_ADV_NONCONN_UNDIRECT                     0x03    /*!< \brief Non-connectable undirected advertising */
#define HCI_ADV_SCAN_RESPONSE                        0x04    /*!< \brief Scan response */
/**@}*/

/** \name Extended advertising data operations
 *
 */
/**@{*/
#define HCI_ADV_DATA_OP_FRAG_INTER                   0x00    /*!< \brief Intermediate fragment */
#define HCI_ADV_DATA_OP_FRAG_FIRST                   0x01    /*!< \brief First fragment */
#define HCI_ADV_DATA_OP_FRAG_LAST                    0x02    /*!< \brief Last fragment */
#define HCI_ADV_DATA_OP_COMP_FRAG                    0x03    /*!< \brief Complete extended advertising data */
#define HCI_ADV_DATA_OP_UNCHANGED_DATA               0x04    /*!< \brief Unchanged data (just update Advertising DID) */
/**@}*/

/** \name Advertising data fragment preference
 *
 */
/**@{*/
#define HCI_ADV_DATA_FRAG_PREF_FRAG                  0x00    /*!< \brief Controller may fragment all Host advertising data */
#define HCI_ADV_DATA_FRAG_PREF_NO_FRAG               0x01    /*!< \brief Controller should not fragment nor minimize fragmentation of Host advertising data */
/**@}*/

/** \name Number of advertising sets
 *
 */
/**@{*/
#define HCI_ADV_NUM_SETS_ALL_DISABLE                 0x00    /*!< \brief Disable all advertising sets */
/**@}*/

/** \name Maximum number of scanning or initiating PHYs
 *
 */
/**@{*/
#define HCI_MAX_NUM_PHYS                             3       /*!< \brief Maximum number of scanning or initiating PHYs */
/**@}*/

/** \name Advertising PHY values
 *
 */
/**@{*/
#define HCI_ADV_PHY_LE_1M                            0x01    /*!< \brief LE 1M PHY */
#define HCI_ADV_PHY_LE_2M                            0x02    /*!< \brief LE 2M PHY */
#define HCI_ADV_PHY_LE_CODED                         0x03    /*!< \brief LE Coded PHY */
/**@}*/

/** \name Scanner PHY value bits
 *
 */
/**@{*/
#define HCI_SCAN_PHY_LE_1M_BIT                       (1<<0)  /*!< \brief LE 1M PHY */
#define HCI_SCAN_PHY_LE_2M_BIT                       (1<<1)  /*!< \brief LE 2M PHY */
#define HCI_SCAN_PHY_LE_CODED_BIT                    (1<<2)  /*!< \brief LE Coded PHY  */
/**@}*/

/** \name Initiator PHY value bits
 *
 */
/**@{*/
#define HCI_INIT_PHY_LE_1M_BIT                       (1<<0)  /*!< \brief LE 1M PHY */
#define HCI_INIT_PHY_LE_2M_BIT                       (1<<1)  /*!< \brief LE 2M PHY */
#define HCI_INIT_PHY_LE_CODED_BIT                    (1<<2)  /*!< \brief LE Coded PHY  */
/**@}*/

/** \name Advertising event properties type bits
 *
 */
/**@{*/
#define HCI_ADV_PROP_CONN_ADV_BIT                    (1<<0)  /*!< \brief Connectable advertising bit */
#define HCI_ADV_PROP_SCAN_ADV_BIT                    (1<<1)  /*!< \brief Scannable advertising bit */
#define HCI_ADV_PROP_DIRECT_ADV_BIT                  (1<<2)  /*!< \brief Directed advertising bit */
#define HCI_ADV_PROP_CONN_DIRECT_ADV_BIT             (1<<3)  /*!< \brief High duty cycle connectable directed advertising bit */
#define HCI_ADV_PROP_USE_LEG_PDU_BIT                 (1<<4)  /*!< \brief Use legacy advertising PDUs bit */
#define HCI_ADV_PROP_OMIT_ADV_ADDR_BIT               (1<<5)  /*!< \brief Omit advertiser's address from all PDUs (anonymous advertising) bit */
#define HCI_ADV_PROP_INC_TX_PWR_BIT                  (1<<6)  /*!< \brief Include TxPower in extended header of advertising PDU bit */
/**@}*/

/** \name Advertising event properties for legacy PDUs
 *
 */
/**@{*/
#define HCI_ADV_PROP_LEG_CONN_UNDIRECT               0x13    /*!< \brief Connectable and scannable undirected advertising (00010011b) */
#define HCI_ADV_PROP_LEG_CONN_DIRECT                 0x1D    /*!< \brief Connectable directed high duty cycle advertising (00011101b) */
#define HCI_ADV_PROP_LEG_SCAN_UNDIRECT               0x12    /*!< \brief Scannable undirected advertising (00010010b) */
#define HCI_ADV_PROP_LEG_NONCONN_UNDIRECT            0x10    /*!< \brief Non-connectable and non-scannable undirected advertising (00010000b) */
#define HCI_ADV_PROP_LEG_CONN_DIRECT_LO_DUTY         0x15    /*!< \brief Connectable directed low duty cycle advertising (00010101b) */
/**@}*/

/** \name Extended advertising report event type bits
 *
 */
/**@{*/
#define HCI_ADV_RPT_CONN_ADV_BIT                     (1<<0)  /*!< \brief Connectable advertising event bit */
#define HCI_ADV_RPT_SCAN_ADV_BIT                     (1<<1)  /*!< \brief Scannable advertising event bit */
#define HCI_ADV_RPT_DIRECT_ADV_BIT                   (1<<2)  /*!< \brief Directed advertising event bit */
#define HCI_ADV_RPT_SCAN_RSP_BIT                     (1<<3)  /*!< \brief Scan response event bit */
#define HCI_ADV_RPT_LEG_ADV_BIT                      (1<<4)  /*!< \brief Legacy advertising PDU event bit */
#define HCI_ADV_RPT_DATA_STATUS_BITS                 (3<<5)  /*!< \brief Data status bits */
/**@}*/

/** \name Advertising report event types for legacy PDUs
 *
 */
/**@{*/
#define HCI_ADV_RPT_LEG_CONN_UNDIRECT                0x13    /*!< \brief Connectable and scannable undirected advertising (0010011b) */
#define HCI_ADV_RPT_LEG_CONN_DIRECT                  0x15    /*!< \brief Connectable directed advertising (0010101b) */
#define HCI_ADV_RPT_LEG_SCAN_UNDIRECT                0x12    /*!< \brief Scannable undirected advertising (0010010b) */
#define HCI_ADV_RPT_LEG_NONCONN_UNDIRECT             0x10    /*!< \brief Non-connectable and non-scannable undirected advertising (0010000b) */
#define HCI_ADV_RPT_LEG_CONN_UNDIRECT_SCAN_RSP       0x1B    /*!< \brief Scan response to connectable and scannable undirected advertising (0011011b) */
#define HCI_ADV_RPT_LEG_SCAN_UNDIRECT_SCAN_RSP       0x1A    /*!< \brief Scan response to scannable undirected advertising (0011010b) */
/**@}*/

/** \name Advertising report data status
 *
 */
/**@{*/
#define HCI_ADV_RPT_DATA_CMPL                        0x00    /*!< \brief Data complete */
#define HCI_ADV_RPT_DATA_INCMPL_MORE                 0x01    /*!< \brief Data incomplete, more date to come */
#define HCI_ADV_RPT_DATA_INCMPL_TRUNC                0x02    /*!< \brief Data incomplete, data truncated, no more date to come */
/**@}*/

/** \name Extended advertising report event primary PHY values
 *
 */
/**@{*/
#define HCI_ADV_RPT_PHY_PRIM_LE_1M                   0x01    /*!< \brief Advertiser PHY is LE 1M */
#define HCI_ADV_RPT_PHY_PRIM_LE_CODED                0x03    /*!< \brief Advertiser PHY is LE Coded */
/**@}*/

/** \name Extended advertising report event seconday PHY values
 *
 */
/**@{*/
#define HCI_ADV_RPT_PHY_SEC_NONE                     0x00    /*!< \brief No packets on seconday advertising channel */
#define HCI_ADV_RPT_PHY_SEC_LE_1M                    0x01    /*!< \brief Advertiser PHY is LE 1M */
#define HCI_ADV_RPT_PHY_SEC_LE_2M                    0x02    /*!< \brief Advertiser PHY is LE 2M */
#define HCI_ADV_RPT_PHY_SEC_LE_CODED                 0x03    /*!< \brief Advertiser PHY is LE Coded */
/**@}*/

/** \name Channel selection algorithm used
 *
 */
/**@{*/
#define HCI_CH_SEL_ALGO_1                            0x00   /*!< \brief LE channel selection algorithm #1 used */
#define HCI_CH_SEL_ALGO_2                            0x01   /*!< \brief LE channel selection algorithm #2 used */
/**@}*/

/** \name Minimum number of used channels
 *
 */
/**@{*/
#define HCI_MIN_NUM_OF_USED_CHAN                     8      /*!< \brief Minimum number of used channels */
/**@}*/

/** \name Misc command parameters
 *
 */
/**@{*/
#define HCI_READ_TX_PWR_CURRENT                      0       /*!< \brief Read current tx power */
#define HCI_READ_TX_PWR_MAX                          1       /*!< \brief Read maximum tx power */
#define HCI_TX_PWR_MIN                               -30     /*!< \brief Minimum tx power dBm */
#define HCI_TX_PWR_MAX                               20      /*!< \brief Maximum tx power dBm */
#define HCI_TX_PWR_NO_PREFERENCE                     127     /*!< \brief Tx power no preference */
#define HCI_VERSION                                  6       /*!< \brief HCI specification version */
#define HCI_RSSI_MIN                                 -127    /*!< \brief Minimum RSSI dBm */
#define HCI_RSSI_MAX                                 20      /*!< \brief Maximum RSSI dBm */
#define HCI_ADDR_TYPE_PUBLIC                         0       /*!< \brief Public device address */
#define HCI_ADDR_TYPE_RANDOM                         1       /*!< \brief Random device address */
#define HCI_ADDR_TYPE_PUBLIC_IDENTITY                2       /*!< \brief Public identity address */
#define HCI_ADDR_TYPE_RANDOM_IDENTITY                3       /*!< \brief Random identity address */
#define HCI_ADDR_TYPE_ANONYMOUS                      0xFF    /*!< \brief Anonymous device address */
#define HCI_FILT_NONE                                0       /*!< \brief Accept all advertising packets */
#define HCI_FILT_WHITE_LIST                          1       /*!< \brief Accept from While List only */
#define HCI_FILT_RES_INIT                            2       /*!< \brief Accept directed advertisements with RPAs */
#define HCI_FILT_WHITE_LIST_RES_INIT                 3       /*!< \brief Accept from White List or directed advertisements with RPAs */
#define HCI_FILT_PER_ADV_PARAM                       0       /*!< \brief Listen to advertiser specified by create sync command parameters */
#define HCI_FILT_PER_ADV_LIST                        1       /*!< \brief Listen to advertiser from Periodic Advertiser List only */
#define HCI_ROLE_MASTER                              0       /*!< \brief Role is master */
#define HCI_ROLE_SLAVE                               1       /*!< \brief Role is slave */
#define HCI_PRIV_MODE_NETWORK                        0x00    /*!< \brief Network privacy mode (default) */
#define HCI_PRIV_MODE_DEVICE                         0x01    /*!< \brief Device privacy mode */
/**@}*/

/** \name PHY types
 *
 */
/**@{*/
#define HCI_PHY_NONE                                 0x00    /*!< \brief No selected PHY */
#define HCI_PHY_LE_1M_BIT                            (1<<0)  /*!< \brief LE 1M PHY */
#define HCI_PHY_LE_2M_BIT                            (1<<1)  /*!< \brief LE 2M PHY */
#define HCI_PHY_LE_CODED_BIT                         (1<<2)  /*!< \brief LE Coded PHY */
/**@}*/

/** \name All PHYs preference
 *
 */
/**@{*/
#define HCI_ALL_PHY_ALL_PREFERENCES                  0x00    /*!< \brief All PHY preferences */
#define HCI_ALL_PHY_TX_PREFERENCE_BIT                (1<<0)  /*!< \brief Tx PHY preference */
#define HCI_ALL_PHY_RX_PREFERENCE_BIT                (1<<1)  /*!< \brief Rx PHY preference */
/**@}*/

/** \name PHY options
 *
 */
/**@{*/
#define HCI_PHY_OPTIONS_NONE                         0x00    /*!< \brief No preferences */
#define HCI_PHY_OPTIONS_S2_PREFERRED                 0x01    /*!< \brief S=2 coding preferred when transmitting on LE Coded PHY */
#define HCI_PHY_OPTIONS_S8_PREFERRED                 0x02    /*!< \brief S=8 coding preferred when transmitting on LE Coded PHY */
/**@}*/

/** \name Parameter lengths
 *
 */
/**@{*/
#define HCI_EVT_MASK_LEN                             8       /*!< \brief Length of event mask byte array */
#define HCI_EVT_MASK_PAGE_2_LEN                      8       /*!< \brief Length of event mask page 2 byte array */
#define HCI_LE_EVT_MASK_LEN                          8       /*!< \brief Length of LE event mask byte array */
#define HCI_FEAT_LEN                                 8       /*!< \brief Length of features byte array */
#define HCI_ADV_DATA_LEN                             31      /*!< \brief Length of advertising data */
#define HCI_SCAN_DATA_LEN                            31      /*!< \brief Length of scan response data */
#define HCI_EXT_ADV_DATA_LEN                         251     /*!< \brief Length of extended advertising data */
#define HCI_PER_ADV_DATA_LEN                         252     /*!< \brief Length of periodic advertising data */
#define HCI_EXT_ADV_RPT_DATA_LEN                     229     /*!< \brief Length of extended advertising report data */
#define HCI_PER_ADV_RPT_DATA_LEN                     248     /*!< \brief Length of periodic advertising report data */
#define HCI_CHAN_MAP_LEN                             5       /*!< \brief Length of channel map byte array */
#define HCI_KEY_LEN                                  16      /*!< \brief Length of encryption key */
#define HCI_ENCRYPT_DATA_LEN                         16      /*!< \brief Length of data used in encryption */
#define HCI_RAND_LEN                                 8       /*!< \brief Length of random number */
#define HCI_LE_STATES_LEN                            8       /*!< \brief Length of LE states byte array */
#define HCI_P256_KEY_LEN                             64      /*!< \brief Length of P256 key */
#define HCI_DH_KEY_LEN                               32      /*!< \brief Length of DH Key */

#define HCI_EXT_ADV_RPT_DATA_LEN_OFFSET              23      /*!< \brief Length field offset of extended advertising report data */
#define HCI_PER_ADV_RPT_DATA_LEN_OFFSET              6       /*!< \brief Length field offset of periodic advertising report data */
/**@}*/

/** \name Company ID
 *
 */
/**@{*/
#define HCI_ID_ARM                                   0x005F  /*!< \brief ARM Ltd. company ID */
/**@}*/

/** \name Manufacturer location in Local version
 *
 */
/**@{*/
#define HCI_LOCAL_VER_MANUFACTURER_POS              4
/**@}*/

/* \} */    /* STACK_HCI_API */

#ifdef __cplusplus
};
#endif

#endif /* HCI_DEFS_H */
