/*! *****************************************************************************
 * @file    adi_tmr.c
 * @brief   GP and RGB timer device driver implementation
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


#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm011 (rule 6.3): the basic types of char, int, short, long, float, and double should not be used
*   Necessary for stdbool.
*
* Pm073 (rule 14.7): a function should have a single point of exit
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function
*   Multiple returns are used for error handling.
*
* Pm123 (rule 8.5): there shall be no definition of objects or functions in a header file
*   Static configuration data file is included.
*
* Pm140 (Rule 11.4): a cast should not be performed between a pointer type and an integral type
*   This violation appears when deferencing the pointer to the register typedef. No way around this.
*
* Pm141 (Rule 11.4): a cast should not be performed between a pointer to object type and a different pointer to object type
*   The pointer casting is necessary to allow the GP and RGB timers to abstracted into one driver. This has been approved by the PO.
*/
#pragma diag_suppress=Pm011,Pm073,Pm123,Pm140,Pm141,Pm143
#endif /* __ICCARM__ */


/** @addtogroup TMR_Driver Timer Driver
 *  @{
 * @brief General Purpose and RGB Timer Driver
 * @details The timer driver controls the timer period, event capture, and
 * pulse width modulation (PWM) features of the General Purpose (GP) Timers and
 * the RGB Timer.
 *  @note The application must include drivers/tmr/adi_tmr.h to use this driver
 */

#include <adi_processor.h>
#include <adi_tmr_config.h>
#include <rtos_map/adi_rtos_map.h>
#include <drivers/tmr/adi_tmr.h>

/* Static configuration data */
#if ADI_TIMER_ENABLE_STATIC_CONFIG_SUPPORT == 1u
#include "adi_tmr_data.c"
#endif


/* In adi_tmr_ConfigPwm, the bit positions for just PWM0 are used for PWM1 and PWM2 to simplify the code. Check here to make sure this is safe. */
#if BITP_TMR_RGB_PWM0CTL_IDLESTATE != BITP_TMR_RGB_PWM1CTL_IDLESTATE
#error "Bit positions for PWM0 and PWM1 do not match. Fix adi_tmr_ConfigPwm."
#endif
#if BITP_TMR_RGB_PWM0CTL_IDLESTATE != BITP_TMR_RGB_PWM2CTL_IDLESTATE
#error "Bit positions for PWM0 and PWM2 do not match. Fix adi_tmr_ConfigPwm."
#endif
#if BITP_TMR_RGB_PWM0CTL_MATCH != BITP_TMR_RGB_PWM1CTL_MATCH
#error "Bit positions for PWM0 and PWM1 do not match. Fix adi_tmr_ConfigPwm."
#endif
#if BITP_TMR_RGB_PWM0CTL_MATCH != BITP_TMR_RGB_PWM2CTL_MATCH
#error "Bit positions for PWM0 and PWM2 do not match. Fix adi_tmr_ConfigPwm."
#endif

/*! Number of events that can be captured */
#define ADI_TMR_NUM_EVENTS (40u)

/*! \cond PRIVATE */

/* Since the RGB typedef is a superset of the GP typedef, treat the GP timers as RGB timers and restrict top register access */
static ADI_TMR_RGB_TypeDef * adi_tmr_registers[ADI_TMR_DEVICE_NUM] = {(ADI_TMR_RGB_TypeDef *) pADI_TMR0, (ADI_TMR_RGB_TypeDef *) pADI_TMR1, (ADI_TMR_RGB_TypeDef *) pADI_TMR2, pADI_TMR_RGB};

/* Interrupt enums */
static const IRQn_Type adi_tmr_interrupt[ADI_TMR_DEVICE_NUM] = {TMR0_EVT_IRQn, TMR1_EVT_IRQn, TMR2_EVT_IRQn, TMR_RGB_EVT_IRQn};

/* Private data that the driver needs to retain between function calls */
static ADI_CALLBACK adi_tmr_callbacks[ADI_TMR_DEVICE_NUM];
static void * adi_tmr_parameters[ADI_TMR_DEVICE_NUM];

