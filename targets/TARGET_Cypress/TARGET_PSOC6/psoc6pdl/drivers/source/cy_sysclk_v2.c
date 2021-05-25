/***************************************************************************//**
* \file cy_sysclk.c
* \version 1.40
*
* Provides an API implementation of the sysclk driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2020 Cypress Semiconductor Corporation
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
*******************************************************************************/

#include "cy_device.h"

#if defined (CY_IP_MXS28SRSS) || defined (CY_IP_MXS40SSRSS)

#include "cy_sysclk.h"
#include "cy_syslib.h"
#include <stdlib.h>


cy_en_sysclk_status_t
                Cy_SysClk_PeriPclkSetDivider(en_clk_dst_t ipBlock, cy_en_divider_types_t dividerType,
                                           uint32_t dividerNum, uint32_t dividerValue)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;
    uint8_t grpNum = (ipBlock & PERI_PCLK_GR_NUM_Msk )>>PERI_PCLK_GR_NUM_Pos;

    if (dividerType == CY_SYSCLK_DIV_8_BIT)
    {
        if ((dividerNum < PERI_DIV_8_NR) &&
            (dividerValue <= (PERI_DIV_8_CTL_INT8_DIV_Msk >> PERI_DIV_8_CTL_INT8_DIV_Pos)))
        {
            CY_REG32_CLR_SET(PERI_DIV_8_CTL(grpNum, dividerNum), PERI_DIV_8_CTL_INT8_DIV, dividerValue);
            retVal = CY_SYSCLK_SUCCESS;
        }
    }
    else if (dividerType == CY_SYSCLK_DIV_16_BIT)
    {
        if ((dividerNum < PERI_DIV_16_NR) &&
            (dividerValue <= (PERI_DIV_16_CTL_INT16_DIV_Msk >> PERI_DIV_16_CTL_INT16_DIV_Pos)))
        {
            CY_REG32_CLR_SET(PERI_DIV_16_CTL(grpNum, dividerNum), PERI_DIV_16_CTL_INT16_DIV, dividerValue);
            retVal = CY_SYSCLK_SUCCESS;
        }
    }
    else
    { /* return bad parameter */
    }
    return (retVal);
}


uint32_t Cy_SysClk_PeriPclkGetDivider(en_clk_dst_t ipBlock, cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    uint32_t retVal;
    uint8_t grpNum = (ipBlock & PERI_PCLK_GR_NUM_Msk )>>PERI_PCLK_GR_NUM_Pos;

    CY_ASSERT_L1(dividerType <= CY_SYSCLK_DIV_16_BIT);

    if (dividerType == CY_SYSCLK_DIV_8_BIT)
    {
        CY_ASSERT_L1(dividerNum < PERI_DIV_8_NR);
        retVal = _FLD2VAL(PERI_DIV_8_CTL_INT8_DIV, PERI_DIV_8_CTL(grpNum, dividerNum));
    }
    else
    { /* 16-bit divider */
        CY_ASSERT_L1(dividerNum < PERI_DIV_16_NR);
        retVal = _FLD2VAL(PERI_DIV_16_CTL_INT16_DIV, PERI_DIV_16_CTL(grpNum, dividerNum));
    }
    return (retVal);
}


cy_en_sysclk_status_t
                Cy_SysClk_PeriPclkSetFracDivider(en_clk_dst_t ipBlock, cy_en_divider_types_t dividerType, uint32_t dividerNum,
                                               uint32_t dividerIntValue, uint32_t dividerFracValue)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;
    uint8_t grpNum = (ipBlock & PERI_PCLK_GR_NUM_Msk )>>PERI_PCLK_GR_NUM_Pos;

    if (dividerType == CY_SYSCLK_DIV_16_5_BIT)
    {
        if ((dividerNum < PERI_DIV_16_5_NR) &&
            (dividerIntValue <= (PERI_DIV_16_5_CTL_INT16_DIV_Msk >> PERI_DIV_16_5_CTL_INT16_DIV_Pos)) &&
            (dividerFracValue <= (PERI_DIV_16_5_CTL_FRAC5_DIV_Msk >> PERI_DIV_16_5_CTL_FRAC5_DIV_Pos)))
        {
            CY_REG32_CLR_SET(PERI_DIV_16_5_CTL(grpNum, dividerNum), PERI_DIV_16_5_CTL_INT16_DIV, dividerIntValue);
            CY_REG32_CLR_SET(PERI_DIV_16_5_CTL(grpNum, dividerNum), PERI_DIV_16_5_CTL_FRAC5_DIV, dividerFracValue);
            retVal = CY_SYSCLK_SUCCESS;
        }
    }
    else if (dividerType == CY_SYSCLK_DIV_24_5_BIT)
    {
        if ((dividerNum < PERI_DIV_24_5_NR) &&
            (dividerIntValue <= (PERI_DIV_24_5_CTL_INT24_DIV_Msk >> PERI_DIV_24_5_CTL_INT24_DIV_Pos)) &&
            (dividerFracValue <= (PERI_DIV_24_5_CTL_FRAC5_DIV_Msk >> PERI_DIV_24_5_CTL_FRAC5_DIV_Pos)))
        {
            CY_REG32_CLR_SET(PERI_DIV_24_5_CTL(grpNum, dividerNum), PERI_DIV_24_5_CTL_INT24_DIV, dividerIntValue);
            CY_REG32_CLR_SET(PERI_DIV_24_5_CTL(grpNum, dividerNum), PERI_DIV_24_5_CTL_FRAC5_DIV, dividerFracValue);
            retVal = CY_SYSCLK_SUCCESS;
        }
    }
    else
    { /* return bad parameter */
    }
    return (retVal);
}


void Cy_SysClk_PeriPclkGetFracDivider(en_clk_dst_t ipBlock, cy_en_divider_types_t dividerType, uint32_t dividerNum,
                                                    uint32_t *dividerIntValue, uint32_t *dividerFracValue)
{
    uint8_t grpNum = (ipBlock & PERI_PCLK_GR_NUM_Msk )>>PERI_PCLK_GR_NUM_Pos;

    CY_ASSERT_L1(((dividerType == CY_SYSCLK_DIV_16_5_BIT) || (dividerType == CY_SYSCLK_DIV_24_5_BIT)) &&
                 (dividerIntValue != NULL) && (dividerFracValue != NULL));

    if (dividerType == CY_SYSCLK_DIV_16_5_BIT)
    {
        CY_ASSERT_L1(dividerNum < PERI_DIV_16_5_NR);
        *dividerIntValue  = _FLD2VAL(PERI_DIV_16_5_CTL_INT16_DIV, PERI_DIV_16_5_CTL(grpNum, dividerNum));
        *dividerFracValue = _FLD2VAL(PERI_DIV_16_5_CTL_FRAC5_DIV, PERI_DIV_16_5_CTL(grpNum, dividerNum));
    }
    else
    { /* 24.5-bit divider */
        CY_ASSERT_L1(dividerNum < PERI_DIV_24_5_NR);
        *dividerIntValue  = _FLD2VAL(PERI_DIV_24_5_CTL_INT24_DIV, PERI_DIV_24_5_CTL(grpNum, dividerNum));
        *dividerFracValue = _FLD2VAL(PERI_DIV_24_5_CTL_FRAC5_DIV, PERI_DIV_24_5_CTL(grpNum, dividerNum));
    }
}


cy_en_sysclk_status_t
                Cy_SysClk_PeriPclkAssignDivider(en_clk_dst_t ipBlock,
                                              cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;
    uint8_t grpNum    = (ipBlock & PERI_PCLK_GR_NUM_Msk )>>PERI_PCLK_GR_NUM_Pos;
    uint8_t periNum  = (ipBlock & PERI_PCLK_PERI_NUM_Msk );

    if ((CY_PERI_CLOCK_NR > periNum) &&
                  (CY_SYSCLK_DIV_24_5_BIT >= dividerType))

    {
        if (((dividerType == CY_SYSCLK_DIV_8_BIT)    && (dividerNum < PERI_DIV_8_NR))    ||
            ((dividerType == CY_SYSCLK_DIV_16_BIT)   && (dividerNum < PERI_DIV_16_NR))   ||
            ((dividerType == CY_SYSCLK_DIV_16_5_BIT) && (dividerNum < PERI_DIV_16_5_NR)) ||
            ((dividerType == CY_SYSCLK_DIV_24_5_BIT) && (dividerNum < PERI_DIV_24_5_NR)))
        {
            PERI_CLOCK_CTL(grpNum, periNum) = _VAL2FLD(CY_PERI_CLOCK_CTL_TYPE_SEL, dividerType) |
                                      _VAL2FLD(CY_PERI_CLOCK_CTL_DIV_SEL, dividerNum);

            retVal = CY_SYSCLK_SUCCESS;
        }
    }
    return (retVal);
}


uint32_t Cy_SysClk_PeriPclkGetAssignedDivider(en_clk_dst_t ipBlock)
{
    uint8_t grpNum = (ipBlock & PERI_PCLK_GR_NUM_Msk )>>PERI_PCLK_GR_NUM_Pos;
    uint8_t periNum  = (ipBlock & PERI_PCLK_PERI_NUM_Msk );

    CY_ASSERT_L1(CY_PERI_CLOCK_NR > periNum);

    return (PERI_CLOCK_CTL(grpNum, periNum) & (CY_PERI_CLOCK_CTL_DIV_SEL_Msk | CY_PERI_CLOCK_CTL_TYPE_SEL_Msk));
}


cy_en_sysclk_status_t
                Cy_SysClk_PeriPclkEnableDivider(en_clk_dst_t ipBlock, cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;
    uint8_t grpNum = (ipBlock & PERI_PCLK_GR_NUM_Msk )>>PERI_PCLK_GR_NUM_Pos;

    if (dividerType <= CY_SYSCLK_DIV_24_5_BIT)
    {
        if (((dividerType == CY_SYSCLK_DIV_8_BIT)    && (dividerNum < PERI_DIV_8_NR))    ||
            ((dividerType == CY_SYSCLK_DIV_16_BIT)   && (dividerNum < PERI_DIV_16_NR))   ||
            ((dividerType == CY_SYSCLK_DIV_16_5_BIT) && (dividerNum < PERI_DIV_16_5_NR)) ||
            ((dividerType == CY_SYSCLK_DIV_24_5_BIT) && (dividerNum < PERI_DIV_24_5_NR)))
        {
            /* specify the divider, make the reference = clk_peri, and enable the divider */
            PERI_DIV_CMD(grpNum) = PERI_DIV_CMD_ENABLE_Msk                         |
                           CY_PERI_DIV_CMD_PA_TYPE_SEL_Msk                 |
                           CY_PERI_DIV_CMD_PA_DIV_SEL_Msk                  |
                           _VAL2FLD(CY_PERI_DIV_CMD_TYPE_SEL, dividerType) |
                           _VAL2FLD(CY_PERI_DIV_CMD_DIV_SEL,  dividerNum);
            (void)PERI_DIV_CMD(grpNum); /* dummy read to handle buffered writes */
            retVal = CY_SYSCLK_SUCCESS;
        }
    }
    return (retVal);
}


cy_en_sysclk_status_t
                Cy_SysClk_PeriPclkDisableDivider(en_clk_dst_t ipBlock, cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;
    uint8_t grpNum = (ipBlock & PERI_PCLK_GR_NUM_Msk )>>PERI_PCLK_GR_NUM_Pos;

    if (dividerType <= CY_SYSCLK_DIV_24_5_BIT)
    {
        if (((dividerType == CY_SYSCLK_DIV_8_BIT)    && (dividerNum < PERI_DIV_8_NR))    ||
            ((dividerType == CY_SYSCLK_DIV_16_BIT)   && (dividerNum < PERI_DIV_16_NR))   ||
            ((dividerType == CY_SYSCLK_DIV_16_5_BIT) && (dividerNum < PERI_DIV_16_5_NR)) ||
            ((dividerType == CY_SYSCLK_DIV_24_5_BIT) && (dividerNum < PERI_DIV_24_5_NR)))
        {
            /* specify the divider and disable it */
            PERI_DIV_CMD(grpNum) = PERI_DIV_CMD_DISABLE_Msk          |
             _VAL2FLD(CY_PERI_DIV_CMD_TYPE_SEL, dividerType) |
             _VAL2FLD(CY_PERI_DIV_CMD_DIV_SEL,  dividerNum);
            retVal = CY_SYSCLK_SUCCESS;
        }
    }
    return (retVal);
}


