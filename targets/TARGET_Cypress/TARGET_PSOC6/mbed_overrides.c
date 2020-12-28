/*
 * mbed Microcontroller Library
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#include "cycfg.h"
#include "cyhal_hwmgr.h"
#include "cybsp.h"
#include "cy_mbed_post_init.h"
#include "mbed_error.h"
#if MBED_CONF_RTOS_PRESENT
#include "rtos_idle.h"
#endif // MBED_CONF_RTOS_PRESENT
#include "us_ticker_api.h"

#if defined(CY_ENABLE_XIP_PROGRAM)
#include "cycfg_qspi_memslot.h"
#endif /*defined(CY_ENABLE_XIP_PROGRAM)*/
#if defined(MBED_CONF_TARGET_XIP_ENABLE)
#include "cy_serial_flash_qspi.h"
#endif /* defined(MBED_CONF_TARGET_XIP_ENABLE) */

MBED_WEAK void cy_mbed_post_bsp_init_hook(void)
{
    /* By default, do nothing */
}

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
#if CY_CPU_CORTEX_M0P
    /* Disable global interrupts */
    __disable_irq();
#endif

    /* Initialize system and clocks. */
    /* Placed here as it must be done after proper LIBC initialization. */
    SystemInit();

    /* Set up the device based on configurator selections */
    if (CY_RSLT_SUCCESS != cybsp_init()) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER, MBED_ERROR_CODE_FAILED_OPERATION), "cybsp_init");
    }

    cy_mbed_post_bsp_init_hook();

#if CY_CPU_CORTEX_M0P
    /* Enable global interrupts */
    __enable_irq();
#else
    /*
     * Init the us Ticker here to avoid imposing on the limited stack space of the idle thread.
     * This also allows the first call to sleep to occur faster.
     */
    us_ticker_init();

#if MBED_CONF_TARGET_XIP_ENABLE
    /* The linker script allows storing data in external memory, if needed, enable access to that memory. */
    const uint32_t bus_frequency = 50000000lu;
    cy_serial_flash_qspi_init(smifMemConfigs[0], CYBSP_QSPI_D0, CYBSP_QSPI_D1, CYBSP_QSPI_D2, CYBSP_QSPI_D3, NC, NC, NC, NC, CYBSP_QSPI_SCK, CYBSP_QSPI_SS, bus_frequency);
    cy_serial_flash_qspi_enable_xip(true);
#endif

    /* Enable global interrupts (disabled in CM4 startup assembly) */
    __enable_irq();
#endif
}