static ADI_TMR_RESULT WaitForStatusBit   (ADI_TMR_DEVICE const eDevice, uint16_t nBusyBit);
static void           CommonIntHandler   (ADI_TMR_DEVICE const eDevice);
       void           GP_Tmr0_Int_Handler(void);
       void           GP_Tmr1_Int_Handler(void);
       void           GP_Tmr2_Int_Handler(void);
       void           RGB_Tmr_Int_Handler(void);

/*! \endcond */


/*********************************************************************************
                                    API IMPLEMENTATIONS 
*********************************************************************************/


/*!
 * @brief       Initialize GP or RGB Timer
 *
 * @details     Setup callback function, device interrupt, and perform static configuration (if applicable).
 *
 * @note        This function can only be called when the timer is disabled. This function should be called
 *              before any other functions are called.
 *
 * @param [in]  eDevice : Device number
 *
 * @param [in]  pfCallback : Callback function
 *
 * @param [in]  pCBParam : Callback function parameter
 *
 * @param [in]  bEnableInt : True to enable the device interrupt, false to disable it
 *
 * @return      ADI_TMR_RESULT
 *                - #ADI_TMR_BAD_DEVICE_NUM        [D]  Invalid eDevice parameter supplied
 *                - #ADI_TMR_OPERATION_NOT_ALLOWED [D]  Timer is currently running
 *                - #ADI_TMR_SUCCESS                    Function call completed successfully
 *
 */
ADI_TMR_RESULT adi_tmr_Init(ADI_TMR_DEVICE const eDevice, ADI_CALLBACK const pfCallback, void * const pCBParam, bool bEnableInt) {
#ifdef ADI_DEBUG
    /* IF(Bad device input parameter) */
    if (eDevice >= ADI_TMR_DEVICE_NUM) {
        return ADI_TMR_BAD_DEVICE_NUM;
    } /* ENDIF */
    /* IF(The timer is already running) */
    if ((adi_tmr_registers[eDevice]->CTL & BITM_TMR_RGB_CTL_EN) == BITM_TMR_RGB_CTL_EN) {
        return ADI_TMR_OPERATION_NOT_ALLOWED;
    } /* ENDIF */         
#endif
    /* Setup the callback function */
    adi_tmr_callbacks [eDevice] = pfCallback;
    adi_tmr_parameters[eDevice] = pCBParam;

    /* IF(Enable interrupt) */
    if (bEnableInt == true) {
        NVIC_EnableIRQ(adi_tmr_interrupt[eDevice]);
    /* ELSE(Disable interrupt) */
    } else {
        NVIC_DisableIRQ(adi_tmr_interrupt[eDevice]);
    } /* ENDIF */

    /* Static configuration */
#if ADI_TIMER_ENABLE_STATIC_CONFIG_SUPPORT == 1u
    adi_tmr_registers[eDevice]->CTL           = aTimerCtlConfig     [eDevice];
    adi_tmr_registers[eDevice]->LOAD          = aTimerLoadConfig    [eDevice];
    adi_tmr_registers[eDevice]->ALOAD         = aTimerALoadConfig   [eDevice];
    adi_tmr_registers[eDevice]->EVENTSELECT   = aTimerEventConfig   [eDevice];
    adi_tmr_registers[eDevice]->PWM0CTL       = aTimerPwmCtlConfig  [eDevice];
    adi_tmr_registers[eDevice]->PWM0MATCH     = aTimerPwmMatchConfig[eDevice];    

    /* IF(Initializing the RGB timer, there are 2 other PWM outputs to configure) */
    if (eDevice == ADI_TMR_DEVICE_RGB) { 
        /* The array is bumped by 1 to get to the 5th entry in the static config array, which contains RGB PWM1 */
        adi_tmr_registers[eDevice]->PWM1CTL   = aTimerPwmCtlConfig  [eDevice+1u];
        adi_tmr_registers[eDevice]->PWM1MATCH = aTimerPwmMatchConfig[eDevice+1u];
        /* The array is bumped by 2 to get to the 6th entry in the static config array, which contains RGB PWM2 */
        adi_tmr_registers[eDevice]->PWM2CTL   = aTimerPwmCtlConfig  [eDevice+2u];        
        adi_tmr_registers[eDevice]->PWM2MATCH = aTimerPwmMatchConfig[eDevice+2u];
    } /* ENDIF */
#endif

    return ADI_TMR_SUCCESS;
}