cy_en_sysclk_status_t
                Cy_SysClk_PeriPclkEnablePhaseAlignDivider(en_clk_dst_t ipBlock, cy_en_divider_types_t dividerType, uint32_t dividerNum,
                                                        cy_en_divider_types_t dividerTypePA, uint32_t dividerNumPA)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;
    uint8_t grpNum = (ipBlock & PERI_PCLK_GR_NUM_Msk )>>PERI_PCLK_GR_NUM_Pos;

    if (dividerTypePA <= CY_SYSCLK_DIV_24_5_BIT)
    {
        if (((dividerTypePA == CY_SYSCLK_DIV_8_BIT)    && (dividerNumPA < PERI_DIV_8_NR))    ||
            ((dividerTypePA == CY_SYSCLK_DIV_16_BIT)   && (dividerNumPA < PERI_DIV_16_NR))   ||
            ((dividerTypePA == CY_SYSCLK_DIV_16_5_BIT) && (dividerNumPA < PERI_DIV_16_5_NR)) ||
            ((dividerTypePA == CY_SYSCLK_DIV_24_5_BIT) && ((dividerNumPA < PERI_DIV_24_5_NR) || (dividerNumPA == 63u))))
        {
            /* First, disable the divider that is to be phase-aligned.
               The other two parameters are checked in that function;
               if they're not valid, the divider is not disabled. */
            retVal = Cy_SysClk_PeriphDisableDivider(dividerType, dividerNum);
            if (retVal == CY_SYSCLK_SUCCESS)
            {
                /* Then, specify the reference divider, and the divider, and enable the divider */
                PERI_DIV_CMD(grpNum) = PERI_DIV_CMD_ENABLE_Msk             |
                 _VAL2FLD(CY_PERI_DIV_CMD_PA_TYPE_SEL, dividerTypePA) |
                 _VAL2FLD(CY_PERI_DIV_CMD_PA_DIV_SEL,  dividerNumPA)  |
                 _VAL2FLD(CY_PERI_DIV_CMD_TYPE_SEL, dividerType)   |
                 _VAL2FLD(CY_PERI_DIV_CMD_DIV_SEL,  dividerNum);
            }
        }
    }
    return (retVal);
}


bool Cy_SysClk_PeriPclkGetDividerEnabled(en_clk_dst_t ipBlock, cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    bool retVal = false;
    uint8_t grpNum = (ipBlock & PERI_PCLK_GR_NUM_Msk )>>PERI_PCLK_GR_NUM_Pos;

    CY_ASSERT_L1(((dividerType == CY_SYSCLK_DIV_8_BIT)    && (dividerNum < PERI_DIV_8_NR))    ||
                 ((dividerType == CY_SYSCLK_DIV_16_BIT)   && (dividerNum < PERI_DIV_16_NR))   ||
                 ((dividerType == CY_SYSCLK_DIV_16_5_BIT) && (dividerNum < PERI_DIV_16_5_NR)) ||
                 ((dividerType == CY_SYSCLK_DIV_24_5_BIT) && (dividerNum < PERI_DIV_24_5_NR)));

    switch(dividerType)
    {
        case CY_SYSCLK_DIV_8_BIT:
            retVal = _FLD2BOOL(PERI_DIV_8_CTL_EN, PERI_DIV_8_CTL(grpNum, dividerNum));
            break;
        case CY_SYSCLK_DIV_16_BIT:
            retVal = _FLD2BOOL(PERI_DIV_16_CTL_EN, PERI_DIV_16_CTL(grpNum, dividerNum));
            break;
        case CY_SYSCLK_DIV_16_5_BIT:
            retVal = _FLD2BOOL(PERI_DIV_16_5_CTL_EN, PERI_DIV_16_5_CTL(grpNum, dividerNum));
            break;
        case CY_SYSCLK_DIV_24_5_BIT:
            retVal = _FLD2BOOL(PERI_DIV_24_5_CTL_EN, PERI_DIV_24_5_CTL(grpNum, dividerNum));
            break;
        default:
            /* Unknown Divider */
            break;
    }
    return (retVal);
}


cy_en_sysclk_status_t
                Cy_SysClk_PeriphSetDivider(cy_en_divider_types_t dividerType,
                                           uint32_t dividerNum, uint32_t dividerValue)
{
    return Cy_SysClk_PeriPclkSetDivider((en_clk_dst_t)PERI_PCLK_PERIPHERAL_GROUP_NUM, dividerType, dividerNum, dividerValue);
}


uint32_t Cy_SysClk_PeriphGetDivider(cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    return Cy_SysClk_PeriPclkGetDivider((en_clk_dst_t)PERI_PCLK_PERIPHERAL_GROUP_NUM, dividerType, dividerNum);
}


cy_en_sysclk_status_t
                Cy_SysClk_PeriphSetFracDivider(cy_en_divider_types_t dividerType, uint32_t dividerNum,
                                               uint32_t dividerIntValue, uint32_t dividerFracValue)
{
    return Cy_SysClk_PeriPclkSetFracDivider((en_clk_dst_t)PERI_PCLK_PERIPHERAL_GROUP_NUM, dividerType, dividerNum, dividerIntValue, dividerFracValue);
}


void Cy_SysClk_PeriphGetFracDivider(cy_en_divider_types_t dividerType, uint32_t dividerNum,
                                                    uint32_t *dividerIntValue, uint32_t *dividerFracValue)
{
    Cy_SysClk_PeriPclkGetFracDivider((en_clk_dst_t)PERI_PCLK_PERIPHERAL_GROUP_NUM, dividerType, dividerNum, dividerIntValue, dividerFracValue);
}


cy_en_sysclk_status_t
                Cy_SysClk_PeriphAssignDivider(en_clk_dst_t ipBlock,
                                              cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    return Cy_SysClk_PeriPclkAssignDivider(ipBlock, dividerType, dividerNum);
}


uint32_t Cy_SysClk_PeriphGetAssignedDivider(en_clk_dst_t ipBlock)
{
    return Cy_SysClk_PeriPclkGetAssignedDivider(ipBlock);
}


cy_en_sysclk_status_t
                Cy_SysClk_PeriphEnableDivider(cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    return Cy_SysClk_PeriPclkEnableDivider((en_clk_dst_t)PERI_PCLK_PERIPHERAL_GROUP_NUM, dividerType, dividerNum);
}


cy_en_sysclk_status_t
                Cy_SysClk_PeriphDisableDivider(cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    return Cy_SysClk_PeriPclkDisableDivider((en_clk_dst_t)PERI_PCLK_PERIPHERAL_GROUP_NUM, dividerType, dividerNum);
}


cy_en_sysclk_status_t
                Cy_SysClk_PeriphEnablePhaseAlignDivider(cy_en_divider_types_t dividerType, uint32_t dividerNum,
                                                        cy_en_divider_types_t dividerTypePA, uint32_t dividerNumPA)
{
    return Cy_SysClk_PeriPclkEnablePhaseAlignDivider((en_clk_dst_t)PERI_PCLK_PERIPHERAL_GROUP_NUM, dividerType, dividerNum, dividerTypePA,  dividerNumPA);
}


bool Cy_SysClk_PeriphGetDividerEnabled(cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    return Cy_SysClk_PeriPclkGetDividerEnabled((en_clk_dst_t)PERI_PCLK_PERIPHERAL_GROUP_NUM, dividerType, dividerNum);
}


/* ========================================================================== */
/* =========================    clk_slow SECTION    ========================= */
/* ========================================================================== */


uint32_t Cy_SysClk_ClkSlowGetFrequency(void)
{
    return 0;
}


void Cy_SysClk_ClkSlowSetDivider(uint8_t divider)
{

}


uint8_t Cy_SysClk_ClkSlowGetDivider(void)
{

    return 0;
}

/* ========================================================================== */
/* =========================    clk_pump SECTION    ========================= */
/* ========================================================================== */

void Cy_SysClk_ClkPumpSetSource(cy_en_clkpump_in_sources_t source)
{
}


cy_en_clkpump_in_sources_t Cy_SysClk_ClkPumpGetSource(void)
{
    return CY_SYSCLK_PUMP_IN_CLKPATH0;
}


void Cy_SysClk_ClkPumpSetDivider(cy_en_clkpump_divide_t divider)
{
}


cy_en_clkpump_divide_t Cy_SysClk_ClkPumpGetDivider(void)
{
    return CY_SYSCLK_PUMP_NO_DIV;
}


void Cy_SysClk_ClkPumpEnable(void)
{
}


void Cy_SysClk_ClkPumpDisable(void)
{
}

/* ========================================================================== */
/* ==========================    clk_bak SECTION    ========================= */
/* ========================================================================== */

void Cy_SysClk_ClkBakSetSource(cy_en_clkbak_in_sources_t source)
{
    CY_ASSERT_L3(source <= CY_SYSCLK_BAK_IN_PILO);
    CY_REG32_CLR_SET(BACKUP_CTL, BACKUP_CTL_CLK_SEL, source);
}


cy_en_clkbak_in_sources_t Cy_SysClk_ClkBakGetSource(void)
{
    return ((cy_en_clkbak_in_sources_t)_FLD2VAL(BACKUP_CTL_CLK_SEL, BACKUP_CTL));
}

/* ========================================================================== */
/* ========================    clk_timer SECTION    ========================= */
/* ========================================================================== */

void Cy_SysClk_ClkTimerSetSource(cy_en_clktimer_in_sources_t source)
{

}

cy_en_clktimer_in_sources_t Cy_SysClk_ClkTimerGetSource(void)
{
    return (cy_en_clktimer_in_sources_t)0;
}

void Cy_SysClk_ClkTimerSetDivider(uint8_t divider)
{

}

uint8_t Cy_SysClk_ClkTimerGetDivider(void)
{
    return 0;
}

void Cy_SysClk_ClkTimerEnable(void)
{
 
}

void Cy_SysClk_ClkTimerDisable(void)
{

}

/* ========================================================================== */
/* ===========================    clkLf SECTION    ========================== */
/* ========================================================================== */

void Cy_SysClk_ClkLfSetSource(cy_en_clklf_in_sources_t source)
{
    CY_ASSERT_L3(source <= CY_SYSCLK_CLKLF_IN_PILO);
    CY_REG32_CLR_SET(SRSS_CLK_SELECT, SRSS_CLK_SELECT_LFCLK_SEL, source);
}


cy_en_clklf_in_sources_t Cy_SysClk_ClkLfGetSource(void)
{
    return ((cy_en_clklf_in_sources_t)(_FLD2VAL(SRSS_CLK_SELECT_LFCLK_SEL, SRSS_CLK_SELECT)));
}

/* ========================================================================== */
/* ========================    clk_peri SECTION    ========================== */
/* ========================================================================== */

uint32_t Cy_SysClk_ClkPeriGetFrequency(void)
{
    return 0;
}


void Cy_SysClk_ClkPeriSetDivider(uint8_t divider)
{
}


uint8_t Cy_SysClk_ClkPeriGetDivider(void)
{
    return 0;
}


/* ========================================================================== */
/* ========================    PERI SECTION    ========================== */
/* ========================================================================== */


