/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
  
#ifndef __BLE_COMMON_H__
#define __BLE_COMMON_H__

#define NRF51
#define DEBUG_NRF_USER
#define BLE_STACK_SUPPORT_REQD
#define BOARD_PCA10001

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**************************************************************************/
/*!
    \brief  Error codes for the BLE API
*/
/**************************************************************************/
typedef enum ble_error_e
{
  BLE_ERROR_NONE                    = 0,    /**< No error */
  BLE_ERROR_BUFFER_OVERFLOW         = 1,    /**< The requested action would cause a buffer overflow and has been aborted */
  BLE_ERROR_NOT_IMPLEMENTED         = 2,    /**< Requested a feature that isn't yet implement or isn't supported by the target HW */
  BLE_ERROR_PARAM_OUT_OF_RANGE      = 3     /**< One of the supplied parameters is outside the valid range */
} ble_error_t;

#ifdef __cplusplus
}
#endif

#endif
