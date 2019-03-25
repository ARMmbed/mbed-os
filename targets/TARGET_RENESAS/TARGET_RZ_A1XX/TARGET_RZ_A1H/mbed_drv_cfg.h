/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
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
#define GPIO_GROUP_MAX      11

/* pwmout_api.c */
#define FUNC_MOTOR_CTL_PWM
#define FUMC_MTU2_PWM

/* rtc_api.c */
//#define USE_RTCX1_CLK
#define USE_EXTAL_CLK
//#define USE_RTCX3_CLK

#define RENESAS_RZ_A1_P0_CLK   CM0_RENESAS_RZ_A1_P0_CLK

/* Clock Mode 0 */
/* CLKIN is between 10MHz and 13.33MHz */
/* Divider 1 uses 1/1 ratio, PLL x30 is ON */
#define RENESAS_RZ_A1_SYS_CLK  (CM0_RENESAS_RZ_A1_CLKIN * 30U)

#define LP_TICKER_MTU2_CH      3

/* flash (MX25L6433FM2I) */
#define FLASH_BASE                 (0x18000000UL) /**< Flash Base Address */
#define FLASH_SIZE                 (0x00800000UL) /**< Available Flash Memory */
#define FLASH_PAGE_SIZE            256            /**< Flash Memory page size (interleaving off) */
                                                  /**< Maximum size per one writing is 256 byte and minimum size per one writing is 1 byte */
#define FLASH_SECTOR_SIZE          4096           /**< Flash Memory sector size (interleaving off) */

#endif