cy_en_sysclk_status_t Cy_SysClk_PeriGroupSetDivider(uint8_t groupNum, int8_t divider)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;

    if (groupNum < CY_PERI_GROUP_NR)
    {
        if ((divider <= (PERI_GR_CLOCK_CTL_INT8_DIV_Msk >> PERI_GR_CLOCK_CTL_INT8_DIV_Pos)))
        {
            CY_REG32_CLR_SET(PERI_GR_CLOCK_CTL(groupNum), PERI_GR_CLOCK_CTL_INT8_DIV, divider);
            retVal = CY_SYSCLK_SUCCESS;
        }
    }

    return retVal;
}


int8_t Cy_SysClk_PeriGroupGetDivider(uint8_t groupNum)
{
    int8_t retVal = 0;

    if (groupNum < CY_PERI_GROUP_NR)
    {
        return(_FLD2VAL(PERI_GR_CLOCK_CTL_INT8_DIV, PERI_GR_CLOCK_CTL(groupNum)));

    }

    return retVal;
}


cy_en_sysclk_status_t Cy_SysClk_PeriGroupSetSlaveCtl(uint8_t groupNum, uint32_t slaveCtl)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;

    if (groupNum < CY_PERI_GROUP_NR)
    {
        CY_REG32_CLR_SET(PERI_GR_SL_CTL(groupNum), PERI_GR_SL_CTL_ENABLED, slaveCtl);
        retVal = CY_SYSCLK_SUCCESS;
    }

    return retVal;
}


uint32_t Cy_SysClk_PeriGroupGetSlaveCtl(uint8_t groupNum)
{
    uint32_t retVal = 0;

    if (groupNum < CY_PERI_GROUP_NR)
    {
        retVal = _FLD2VAL(PERI_GR_SL_CTL_ENABLED, PERI_GR_SL_CTL(groupNum));

    }

    return retVal;
}


/* ========================================================================== */
/* =========================    clk_fast SECTION    ========================= */
/* ========================================================================== */


uint32_t Cy_SysClk_ClkFastGetFrequency(void)
{
    return 0;
}


void Cy_SysClk_ClkFastSetDivider(uint8_t divider)
{

}


uint8_t Cy_SysClk_ClkFastGetDivider(void)
{
    return 0;
}

/* ========================================================================== */
/* =========================    clkHf[n] SECTION    ========================= */
/* ========================================================================== */

cy_en_sysclk_status_t Cy_SysClk_ClkHfEnable(uint32_t clkHf)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;
    if (clkHf < CY_SRSS_NUM_HFROOT)
    {
        SRSS_CLK_ROOT_SELECT[clkHf] |= SRSS_CLK_ROOT_SELECT_ENABLE_Msk;
        retVal = CY_SYSCLK_SUCCESS;
    }
    return (retVal);
}


bool Cy_SysClk_ClkHfIsEnabled(uint32_t clkHf)
{
    bool retVal = false;
    if (clkHf < CY_SRSS_NUM_HFROOT)
    {
        retVal = _FLD2BOOL(SRSS_CLK_ROOT_SELECT_ENABLE, SRSS_CLK_ROOT_SELECT[clkHf]);
    }
    return (retVal);
}

cy_en_sysclk_status_t Cy_SysClk_ClkHfDisable(uint32_t clkHf)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;
    if ((0UL < clkHf) /* prevent CLK_HF0 disabling */
           && (clkHf < CY_SRSS_NUM_HFROOT))
    {
        SRSS_CLK_ROOT_SELECT[clkHf] &= ~SRSS_CLK_ROOT_SELECT_ENABLE_Msk;
        retVal = CY_SYSCLK_SUCCESS;
    }
    return (retVal);
}


cy_en_sysclk_status_t Cy_SysClk_ClkHfSetSource(uint32_t clkHf, cy_en_clkhf_in_sources_t source)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;
    if ((clkHf < CY_SRSS_NUM_HFROOT) && (source <= CY_SYSCLK_CLKHF_IN_CLKPATH15))
    {
        CY_REG32_CLR_SET(SRSS_CLK_ROOT_SELECT[clkHf], SRSS_CLK_ROOT_SELECT_ROOT_MUX, source);
        retVal = CY_SYSCLK_SUCCESS;
    }
    return (retVal);
}


cy_en_clkhf_in_sources_t Cy_SysClk_ClkHfGetSource(uint32_t clkHf)
{
    CY_ASSERT_L1(clkHf < CY_SRSS_NUM_HFROOT);
    return ((cy_en_clkhf_in_sources_t)((uint32_t)(_FLD2VAL(SRSS_CLK_ROOT_SELECT_ROOT_MUX, SRSS_CLK_ROOT_SELECT[clkHf]))));
}


cy_en_sysclk_status_t Cy_SysClk_ClkHfSetDivider(uint32_t clkHf, cy_en_clkhf_dividers_t divider)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;
    if ((clkHf < CY_SRSS_NUM_HFROOT) && (divider <= CY_SYSCLK_CLKHF_DIVIDE_BY_8))
    {
        CY_REG32_CLR_SET(SRSS_CLK_ROOT_SELECT[clkHf], SRSS_CLK_ROOT_SELECT_ROOT_DIV, divider);
        retVal = CY_SYSCLK_SUCCESS;
    }
    return (retVal);
}


cy_en_clkhf_dividers_t Cy_SysClk_ClkHfGetDivider(uint32_t clkHf)
{
    CY_ASSERT_L1(clkHf < CY_SRSS_NUM_HFROOT);
    return ((cy_en_clkhf_dividers_t)(((uint32_t)_FLD2VAL(SRSS_CLK_ROOT_SELECT_ROOT_DIV, SRSS_CLK_ROOT_SELECT[clkHf]))));
}


/* ========================================================================== */
/* ============================    MFO SECTION    ============================ */
/* ========================================================================== */


void Cy_SysClk_MfoEnable(bool deepSleepEnable)
{
#if (CY_SRSS_MFO_PRESENT)
#if defined (CY_IP_MXS28SRSS)
    SRSS_CLK_MFO_CONFIG = SRSS_CLK_MFO_CONFIG_ENABLE_Msk;
#else
    SRSS_CLK_MFO_CONFIG = SRSS_CLK_MFO_CONFIG_ENABLE_Msk | (deepSleepEnable ? SRSS_CLK_MFO_CONFIG_DPSLP_ENABLE_Msk : 0UL);
#endif
#endif
}

bool Cy_SysClk_MfoIsEnabled(void)
{
#if (CY_SRSS_MFO_PRESENT)
    return (CY_SRSS_MFO_PRESENT && (0UL != (SRSS_CLK_MFO_CONFIG & SRSS_CLK_MFO_CONFIG_ENABLE_Msk)));
#else
    return false;
#endif

}

void Cy_SysClk_MfoDisable(void)
{
#if (CY_SRSS_MFO_PRESENT)
    SRSS_CLK_MFO_CONFIG = 0UL;
#endif
}


/* ========================================================================== */
/* ============================    CLK_MF SECTION    ============================ */
/* ========================================================================== */


void Cy_SysClk_ClkMfEnable(void)
{
#if (CY_SRSS_MFO_PRESENT)
    SRSS_CLK_MF_SELECT |= SRSS_CLK_MF_SELECT_ENABLE_Msk;
#endif
}


bool Cy_SysClk_ClkMfIsEnabled(void)
{
#if (CY_SRSS_MFO_PRESENT)
    return ((0UL != (SRSS_CLK_MF_SELECT & SRSS_CLK_MF_SELECT_ENABLE_Msk)));
#else
    return false;
#endif
}


void Cy_SysClk_ClkMfDisable(void)
{
#if (CY_SRSS_MFO_PRESENT)
    SRSS_CLK_MF_SELECT &= ~SRSS_CLK_MF_SELECT_ENABLE_Msk;
#endif
}


void Cy_SysClk_ClkMfSetDivider(uint32_t divider)
{
#if (CY_SRSS_MFO_PRESENT)
    if ((CY_SRSS_MFO_PRESENT) && CY_SYSCLK_IS_MF_DIVIDER_VALID(divider))
    { 
        if (!Cy_SysClk_ClkMfIsEnabled())
        {
            CY_REG32_CLR_SET(SRSS_CLK_MF_SELECT, SRSS_CLK_MF_SELECT_MFCLK_DIV, divider - 1UL);
        }
    }
#endif
}


uint32_t Cy_SysClk_ClkMfGetDivider(void)
{
#if (CY_SRSS_MFO_PRESENT)
    return ((CY_SRSS_MFO_PRESENT) ? (1UL + _FLD2VAL(SRSS_CLK_MF_SELECT_MFCLK_DIV, SRSS_CLK_MF_SELECT)) : 1UL);
#else
    return 0;
#endif
}


uint32_t Cy_SysClk_ClkMfGetFrequency(void)
{
#if (CY_SRSS_MFO_PRESENT)
    uint32_t locFreq = (Cy_SysClk_MfoIsEnabled()) ? CY_SYSCLK_MFO_FREQ : 0UL; /* Get root frequency */
    uint32_t locDiv = Cy_SysClk_ClkMfGetDivider(); /* clkMf prescaler (1-256) */

    /* Divide the path input frequency down and return the result */
    return (CY_SYSLIB_DIV_ROUND(locFreq, locDiv));
#else
    return 0;
#endif
}

/* ========================================================================== */
/* ===========================    WCO SECTION    =========================== */
/* ========================================================================== */

cy_en_sysclk_status_t Cy_SysClk_WcoEnable(uint32_t timeoutus)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_TIMEOUT;
    
    /* Enable WCO */
#if defined (CY_IP_MXS28SRSS)
    BACKUP_WCO_CTL |= BACKUP_WCO_CTL_WCO_EN_Msk;
#else
    BACKUP_CTL |= BACKUP_CTL_WCO_EN_Msk;
#endif

    /* now do the timeout wait for STATUS, bit WCO_OK */
    for (; (Cy_SysClk_WcoOkay() == false) && (0UL != timeoutus); timeoutus--)
    {
        Cy_SysLib_DelayUs(1U);
    }
    
    if (0UL != timeoutus)
    {
        retVal = CY_SYSCLK_SUCCESS;
    }
    
    return (retVal);
}

bool Cy_SysClk_WcoOkay(void)
{
    return (_FLD2BOOL(BACKUP_WCO_STATUS_WCO_OK, BACKUP_WCO_STATUS));
}

void Cy_SysClk_WcoDisable(void)
{
#if defined (CY_IP_MXS28SRSS)
    BACKUP_WCO_CTL &= (uint32_t)~BACKUP_WCO_CTL_WCO_EN_Msk;
#else
    BACKUP_CTL &= (uint32_t)~BACKUP_CTL_WCO_EN_Msk;
#endif
}

#if defined (CY_IP_MXS28SRSS)
void Cy_SysClk_WcoGainControl(cy_en_wco_gain_ctrl_modes_t gmMode)
{
    CY_REG32_CLR_SET(BACKUP_WCO_CTL, BACKUP_WCO_CTL_GM, gmMode);
}
#endif

void Cy_SysClk_WcoBypass(cy_en_wco_bypass_modes_t bypass)
{
#if defined (CY_IP_MXS28SRSS)
    CY_REG32_CLR_SET(BACKUP_WCO_CTL, BACKUP_WCO_CTL_WCO_BYP_EN, bypass);
#else
    CY_REG32_CLR_SET(BACKUP_CTL, BACKUP_CTL_WCO_BYPASS, bypass);
#endif
}

/* ========================================================================== */
/* ===========================    PILO SECTION    =========================== */
/* ========================================================================== */

void Cy_SysClk_PiloEnable(void)
{
#if (CY_SRSS_PILO_PRESENT)
    SRSS_CLK_PILO_CONFIG |= SRSS_CLK_PILO_CONFIG_PILO_EN_Msk; /* 1 = enable */
    Cy_SysLib_Delay(1U/*msec */);
    /* release the reset and enable clock output */
    SRSS_CLK_PILO_CONFIG |= SRSS_CLK_PILO_CONFIG_PILO_RESET_N_Msk |
                            SRSS_CLK_PILO_CONFIG_PILO_CLK_EN_Msk;
#endif
}

