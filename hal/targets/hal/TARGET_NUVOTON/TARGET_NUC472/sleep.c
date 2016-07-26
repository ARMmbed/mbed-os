/* mbed Microcontroller Library
 * Copyright (c) 2015-2016 Nuvoton
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

#include "sleep_api.h"
#include "serial_api.h"
#include "lp_ticker_api.h"

#if DEVICE_SLEEP

#include "cmsis.h"
#include "device.h"
#include "objects.h"
#include "PeripheralPins.h"

void us_ticker_prepare_sleep(struct sleep_s *obj);
void us_ticker_wakeup_from_sleep(struct sleep_s *obj);
static void mbed_enter_sleep(struct sleep_s *obj);
static void mbed_exit_sleep(struct sleep_s *obj);

int serial_allow_powerdown(void);
int spi_allow_powerdown(void);
int i2c_allow_powerdown(void);
int pwmout_allow_powerdown(void);

/**
 * Enter Idle mode.
 */
void sleep(void)
{
    struct sleep_s sleep_obj;
    sleep_obj.powerdown = 0;
    mbed_enter_sleep(&sleep_obj);
    mbed_exit_sleep(&sleep_obj);
}

/**
 * Enter Power-down mode while no peripheral is active; otherwise, enter Idle mode.
 */
void deepsleep(void)
{
    struct sleep_s sleep_obj;
    sleep_obj.powerdown = 1;
    mbed_enter_sleep(&sleep_obj);
    mbed_exit_sleep(&sleep_obj);
}

static void mbed_enter_sleep(struct sleep_s *obj)
{
    // Check if serial allows entering power-down mode
    if (obj->powerdown) {
        obj->powerdown = serial_allow_powerdown();
    }
    // Check if spi allows entering power-down mode
    if (obj->powerdown) {
        obj->powerdown = spi_allow_powerdown();
    }
    // Check if i2c allows entering power-down mode
    if (obj->powerdown) {
        obj->powerdown = i2c_allow_powerdown();
    }
    // Check if pwmout allows entering power-down mode
    if (obj->powerdown) {
        obj->powerdown = pwmout_allow_powerdown();
    }
    // TODO: Check if other peripherals allow entering power-down mode
    
    obj->start_us = lp_ticker_read();
    // Let us_ticker prepare for power-down or reject it.
    us_ticker_prepare_sleep(obj);
    
    // NOTE(STALE): To pass mbed-drivers test, timer requires to be fine-grained, so its implementation needs HIRC rather than LIRC/LXT as its clock source.
    //       But as CLK_PowerDown()/CLK_Idle() is called, HIRC will be disabled and timer cannot keep counting and alarm. To overcome the dilemma, 
    //       just make CPU halt and compromise power saving.
    // NOTE: As CLK_PowerDown()/CLK_Idle() is called, HIRC/HXT will be disabled in normal mode, but not in ICE mode. This may cause confusion in development.

    if (obj->powerdown) {   // Power-down mode (HIRC/HXT disabled, LIRC/LXT enabled)
        SYS_UnlockReg();
        CLK_PowerDown();
        SYS_LockReg();
    }
    else {  // CPU halt mode (HIRC/HXT enabled, LIRC/LXT enabled)
        // NOTE: NUC472's CLK_Idle() will also disable HIRC/HXT.
        SYS_UnlockReg();
        SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
        CLK->PWRCTL &= ~CLK_PWRCTL_PDEN_Msk;
        __WFI();
        SYS_LockReg();
    }
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    
    obj->end_us = lp_ticker_read();
    obj->period_us = (obj->end_us > obj->start_us) ? (obj->end_us - obj->start_us) : (uint32_t) ((uint64_t) obj->end_us + 0xFFFFFFFFu - obj->start_us);
    // Let us_ticker recover from power-down.
    us_ticker_wakeup_from_sleep(obj);
}

static void mbed_exit_sleep(struct sleep_s *obj)
{
    // TODO: TO BE CONTINUED
    
    (void)obj;
}

#endif
