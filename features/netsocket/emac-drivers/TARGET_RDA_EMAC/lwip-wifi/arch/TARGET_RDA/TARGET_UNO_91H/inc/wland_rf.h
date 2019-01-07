/* Copyright (c) 2019 Unisoc Communications Inc.
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

#ifndef _WLAND_RF_H_
#define _WLAND_RF_H_

#include "wland_types.h"

#define WLAND_TXP_NUM       2
#define WLAND_CHANNEL_NUM   14
#define WLAND_TX_POWER_PHY_GN_REG  0x11F
#define WLAND_TX_POWER_PHY_B_REG   0x120
#define WLAND_TX_POWER_RF_REG      0x8A
#define WLAND_XTAL_CAL_REG         0xDA

#define MAKE_WORD16(lsb, msb)           (((u16)(msb) << 8)  & 0xFF00) | (lsb)
#define MAKE_WORD32(lsw, msw)           (((u32)(msw) << 16) & 0xFFFF0000) | (lsw)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

