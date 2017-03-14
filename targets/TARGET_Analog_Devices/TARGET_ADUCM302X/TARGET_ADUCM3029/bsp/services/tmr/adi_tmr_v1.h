/*!
 *****************************************************************************
 @file:    adi_tmr_v1.h
 @brief:   Gptimer Device Definitions for ADuCxxx
 @version: $Revision$
 @date:    $Date$
 -----------------------------------------------------------------------------
Copyright (c) 2010-2016 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

/*! \addtogroup GP_Timer_Service  GP Timer Service
 *  @{
 */

#ifndef __ADI_TMR_CM302X_H__
#define __ADI_TMR_CM302X_H__

#include <services/int/adi_int.h>
#include <adi_processor.h>
#include <adi_tmr_config.h>
#include <adi_types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef __ICCARM__
/* IAR MISRA C 2004 error suppressions.
 *
 * Pm009 (rule 5.1): identifiers shall not rely on significance of more than 31 characters.
 *   IAR compiler supports longer identifiers.
 */
#pragma diag_suppress=Pm009
#endif /* __ICCARM__ */

/*================ D A T A   T Y P E S ================*/

/*! Amount of memory(In bytes) required by the timer device driver  for managing the operation.
 * This memory is completely owned by the driver till the end of
 *  the operation.
 */
#define ADI_TMR_MEMORY_SIZE   (16u)


/*!
    @brief  Gptimer Handle.
    @details The Timer Handle declaration. The handle is used in all subsequent
             Timer API calls.
 * */
typedef void*   ADI_TMR_HANDLE;

/*! enumeration of possible return codes from timer service */
typedef enum
{
    /*! Success                                            */
    ADI_TMR_SUCCESS  = 0,
    /*! Generic failure                                    */
    ADI_TMR_FAILURE,
    /*! The given timer handle is invalid                  */
    ADI_TMR_INVALID_HANDLE,
    /*! Insufficient memory allocated                      */
    ADI_TMR_INSUFFICIENT_MEMORY,
    /*! Trying to open a timer which is already in use     */
    ADI_TMR_IN_USE,
    /*! If any of the given pointers are invalid          */
    ADI_TMR_INVALID_POINTER,
    /*! If the TIMER ID is out of range                    */
    ADI_TMR_INVALID_TIMERID,
    /*! Specified operation is not allowed when device is enabled. */
    ADI_TMR_OPERATION_NOT_ALLOWED

}ADI_TMR_RESULT;


/*!
 * Timer Prescaler Definitions used to scale down the clock source.
 */
typedef enum
{
    /*! Count up/down every source clock */
    ADI_GPT_PRESCALER_1         = 0 << BITP_TMR_CTL_PRE,
    /*! Count up/down every 16 source clocks */
    ADI_GPT_PRESCALER_16        = 1 << BITP_TMR_CTL_PRE,
    /*! Count up/down every 64 source clocks */
    ADI_GPT_PRESCALER_64        = 2 << BITP_TMR_CTL_PRE,
    /*! Count up/down every 256 source clocks */
    ADI_GPT_PRESCALER_256       = 3 << BITP_TMR_CTL_PRE
} ADI_TMR_PRESCALER;


/*!
 * Timer Clock Select Definitions. The GP Timers can be clocked from a variety of sources.
 * FIXME: Not clear in  the documentation in the HRM. Need to fix it.
 */
typedef enum
{
    /*! Use periphreal clock (PCLK) */
    ADI_TMR_CLOCK_PCLK     = 0 << BITP_TMR_CTL_CLK,
    /*! use internal high frequency clock */
    ADI_TMR_CLOCK_HFOSC     = 1 << BITP_TMR_CTL_CLK,
    /*! Use internal low frequency clock */
    ADI_TMR_CLOCK_LFOSC     = 2 << BITP_TMR_CTL_CLK,
    /*! Use external low frequency clock */
    ADI_TMR_CLOCK_LFXTL     = 3 << BITP_TMR_CTL_CLK
} ADI_TMR_CLOCK_SOURCE;


