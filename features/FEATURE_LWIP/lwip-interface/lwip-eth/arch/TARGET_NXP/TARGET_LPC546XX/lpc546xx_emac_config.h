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
#ifndef LPC546XX_EMAC_CONFIG_H__
#define LPC546XX_EMAC_CONFIG_H__

#include "fsl_enet.h"

#define ENET_RX_RING_LEN              (16)
#define ENET_TX_RING_LEN              (8)

#define ENET_ETH_MAX_FLEN             (ENET_FRAME_MAX_FRAMELEN)

#if defined(__cplusplus)
extern "C" {
#endif

int phy_link_status(void);

#if defined(__cplusplus)
}
#endif

#endif // #define LPC546XX_EMAC_CONFIG_H__

