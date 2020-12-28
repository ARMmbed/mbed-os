/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  L2CAP constants and definitions from the Bluetooth specification.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *      http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/
#ifndef L2C_DEFS_H
#define L2C_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup STACK_L2CAP_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/** \name L2CAP Packet Constants
 *
 */
/**@{*/
#define L2C_HDR_LEN                   4         /*!< \brief L2CAP packet header length */
#define L2C_MIN_MTU                   23        /*!< \brief Minimum packet payload MTU for LE */
#define L2C_SIG_HDR_LEN               4         /*!< \brief L2CAP signaling command header length */
#define L2C_LE_SDU_HDR_LEN            2         /*!< \brief L2CAP LE SDU data header length */
/**@}*/

/*! \brief Max number of channels per enhanced connection request */
#define L2C_MAX_EN_CHAN               5

/*! \brief Start of L2CAP payload in an HCI ACL packet buffer */
#define L2C_PAYLOAD_START             (HCI_ACL_HDR_LEN + L2C_HDR_LEN)

/*! \brief L2CAP signaling packet base length, including HCI header */
#define L2C_SIG_PKT_BASE_LEN          (HCI_ACL_HDR_LEN + L2C_HDR_LEN + L2C_SIG_HDR_LEN)

/*! \brief L2CAP LE SDU packet base length, including HCI header */
#define L2C_LE_SDU_PKT_BASE_LEN       (HCI_ACL_HDR_LEN + L2C_HDR_LEN + L2C_LE_SDU_HDR_LEN)

/** \name L2CAP Parameter Lengths
 * Signaling packet parameter lengths
 */
/**@{*/
#define L2C_SIG_CONN_UPDATE_REQ_LEN   8   /*!< \brief Connection update request length. */
#define L2C_SIG_CONN_UPDATE_RSP_LEN   2   /*!< \brief Connection update response length. */
#define L2C_SIG_CMD_REJ_LEN           2   /*!< \brief Command reject length. */
#define L2C_SIG_DISCONN_REQ_LEN       4   /*!< \brief Disconnection request length. */
#define L2C_SIG_DISCONN_RSP_LEN       4   /*!< \brief Disconnection response length. */
#define L2C_SIG_LE_CONN_REQ_LEN       10  /*!< \brief LE connection request length. */
#define L2C_SIG_LE_CONN_RSP_LEN       10  /*!< \brief LE connection response length. */
#define L2C_SIG_FLOW_CTRL_CREDIT_LEN  4   /*!< \brief Flow control credit lenghth. */
#define L2C_SIG_EN_CONNECT_REQ_LEN    8   /*!< \brief Enhanced credit based connection request */
#define L2C_SIG_EN_CONNECT_RSP_LEN    8   /*!< \brief Enhanced credit based connection response */
#define L2C_SIG_EN_RECONFIG_REQ_LEN   4   /*!< \brief Enhanced credit based reconfiguration request */
#define L2C_SIG_EN_RECONFIG_RSP_LEN   2   /*!< \brief Enhanced credit based reconfiguration response */
/**@}*/

/** \name L2CAP Connection Identifiers
 * BLE Defined Connection Identifiers (CID)
 */
/**@{*/
#define L2C_CID_ATT                   0x0004    /*!< \brief CID for attribute protocol */
#define L2C_CID_LE_SIGNALING          0x0005    /*!< \brief CID for LE signaling */
#define L2C_CID_SMP                   0x0006    /*!< \brief CID for security manager protocol */
/**@}*/

/** \name L2CAP Signaling Codes
 *
 */
/**@{*/
#define L2C_SIG_CMD_REJ               0x01      /*!< \brief Comand reject */
#define L2C_SIG_DISCONNECT_REQ        0x06      /*!< \brief Disconnect request */
#define L2C_SIG_DISCONNECT_RSP        0x07      /*!< \brief Disconnect response */
#define L2C_SIG_CONN_UPDATE_REQ       0x12      /*!< \brief Connection parameter update request */
#define L2C_SIG_CONN_UPDATE_RSP       0x13      /*!< \brief Connection parameter update response */
#define L2C_SIG_LE_CONNECT_REQ        0x14      /*!< \brief LE credit based connection request */
#define L2C_SIG_LE_CONNECT_RSP        0x15      /*!< \brief LE credit based connection response */
#define L2C_SIG_FLOW_CTRL_CREDIT      0x16      /*!< \brief LE flow control credit */
#define L2C_SIG_EN_CONNECT_REQ        0x17      /*!< \brief Enhanced credit based connection request */
#define L2C_SIG_EN_CONNECT_RSP        0x18      /*!< \brief Enhanced credit based connection response */
#define L2C_SIG_EN_RECONFIG_REQ       0x19      /*!< \brief Enhanced credit based reconfiguration request */
#define L2C_SIG_EN_RECONFIG_RSP       0x1A      /*!< \brief Enhanced credit based reconfiguration response */
/**@}*/

