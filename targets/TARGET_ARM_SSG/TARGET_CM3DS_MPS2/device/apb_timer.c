/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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
 */
#include "cmsis.h"
#include "apb_timer.h"

/* Timer Private Data */
typedef struct {
    /* Timer Definition */
    CMSDK_TIMER_TypeDef *timerN;
    /* Timer IRQn */
    uint32_t timerIRQn;
    /* Timer Reload Value */
    uint32_t timerReload;
    /* Timer state */
    uint32_t state;
} apb_timer_t;

/* Timer state definitions */
#define TIMER_INITIALIZED   (1)
#define TIMER_ENABLED       (1 << 1)

/*
 * This Timer is written for MBED OS and keeps count
 * of the ticks. All the elaboration logic is demanded
 * to the upper layers.
 */
#define TIMER_MAX_VALUE     0xFFFFFFFF
#define TIMER_TICKS_US      (SystemCoreClock/1000000)

/* Timers Array */
static apb_timer_t Timers[NUM_TIMERS];

void Timer_Index_Init(uint32_t timer, uint32_t reload,
    CMSDK_TIMER_TypeDef *TimerN, uint32_t IRQn)
{
    Timers[timer].timerN = TimerN;
    Timers[timer].timerIRQn = IRQn;
    Timers[timer].timerReload = reload;
    Timers[timer].state = TIMER_INITIALIZED;
}

/*
 * Timer_Initialize(): Initializes an hardware timer
 * timer: timer to be Initialized
 * time_us: timer reload value in us - 0 to reload to timer max value
 *          time_us = tick_value / TIMER_TICKS_US
 */
#define TIMER_INIT(index, reload) Timer_Index_Init(index, reload, CMSDK_TIMER##index, TIMER##index##_IRQn)
void Timer_Initialize(uint32_t timer, uint32_t time_us)
{
    uint32_t reload = 0;

    if (timer < NUM_TIMERS) {
        if (time_us == 0) {
            reload = TIMER_MAX_VALUE;
        } else {
            reload = (time_us) * TIMER_TICKS_US;
        }
        switch (timer) {
            case 0:
                TIMER_INIT(0, reload);
                break;
            case 1:
                TIMER_INIT(1, reload);
                break;
            default:
                break;
        }
    }
}

/*
 * Timer_Enable(): Enables a hardware timer
 * timer: timer to be enabled
 */
void Timer_Enable(uint32_t timer)
{
    /* The timer has to be contained in a valid range */
    if (timer < NUM_TIMERS) {
        /* Timer has to be already initialized */
        if (Timers[timer].state == TIMER_INITIALIZED) {
            /* Disable Timer */
            (Timers[timer].timerN)->CTRL = 0x0;
            /* Reload Value */
            (Timers[timer].timerN)->RELOAD = Timers[timer].timerReload;
            /* Enable Interrupt */
            (Timers[timer].timerN)->CTRL = CMSDK_TIMER_CTRL_IRQEN_Msk;
            /* Enable Counter */
            (Timers[timer].timerN)->CTRL |= CMSDK_TIMER_CTRL_EN_Msk;
            /* Change timer state */
            Timers[timer].state |= TIMER_ENABLED;
        }
    }
}

/*
 * Timer_Disable(): Disables a hardware timer
 * timer: timer to be disabled
 */
void Timer_Disable(uint32_t timer)
{
    /* The timer has to be contained in a valid range */
    if (timer < NUM_TIMERS) {
        /* Timer has to be already initialized and enabled */
        if (Timers[timer].state == (TIMER_INITIALIZED | TIMER_ENABLED)) {
            /* Disable Timer */
            (Timers[timer].timerN)->CTRL = 0x0;
            /* Change timer state */
            Timers[timer].state = TIMER_INITIALIZED;
        }
    }
}

/*
 * Timer_isEnabled(): verifies if a timer is enabled
 * timer: timer to be verified
 * @return: 0 disabled - 1 enabled
 */
uint32_t Timer_isEnabled(uint32_t timer)
{
    /* The timer has to be contained in a valid range */
    if (timer < NUM_TIMERS) {
        /* Timer has to be already initialized and enabled */
        if (Timers[timer].state == (TIMER_INITIALIZED | TIMER_ENABLED)) {
            return 1;
        }
    }

    return 0;
}