bool Cy_SysClk_PiloIsEnabled(void)
{
#if (CY_SRSS_PILO_PRESENT)
    return (_FLD2BOOL(SRSS_CLK_PILO_CONFIG_PILO_CLK_EN, SRSS_CLK_PILO_CONFIG));
#else
    return false;
#endif
}

void Cy_SysClk_PiloDisable(void)
{
#if (CY_SRSS_PILO_PRESENT)
    /* Clear PILO_EN, PILO_RESET_N, and PILO_CLK_EN bitfields. This disables the
       PILO and holds the PILO in a reset state. */
    SRSS_CLK_PILO_CONFIG &= (uint32_t)~(SRSS_CLK_PILO_CONFIG_PILO_EN_Msk    |
                        SRSS_CLK_PILO_CONFIG_PILO_RESET_N_Msk |
                        SRSS_CLK_PILO_CONFIG_PILO_CLK_EN_Msk);
#endif
}


void Cy_SysClk_PiloSetTrim(uint32_t trimVal)
{
#if (CY_SRSS_PILO_PRESENT)
    CY_REG32_CLR_SET(SRSS_CLK_PILO_CONFIG, SRSS_CLK_PILO_CONFIG_PILO_FFREQ, trimVal);
#endif
}


uint32_t Cy_SysClk_PiloGetTrim(void)
{
#if (CY_SRSS_PILO_PRESENT)
    return (_FLD2VAL(SRSS_CLK_PILO_CONFIG_PILO_FFREQ, SRSS_CLK_PILO_CONFIG));
#else
    return 0;
#endif
}

/* ========================================================================== */
/* ==========================    ALTHF SECTION    =========================== */
/* ========================================================================== */

uint32_t Cy_SysClk_AltHfGetFrequency(void)
{
    #if defined (CY_IP_MXBLESS)
        return (cy_BleEcoClockFreqHz);
    #else /* CY_IP_MXBLESS */
        return (0UL);
    #endif /* CY_IP_MXBLESS */
}

/* ========================================================================== */
/* ==========================    ALTLF SECTION    =========================== */
/* ========================================================================== */


uint32_t Cy_SysClk_AltLfGetFrequency(void)
{
    return (0UL);
}


bool Cy_SysClk_AltLfIsEnabled(void)
{
    return (false);
}


/* ========================================================================== */
/* ===========================    ILO SECTION    ============================ */
/* ========================================================================== */


void Cy_SysClk_IloEnable(void)
{
    SRSS_CLK_ILO_CONFIG |= SRSS_CLK_ILO_CONFIG_ENABLE_Msk;
}


cy_en_sysclk_status_t Cy_SysClk_IloDisable(void)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_INVALID_STATE;
    if (!_FLD2BOOL(SRSS_WDT_CTL_WDT_EN, SRSS_WDT_CTL)) /* if disabled */
    {
        SRSS_CLK_ILO_CONFIG &= ~SRSS_CLK_ILO_CONFIG_ENABLE_Msk;
        retVal = CY_SYSCLK_SUCCESS;
    }
    return (retVal);
}


void Cy_SysClk_IloHibernateOn(bool on)
{
    CY_REG32_CLR_SET(SRSS_CLK_ILO_CONFIG, SRSS_CLK_ILO_CONFIG_ILO_BACKUP, ((on) ? 1UL : 0UL));
}


/* ========================================================================== */
/* =========================    EXTCLK SECTION    =========================== */
/* ========================================================================== */


    static uint32_t cySysClkExtFreq = 0UL;


#define CY_SYSCLK_EXTCLK_MAX_FREQ (100000000UL) /* 100 MHz */


void Cy_SysClk_ExtClkSetFrequency(uint32_t freq)
{
    if (freq <= CY_SYSCLK_EXTCLK_MAX_FREQ)
    {
        cySysClkExtFreq = freq;
    }
}

/*******************************************************************************
* Function Name: Cy_SysClk_ExtClkGetFrequency
****************************************************************************//**
*
* Returns the frequency of the External Clock Source (EXTCLK) from the
* internal storage.
*
* \return The frequency of the External Clock Source.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_ExtClkSetFrequency
*
*******************************************************************************/
uint32_t Cy_SysClk_ExtClkGetFrequency(void)
{
    return (cySysClkExtFreq);
}

/* ========================================================================== */
/* ===========================    ECO SECTION    ============================ */
/* ========================================================================== */
#if (CY_SRSS_ECO_PRESENT)
static uint32_t ecoFreq = 0UL; /* Internal storage for ECO frequency user setting */
#endif

void Cy_SysClk_EcoDisable(void)
{
#if (CY_SRSS_ECO_PRESENT)
    SRSS_CLK_ECO_CONFIG &= ~SRSS_CLK_ECO_CONFIG_ECO_EN_Msk;
#endif
}


uint32_t Cy_SysClk_EcoGetStatus(void)
{
#if (CY_SRSS_ECO_PRESENT)
    uint32_t eco_status        = SRSS_CLK_ECO_STATUS;
    uint32_t eco_status_mask   = SRSS_CLK_ECO_AMP_OK_Msk | SRSS_CLK_ECO_READY_Msk;

    /* if ECO is not ready, just report the ECO_OK bit. Otherwise report 2 = ECO ready */
    return ((eco_status_mask == (eco_status_mask & eco_status)) ?
        CY_SYSCLK_ECOSTAT_STABLE : (SRSS_CLK_ECO_AMP_OK_Msk & eco_status));
#else
    return 0;
#endif
}


uint32_t Cy_SysClk_EcoBleGetStatus(void)
{
#if (CY_SRSS_ECO_PRESENT)
    /* if ECO for BLE is Enabled, report 1. Otherwise report 0 */
    return ((SRSS_CLK_ECO_STATUS_ECO_BLE_ENABLED_Msk == (SRSS_CLK_ECO_STATUS_ECO_BLE_ENABLED_Msk & SRSS_CLK_ECO_STATUS)) ?
      CY_SYSCLK_ECOSTAT_BLE_ENABLED : CY_SYSCLK_ECOSTAT_BLE_DISABLED);
#else
    return 0;
#endif
}


cy_en_sysclk_status_t Cy_SysClk_EcoConfigure(uint32_t freq, uint32_t cSum, uint32_t esr, uint32_t driveLevel)
{
    /* Legacy */
    return CY_SYSCLK_INVALID_STATE;
}


cy_en_sysclk_status_t Cy_SysClk_EcoEnable(uint32_t timeoutus)
{
#if (CY_SRSS_ECO_PRESENT)
    cy_en_sysclk_status_t retVal = CY_SYSCLK_INVALID_STATE;
    bool zeroTimeout = (0UL == timeoutus);

    /* Invalid state error if ECO is already enabled */
    if (0UL == (SRSS_CLK_ECO_CONFIG_ECO_EN_Msk & SRSS_CLK_ECO_CONFIG))
    {
        /* Set ECO enable */
        SRSS_CLK_ECO_CONFIG |= SRSS_CLK_ECO_CONFIG_ECO_EN_Msk;

        /* Wait for CY_SYSCLK_ECOSTAT_STABLE */
        for (; (CY_SYSCLK_ECOSTAT_STABLE != Cy_SysClk_EcoGetStatus()) && (0UL != timeoutus); timeoutus--)
        {
            Cy_SysLib_DelayUs(1U);
        }

        if (zeroTimeout || (0UL != timeoutus))
        {
            retVal = CY_SYSCLK_SUCCESS;
        }
        else
        {
            /* If ECO doesn't start, then disable it */
            SRSS_CLK_ECO_CONFIG &= ~SRSS_CLK_ECO_CONFIG_ECO_EN_Msk;
            retVal = CY_SYSCLK_TIMEOUT;
        }
    }

    return (retVal);

#else
    return CY_SYSCLK_INVALID_STATE;
#endif
}

/*******************************************************************************
* Function Name: Cy_SysClk_EcoGetFrequency
****************************************************************************//**
*
* Returns the frequency of the external crystal oscillator (ECO).
*
* \return The frequency of the ECO.
*
* \note If the ECO is not enabled or stable - a zero is returned.
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_EcoEnable
*
*******************************************************************************/
uint32_t Cy_SysClk_EcoGetFrequency(void)
{
#if (CY_SRSS_ECO_PRESENT)
    return ((CY_SYSCLK_ECOSTAT_STABLE == Cy_SysClk_EcoGetStatus()) ? ecoFreq : 0UL);
#else
    return 0;
#endif
}

#if defined (CY_IP_MXS28SRSS)
cy_en_sysclk_status_t Cy_SysClk_EcoBleControl(cy_en_eco_for_ble_t control, uint32_t timeoutus)
{
#if (CY_SRSS_ECO_PRESENT)
    cy_en_sysclk_status_t retVal = CY_SYSCLK_INVALID_STATE;
    bool zeroTimeout = (0UL == timeoutus);

    CY_ASSERT_L1(control < sizeof(cy_en_eco_for_ble_t));

    /* Set ECO for BLE with control value */
    SRSS_CLK_ECO_CONFIG |= (control << SRSS_CLK_ECO_CONFIG_ECO_BLE_EN_Pos);

    /* Wait for CY_SYSCLK_ECOSTAT_STABLE */
    for (; (CY_SYSCLK_ECOSTAT_BLE_ENABLED != Cy_SysClk_EcoBleGetStatus()) && (0UL != timeoutus); timeoutus--)
    {
        Cy_SysLib_DelayUs(1U);
    }

    retVal = (zeroTimeout || (0UL != timeoutus)) ? CY_SYSCLK_SUCCESS : CY_SYSCLK_TIMEOUT;

    return (retVal);
#else
    return CY_SYSCLK_INVALID_STATE;
#endif
}
#endif

cy_en_sysclk_status_t Cy_SysClk_EcoPrescaleConfigure(uint32_t enable, uint32_t frac_div, uint32_t int_div)
{
#if (CY_SRSS_ECO_PRESENT)
    cy_en_sysclk_status_t retVal = CY_SYSCLK_INVALID_STATE;

    if(enable) {
        /* Invalid state error if CO_DIV_ENABLED is already enabled */
        if (0UL == (SRSS_CLK_ECO_PRESCALE_ECO_DIV_ENABLED_Msk & SRSS_CLK_ECO_PRESCALE))
        {
            SRSS_CLK_ECO_PRESCALE |= SRSS_CLK_ECO_PRESCALE_ECO_DIV_ENABLED_Msk;
            SRSS_CLK_ECO_PRESCALE = _VAL2FLD(SRSS_CLK_ECO_PRESCALE_ECO_FRAC_DIV, frac_div);
            SRSS_CLK_ECO_PRESCALE = _VAL2FLD(SRSS_CLK_ECO_PRESCALE_ECO_INT_DIV, int_div);    
            retVal = CY_SYSCLK_SUCCESS;
        }
    }
    else {
        /* Invalid state error if CO_DIV_ENABLED is already disabled */
        if (1UL == (SRSS_CLK_ECO_PRESCALE_ECO_DIV_ENABLED_Msk & SRSS_CLK_ECO_PRESCALE))
        {
            SRSS_CLK_ECO_PRESCALE &= ~(SRSS_CLK_ECO_PRESCALE_ECO_DIV_ENABLED_Msk);
            retVal = CY_SYSCLK_SUCCESS;
        }
    }
    return retVal;
#else
    return CY_SYSCLK_INVALID_STATE;
#endif
}


/* ========================================================================== */
/* ====================    INPUT MULTIPLEXER SECTION    ===================== */
/* ========================================================================== */