/*!
 * GP Timer Capture Event Definitions
 *
 * There are 32 interrupt events that can be captured by the general-purpose timers.
 * Any one of the 32 events associated with a general-purpose timer can cause a capture
 * of the 16-bit GPTVAL register into the 16-bit GPTCAP register. GPTCON has a 5-bit field
 * selecting which of the 32 events to capture.
 */
typedef enum
{
    /* GPT0 EVENTS */

    /*! Wakeup Timer triggered event capture */
    ADI_TMR0_CAPTURE_EVENT_RTC            =  0 << BITP_TMR_CTL_EVTRANGE,
    /*! Implemenation specific externally triggered event capture */
    ADI_TMR0_CAPTURE_EVENT_EXT_INT_0      =  1 << BITP_TMR_CTL_EVTRANGE,
    /*! Implemenation specific externally triggered event capture */
    ADI_TMR0_CAPTURE_EVENT_EXT_INT_1      =  2 << BITP_TMR_CTL_EVTRANGE,
    /*! Implemenation specific externally triggered event capture */
    ADI_TMR0_CAPTURE_EVENT_EXT_INT_2      =  3 << BITP_TMR_CTL_EVTRANGE,
    /*! Implemenation specific externally triggered event capture */
    ADI_TMR0_CAPTURE_EVENT_EXT_INT_3      =  4 << BITP_TMR_CTL_EVTRANGE,
    /*! Watch Dog Timer triggered event capture */
    ADI_TMR0_CAPTURE_EVENT_WDT_TIMER      =  5 << BITP_TMR_CTL_EVTRANGE,
    /*! VREG over voltage interrupt */
    ADI_TMR0_CAPTURE_EVENT_VREG_OVER      =  6 << BITP_TMR_CTL_EVTRANGE,
    /*! Battery voltage range interrupt */
    ADI_TMR0_CAPTURE_EVENT_VBAT_RANGE     =  7 << BITP_TMR_CTL_EVTRANGE,
    /*! FIXME: No details */
    ADI_TMR0_CAPTURE_EVENT_P0_8_DIN       =  8 << BITP_TMR_CTL_EVTRANGE,
    /*! Group-A interrupt */
    ADI_TMR0_CAPTURE_EVENT_GPIO_INTA      =  9 << BITP_TMR_CTL_EVTRANGE,
    /*! Group-B interrupt */
    ADI_TMR0_CAPTURE_EVENT_GPIO_INTB      = 10 << BITP_TMR_CTL_EVTRANGE,
    /*! GP Timer1 triggered event capture */
    ADI_TMR0_CAPTURE_EVENT_TIMER1         = 11 << BITP_TMR_CTL_EVTRANGE,
    /*! GP Timer2 triggered event capture */
    ADI_TMR0_CAPTURE_EVENT_TIMER2         = 12 << BITP_TMR_CTL_EVTRANGE,
    /*! Flash Controller event capture */
    ADI_TMR0_CAPTURE_EVENT_FLASH          = 13 << BITP_TMR_CTL_EVTRANGE,
    /*! SPORT-A event capture */
    ADI_TMR0_CAPTURE_EVENT_SPORTA         = 14 << BITP_TMR_CTL_EVTRANGE,
    /*! SPORT-B event capture  */
    ADI_TMR0_CAPTURE_EVENT_SPORTB         = 15 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-0 done event capture  */
    ADI_TMR0_CAPTURE_EVENT_DMA0_DONE      = 16 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-1 done event capture  */
    ADI_TMR0_CAPTURE_EVENT_DMA1_DONE      = 17 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-2 done event capture  */
    ADI_TMR0_CAPTURE_EVENT_DMA2_DONE      = 18 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-3 done event capture  */
    ADI_TMR0_CAPTURE_EVENT_DMA3_DONE      = 19 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-4 done event capture  */
    ADI_TMR0_CAPTURE_EVENT_DMA4_DONE      = 20 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-5 done event capture  */
    ADI_TMR0_CAPTURE_EVENT_DMA5_DONE      = 21 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-6 done event capture  */
    ADI_TMR0_CAPTURE_EVENT_DMA6_DONE      = 22 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-7 done event capture  */
    ADI_TMR0_CAPTURE_EVENT_DMA7_DONE      = 23 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-8 done event capture  */
    ADI_TMR0_CAPTURE_EVENT_DMA8_DONE      = 24 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-9 done event capture  */
    ADI_TMR0_CAPTURE_EVENT_DMA9_DONE      = 25 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-10 done event capture  */
    ADI_TMR0_CAPTURE_EVENT_DMA10_DONE     = 26 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-11 done event capture  */
    ADI_TMR0_CAPTURE_EVENT_DMA11_DONE     = 27 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-12 done event capture  */
    ADI_TMR0_CAPTURE_EVENT_DMA12_DONE     = 28 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-13 done event capture  */
    ADI_TMR0_CAPTURE_EVENT_DMA13_DONE     = 29 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-14 done event capture  */
    ADI_TMR0_CAPTURE_EVENT_DMA14_DONE     = 30 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-15 done event capture  */
    ADI_TMR0_CAPTURE_EVENT_DMA15_DONE     = 31 << BITP_TMR_CTL_EVTRANGE,

    /* GPT1 EVENTS */

    /*! UART-0 triggered event capture */
    ADI_TMR1_CAPTURE_EVENT_UART0          = 0  << BITP_TMR_CTL_EVTRANGE,
    /*! SPI-0 triggered event capture */
    ADI_TMR1_CAPTURE_EVENT_SPI0           = 1 << BITP_TMR_CTL_EVTRANGE,
    /*! SPI-1 triggered event capture */
    ADI_TMR1_CAPTURE_EVENT_SPI1           = 2 << BITP_TMR_CTL_EVTRANGE,
    /*! SPI-2 triggered event capture */
    ADI_TMR1_CAPTURE_EVENT_SPI2           = 3 << BITP_TMR_CTL_EVTRANGE,
    /*! I2C Slave triggered event capture */
    ADI_TMR1_CAPTURE_EVENT_I2C0S          = 4 << BITP_TMR_CTL_EVTRANGE,
    /*! I2C Master triggered event capture */
    ADI_TMR1_CAPTURE_EVENT_I2C0M          = 5 << BITP_TMR_CTL_EVTRANGE,
    /*! Crypto triggered event capture */
    ADI_TMR1_CAPTURE_EVENT_CRYPTO         = 6 << BITP_TMR_CTL_EVTRANGE,
    /*! Exernal osc triggered event capture */
    ADI_TMR1_CAPTURE_EVENT_XTL_OSC        = 8 << BITP_TMR_CTL_EVTRANGE,
    /*! PLL triggered event capture */
    ADI_TMR1_CAPTURE_EVENT_PLL            = 9 << BITP_TMR_CTL_EVTRANGE,
    /*! Random number generator (RNG) triggered event capture */
    ADI_TMR1_CAPTURE_EVENT_RNG            = 10 << BITP_TMR_CTL_EVTRANGE,
    /*!Beep device triggered event capture */
    ADI_TMR1_CAPTURE_EVENT_BEEP	          = 11 << BITP_TMR_CTL_EVTRANGE,
    /*! Implemenation specific externally triggered event capture */
    ADI_TMR1_CAPTURE_EVENT_EXT_INT_0      = 12 << BITP_TMR_CTL_EVTRANGE,
    /*! Implemenation specific externally triggered event capture */
    ADI_TMR1_CAPTURE_EVENT_EXT_INT_1      = 13 << BITP_TMR_CTL_EVTRANGE,
    /*! GP Timer0 triggered event capture */
    ADI_TMR1_CAPTURE_EVENT_TIMER0         = 14 << BITP_TMR_CTL_EVTRANGE,
    /*! GP Timer2 triggered event capture */
    ADI_TMR1_CAPTURE_EVENT_TIMER2         = 15 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-16 done event capture  */
    ADI_TMR1_CAPTURE_EVENT_DMA16_DONE     = 24 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-17 done event capture  */
    ADI_TMR1_CAPTURE_EVENT_DMA17_DONE     = 25 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-18 done event capture  */
    ADI_TMR1_CAPTURE_EVENT_DMA18_DONE     = 26 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-19 done event capture  */
    ADI_TMR1_CAPTURE_EVENT_DMA19_DONE     = 27 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-20 done event capture  */
    ADI_TMR1_CAPTURE_EVENT_DMA20_DONE     = 28 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-21 done event capture  */
    ADI_TMR1_CAPTURE_EVENT_DMA21_DONE     = 29 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-22 done event capture  */
    ADI_TMR1_CAPTURE_EVENT_DMA22_DONE     = 30 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-23 done event capture  */
    ADI_TMR1_CAPTURE_EVENT_DMA23_DONE     = 31 << BITP_TMR_CTL_EVTRANGE,

    /* GPT2 EVENTS */

    /*! Implemenation specific externally triggered event capture */
    ADI_TMR2_CAPTURE_EVENT_EXT_INT_0      = 0 << BITP_TMR_CTL_EVTRANGE,
    /*! Implemenation specific externally triggered event capture */
    ADI_TMR2_CAPTURE_EVENT_EXT_INT_1      = 1 << BITP_TMR_CTL_EVTRANGE,
    /*! Implemenation specific externally triggered event capture */
    ADI_TMR2_CAPTURE_EVENT_EXT_INT_2      = 2 << BITP_TMR_CTL_EVTRANGE,
    /*! Implemenation specific externally triggered event capture */
    ADI_TMR2_CAPTURE_EVENT_EXT_INT_3      = 3 << BITP_TMR_CTL_EVTRANGE,
    /*! DMA channel-error  event capture  */
    ADI_TMR1_CAPTURE_EVENT_DMA_ERROR      = 4 << BITP_TMR_CTL_EVTRANGE,
    /*! Wakeup Timer triggered event capture */
    ADI_TMR2_CAPTURE_EVENT_RTC            = 5 << BITP_TMR_CTL_EVTRANGE,
    /*! GP Timer0 triggered event capture */
    ADI_TMR2_CAPTURE_EVENT_TIMER0         = 6 << BITP_TMR_CTL_EVTRANGE,
    /*! GP Timer1 triggered event capture */
    ADI_TMR2_CAPTURE_EVENT_TIMER1         = 7 << BITP_TMR_CTL_EVTRANGE

} ADI_TMR_CAPTURE_EVENTS;


