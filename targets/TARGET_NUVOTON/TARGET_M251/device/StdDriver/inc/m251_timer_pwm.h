/**************************************************************************//**
 * @file     timer_pwm.h
 * @version  V1.01
 * @brief    M251 series Timer PWM Controller(Timer PWM) driver header file
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
#ifndef __TIMER_PWM_H__
#define __TIMER_PWM_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup TIMER_PWM_Driver TIMER PWM Driver
  @{
*/

/** @addtogroup TIMER_PWM_EXPORTED_CONSTANTS TIMER PWM Exported Constants
  @{
*/
/*---------------------------------------------------------------------------------------------------------*/
/*  TPWM Output Channel Constant Definitions                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define TPWM_CH0                                (BIT0)       /*!< Indicate PWMx_CH0 \hideinitializer */

/*---------------------------------------------------------------------------------------------------------*/
/*  TPWM Output Channel Selection Definitions                                                               */
/*---------------------------------------------------------------------------------------------------------*/
#define TPWM_TOUT_PIN_FROM_TX                   (BIT0)       /*!< Indicate PWMx output to Tx pins \hideinitializer */
#define TPWM_TOUT_PIN_FROM_TX_EXT               (BIT8)       /*!< Indicate PWMx output to Tx_ext pins \hideinitializer */

/*---------------------------------------------------------------------------------------------------------*/
/*  TPWM Counter Mode Constant Definitions                                                                 */
/*---------------------------------------------------------------------------------------------------------*/
#define TPWM_AUTO_RELOAD_MODE                   (0UL)       /*!< Auto-reload mode \hideinitializer */
#define TPWM_ONE_SHOT_MODE                      (1UL)       /*!< One-shot mode \hideinitializer */

/*---------------------------------------------------------------------------------------------------------*/
/*  TPWM Trigger ADC/DAC/PDMA Source Select Constant Definitions                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define TPWM_TRIGGER_AT_PERIOD_POINT            (0UL)       /*!< Timer PWM trigger EADC while counter period point event occurred \hideinitializer */
#define TPWM_TRIGGER_AT_COMPARE_POINT           (1UL)       /*!< Timer PWM trigger EADC while counter compare point event occurred \hideinitializer */
#define TPWM_TRIGGER_AT_PERIOD_OR_COMPARE_POINT (2UL)       /*!< Timer PWM trigger EADC while counter period or compare point event occurred \hideinitializer */

/*@}*/ /* end of group TIMER_PWM_EXPORTED_CONSTANTS */


/** @addtogroup TIMER_PWM_EXPORTED_FUNCTIONS TIMER PWM Exported Functions
  @{
*/

/**
  * @brief      Enable TPWM Counter Mode
  *
  * @param[in]  timer   The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This macro is used to enable specified Timer channel as PWM counter mode, then timer counter mode is invalid.
  * @note       All registers about time counter function will be cleared to 0 and timer clock source will be changed to PCLKx automatically after executing this macro.
  * \hideinitializer
  */
#define TPWM_ENABLE_PWM_MODE(timer)         ((timer)->CTL |= TIMER_CTL_FUNCSEL_Msk)

/**
  * @brief      Disable TPWM Counter Mode
  *
  * @param[in]  timer   The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This macro is used to disable specified Timer channel as PWM counter mode, then timer counter mode is available.
  * @note       All registers about TPWM counter function will be cleared to 0 after executing this macro.
  * \hideinitializer
  */
#define TPWM_DISABLE_PWM_MODE(timer)        ((timer)->CTL &= ~TIMER_CTL_FUNCSEL_Msk)

/**
  * @brief      Start TPWM Counter
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This macro is used to enable TPWM generator and start counter counting.
  * \hideinitializer
  */
#define TPWM_START_COUNTER(timer)               ((timer)->PWMCTL |= TIMER_PWMCTL_CNTEN_Msk)

/**
  * @brief      Stop TPWM Counter
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This macro is used to stop TPWM counter after current period is completed.
  * \hideinitializer
  */
#define TPWM_STOP_COUNTER(timer)                ((timer)->PWMPERIOD = 0x0UL)