/*!
 * @brief       Configure GP or RGB Timer
 *
 * @details     Configure the basic hardware timer parameters.
 *
 * @note        This function can only be called when the timer is disabled.
 *
 * @param [in]  eDevice : Device number
 *
 * @param [in]  timerConfig : Timer configuration structure, filled by user prior to function call
 *
 * @return      ADI_TMR_RESULT
 *                - #ADI_TMR_BAD_DEVICE_NUM           [D] Invalid eDevice parameter supplied
 *                - #ADI_TMR_BAD_RELOAD_CONFIGURATION [D] bPeriodic is false and bReloading is true
 *                - #ADI_TMR_OPERATION_NOT_ALLOWED    [D] Timer is currently running
 *                - #ADI_TMR_DEVICE_BUSY                  Timer is busy processing a previous control register write
 *                - #ADI_TMR_SUCCESS                      Function call completed successfully
 *
 */
ADI_TMR_RESULT adi_tmr_ConfigTimer(ADI_TMR_DEVICE const eDevice, ADI_TMR_CONFIG timerConfig) {
    uint16_t nTemp;
#ifdef ADI_DEBUG
    /* IF(Bad device input parameter) */
    if (eDevice >= ADI_TMR_DEVICE_NUM) {
        return ADI_TMR_BAD_DEVICE_NUM;
    } /* ENDIF */
    /* IF(Bad configuration, cannot enable reloading while in free running mode) */
    if ((timerConfig.bPeriodic == false) && (timerConfig.bReloading == true)) {
        return ADI_TMR_BAD_RELOAD_CONFIGURATION;
    } /* ENDIF */
    /* IF(The timer is already running) */
    if ((adi_tmr_registers[eDevice]->CTL & BITM_TMR_RGB_CTL_EN) == BITM_TMR_RGB_CTL_EN) {
        return ADI_TMR_OPERATION_NOT_ALLOWED;
    } /* ENDIF */
#endif
    /* Set the load registers */
    adi_tmr_registers[eDevice]->LOAD  = timerConfig.nLoad;
    adi_tmr_registers[eDevice]->ALOAD = timerConfig.nAsyncLoad;

    /* IF(Busy bit does not clear after waiting) */
    if (ADI_TMR_SUCCESS != WaitForStatusBit(eDevice, (uint16_t) BITM_TMR_RGB_STAT_BUSY)) {
        return ADI_TMR_DEVICE_BUSY; 
    } /* ENDIF */

    /* Read the control register and clear everything aside to the event capture bits, which are the only fields not set in this function */
    nTemp  = adi_tmr_registers[eDevice]->CTL;
    nTemp &= (uint16_t) (BITM_TMR_RGB_CTL_EVTEN | BITM_TMR_RGB_CTL_RSTEN);

    /* Setup the prescaler and the clock source */
    nTemp |=  (uint16_t)(((uint16_t) timerConfig.ePrescaler  ) << BITP_TMR_RGB_CTL_PRE);
    nTemp |=  (uint16_t)(((uint16_t) timerConfig.eClockSource) << BITP_TMR_RGB_CTL_CLK);

    /* IF(Periodic mode) */
    if (timerConfig.bPeriodic == true) {
        nTemp |= (1u << BITP_TMR_RGB_CTL_MODE);
    } /* ENDIF */    
    
    /* IF(Counting up) */
    if (timerConfig.bCountingUp == true) {
        nTemp |= (1u << BITP_TMR_RGB_CTL_UP);
    } /* ENDIF */
    
    /* IF(Reloading is enabled) */
    if (timerConfig.bReloading == true) {
        nTemp |= (1u << BITP_TMR_RGB_CTL_RLD);
    } /* ENDIF */
    
    /* IF(Sync bypass is enabled) */
    if (timerConfig.bSyncBypass == true) {
        nTemp |= (1u << BITP_TMR_RGB_CTL_SYNCBYP);
    } /* ENDIF */
    
    /* Update the control register with the new configuration */
    adi_tmr_registers[eDevice]->CTL = nTemp;

    return ADI_TMR_SUCCESS;
}


