/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
#if DEVICE_SLEEP

#include <stdio.h>
#include <string.h>
#include "sleep_api.h"
#include "cmsis.h"
#include "gpio_api.h"
#include "mbed_power_mgmt.h"
#include "mbed_critical.h"
#include "rtos_idle.h"

int nbsleep_req = 0;
int external_pin = 0;
static int initialize_policy = 0;
static int disable_poweroffbycp = 1;

static inline void hw_delay_us(unsigned int value)
{
    volatile unsigned i, j;
    for (i = 0; i < (value * 2); i++) {
        for (j = 0; j < 100; j++);
    }
}

int s5js100_idle_sicd(void)
{
    if (getreg32(0x85023090) & (0x1 << 1)) {
        /* send CP to ready status */
        modifyreg32(0x85023090, (0x1 << 2), (0x1 << 2));
        modifyreg32(0x85026100, 0x2, 0x0);

        pmu_short_sleep(); //go to sleep

        modifyreg32(0x85026100, 0x2, 0x2);

        /* wait for mbox */
        while ((getreg32(0x85023090) & (0x1 << 1))) {
            hw_delay_us(10);
            putreg32(0x2, 0x85023020);
            putreg32(0x2, 0x8502301C);
        };

        /* release short sleep status */
        modifyreg32(0x85023090, (0x1 << 2), (0x0 << 2));
        return 1;
    } else {
        asm("wfi");
    }

    return 0;
}

static void s5js100_idle_hook(void)
{
    core_util_critical_section_enter();
    sleep();
    core_util_critical_section_exit();
}

static void set_sleep_policy(void)
{
    rtos_attach_idle_hook(&s5js100_idle_hook);
    initialize_policy = 1;
}

void config_poweroffbycp(int enable)
{
    disable_poweroffbycp = (enable == 0) ? 1 : 0;
}

static int change_cp_pwr_lock(int lock)
{
    static int prev_lock = 1;
    int i;

    lock &= 0x1;

    if (lock == prev_lock) {
        return 0;
    }

    if (lock == 0) {
        modifyreg32(0x85023088, 0x1, 0x0);
        prev_lock = 0;
        return 0;
    }

    modifyreg32(0x85023088, 0x1, 0x1);
    prev_lock = 1;

    if (prev_lock == 0) {
        for (i = 0; i < 1000; i++)
            if (s5js100_idle_sicd()) {
                return 1;
            }
    }

    return 0;
}


void hal_sleep(void)
{
    if (initialize_policy == 0) {
        set_sleep_policy();
    }

    if (!strcmp(get_env("SLEEP"), "ON") && disable_poweroffbycp != 1) {
        hal_deepsleep();
        return;
    }

    modifyreg32(0x85026100, 0x2, 0x2);

    if (!change_cp_pwr_lock(1)) {
        asm("wfi");
    }

    modifyreg32(0x85026100, 0x2, 0x0);
}

void hal_deepsleep(void)
{
    int cp_pwr_lock = external_pin | disable_poweroffbycp;

    if (nbsleep_req) {
        pmu_nb_sleep();
    }

    modifyreg32(0x85026100, 0x2, 0x2);

    if (change_cp_pwr_lock(cp_pwr_lock)) {
        goto out;
    }

    s5js100_idle_sicd();

out:
    modifyreg32(0x85026100, 0x2, 0x2);
}

#endif // DEVICE_SLEEP
