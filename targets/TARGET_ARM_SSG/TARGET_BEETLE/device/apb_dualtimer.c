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
#include "apb_dualtimer.h"

/* DualTimer Private Data */
typedef struct {
    /* DualTimer 1 Definition */
    CMSDK_DUALTIMER_SINGLE_TypeDef *dualtimer1;
    /* DualTimer 2 Definition */
    CMSDK_DUALTIMER_SINGLE_TypeDef *dualtimer2;
    /* Dual Timer IRQn */
    uint32_t dualtimerIRQn;
    /* DualTimer 1 Reload Value */
    uint32_t dualtimer1Reload;
    /* DualTimer 2 Reload Value */
    uint32_t dualtimer2Reload;
    /* Timer state */
    uint32_t state;
} apb_dualtimer_t;

/* Timer state definitions */
#define DUALTIMER_INITIALIZED   (1)
#define DUALTIMER_ENABLED       (1 << 1)

/*
 * This Timer is written for MBED OS and keeps count
 * of the ticks. All the elaboration logic is demanded
 * to the upper layers.
 */
#define DUALTIMER_MAX_VALUE     0xFFFFFFFF
#define DUALTIMER_TICKS_US      (SystemCoreClock/1000000)

/* Dual Timers Array */
static apb_dualtimer_t DualTimers[NUM_DUALTIMERS];

/*
 * DualTimer_Initialize(): Initializes a hardware timer
 * timer: timer to be Initialized
 * time_us: timer reload value in us - 0 to reload to timer max value
 *          time_us = ticks_value / TIMER_TICK_US
 */
void DualTimer_Initialize(uint32_t timer, uint32_t time_us)
{
    uint32_t reload = 0;

    if (timer < NUM_DUALTIMERS)
    {
        if (time_us == 0)
            reload = DUALTIMER_MAX_VALUE;
        else
            reload = (time_us) * DUALTIMER_TICKS_US;

        switch(timer) {
            case 0: DualTimers[timer].dualtimer1 = CMSDK_DUALTIMER1;
                    DualTimers[timer].dualtimer2 = CMSDK_DUALTIMER2;
                    DualTimers[timer].dualtimerIRQn = DUALTIMER_IRQn;
                    DualTimers[timer].dualtimer1Reload = reload;
                    DualTimers[timer].dualtimer2Reload = reload;
                    DualTimers[timer].state = DUALTIMER_INITIALIZED;
            default: break;
        }
    }
}

/*
 * DualTimer_ReturnMode(): returns the correct mode for Dual Timer Control
 * mode: mode set by user
 * @return: mode for TimeControl register
 */
uint32_t DualTimer_ReturnMode(timerenable_t mode)
{
    uint32_t return_mode = 0;
    /* Check Interrupt Enable */
    if (((mode & DUALTIMER_INT) >> DUALTIMER_INT_MASK) == 1)
        return_mode |= CMSDK_DUALTIMER_CTRL_INTEN_Msk;
    /* Check 32 bit Counter */
    if (((mode & DUALTIMER_COUNT_32) >> DUALTIMER_COUNT_32_MASK) == 1)
        return_mode |= CMSDK_DUALTIMER_CTRL_SIZE_Msk;
    /* Check Periodic Mode */
    if (((mode & DUALTIMER_PERIODIC) >> DUALTIMER_PERIODIC_MASK) == 1)
        return_mode |= CMSDK_DUALTIMER_CTRL_MODE_Msk;
    /* Check OneShot Mode */
    if (((mode & DUALTIMER_ONESHOT) >> DUALTIMER_ONESHOT_MASK) == 1)
        return_mode |= CMSDK_DUALTIMER_CTRL_ONESHOOT_Msk;

    return return_mode;
}

/*
 * DualTimer_Enable(): Enables a hardware timer
 * timer: timer to be enabled
 * mode: enable mode
 */