/*!
 * @brief       Setup GP or RGB Timer Event Capture
 *
 * @details     The timer can be configured to capture the timer value when a specific event occurs. The
 *              list of events can be found in the hardware reference manual. The callback function specified
 *              in #adi_tmr_Init will be supplied #ADI_TMR_EVENT_CAPTURE to indicate the event occured. The 
 *              user can then read the captured value by calling #adi_tmr_GetCaptureCount.
 *
 * @note        This function can only be called when the timer is disabled.
 *
 * @param [in]  eDevice : Device number
 *
 * @param [in]  eventConfig : Event configuration structure, filled by user prior to function call
 *
 * @return      ADI_TMR_RESULT
 *                - #ADI_TMR_BAD_DEVICE_NUM        [D] Invalid eDevice parameter supplied
 *                - #ADI_TMR_BAD_EVENT_ID          [D] Event ID was not out of the valid range [0,#ADI_TMR_NUM_EVENTS]
 *                - #ADI_TMR_OPERATION_NOT_ALLOWED [D] Timer is currently running
 *                - #ADI_TMR_DEVICE_BUSY               Timer is busy processing a previous control register write
 *                - #ADI_TMR_SUCCESS                   Function call completed successfully
 *
 */
ADI_TMR_RESULT adi_tmr_ConfigEvent(ADI_TMR_DEVICE const eDevice, ADI_TMR_EVENT_CONFIG eventConfig) {
#ifdef ADI_DEBUG
    /* IF(Bad device input parameter) */
    if (eDevice >= ADI_TMR_DEVICE_NUM) {
        return ADI_TMR_BAD_DEVICE_NUM;
    } /* ENDIF */
    /* IF(Bad event input parameter) */
    if (eventConfig.nEventID >= ADI_TMR_NUM_EVENTS) {
        return ADI_TMR_BAD_EVENT_ID;
    } /* ENDIF */
    /* IF(The timer is already running) */
    if ((adi_tmr_registers[eDevice]->CTL & BITM_TMR_RGB_CTL_EN) == BITM_TMR_RGB_CTL_EN) {
        return ADI_TMR_OPERATION_NOT_ALLOWED;
    } /* ENDIF */        
#endif
    /* Set the event number */
    adi_tmr_registers[eDevice]->EVENTSELECT = (uint16_t) eventConfig.nEventID;

    /* IF(Busy bit does not clear after waiting) */
    if (ADI_TMR_SUCCESS != WaitForStatusBit(eDevice, (uint16_t) BITM_TMR_RGB_STAT_BUSY)) {
        return ADI_TMR_DEVICE_BUSY; 
    } /* ENDIF */

    /* Clear the event enable bit and keep the other bits */
    adi_tmr_registers[eDevice]->CTL &= (uint16_t) ~(BITM_TMR_RGB_CTL_EVTEN | BITM_TMR_RGB_CTL_RSTEN);

    /* IF(Turning event capture on) */
    if (eventConfig.bEnable == true) {
        adi_tmr_registers[eDevice]->CTL |= (uint16_t) BITM_TMR_RGB_CTL_EVTEN;
    } /* ENDIF */

    /* IF(Enabling reset on event capture) */
    if (eventConfig.bPrescaleReset == true) {
        adi_tmr_registers[eDevice]->CTL |= (uint16_t) BITM_TMR_RGB_CTL_RSTEN;
    } /* ENDIF */

    return ADI_TMR_SUCCESS;
}


