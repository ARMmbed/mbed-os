/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#ifndef NRF_SD_DEF_H__
#define NRF_SD_DEF_H__

#include <stdint.h>

#define SD_PPI_CHANNELS_USED            0xFFF0C000uL /**< PPI channels utilized by SotfDevice (not available to the application). */
#define SD_PPI_GROUPS_USED              0x0000000CuL /**< PPI groups utilized by SoftDevice (not available to the application). */
#define SD_TIMERS_USED                  0x00000001uL /**< Timers used by SoftDevice. */
#define SD_SWI_USED                     0x0000003CuL /**< Software interrupts used by SoftDevice */

#endif /* NRF_SD_DEF_H__ */
