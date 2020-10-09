/***************************************************************************//**
 * @file sl_eth_hw.h
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
#ifndef SL_ETH_HW_H
#define SL_ETH_HW_H

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Sets up hardware pins and configures internal clocks
 */
void sl_eth_hw_init(void);

/**
 * Releases hardware pins and turns off internal clocks
 */
void sl_eth_hw_deinit(void);

#if defined(__cplusplus)
}
#endif

#endif
