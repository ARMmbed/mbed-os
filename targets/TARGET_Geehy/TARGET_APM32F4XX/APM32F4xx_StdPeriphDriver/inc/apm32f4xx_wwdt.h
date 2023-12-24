/*!
 * @file        apm32f4xx_wwdt.h
 *
 * @brief       This file contains all the functions prototypes for the WWDT firmware library
 *
 * @version     V1.0.3
 *
 * @date        2023-07-31
 *
 * @attention
 *
 *  Copyright (C) 2021-2023 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

/* Define to prevent recursive inclusion */
#ifndef __APM32F4XX_WWDT_H
#define __APM32F4XX_WWDT_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes */
#include "apm32f4xx.h"

/** @addtogroup APM32F4xx_StdPeriphDriver
  @{
*/

/** @addtogroup WWDT_Driver
  @{
*/

/** @defgroup WWDT_Enumerations
  @{
*/

/**
 * @brief WWDT Timebase(Prescaler)
 */
typedef enum
{
    WWDT_TIME_BASE_1,   /*!< WWDT counter clock = (PCLK1 / 4096) / 1 */
    WWDT_TIME_BASE_2,   /*!< WWDT counter clock = (PCLK1 / 4096) / 2 */
    WWDT_TIME_BASE_4,   /*!< WWDT counter clock = (PCLK1 / 4096) / 4 */
    WWDT_TIME_BASE_8    /*!< WWDT counter clock = (PCLK1 / 4096) / 8 */
} WWDT_TIME_BASE_T;

/**@} end of group WWDT_Enumerations*/

/** @defgroup WWDT_Functions
  @{
*/

/* WWDT reset */
void WWDT_Reset(void);

/* Config WWDT Timebase */
void WWDT_ConfigTimebase(WWDT_TIME_BASE_T timeBase);

/* Config Window Data */
void WWDT_ConfigWindowData(uint8_t windowData);

/* Config Couter */
void WWDT_ConfigCounter(uint8_t counter);

/* Enable WWDT and Early Wakeup interrupt */
void WWDT_EnableEWI(void); //5
void WWDT_Enable(uint8_t count);

/* Read Flag and Clear Flag */
uint8_t WWDT_ReadFlag(void);
void WWDT_ClearFlag(void);

#ifdef __cplusplus
}
#endif

#endif /* __APM32F4XX_WWDT_H */

/**@} end of group WWDT_Enumerations */
/**@} end of group WWDT_Driver */
/**@} end of group APM32F4xx_StdPeriphDriver */