/*!
 * @brief       Setup GP or RGB Timer Pulse Width Modulation
 *
 * @details     The timer can be configured to generate a pulse width modulation output signal.
 *              The period of this signal is simply determined by the period of timer. The duty
 *              cycle will be 50% in toggle mode, or can be configured by the user for a different
 *              value using the match value. The pulse will toggle when the timer count matches
 *              the match value. The user can also specify the polarity of the signal by choosing
 *              if the signal idles low or high. GPIO muxing will be required to use the PWM output.
 *
 * @note        This function can only be called when the timer is disabled.
 *
 * @param [in]  eDevice : Device number
 *
 * @param [in]  pwmConfig : PWM configuration structure, filled by user prior to function call
 *
 * @return      ADI_TMR_RESULT
 *                - #ADI_TMR_BAD_DEVICE_NUM          [D] Invalid eDevice parameter supplied
 *                - #ADI_TMR_OPERATION_NOT_ALLOWED   [D] Timer is currently running
 *                - #ADI_TMR_BAD_PWM_NUM             [D] Invalid eOutput parameter supplied
 *                - #ADI_TMR_SUCCESS                     Function call completed successfully
 *
 */
ADI_TMR_RESULT adi_tmr_ConfigPwm(ADI_TMR_DEVICE const eDevice, ADI_TMR_PWM_CONFIG pwmConfig) {
    uint16_t nControl = 0u;
#ifdef ADI_DEBUG
    /* IF(Bad device input parameter) */
    if (eDevice >= ADI_TMR_DEVICE_NUM) {
        return ADI_TMR_BAD_DEVICE_NUM;
    } /* ENDIF */
    /* IF(The timer is already running) */
    if ((adi_tmr_registers[eDevice]->CTL & BITM_TMR_RGB_CTL_EN) == BITM_TMR_RGB_CTL_EN) {
        return ADI_TMR_OPERATION_NOT_ALLOWED;
    } /* ENDIF */
    /* IF(Bad PWM output and device combo OR bad PWM output) */
    if (((eDevice != ADI_TMR_DEVICE_RGB) && (pwmConfig.eOutput != ADI_TMR_PWM_OUTPUT_0)) || (pwmConfig.eOutput >= ADI_TMR_PWM_OUTPUT_NUM)) {
        return ADI_TMR_BAD_PWM_NUM;
    } /* ENDIF */
#endif
    /* IF(Idle high is set) */
    if (pwmConfig.bIdleHigh == true) {
        nControl = (1u << ((uint16_t) BITP_TMR_RGB_PWM0CTL_IDLESTATE));
    } /* ENDIF */
 
    /* IF(Match mode is enabled) */
    if (pwmConfig.bMatch == true) {
        nControl |= (1u << ((uint16_t) BITP_TMR_RGB_PWM0CTL_MATCH));
    } /* ENDIF */    
    
    /* IF(PWM output 0) */
    if (pwmConfig.eOutput == ADI_TMR_PWM_OUTPUT_0) {
        adi_tmr_registers[eDevice]->PWM0CTL   = nControl;
        adi_tmr_registers[eDevice]->PWM0MATCH = pwmConfig.nMatchValue; 
    /* IF(PWM output 1) */
    } else if (pwmConfig.eOutput == ADI_TMR_PWM_OUTPUT_1) {
        adi_tmr_registers[eDevice]->PWM1CTL   = nControl;
        adi_tmr_registers[eDevice]->PWM1MATCH = pwmConfig.nMatchValue;     
    /* ELSE(PWM output 2) */
    } else {
        adi_tmr_registers[eDevice]->PWM2CTL   = nControl;
        adi_tmr_registers[eDevice]->PWM2MATCH = pwmConfig.nMatchValue;
    } /* ENDIF */

    return ADI_TMR_SUCCESS;
}


/*!
 * @brief       Enable or Disable the GP or RGB Timer
 *
 * @details     Start or stop the timer.
 *
 * @param [in]  eDevice : Device number
 *
 * @param [in]  bEnable : True to enable, false to disable
 *
 * @return      ADI_TMR_RESULT
 *                - #ADI_TMR_BAD_DEVICE_NUM [D] Invalid eDevice parameter supplied
 *                - #ADI_TMR_DEVICE_BUSY        Timer is busy processing a previous control register write
 *                - #ADI_TMR_SUCCESS            Function call completed successfully
 *
 */
