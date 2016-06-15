/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 */

/** @file
 * @brief Definition file for resource usage by SoftDevice, ESB and Gazell.
 */

#ifndef APP_RESOURCES_H__
#define APP_RESOURCES_H__

#ifdef SOFTDEVICE_PRESENT
	#include "nrf_sd_def.h"
#else
	#define SD_PPI_RESTRICTED	      0uL /**< 1 if PPI peripheral is restricted, 0 otherwise. */
	#define SD_PPI_CHANNELS_USED      0uL /**< PPI channels utilized by SotfDevice (not available to th spplication). */
	#define SD_PPI_GROUPS_USED        0uL /**< PPI groups utilized by SotfDevice (not available to th spplication). */
	#define SD_TIMERS_USED            0uL /**< Timers used by SoftDevice. */
	#define SD_SWI_USED               0uL /**< Software interrupts used by SoftDevice. */
#endif

#ifdef GAZELL_PRESENT
	#include "nrf_gzll_resources.h"
#else
	#define GZLL_PPI_CHANNELS_USED    0uL /**< PPI channels utilized by Gazell (not available to th spplication). */
	#define GZLL_TIMERS_USED          0uL /**< Timers used by Gazell. */
	#define GZLL_SWI_USED             0uL /**< Software interrupts used by Gazell */
#endif

#ifdef ESB_PRESENT
	#include "nrf_esb_resources.h"
#else
	#define ESB_PPI_CHANNELS_USED    0uL /**< PPI channels utilized by ESB (not available to th spplication). */
	#define ESB_TIMERS_USED          0uL /**< Timers used by ESB. */
	#define ESB_SWI_USED             0uL /**< Software interrupts used by ESB */
#endif

#define NRF_PPI_CHANNELS_USED (SD_PPI_CHANNELS_USED | GZLL_PPI_CHANNELS_USED | ESB_PPI_CHANNELS_USED)
#define NRF_PPI_GROUPS_USED   (SD_PPI_GROUPS_USED)
#define NRF_SWI_USED          (SD_SWI_USED | GZLL_SWI_USED | ESB_SWI_USED)
#define NRF_TIMERS_USED       (SD_TIMERS_USED | GZLL_TIMERS_USED | ESB_TIMERS_USED)

#endif // APP_RESOURCES_H__
