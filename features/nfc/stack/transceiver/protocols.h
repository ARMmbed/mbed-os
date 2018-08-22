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
 * \file protocols.h
 * \copyright Copyright (c) ARM Ltd 2013
 * \author Donatien Garnier
 * \details List of RF protocols
 */

/** \addtogroup Transceiver
 *  @{
 *  \name Protocols and RF configuration
 *  @{
 */

#ifndef PROTOCOLS_H_
#define PROTOCOLS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stack/nfc_common.h"

typedef enum __RF_PROTOCOL {
    __RF_PROTOCOL_UNKNOWN = 0,
    //Reader
    RF_PROTOCOL_ISO_14443_A_READER,
    RF_PROTOCOL_ISO_14443_B_READER,
    RF_PROTOCOL_INNOVATRON_READER,
    RF_PROTOCOL_ISO_15693_READER,
    RF_PROTOCOL_FELICA_READER,
    //... add other protocols here
    RF_PROTOCOL_ISO_14443_A_TARGET,
    RF_PROTOCOL_ISO_14443_B_TARGET,
    RF_PROTOCOL_INNOVATRON_TARGET,
    RF_PROTOCOL_ISO_15693_TARGET,
    RF_PROTOCOL_FELICA_TARGET,
    RF_PROTOCOL_ISO_DEP_TARGET, //ISO 14443-4 transport protocol
    RF_PROTOCOL_NFC_DEP_TARGET, //NFC-IP 1 transport protocol
    //... add other protocols here

} RF_PROTOCOL;

#define RF_PROTOCOL_IS_TARGET(x) ((x)>=RF_PROTOCOL_ISO_14443_A_TARGET)
#define RF_PROTOCOL_IS_READER(x) (!RF_PROTOCOL_IS_TARGET(x))

typedef uint32_t RF_OPTION;
//These options can be ORed
#define RF_OPTION_NONE            0x00
#define RF_OPTION_COMPUTE_CRC     0x01
#define RF_OPTION_COMPUTE_PARITY  0x02
#define RF_OPTION_CHECK_CRC       0x04
#define RF_OPTION_CHECK_PARITY    0x08
#define RF_OPTION_CLOSE           0x10 //Last frame

typedef enum __RF_BITRATE {
    RF_BITRATE_106K = 0x00,
    RF_BITRATE_212K = 0x01,
    RF_BITRATE_424K = 0x02,
    RF_BITRATE_848K = 0x03,

} RF_BITRATE;

#ifdef __cplusplus
}
#endif

#endif /* PROTOCOLS_H_ */

/**
 * @}
 * @}
 * */

