/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2014, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#include "sleep_ex_api.h"
#include "cmsis.h"

extern VOID SleepCG(u8  Option, u32 SDuration, u8 ClkSourceEn, u8 SDREn);
extern VOID DeepStandby(u8  Option, u32 SDuration, u8  GpioOption);
extern VOID DeepSleep(u8  Option, u32 SDuration);

SLEEP_WAKEUP_EVENT DStandbyWakeupEvent={0};

/**
  * @brief  To make the system entering the Clock Gated power saving.
  *         This function just make the system to enter the clock gated 
  *         power saving mode and pending on wake up event waitting.
  *         The user application need to configure the peripheral to 
  *         generate system wake up event, like GPIO interrupt
  *         , G-Timer timeout, etc. befor entering power saving mode.
  *
  * @param  wakeup_event: A bit map of wake up event. Available event:
  *                         SLEEP_WAKEUP_BY_STIMER
  *                         SLEEP_WAKEUP_BY_GTIMER
  *                         SLEEP_WAKEUP_BY_GPIO_INT
  *                         SLEEP_WAKEUP_BY_WLAN
  *                         SLEEP_WAKEUP_BY_NFC
  *                         SLEEP_WAKEUP_BY_SDIO
  *                         SLEEP_WAKEUP_BY_USB
  *         sleep_duration: the system sleep duration in ms, only valid
  *         for SLEEP_WAKEUP_BY_STIMER wake up event.
  *
  * @retval None
  */
void sleep_ex(uint32_t wakeup_event,  uint32_t sleep_duration)
{
    u8 wake_ev=0;

    wake_ev = wakeup_event & 0xff;
    
    if (sleep_duration == 0) {
        wake_ev &= ~SLP_STIMER;
    }

    if (wake_ev == 0) {
        // error: No wakeup event, skip the entering sleep mode
        return;
    }
    SleepCG(wake_ev, sleep_duration, 0, 0); // same as old configuration: SCLK off & SDR no power off 
}

/**
  * @brief  To make the system entering the Clock Gated power saving.
  *         This function just make the system to enter the clock gated 
  *         power saving mode and pending on wake up event waitting.
  *         The user application need to configure the peripheral to 
  *         generate system wake up event, like GPIO interrupt
  *         , G-Timer timeout, etc. befor entering power saving mode.
  *
  * @param  wakeup_event: A bit map of wake up event. Available event:
  *                         SLEEP_WAKEUP_BY_STIMER
  *                         SLEEP_WAKEUP_BY_GTIMER
  *                         SLEEP_WAKEUP_BY_GPIO_INT
  *                         SLEEP_WAKEUP_BY_WLAN
  *                         SLEEP_WAKEUP_BY_NFC
  *                         SLEEP_WAKEUP_BY_SDIO
  *                         SLEEP_WAKEUP_BY_USB
  *         sleep_duration: the system sleep duration in ms, only valid
  *         for SLEEP_WAKEUP_BY_STIMER wake up event.
  *         clk_sourec_enable: the option for SCLK on(1)/off(0)
  *         sdr_enable: the option for turn off the SDR controller (1:off, 0:on)
  *
  * @retval None
  */
void sleep_ex_selective(uint32_t wakeup_event,  uint32_t sleep_duration, uint32_t clk_sourec_enable, uint32_t sdr_enable)
{
    u8 wake_ev=0;
    u8 sdr_en=0;
    u8 clk_source_en=0;

    wake_ev = wakeup_event & 0xff;
    sdr_en = sdr_enable & 0xff;
    clk_source_en = clk_sourec_enable & 0xff;
    
    if (sleep_duration == 0) {
        wake_ev &= ~SLP_STIMER;
    }

    if (wake_ev == 0) {
        // error: No wakeup event, skip the entering sleep mode
        return;
    }
    SleepCG(wake_ev, sleep_duration, clk_source_en, sdr_en);
}

/**
  * @brief  To add a wake up event to wake up the system from the
  *         deep standby power saving mode.
  *
  * @param  wakeup_event: A bit map of wake up event. Available event:
  *                         STANDBY_WAKEUP_BY_STIMER
  *                         STANDBY_WAKEUP_BY_NFC
  *                         STANDBY_WAKEUP_BY_PA5 (GPIO)
  *                         STANDBY_WAKEUP_BY_PC7 (GPIO)
  *                         STANDBY_WAKEUP_BY_PD5 (GPIO)
  *                         STANDBY_WAKEUP_BY_PE3 (GPIO)
  *         sleep_duration_ms: the system sleep duration in ms, only valid
  *             for STANDBY_WAKEUP_BY_STIMER wake up event.
  *         gpio_active: for a GPIO pin to wake up the system by 
  *             goes high(1) or low(0)
  *
  * @retval None
  */
