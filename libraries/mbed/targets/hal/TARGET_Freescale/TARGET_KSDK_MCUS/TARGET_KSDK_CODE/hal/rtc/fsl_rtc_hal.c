/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_rtc_hal.h"
#include "fsl_device_registers.h"

/*******************************************************************************
 * Code
 ******************************************************************************/


/*FUNCTION**********************************************************************
 *
 * Function Name : rtc_hal_init
 * Description   : initializes the RTC module.
 * This function will first configure the oscillator load in pF, set/clear the
 * 32kHz clock out to peripheral, enable/disable 32.768 kHz oscillator. After
 * setting 32.768 kHz oscillator bit, wait the oscillator startup time before
 * enabling the time counter to allow the 32.768 kHz clock time to stabilize.
 * If devices have the wakeup pin, this function will enable/disable the wakeup
 * pin. Then this function configures time prescaler, time seconds, time alarm,
 * the compensation interval in seconds from 1 to 256 if requested, and the
 * number of 32.768 kHz clock cycles in each second. This function also
 * enable/disable the RTC interrupts as requested.
 *
 *END**************************************************************************/
void rtc_hal_init(rtc_hal_init_config_t * configs)
{
    /* check for null pointer*/
    if(NULL == configs)
    {
        return;
    }

    /* Clear to the default value */
    HW_RTC_CR_WR(0x0);

    /* Use the following macro masks to set the flags which enable load config
     */
    switch(configs->enableOscillatorLoadConfg)
    {
    case 2:
       HW_RTC_CR_WR(BM_RTC_CR_SC2P);
       break;
    case 4:
       HW_RTC_CR_WR(BM_RTC_CR_SC4P);
       break;
    case 8:
       HW_RTC_CR_WR(BM_RTC_CR_SC8P);
       break;
    case 16:
       HW_RTC_CR_WR(BM_RTC_CR_SC16P);
       break;
    default:
       break;
    }

    BW_RTC_CR_CLKO((uint32_t)configs->disableClockOutToPeripheral);

    /* After enabling this, will wait the oscillator startup time before enabling
    * the time counter TSR[TSR] to allow the 32.768 kHz clock time to stabilize.
    */
    BW_RTC_CR_OSCE((uint32_t)configs->enable32kOscillator);

#if FSL_FEATURE_RTC_HAS_WAKEUP_PIN
    /*! For devices that have the wakeup pin this variable will indicate if it is
    *  to be enabled (set to 'true') or not (set to 'false'). \n
    *  See the device's user manual for details depending on each device's 
    *  specific wakeup pin feature implementation.
    */
    BW_RTC_CR_WPE((uint32_t)configs->enableWakeupPin);
#endif

    /* write prescaler before writing to counter register*/
    if(configs->prescalerAt)
    {
        BW_RTC_SR_TCE(0x00U); /* disable counter before configuring the prescaler*/
        HW_RTC_TPR_WR(configs->prescalerAt);
    }

    /* write time seconds should be written before writing to counter register*/
    if(configs->startSecondsCounterAt)
    {
        BW_RTC_SR_TCE(0x00U); /* disable time counter before configuring*/
        HW_RTC_TSR_WR(configs->startSecondsCounterAt);
    }

    if(configs->alarmCounterAt)
    {
        HW_RTC_TAR_WR(configs->alarmCounterAt);
    }

    /*  Configures the compensation interval in seconds from 1 to 256 to control
    *  how frequently the TCR should adjust the number of 32.768 kHz cycles in
    *  each second. The value written should be one less than the number of
    *  seconds (for example, write zero to configure for a compensation interval
    *  of one second). This register is double buffered and writes do not take
    *  affect until the end of the current compensation interval.
    */
    if(configs->compensationInterval)
    {
        BW_RTC_TCR_CIR(configs->compensationInterval);
    }

    /*  Configures the number of 32.768 kHz clock cycles in each second. This
    *  register is double buffered and writes do not take affect until the end
    *  of the current compensation interval.\n
    * \n
    *    80h Time prescaler register overflows every 32896 clock cycles.\n
    *    ... ...\n
    *    FFh Time prescaler register overflows every 32769 clock cycles.\n
    *    00h Time prescaler register overflows every 32768 clock cycles.\n
    *    01h Time prescaler register overflows every 32767 clock cycles.\n
    *    ... ...\n
    *    7Fh Time prescaler register overflows every 32641 clock cycles.\n
    */
    BW_RTC_TCR_TCR(configs->timeCompensation);

    /* Set/clear any of the following bitfields to enable/disable the
    * respective interrupts.\n
    *   TSIE:    Time Seconds Interrupt Enable \n
    *   TAIE:    Time Alarm Interrupt Enable \n
    *   TOIE:    Time Overflow Interrupt Enable \n
    *   TIIE:    Time Invalid Interrupt Enable \n
    * \n
    * For MCUs that have the Wakeup Pin only: \n
    *   WPON:    Wakeup Pin On (see the corresponding MCU's reference manual)\n
    * \n
    * For MCUs that have the Monotonic Counter only: \n
    *   MOIE:    Monotonic Overflow Interrupt Enable \n
    */
    rtc_hal_config_interrupts(&(configs->enableInterrupts));

#if FSL_FEATURE_RTC_HAS_MONOTONIC
    if(configs->monotonicCounterAt)
    {
        rtc_hal_set_monotonic_counter(configs->monotonicCounterAt);
    }
#endif
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

