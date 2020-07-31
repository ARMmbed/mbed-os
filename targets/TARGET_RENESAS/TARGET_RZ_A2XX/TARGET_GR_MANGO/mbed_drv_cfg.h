/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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

#ifndef MBED_DRV_CFG_H
#define MBED_DRV_CFG_H

/* can_api.c */
#define CAN_TEST_GLOBAL_CH  1

/* gpio_api.c */
#define GPIO_GROUP_MAX      21

/* pwmout_api.c */
#define FUNC_GENERAL_PWM_TIMER

/* rtc_api.c */
//#define USE_RTCX1_CLK
#define USE_EXTAL_CLK
#define RENESAS_RZ_A2_P0_CLK   CM0_RENESAS_RZ_A2_P0_CLK

/* flash (MX25UW12845GXDI00) */
#define USE_OCTAFLASH
#define OCTAFLASH_BASE             (0x50000000UL) /**< Flash Base Address */
#define OCTAFLASH_SIZE             (0x01000000UL) /**< Available Flash Memory */
#define OCTAFLASH_PAGE_SIZE        256            /**< Flash Memory page size (interleaving off) */
/**< Maximum size per one writing is 256 byte and minimum size per one writing is 1 byte */
#define OCTAFLASH_SECTOR_SIZE      4096           /**< Flash Memory sector size (interleaving off) */

#define FLASH_BASE                 OCTAFLASH_BASE /**< Flash Base Address */
#define FLASH_SIZE                 OCTAFLASH_SIZE /**< Available Flash Memory */

/* HyperRAM */
//#define USE_HYPERRAM

/* OctaRAM */
#define USE_OCTARAM
#define OCTARAM_SIZE               (0x00800000UL) /**< Available Memory */

#endif
