/***************************************************************************//**
* @file mbed_overrides.c
*******************************************************************************
* @section License
* <b>Copyright (c) 2021 Microchip Technology Inc. and its subsidiaries.</b>
*******************************************************************************
*
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License"); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************/

#include "pic32cx.h"
#include "sysclk.h"
#include "device_peripherals.h"
#include "device.h"

/* Called before main - implement here if board needs it.
* Otherwise, let the application override this if necessary */
void mbed_sdk_init()
{
    /* Unlock JTAG access */
    SFR->SFR_WPMR = SFR_WPMR_WPKEY_PASSWD;
    SFR->SFR_JTAG = 0;
    SFR->SFR_WPMR = SFR_WPMR_WPKEY_PASSWD | SFR_WPMR_WPEN;

    /* Disable the watchdog */
    DWDT->WDT0_MR |= WDT0_MR_WDDIS;
    DWDT->WDT1_MR |= WDT1_MR_WDDIS;

    /* Disable "Execute never (XN)" for RAM region */
    mbed_mpu_enable_ram_xn(false);

    /* Initialize the SAM system */
    sysclk_init();
}