void DualTimer_Enable(uint32_t timer, timerenable_t mode)
{
    uint32_t dualtimerControl = 0;
    /* The timer has to be contained in a valid range */
    if (timer < NUM_DUALTIMERS) {
        /* Timer has to be already initialized */
        if (DualTimers[timer].state == DUALTIMER_INITIALIZED) {
            /* Disable Timer */
            (DualTimers[timer].dualtimer1)->TimerControl = 0x0;
            (DualTimers[timer].dualtimer2)->TimerControl = 0x0;
            /* Reload Value */
            (DualTimers[timer].dualtimer1)->TimerLoad =
                                            DualTimers[timer].dualtimer1Reload;
            (DualTimers[timer].dualtimer2)->TimerLoad =
                                            DualTimers[timer].dualtimer2Reload;
            /* Set up Dual Timer Control */
            dualtimerControl = DualTimer_ReturnMode(mode);
            (DualTimers[timer].dualtimer1)->TimerControl = dualtimerControl;
            (DualTimers[timer].dualtimer2)->TimerControl = dualtimerControl;
            /* Enable Counter */
            (DualTimers[timer].dualtimer1)->TimerControl |=
                                    CMSDK_DUALTIMER_CTRL_EN_Msk;
            (DualTimers[timer].dualtimer2)->TimerControl |=
                                    CMSDK_DUALTIMER_CTRL_EN_Msk;
            /* Change timer state */
            DualTimers[timer].state |= DUALTIMER_ENABLED;
        }
    }
}

/*
 * DualTimer_Disable(): Disables a hardware timer
 * timer: timer to be disabled
 * dis_timer: 0 both - 1 dual timer 1 - 2 dual timer 2
 */
void DualTimer_Disable(uint32_t timer, uint32_t dis_timer)
{
    /* The timer has to be contained in a valid range */
    if (timer < NUM_DUALTIMERS) {
        /* Timer has to be already initialized and enabled */
        if (DualTimers[timer].state == (DUALTIMER_INITIALIZED | DUALTIMER_ENABLED)) {
            /* Disable Timer */
            switch (dis_timer)
            {
                case 0: (DualTimers[timer].dualtimer1)->TimerControl = 0x0;
                        (DualTimers[timer].dualtimer2)->TimerControl = 0x0;
                        break;
                case 1: (DualTimers[timer].dualtimer1)->TimerControl = 0x0;
                        break;
                case 2: (DualTimers[timer].dualtimer2)->TimerControl = 0x0;
                        break;
                default: break;
            }
            /* Change timer state */
            DualTimers[timer].state = DUALTIMER_INITIALIZED;
        }
    }
}

/*
 * DualTimer_isEnabled(): verifies if a timer is enabled
 * timer: timer to be verified
 * @return: 0 disabled - 1 enabled
 */
uint32_t DualTimer_isEnabled(uint32_t timer)
{
    /* The timer has to be contained in a valid range */
    if (timer < NUM_DUALTIMERS) {
        /* Timer has to be already initialized and enabled */
        if (DualTimers[timer].state == (DUALTIMER_INITIALIZED | DUALTIMER_ENABLED))
            return 1;
    } else {
        return 0;
    }
    return 0;
}

/*
 * DualTimer_Read_1(): provides single timer 1 VALUE
 * timer: timer to be read
 * @return: timer VALUE
 */
uint32_t DualTimer_Read_1(uint32_t timer)
{
    uint32_t return_value = 0;
    /* Verify if the Timer is enabled */
    if (DualTimer_isEnabled(timer) == 1) {
        return_value = (DualTimers[timer].dualtimer1Reload
                    - (DualTimers[timer].dualtimer1)->TimerValue)
                    / DUALTIMER_TICKS_US;
    }

    return return_value;
}

/*
 * DualTimer_Read_2(): provides single timer 2 VALUE
 * timer: timer to be read
 * @return: timer VALUE
 */
