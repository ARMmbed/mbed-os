/*! *****************************************************************************
 * @file    adi_tmr_data.c
 * @brief   GP and RGB timer static configuration data
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


#ifndef ADI_TMR_DATA
#define ADI_TMR_DATA


#include <stdlib.h>
#include <adi_tmr_config.h>
#include <drivers/tmr/adi_tmr.h>


/* CTL register static configuration */
static uint16_t aTimerCtlConfig[] = 
{
    (TMR0_CFG_COUNT_UP              << BITP_TMR_RGB_CTL_UP)      |
    (TMR0_CFG_MODE                  << BITP_TMR_RGB_CTL_MODE)    |
    (TMR0_CFG_PRESCALE_FACTOR       << BITP_TMR_RGB_CTL_PRE)     |       
    (TMR0_CFG_CLOCK_SOURCE          << BITP_TMR_RGB_CTL_CLK)     |
    (TMR0_CFG_ENABLE_RELOADING      << BITP_TMR_RGB_CTL_RLD)     |
    (TMR0_CFG_ENABLE_SYNC_BYPASS    << BITP_TMR_RGB_CTL_SYNCBYP) |
    (TMR0_CFG_ENABLE_PRESCALE_RESET << BITP_TMR_RGB_CTL_RSTEN)   |
    (TMR0_CFG_ENABLE_EVENT_CAPTURE  << BITP_TMR_RGB_CTL_EVTEN),

    (TMR1_CFG_COUNT_UP              << BITP_TMR_RGB_CTL_UP)      |
    (TMR1_CFG_MODE                  << BITP_TMR_RGB_CTL_MODE)    |
    (TMR1_CFG_PRESCALE_FACTOR       << BITP_TMR_RGB_CTL_PRE)     |       
    (TMR1_CFG_CLOCK_SOURCE          << BITP_TMR_RGB_CTL_CLK)     |
    (TMR1_CFG_ENABLE_RELOADING      << BITP_TMR_RGB_CTL_RLD)     |
    (TMR1_CFG_ENABLE_SYNC_BYPASS    << BITP_TMR_RGB_CTL_SYNCBYP) |
    (TMR1_CFG_ENABLE_PRESCALE_RESET << BITP_TMR_RGB_CTL_RSTEN)   |
    (TMR1_CFG_ENABLE_EVENT_CAPTURE  << BITP_TMR_RGB_CTL_EVTEN),

    (TMR2_CFG_COUNT_UP              << BITP_TMR_RGB_CTL_UP)      |
    (TMR2_CFG_MODE                  << BITP_TMR_RGB_CTL_MODE)    |
    (TMR2_CFG_PRESCALE_FACTOR       << BITP_TMR_RGB_CTL_PRE)     |       
    (TMR2_CFG_CLOCK_SOURCE          << BITP_TMR_RGB_CTL_CLK)     |
    (TMR2_CFG_ENABLE_RELOADING      << BITP_TMR_RGB_CTL_RLD)     |
    (TMR2_CFG_ENABLE_SYNC_BYPASS    << BITP_TMR_RGB_CTL_SYNCBYP) |
    (TMR2_CFG_ENABLE_PRESCALE_RESET << BITP_TMR_RGB_CTL_RSTEN)   |
    (TMR2_CFG_ENABLE_EVENT_CAPTURE  << BITP_TMR_RGB_CTL_EVTEN),

    (TMR3_CFG_COUNT_UP              << BITP_TMR_RGB_CTL_UP)      |
    (TMR3_CFG_MODE                  << BITP_TMR_RGB_CTL_MODE)    |
    (TMR3_CFG_PRESCALE_FACTOR       << BITP_TMR_RGB_CTL_PRE)     |       
    (TMR3_CFG_CLOCK_SOURCE          << BITP_TMR_RGB_CTL_CLK)     |
    (TMR3_CFG_ENABLE_RELOADING      << BITP_TMR_RGB_CTL_RLD)     |
    (TMR3_CFG_ENABLE_SYNC_BYPASS    << BITP_TMR_RGB_CTL_SYNCBYP) |
    (TMR3_CFG_ENABLE_PRESCALE_RESET << BITP_TMR_RGB_CTL_RSTEN)   |
    (TMR3_CFG_ENABLE_EVENT_CAPTURE  << BITP_TMR_RGB_CTL_EVTEN),      
};

/* LOAD register static configuration */
static uint16_t aTimerLoadConfig[] = 
{
    TMR0_CFG_LOAD_VALUE,
    TMR1_CFG_LOAD_VALUE,
    TMR2_CFG_LOAD_VALUE,
    TMR3_CFG_LOAD_VALUE,
};

/* Asynchronous LOAD static configuraton */
static uint16_t aTimerALoadConfig[] = 
{
    TMR0_CFG_ASYNC_LOAD_VALUE,
    TMR1_CFG_ASYNC_LOAD_VALUE,
    TMR2_CFG_ASYNC_LOAD_VALUE,
    TMR3_CFG_ASYNC_LOAD_VALUE,
};

/* EVENTSELECT static configuration */
static uint16_t aTimerEventConfig[] =
{
    TMR0_CFG_EVENT_CAPTURE,
    TMR1_CFG_EVENT_CAPTURE,
    TMR2_CFG_EVENT_CAPTURE,
    TMR3_CFG_EVENT_CAPTURE,
};

/* PWM CTL static configuration */
static uint16_t aTimerPwmCtlConfig[] =
{
  (TMR0_CFG_PWM0_IDLE_STATE  << BITP_TMR_RGB_PWM0CTL_IDLESTATE) |
  (TMR0_CFG_PWM0_MATCH_VALUE << BITP_TMR_RGB_PWM0CTL_MATCH),

  (TMR1_CFG_PWM0_IDLE_STATE  << BITP_TMR_RGB_PWM0CTL_IDLESTATE) |
  (TMR1_CFG_PWM0_MATCH_VALUE << BITP_TMR_RGB_PWM0CTL_MATCH),

  (TMR2_CFG_PWM0_IDLE_STATE  << BITP_TMR_RGB_PWM0CTL_IDLESTATE) |
  (TMR2_CFG_PWM0_MATCH_VALUE << BITP_TMR_RGB_PWM0CTL_MATCH),

  (TMR3_CFG_PWM0_IDLE_STATE  << BITP_TMR_RGB_PWM0CTL_IDLESTATE) |
  (TMR3_CFG_PWM0_MATCH_VALUE << BITP_TMR_RGB_PWM0CTL_MATCH),

  (TMR3_CFG_PWM1_IDLE_STATE  << BITP_TMR_RGB_PWM1CTL_IDLESTATE) |
  (TMR3_CFG_PWM1_MATCH_VALUE << BITP_TMR_RGB_PWM1CTL_MATCH),        

  (TMR3_CFG_PWM2_IDLE_STATE  << BITP_TMR_RGB_PWM2CTL_IDLESTATE) |
  (TMR3_CFG_PWM2_MATCH_VALUE << BITP_TMR_RGB_PWM2CTL_MATCH),     
};

/* PWM MATCH static configuration */
static uint16_t aTimerPwmMatchConfig[] = {
  TMR0_CFG_PWM0_MATCH_VALUE,
  TMR1_CFG_PWM0_MATCH_VALUE,
  TMR2_CFG_PWM0_MATCH_VALUE,
  TMR3_CFG_PWM0_MATCH_VALUE,
  TMR3_CFG_PWM1_MATCH_VALUE,
  TMR3_CFG_PWM2_MATCH_VALUE
};


#endif /* ADI_TMR_DATA */