ADI_TMR_RESULT adi_tmr_Enable(ADI_TMR_DEVICE const eDevice, bool bEnable) {
#ifdef ADI_DEBUG
    /* IF(Bad device input parameter) */
    if (eDevice >= ADI_TMR_DEVICE_NUM) {
        return ADI_TMR_BAD_DEVICE_NUM;
    } /* ENDIF */
#endif
    /* IF(Busy bit does not clear after waiting) */
    if (ADI_TMR_SUCCESS != WaitForStatusBit(eDevice, (uint16_t) BITM_TMR_RGB_STAT_BUSY)) {
        return ADI_TMR_DEVICE_BUSY; 
    } /* ENDIF */

    /* Clear the enable bit and keep the other bits */
    adi_tmr_registers[eDevice]->CTL &= (uint16_t) ~BITM_TMR_RGB_CTL_EN;

    /* IF(Turning the timer on) */
    if (bEnable == true) {
        adi_tmr_registers[eDevice]->CTL |= (uint16_t) BITM_TMR_RGB_CTL_EN;
    } /* ENDIF */

    return ADI_TMR_SUCCESS;
}


/*!
 * @brief       Get GP or RGB Timer Current Count
 *
 * @details     Read the timer.
 *
 * @param [in]  eDevice : Device number
 *
 * @param [out] pCount : Pointer to the result.
 *
 * @return      ADI_TMR_RESULT
 *                - #ADI_TMR_BAD_DEVICE_NUM [D]      Invalid eDevice parameter supplied
 *                - #ADI_TMR_NULL_POINTER   [D]      Invalid pCount parameter supplied
 *                - #ADI_TMR_SUCCESS                 Function call completed successfully
 *
 */
ADI_TMR_RESULT adi_tmr_GetCurrentCount(ADI_TMR_DEVICE const eDevice, uint16_t *pCount) {
#ifdef ADI_DEBUG
    /* IF(Bad device input parameter) */
    if (eDevice >= ADI_TMR_DEVICE_NUM) {
        return ADI_TMR_BAD_DEVICE_NUM;
    } /* ENDIF */
    /* IF(Null pointer) */
    if (pCount == NULL) {
        return ADI_TMR_NULL_POINTER; 
    } /* ENDIF */
#endif
    *pCount = adi_tmr_registers[eDevice]->CURCNT;
    return ADI_TMR_SUCCESS;
}


/*!
 * @brief       Get GP or RGB Timer Captured Count
 *
 * @details     Read the captured timer value. This should be called after the callback function
 *              is called with #ADI_TMR_EVENT_CAPTURE in the Event field.
 *
 * @param [in]  eDevice : Device number
 *
 * @param [out] pCount : Pointer to the result.
 *
 * @return      ADI_TMR_RESULT
 *                - #ADI_TMR_BAD_DEVICE_NUM [D]      Invalid eDevice parameter supplied
 *                - #ADI_TMR_NULL_POINTER   [D]      Invalid pCount parameter supplied
 *                - #ADI_TMR_SUCCESS                 Function call completed successfully
 *
 */
ADI_TMR_RESULT adi_tmr_GetCaptureCount(ADI_TMR_DEVICE const eDevice, uint16_t *pCount) {
#ifdef ADI_DEBUG
    /* IF(Bad device input parameter) */
    if (eDevice >= ADI_TMR_DEVICE_NUM) {
        return ADI_TMR_BAD_DEVICE_NUM;
    } /* ENDIF */
    /* IF(Null pointer) */
    if (pCount == NULL) {
        return ADI_TMR_NULL_POINTER; 
    } /* ENDIF */
#endif
    *pCount = adi_tmr_registers[eDevice]->CAPTURE;
    return ADI_TMR_SUCCESS;
}


/*!
 * @brief       Reload GP or RGB Timer
 *
 * @details     Only relevent in peridic mode and when bReloading was set to 
 *              true when configuring the timer. Calling this function will
 *              reload (i.e. reset) the timer to the LOAD value.
 *
 * @param [in]  eDevice : Device number
 *
 * @return      ADI_TMR_RESULT
 *                - #ADI_TMR_BAD_DEVICE_NUM  [D]     Invalid eDevice parameter supplied
 *                - #ADI_TMR_RELOAD_DISABLED [D]     Reloading not enabled for this timer
 *                - #ADI_TMR_DEVICE_BUSY             Reload did not take effect in time
 *                - #ADI_TMR_SUCCESS                 Function call completed successfully
 *
 */
