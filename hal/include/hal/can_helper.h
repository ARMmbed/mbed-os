
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
#ifndef MBED_CAN_HELPER_H
#define MBED_CAN_HELPER_H

#if DEVICE_CAN || FEATURE_EXPERIMENTAL_API

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * \enum    CANFormat
 *
 * \brief   Values that represent CAN Format
**/
enum CANFormat {
    CANStandard = 0,
    CANExtended = 1,
    CANAny = 2
};
typedef enum CANFormat CANFormat;

/**
 *
 * \enum    CANType
 *
 * \brief   Values that represent CAN Type
**/
enum CANType {
    CANData   = 0,
    CANRemote = 1
};
typedef enum CANType CANType;

/**
 *
 * \struct  CAN_Message
 *
 * \brief   Holder for single CAN message.
 *
**/
struct CAN_Message {
    unsigned int   id;                 // 29 bit identifier
    unsigned char  data[8];            // Data field
    unsigned char  len;                // Length of data field in bytes
    CANFormat      format;             // Format ::CANFormat
    CANType        type;               // Type ::CANType
};
typedef struct CAN_Message CAN_Message;

#ifdef __cplusplus
}
#endif

#endif

#endif // MBED_CAN_HELPER_H

/** @}*/
