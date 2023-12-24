/*!
 * @file        apm32f4xx_comp.h
 *
 * @brief       This file contains all the functions prototypes for the COMP firmware library for APM32F411
 *
 * @version     V1.0.3
 *
 * @date        2023-07-31
 *
 * @attention
 *
 *  Copyright (C) 2020-2022 Geehy Semiconductor
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
#ifndef __APM32F4XX_COMP_H
#define __APM32F4XX_COMP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "apm32f4xx.h"

/** @addtogroup APM32F4xx_StdPeriphDriver
  @{
*/

/** @addtogroup COMP_Driver
  @{
*/

/** @defgroup COMP_Enumerations Enumerations
  @{
*/

/**
 * @brief    COMP_Selection
 */
typedef enum
{
    COMP_SELECT_COMP1,  /*!< COMP1 */
    COMP_SELECT_COMP2   /*!< COMP2 */
} COMP_SELECT_T;

/**
 * @brief    COMP_InvertingInput
 */
typedef enum
{
    COMP_INVERTING_INPUT_VREFINT     = 0x00, /*!< VREFINT */
    COMP_INVERTING_INPUT_PC1_3       = 0x01, /*!< PC1 for COMP1, PC3 for COMP2 */
    COMP_INVERTING_INPUT_1_4VREFINT  = 0x04, /*!< 1/4 of VREFINT */
    COMP_INVERTING_INPUT_1_2VREFINT  = 0x05, /*!< 1/2 of VREFINT */
    COMP_INVERTING_INPUT_3_4VREFINT  = 0x06, /*!< 3/4 of VREFINT */

} COMP_INVERTING_INPUT_T;

/**
 * @brief    COMP_Output
 */
typedef enum
{
    COMP_OUTPUT_NONE         = 0x00, /*!< no selection */
    COMP_OUTPUT_TIM1BKIN     = 0x01, /*!< Timer 1 break input */
    COMP_OUTPUT_TIM1IC1      = 0x02, /*!< Timer 1 Input capture 1 */
    COMP_OUTPUT_TIM1ETRF     = 0x03, /*!< Timer 1 ETRF input */
    COMP_OUTPUT_TIM8BKIN     = 0x04, /*!< Timer 8 break input */
    COMP_OUTPUT_TIM8IC4      = 0x05, /*!< Timer 8 input capture 1 */
    COMP_OUTPUT_TIM8ETRF     = 0x06, /*!< Timer 8 ETRF input */
    COMP_OUTPUT_TIM2IC4      = 0x07, /*!< Timer 2 input capture 4 */
    COMP_OUTPUT_TIM2ETRF     = 0x08, /*!< Timer 2 ETRF input */
    COMP_OUTPUT_TIM3IC1      = 0x09, /*!< Timer 3 input capture 1 */
    COMP_OUTPUT_TIM3ETRF     = 0x0A, /*!< Timer 3 ETRF input */
    COMP_OUTPUT_TIM4IC1      = 0x0B, /*!< Timer 4 input capture 1 */
} COMP_OUTPUT_T;

/**
 * @brief    COMP_OutputPolarity
 */
typedef enum
{
    COMP_OUTPUTPOL_NONINVERTED, /*!< output is not inverted */
    COMP_OUTPUTPOL_INVERTED     /*!< output is inverted */
} COMP_OUTPUTPOL_T;

/**
 * @brief    COMP_Mode
 */
typedef enum
{
    COMP_MODE_HIGHSPEED,    /*!< High speed / full power */
    COMP_MODE_LOWSPEED      /*!< Low speed / low-power */
} COMP_MODE_T;

/**@} end of group COMP_Enumerations*/

/** @defgroup COMP_Structures Structures
  @{
*/

/**
 * @brief    OMP Config structure definition
 */
typedef struct
{
    COMP_INVERTING_INPUT_T  invertingInput; /*!< Comparator inverting input selection */
    COMP_OUTPUT_T           output;             /*!< Comparator output selection */
    COMP_OUTPUTPOL_T        outputPol;          /*!< Comparator output polarity */
    COMP_MODE_T             mode;               /*!< Comparator mode */
} COMP_Config_T;

/**@} end of group COMP_Structures */

/** @defgroup COMP_Functions Functions
  @{
*/

/* COMP configuration */
void COMP_Reset(void);
void COMP_Config(COMP_SELECT_T compSelect, COMP_Config_T* compConfig);
void COMP_ConfigStructInit(COMP_Config_T* compConfig);
void COMP_Enable(COMP_SELECT_T compSelect);
void COMP_Disable(COMP_SELECT_T compSelect);

uint32_t COMP_ReadOutPutLevel(COMP_SELECT_T compSelect);

/* Window mode control */
void COMP_EnableWindow(void);
void COMP_DisnableWindow(void);

/* COMP configuration locking */
void COMP_ConfigLOCK(COMP_SELECT_T compSelect);

#ifdef __cplusplus
}
#endif

#endif /* __APM32F4XX_COMP_H */

/**@} end of group COMP_Functions */
/**@} end of group COMP_Driver */
/**@} end of group APM32F4xx_StdPeriphDriver */