/*! \brief Signaling response code flag */
#define L2C_SIG_RSP_FLAG              0x01

/** \name L2CAP Command Rejection Codes
 * BLE defined Command rejection reason codes
 */
/**@{*/
#define L2C_REJ_NOT_UNDERSTOOD        0x0000    /*!< \brief Command not understood */
#define L2C_REJ_MTU_EXCEEDED          0x0001    /*!< \brief Signaling MTU exceeded */
#define L2C_REJ_INVALID_CID           0x0002    /*!< \brief Invalid CID in request */
/**@}*/

/** \name L2CAP Connection Parameter Update Result Codes
 * BLE defined result codes
 */
/**@{*/
#define L2C_CONN_PARAM_ACCEPTED       0x0000    /*!< \brief Connection parameters accepted */
#define L2C_CONN_PARAM_REJECTED       0x0001    /*!< \brief Connection parameters rejected */
/**@}*/

/** \name L2CAP Connection Result Codes
 * BLE defined result codes
 */
/**@{*/
#define L2C_CONN_SUCCESS              0x0000    /*!< \brief Connection successful */
#define L2C_CONN_NONE                 0x0001    /*!< \brief No connection result value available */
#define L2C_CONN_FAIL_PSM             0x0002    /*!< \brief Connection refused LE_PSM not supported */
#define L2C_CONN_FAIL_RES             0x0004    /*!< \brief Connection refused no resources available */
#define L2C_CONN_FAIL_AUTH            0x0005    /*!< \brief Connection refused insufficient authentication */
#define L2C_CONN_FAIL_AUTHORIZ        0x0006    /*!< \brief Connection refused insufficient authorization */
#define L2C_CONN_FAIL_KEY_SIZE        0x0007    /*!< \brief Connection refused insufficient encryption key size */
#define L2C_CONN_FAIL_ENC             0x0008    /*!< \brief Connection Refused insufficient encryption */
#define L2C_CONN_FAIL_INVALID_SCID    0x0009    /*!< \brief Connection refused invalid source CID */
#define L2C_CONN_FAIL_ALLOCATED_SCID  0x000A    /*!< \brief Connection refused source CID already allocated */
#define L2C_CONN_FAIL_UNACCEPT_PARAM  0x000B    /*!< \brief Connection refused unacceptable parameters */
#define L2C_CONN_FAIL_INVALID_PARAM   0x000C    /*!< \brief Connection refused invalid parameters */
/**@}*/

/** \name L2CAP Interal Connection Result Codes
 * Proprietary codes not sent in any L2CAP packet.
 */
/**@{*/
#define L2C_CONN_FAIL_TIMEOUT         0xF000    /*!< \brief Request timeout */
/**@}*/

/** \name L2CAP Signaling Parameter Value Ranges
 *
 */
/**@{*/
#define L2C_PSM_MIN                   0x0001  /*!< \brief PSM minimum. */
#define L2C_PSM_MAX                   0x00FF  /*!< \brief PSM maximum. */
#define L2C_CID_DYN_MIN               0x0040  /*!< \brief CID dynamic minimum. */
#define L2C_CID_DYN_MAX               0x007F  /*!< \brief CID dynamic maximum. */
#define L2C_MTU_MIN                   0x0017  /*!< \brief MTU minimum. */
#define L2C_MPS_MIN                   0x0017  /*!< \brief MPS minimum. */
#define L2C_MPS_MAX                   0xFFFD  /*!< \brief MPS maximum. */
#define L2C_CREDITS_MAX               0xFFFF  /*!< \brief Credits maximum. */
/**@}*/

/** \name L2CAP Enhanced Connection Reconfigure Result Codes
 *
 */
/**@{*/
#define L2C_RECONFIG_FAIL_MTU         0x0001  /*!< \brief Enhanced Reconfiguration refuded - cannot reduce MTU. */
#define L2C_RECONFIG_FAIL_MPS         0x0002  /*!< \brief Enhanced Reconfiguration refuded - cannot reduce MPS on more than one channel. */
#define L2C_RECONFIG_FAIL_CID         0x0003  /*!< \brief Enhanced Reconfiguration refuded - invalid CID. */
#define L2C_RECONFIG_FAIL_PARAM       0x0004  /*!< \brief Enhanced Reconfiguration refuded - unacceptable parameters. */
/**@}*/

/*! \} */    /*! STACK_L2CAP_API */

#ifdef __cplusplus
};
#endif

#endif /* L2C_DEFS_H */