cy_en_sysclk_status_t Cy_SysClk_ClkPathSetSource(uint32_t clkPath, cy_en_clkpath_in_sources_t source)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;
    if ((clkPath < CY_SRSS_NUM_CLKPATH) &&
        ((source <= CY_SYSCLK_CLKPATH_IN_DSIMUX) ||
         ((CY_SYSCLK_CLKPATH_IN_DSI <= source) && (source <= CY_SYSCLK_CLKPATH_IN_PILO))))
    {
        if (source >= CY_SYSCLK_CLKPATH_IN_DSI)
        {
            SRSS_CLK_DSI_SELECT[clkPath] = _VAL2FLD(SRSS_CLK_DSI_SELECT_DSI_MUX, (uint32_t)source);
            SRSS_CLK_PATH_SELECT[clkPath] = _VAL2FLD(SRSS_CLK_PATH_SELECT_PATH_MUX, (uint32_t)CY_SYSCLK_CLKPATH_IN_DSIMUX);
        }
        else
        {
            SRSS_CLK_PATH_SELECT[clkPath] = _VAL2FLD(SRSS_CLK_PATH_SELECT_PATH_MUX, (uint32_t)source);
        }
        retVal = CY_SYSCLK_SUCCESS;
    }
    return (retVal);
}

cy_en_clkpath_in_sources_t Cy_SysClk_ClkPathGetSource(uint32_t clkPath)
{
    CY_ASSERT_L1(clkPath < CY_SRSS_NUM_CLKPATH);
    cy_en_clkpath_in_sources_t retVal =
        (cy_en_clkpath_in_sources_t )((uint32_t)_FLD2VAL(SRSS_CLK_PATH_SELECT_PATH_MUX, SRSS_CLK_PATH_SELECT[clkPath]));
    if (retVal == CY_SYSCLK_CLKPATH_IN_DSIMUX)
    {
        retVal = (cy_en_clkpath_in_sources_t)((uint32_t)(((uint32_t)CY_SYSCLK_CLKPATH_IN_DSI) |
                    ((uint32_t)(_FLD2VAL(SRSS_CLK_DSI_SELECT_DSI_MUX, SRSS_CLK_DSI_SELECT[clkPath])))));
    }
    return (retVal);
}

uint32_t Cy_SysClk_ClkPathMuxGetFrequency(uint32_t clkPath)
{
    CY_ASSERT_L1(clkPath < CY_SRSS_NUM_CLKPATH);

    uint32_t freq = 0UL;    /* The path mux output frequency in Hz, 0 = an unknown frequency */

    /* Get the frequency of the source, i.e., the path mux input */
    switch(Cy_SysClk_ClkPathGetSource(clkPath))
    {
        case CY_SYSCLK_CLKPATH_IN_IMO: /* The IMO frequency is fixed at 8 MHz */
            freq = CY_SYSCLK_IMO_FREQ;
            break;

        case CY_SYSCLK_CLKPATH_IN_EXT:
            freq = Cy_SysClk_ExtClkGetFrequency();
            break;

        case CY_SYSCLK_CLKPATH_IN_ECO:
            freq = Cy_SysClk_EcoGetFrequency();
            break;

        case CY_SYSCLK_CLKPATH_IN_ALTHF:
            freq = Cy_SysClk_AltHfGetFrequency();
            break;

        case CY_SYSCLK_CLKPATH_IN_ILO:
            freq = (0UL != (SRSS_CLK_ILO_CONFIG & SRSS_CLK_ILO_CONFIG_ENABLE_Msk)) ? CY_SYSCLK_ILO_FREQ : 0UL;
            break;

        case CY_SYSCLK_CLKPATH_IN_WCO:
            freq = (Cy_SysClk_WcoOkay()) ? CY_SYSCLK_WCO_FREQ : 0UL;
            break;

        case CY_SYSCLK_CLKPATH_IN_PILO:
            freq = (0UL != (SRSS_CLK_PILO_CONFIG & SRSS_CLK_PILO_CONFIG_PILO_EN_Msk)) ? CY_SYSCLK_PILO_FREQ : 0UL;
            break;

        case CY_SYSCLK_CLKPATH_IN_ALTLF:
            freq = Cy_SysClk_AltLfGetFrequency();
            break;

        default:
            /* Don't know the frequency of dsi_out, leave freq = 0UL */
            break;
    }

    return (freq);
}

/*******************************************************************************
* Function Name: Cy_SysClk_ClkPathGetFrequency
****************************************************************************//**
*
* Returns the output frequency of the clock path mux.
*
* \return The output frequency of the path mux.
*
* \note If the return value equals zero, that means either:
* - the selected path mux source signal frequency is unknown (e.g. dsi_out, etc.) or
* - the selected path mux source is not configured/enabled/stable (e.g. ECO, EXTCLK, etc.).
*
* \funcusage
* \snippet sysclk/snippet/main.c snippet_Cy_SysClk_FllEnable
*
*******************************************************************************/
uint32_t Cy_SysClk_ClkPathGetFrequency(uint32_t clkPath)
{
    CY_ASSERT_L1(clkPath < CY_SRSS_NUM_CLKPATH);

    uint32_t freq = Cy_SysClk_ClkPathMuxGetFrequency(clkPath);
    uint32_t fDiv = 1UL;    /* FLL/PLL multiplier/feedback divider */
    uint32_t rDiv = 1UL;    /* FLL/PLL reference divider */
    uint32_t oDiv = 1UL;    /* FLL/PLL output divider */
    bool  enabled = false;  /* FLL or PLL enable status; n/a for direct */

    if (clkPath == (uint32_t)CY_SYSCLK_CLKHF_IN_CLKPATH0) /* FLL? (always path 0) */
    {
        cy_stc_fll_manual_config_t fllCfg = {0UL,0U,CY_SYSCLK_FLL_CCO_RANGE0,false,0U,0U,0U,0U,CY_SYSCLK_FLLPLL_OUTPUT_AUTO,0U};
        Cy_SysClk_FllGetConfiguration(&fllCfg);
        enabled = (Cy_SysClk_FllIsEnabled()) && (CY_SYSCLK_FLLPLL_OUTPUT_INPUT != fllCfg.outputMode);
        fDiv = fllCfg.fllMult;
        rDiv = fllCfg.refDiv;
        oDiv = (fllCfg.enableOutputDiv) ? 2UL : 1UL;
    }
    else if (clkPath <= CY_SRSS_NUM_PLL) /* PLL? (always path 1...N)*/
    {
        cy_stc_pll_manual_config_t pllcfg = {0U,0U,0U,false,CY_SYSCLK_FLLPLL_OUTPUT_AUTO};
        (void)Cy_SysClk_PllGetConfiguration(clkPath, &pllcfg);
        enabled = (Cy_SysClk_PllIsEnabled(clkPath)) && (CY_SYSCLK_FLLPLL_OUTPUT_INPUT != pllcfg.outputMode);
        fDiv = pllcfg.feedbackDiv;
        rDiv = pllcfg.referenceDiv;
        oDiv = pllcfg.outputDiv;
    }
    else
    {
        /* Do nothing with the path mux frequency */
    }

    if (enabled && /* If FLL or PLL is enabled and not bypassed */
        (0UL != rDiv) && (0UL != oDiv)) /* to avoid division by zero */
    {
        freq = (uint32_t)CY_SYSLIB_DIV_ROUND(((uint64_t)freq * (uint64_t)fDiv),
                                             ((uint64_t)rDiv * (uint64_t)oDiv));
    }

    return (freq);
}

/* ========================================================================== */
/* ===========================    FLL SECTION    ============================ */
/* ========================================================================== */

/* min and max FLL output frequencies, in Hz */
#define  CY_SYSCLK_FLL_MIN_CCO_OUTPUT_FREQ (48000000UL)
#define  CY_SYSCLK_FLL_MIN_OUTPUT_FREQ     (CY_SYSCLK_FLL_MIN_CCO_OUTPUT_FREQ / 2U)
#define  CY_SYSCLK_FLL_MAX_OUTPUT_FREQ     (100000000UL)

#define  CY_SYSCLK_FLL_IS_CCO_RANGE_VALID(range) (((range) == CY_SYSCLK_FLL_CCO_RANGE0) || \
                                                  ((range) == CY_SYSCLK_FLL_CCO_RANGE1) || \
                                                  ((range) == CY_SYSCLK_FLL_CCO_RANGE2) || \
                                                  ((range) == CY_SYSCLK_FLL_CCO_RANGE3) || \
                                                  ((range) == CY_SYSCLK_FLL_CCO_RANGE4))
/** \cond INTERNAL */
#define  CY_SYSCLK_FLL_INT_COEF (327680000UL)
#define  CY_SYSCLK_FLL_GAIN_IDX (11U)
#define  CY_SYSCLK_FLL_GAIN_VAL (8UL * CY_SYSCLK_FLL_INT_COEF)

#define TRIM_STEPS_SCALE        (100000000ULL) /* 10 ^ 8 */
#define MARGIN_SCALE            (100000ULL) /* 10 ^ 5 */
/** \endcond */

bool Cy_SysClk_FllIsEnabled(void)
{
#if (CY_SRSS_FLL_PRESENT)
    return (_FLD2BOOL(SRSS_CLK_FLL_CONFIG_FLL_ENABLE, SRSS_CLK_FLL_CONFIG));
#else
    return false;
#endif
}


bool Cy_SysClk_FllLocked(void)
{
#if (CY_SRSS_FLL_PRESENT)
    return (_FLD2BOOL(SRSS_CLK_FLL_STATUS_LOCKED, SRSS_CLK_FLL_STATUS));
#else
    return false;
#endif
}

cy_en_sysclk_status_t Cy_SysClk_FllDisable(void)
{
#if (CY_SRSS_FLL_PRESENT)
    CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG3, SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, CY_SYSCLK_FLLPLL_OUTPUT_INPUT);
    SRSS_CLK_FLL_CONFIG  &= ~SRSS_CLK_FLL_CONFIG_FLL_ENABLE_Msk;
    SRSS_CLK_FLL_CONFIG4 &= ~SRSS_CLK_FLL_CONFIG4_CCO_ENABLE_Msk;
    return (CY_SYSCLK_SUCCESS);
#else
    return (CY_SYSCLK_INVALID_STATE);
#endif
}

void Cy_SysClk_FllOutputDividerEnable(bool enable)
{
#if (CY_SRSS_FLL_PRESENT)
    SRSS_CLK_FLL_CONFIG = _BOOL2FLD(SRSS_CLK_FLL_CONFIG_FLL_OUTPUT_DIV, enable);
#endif
}


