/*
 * mbed Microcontroller Library
 * Copyright (c) 2018-2019 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
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

#include "device.h"
#include "psoc6_utils.h"
#include "cycfg.h"

#if defined(COMPONENT_SPM_MAILBOX)
void mailbox_init(void);
#endif

/*******************************************************************************
* Function Name: mbed_sdk_init
****************************************************************************//**
*
* Mbed's post-memory-initialization function.
* Used here to initialize common parts of the Cypress libraries.
*
*******************************************************************************/
void mbed_sdk_init(void)
{
    /* Initialize shared resource manager */
    cy_srm_initialize();

    /* Initialize system and clocks. */
    /* Placed here as it must be done after proper LIBC initialization. */
    SystemInit();

#if defined(COMPONENT_SPM_MAILBOX)
    mailbox_init();
#endif

#if (!CY_CPU_CORTEX_M0P)
    /* Set up the device based on configurator selections */
    init_cycfg_all();

    /* Enable global interrupts (disabled in CM4 startup assembly) */
    __enable_irq();
#endif
}
