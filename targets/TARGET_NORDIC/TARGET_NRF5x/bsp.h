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

// Dummy header file to satify import requirement of nrf_log.c 
// when NRF_LOG_USES_UART is enabled

#ifndef DUMMY_BSP_H_
#define DUMMY_BSP_H_

const int RX_PIN_NUMBER = -1;
const int TX_PIN_NUMBER = -1;
const int RTS_PIN_NUMBER = -1;
const int CTS_PIN_NUMBER = -1;

// This is not used for any functional code so does not need to be correct
#ifndef NRF51
#define NRF51
#endif

#endif // DUMMY_BSP_H_