/**
  * @brief      Set Counter Clock Prescaler
  *
  * @param[in]  timer           The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @param[in]  u32Prescaler    Clock prescaler of specified channel. Valid values are between 0x0~0xFF.
  *
  * @return     None
  *
  * @details    This macro is used to set the prescaler of specified TIMER PWM.
  * @note       If prescaler is 0, then there is no scaling in counter clock source.
  * \hideinitializer
  */
#define TPWM_SET_PRESCALER(timer, u32Prescaler) ((timer)->PWMCLKPSC = (u32Prescaler))

/**
  * @brief      Get Counter Clock Prescaler
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     Target prescaler setting, CLKPSC (TIMERx_PWMCLKPSC[7:0])
  *
  * @details    Get the prescaler setting, the target counter clock divider is (CLKPSC + 1).
  * \hideinitializer
  */
#define TPWM_GET_PRESCALER(timer)       ((timer)->PWMCLKPSC)

/**
  * @brief      Set Counter Period
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @param[in]  u32Period   Period of specified channel. Valid values are between 0x0~0xFFFF.
  *
  * @return     None
  *
  * @details    This macro is used to set the period of specified TIMER PWM.
  * \hideinitializer
  */
#define TPWM_SET_PERIOD(timer, u32Period)  ((timer)->PWMPERIOD = (u32Period))

/**
  * @brief      Get Counter Period
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     Target period setting, PERIOD (TIMERx_PWMPERIOD[15:0])
  *
  * @details    This macro is used to get the period of specified TIMER PWM.
  * \hideinitializer
  */
#define TPWM_GET_PERIOD(timer)          ((timer)->PWMPERIOD)

/**
  * @brief      Set Comparator Value
  *
  * @param[in]  timer   The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @param[in]  u32Cmp  Comparator of specified channel. Valid values are between 0x0~0xFFFF.
  *
  * @return     None
  *
  * @details    This macro is used to set the comparator value of specified TIMER PWM.
  * \hideinitializer
  */
#define TPWM_SET_CMPDAT(timer, u32Cmp)     ((timer)->PWMCMPDAT = (u32Cmp))

/**
  * @brief      Get Comparator Value
  *
  * @param[in]  timer   The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     Target comparator setting, CMPDAT (TIMERx_PWMCMPDAT[15:0])
  *
  * @details    This macro is used to get the comparator value of specified TIMER PWM.
  * \hideinitializer
  */
#define TPWM_GET_CMPDAT(timer)          ((timer)->PWMCMPDAT)

/**
  * @brief      Clear Counter
  *
  * @param[in]  timer   The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This macro is used to clear counter of specified TIMER PWM.
  * \hideinitializer
  */
#define TPWM_CLEAR_COUNTER(timer)       ((timer)->PWMCNTCLR = TIMER_PWMCNTCLR_CNTCLR_Msk)

/**
  * @brief      Enable Output Function
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @param[in]  u32Channel  Enable specified channel output function. Valid values are:
  *                             - \ref TPWM_CH0
  *
  * @return     None
  *
  * @details    This macro is used to enable output function of specified output pins.
  * @note       If the corresponding bit in u32ChMask parameter is 0, then output function will be disabled in this channel.
  * \hideinitializer
  */
#define TPWM_ENABLE_OUTPUT(timer, u32Channel)  ((timer)->PWMPOCTL = (u32Channel))

/**
  * @brief      Select Toggle-output Pin
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  * @param[in]  u32ToutSel  Toggle-output pin selection, valid values are:
  *                         - \ref TPWM_TOUT_PIN_FROM_TX
  *                         - \ref TPWM_TOUT_PIN_FROM_TX_EXT
  *
  * @return     None
  *
  * @details    This macro is used to select TPWM toggle-output pin is output on Tx or Tx_EXT pin.
  */
#define TPWM_SELECT_TOUT_PIN(timer, u32ToutSel)    ((timer)->PWMPOCTL = ((timer)->PWMPOCTL & ~TIMER_PWMPOCTL_POSEL_Msk) | (u32ToutSel))

/**
  * @brief      Set Output Inverse
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @param[in]  u32Channel  Set specified channel output is inversed or not. Valid values are:
  *                             - \ref TPWM_CH0
    *
  * @return     None
  *
  * @details    This macro is used to enable output inverse of specified output pins.
  * @note       If u32ChMask parameter is 0, then output inverse function will be disabled.
  * \hideinitializer
  */
