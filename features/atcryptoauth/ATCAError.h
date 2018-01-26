/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#ifndef ATCAERROR_H
#define ATCAERROR_H

enum ATCAError {
    /* Device error codes from datasheet section 9.1.2 */
    ATCA_ERR_NO_ERROR                               = 0x0000,
    ATCA_ERR_CHECK_MAC_OR_VERIFY_FAIL               = 0x0001,
    ATCA_ERR_PARSE_ERROR                            = 0x0003,
    ATCA_ERR_ECC_FAULT                              = 0x0005,
    ATCA_ERR_WAKE_TOKEN_RECVD                       = 0x0011,
    ATCA_ERR_WATCHDOG_WILL_EXPIRE                   = 0x00EE,
    ATCA_ERR_CRC_OR_OTHR_ERROR                      = 0x00FF,

    /* Driver defined errors have MSByte set to 0x1*/
    ATCA_ERR_DEVICE_ERROR                           = 0x0101, // rename to WAKE_TOKEN_NOT_RECVD
    ATCA_ERR_UNSUPPORTED_DEVICE_REVISION,
    ATCA_ERR_SMALL_BUFFER,
    ATCA_ERR_INVALID_PARAM,
    ATCA_ERR_SLOT_NOT_PRIV_KEY,
    ATCA_ERR_DEVICE_NOT_INITIALIZED,
    ATCA_ERR_DEVICE_ALREADY_INITIALIZED,
    ATCA_ERR_CONFIG_LOCKED,
    ATCA_ERR_MEM_ALLOC_FAILURE,
    ATCA_ERR_I2C_READ_ERROR,
    ATCA_ERR_I2C_WRITE_ERROR
};

#endif /* ATCAERROR_H */
