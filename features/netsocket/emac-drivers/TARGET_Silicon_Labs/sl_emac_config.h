/***************************************************************************//**
 * @file sl_emac_config.h
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2018 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
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
 *
 ******************************************************************************/
#ifndef SL_EMAC_CONFIG_H
#define SL_EMAC_CONFIG_H
// -----------------------------------------------------------------------------
// Config options
// -----------------------------------------------------------------------------
/** Number of descriptors in receive list */
#define SL_ETH_NUM_RX_BD              (16)
/** Number of descriptors in transmit list */
#define SL_ETH_NUM_TX_BD              (8)
/** Size of one buffer in a buffer descriptor (must be multiple of 64) */
#define SL_ETH_RX_BUF_SIZE            (1536)
/** Timeout in milliseconds for polling the PHY link status */
#define SL_ETH_LINK_POLL_PERIOD_MS    (500)
/** Default Ethernet worker thread stack size in bytes */
#include "mbed_trace.h"
#if MBED_CONF_MBED_TRACE_ENABLE == 1
#define SL_ETH_THREAD_STACKSIZE       (768)
#else
#define SL_ETH_THREAD_STACKSIZE       (512)
#endif
/** Default Ethernet worker thread stack priority */
#define SL_ETH_THREAD_PRIORITY        (osPriorityHigh)
/** Name of interface */
#define SL_ETH_IF_NAME                "sl"
/** Required alignment (in bytes) for packet buffers */
#define SL_ETH_ALIGN 				  (16)
/** Link MTU */
#define SL_ETH_MTU 					  (1500)

#endif /* SL_EMAC_CONFIG_H */
