/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief Security manager constants and definitions from the Bluetooth specification.
 */
/*************************************************************************************************/
#ifndef SMP_DEFS_H
#define SMP_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup STACK_SMP_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief PDU format */
#define SMP_HDR_LEN                   1         /*!< \brief Attribute PDU header length. */

/*! \brief Protocol timeout */
#define SMP_TIMEOUT                   30        /*!< \brief Protocol timeout in seconds. */

/** \name SMP Encryption Key Size
 *
 */
/**@{*/
#define SMP_KEY_SIZE_MAX              16        /*!< \brief Maximum encryption key size. */
#define SMP_KEY_SIZE_MIN              7         /*!< \brief Minimum encryption key size. */
/**@}*/

#define SMP_OOB_LEN                   16        /*!< \brief OOB Data length in bytes. */
#define SMP_PIN_LEN                   3         /*!< \brief Passkey Pin lenght in bytes. */

/** \name SMP Error Codes
 * SMP PDU status codes
 */
/**@{*/
#define SMP_ERR_PASSKEY_ENTRY         0x01      /*!< \brief User input of passkey failed. */
#define SMP_ERR_OOB                   0x02      /*!< \brief OOB data is not available. */
#define SMP_ERR_AUTH_REQ              0x03      /*!< \brief Authentication requirements cannot be met. */
#define SMP_ERR_CONFIRM_VALUE         0x04      /*!< \brief Confirm value does not match. */
#define SMP_ERR_PAIRING_NOT_SUP       0x05      /*!< \brief Pairing is not supported by the device. */
#define SMP_ERR_ENC_KEY_SIZE          0x06      /*!< \brief Insufficient encryption key size. */
#define SMP_ERR_COMMAND_NOT_SUP       0x07      /*!< \brief Command not supported. */
#define SMP_ERR_UNSPECIFIED           0x08      /*!< \brief Unspecified reason. */
#define SMP_ERR_ATTEMPTS              0x09      /*!< \brief Repeated attempts. */
#define SMP_ERR_INVALID_PARAM         0x0A      /*!< \brief Invalid parameter or command length. */
#define SMP_ERR_DH_KEY_CHECK          0x0B      /*!< \brief DH Key check did not match. */
#define SMP_ERR_NUMERIC_COMPARISON    0x0C      /*!< \brief Numeric comparison did not match. */
#define SMP_ERR_BR_EDR_IN_PROGRESS    0x0D      /*!< \brief BR/EDR in progress. */
#define SMP_ERR_CROSS_TRANSPORT       0x0E      /*!< \brief BR/EDR cross transport key generation not allowed. */
/**@}*/

/** \name Proprietary Error Codes
 * Internal error codes not sent in any SMP PDU.
 */
/**@{*/
#define SMP_ERR_MEMORY                0xE0      /*!< \brief Out of memory. */
#define SMP_ERR_TIMEOUT               0xE1      /*!< \brief Transaction timeout. */
/**@}*/

/** \name SMP PDU Codes
 * SMP PDU Code describing command received or sent.
 */
/**@{*/
#define SMP_CMD_PAIR_REQ              0x01      /*!< \brief Pairing request. */
#define SMP_CMD_PAIR_RSP              0x02      /*!< \brief Pairing response. */
#define SMP_CMD_PAIR_CNF              0x03      /*!< \brief Pairing confirm. */
#define SMP_CMD_PAIR_RAND             0x04      /*!< \brief Pairing random. */
#define SMP_CMD_PAIR_FAIL             0x05      /*!< \brief Pairing failed. */
#define SMP_CMD_ENC_INFO              0x06      /*!< \brief Encryption information. */
#define SMP_CMD_MASTER_ID             0x07      /*!< \brief Master identification. */
#define SMP_CMD_ID_INFO               0x08      /*!< \brief Identity information. */
#define SMP_CMD_ID_ADDR_INFO          0x09      /*!< \brief Identity address information. */
#define SMP_CMD_SIGN_INFO             0x0A      /*!< \brief Signing information. */
#define SMP_CMD_SECURITY_REQ          0x0B      /*!< \brief Security fequest. */
#define SMP_CMD_PUBLIC_KEY            0x0C      /*!< \brief Public Kkey. */
#define SMP_CMD_DHKEY_CHECK           0x0D      /*!< \brief DH Key check. */
#define SMP_CMD_KEYPRESS              0x0E      /*!< \brief User key press. */
#define SMP_CMD_MAX                   0x0F      /*!< \brief Command code maximum. */
/**@}*/