cy_en_sysclk_status_t Cy_SysClk_FllConfigure(uint32_t inputFreq, uint32_t outputFreq, cy_en_fll_pll_output_mode_t outputMode)
{
#if (CY_SRSS_FLL_PRESENT)
    cy_en_sysclk_status_t retVal = CY_SYSCLK_SUCCESS;

    /* check for errors */
    if ((outputFreq < CY_SYSCLK_FLL_MIN_OUTPUT_FREQ) || (CY_SYSCLK_FLL_MAX_OUTPUT_FREQ < outputFreq) || /* invalid output frequency */
      (((outputFreq * 5UL) / inputFreq) < 11UL)) /* check output/input frequency ratio */
    {
        retVal = CY_SYSCLK_BAD_PARAM;
    }
    else /* no errors */
    {
        /* If output mode is bypass (input routed directly to output), then done.
           The output frequency equals the input frequency regardless of the
           frequency parameters. */
        if (outputMode != CY_SYSCLK_FLLPLL_OUTPUT_INPUT)
        {
            cy_stc_fll_manual_config_t config;
            uint32_t ccoFreq;
            bool wcoSource = (CY_SYSCLK_CLKPATH_IN_WCO == Cy_SysClk_ClkPathGetSource(0UL/*FLL*/)) ? true : false;

            config.outputMode = outputMode;
            /* 1. Output division by 2 is always required */
            config.enableOutputDiv = true;
            /* 2. Compute the target CCO frequency from the target output frequency and output division */
            ccoFreq = outputFreq * 2UL;
            /* 3. Compute the CCO range value from the CCO frequency */
            config.ccoRange = ((ccoFreq >= 150339200UL) ? CY_SYSCLK_FLL_CCO_RANGE4 :
                               ((ccoFreq >= 113009380UL) ? CY_SYSCLK_FLL_CCO_RANGE3 :
                                ((ccoFreq >=  84948700UL) ? CY_SYSCLK_FLL_CCO_RANGE2 :
                                 ((ccoFreq >=  63855600UL) ? CY_SYSCLK_FLL_CCO_RANGE1 : CY_SYSCLK_FLL_CCO_RANGE0))));

        /* 4. Compute the FLL reference divider value.
              refDiv is a constant if the WCO is the FLL source, otherwise the formula is
              refDiv = ROUNDUP((inputFreq / outputFreq) * 250) */
            config.refDiv = wcoSource ? 19U : (uint16_t)CY_SYSLIB_DIV_ROUNDUP((uint64_t)inputFreq * 250ULL, (uint64_t)outputFreq);

        /* 5. Compute the FLL multiplier value.
              Formula is fllMult = ccoFreq / (inputFreq / refDiv) */
            config.fllMult = (uint32_t)CY_SYSLIB_DIV_ROUNDUP((uint64_t)ccoFreq * (uint64_t)config.refDiv, (uint64_t)inputFreq);
        /* 6. Compute the lock tolerance.
              Formula is lock tolerance = 1.5 * fllMult * (((1 + CCO accuracy) / (1 - source clock accuracy)) - 1)
              We assume CCO accuracy is 0.25%.
              We assume the source clock accuracy = 1%. This is the accuracy of the IMO.
              Therefore the formula is lock tolerance = 1.5 * fllMult * 0.012626 = 0.018939 * fllMult */
            config.lockTolerance = (uint16_t)CY_SYSLIB_DIV_ROUNDUP(config.fllMult * 18939UL, 1000000UL);

            {
                /* constants indexed by ccoRange */
                const uint32_t trimSteps[] = {110340UL, 110200UL, 110000UL, 110000UL, 117062UL}; /* Scaled by 10^8 */
                const uint32_t margin[] = {436UL, 581UL, 772UL, 1030UL, 1320UL}; /* Scaled by 10^5 */
        /* 7. Compute the CCO igain and pgain */
                {
                    /* intermediate parameters */
                    uint32_t kcco = (trimSteps[config.ccoRange] * margin[config.ccoRange]);
                    uint32_t ki_p = (uint32_t)CY_SYSLIB_DIV_ROUND(850ULL * CY_SYSCLK_FLL_INT_COEF * inputFreq, (uint64_t)kcco * (uint64_t)config.refDiv);

                    /* find the largest IGAIN value that is less than or equal to ki_p */
                    uint32_t locigain = CY_SYSCLK_FLL_GAIN_VAL;
                    uint32_t locpgain = CY_SYSCLK_FLL_GAIN_VAL;

                    /* find the largest IGAIN value that is less than or equal to ki_p */
                    for(config.igain = CY_SYSCLK_FLL_GAIN_IDX; config.igain != 0UL; config.igain--)
                    {
                       if(locigain <= ki_p)
                       {
                          break;
                       }
                       locigain >>= 1U;
                    }
                    /* decrement igain if the WCO is the FLL source */
                    if (wcoSource && (config.igain > 0U))
                    {
                        config.igain--;
                        locigain >>= 1U;
                    }

                    /* then find the largest PGAIN value that is less than or equal to ki_p - igain */
                    for(config.pgain = CY_SYSCLK_FLL_GAIN_IDX; config.pgain != 0UL; config.pgain--)
                    {
                      if(locpgain <= (ki_p - locigain))
                      {
                          break;
                      }
                      locpgain >>= 1U;
                    }

                    /* decrement pgain if the WCO is the FLL source */
                    if (wcoSource && (config.pgain > 0U))
                    {
                        config.pgain--;
                    }
                }

        /* 8. Compute the CCO_FREQ bits in CLK_FLL_CONFIG4 register */
                {
                    uint64_t cmp = CY_SYSLIB_DIV_ROUND(((TRIM_STEPS_SCALE / MARGIN_SCALE) * (uint64_t)ccoFreq), (uint64_t)margin[config.ccoRange]);
                    uint64_t mlt = TRIM_STEPS_SCALE + (uint64_t)trimSteps[config.ccoRange];
                    uint64_t res = mlt;

                    config.cco_Freq = 0U;

                    while(res < cmp)
                    {
                        res *= mlt;
                        res /= TRIM_STEPS_SCALE;
                        config.cco_Freq++;
                    }
                }
            }

        /* 9. Compute the settling count, using a 1 usec settling time. Use a constant if the WCO is the FLL source */
            {
                uint64_t fref = CY_SYSLIB_DIV_ROUND(6000ULL * (uint64_t)inputFreq, (uint64_t)config.refDiv);
                uint32_t divval = CY_SYSLIB_DIV_ROUNDUP(inputFreq, 1000000UL);
                uint32_t altval = (uint32_t)CY_SYSLIB_DIV_ROUNDUP((uint64_t)divval * fref, 6000000ULL) + 1UL;

                config.settlingCount = wcoSource ? 200U : (uint16_t)
                          ((outputFreq < fref) ? divval :
                            ((divval > altval) ? divval : altval));
            }
        /* Configure FLL based on calculated values */
            retVal = Cy_SysClk_FllManualConfigure(&config);
        }
        else /* if not, bypass output mode */
        {
            CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG3, SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, CY_SYSCLK_FLLPLL_OUTPUT_INPUT);
        }
    }

    return (retVal);
#else
    return CY_SYSCLK_INVALID_STATE;
#endif
}


cy_en_sysclk_status_t Cy_SysClk_FllManualConfigure(const cy_stc_fll_manual_config_t *config)
{
#if (CY_SRSS_FLL_PRESENT)
    cy_en_sysclk_status_t retVal = CY_SYSCLK_INVALID_STATE;

    /* Check for errors */
    CY_ASSERT_L1(config != NULL);

    if (!Cy_SysClk_FllIsEnabled()) /* If disabled */
    {
        /* update CLK_FLL_CONFIG register with 2 parameters; FLL_ENABLE is already 0 */
        /* asserts just check for bitfield overflow */
        CY_ASSERT_L1(config->fllMult <= (SRSS_CLK_FLL_CONFIG_FLL_MULT_Msk >> SRSS_CLK_FLL_CONFIG_FLL_MULT_Pos));

        SRSS_CLK_FLL_CONFIG = _VAL2FLD(SRSS_CLK_FLL_CONFIG_FLL_MULT, config->fllMult) |
                             _BOOL2FLD(SRSS_CLK_FLL_CONFIG_FLL_OUTPUT_DIV, config->enableOutputDiv);

        /* update CLK_FLL_CONFIG2 register with 2 parameters */
        /* asserts just check for bitfield overflow */
        CY_ASSERT_L1(config->refDiv <= (SRSS_CLK_FLL_CONFIG2_FLL_REF_DIV_Msk >> SRSS_CLK_FLL_CONFIG2_FLL_REF_DIV_Pos));
        CY_ASSERT_L1(config->lockTolerance <= (SRSS_CLK_FLL_CONFIG2_LOCK_TOL_Msk >> SRSS_CLK_FLL_CONFIG2_LOCK_TOL_Pos));

        SRSS_CLK_FLL_CONFIG2 = _VAL2FLD(SRSS_CLK_FLL_CONFIG2_FLL_REF_DIV, config->refDiv) |
                               _VAL2FLD(SRSS_CLK_FLL_CONFIG2_LOCK_TOL, config->lockTolerance);

        /* update CLK_FLL_CONFIG3 register with 4 parameters */
        /* asserts just check for bitfield overflow */
        CY_ASSERT_L1(config->igain <= (SRSS_CLK_FLL_CONFIG3_FLL_LF_IGAIN_Msk >> SRSS_CLK_FLL_CONFIG3_FLL_LF_IGAIN_Pos));
        CY_ASSERT_L1(config->pgain <= (SRSS_CLK_FLL_CONFIG3_FLL_LF_PGAIN_Msk >> SRSS_CLK_FLL_CONFIG3_FLL_LF_PGAIN_Pos));
        CY_ASSERT_L1(config->settlingCount <= (SRSS_CLK_FLL_CONFIG3_SETTLING_COUNT_Msk >> SRSS_CLK_FLL_CONFIG3_SETTLING_COUNT_Pos));

        SRSS_CLK_FLL_CONFIG3 = _VAL2FLD(SRSS_CLK_FLL_CONFIG3_FLL_LF_IGAIN, config->igain) |
                               _VAL2FLD(SRSS_CLK_FLL_CONFIG3_FLL_LF_PGAIN, config->pgain) |
                               _VAL2FLD(SRSS_CLK_FLL_CONFIG3_SETTLING_COUNT, config->settlingCount) |
                               _VAL2FLD(SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, config->outputMode);

        /* update CLK_FLL_CONFIG4 register with 1 parameter; preserve other bits */
        /* asserts just check for bitfield overflow */
        CY_ASSERT_L1(CY_SYSCLK_FLL_IS_CCO_RANGE_VALID(config->ccoRange));
        CY_ASSERT_L1(config->cco_Freq <= (SRSS_CLK_FLL_CONFIG4_CCO_FREQ_Msk >> SRSS_CLK_FLL_CONFIG4_CCO_FREQ_Pos));

        CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG4, SRSS_CLK_FLL_CONFIG4_CCO_RANGE, (uint32_t)(config->ccoRange));
        CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG4, SRSS_CLK_FLL_CONFIG4_CCO_FREQ, (uint32_t)(config->cco_Freq));

        retVal = CY_SYSCLK_SUCCESS;
    }

    return (retVal);
#else
    return CY_SYSCLK_INVALID_STATE;
#endif
}


void Cy_SysClk_FllGetConfiguration(cy_stc_fll_manual_config_t *config)
{
#if (CY_SRSS_FLL_PRESENT)
    CY_ASSERT_L1(config != NULL);
    /* read 2 parameters from CLK_FLL_CONFIG register */
    uint32_t tempReg = SRSS_CLK_FLL_CONFIG;
    config->fllMult         = _FLD2VAL(SRSS_CLK_FLL_CONFIG_FLL_MULT, tempReg);
    config->enableOutputDiv = _FLD2BOOL(SRSS_CLK_FLL_CONFIG_FLL_OUTPUT_DIV, tempReg);
    /* read 2 parameters from CLK_FLL_CONFIG2 register */
    tempReg = SRSS_CLK_FLL_CONFIG2;
    config->refDiv          = (uint16_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG2_FLL_REF_DIV, tempReg);
    config->lockTolerance   = (uint16_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG2_LOCK_TOL, tempReg);
    /* read 4 parameters from CLK_FLL_CONFIG3 register */
    tempReg = SRSS_CLK_FLL_CONFIG3;
    config->igain           = (uint8_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG3_FLL_LF_IGAIN, tempReg);
    config->pgain           = (uint8_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG3_FLL_LF_PGAIN, tempReg);
    config->settlingCount   = (uint16_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG3_SETTLING_COUNT, tempReg);
    config->outputMode      = (cy_en_fll_pll_output_mode_t)((uint32_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, tempReg));
    /* read 2 parameters from CLK_FLL_CONFIG4 register */
    tempReg = SRSS_CLK_FLL_CONFIG4;
    config->ccoRange        = (cy_en_fll_cco_ranges_t)((uint32_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG4_CCO_RANGE, tempReg));
    config->cco_Freq        = (uint16_t)_FLD2VAL(SRSS_CLK_FLL_CONFIG4_CCO_FREQ, tempReg);
#endif
}


