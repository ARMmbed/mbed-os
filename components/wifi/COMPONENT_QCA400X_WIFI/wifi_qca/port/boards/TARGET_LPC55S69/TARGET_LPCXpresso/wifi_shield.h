/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __WIFI_SHIELD_H__
#define __WIFI_SHIELD_H__

/* Select specific shield support */
//#define WIFISHIELD_IS_GT202
#define WIFISHIELD_IS_SILEX2041

/* NOTE: Silex is not supported. WLAN_IRQ is routed to P3_2
 * that does not support interrupts */

/* Include shields support */
#if defined(WIFISHIELD_IS_GT202)
#include "wifi_shield_gt202.h"
#elif defined(WIFISHIELD_IS_SILEX2041)
#include "wifi_shield_silex2401.h"
#else
#error "No shield is selected !"
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/* Fn prototypes, which need to be implemented */
A_STATUS WIFISHIELD_Init(void);

#if defined(__cplusplus)
}
#endif

A_STATUS WIFISHIELD_InitDrivers(void *param);
A_STATUS WIFISHIELD_DeinitDrivers(void *param);
A_STATUS WIFISHIELD_PowerUp(uint32_t enable);
void WIFISHIELD_NotifyDriverTask(void *param);

#endif