/*!
  @brief        GP Timer callback events.
  @details      When a callback is registered to get notification about the time out
                and event capture, the application supplied callback will be called
                with one of the following events.
*/
typedef enum
{
    /*! The timer has expired. */
    ADI_TMR_EVENT_TIMEOUT     = BITM_TMR_STAT_TIMEOUT,

    /*! The given timer event is captured. Application should use the API
         #adi_tmr_GetCapturedValue to get the timer value when the event is
         captured. */
    ADI_TMR_EVENT_CAPTURED    = BITM_TMR_STAT_CAPTURE

} ADI_TMR_EVENT;

/*!
    @brief  GP Timer couting modes.
    @details GP Timers can be set to count up or to count down.
*/
typedef enum
{
  /*! Timer count down mode */
  ADI_TMR_COUNT_DOWN   = 0 << BITP_TMR_CTL_UP,
  /*! Timer count up mode */
  ADI_TMR_COUNT_UP     = 1 << BITP_TMR_CTL_UP

} ADI_TMR_COUNT_MODE;

/*!
    @brief  GP Timer couting modes.
    @details GP Timers can be set to count up or to count down.
*/
typedef enum
{
   /*! Timer free running mode */
  ADI_TMR_FREE_RUNNING_MODE   = 0 << BITP_TMR_CTL_MODE,
   /*! Timer preiodic mode */
  ADI_TMR_PERIODIC_MODE       = 1 << BITP_TMR_CTL_MODE

} ADI_TMR_RUNNING_MODE;

