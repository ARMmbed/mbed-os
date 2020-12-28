/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#ifndef MBED_ITM_API_H
#define MBED_ITM_API_H

#if DEVICE_ITM

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup itm_hal Instrumented Trace Macrocell HAL API
 * @{
 */

enum {
    ITM_PORT_SWO = 0
};

/**
 * @brief      Target specific initialization function.
 *             This function is responsible for initializing and configuring
 *             the debug clock for the ITM and setting up the SWO pin for
 *             debug output.
 *
 *             The only Cortex-M register that should be modified is the clock
 *             prescaler in TPI->ACPR.
 *
 *             The generic mbed_itm_init initialization function will setup:
 *
 *                  ITM->LAR
 *                  ITM->TPR
 *                  ITM->TCR
 *                  ITM->TER
 *                  TPI->SPPR
 *                  TPI->FFCR
 *                  DWT->CTRL
 *
 *             for SWO output on stimulus port 0.
 */
void itm_init(void);

/**
 * @brief      Initialization function for both generic registers and target specific clock and pin.
 */
void mbed_itm_init(void);

/**
 * @brief      Send data over ITM stimulus port.
 *
 * @param[in]  port  The stimulus port to send data over.
 * @param[in]  data  The 32-bit data to send.
 *
 * The data is written as a single 32-bit write to the port.
 *
 * @return     value of data sent.
 */
uint32_t mbed_itm_send(uint32_t port, uint32_t data);

/**
 * @brief      Send a block of data over ITM stimulus port.
 *
 * @param[in]  port  The stimulus port to send data over.
 * @param[in]  data  The block of data to send.
 * @param[in]  len   The number of bytes of data to send.
 *
 * The data is written using multiple appropriately-sized port accesses for
 * efficient transfer.
 */
void mbed_itm_send_block(uint32_t port, const void *data, size_t len);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif /* MBED_ITM_API_H */

/**@}*/