#define TPWM_SET_OUTPUT_INVERSE(timer, u32Channel)  ((timer)->PWMPOLCTL = (u32Channel))

/**
  * @brief      Enable Period Event Interrupt
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This macro is used to enable the period event interrupt function.
  * \hideinitializer
  */
#define TPWM_ENABLE_PERIOD_INT(timer)       ((timer)->PWMINTEN0 |= TIMER_PWMINTEN0_PIEN_Msk)

/**
  * @brief      Disable Period Event Interrupt
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This macro is used to disable the period event interrupt function.
  * \hideinitializer
  */
#define TPWM_DISABLE_PERIOD_INT(timer)      ((timer)->PWMINTEN0 &= ~TIMER_PWMINTEN0_PIEN_Msk)

/**
  * @brief      Get Period Event Interrupt Flag
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @retval     0   Period event interrupt did not occur
  * @retval     1   Period event interrupt occurred
  *
  * @details    This macro indicates period event occurred or not.
  * \hideinitializer
  */
#define TPWM_GET_PERIOD_INT_FLAG(timer)     (((timer)->PWMINTSTS0 & TIMER_PWMINTSTS0_PIF_Msk)? 1 : 0)

/**
  * @brief      Clear Period Event Interrupt Flag
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This macro clears period event interrupt flag.
  * \hideinitializer
  */
#define TPWM_CLEAR_PERIOD_INT_FLAG(timer)   ((timer)->PWMINTSTS0 = TIMER_PWMINTSTS0_PIF_Msk)

/**
  * @brief      Enable Compare Up Event Interrupt
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This macro is used to enable the compare up event interrupt function.
  * \hideinitializer
  */
#define TPWM_ENABLE_CMP_UP_INT(timer)       ((timer)->PWMINTEN0 |= TIMER_PWMINTEN0_CMPUIEN_Msk)

/**
  * @brief      Disable Compare Up Event Interrupt
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This macro is used to disable the compare up event interrupt function.
  * \hideinitializer
  */
#define TPWM_DISABLE_CMP_UP_INT(timer)      ((timer)->PWMINTEN0 &= ~TIMER_PWMINTEN0_CMPUIEN_Msk)

/**
  * @brief      Get Compare Up Event Interrupt Flag
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @retval     0   Compare up event interrupt did not occur
  * @retval     1   Compare up event interrupt occurred
  *
  * @details    This macro indicates compare up event occurred or not.
  * \hideinitializer
  */
#define TPWM_GET_CMP_UP_INT_FLAG(timer)     (((timer)->PWMINTSTS0 & TIMER_PWMINTSTS0_CMPUIF_Msk)? 1 : 0)

/**
  * @brief      Clear Compare Up Event Interrupt Flag
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This macro clears compare up event interrupt flag.
  * \hideinitializer
  */
#define TPWM_CLEAR_CMP_UP_INT_FLAG(timer)   ((timer)->PWMINTSTS0 = TIMER_PWMINTSTS0_CMPUIF_Msk)

/**
  * @brief      Get Counter Reach Maximum Count Status
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @retval     0   Timer PWM counter never counts to maximum value
  * @retval     1   Timer PWM counter counts to maximum value, 0xFFFF
  *
  * @details    This macro indicates Timer PWM counter has count to 0xFFFF or not.
  * \hideinitializer
  */
#define TPWM_GET_REACH_MAX_CNT_STATUS(timer)    (((timer)->PWMSTATUS & TIMER_PWMSTATUS_CNTMAXF_Msk)? 1 : 0)

/**
  * @brief      Clear Counter Reach Maximum Count Status
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This macro clears reach maximum count status.
  * \hideinitializer
  */
#define TPWM_CLEAR_REACH_MAX_CNT_STATUS(timer)  ((timer)->PWMSTATUS = TIMER_PWMSTATUS_CNTMAXF_Msk)

/**
  * @brief      Get Trigger ADC Status
  *
  * @param[in]  timer   The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @retval     0       Trigger ADC start conversion is not occur
  * @retval     1       Specified counter event has trigger ADC start conversion
  *
  * @details    This macro is used to indicate TPWM counter event has triggered ADC start conversion.
  * \hideinitializer
  */