void standby_wakeup_event_add(uint32_t wakeup_event, uint32_t sleep_duration_ms, uint32_t gpio_active)
{
    u32 i;
    u8 gpio_event;
    u8 gpio_en;
    u8 gpio_act;
    
    if (wakeup_event & STANDBY_WAKEUP_BY_STIMER) {
        DStandbyWakeupEvent.wakeup_event |= DSTBY_STIMER;
        DStandbyWakeupEvent.timer_duration = sleep_duration_ms;
    }

#if 0
    if (wakeup_event & STANDBY_WAKEUP_BY_DS_TIMER) {
        DStandbyWakeupEvent.wakeup_event |= DSTBY_TIMER33;
        // TODO: Sleep Duration ?
    }
#endif

    if (wakeup_event & STANDBY_WAKEUP_BY_NFC) {
        DStandbyWakeupEvent.wakeup_event |= DSTBY_NFC;
    }

    gpio_event = STANDBY_WAKEUP_BY_PA5;
    gpio_en = BIT0;
    gpio_act = BIT4;
    // Loop 4 to check 4 GPIO wake up event
    for (i=0;i<4;i++) {
        if (wakeup_event & gpio_event) {
            DStandbyWakeupEvent.wakeup_event |= DSTBY_GPIO;
            DStandbyWakeupEvent.gpio_option |= gpio_en;
            if (gpio_active) {
                // Active High
                DStandbyWakeupEvent.gpio_option |= gpio_act;
            }
            else {
                // Active Low
                DStandbyWakeupEvent.gpio_option &= ~gpio_act;
            }
        }
        gpio_event = gpio_event << 1;
        gpio_en = gpio_en << 1;
        gpio_act = gpio_act << 1;
    }
}

/**
  * @brief  To delete a wake up event for wakeing up the system from the
  *         deep standby power saving mode.
  *
  * @param  wakeup_event: A bit map of wake up event. Available event:
  *                         STANDBY_WAKEUP_BY_STIMER
  *                         STANDBY_WAKEUP_BY_NFC
  *                         STANDBY_WAKEUP_BY_PA5 (GPIO)
  *                         STANDBY_WAKEUP_BY_PC7 (GPIO)
  *                         STANDBY_WAKEUP_BY_PD5 (GPIO)
  *                         STANDBY_WAKEUP_BY_PE3 (GPIO)
  * @retval None
  */
void standby_wakeup_event_del(uint32_t wakeup_event)
{
    if (wakeup_event & STANDBY_WAKEUP_BY_STIMER) {
        DStandbyWakeupEvent.wakeup_event &= ~DSTBY_STIMER;
    }

#if 0
    if (wakeup_event & STANDBY_WAKEUP_BY_DS_TIMER) {
        DStandbyWakeupEvent.wakeup_event &= ~DSTBY_TIMER33;
    }
#endif

    if (wakeup_event & STANDBY_WAKEUP_BY_NFC) {
        DStandbyWakeupEvent.wakeup_event &= ~DSTBY_NFC;
    }

    if (wakeup_event & STANDBY_WAKEUP_BY_PA5) {
        DStandbyWakeupEvent.gpio_option &= ~BIT0;
    }

    if (wakeup_event & STANDBY_WAKEUP_BY_PC7) {
        DStandbyWakeupEvent.gpio_option &= ~BIT1;
    }

    if (wakeup_event & STANDBY_WAKEUP_BY_PD5) {
        DStandbyWakeupEvent.gpio_option &= ~BIT2;
    }

    if (wakeup_event & STANDBY_WAKEUP_BY_PE3) {
        DStandbyWakeupEvent.gpio_option &= ~BIT3;
    }

    if ((DStandbyWakeupEvent.gpio_option & 0x0f) == 0) {
        // All GPIO wake up pin are disabled
        DStandbyWakeupEvent.wakeup_event &= ~DSTBY_GPIO;
    }
}

/**
  * @brief  To make the system entering the Deep Standby power saving.
  *         The CPU, memory and part fo peripheral power is off when 
  *         entering deep standby power saving mode. The program needs 
  *         to be reload from the flash at system resume.
  *
  * @retval None
  */
void deepstandby_ex(void) 
{
    if ((DStandbyWakeupEvent.wakeup_event & (DSTBY_STIMER|DSTBY_NFC|DSTBY_GPIO)) == 0) {
        // error: no wakeup event was added, so skip the entering standby power saving
        return;
    }
    
    DeepStandby(DStandbyWakeupEvent.wakeup_event, 
        DStandbyWakeupEvent.timer_duration, DStandbyWakeupEvent.gpio_option);
}

/**
  * @brief  To make the system entering the Deep Sleep power saving mode.
  *         The CPU, memory and peripheral power is off when entering
  *         deep sleep power saving mode. The program needs to be reload
  *         and all peripheral needs be re-configure when system resume.
  *
  * @param  wakeup_event: A bit map of wake up event. Available event:
  *                         DSLEEP_WAKEUP_BY_TIMER
  *                         DSLEEP_WAKEUP_BY_GPIO
  *         sleep_duration: the system sleep duration in ms, only valid
  *         for DSLEEP_WAKEUP_BY_TIMER wake up event.
  *
  * @retval None
  */
void deepsleep_ex(uint32_t wakeup_event, uint32_t sleep_duration)
{
    u8 wake_ev=0;
    
    if ((wakeup_event & DSLEEP_WAKEUP_BY_TIMER) && (sleep_duration > 0)) {
        // wake up by timeout
        wake_ev |= DS_TIMER33;
    }
    
    if (wakeup_event & DSLEEP_WAKEUP_BY_GPIO) {
        // wake up by GPIO pin goes high
        wake_ev |= DS_GPIO;
    }

    if (wake_ev == 0) {
        // error: No wake up event, skip entering deep sleep mode
        return;
    }
    DeepSleep (wake_ev, sleep_duration);
}
