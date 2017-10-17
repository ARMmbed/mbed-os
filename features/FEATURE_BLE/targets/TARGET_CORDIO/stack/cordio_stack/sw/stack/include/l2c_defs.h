/*************************************************************************************************/
/*!
 *  \file   l2c_defs.h
 *
 *  \brief  L2CAP constants and definitions from the Bluetooth specification.
 *
 *          $Date: 2016-12-28 16:12:14 -0600 (Wed, 28 Dec 2016) $
 *          $Revision: 10805 $
 *
 *  Copyright (c) 2009-2017 ARM Ltd., all rights reserved.
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
#ifndef L2C_DEFS_H
#define L2C_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! Packet definitions */
#define L2C_HDR_LEN                   4         /*! L2CAP packet header length */
#define L2C_MIN_MTU                   23        /*! Minimum packet payload MTU for LE */
#define L2C_SIG_HDR_LEN               4         /*! L2CAP signaling command header length */
#define L2C_LE_SDU_HDR_LEN            2         /*! L2CAP LE SDU data header length */

/*! Start of L2CAP payload in an HCI ACL packet buffer */
#define L2C_PAYLOAD_START             (HCI_ACL_HDR_LEN + L2C_HDR_LEN)

/*! L2CAP signaling packet base length, including HCI header */
#define L2C_SIG_PKT_BASE_LEN          (HCI_ACL_HDR_LEN + L2C_HDR_LEN + L2C_SIG_HDR_LEN)

/*! L2CAP LE SDU packet base length, including HCI header */
#define L2C_LE_SDU_PKT_BASE_LEN       (HCI_ACL_HDR_LEN + L2C_HDR_LEN + L2C_LE_SDU_HDR_LEN)

/*! Signaling packet parameter lengths */
#define L2C_SIG_CONN_UPDATE_REQ_LEN   8
#define L2C_SIG_CONN_UPDATE_RSP_LEN   2
#define L2C_SIG_CMD_REJ_LEN           2
#define L2C_SIG_DISCONN_REQ_LEN       4
#define L2C_SIG_DISCONN_RSP_LEN       4
#define L2C_SIG_LE_CONN_REQ_LEN       10
#define L2C_SIG_LE_CONN_RSP_LEN       10
#define L2C_SIG_FLOW_CTRL_CREDIT_LEN  4

/*! Connection identifiers */
#define L2C_CID_ATT                   0x0004    /*! CID for attribute protocol */
#define L2C_CID_LE_SIGNALING          0x0005    /*! CID for LE signaling */
#define L2C_CID_SMP                   0x0006    /*! CID for security manager protocol */

/*! Signaling codes */
#define L2C_SIG_CMD_REJ               0x01      /*! Comand reject */
#define L2C_SIG_DISCONNECT_REQ        0x06      /*! Disconnect request */
#define L2C_SIG_DISCONNECT_RSP        0x07      /*! Disconnect response */
#define L2C_SIG_CONN_UPDATE_REQ       0x12      /*! Connection parameter update request */
#define L2C_SIG_CONN_UPDATE_RSP       0x13      /*! Connection parameter update response */
#define L2C_SIG_LE_CONNECT_REQ        0x14      /*! LE credit based connection request */
#define L2C_SIG_LE_CONNECT_RSP        0x15      /*! LE credit based connection response */
#define L2C_SIG_FLOW_CTRL_CREDIT      0x16      /*! LE flow control credit */

/*! Signaling response code flag */
#define L2C_SIG_RSP_FLAG              0x01

/*! Command reject reason codes */
#define L2C_REJ_NOT_UNDERSTOOD        0x0000    /*! Command not understood */
#define L2C_REJ_MTU_EXCEEDED          0x0001    /*! Signaling MTU exceeded */
#define L2C_REJ_INVALID_CID           0x0002    /*! Invalid CID in request */

/*! Connection parameter update result */
#define L2C_CONN_PARAM_ACCEPTED       0x0000    /*! Connection parameters accepted */
#define L2C_CONN_PARAM_REJECTED       0x0001    /*! Connection parameters rejected */

/*! LE connection result */
#define L2C_CONN_SUCCESS              0x0000    /*! Connection successful */
#define L2C_CONN_NONE                 0x0001    /*! No connection result value available */
#define L2C_CONN_FAIL_PSM             0x0002    /*! Connection refused LE_PSM not supported */
#define L2C_CONN_FAIL_RES             0x0004    /*! Connection refused no resources available */
#define L2C_CONN_FAIL_AUTH            0x0005    /*! Connection refused insufficient authentication */
#define L2C_CONN_FAIL_AUTHORIZ        0x0006    /*! Connection refused insufficient authorization */
#define L2C_CONN_FAIL_KEY_SIZE        0x0007    /*! Connection refused insufficient encryption key size */
#define L2C_CONN_FAIL_ENC             0x0008    /*! Connection Refused insufficient encryption */
#define L2C_CONN_FAIL_INVALID_SCID    0x0009    /*! Connection refused invalid source CID */
#define L2C_CONN_FAIL_ALLOCATED_SCID  0x000A    /*! Connection refused source CID already allocated */
#define L2C_CONN_FAIL_UNACCEPT_PARAM  0x000B    /*! Connection refused unacceptable parameters */

/*! LE connection result proprietary codes */
#define L2C_CONN_FAIL_TIMEOUT         0xF000    /*! Request timeout */

/*! Signaling parameter value ranges */
#define L2C_PSM_MIN                   0x0001
#define L2C_PSM_MAX                   0x00FF
#define L2C_CID_DYN_MIN               0x0040
#define L2C_CID_DYN_MAX               0x007F
#define L2C_MTU_MIN                   0x0017
#define L2C_MPS_MIN                   0x0017
#define L2C_MPS_MAX                   0xFFFD
#define L2C_CREDITS_MAX               0xFFFF

#ifdef __cplusplus
};
#endif

#endif /* L2C_DEFS_H */
