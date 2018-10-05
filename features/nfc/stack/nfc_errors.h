/*
 * Copyright (c) 2013-2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * \file nfc_errors.h
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 * \details NFC Error codes
 */

/** \addtogroup Core
 *  @{
 *  \name Error codes
 *  @{
 */

#ifndef NFC_ERRORS_H_
#define NFC_ERRORS_H_

#define NFC_OK                    0   ///< No error

#define NFC_ERR_UNKNOWN           1   ///< Unknown error
#define NFC_ERR_LENGTH            2   ///< Length of parameter is wrong
#define NFC_ERR_NOT_FOUND         3   ///< Could not find item
#define NFC_ERR_UNSUPPORTED       4   ///< This action is not supported
#define NFC_ERR_PARAMS            5   ///< These parameters are not correct
#define NFC_ERR_BUFFER_TOO_SMALL  6   ///< The buffer is too small to store all data (buffer overflow)
#define NFC_ERR_TIMEOUT           7   ///< Timeout
#define NFC_ERR_CRC               8   ///< Checksum does not match
#define NFC_ERR_NOPEER            9   ///< No target/initiator in vicinity
#define NFC_ERR_PARITY            10  ///< Parity error
#define NFC_ERR_FIELD             11  ///< No RF field detected (or RF field lost)
#define NFC_ERR_COLLISION         12  ///< Collision detected
#define NFC_ERR_WRONG_COMM        13  ///< Communication error
#define NFC_ERR_PROTOCOL          14  ///< Protocol is not conformant
#define NFC_ERR_BUSY              15  ///< Resource is busy
#define NFC_ERR_CONTROLLER        16  ///< Controller failure
#define NFC_ERR_HALTED            17  ///< Target has been halted
#define NFC_ERR_MAC               18  ///< MAC does not match
#define NFC_ERR_UNDERFLOW         19  ///< Could not send data in time
#define NFC_ERR_DISCONNECTED      20  ///< Link has disconnected
#define NFC_ERR_ABORTED           21  ///< Command was aborted

/** Type for NFC errors
 */
typedef int nfc_err_t;

#endif /* NFC_ERRORS_H_ */

/**
 * @}
 * @}
 * */