#define TPWM_GET_TRG_ADC_STATUS(timer)              (((timer)->PWMSTATUS & TIMER_PWMSTATUS_EADCTRGF_Msk)? 1 : 0)

/**
  * @brief      Clear Trigger ADC Status
  *
  * @param[in]  timer   The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This macro is used to clear TPWM counter event trigger ADC status.
  * \hideinitializer
  */
#define TPWM_CLEAR_TRG_ADC_STATUS(timer)            ((timer)->PWMSTATUS = TIMER_PWMSTATUS_EADCTRGF_Msk)

/**
  * @brief      Get Trigger DAC Status
  *
  * @param[in]  timer   The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @retval     0       Trigger DAC start conversion is not occur
  * @retval     1       Specified counter event has trigger DAC start conversion
  *
  * @details    This macro is used to indicate TPWM counter event has triggered DAC start conversion.
  * \hideinitializer
  */
#define TPWM_GET_TRG_DAC_STATUS(timer)              (((timer)->PWMSTATUS & TIMER_PWMSTATUS_DACTRGF_Msk)? 1 : 0)

/**
  * @brief      Clear Trigger DAC Status
  *
  * @param[in]  timer   The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This macro is used to clear TPWM counter event trigger DAC status.
  * \hideinitializer
  */
#define TPWM_CLEAR_TRG_DAC_STATUS(timer)            ((timer)->PWMSTATUS = TIMER_PWMSTATUS_DACTRGF_Msk)

/**
  * @brief      Get Trigger PDMA Status
  *
  * @param[in]  timer   The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @retval     0       Trigger PDMA start conversion is not occur
  * @retval     1       Specified counter event has trigger PDMA start conversion
  *
  * @details    This macro is used to indicate TPWM counter event has triggered PDMA start conversion.
  * \hideinitializer
  */
#define TPWM_GET_TRG_PDMA_STATUS(timer)              (((timer)->PWMSTATUS & TIMER_PWMSTATUS_PDMATRGF_Msk)? 1 : 0)

/**
  * @brief      Clear Trigger PDMA Status
  *
  * @param[in]  timer   The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This macro is used to clear TPWM counter event trigger PDMA status.
  * \hideinitializer
  */
#define TPWM_CLEAR_TRG_PDMA_STATUS(timer)            ((timer)->PWMSTATUS = TIMER_PWMSTATUS_PDMATRGF_Msk)

uint32_t TPWM_ConfigOutputFreqAndDuty(TIMER_T *timer, uint32_t u32Frequency, uint32_t u32DutyCycle);
void TPWM_EnableCounter(TIMER_T *timer);
void TPWM_DisableCounter(TIMER_T *timer);
void TPWM_EnableTrigger(TIMER_T *timer, uint32_t u32TargetMask, uint32_t u32Condition);
void TPWM_DisableTrigger(TIMER_T *timer, uint32_t u32TargetMask);

/**
  * @brief      Enable Trigger ADC
  *
  * @param[in]  timer           The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  * @param[in]  u32Condition    The condition to trigger ADC. It could be one of following conditions:
  *                                 - \ref TPWM_TRIGGER_AT_PERIOD_POINT
  *                                 - \ref TPWM_TRIGGER_AT_COMPARE_POINT
  *                                 - \ref TPWM_TRIGGER_AT_PERIOD_OR_COMPARE_POINT
  * @return     None
  *
  * @details    This function is used to enable specified counter compare event to trigger ADC.
  */
#define TPWM_EnableTriggerADC(timer, u32Condition)	TPWM_EnableTrigger(timer, TIMER_PWMTRGCTL_PWMTRGEADC_Msk, u32Condition)

/**
  * @brief      Disable Trigger ADC
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to disable counter compare event to trigger ADC.
  */
#define TPWM_DisableTriggerADC(timer)				TPWM_DisableTrigger(timer, TIMER_PWMTRGCTL_PWMTRGEADC_Msk)

/**
  * @brief      Enable Trigger DAC
  *
  * @param[in]  timer           The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  * @param[in]  u32Condition    The condition to trigger DAC. It could be one of following conditions:
  *                                 - \ref TPWM_TRIGGER_AT_PERIOD_POINT
  *                                 - \ref TPWM_TRIGGER_AT_COMPARE_POINT
  *                                 - \ref TPWM_TRIGGER_AT_PERIOD_OR_COMPARE_POINT
  * @return     None
  *
  * @details    This function is used to enable specified counter compare event to trigger DAC.
  */