cy_en_sysclk_status_t Cy_SysClk_FllEnable(uint32_t timeoutus)
{
#if (CY_SRSS_FLL_PRESENT)
    bool zeroTimeout = (0UL == timeoutus);

    /* first set the CCO enable bit */
    SRSS_CLK_FLL_CONFIG4 |= SRSS_CLK_FLL_CONFIG4_CCO_ENABLE_Msk;

    /* Wait until CCO is ready */
    for (; (!_FLD2BOOL(SRSS_CLK_FLL_STATUS_CCO_READY, SRSS_CLK_FLL_STATUS)) && /* if cco_ready == 0 */
           (0UL != timeoutus);
         timeoutus--)
    {
        Cy_SysLib_DelayUs(1U);
    }

    /* Set the FLL bypass mode to FLL_REF */
    CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG3, SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, CY_SYSCLK_FLLPLL_OUTPUT_INPUT);

    /* Set the FLL enable bit, if CCO is ready */
    if (zeroTimeout || (0UL != timeoutus))
    {
        SRSS_CLK_FLL_CONFIG |= SRSS_CLK_FLL_CONFIG_FLL_ENABLE_Msk;
    }

    /* now do the timeout wait for FLL_STATUS, bit LOCKED */
    for (; (!Cy_SysClk_FllLocked()) && /* if locked == 0 */
           (0UL != timeoutus);
         timeoutus--)
    {
        Cy_SysLib_DelayUs(1U);
    }

    if (zeroTimeout || (0UL != timeoutus))
    {
        /* Set the FLL bypass mode to FLL_OUT (ignoring lock indicator) */
        CY_REG32_CLR_SET(SRSS_CLK_FLL_CONFIG3, SRSS_CLK_FLL_CONFIG3_BYPASS_SEL, CY_SYSCLK_FLLPLL_OUTPUT_OUTPUT);
    }
    else
    {
        /* If lock doesn't occur, FLL is stopped */
        (void)Cy_SysClk_FllDisable();
    }

    return ((zeroTimeout || (0UL != timeoutus)) ? CY_SYSCLK_SUCCESS : CY_SYSCLK_TIMEOUT);
#else
    return CY_SYSCLK_INVALID_STATE;
#endif
}


/* ========================================================================== */
/* ===========================    PLL SECTION    ============================ */
/* ========================================================================== */

/* PLL OUTPUT_DIV bitfield allowable range */
#define CY_SYSCLK_PLL_MIN_OUTPUT_DIV   (2UL)
#define CY_SYSCLK_PLL_MAX_OUTPUT_DIV   (16UL)

/* PLL REFERENCE_DIV bitfield allowable range */
#define CY_SYSCLK_PLL_MIN_REF_DIV      (1UL)
#define CY_SYSCLK_PLL_MAX_REF_DIV      (18UL)

/* PLL FEEDBACK_DIV bitfield allowable ranges, LF and normal modes */
#define CY_SYSCLK_PLL_MIN_FB_DIV_LF    (19UL)
#define CY_SYSCLK_PLL_MAX_FB_DIV_LF    (56UL)
#define CY_SYSCLK_PLL_MIN_FB_DIV_NORM  (22UL)
#define CY_SYSCLK_PLL_MAX_FB_DIV_NORM  (112UL)

/* PLL FEEDBACK_DIV bitfield allowable range selection */
#define CY_SYSCLK_PLL_MIN_FB_DIV       ((config->lfMode) ? CY_SYSCLK_PLL_MIN_FB_DIV_LF : CY_SYSCLK_PLL_MIN_FB_DIV_NORM)
#define CY_SYSCLK_PLL_MAX_FB_DIV       ((config->lfMode) ? CY_SYSCLK_PLL_MAX_FB_DIV_LF : CY_SYSCLK_PLL_MAX_FB_DIV_NORM)

/* PLL Fvco range allowable ranges, LF and normal modes */
#define CY_SYSCLK_PLL_MIN_FVCO_LF      (170000000UL)
#define CY_SYSCLK_PLL_MAX_FVCO_LF      (200000000UL)
#define CY_SYSCLK_PLL_MIN_FVCO_NORM    (200000000UL)
#define CY_SYSCLK_PLL_MAX_FVCO_NORM    (400000000UL)
/* PLL Fvco range selection */
#define CY_SYSCLK_PLL_MIN_FVCO         ((config->lfMode) ? CY_SYSCLK_PLL_MIN_FVCO_LF : CY_SYSCLK_PLL_MIN_FVCO_NORM)
#define CY_SYSCLK_PLL_MAX_FVCO         ((config->lfMode) ? CY_SYSCLK_PLL_MAX_FVCO_LF : CY_SYSCLK_PLL_MAX_FVCO_NORM)

/* PLL input and output frequency limits */
#define CY_SYSCLK_PLL_MIN_IN_FREQ  (4000000UL)
#define CY_SYSCLK_PLL_MAX_IN_FREQ  (64000000UL)
#define CY_SYSCLK_PLL_MIN_OUT_FREQ (CY_SYSCLK_PLL_MIN_FVCO / CY_SYSCLK_PLL_MAX_OUTPUT_DIV)
#define CY_SYSCLK_PLL_MAX_OUT_FREQ (150000000UL)


bool Cy_SysClk_PllIsEnabled(uint32_t clkPath)
{
#if (CY_IP_MXS28SRSS)
    CY_ASSERT_L1(clkPath < CY_SRSS_NUM_PLL);
    return (_FLD2BOOL(CLK_LP_PLL_PLL28LP_STRUCT_CONFIG_PLL_ENABLE, SRSS_CLK_LP_PLL_CONFIG(clkPath)));
#else
    return false;
#endif
}


bool Cy_SysClk_PllLocked(uint32_t clkPath)
{
    return false;
}


bool Cy_SysClk_PllLostLock(uint32_t clkPath)
{
    return false;
}


cy_en_sysclk_status_t Cy_SysClk_PllDisable(uint32_t clkPath)
{
    return CY_SYSCLK_INVALID_STATE;
}


cy_en_sysclk_status_t Cy_SysClk_PllConfigure(uint32_t clkPath, const cy_stc_pll_config_t *config)
{
    return CY_SYSCLK_INVALID_STATE;
}


cy_en_sysclk_status_t Cy_SysClk_PllManualConfigure(uint32_t clkPath, const cy_stc_pll_manual_config_t *config)
{
    return CY_SYSCLK_INVALID_STATE;
}


cy_en_sysclk_status_t Cy_SysClk_PllGetConfiguration(uint32_t clkPath, cy_stc_pll_manual_config_t *config)
{
    return CY_SYSCLK_INVALID_STATE;
}


cy_en_sysclk_status_t Cy_SysClk_PllEnable(uint32_t clkPath, uint32_t timeoutus)
{
#if (CY_IP_MXS28SRSS)
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;
    bool nonZeroTimeout = (timeoutus != 0UL);

    if (clkPath < CY_SRSS_NUM_PLL)
    {
        /* Set the PLL BYPASS_SEL bits to PLL_OUT(0x3)*/
        SRSS_CLK_LP_PLL_CONFIG(clkPath) |= CLK_LP_PLL_PLL28LP_STRUCT_CONFIG_BYPASS_SEL_Msk; 

        /* Set the PLL enable bit */
        SRSS_CLK_LP_PLL_CONFIG(clkPath) |= CLK_LP_PLL_PLL28LP_STRUCT_CONFIG_PLL_ENABLE_Msk; 

        /* Set the OUTPU_DIV to 1, Need to be relooked */
        SRSS_CLK_LP_PLL_CONFIG4(clkPath) = _VAL2FLD(CLK_LP_PLL_PLL28LP_STRUCT_CONFIG4_OUTPUT_DIV, 1);

    /* now do the timeout wait for PLL_STATUS, bit LOCKED */
    for (; (0UL == (CLK_LP_PLL_PLL28LP_STRUCT_STATUS_LOCKED_Msk & SRSS_CLK_LP_PLL_CONFIG2(clkPath))) &&
           (0UL != timeoutus);
         timeoutus--)
    {
        Cy_SysLib_DelayUs(1U);
    }
    retVal = ((nonZeroTimeout && (timeoutus == 0UL)) ? CY_SYSCLK_TIMEOUT : CY_SYSCLK_SUCCESS);
    }
    return (retVal);
#else
    return CY_SYSCLK_INVALID_STATE;
#endif

}

/* ========================================================================== */
/* ===========================    IHO SECTION    ============================ */
/* ========================================================================== */

#if defined (CY_IP_MXS40SSRSS)

bool Cy_SysClk_IhoIsEnabled(void)
{
#if (CY_SRSS_IHO_PRESENT)
    return (_FLD2BOOL(SRSS_CLK_IHO_CONFIG_ENABLE, SRSS_CLK_IHO_CONFIG));
#else
    return false;
#endif
}

void Cy_SysClk_IhoDisable(void)
{
#if (CY_SRSS_IHO_PRESENT)
    SRSS_CLK_IHO_CONFIG = 0UL;
#endif
}

void Cy_SysClk_IhoEnable(void)
{
#if (CY_SRSS_IHO_PRESENT)
    SRSS_CLK_IHO_CONFIG |= SRSS_CLK_IHO_CONFIG_ENABLE_Msk;
#endif

}
#endif
/* ========================================================================== */
/* ====================    Clock Measurement section    ===================== */
/* ========================================================================== */
/* Slow control register default value */
#define TST_DDFT_SLOW_CTL_DEFAULT_VAL      (0x00001F1FUL)

/* Fast control register */
#define TST_DDFT_FAST_CTL_REG              (*(volatile uint32_t *) 0x40260104U)

/* Slow control register default value */
#define TST_DDFT_FAST_CTL_DEFAULT_VAL      (0x00003D3DUL)

/* Define for select signal outputs in slow clock */
#define SRSS_CLK_OUTPUT_SLOW_MASK  ((uint32_t) SRSS_CLK_OUTPUT_SLOW_SLOW_SEL0_Msk | \
                                               SRSS_CLK_OUTPUT_SLOW_SLOW_SEL1_Msk)

/* Define for select signal outputs in fast clock */
#define SRSS_CLK_OUTPUT_FAST_MASK  ((uint32_t) SRSS_CLK_OUTPUT_FAST_FAST_SEL0_Msk  | \
                                               SRSS_CLK_OUTPUT_FAST_FAST_SEL1_Msk  | \
                                               SRSS_CLK_OUTPUT_FAST_PATH_SEL0_Msk  | \
                                               SRSS_CLK_OUTPUT_FAST_PATH_SEL1_Msk  | \
                                               SRSS_CLK_OUTPUT_FAST_HFCLK_SEL0_Msk | \
                                               SRSS_CLK_OUTPUT_FAST_HFCLK_SEL1_Msk)

/* Cy_SysClk_StartClkMeasurementCounters() input parameter saved for use later in other functions */
static uint32_t clk1Count1;

/* These variables act as locks to prevent collisions between clock measurement and entry into
   Deep Sleep mode. See Cy_SysClk_DeepSleep(). */
static bool preventCounting = false;


bool Cy_SysClk_ClkMeasurementCountersDone(void)
{
    return (_FLD2BOOL(SRSS_CLK_CAL_CNT1_CAL_COUNTER_DONE, SRSS_CLK_CAL_CNT1));
}