/*!
    @brief  Timer PWM  modes.
*/
typedef enum
{
  /*! PWM output toggle mode */
  ADI_TMR_PWM_TOGGLE_MODE    = 0 << BITP_TMR_PWMCTL_MATCH,
   /*! PWM output match mode */
  ADI_TMR_PWM_MATCH_MODE     = 1 << BITP_TMR_PWMCTL_MATCH

} ADI_TMR_PWM_MODE;

/*************************************/
/* Timer API                         */
/*************************************/
ADI_TMR_RESULT adi_tmr_Open(
               uint32_t         DeviceNumber,
	           void            *pDeviceMemory,
               uint32_t         MemorySize,
               ADI_TMR_HANDLE  *phDevice
               );

ADI_TMR_RESULT adi_tmr_Close(
               ADI_TMR_HANDLE hDevice
               );

ADI_TMR_RESULT adi_tmr_GetCapturedValue(
		       ADI_TMR_HANDLE hDevice,
		       uint16_t *pCapturedValue
		       );

ADI_TMR_RESULT adi_tmr_IsBusy(
		       ADI_TMR_HANDLE hDevice,
		       bool_t *pbBusy
		       );

ADI_TMR_RESULT adi_tmr_SetLoadValue(
	           ADI_TMR_HANDLE hDevice,
	           const uint16_t nLoadValue
	           );

