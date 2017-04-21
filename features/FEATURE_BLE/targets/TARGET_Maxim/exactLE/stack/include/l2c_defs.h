/*************************************************************************************************/
/*!
 *  \file   l2c_defs.h
 *        
 *  \brief  L2CAP constants and definitions from the Bluetooth specification.
 *
 *          $Date: 2011-10-14 21:35:03 -0700 (Fri, 14 Oct 2011) $
 *          $Revision: 191 $
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

/*! Start of L2CAP payload in an HCI ACL packet buffer */
#define L2C_PAYLOAD_START             (HCI_ACL_HDR_LEN + L2C_HDR_LEN)

/*! L2CAP signaling packet base length, including HCI header */
#define L2C_SIG_PKT_BASE_LEN          (HCI_ACL_HDR_LEN + L2C_HDR_LEN + L2C_SIG_HDR_LEN)

/*! Signaling packet parameter lengths */
#define L2C_SIG_CONN_UPDATE_REQ_LEN   8
#define L2C_SIG_CONN_UPDATE_RSP_LEN   2
#define L2C_SIG_CMD_REJ_LEN           2

/*! Connection identifiers */
#define L2C_CID_ATT                   0x0004    /*! CID for attribute protocol */
#define L2C_CID_LE_SIGNALING          0x0005    /*! CID for LE signaling */
#define L2C_CID_SMP                   0x0006    /*! CID for security manager protocol */

/*! Signaling codes */
#define L2C_SIG_CMD_REJ               0x01      /*! Comand reject */
#define L2C_SIG_CONN_UPDATE_REQ       0x12      /*! Connection parameter update request */
#define L2C_SIG_CONN_UPDATE_RSP       0x13      /*! Connection parameter update response */

/*! Signaling response code flag */
#define L2C_SIG_RSP_FLAG              0x01

/*! Command reject reason codes */
#define L2C_REJ_NOT_UNDERSTOOD        0x0000    /*! Command not understood */
#define L2C_REJ_MTU_EXCEEDED          0x0001    /*! Signaling MTU exceeded */
#define L2C_REJ_INVALID_CID           0x0002    /*! Invalid CID in request */

/*! Connection parameter update result */
#define L2C_CONN_PARAM_ACCEPTED       0x0000    /*! Connection parameters accepted */
#define L2C_CONN_PARAM_REJECTED       0x0001    /*! Connection parameters rejected */


#ifdef __cplusplus
};
#endif

#endif /* L2C_DEFS_H */