uint32_t DualTimer_Read_2(uint32_t timer)
{
    uint32_t return_value = 0;
    /* Verify if the Timer is enabled */
    if (DualTimer_isEnabled(timer) == 1) {
        return_value = (DualTimers[timer].dualtimer2Reload
                    - (DualTimers[timer].dualtimer2)->TimerValue)
                    / DUALTIMER_TICKS_US;
    }

    return return_value;
}

/*
 * DualTimer_SetInterrupt_1(): sets timer 1 Interrupt
 * timer: timer on which interrupt is set
 * time_us: reloading value us
 * mode: enable mode
 */
void DualTimer_SetInterrupt_1(uint32_t timer, uint32_t time_us,
            timerenable_t mode)
{
    uint32_t dualtimerControl = 0;
    uint32_t load_time_us = 0;
    /* Verify if the Timer is enabled */
    if (DualTimer_isEnabled(timer) == 1) {
        /* Disable Timer */
        DualTimer_Disable(timer, SINGLETIMER1);
        /* Set up Dual Timer Control */
        dualtimerControl = DualTimer_ReturnMode(mode);
        (DualTimers[timer].dualtimer1)->TimerControl =
                                CMSDK_DUALTIMER_CTRL_INTEN_Msk
                                | dualtimerControl;

        /* Check time us condition */
        if(time_us == DUALTIMER_DEFAULT_RELOAD)
            load_time_us = DUALTIMER_MAX_VALUE;
        else
            load_time_us = time_us * DUALTIMER_TICKS_US;

        /* Reload Value */
        DualTimers[timer].dualtimer1Reload = load_time_us;
        (DualTimers[timer].dualtimer1)->TimerLoad =
                                DualTimers[timer].dualtimer1Reload;
        /* Enable Counter */
        (DualTimers[timer].dualtimer1)->TimerControl |=
                                CMSDK_DUALTIMER_CTRL_EN_Msk;
        /* Change timer state */
        DualTimers[timer].state |= DUALTIMER_ENABLED;
    }
}

/*
 * DualTimer_SetInterrupt_2(): sets timer 2 Interrupt
 * timer: timer on which interrupt is set
 * time_us: reloading value us
 * mode: enable mode
 */
void DualTimer_SetInterrupt_2(uint32_t timer, uint32_t time_us,
            timerenable_t mode)
{
    uint32_t dualtimerControl = 0;
    uint32_t load_time_us = 0;
    /* Verify if the Timer is enabled */
    if (DualTimer_isEnabled(timer) == 1) {
        /* Disable Timer */
        DualTimer_Disable(timer, SINGLETIMER2);
        /* Set up Dual Timer Control */
        dualtimerControl = DualTimer_ReturnMode(mode);
        (DualTimers[timer].dualtimer2)->TimerControl =
                                CMSDK_DUALTIMER_CTRL_INTEN_Msk
                                | dualtimerControl;

        /* Check time us condition */
        if(time_us == DUALTIMER_DEFAULT_RELOAD)
            load_time_us = DUALTIMER_MAX_VALUE;
        else
            load_time_us = time_us * DUALTIMER_TICKS_US;

        /* Reload Value */
        DualTimers[timer].dualtimer2Reload = load_time_us;
        (DualTimers[timer].dualtimer2)->TimerLoad =
                                DualTimers[timer].dualtimer2Reload;
        /* Enable Counter */
        (DualTimers[timer].dualtimer2)->TimerControl |=
                                CMSDK_DUALTIMER_CTRL_EN_Msk;
        /* Change timer state */
        DualTimers[timer].state |= DUALTIMER_ENABLED;
    }
}

/*
 * DualTimer_DisableInterrupt(): disables timer interrupts
 * dualimer: dualtimer on which interrupt is disabled
 * single_timer: single timer in the dualtimer on which
 *               interrupt is disabled
 */
