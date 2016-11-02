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

#ifndef _APB_DUAL_TIMER_DRV_H
#define _APB_DUAL_TIMER_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

/* Supported Number of Dual Timers */
#define NUM_DUALTIMERS   1
#define DUALTIMER0       0
#define SINGLETIMER1     1
#define SINGLETIMER2     2
#define ALL_SINGLETIMERS 3

/*
 * DualTimer_Initialize(): Initializes a hardware timer
 * timer: timer to be Initialized
 * time_us: timer reload value in us - 0 to reload to timer max value
 *          time_us = tick_value / TIMER_TICK_US
 */
void DualTimer_Initialize(uint32_t timer, uint32_t time_us);

/* Enable Mode */
typedef uint8_t timerenable_t;
/* Interrupt */
#define DUALTIMER_INT_MASK      (0)
#define DUALTIMER_INT           (1 << DUALTIMER_INT_MASK)
/* 32 bit Counter */
#define DUALTIMER_COUNT_32_MASK (1)
#define DUALTIMER_COUNT_32      (1 << DUALTIMER_COUNT_32_MASK)
/* Periodic mode */
#define DUALTIMER_PERIODIC_MASK (2)
#define DUALTIMER_PERIODIC      (1 << DUALTIMER_PERIODIC_MASK)
/* OneShot mode */
#define DUALTIMER_ONESHOT_MASK  (3)
#define DUALTIMER_ONESHOT       (1 << DUALTIMER_ONESHOT_MASK)

/* Default reload */
#define DUALTIMER_DEFAULT_RELOAD    0xFFFFFFFF

/*
 * DualTimer_Enable(): Enables a hardware timer
 * timer: timer to be enabled
 * mode: enable mode
 */
void DualTimer_Enable(uint32_t timer, timerenable_t mode);

/*
 * DualTimer_Disable(): Disables a hardware timer
 * timer: timer to be disabled
 * dis_timer: 0 both - 1 dual timer 1 - 2 dual timer 2
 */
void DualTimer_Disable(uint32_t timer, uint32_t dis_timer);

/*
 * DualTimer_isEnabled(): verifies if a timer is enabled
 * timer: timer to be verified
 * @return: 0 disabled - 1 enabled
 */
uint32_t DualTimer_isEnabled(uint32_t timer);

/*
 * DualTimer_Read_1(): provides single timer 1 VALUE
 * timer: timer to be read
 * @return: timer VALUE us
 */
uint32_t DualTimer_Read_1(uint32_t timer);

/*
 * DualTimer_Read_2(): provides single timer 2 VALUE
 * timer: timer to be read
 * @return: timer VALUE us
 */
uint32_t DualTimer_Read_2(uint32_t timer);

/*
 * DualTimer_SetInterrupt_1(): sets timer 1 Interrupt
 * timer: timer on which interrupt is set
 * time_us: reloading value us
 * mode: enable mode
 */
void DualTimer_SetInterrupt_1(uint32_t timer, uint32_t time_us,
            timerenable_t mode);

/*
 * DualTimer_SetInterrupt_2(): sets timer 2 Interrupt
 * timer: timer on which interrupt is set
 * time_us: reloading value us
 * mode: enable mode
 */
void DualTimer_SetInterrupt_2(uint32_t timer, uint32_t time_us,
            timerenable_t mode);

/*
 * DualTimer_DisableInterrupt(): disables timer interrupts
 * dualimer: dualtimer on which interrupt is disabled
 * single_timer: single timer in the dualtimer on which
 *               interrupt is disabled
 */
void DualTimer_DisableInterrupt(uint32_t dualtimer,
                                uint32_t single_timer);

/*
 * DualTimer_ClearInterrupt(): clear timer interrupt
 * timer: timer on which interrupt needs to be cleared
 */
void DualTimer_ClearInterrupt(uint32_t timer);

/*
 * DualTimer_GetIRQn(): returns IRQn of a DualTimer
 * timer: timer on which IRQn is defined - 0 if it is not defined
 */
uint32_t DualTimer_GetIRQn(uint32_t timer);

/*
 * DualTimer_GetIRQInfo(): provides the single timer who caused
 * the interrupt.
 * timer: dualtimer that triggered the IRQ
 * @return: a single timer
 */
uint32_t DualTimer_GetIRQInfo(uint32_t dualtimer);

/*
 * DualTimer_GetTicksUS(): returns the Ticks per us
 * timer: timer associated with the Ticks per us
 * @return: Ticks per us - 0 if the timer is disables
 */
uint32_t DualTimer_GetTicksUS(uint32_t timer);

/*
 * DualTimer_GetReloadValue(): returns the load value of the selected
 * singletimer.
 * timer: timer associated with the Ticks per us
 * singletimer: selected singletimer
 * @return: reload value of the selected singletimer
 */
uint32_t DualTimer_GetReloadValue(uint32_t timer, uint32_t singletimer);

#ifdef __cplusplus
}
#endif
#endif /* _APB_DUAL_TIMER_DRV_H */