ADI_TMR_RESULT adi_tmr_Reload(ADI_TMR_DEVICE const eDevice) {
#ifdef ADI_DEBUG
    /* IF(Bad device input parameter) */
    if (eDevice >= ADI_TMR_DEVICE_NUM) {
        return ADI_TMR_BAD_DEVICE_NUM;
    } /* ENDIF */
    /* IF(Reloading has not been enabled) */
    if ((adi_tmr_registers[eDevice]->CTL & BITM_TMR_RGB_CTL_RLD) != BITM_TMR_RGB_CTL_RLD) {
        return ADI_TMR_RELOAD_DISABLED;
    } /* ENDIF */
#endif
    /* Clear the timeout bit to cause a reload to happen */
    adi_tmr_registers[eDevice]->CLRINT = BITM_TMR_RGB_CLRINT_TIMEOUT;
    /* IF(The clear interrupt does not take effect in a reasonable amount of time) */
    if (ADI_TMR_SUCCESS != WaitForStatusBit(eDevice, (uint16_t) BITM_TMR_RGB_STAT_PDOK)) {
        return ADI_TMR_DEVICE_BUSY; 
    } /* ENDIF */    
    return ADI_TMR_SUCCESS;
}


/*********************************************************************************
                                   PRIVATE FUNCTIONS
*********************************************************************************/

 /*! \cond PRIVATE */

static ADI_TMR_RESULT WaitForStatusBit(ADI_TMR_DEVICE const eDevice, uint16_t nBusyBit) {
    /* FOR(Number of arbitrary iterations) */
    for (uint16_t i = 0u; i < 1000u; i++) { 
            /* IF(Busy bit is low) */
            if ((adi_tmr_registers[(eDevice)]->STAT & nBusyBit) == ((uint16_t) 0u)) {
                return ADI_TMR_SUCCESS;
            } /* ENDIF */
    } /* ENDFOR */
    return ADI_TMR_DEVICE_BUSY;
}

static void CommonIntHandler(ADI_TMR_DEVICE const eDevice) {
    /* Read status register */
    uint16_t IntStatus = adi_tmr_registers[eDevice]->STAT;
    /* IF(Callback function has been set) */
    if(adi_tmr_callbacks[eDevice] != NULL) {
        /* IF(Timeout interrupt occurred) */
        if((IntStatus & ((uint16_t) BITM_TMR_RGB_STAT_TIMEOUT)) != ((uint16_t) 0u)) {
            adi_tmr_callbacks[eDevice](adi_tmr_parameters[eDevice], ADI_TMR_EVENT_TIMEOUT, NULL);
        } /* ENDIF */
        /* IF(Event capture interrupt occurred) */
        if((IntStatus & ((uint16_t) BITM_TMR_RGB_STAT_CAPTURE)) != ((uint16_t) 0u)) {
            adi_tmr_callbacks[eDevice](adi_tmr_parameters[eDevice], ADI_TMR_EVENT_CAPTURE, NULL);
        } /* ENDIF */
    } /* ENDIF */
    /* Clear pending interrupt */
    adi_tmr_registers[eDevice]->CLRINT = (BITM_TMR_RGB_CLRINT_EVTCAPT | BITM_TMR_RGB_CLRINT_TIMEOUT);
}

void GP_Tmr0_Int_Handler(void) {
    ISR_PROLOG()
    CommonIntHandler(ADI_TMR_DEVICE_GP0);
    ISR_EPILOG()
}

void GP_Tmr1_Int_Handler(void) {
    ISR_PROLOG()
    CommonIntHandler(ADI_TMR_DEVICE_GP1);
    ISR_EPILOG()
}

void GP_Tmr2_Int_Handler(void) {
    ISR_PROLOG()
    CommonIntHandler(ADI_TMR_DEVICE_GP2);
    ISR_EPILOG()
}

void RGB_Tmr_Int_Handler(void) {
    ISR_PROLOG()
    CommonIntHandler(ADI_TMR_DEVICE_RGB);
    ISR_EPILOG()
}

/*! \endcond */

/*! @} */
