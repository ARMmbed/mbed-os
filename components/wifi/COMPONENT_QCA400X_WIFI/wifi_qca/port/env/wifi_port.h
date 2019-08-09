/*
 * Copyright (c) 2016, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __WIFI_PORT_H__
#define __WIFI_PORT_H__

/* "wifi_env_port.h" macros can be overloaded here.
 * This file can contain only porting macros, nothing else */

#include "wifi_env_port.h"

extern A_STATUS WIFISHIELD_PowerUp(uint32_t enable);
#define CUSTOM_HW_POWER_UP_DOWN(pCxt, powerup) \
    WIFISHIELD_PowerUp(powerup)

extern A_STATUS WIFIDRVS_SPI_InOutBuffer(uint8_t *pBuffer, uint16_t length, uint8_t doRead, boolean sync);
#define CUSTOM_BUS_INOUT_BUFFER(pCxt, pBuffer, length, doRead, sync) \
    WIFIDRVS_SPI_InOutBuffer((pBuffer), (length), (doRead), (sync))

extern A_STATUS WIFIDRVS_SPI_InOutToken(uint32_t OutToken, uint8_t DataSize, uint32_t *pInToken);
#define CUSTOM_BUS_INOUT_TOKEN(pCxt, oT, dS, pInT) \
    WIFIDRVS_SPI_InOutToken((oT), (dS), (pInT))

#define CUSTOM_BUS_START_TRANSFER(pCxt, sync) (A_OK)

#define CUSTOM_BUS_COMPLETE_TRANSFER(pCxt, sync) (A_OK)

extern A_STATUS WIFISHIELD_InitDrivers(void *param);
#define CUSTOM_HW_INIT(pCxt) WIFISHIELD_InitDrivers((pCxt))

extern A_STATUS WIFISHIELD_DeinitDrivers(void *param);
#define CUSTOM_HW_DEINIT(pCxt) WIFISHIELD_DeinitDrivers((pCxt))

#endif