#define TPWM_EnableTriggerDAC(timer, u32Condition)	TPWM_EnableTrigger(timer, TIMER_PWMTRGCTL_PWMTRGDAC_Msk, u32Condition)

/**
  * @brief      Disable Trigger DAC
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to disable counter compare event to trigger DAC.
  */
#define TPWM_DisableTriggerDAC(timer)				TPWM_DisableTrigger(timer, TIMER_PWMTRGCTL_PWMTRGDAC_Msk)

/**
  * @brief      Enable Trigger PDMA
  *
  * @param[in]  timer           The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  * @param[in]  u32Condition    The condition to trigger PDMA. It could be one of following conditions:
  *                                 - \ref TPWM_TRIGGER_AT_PERIOD_POINT
  *                                 - \ref TPWM_TRIGGER_AT_COMPARE_POINT
  *                                 - \ref TPWM_TRIGGER_AT_PERIOD_OR_COMPARE_POINT
  * @return     None
  *
  * @details    This function is used to enable specified counter compare event to trigger PDMA.
  */
#define TPWM_EnableTriggerPDMA(timer, u32Condition)	TPWM_EnableTrigger(timer, TIMER_PWMTRGCTL_PWMTRGPDMA_Msk, u32Condition)

/**
  * @brief      Disable Trigger PDMA
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to disable counter compare event to trigger PDMA.
  */
#define TPWM_DisableTriggerPDMA(timer)				TPWM_DisableTrigger(timer, TIMER_PWMTRGCTL_PWMTRGPDMA_Msk)

/* Declare these inline functions here to avoid MISRA C 2004 rule 8.1 error */
__STATIC_INLINE void TPWM_EnableWakeup(TIMER_T *timer);
__STATIC_INLINE void TPWM_DisableWakeup(TIMER_T *timer);
__STATIC_INLINE uint32_t TPWM_GetWakeupFlag(TIMER_T *timer);
__STATIC_INLINE void TPWM_ClearWakeupFlag(TIMER_T *timer);

/**
  * @brief      Enable TPWM Interrupt Wake-up Function
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to enable the TPWM interrupt Wake-up function and interrupt source could be period interrupt, \n
  *             or compare interrupt.
  * @note       To wake the system from Power-down mode, PCLKx source must be ether LXT or LIRC.
  * \hideinitializer
  */
__STATIC_INLINE void TPWM_EnableWakeup(TIMER_T *timer)
{
    timer->PWMCTL |= TIMER_PWMCTL_PWMINTWKEN_Msk;
}

/**
  * @brief      Disable TPWM Interrupt Wake-up Function
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function is used to disable the TPWM interrupt Wake-up function.
  * \hideinitializer
  */
__STATIC_INLINE void TPWM_DisableWakeup(TIMER_T *timer)
{
    timer->PWMCTL &= ~TIMER_PWMCTL_PWMINTWKEN_Msk;
}

/**
  * @brief      Get TPWM Wake-up Flag
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @retval     0   TPWM does not cause CPU wake-up
  * @retval     1   TPWM interrupt event cause CPU wake-up
  *
  * @details    This function indicates TPWM interrupt event has waked up system or not.
  * \hideinitializer
  */
__STATIC_INLINE uint32_t TPWM_GetWakeupFlag(TIMER_T *timer)
{
    return ((timer->PWMSTATUS & TIMER_PWMSTATUS_PWMINTWKF_Msk)? 1 : 0);
}

/**
  * @brief      Clear TPWM Wake-up Flag
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return     None
  *
  * @details    This function clears TPWM Wake-up flag.
  * \hideinitializer
  */
__STATIC_INLINE void TPWM_ClearWakeupFlag(TIMER_T *timer)
{
    timer->PWMSTATUS = TIMER_PWMSTATUS_PWMINTWKF_Msk;
}

/*@}*/ /* end of group TIMER_PWM_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group TIMER_PWM_Driver */

/*@}*/ /* end of group Standard_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __TIMER_PWM_H__ */

/*** (C) COPYRIGHT 2018 Nuvoton Technology Corp. ***/
