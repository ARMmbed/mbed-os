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

#ifndef PLATFORM_INT_H
#define PLATFORM_INT_H

#include "platform_api.h"

/* Nordic specific definitions. */
#include <stdbool.h>
#include "boards.h"
#include "nrf.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

#if defined(BOARD_PCA10000) || defined(BOARD_PCA10031)

#define LED_CPU_ACTIVE()        nrf_gpio_pin_set  (LED_RGB_RED); \
                                nrf_gpio_pin_clear(LED_RGB_GREEN); \
                                nrf_gpio_pin_set  (LED_RGB_BLUE)
#define LED_CPU_SLEEP()         nrf_gpio_pin_set  (LED_RGB_RED); \
                                nrf_gpio_pin_set  (LED_RGB_GREEN); \
                                nrf_gpio_pin_set  (LED_RGB_BLUE)
#define LED_ERROR()             nrf_gpio_pin_clear(LED_RGB_RED); \
                                nrf_gpio_pin_set  (LED_RGB_GREEN); \
                                nrf_gpio_pin_set  (LED_RGB_BLUE)
#define LED_CLEAR_ERROR()

#elif defined(BOARD_PCA10001)

#define LED_CPU_ACTIVE()        nrf_gpio_pin_set  (BSP_LED_0)
#define LED_CPU_SLEEP()         nrf_gpio_pin_clear(BSP_LED_0)
#define LED_ERROR()             nrf_gpio_pin_set  (BSP_LED_1)
#define LED_CLEAR_ERROR()       nrf_gpio_pin_clear(BSP_LED_1)

#elif defined(BOARD_PCA10028) || defined(BOARD_PCA10040) || defined(BOARD_PCA10056)

/* Inverted LED "on" logic */
#define LED_CPU_ACTIVE()        nrf_gpio_pin_clear(BSP_LED_1)   /* edge of board */
#define LED_CPU_SLEEP()         nrf_gpio_pin_set  (BSP_LED_1)   /* edge of board */
#define LED_ERROR()             nrf_gpio_pin_clear(BSP_LED_3)   /* edge of board */
#define LED_CLEAR_ERROR()       nrf_gpio_pin_set  (BSP_LED_3)   /* edge of board */

#else

#warning "Board not specified"

#define LED_CPU_ACTIVE()
#define LED_CPU_SLEEP()
#define LED_ERROR()
#define LED_CLEAR_ERROR()

#endif

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern uint8_t *SystemHeapStart;
extern uint32_t SystemHeapSize;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

void platformTimeInit(void);

#endif  /* PLATFORM_INT_H */
