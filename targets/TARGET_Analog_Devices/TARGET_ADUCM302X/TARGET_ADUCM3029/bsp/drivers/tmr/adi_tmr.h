/*! *****************************************************************************
 * @file    adi_tmr.h
 * @brief   GP and RGB timer device driver public header file
 -----------------------------------------------------------------------------
Copyright (c) 2016 Analog Devices, Inc.

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


#ifndef ADI_TMR_H
#define ADI_TMR_H


#include <stdint.h>
#include <stdbool.h>
#include <adi_processor.h>
#include <adi_callback.h>


/** @addtogroup TMR_Driver Timer Driver
 *  @{
 */


/*!
 *****************************************************************************
 *  \enum ADI_TMR_RESULT
 *   Enumeration for result code returned from the timer device driver functions.
 *   The return value of all timer APIs returning #ADI_TMR_RESULT should always 
 *   be tested at the application level for success or failure.
 *****************************************************************************/
typedef enum {
    /*! Successful operation */
    ADI_TMR_SUCCESS,
    /*! Bad device number supplied by user */
    ADI_TMR_BAD_DEVICE_NUM,
    /*! Bad PWM output number supplied by user to #adi_tmr_ConfigPwm */
    ADI_TMR_BAD_PWM_NUM,
    /*! Bad event number supplied by user to #adi_tmr_ConfigEvent */
    ADI_TMR_BAD_EVENT_ID,
    /*! Bad timer configuration, reloading and free running are mutually exclusive options */
    ADI_TMR_BAD_RELOAD_CONFIGURATION,
    /*! Setup or enable function called while the timer is running */
    ADI_TMR_OPERATION_NOT_ALLOWED,
    /*! Timeout while waiting for busy bit to clear before writing control register */
    ADI_TMR_DEVICE_BUSY,
    /*! User attempts to reload the timer when reloading has not been enabled */
    ADI_TMR_RELOAD_DISABLED,
    /*! User attempts to read the current or captured count with a NULL pointer */
    ADI_TMR_NULL_POINTER
} ADI_TMR_RESULT;

/*!
 *****************************************************************************
 *  \enum ADI_TMR_DEVICE
 *   Enumeration for the hardware peripheral being used during the API call
 *****************************************************************************/
typedef enum {
    /*! General purpose timer 0 */
    ADI_TMR_DEVICE_GP0 = 0u,
    /*! General purpose timer 1 */
    ADI_TMR_DEVICE_GP1 = 1u,
    /*! General purpose timer 2 */
    ADI_TMR_DEVICE_GP2 = 2u,
#if defined(__ADUCM302x__)
    /*! Total number of devices (private) */
    ADI_TMR_DEVICE_NUM = 3u,
#elif defined(__ADUCM4x50__)
    /*! RGB timer */
    ADI_TMR_DEVICE_RGB = 3u,
    /*! Total number of devices (private) */
    ADI_TMR_DEVICE_NUM = 4u,
#else 
#error TMR is not ported for this processor
#endif
} ADI_TMR_DEVICE;

/*!
 *****************************************************************************
 *  \enum ADI_TMR_EVENT
 *   Enumeration of events notified in the application provided callback.
 *****************************************************************************/
typedef enum {
  /*! Timeout event occurred */
  ADI_TMR_EVENT_TIMEOUT = 0x01,  
  /*! Event capture event occurred */
  ADI_TMR_EVENT_CAPTURE = 0x02,
} ADI_TMR_EVENT;

/*!
 *****************************************************************************
 *  \enum ADI_TMR_PRESCALER
 *   Prescale options when configuring the timer
 *****************************************************************************/
typedef enum {
    /*! Count every 1 source clock periods */
    ADI_TMR_PRESCALER_1   = 0u,
    /*! Count every 16 source clock periods */
    ADI_TMR_PRESCALER_16  = 1u, 
    /*! Count every 64 source clock periods */
    ADI_TMR_PRESCALER_64  = 2u,
    /*! Count every 256 source clock periods */
    ADI_TMR_PRESCALER_256 = 3u,
} ADI_TMR_PRESCALER;

/*!
 *****************************************************************************
 *  \enum ADI_TMR_CLOCK_SOURCE
 *   Source clock options when configuring the timer
 *****************************************************************************/
typedef enum {
    /*! Use periphreal clock (PCLK) */
    ADI_TMR_CLOCK_PCLK  = 0u, 
    /*! Use internal high frequency clock (HFOSC) */
    ADI_TMR_CLOCK_HFOSC = 1u, 
    /*! Use internal low frequency clock (LFOSC) */
    ADI_TMR_CLOCK_LFOSC = 2u, 
    /*! Use external low frequency clock (LFXTAL) */
    ADI_TMR_CLOCK_LFXTAL = 3u,
} ADI_TMR_CLOCK_SOURCE;