ADI_TMR_RESULT adi_tmr_GetLoadValue(
	           ADI_TMR_HANDLE hDevice,
	           uint16_t *pTimerLdVal
	           );

ADI_TMR_RESULT adi_GPT_GetCurrentValue(
	           ADI_TMR_HANDLE hDevice,
	           uint16_t *pTimerValue
	           );

ADI_TMR_RESULT adi_tmr_EnableEventCapture(
		       ADI_TMR_HANDLE hDevice,
		       bool_t bEnable
		       );

ADI_TMR_RESULT adi_tmr_SetPrescaler(
	           ADI_TMR_HANDLE hDevice,
	           ADI_TMR_PRESCALER ePreScaler
	           );

ADI_TMR_RESULT adi_tmr_Enable(
               ADI_TMR_HANDLE hDevice,
               bool_t bEnable
	           );

ADI_TMR_RESULT adi_tmr_SetRunMode(
	           ADI_TMR_HANDLE hDevice,
	           ADI_TMR_RUNNING_MODE eMode
	           );

ADI_TMR_RESULT adi_tmr_SetCountMode(
	           ADI_TMR_HANDLE hDevice,
	           ADI_TMR_COUNT_MODE eMode
	           );

ADI_TMR_RESULT adi_tmr_SetClockSource(
	           ADI_TMR_HANDLE hDevice,
	           ADI_TMR_CLOCK_SOURCE eClockSource
	           );

ADI_TMR_RESULT adi_tmr_SetPWMidle(
	           ADI_TMR_HANDLE hDevice,
	           bool_t bIdleHigh);

ADI_TMR_RESULT adi_tmr_SetPWMMode(
	           ADI_TMR_HANDLE hDevice,
	           ADI_TMR_PWM_MODE  eMode
	           );

ADI_TMR_RESULT adi_tmr_SetPWMMatchValue(
	           ADI_TMR_HANDLE hDevice,
	           uint16_t  nValue
	           );

ADI_TMR_RESULT adi_tmr_SetEventToCapture(
               ADI_TMR_HANDLE hDevice,
               ADI_TMR_CAPTURE_EVENTS eEvent
               );

ADI_TMR_RESULT adi_tmr_EnableReloading(
               ADI_TMR_HANDLE hDevice,
               bool_t bEnable
               );

ADI_TMR_RESULT adi_tmr_RegisterCallback(
               ADI_TMR_HANDLE const  hDevice,
               ADI_CALLBACK   const   pfCallback,
               void        *const     pCBparam
               );

ADI_TMR_RESULT adi_tmr_EnableSyncBypass(
               ADI_TMR_HANDLE hDevice, 
               bool_t bEnable);   

ADI_TMR_RESULT adi_tmr_EnablePrescaleReset(
               ADI_TMR_HANDLE hDevice, 
               bool_t bEnable);
#ifdef __cplusplus
}
#endif

#endif /*__ADI_TMR_CM302X_H__ */

/*
** EOF
*/

/*@}*/