/** \name SMP PDU Packet Lengths
 * Fixed length of the PDU to be sent.
 */
/**@{*/
#define SMP_PAIR_REQ_LEN              7                          /*!< \brief Pairing request message length. */
#define SMP_PAIR_RSP_LEN              7                          /*!< \brief Pairing response message length. */
#define SMP_PAIR_CNF_LEN              17                         /*!< \brief Pairing confirmation message length. */
#define SMP_PAIR_RAND_LEN             17                         /*!< \brief Pairing random message length. */
#define SMP_PAIR_FAIL_LEN             2                          /*!< \brief Pairing fail message length. */
#define SMP_ENC_INFO_LEN              17                         /*!< \brief Encryption information message length. */
#define SMP_MASTER_ID_LEN             11                         /*!< \brief Master identification messagelength. */
#define SMP_ID_INFO_LEN               17                         /*!< \brief Identity information message length. */
#define SMP_ID_ADDR_INFO_LEN          8                          /*!< \brief Identity address information message length. */
#define SMP_SIGN_INFO_LEN             17                         /*!< \brief Signing information message length. */
#define SMP_SECURITY_REQ_LEN          2                          /*!< \brief Security request message length. */
#define SMP_PUB_KEY_MSG_LEN           (1 + 2*SMP_PUB_KEY_LEN)    /*!< \brief Public key message length. */
#define SMP_DHKEY_CHECK_MSG_LEN       (1 + SMP_DHKEY_CHECK_LEN)  /*!< \brief Diffie-Hellman key check message length. */
#define SMP_KEYPRESS_MSG_LEN          2                          /*!< \brief Keypress message length. */
/**@}*/

/** \name SMP I/O Capabilities
 * I/O capabilities codes to be set for \ref SMP_CMD_PAIR_REQ and \ref SMP_CMD_PAIR_RSP
 */
/**@{*/
#define SMP_IO_DISP_ONLY              0x00      /*!< \brief Display only. */
#define SMP_IO_DISP_YES_NO            0x01      /*!< \brief Display yes/no. */
#define SMP_IO_KEY_ONLY               0x02      /*!< \brief Keyboard only. */
#define SMP_IO_NO_IN_NO_OUT           0x03      /*!< \brief No input, no output. */
#define SMP_IO_KEY_DISP               0x04      /*!< \brief Keyboard display. */
/**@}*/

/** \name SMP OOB Data Flag
 * Out-of-Band codes to be set for \ref SMP_CMD_PAIR_REQ and \ref SMP_CMD_PAIR_RSP
 */
/**@{*/
#define SMP_OOB_DATA_NONE             0x00      /*!< \brief No OOB data from the remote device is present. */
#define SMP_OOB_DATA_PRESENT          0x01      /*!< \brief OOB data from the remote device is present. */
/**@}*/

/** \name SMP Authentication Requirements Flags
 * Authentication Requirements Flags to be set for \ref SMP_CMD_PAIR_REQ and \ref SMP_CMD_PAIR_RSP.
 */
/**@{*/
#define SMP_AUTH_BOND_MASK            0x03      /*!< \brief Mask for bonding bits. */
#define SMP_AUTH_BOND_FLAG            0x01      /*!< \brief Bonding requested. */
#define SMP_AUTH_MITM_FLAG            0x04      /*!< \brief MITM (authenticated pairing) requested. */
#define SMP_AUTH_SC_FLAG              0x08      /*!< \brief LE Secure Connections requested. */
#define SMP_AUTH_KP_FLAG              0x10      /*!< \brief Keypress notifications requested. */
/**@}*/

