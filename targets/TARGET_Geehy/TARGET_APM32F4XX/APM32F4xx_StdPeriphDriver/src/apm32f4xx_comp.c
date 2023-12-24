/*!
 * @file        apm32f4xx_comp.c
 *
 * @brief       This file contains all the functions for the COMP peripheral for APM32F411
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

/* Includes */
#include "apm32f4xx_comp.h"
#include "apm32f4xx_rcm.h"

/** @addtogroup APM32F4xx_StdPeriphDriver
  @{
*/

/** @addtogroup COMP_Driver
  @{
*/

/** @defgroup COMP_Functions Functions
  @{
*/

/*!
 * @brief    Reset COMP peripheral registers to their default values.
 *
 * @param    None
 *
 * @retval   None
 *
 * @note     Deinitialization can't be performed if the COMP configuration is locked.
 *           To unlock the configuration, perform a system reset.
 */
void COMP_Reset(void)
{
    COMP->CSTS1 = ((uint32_t)0x00000000);
    COMP->CSTS2 = ((uint32_t)0x00000000);
}

/*!
 * @brief    Configs the COMP peripheral according to the specified parameters
 *           in COMP_InitStruct
 *
 * @param    compSelect: the selected comparator.
 *            This parameter can be one of the following values:
 *              @arg COMP_SELECT_COMP1: COMP1 selected
 *              @arg COMP_SELECT_COMP2: COMP2 selected
 *
 * @param    compConfig: pointer to an COMP_Config_T structure that contains
 *           the configuration information for the specified COMP peripheral.
 *
 * @retval   None
 *
 * @note     If the selected comparator is locked, initialization can't be performed.
 *           To unlock the configuration, perform a system reset.
 *
 * @note     By default, PA1 is selected as COMP1 non inverting input.
 *           To use PA4 as COMP1 non inverting input call COMP_EnableSwitch() after COMP_Config()
 *
 */
void COMP_Config(COMP_SELECT_T compSelect, COMP_Config_T* compConfig)
{
    if (compSelect == COMP_SELECT_COMP1)
    {
        COMP->CSTS1_B.INMCCFG1 = compConfig->invertingInput;
        COMP->CSTS1_B.OUTSEL1  = compConfig->output;
        COMP->CSTS1_B.POLCFG1  = compConfig->outputPol;
    }
    else
    {
        COMP->CSTS2_B.INMCCFG2 = compConfig->invertingInput;
        COMP->CSTS2_B.OUTSEL2  = compConfig->output;
        COMP->CSTS2_B.POLCFG2  = compConfig->outputPol;
        COMP->CSTS2_B.SPEEDM2  = compConfig->mode;
    }
}

/*!
 * @brief    Fills each compConfig member with initial value value.
 *
 * @param    compConfig: pointer to an COMP_InitTypeDef structure which will
 *           be initialized.
 *
 * @retval   None
 */
void COMP_ConfigStructInit(COMP_Config_T* compConfig)
{
    compConfig->invertingInput  = COMP_INVERTING_INPUT_1_4VREFINT;
    compConfig->output          = COMP_OUTPUT_NONE;
    compConfig->outputPol       = COMP_OUTPUTPOL_NONINVERTED;
    compConfig->mode            = COMP_MODE_HIGHSPEED;
}

/*!
 * @brief    Enable the COMP peripheral.
 *
 * @param    compSelect: the selected comparator.
 *            This parameter can be one of the following values:
 *              @arg COMP_SELECT_COMP1: COMP1 selected
 *              @arg COMP_SELECT_COMP2: COMP2 selected
 *
 * @retval   None
 *
 * @note     If the selected comparator is locked, enable can't be performed.
 *           To unlock the configuration, perform a system reset.
 */
void COMP_Enable(COMP_SELECT_T compSelect)
{
    if (compSelect == COMP_SELECT_COMP1)
    {
        COMP->CSTS1_B.EN1 = SET;
    }
    else
    {
        COMP->CSTS2_B.EN2 = SET;
    }
}

/*!
 * @brief    Disable the COMP peripheral.
 *
 * @param    compSelect: the selected comparator.
 *            This parameter can be one of the following values:
 *              @arg COMP_SELECT_COMP1: COMP1 selected
 *              @arg COMP_SELECT_COMP2: COMP2 selected
 *
 * @retval   None
 *
 * @note     If the selected comparator is locked, disable can't be performed.
 *           To unlock the configuration, perform a system reset.
 */
void COMP_Disable(COMP_SELECT_T compSelect)
{
    if (compSelect == COMP_SELECT_COMP1)
    {
        COMP->CSTS1_B.EN1 = RESET;
    }
    else
    {
        COMP->CSTS2_B.EN2 = RESET;
    }
}

/*!
 * @brief    Return the output level (high or low) of the selected comparator.
 *
 * @param    compSelect: the selected comparator.
 *            This parameter can be one of the following values:
 *              @arg COMP_SELECT_COMP1: COMP1 selected
 *              @arg COMP_SELECT_COMP2: COMP2 selected
 *
 * @retval   Returns the selected comparator output level: low or high.
 */
uint32_t COMP_ReadOutPutLevel(COMP_SELECT_T compSelect)
{
    uint32_t compOUT = 0x00;
    if (compSelect == COMP_SELECT_COMP1)
    {
        compOUT =  COMP->CSTS1_B.OUTVAL1;
    }
    else
    {
        compOUT =  COMP->CSTS2_B.OUTVAL2;
    }
    return (uint32_t)(compOUT);
}

/*!
 * @brief    Enablesthe window mode.
 *
 * @param    None
 *
 * @retval   None
 */
void COMP_EnableWindow(void)
{
    COMP->CSTS1_B.WMODESEL1 = SET;
}

/*!
 * @brief    Disables the window mode.
 *
 * @param    None
 *
 * @retval   None
 */
void COMP_DisnableWindow(void)
{
    COMP->CSTS1_B.WMODESEL1 = RESET;
}

/*!
 * @brief    Lock the selected comparator configuration.
 *
 * @param    compSelect: selects the comparator to be locked
 *             This parameter can be one of  the following values:
 *               @arg COMP_SELECT_COMP1: COMP1 configuration is locked.
 *               @arg COMP_SELECT_COMP2: COMP2 configuration is locked.
 *
 * @retval   None
 */
void COMP_ConfigLOCK(COMP_SELECT_T compSelect)
{
    if (compSelect == COMP_SELECT_COMP1)
    {
        COMP->CSTS1_B.LOCK1 = SET;
    }
    else
    {
        COMP->CSTS2_B.LOCK2 = SET;
    }
}

/**@} end of group COMP_Functions */
/**@} end of group COMP_Driver */
/**@} end of group APM32F4xx_StdPeriphDriver */