/*
 * Timer_Read(): provides timer VALUE
 * timer: timer to be read
 * @return: timer VALUE us
 */
uint32_t Timer_Read(uint32_t timer)
{
    uint32_t return_value = 0;
    /* Verify if the Timer is enabled */
    if (Timer_isEnabled(timer) == 1) {
        return_value = (Timers[timer].timerReload
                        - (Timers[timer].timerN)->VALUE) / TIMER_TICKS_US;
    }

    return return_value;
}

/*
 * Timer_SetInterrupt(): sets timer Interrupt
 * timer: timer on which interrupt is set
 * time_us: reloading time in us
 */
void Timer_SetInterrupt(uint32_t timer, uint32_t time_us)
{
    uint32_t load_time_us = 0;
    /* Verify if the Timer is enabled */
    if (Timer_isEnabled(timer) == 1) {
        /* Disable Timer */
        Timer_Disable(timer);
        /* Enable Interrupt */
        (Timers[timer].timerN)->CTRL = CMSDK_TIMER_CTRL_IRQEN_Msk;

        /* Check time us condition */
        if (time_us == TIMER_DEFAULT_RELOAD) {
            load_time_us = TIMER_MAX_VALUE;
        } else {
            load_time_us = time_us * TIMER_TICKS_US;
        }

        /* Initialize Timer Value */
        Timers[timer].timerReload = load_time_us;
        (Timers[timer].timerN)->RELOAD = Timers[timer].timerReload;
        (Timers[timer].timerN)->VALUE = Timers[timer].timerReload;
        /* Enable Counter */
        (Timers[timer].timerN)->CTRL |= CMSDK_TIMER_CTRL_EN_Msk;
        /* Change timer state */
        Timers[timer].state |= TIMER_ENABLED;
    }
}

/*
 * Timer_DisableInterrupt(): disables timer interrupt
 * timer: timer on which interrupt is disabled
 */
void Timer_DisableInterrupt(uint32_t timer)
{
    /* Verify if the Timer is enabled */
    if (Timer_isEnabled(timer) == 1) {
        /* Disable Interrupt */
        (Timers[timer].timerN)->CTRL &= CMSDK_TIMER_CTRL_EN_Msk;
    }
}

/*
 * Timer_ClearInterrupt(): clear timer interrupt
 * timer: timer on which interrupt needs to be cleared
 */
void Timer_ClearInterrupt(uint32_t timer)
{
    /* Verify if the Timer is enabled */
    if (Timer_isEnabled(timer) == 1) {
        /* Clear Interrupt */
        (Timers[timer].timerN)->INTCLEAR = CMSDK_TIMER_INTCLEAR_Msk;
    }
}

/*
 * Timer_GetIRQn(): returns IRQn of a Timer
 * timer: timer on which IRQn is defined - 0 if it is not defined
 */
uint32_t Timer_GetIRQn(uint32_t timer)
{
    /* Verify if the Timer is enabled */
    if (Timer_isEnabled(timer) == 1) {
        return Timers[timer].timerIRQn;
    }
    return 0;
}

/*
 * Timer_GetTicksUS(): returns the number of Ticks per us
 * timer: timer associated with the Ticks per us
 * @return: Ticks per us - 0 if the timer is disables
 */
uint32_t Timer_GetTicksUS(uint32_t timer)
{
    /* Verify if the Timer is enabled */
    if (Timer_isEnabled(timer) == 1) {
        return TIMER_TICKS_US;
    }
    return 0;
}

/*
 * Timer_GetReloadValue(): returns the load value of the selected
 * timer.
 * timer: timer associated with the Ticks per us
 * @return: reload value of the selected singletimer
 */
uint32_t Timer_GetReloadValue(uint32_t timer)
{
    /* Verify if the Timer is enabled */
    if (Timer_isEnabled(timer) == 1) {
        if (timer == TIMER1) {
            return Timers[timer].timerReload / TIMER_TICKS_US;
        } else {
            return Timers[timer].timerReload / TIMER_TICKS_US;
        }
    }
    return 0;
}
