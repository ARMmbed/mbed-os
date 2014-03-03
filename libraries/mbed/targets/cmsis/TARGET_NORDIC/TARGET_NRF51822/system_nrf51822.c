/* mbed Microcontroller Library

 * Copyright (c) 2013 Nordic Semiconductor.
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


#include <stdint.h>
#include <stdbool.h>
#include "nrf51822.h"
#include "system_nrf51822.h"


#define __SYSTEM_CLOCK      (16000000UL)     //!< nRF51 devices use a fixed System Clock Frequency of 16MHz

static bool is_manual_peripheral_setup_needed(void);
static bool is_disabled_in_debug_needed(void);


#if defined ( __CC_ARM )
    uint32_t SystemCoreClock __attribute__((used)) = __SYSTEM_CLOCK;  
#elif defined ( __ICCARM__ )
    __root uint32_t SystemCoreClock = __SYSTEM_CLOCK;
#elif defined   ( __GNUC__ )
    uint32_t SystemCoreClock __attribute__((used)) = __SYSTEM_CLOCK;
#endif


void SystemCoreClockUpdate(void)
{
    SystemCoreClock = __SYSTEM_CLOCK;
}

void SystemInit(void)
{   
    NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos);
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy){
    }
    //write FWID (NRF_UICR->FWID is readonly)
    *(uint32_t *)0x10001010 = 0xFFFF0049;

    while (NRF_NVMC->READY == NVMC_READY_READY_Busy){
    }

    NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos);
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy){
    }
    // Prepare the peripherals for use as indicated by the PAN 26 "System: Manual setup is required
    // to enable the use of peripherals" found at Product Anomaly document for your device found at
    // https://www.nordicsemi.com/. The side effect of executing these instructions in the devices 
    // that do not need it is that the new peripherals in the second generation devices (LPCOMP for
    // example) will not be available.
    
    if (is_manual_peripheral_setup_needed()){
        *(uint32_t volatile *)0x40000504 = 0xC007FFDF;
        *(uint32_t volatile *)0x40006C18 = 0x00008000;
    }
    
    // Disable PROTENSET registers under debug, as indicated by PAN 59 "MPU: Reset value of DISABLEINDEBUG
    // register is incorrect" found at Product Anomaly document four your device found at 
    // https://www.nordicsemi.com/. There is no side effect of using these instruction if not needed. 
    if (is_disabled_in_debug_needed()){
        NRF_MPU->DISABLEINDEBUG = MPU_DISABLEINDEBUG_DISABLEINDEBUG_Disabled << MPU_DISABLEINDEBUG_DISABLEINDEBUG_Pos;
    }
	
	// Start 16 MHz crystal oscillator.
    NRF_CLOCK->EVENTS_HFCLKSTARTED  = 0;
    NRF_CLOCK->TASKS_HFCLKSTART     = 1;

    // Wait for the external oscillator to start up.
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0) {
        // Do nothing.
    }
}

static bool is_manual_peripheral_setup_needed(void) 
{
    if ((((*(uint32_t *)0xF0000FE0) & 0x000000FF) == 0x1) && (((*(uint32_t *)0xF0000FE4) & 0x0000000F) == 0x0))
    {
        if ((((*(uint32_t *)0xF0000FE8) & 0x000000F0) == 0x00) && (((*(uint32_t *)0xF0000FEC) & 0x000000F0) == 0x0))
        {
            return true;
        }
        if ((((*(uint32_t *)0xF0000FE8) & 0x000000F0) == 0x10) && (((*(uint32_t *)0xF0000FEC) & 0x000000F0) == 0x0))
        {
            return true;
        }
        if ((((*(uint32_t *)0xF0000FE8) & 0x000000F0) == 0x30) && (((*(uint32_t *)0xF0000FEC) & 0x000000F0) == 0x0))
        {
            return true;
        }
    }
    
    return false;
}

static bool is_disabled_in_debug_needed(void) 
{
    if ((((*(uint32_t *)0xF0000FE0) & 0x000000FF) == 0x1) && (((*(uint32_t *)0xF0000FE4) & 0x0000000F) == 0x0))
    {
        if ((((*(uint32_t *)0xF0000FE8) & 0x000000F0) == 0x40) && (((*(uint32_t *)0xF0000FEC) & 0x000000F0) == 0x0))
        {
            return true;
        }
    }
    
    return false;
}