void DualTimer_DisableInterrupt(uint32_t dualtimer,
                                uint32_t single_timer)
{
    /* Verify if the Timer is enabled */
    if (DualTimer_isEnabled(dualtimer) == 1) {
        switch(single_timer) {
            case SINGLETIMER1:
                /* Disable Interrupt for single timer 1 */
                (DualTimers[dualtimer].dualtimer1)->TimerControl &=
                                       CMSDK_DUALTIMER_CTRL_EN_Msk;
                break;
            case SINGLETIMER2:
                /* Disable Interrupt for single timer 2 */
                (DualTimers[dualtimer].dualtimer2)->TimerControl &=
                                       CMSDK_DUALTIMER_CTRL_EN_Msk;
                break;
            case ALL_SINGLETIMERS:
                /* Disable Interrupt for single timer 1 */
                (DualTimers[dualtimer].dualtimer1)->TimerControl &=
                                       CMSDK_DUALTIMER_CTRL_EN_Msk;
                /* Disable Interrupt for single timer 2 */
                (DualTimers[dualtimer].dualtimer2)->TimerControl &=
                                       CMSDK_DUALTIMER_CTRL_EN_Msk;
                break;
            default:
                break;
        }
    }
}

/*
 * DualTimer_ClearInterrupt(): clear timer interrupt
 * timer: timer on which interrupt needs to be cleared
 */
void DualTimer_ClearInterrupt(uint32_t timer)
{
    /* Verify if the Timer is enabled */
    if (DualTimer_isEnabled(timer) == 1) {
        /* Clear Interrupt */
        (DualTimers[timer].dualtimer1)->TimerIntClr =
                                CMSDK_DUALTIMER_INTCLR_Msk;
        (DualTimers[timer].dualtimer2)->TimerIntClr =
                                CMSDK_DUALTIMER_INTCLR_Msk;
    }
}

/*
 * DualTimer_GetIRQn(): returns IRQn of a DualTimer
 * timer: timer on which IRQn is defined - 0 if it is not defined
 */
uint32_t DualTimer_GetIRQn(uint32_t timer)
{
    /* Verify if the Timer is enabled */
    if (DualTimer_isEnabled(timer) == 1) {
        return DualTimers[timer].dualtimerIRQn;
    }
    return 0;
}

/*
 * DualTimer_GetIRQInfo(): provides the single timer who caused
 * the interrupt.
 * dualtimer: dualtimer that triggered the IRQ
 * @return: a single timer - 0 if it is not defined
 */
uint32_t DualTimer_GetIRQInfo(uint32_t timer)
{
    /* Verify if the Timer is enabled */
    if (DualTimer_isEnabled(timer) == 1) {
        if((DualTimers[timer].dualtimer1)->TimerRIS)
            return SINGLETIMER1;
        else
            return SINGLETIMER2;
    }
    return 0;
}

/*
 * DualTimer_GetTicksUS(): returns the Ticks per us
 * timer: timer associated with the Ticks per us
 * @return: Ticks per us - 0 if the timer is disables
 */
uint32_t DualTimer_GetTicksUS(uint32_t timer)
{
    /* Verify if the Timer is enabled */
    if (DualTimer_isEnabled(timer) == 1) {
        return DUALTIMER_TICKS_US;
    }
    return 0;
}

/*
 * DualTimer_GetReloadValue(): returns the load value of the selected
 * singletimer.
 * timer: timer associated with the Ticks per us
 * singletimer: selected singletimer
 * @return: reload value of the selected singletimer - 0 if timer is disabled
 */
uint32_t DualTimer_GetReloadValue(uint32_t timer, uint32_t singletimer)
{
    /* Verify if the Timer is enabled */
    if (DualTimer_isEnabled(timer) == 1) {
        if (singletimer == SINGLETIMER1)
            return DualTimers[timer].dualtimer1Reload / DUALTIMER_TICKS_US;
        else
            return DualTimers[timer].dualtimer2Reload / DUALTIMER_TICKS_US;
    }
    return 0;
}