cy_en_sysclk_status_t Cy_SysClk_StartClkMeasurementCounters(cy_en_meas_clks_t clock1, uint32_t count1, cy_en_meas_clks_t clock2)
{
    cy_en_sysclk_status_t retVal = CY_SYSCLK_BAD_PARAM;

    uint32_t clkOutputSlowVal = 0UL;
    uint32_t clkOutputFastVal = 0UL;

    uint32_t clkOutputSlowMask = 0UL;
    uint32_t clkOutputFastMask = 0UL;

    /* Prepare values for measurement control registers */

    /* Connect the indicated clocks to the respective counters:

       if clock1 is a slow clock,
         select it in SRSS_CLK_OUTPUT_SLOW.SLOW_SEL0, and SRSS_CLK_OUTPUT_FAST.FAST_SEL0 = SLOW_SEL0
       else if clock1 is a fast clock,
         select it in SRSS_CLK_OUTPUT_FAST.FAST_SEL0,
       else error, do nothing and return.

       if clock2 is a slow clock,
         select it in SRSS_CLK_OUTPUT_SLOW.SLOW_SEL1, and SRSS_CLK_OUTPUT_FAST.FAST_SEL1 = SLOW_SEL1
       else if clock2 is a fast clock,
         select it in SRSS_CLK_OUTPUT_FAST.FAST_SEL1,
       else error, do nothing and return.
    */
    if ((clock1 < CY_SYSCLK_MEAS_CLK_LAST_CLK) && (clock2 < CY_SYSCLK_MEAS_CLK_LAST_CLK) &&
        (count1 <= (SRSS_CLK_CAL_CNT1_CAL_COUNTER1_Msk >> SRSS_CLK_CAL_CNT1_CAL_COUNTER1_Pos)))
    {
        if (clock1 < CY_SYSCLK_MEAS_CLK_FAST_CLKS)
        { /* slow clock */
            clkOutputSlowVal |= _VAL2FLD(SRSS_CLK_OUTPUT_SLOW_SLOW_SEL0, (uint32_t)clock1);
            clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_FAST_SEL0, 7UL/*slow_sel0 output*/);

            clkOutputSlowMask |= SRSS_CLK_OUTPUT_SLOW_SLOW_SEL0_Msk;
            clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_FAST_SEL0_Msk;
        }
        else
        { /* fast clock */
            if (clock1 < CY_SYSCLK_MEAS_CLK_PATH_CLKS)
            { /* ECO, EXT, ALTHF */
                clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_FAST_SEL0, (uint32_t)clock1);
                clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_FAST_SEL0_Msk;
            }
            else
            { /* PATH or CLKHF */
                clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_FAST_SEL0, (((uint32_t)clock1 >> 8) & 0xFUL) /*use enum bits [11:8]*/);
                clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_FAST_SEL0_Msk;

                if (clock1 < CY_SYSCLK_MEAS_CLK_CLKHFS)
                { /* PATH select */
                    clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_PATH_SEL0, ((uint32_t)clock1 & 0xFUL) /*use enum bits [3:0]*/);
                    clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_PATH_SEL0_Msk;
                }
                else
                { /* CLKHF select */
                    clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_HFCLK_SEL0, ((uint32_t)clock1 & 0xFUL) /*use enum bits [3:0]*/);
                    clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_HFCLK_SEL0_Msk;
                }
            }
        } /* clock1 fast clock */

        if (clock2 < CY_SYSCLK_MEAS_CLK_FAST_CLKS)
        { /* slow clock */
            clkOutputSlowVal |= _VAL2FLD(SRSS_CLK_OUTPUT_SLOW_SLOW_SEL1, (uint32_t)clock2);
            clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_FAST_SEL1, 7UL/*slow_sel1 output*/);

            clkOutputSlowMask |= SRSS_CLK_OUTPUT_SLOW_SLOW_SEL1_Msk;
            clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_FAST_SEL1_Msk;
        }
        else
        { /* fast clock */
            if (clock2 < CY_SYSCLK_MEAS_CLK_PATH_CLKS)
            { /* ECO, EXT, ALTHF */
                clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_FAST_SEL1, (uint32_t)clock2);
                clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_FAST_SEL1_Msk;
            }
            else
            { /* PATH or CLKHF */
                clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_FAST_SEL1, (((uint32_t)clock2 >> 8) & 0xFUL) /*use enum bits [11:8]*/);
                clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_FAST_SEL1_Msk;

                if (clock2 < CY_SYSCLK_MEAS_CLK_CLKHFS)
                { /* PATH select */
                    clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_PATH_SEL1, ((uint32_t)clock2 & 0xFUL) /*use enum bits [3:0]*/);
                    clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_PATH_SEL1_Msk;
                }
                else
                { /* CLKHF select */
                    clkOutputFastVal |= _VAL2FLD(SRSS_CLK_OUTPUT_FAST_HFCLK_SEL1, ((uint32_t)clock2 & 0xFUL) /*use enum bits [3:0]*/);
                    clkOutputFastMask |= SRSS_CLK_OUTPUT_FAST_HFCLK_SEL1_Msk;
                }
            }
        } /* clock2 fast clock */

        if ((!preventCounting) /* don't start a measurement if about to enter Deep Sleep mode */  ||
            (_FLD2VAL(SRSS_CLK_CAL_CNT1_CAL_COUNTER_DONE, SRSS_CLK_CAL_CNT1) != 0UL/*1 = done */))
        {
            /* Set default values for counters measurement control registers */
            SRSS_TST_DDFT_SLOW_CTL_REG = TST_DDFT_SLOW_CTL_DEFAULT_VAL;
            SRSS_TST_DDFT_FAST_CTL_REG = TST_DDFT_FAST_CTL_DEFAULT_VAL;

            SRSS_CLK_OUTPUT_SLOW = ((SRSS_CLK_OUTPUT_SLOW & ((uint32_t) ~clkOutputSlowMask)) | clkOutputSlowVal);
            SRSS_CLK_OUTPUT_FAST = ((SRSS_CLK_OUTPUT_FAST & ((uint32_t) ~clkOutputFastMask)) | clkOutputFastVal);

            /* Save this input parameter for use later, in other functions.
               No error checking is done on this parameter */
            clk1Count1 = count1;

            /* Counting starts when counter1 is written with a nonzero value */
            SRSS_CLK_CAL_CNT1 = clk1Count1;

            retVal = CY_SYSCLK_SUCCESS;
        }
    }

    return (retVal);
}


uint32_t Cy_SysClk_ClkMeasurementCountersGetFreq(bool measuredClock, uint32_t refClkFreq)
{
    uint32_t retVal = 0UL;
    bool isMeasurementValid = false;

    /* Check whether the device was in the Deep Sleep mode or the flash partially blocked while the
    *  operation was done
    */
    if(SRSS_TST_DDFT_SLOW_CTL_REG == TST_DDFT_SLOW_CTL_DEFAULT_VAL)
    {
       if(SRSS_TST_DDFT_FAST_CTL_REG == TST_DDFT_FAST_CTL_DEFAULT_VAL)
       {
           isMeasurementValid = true;
       }
    }

    retVal = _FLD2VAL(SRSS_CLK_CAL_CNT2_CAL_COUNTER2, SRSS_CLK_CAL_CNT2);

    if (isMeasurementValid && (0UL != retVal))
    {
        if (!measuredClock)
        {   /* clock1 is the measured clock */
            retVal = (uint32_t)CY_SYSLIB_DIV_ROUND((uint64_t)clk1Count1 * (uint64_t)refClkFreq, (uint64_t)retVal);
        }
        else
        {   /* clock2 is the measured clock */
            retVal = (uint32_t)CY_SYSLIB_DIV_ROUND((uint64_t)retVal * (uint64_t)refClkFreq, (uint64_t)clk1Count1);
        }
    }
    else
    {
        /* Return zero value to indicate invalid measurement */
        retVal = 0UL;
    }

    return (retVal);
}


/* ========================================================================== */
/* ==========================    TRIM SECTION    ============================ */
/* ========================================================================== */


/** \cond INTERNAL */
/* target frequency */
#define CY_SYSCLK_ILO_TARGET_FREQ  (32768UL)
/* Nominal trim step size is 1.5% of "the frequency". Using the target frequency */
#define CY_SYSCLK_ILO_TRIM_STEP    (CY_SYSLIB_DIV_ROUND(CY_SYSCLK_ILO_TARGET_FREQ * 15UL, 1000UL))//tbd
/** \endcond */

int32_t Cy_SysClk_IloTrim(uint32_t iloFreq)
{
    return 0;
}

/** \cond INTERNAL */
#define CY_SYSCLK_PILO_TARGET_FREQ  (32768UL)
/* nominal trim step size */
#define CY_SYSCLK_PILO_TRIM_STEP        (5UL)//tbd
/** \endcond */

int32_t Cy_SysClk_PiloTrim(uint32_t piloFreq)
{
    return 0;
}

/* ========================================================================== */
/* ======================    POWER MANAGEMENT SECTION    ==================== */
/* ========================================================================== */


/** \cond INTERNAL */
/* Timeout count for use in function Cy_SysClk_DeepSleepCallback() is sufficiently large for ~1 second */
#define TIMEOUT (1000000UL)
/** \endcond */

cy_en_syspm_status_t Cy_SysClk_DeepSleepCallback(cy_stc_syspm_callback_params_t * callbackParams, cy_en_syspm_callback_mode_t mode)
{
    cy_en_syspm_status_t retVal = CY_SYSPM_FAIL;
    
    return (retVal);
}


/* ========================================================================== */
/* =========================    clkHf[n] SECTION    ========================= */
/* ========================================================================== */

/** \cond INTERNAL */
uint32_t altHfFreq = 0UL; /* Internal storage for BLE ECO frequency user setting */
/** \endcond */


uint32_t Cy_SysClk_ClkHfGetFrequency(uint32_t clkHf)
{
    /* variables holding intermediate clock frequencies, dividers and FLL/PLL settings */
    uint32_t pDiv = 1UL << (uint32_t)Cy_SysClk_ClkHfGetDivider(clkHf); /* root prescaler (1/2/4/8) */
    uint32_t path = (uint32_t) Cy_SysClk_ClkHfGetSource(clkHf); /* path input for root 0 (clkHf[0]) */
    uint32_t freq = Cy_SysClk_ClkPathGetFrequency(path);

    /* Divide the path input frequency down and return the result */
    return (CY_SYSLIB_DIV_ROUND(freq, pDiv));
}


/* ========================================================================== */
/* =====================    clk_peripherals SECTION    ====================== */
/* ========================================================================== */


uint32_t Cy_SysClk_PeriphGetFrequency(cy_en_divider_types_t dividerType, uint32_t dividerNum)
{
    uint32_t integer = 0UL;        /* Integer part of peripheral divider */
    uint32_t freq = Cy_SysClk_ClkPeriGetFrequency(); /* Get Peri frequency */

    CY_ASSERT_L1(((dividerType == CY_SYSCLK_DIV_8_BIT)    && (dividerNum < PERI_DIV_8_NR))    || \
                 ((dividerType == CY_SYSCLK_DIV_16_BIT)   && (dividerNum < PERI_DIV_16_NR))   || \
                 ((dividerType == CY_SYSCLK_DIV_16_5_BIT) && (dividerNum < PERI_DIV_16_5_NR)) || \
                 ((dividerType == CY_SYSCLK_DIV_24_5_BIT) && (dividerNum < PERI_DIV_24_5_NR)));

    /* get the divider value for clk_peri to the selected peripheral clock */
    switch(dividerType)
    {
        case CY_SYSCLK_DIV_8_BIT:
        case CY_SYSCLK_DIV_16_BIT:
            integer = 1UL + Cy_SysClk_PeriphGetDivider(dividerType, dividerNum);
            freq = CY_SYSLIB_DIV_ROUND(freq, integer);
            break;

        case CY_SYSCLK_DIV_16_5_BIT:
        case CY_SYSCLK_DIV_24_5_BIT:
            {
                uint32_t locFrac;
                uint32_t locDiv;
                uint64_t locFreq = freq * 32ULL;
                Cy_SysClk_PeriphGetFracDivider(dividerType, dividerNum, &integer, &locFrac);
                /* For fractional dividers, the divider is (int + 1) + frac/32 */
                locDiv = ((1UL + integer) * 32UL) + locFrac;
                freq = (uint32_t) CY_SYSLIB_DIV_ROUND(locFreq, (uint64_t)locDiv);
            }
            break;

        default:
            /* Unknown Divider */
            break;
    }

    return (freq);
}


#endif
/* [] END OF FILE */
