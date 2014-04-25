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
 
#include "fsl_pit_driver.h"
#include "fsl_clock_manager.h"
#include "fsl_interrupt_manager.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* pit source clock variable which will be updated in pit_init_module. */
uint64_t pitSourceClock;

/* Table to save PIT IRQ enum numbers defined in CMSIS files. This table is 
 * defined in fsl_pit_irq.c */
extern IRQn_Type pit_irq_ids[FSL_FEATURE_PIT_TIMER_COUNT];

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : pit_init_module
 * Description   : Initialize PIT module.
 * This function must be called before calling all the other PIT driver functions.
 * This function un-gates the PIT clock and enables the PIT module. The isRunInDebug
 * passed into function will affect all timer channels. 
 *
 *END**************************************************************************/
void pit_init_module(bool isRunInDebug)
{
    uint32_t busClock;

    /* Un-gate pit clock*/
    clock_manager_set_gate(kClockModulePIT, 0U, true);

    /* Enable PIT module clock*/
    pit_hal_enable();

    /* Set timer run or stop in debug mode*/
    pit_hal_configure_timer_run_in_debug(isRunInDebug);
   
    /* Finally, update pit source clock frequency.*/
    clock_manager_get_frequency(kBusClock, &busClock);    
    pitSourceClock = (uint64_t)busClock;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pit_init_channel
 * Description   : Initialize PIT channel.
 * This function initialize PIT timers by channel. Pass in timer number and its
 * config structure. Timers do not start counting by default after calling this
 * function. Function pit_timer_start must be called to start timer counting. 
 * Call pit_set_timer_period_us to re-set the period.
 *
 *END**************************************************************************/
void pit_init_channel(uint32_t timer, const pit_user_config_t * config)
{
    /* Set timer period.*/
    pit_set_timer_period_us(timer, config->periodUs);

    #if FSL_FEATURE_PIT_HAS_CHAIN_MODE
    /* Configure timer chained or not.*/
    pit_hal_configure_timer_chain(timer, config->isTimerChained);
    #endif

    /* Enable or disable interrupt.*/
    pit_hal_configure_interrupt(timer, config->isInterruptEnabled);

    /* Configure NVIC*/
    if (config->isInterruptEnabled)
    {
        /* Enable PIT interrupt.*/
        interrupt_enable(pit_irq_ids[timer]);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pit_shutdown 
 * Description   : Disable PIT module and gate control
 * This function will disable all PIT interrupts and PIT clock. Then gate the
 * PIT clock control. pit_init must be called in order to use PIT again.
 * 
 *END**************************************************************************/
void pit_shutdown(void)
{
    uint32_t i;

    /* Disable all PIT interrupts. */
    for (i=0; i < FSL_FEATURE_PIT_TIMER_COUNT; i++)
    {
        pit_hal_configure_interrupt(i, false);
        interrupt_disable(pit_irq_ids[i]);
    }

    /* Disable PIT module clock*/
    pit_hal_disable();

    /* Gate PIT clock control*/
    clock_manager_set_gate(kClockModulePIT, 0U, false);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pit_timer_start
 * Description   : Start timer counting.
 * After calling this function, timers load period value, count down to 0 and
 * then load the respective start value again. Each time a timer reaches 0,
 * it will generate a trigger pulse and set the timeout interrupt flag.
 * 
 *END**************************************************************************/
void pit_timer_start(uint32_t timer)
{
    pit_hal_timer_start(timer);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pit_timer_stop
 * Description   : Stop timer counting.
 * This function will stop every timer counting. Timers will reload their periods
 * respectively after calling pit_timer_start next time.
 *
 *END**************************************************************************/
void pit_timer_stop(uint32_t timer)
{
    pit_hal_timer_stop(timer);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pit_set_timer_period_us
 * Description   : Set timer period in microseconds unit.
 * The period range depends on the frequency of PIT source clock. If required
 * period is out the range, try to use lifetime timer if applicable. 
 *
 *END**************************************************************************/
void pit_set_timer_period_us(uint32_t timer, uint32_t us)
{
    /* Calculate the count value, assign it to timer counter register.*/
    uint32_t count = (uint32_t)(us * pitSourceClock / 1000000U - 1U);
    pit_hal_set_timer_period_count(timer, count);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pit_read_timer_us
 * Description   : Read current timer value in microseconds unit.
 * This function will return an absolute time stamp in the unit of microseconds.
 * One common use of this function is to measure the running time of part of
 * code. Just call this function at both the beginning and end of code, the time
 * difference between these two time stamp will be the running time (Need to 
 * make sure the running time will not exceed the timer period). Also, the time
 * stamp returned is up-counting.
 *
 *END**************************************************************************/
uint32_t pit_read_timer_us(uint32_t timer)
{
    /* Get current timer count, and reverse it to up-counting.*/
    uint64_t currentTime = (~pit_hal_read_timer_count(timer));

    /* Convert count numbers to microseconds unit.*/
    currentTime = (currentTime * 1000000U) / pitSourceClock;
    return (uint32_t)currentTime;
}

#if FSL_FEATURE_PIT_HAS_LIFETIME_TIMER
/*FUNCTION**********************************************************************
 *
 * Function Name : pit_set_lifetime_timer_period_us
 * Description   : Set lifetime timer period (Timers must be chained).
 * Timer 1 must be chained with timer 0 before using lifetime timer. The period
 * range is restricted by "period * pitSourceClock < max of an uint64_t integer",
 * or it may cause a overflow and is not able to set correct period.
 *
 *END**************************************************************************/
void pit_set_lifetime_timer_period_us(uint64_t us)
{
    uint64_t lifeTimeCount;
    
    /* Calculate the counter value.*/
    lifeTimeCount = us * pitSourceClock / 1000000U - 1U;

    /* Assign to timers.*/
    pit_hal_set_timer_period_count(0U, (uint32_t)lifeTimeCount);
    pit_hal_set_timer_period_count(1U, (uint32_t)(lifeTimeCount >> 32U));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : pit_read_lifetime_timer_us
 * Description   : Read current lifetime value in microseconds unit.
 * Return an absolute time stamp in the unit of microseconds. The time stamp 
 * value will not exceed the timer period. Also, the timer is up-counting.
 *
 *END**************************************************************************/
uint64_t pit_read_lifetime_timer_us(void)
{
    /* Get current lifetime timer count, and reverse it to up-counting.*/
    uint64_t currentTime = (~pit_hal_read_lifetime_timer_count());

    /* Convert count numbers to microseconds unit.*/
    /* Note: using currentTime * 1000 rather than 1000000 to avoid short time overflow. */
    return currentTime = (currentTime * 1000U) / (pitSourceClock / 1000U);
}
#endif /* FSL_FEATURE_PIT_HAS_LIFETIME_TIMER*/

/*******************************************************************************
 * EOF
 ******************************************************************************/

