/**************************************************************************//**
 * @file     timer.c
 * @version  V3.01
 * @brief    Timer PWM Controller(Timer PWM) driver source file
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Nuvoton Technology Corp. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of Nuvoton Technology Corp. nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/
#include "NuMicro.h"


/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup TIMER_PWM_Driver TIMER PWM Driver
  @{
*/

/** @addtogroup TIMER_PWM_EXPORTED_FUNCTIONS TIMER PWM Exported Functions
  @{
*/

/**
  * @brief      Configure TPWM Output Frequency and Duty Cycle
  *
  * @param[in]  timer           The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  * @param[in]  u32Frequency    Target generator frequency.
  * @param[in]  u32DutyCycle    Target generator duty cycle percentage. Valid range are between 0~100. 10 means 10%, 20 means 20%...
  *
  * @return     Nearest frequency clock in nano second
  *
  * @details    This API is used to configure TPWM output frequency and duty cycle in up count type and auto-reload operation mode.
  * @note       This API is only available if Timer PWM counter clock source is from TMRx_CLK.
  */
uint32_t TPWM_ConfigOutputFreqAndDuty(TIMER_T *timer, uint32_t u32Frequency, uint32_t u32DutyCycle)
{
    uint32_t u32PWMClockFreq, u32TargetFreq;
    uint32_t u32Prescaler = 0x100UL, u32Period, u32CMP;

    if((timer == TIMER0) || (timer == TIMER1))
    {
        u32PWMClockFreq = CLK_GetPCLK0Freq();
    }
    else
    {
        u32PWMClockFreq = CLK_GetPCLK1Freq();
    }

    /* Calculate u8PERIOD and u8PSC */
    for (u32Prescaler = 1; u32Prescaler <= 0x100UL; u32Prescaler++)
    {
        u32Period = (u32PWMClockFreq / u32Prescaler) / u32Frequency;

        /* If target u32Period is larger than 0x10000, need to use a larger prescaler */
        if (u32Period > 0x10000UL)
            continue;

        break;
    }

    /* Store return value here 'cos we're gonna change u32Prescaler & u32Period to the real value to fill into register */
    u32TargetFreq = (u32PWMClockFreq / u32Prescaler) / u32Period;

    /* Set PWM to auto-reload mode */
    timer->PWMCTL = (timer->PWMCTL & ~TIMER_PWMCTL_CNTMODE_Msk) | (TPWM_AUTO_RELOAD_MODE << TIMER_PWMCTL_CNTMODE_Pos);

    /* Convert to real register value */
    TPWM_SET_PRESCALER(timer, (u32Prescaler - 1UL));

    TPWM_SET_PERIOD(timer, (u32Period - 1UL));

    if (u32DutyCycle)
    {
        u32CMP = (u32DutyCycle * u32Period) / 100UL;
    }
    else
    {
        u32CMP = 0UL;
    }

    TPWM_SET_CMPDAT(timer, u32CMP);
    return (u32TargetFreq);
}

/**
  * @brief      Enable TPWM Counter
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to enable TPWM generator and start counter counting.
  */
void TPWM_EnableCounter(TIMER_T *timer)
{
    timer->PWMCTL |= TIMER_PWMCTL_CNTEN_Msk;
}

/**
  * @brief      Disable TPWM Generator
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details This function is used to disable TPWM counter immediately by clear CNTEN (TIMERx_PWMCTL[0]) bit.
  */
void TPWM_DisableCounter(TIMER_T *timer)
{
    timer->PWMCTL &= ~TIMER_PWMCTL_CNTEN_Msk;
}

/**
  * @brief      Enable TPWM Trigger ADC/DAC/PDMA
  *
  * @param[in]  timer           The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  * @param[in]  u32TargetMask   The mask of modules (DAC/EADC/PDMA) trigger by TPWM, the combination of:
  *                                 - \ref TIMER_PWMTRGCTL_PWMTRGDAC_Msk
  *                                 - \ref TIMER_PWMTRGCTL_PWMTRGEADC_Msk
  *                                 - \ref TIMER_PWMTRGCTL_PWMTRGPDMA_Msk
  * @param[in]  u32Condition    The condition to trigger ADC/DAC/PDMA. It could be one of following conditions:
  *                                 - \ref TPWM_TRIGGER_AT_PERIOD_POINT
  *                                 - \ref TPWM_TRIGGER_AT_COMPARE_POINT
  *                                 - \ref TPWM_TRIGGER_AT_PERIOD_OR_COMPARE_POINT
  * @return     None
  *
  * @details    This function is used to enable specified counter event to trigger ADC/DAC/PDMA.
  */
void TPWM_EnableTrigger(TIMER_T *timer, uint32_t u32TargetMask, uint32_t u32Condition)
{
    timer->PWMTRGCTL &= ~(TIMER_PWMTRGCTL_PWMTRGDAC_Msk | TIMER_PWMTRGCTL_PWMTRGEADC_Msk | TIMER_PWMTRGCTL_PWMTRGPDMA_Msk | TIMER_PWMTRGCTL_TRGSEL_Msk);
    timer->PWMTRGCTL |= (u32TargetMask) | (u32Condition);
}

/**
  * @brief      Disable Trigger ADC/DAC/PDMA
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  * @param[in]  u32TargetMask   The mask of modules (DAC/EADC/PDMA) trigger by TPWM, the combination of:
  *                                 - \ref TIMER_PWMTRGCTL_PWMTRGDAC_Msk
  *                                 - \ref TIMER_PWMTRGCTL_PWMTRGEADC_Msk
  *                                 - \ref TIMER_PWMTRGCTL_PWMTRGPDMA_Msk
  *
  * @return     None
  *
  * @details    This function is used to disable counter event to trigger ADC/DAC/PDMA.
  */
void TPWM_DisableTrigger(TIMER_T *timer, uint32_t u32TargetMask)
{
    timer->PWMTRGCTL &= ~(u32TargetMask);
}

/*@}*/ /* end of group TIMER_PWM_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group TIMER_PWM_Driver */

/*@}*/ /* end of group Standard_Driver */

/*** (C) COPYRIGHT 2018 Nuvoton Technology Corp. ***/
