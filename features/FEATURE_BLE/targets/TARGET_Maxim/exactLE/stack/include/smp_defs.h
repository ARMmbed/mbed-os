/*************************************************************************************************/
/*!
 *  \file   smp_defs.h
 *        
 *  \brief  Security manager constants and definitions from the Bluetooth specification.
 *
 *          $Date: 2011-10-14 21:35:03 -0700 (Fri, 14 Oct 2011) $
 *          $Revision: 191 $
 *  
 *  Copyright (c) 2010-2016 ARM Limited. All rights reserved.
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
#ifndef SMP_DEFS_H
#define SMP_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! PDU format */
#define SMP_HDR_LEN                   1         /*! Attribute PDU header length */

/*! Protocol timeout */
#define SMP_TIMEOUT                   30        /*! Protocol timeout in seconds */

/*! Encryption key size */
#define SMP_KEY_SIZE_MAX              16        /*! Maximum encryption key size */
#define SMP_KEY_SIZE_MIN              7         /*! Minimum encryption key size */

/*! OOB and PIN data lengths in bytes */
#define SMP_OOB_LEN                   16
#define SMP_PIN_LEN                   3

/*! Error codes */
#define SMP_ERR_PASSKEY_ENTRY         0x01      /*! User input of passkey failed */
#define SMP_ERR_OOB                   0x02      /*! OOB data is not available */
#define SMP_ERR_AUTH_REQ              0x03      /*! Authentication requirements cannot be met */
#define SMP_ERR_CONFIRM_VALUE         0x04      /*! Confirm value does not match */
#define SMP_ERR_PAIRING_NOT_SUP       0x05      /*! Pairing is not supported by the device */
#define SMP_ERR_ENC_KEY_SIZE          0x06      /*! Insufficient encryption key size */
#define SMP_ERR_COMMAND_NOT_SUP       0x07      /*! Command not supported */
#define SMP_ERR_UNSPECIFIED           0x08      /*! Unspecified reason */
#define SMP_ERR_ATTEMPTS              0x09      /*! Repeated attempts */
#define SMP_ERR_INVALID_PARAM         0x0A      /*! Invalid parameter or command length */

/*! Proprietary internal error codes */
#define SMP_ERR_MEMORY                0xE0      /*! Out of memory */
#define SMP_ERR_TIMEOUT               0xE1      /*! Transaction timeout */

/*! Command codes */
#define SMP_CMD_PAIR_REQ              0x01      /*! Pairing Request */
#define SMP_CMD_PAIR_RSP              0x02      /*! Pairing Response */
#define SMP_CMD_PAIR_CNF              0x03      /*! Pairing Confirm */
#define SMP_CMD_PAIR_RAND             0x04      /*! Pairing Random */
#define SMP_CMD_PAIR_FAIL             0x05      /*! Pairing Failed */
#define SMP_CMD_ENC_INFO              0x06      /*! Encryption Information */
#define SMP_CMD_MASTER_ID             0x07      /*! Master Identification */
#define SMP_CMD_ID_INFO               0x08      /*! Identity Information */
#define SMP_CMD_ID_ADDR_INFO          0x09      /*! Identity Address Information */
#define SMP_CMD_SIGN_INFO             0x0A      /*! Signing Information */
#define SMP_CMD_SECURITY_REQ          0x0B      /*! Security Request */
#define SMP_CMD_MAX                   0x0C      /*! Command code maximum */

/*! Command packet lengths */
#define SMP_PAIR_REQ_LEN              7
#define SMP_PAIR_RSP_LEN              7
#define SMP_PAIR_CNF_LEN              17
#define SMP_PAIR_RAND_LEN             17
#define SMP_PAIR_FAIL_LEN             2
#define SMP_ENC_INFO_LEN              17
#define SMP_MASTER_ID_LEN             11
#define SMP_ID_INFO_LEN               17
#define SMP_ID_ADDR_INFO_LEN          8
#define SMP_SIGN_INFO_LEN             17
#define SMP_SECURITY_REQ_LEN          2

/*! I/O capabilities */
#define SMP_IO_DISP_ONLY              0x00      /*! DisplayOnly */
#define SMP_IO_DISP_YES_NO            0x01      /*! DisplayYesNo */
#define SMP_IO_KEY_ONLY               0x02      /*! KeyboardOnly */
#define SMP_IO_NO_IN_NO_OUT           0x03      /*! NoInputNoOutput */
#define SMP_IO_KEY_DISP               0x04      /*! KeyboardDisplay */

/*! OOB data present */
#define SMP_OOB_DATA_NONE             0x00
#define SMP_OOB_DATA_PRESENT          0x01

/*! Authentication/security properties bit mask */
#define SMP_AUTH_BOND_MASK            0x03      /*! Mask for bonding bits */
#define SMP_AUTH_BOND_FLAG            0x01      /*! Bonding requested */
#define SMP_AUTH_MITM_FLAG            0x04      /*! MITM (authenticated pairing) requested */

/*! Key distribution bit mask */
#define SMP_KEY_DIST_ENC              0x01      /*! Distribute LTK */
#define SMP_KEY_DIST_ID               0x02      /*! Distribute IRK */
#define SMP_KEY_DIST_SIGN             0x04      /*! Distribute CSRK */
#define SMP_KEY_DIST_MASK             (SMP_KEY_DIST_ENC | SMP_KEY_DIST_ID | SMP_KEY_DIST_SIGN)

/*! Various parameter lengths */
#define SMP_RAND_LEN                  16
#define SMP_CONFIRM_LEN               16
#define SMP_KEY_LEN                   16
#define SMP_RAND8_LEN                 8

#ifdef __cplusplus
};
#endif

#endif /* SMP_DEFS_H */
