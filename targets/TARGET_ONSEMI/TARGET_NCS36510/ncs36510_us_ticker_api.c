/**
 ******************************************************************************
 * @file us_ticker_api.h
 * @brief Implementation of a Timer driver
 * @internal
 * @author ON Semiconductor
 * $Rev:  $
 * $Date: 2015-11-15 $
 ******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup timer
*/

#include <stddef.h>
#include "timer.h"

#define US_TIMER  TIMER0
#define US_TICKER TIMER1

static int us_ticker_inited = 0;

static void us_timer_init(void);

static uint32_t us_ticker_target = 0;
static volatile uint32_t msb_counter = 0;

void us_ticker_init(void)
{
    if (!us_ticker_inited) {
        us_timer_init();
    }
}

/*******************************************************************************
 * Timer for us timing reference
 *
 * Uptime counter for scheduling reference.  It uses TIMER0.
 * The NCS36510 does not have a 32 bit timer nor the option to chain timers,
 * which is why a software timer is required to get 32-bit word length.
 ******************************************************************************/
/* TODO - Need some sort of load value/prescale calculation for non-32MHz clock */
/* TODO - Add msb_counter rollover protection at 16 bits count? */
/* TODO - How is overflow handled? */

/* Timer 0 for free running time */
extern void us_timer_isr(void)
{
    TIM0REG->CLEAR = 0;
    msb_counter++;
}

/* Initializing TIMER 0(TImer) and TIMER 1(Ticker) */
static void us_timer_init(void)
{
    /* Enable the timer0 periphery clock */
    CLOCK_ENABLE(CLOCK_TIMER0);
    /* Enable the timer0 periphery clock */
    CLOCK_ENABLE(CLOCK_TIMER1);

    /* Timer init */
    /* load timer value */
    TIM0REG->LOAD = 0xFFFF;

    /* set timer prescale 32 (1 us), mode & enable */
    TIM0REG->CONTROL.WORD = ((CLK_DIVIDER_32        << TIMER_PRESCALE_BIT_POS) |
                             (TIME_MODE_PERIODIC    << TIMER_MODE_BIT_POS) |
                             (TIMER_ENABLE_BIT      << TIMER_ENABLE_BIT_POS));

    /* Ticker init */
    /* load timer value */
    TIM1REG->LOAD = 0xFFFF;

    /* set timer prescale 32 (1 us), mode & enable */
    TIM1REG->CONTROL.WORD = ((CLK_DIVIDER_32        << TIMER_PRESCALE_BIT_POS) |
                             (TIME_MODE_PERIODIC    << TIMER_MODE_BIT_POS));

    /* Register & enable interrupt associated with the timer */
    NVIC_SetVector(Tim0_IRQn,(uint32_t)us_timer_isr);
    NVIC_SetVector(Tim1_IRQn,(uint32_t)us_ticker_isr);

    /* Clear pending irqs */
    NVIC_ClearPendingIRQ(Tim0_IRQn);
    NVIC_ClearPendingIRQ(Tim1_IRQn);

    /* Setup NVIC for timer */
    NVIC_EnableIRQ(Tim0_IRQn);
    NVIC_EnableIRQ(Tim1_IRQn);

    us_ticker_inited = 1;
}

/* Reads 32 bit timer's current value (16 bit s/w timer | 16 bit h/w timer) */
uint32_t us_ticker_read()
{
    uint32_t retval, tim0cval;

    if (!us_ticker_inited) {
        us_timer_init();
    }

    /* Get the current tick from the hw and sw timers */
    tim0cval = TIM0REG->VALUE;         /* read current time */
    retval = (0xFFFF - tim0cval);      /* subtract down count */

    NVIC_DisableIRQ(Tim0_IRQn);
    if (TIM0REG->CONTROL.BITS.INT) {
        TIM0REG->CLEAR = 0;
        msb_counter++;
        tim0cval = TIM0REG->VALUE;    /* read current time again after interrupt */
        retval = (0xFFFF - tim0cval);
    }
    retval |= msb_counter << 16;      /* add software bits */
    NVIC_EnableIRQ(Tim0_IRQn);
    return retval;
}

/*******************************************************************************
 * Event Timer
 *
 * Schedules interrupts at given (32bit)us interval of time.  It uses TIMER1.
 * The NCS36510 does not have a 32 bit timer nor the option to chain timers,
 * which is why a software timer is required to get 32-bit word length.
 *******************************************************************************/
/* TODO - Need some sort of load value/prescale calculation for non-32MHz clock */

/* TImer 1 disbale interrupt */
void us_ticker_disable_interrupt(void)
{
    /* Disable the TIMER1 interrupt */
    TIM1REG->CONTROL.BITS.ENABLE = 0x0;
}

/* TImer 1 clear interrupt */
void us_ticker_clear_interrupt(void)
{
    /* Clear the Ticker (TIMER1) interrupt */
    TIM1REG->CLEAR = 0;
}

/* Setting TImer 1 (ticker) */
inline static void ticker_set(uint32_t count)
{
    /* Disable TIMER1, load the new value, and re-enable */
    TIM1REG->CONTROL.BITS.ENABLE = 0;
    TIM1REG->LOAD = count;
    TIM1REG->CONTROL.BITS.ENABLE = 1;
}

/* TImer 1 - ticker ISR */
extern void us_ticker_isr(void)
{
    /* Clear IRQ flag */
    TIM1REG->CLEAR = 0;

    int32_t delta = us_ticker_target - us_ticker_read();
    if (delta <= 0) {
        TIM1REG->CONTROL.BITS.ENABLE = False;
        us_ticker_irq_handler();
    } else {
        // Clamp at max value of timer
        if (delta > 0xFFFF) {
            delta = 0xFFFF;
        }

        ticker_set(delta);
    }
}

/* Set timer 1 ticker interrupt */
void us_ticker_set_interrupt(timestamp_t timestamp)
{
    us_ticker_target = (uint32_t)timestamp;
    int32_t delta = us_ticker_target - us_ticker_read();

    if (delta <= 0) {
        /* This event was in the past */
        //us_ticker_irq_handler();
        // This event was in the past.
        // Set the interrupt as pending, but don't process it here.
        // This prevents a recurive loop under heavy load
        // which can lead to a stack overflow.
        NVIC_SetPendingIRQ(Tim1_IRQn);

        return;
    }

    // Clamp at max value of timer
    if (delta > 0xFFFF) {
        delta = 0xFFFF;
    }

    ticker_set(delta);
}
