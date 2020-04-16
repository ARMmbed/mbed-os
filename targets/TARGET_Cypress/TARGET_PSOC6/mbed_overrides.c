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
#include "mbed_power_mgmt.h"
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


#if (defined(CY_CFG_PWR_SYS_IDLE_MODE) && (CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_ACTIVE))
/*******************************************************************************
* Function Name: active_idle_hook
****************************************************************************//**
*
* Empty idle hook function to prevent the system entering sleep mode
* automatically any time the system is idle.
*
*******************************************************************************/
static void active_idle_hook(void)
{
    /* Do nothing, so the rtos_idle_loop() performs while(1) */
}
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
#if CY_CPU_CORTEX_M0P
    /* Disable global interrupts */
    __disable_irq();
#endif

    /* Initialize system and clocks. */
    /* Placed here as it must be done after proper LIBC initialization. */
    SystemInit();

    /* Set up the device based on configurator selections */
    cybsp_init();

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

#if defined (CY_CFG_PWR_SYS_IDLE_MODE)
    /* Configure the lowest power state the system is allowed to enter
    * based on the System Idle Power Mode parameter value in the Device
    * Configurator. The default value is system deep sleep.
    */
#if (CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_ACTIVE)
    rtos_attach_idle_hook(&active_idle_hook);
#elif (CY_CFG_PWR_SYS_IDLE_MODE == CY_CFG_PWR_MODE_SLEEP)
    sleep_manager_lock_deep_sleep();
#else
    /* Deep sleep is default state */
#endif
#endif
}
