/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Private platform definitions.
 *
 *  Copyright (c) 2009-2018 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef PLATFORM_BTN_H
#define PLATFORM_BTN_H

#include "platform_nordic.h"
#include "wsf_msg.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

#if (BUTTONS_NUMBER >= 1)
#define BUTTON_0_BITMASK        (1 << BSP_BUTTON_0)
#endif
#if (BUTTONS_NUMBER >= 2)
#define BUTTON_1_BITMASK        (1 << BSP_BUTTON_1)
#endif
#if (BUTTONS_NUMBER >= 3)
#define BUTTON_2_BITMASK        (1 << BSP_BUTTON_2)
#endif
#if (BUTTONS_NUMBER >= 4)
#define BUTTON_3_BITMASK        (1 << BSP_BUTTON_3)
#endif

#define BUTTON_ACTIVE_STATE     0

/**************************************************************************************************
  Types
**************************************************************************************************/

/*! \brief      Button configuration structure. */
typedef struct
{
    uint8_t             pinNo;       /*!< Pin to be used as a button. */
    uint8_t             activeState; /*!< APP_BUTTON_ACTIVE_HIGH or APP_BUTTON_ACTIVE_LOW. */
    nrf_gpio_pin_pull_t pullCfg;     /*!< Pull-up or -down configuration. */
} platformButtonCfg_t;

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern platformButtonCfg_t platformButtons[];

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void PlatformInitBtns(platformButtonCfg_t *pButtons, uint8_t buttonCount);
uint32_t PlatformReadBtns(void);
void PlatformRegisterBtnsHandler(wsfHandlerId_t handlerId, wsfEventMask_t eventMask);

#endif  /* PLATFORM_BTN_H */