/*!
 *****************************************************************************
 *  \enum ADI_TMR_PWM_OUTPUT
 *   RGB PWM outputs, used to specify which PWM output to configure. For the GP
 *   timers only #ADI_TMR_PWM_OUTPUT_0 is allowed. The RGB timer has all three 
 *   outputs.
 *****************************************************************************/
typedef enum {
    /*! PWM output 0 */
    ADI_TMR_PWM_OUTPUT_0   = 0u,
    /*! PWM output 1 */
    ADI_TMR_PWM_OUTPUT_1   = 1u,
    /*! PWM output 2 */
    ADI_TMR_PWM_OUTPUT_2   = 2u,
    /*! Total number of outputs (private) */
    ADI_TMR_PWM_OUTPUT_NUM = 3u,
} ADI_TMR_PWM_OUTPUT;

/*!
 *****************************************************************************
 *  \struct ADI_TMR_CONFIG
 *   Configuration structure to fill and pass to #adi_tmr_ConfigTimer when  
 *   configuring the GP or RGB timer
 *****************************************************************************/
typedef struct {
    /*! True to count up, false to count down */
    bool                 bCountingUp;
    /*! True for periodic (specific load value), false for free running (0xFFFF) */
    bool                 bPeriodic;
    /*! Prescaler */
    ADI_TMR_PRESCALER    ePrescaler;
    /*! Clock source */
    ADI_TMR_CLOCK_SOURCE eClockSource;
    /*! Load value (only relevant in periodic mode) */
    uint16_t             nLoad;
    /*! Asynchronous load value (only relevant in periodic mode, and when PCLK is used) */
    uint16_t             nAsyncLoad;
    /*! True to enable reloading, false to disable it (only relevant in periodic mode) */
    bool                 bReloading;
    /*! True to enable sync bypass, false to disable it */
    bool                 bSyncBypass;
} ADI_TMR_CONFIG;

/*!
 *****************************************************************************
 *  \struct ADI_TMR_EVENT_CONFIG
 *   Configuration structure to fill and pass to #adi_tmr_ConfigEvent when  
 *   configuring event capture
 *****************************************************************************/
typedef struct {
    /*! True to enable event capture, false to disable it */
    bool    bEnable;
    /*! True to reset the counter and prescaler when the selected event occurs, false to let it continue */
    bool    bPrescaleReset;
    /*! Event identifier, see hardware reference manual for details */
    uint8_t nEventID;
} ADI_TMR_EVENT_CONFIG;

/*!
 *****************************************************************************
 *  \struct ADI_TMR_PWM_CONFIG
 *   Configuration structure to fill and pass to #adi_tmr_ConfigPwm when  
 *   configuring pulse width modulation output
 *****************************************************************************/
typedef struct {
    /*! PWM output */
    ADI_TMR_PWM_OUTPUT     eOutput;
    /*! True if match mode (configurable duty cycle), false if toggle mode (50% duty cycle) */
    bool                   bMatch;
    /*! True for PWM idle high, false for PWM idle low */
    bool                   bIdleHigh;
    /*! Match value, only applicable if in match mode */
    uint16_t               nMatchValue;
} ADI_TMR_PWM_CONFIG;

/******************************************************************************
 * PUBLIC API
 * 1.) Eliminate functions that may be optimized out by the linker
 * 2.) Ordered by designed function call sequence
 *****************************************************************************/

/* Initialize timer driver */
ADI_TMR_RESULT adi_tmr_Init            (ADI_TMR_DEVICE const eDevice, ADI_CALLBACK const pfCallback, void * const pCBParam, bool bEnableInt);

/* Configuration interface functions */
ADI_TMR_RESULT adi_tmr_ConfigTimer     (ADI_TMR_DEVICE const eDevice, ADI_TMR_CONFIG*       timerConfig);
ADI_TMR_RESULT adi_tmr_ConfigEvent     (ADI_TMR_DEVICE const eDevice, ADI_TMR_EVENT_CONFIG* eventConfig);
ADI_TMR_RESULT adi_tmr_ConfigPwm       (ADI_TMR_DEVICE const eDevice, ADI_TMR_PWM_CONFIG*   pwmConfig  );

/* Timer start and stop */
ADI_TMR_RESULT adi_tmr_Enable          (ADI_TMR_DEVICE const eDevice, bool bEnable);

/* Read functions */
ADI_TMR_RESULT adi_tmr_GetCurrentCount (ADI_TMR_DEVICE const eDevice, uint16_t *pCount);
ADI_TMR_RESULT adi_tmr_GetCaptureCount (ADI_TMR_DEVICE const eDevice, uint16_t *pCount);

/* Reload function */
ADI_TMR_RESULT adi_tmr_Reload          (ADI_TMR_DEVICE const eDevice);


/*! @} */


#endif /* ADI_TMR_H */