/** \name SMP Key Distribution Flags
 * Flags of security keys this device is requesting to be distribution once pairing completes.
 */
/**@{*/
#define SMP_KEY_DIST_ENC              0x01      /*!< \brief Distribute LTK. */
#define SMP_KEY_DIST_ID               0x02      /*!< \brief Distribute IRK. */
#define SMP_KEY_DIST_SIGN             0x04      /*!< \brief Distribute CSRK. */
/*! \brief Key distribution mask. */
#define SMP_KEY_DIST_MASK             (SMP_KEY_DIST_ENC | SMP_KEY_DIST_ID | SMP_KEY_DIST_SIGN)
/**@}*/

/** \name SMP LE Secure Connection Keypress Codes
 * Keypress codes found in \ref SMP_CMD_KEYPRESS PDU to be sent on the respective action when
 * the \ref SMP_AUTH_KP_FLAG is set in both the \ref SMP_CMD_PAIR_REQ and \ref SMP_CMD_PAIR_RSP.
 */
/**@{*/
#define SMP_PASSKEY_ENTRY_STARTED     0x00      /*!< \brief Passkey entry started keypress type. */
#define SMP_PASSKEY_DIGIT_ENTERED     0x01      /*!< \brief Passkey digit entered keypress type. */
#define SMP_PASSKEY_DIGIT_ERASED      0x02      /*!< \brief Passkey digit erased keypress type. */
#define SMP_PASSKEY_CLEARED           0x03      /*!< \brief Passkey cleared keypress type. */
#define SMP_PASSKEY_ENTRY_COMPLETED   0x04      /*!< \brief Passkey entry complete keypress type. */
/**@}*/

/** \name SMP Value Length Constants
 * Lengths of various keys and values.
 */
/**@{*/
#define SMP_RAND_LEN                  16 /*!< \brief Random number length. */
#define SMP_CONFIRM_LEN               16 /*!< \brief Confirm number length. */
#define SMP_KEY_LEN                   16 /*!< \brief Key length. */
#define SMP_RAND8_LEN                 8  /*!< \brief Random 8-byte number length. */
#define SMP_PRIVATE_KEY_LEN           32 /*!< \brief Secure connections private key length. */
#define SMP_PUB_KEY_LEN               32 /*!< \brief Secure connecdtions public key length. */
#define SMP_DHKEY_LEN                 32 /*!< \brief Secure connection Diffie-Hellman key length. */
#define SMP_DHKEY_CHECK_LEN           16 /*!< \brief Secure connection Diffie-Hellman key check length. */
/**@}*/

/** \name CMAC Input Lengths Constants
 * Input lengths of SMP cryptopgraphic toolbox functions.
 */
/**@{*/
#define SMP_F4_TEXT_LEN               (SMP_PUB_KEY_LEN * 2 + 1)             /*!< \brief F4 input length. */
#define SMP_G2_TEXT_LEN               (SMP_PUB_KEY_LEN * 2 + SMP_RAND_LEN)  /*!< \brief G2 input length. */
#define SMP_F5_TKEY_TEXT_LEN          (SMP_DHKEY_LEN)                       /*!< \brief F5 Temporary key input length. */
#define SMP_F5_TEXT_LEN               (9 + 2*BDA_ADDR_LEN + 2*SMP_RAND_LEN) /*!< \brief F5 input length. */
#define SMP_F6_TEXT_LEN               (2*BDA_ADDR_LEN + 3*SMP_RAND_LEN + 5) /*!< \brief F6 input length. */
/**@}*/

/*! \} */    /* STACK_SMP_API */

#ifdef __cplusplus
};
#endif

#endif /* SMP_DEFS_H */
