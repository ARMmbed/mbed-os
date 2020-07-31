/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2012 - 2020 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/* Copyright (c) 2012-2020 Renesas Electronics Corporation.
 * SPDX-License-Identifier: Apache-2.0
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
/**************************************************************************//**
* @file         RZ_A2_Init.c
* $Rev: 624 $
* $Date:: 2013-04-24 13:37:48 +0900#$
* @brief        RZ_A2 Initialize
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "MBRZA2M.h"
#include "RZ_A2_Init.h"
#include "pinmap.h"
#include "gpio_api.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

/**************************************************************************//**
* Function Name: RZ_A2_SetSramWriteEnable
* @brief         Initialize Board settings
*
*                Description:<br>
*                Set SRAM write enable
* @param         none
* @retval        none
******************************************************************************/
void RZ_A2_SetSramWriteEnable(void)
{
    /* Enable SRAM write access */
    CPG.SYSCR3.BYTE = 0x0F;

    return;
}

/**************************************************************************//**
* Function Name: RZ_A2_InitClock
* @brief         Initialize Board settings
*
*                Description:<br>
*                Initialize Clock
* @param         none
* @retval        none
******************************************************************************/
void RZ_A2_InitClock(void)
{
    /* WARNING:                                                                */
    /* The section area for the .data data or the .bss data is not initialized */
    /* because this function is called by the Peripheral_BasicInit             */
    /* function. Do not use the variables allocated to the section area        */
    /* for the .data or the .bss data within this function and the user-       */
    /* defined function called by this function.                               */

    volatile uint32_t dummy_buf_32b;

    /* standby_mode_en bit of Power Control Register setting */
    pl310.REG15_POWER_CTRL.BIT.standby_mode_en = 1;
    dummy_buf_32b = pl310.REG15_POWER_CTRL.LONG;
    (void)dummy_buf_32b;

    /* ==== CPG Settings ==== */
    CPG.FRQCR.WORD  = 0x1012u;  /* PLL(x88), I:G:B:P1:P0 = 22:11:5.5:2.75:1.375 */
    /* CKIO:Output at time usually,                 */
    /* Output when bus right is opened,             */
    /* output at standby"L"                         */
    /* Clockin  = 24MHz,                            */
    /* I  Clock = 528MHz,                           */
    /* G  Clock = 264MHz                            */
    /* B  Clock = 132MHz,                           */
    /* P1 Clock = 66MHz,                            */
    /* P0 Clock = 33MHz                             */

    return;
}

/**************************************************************************//**
* Function Name: RZ_A2_IsClockMode0
* @brief         Query Clock Mode
*
*                Description:<br>
*                Answer ClockMode0 or not
* @param         none
* @retval        true  : clock mode 0
* @retval        false : clock mode 1
******************************************************************************/
int RZ_A2_IsClockMode0(void)
{
    /* ClockMode0 */
    return true;
}

/**************************************************************************//**
* Function Name: RZ_A2_InitBus
* @brief         Initialize Bus
*
*                Description:<br>
*                Initialize Pin Setting
* @param         none
* @retval        none
******************************************************************************/
void RZ_A2_InitBus(void)
{
    /*************************************************************************/
    /* If need Pin Setting before run program, the setting will be wrote here*/
    /*************************************************************************/

    pin_function(P6_5, 4); // AUDIO_XOUT

    return;
}

/******************************************************************************
End of file
******************************************************************************/
