//*****************************************************************************
//
//  am_hal_pwrctrl.c
//! @file
//!
//! @brief Functions for enabling and disabling power domains.
//!
//! @addtogroup pwrctrl3 Power Control
//! @ingroup apollo3hal
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause

#include <stdint.h>
#include <stdbool.h>
#include "am_mcu_apollo.h"

//
// Maximum number of checks to memory power status before declaring error.
//
#define AM_HAL_PWRCTRL_MAX_WFE  20

//
// Define the peripheral control structure.
//
const struct
{
    uint32_t      ui32PeriphEnable;
    uint32_t      ui32PeriphStatus;
    uint32_t      ui32PeriphEvent;
}
am_hal_pwrctrl_peripheral_control[AM_HAL_PWRCTRL_PERIPH_MAX] =
{
    {0, 0, 0},                                  //  AM_HAL_PWRCTRL_PERIPH_NONE
    {_VAL2FLD(PWRCTRL_DEVPWREN_PWRIOS, PWRCTRL_DEVPWREN_PWRIOS_EN),
     PWRCTRL_DEVPWRSTATUS_HCPA_Msk,
     _VAL2FLD(PWRCTRL_DEVPWREVENTEN_HCPAEVEN, PWRCTRL_DEVPWREVENTEN_HCPAEVEN_EN)},  // AM_HAL_PWRCTRL_PERIPH_IOS
    {_VAL2FLD(PWRCTRL_DEVPWREN_PWRIOM0, PWRCTRL_DEVPWREN_PWRIOM0_EN),
     PWRCTRL_DEVPWRSTATUS_HCPB_Msk,
     _VAL2FLD(PWRCTRL_DEVPWREVENTEN_HCPBEVEN, PWRCTRL_DEVPWREVENTEN_HCPBEVEN_EN)},  //  AM_HAL_PWRCTRL_PERIPH_IOM0
    {_VAL2FLD(PWRCTRL_DEVPWREN_PWRIOM1, PWRCTRL_DEVPWREN_PWRIOM1_EN),
     PWRCTRL_DEVPWRSTATUS_HCPB_Msk,
     _VAL2FLD(PWRCTRL_DEVPWREVENTEN_HCPBEVEN, PWRCTRL_DEVPWREVENTEN_HCPBEVEN_EN)},  //  AM_HAL_PWRCTRL_PERIPH_IOM1
    {_VAL2FLD(PWRCTRL_DEVPWREN_PWRIOM2, PWRCTRL_DEVPWREN_PWRIOM2_EN),
     PWRCTRL_DEVPWRSTATUS_HCPB_Msk,
     _VAL2FLD(PWRCTRL_DEVPWREVENTEN_HCPBEVEN, PWRCTRL_DEVPWREVENTEN_HCPBEVEN_EN)},  //  AM_HAL_PWRCTRL_PERIPH_IOM2
    {_VAL2FLD(PWRCTRL_DEVPWREN_PWRIOM3, PWRCTRL_DEVPWREN_PWRIOM3_EN),
     PWRCTRL_DEVPWRSTATUS_HCPC_Msk,
     _VAL2FLD(PWRCTRL_DEVPWREVENTEN_HCPCEVEN, PWRCTRL_DEVPWREVENTEN_HCPCEVEN_EN)},  //  AM_HAL_PWRCTRL_PERIPH_IOM3
    {_VAL2FLD(PWRCTRL_DEVPWREN_PWRIOM4, PWRCTRL_DEVPWREN_PWRIOM4_EN),
     PWRCTRL_DEVPWRSTATUS_HCPC_Msk,
     _VAL2FLD(PWRCTRL_DEVPWREVENTEN_HCPCEVEN, PWRCTRL_DEVPWREVENTEN_HCPCEVEN_EN)},  //  AM_HAL_PWRCTRL_PERIPH_IOM4
    {_VAL2FLD(PWRCTRL_DEVPWREN_PWRIOM5, PWRCTRL_DEVPWREN_PWRIOM5_EN),
     PWRCTRL_DEVPWRSTATUS_HCPC_Msk,
     _VAL2FLD(PWRCTRL_DEVPWREVENTEN_HCPCEVEN, PWRCTRL_DEVPWREVENTEN_HCPCEVEN_EN)},  //  AM_HAL_PWRCTRL_PERIPH_IOM5
    {_VAL2FLD(PWRCTRL_DEVPWREN_PWRUART0, PWRCTRL_DEVPWREN_PWRUART0_EN),
     PWRCTRL_DEVPWRSTATUS_HCPA_Msk,
     _VAL2FLD(PWRCTRL_DEVPWREVENTEN_HCPAEVEN, PWRCTRL_DEVPWREVENTEN_HCPAEVEN_EN)},  //  AM_HAL_PWRCTRL_PERIPH_UART0
    {_VAL2FLD(PWRCTRL_DEVPWREN_PWRUART1, PWRCTRL_DEVPWREN_PWRUART1_EN),
     PWRCTRL_DEVPWRSTATUS_HCPA_Msk,
     _VAL2FLD(PWRCTRL_DEVPWREVENTEN_HCPAEVEN, PWRCTRL_DEVPWREVENTEN_HCPAEVEN_EN)},  //  AM_HAL_PWRCTRL_PERIPH_UART1
    {_VAL2FLD(PWRCTRL_DEVPWREN_PWRADC, PWRCTRL_DEVPWREN_PWRADC_EN),
     PWRCTRL_DEVPWRSTATUS_PWRADC_Msk,
     _VAL2FLD(PWRCTRL_DEVPWREVENTEN_ADCEVEN, PWRCTRL_DEVPWREVENTEN_ADCEVEN_EN)},    //  AM_HAL_PWRCTRL_PERIPH_ADC
    {_VAL2FLD(PWRCTRL_DEVPWREN_PWRSCARD, PWRCTRL_DEVPWREN_PWRSCARD_EN),
     PWRCTRL_DEVPWRSTATUS_HCPA_Msk,
     _VAL2FLD(PWRCTRL_DEVPWREVENTEN_HCPAEVEN, PWRCTRL_DEVPWREVENTEN_HCPAEVEN_EN)},  //  AM_HAL_PWRCTRL_PERIPH_SCARD
    {_VAL2FLD(PWRCTRL_DEVPWREN_PWRMSPI, PWRCTRL_DEVPWREN_PWRMSPI_EN),
     PWRCTRL_DEVPWRSTATUS_PWRMSPI_Msk,
     _VAL2FLD(PWRCTRL_DEVPWREVENTEN_MSPIEVEN, PWRCTRL_DEVPWREVENTEN_MSPIEVEN_EN)},  //  AM_HAL_PWRCTRL_PERIPH_MSPI
    {_VAL2FLD(PWRCTRL_DEVPWREN_PWRPDM, PWRCTRL_DEVPWREN_PWRPDM_EN),
     PWRCTRL_DEVPWRSTATUS_PWRPDM_Msk,
     _VAL2FLD(PWRCTRL_DEVPWREVENTEN_PDMEVEN, PWRCTRL_DEVPWREVENTEN_PDMEVEN_EN)},    //  AM_HAL_PWRCTRL_PERIPH_PDM
    {_VAL2FLD(PWRCTRL_DEVPWREN_PWRBLEL, PWRCTRL_DEVPWREN_PWRBLEL_EN),
     PWRCTRL_DEVPWRSTATUS_BLEL_Msk,
     _VAL2FLD(PWRCTRL_DEVPWREVENTEN_BLELEVEN, PWRCTRL_DEVPWREVENTEN_BLELEVEN_EN)}   //  AM_HAL_PWRCTRL_PERIPH_BLEL
};


//
// Define the memory control structure.
//
const struct
{
    uint32_t      ui32MemoryEnable;
    uint32_t      ui32MemoryStatus;
    uint32_t      ui32MemoryEvent;
    uint32_t      ui32MemoryMask;
    uint32_t      ui32StatusMask;
    uint32_t      ui32PwdSlpEnable;
}
am_hal_pwrctrl_memory_control[AM_HAL_PWRCTRL_MEM_MAX] =
{
    {0, 0, 0, 0, 0, 0},
    {AM_HAL_PWRCTRL_MEMEN_SRAM_8K_DTCM,
     AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_8K_DTCM,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_8K_DTCM,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_8K_DTCM},
    {AM_HAL_PWRCTRL_MEMEN_SRAM_32K_DTCM,
     AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_32K_DTCM,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_32K_DTCM,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_32K_DTCM},
    {AM_HAL_PWRCTRL_MEMEN_SRAM_64K_DTCM,
     AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_64K_DTCM,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_64K_DTCM,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_64K_DTCM},
    {AM_HAL_PWRCTRL_MEMEN_SRAM_96K,
     AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_96K,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_96K,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_96K},
    {AM_HAL_PWRCTRL_MEMEN_SRAM_128K,
     AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_128K,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_128K,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_128K},
    {AM_HAL_PWRCTRL_MEMEN_SRAM_160K,
     AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_160K,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_160K,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_160K},
    {AM_HAL_PWRCTRL_MEMEN_SRAM_192K,
     AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_192K,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_192K,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_192K},
    {AM_HAL_PWRCTRL_MEMEN_SRAM_224K,
     AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_224K,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_224K,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_224K},
    {AM_HAL_PWRCTRL_MEMEN_SRAM_256K,
     AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_256K,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_256K,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_256K},
    {AM_HAL_PWRCTRL_MEMEN_SRAM_288K,
     AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_288K,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_288K,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_288K},
    {AM_HAL_PWRCTRL_MEMEN_SRAM_320K,
     AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_320K,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_320K,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_320K},
    {AM_HAL_PWRCTRL_MEMEN_SRAM_352K,
     AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_352K,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_352K,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_352K},
    {AM_HAL_PWRCTRL_MEMEN_SRAM_384K,
     AM_HAL_PWRCTRL_PWRONSTATUS_SRAM_384K,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_SRAM_384K,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEM_REGION_SRAM_MASK,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_SRAM_384K},
    {AM_HAL_PWRCTRL_MEMEN_FLASH_512K,
     AM_HAL_PWRCTRL_PWRONSTATUS_FLASH_512K,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_FLASH_512K,
     AM_HAL_PWRCTRL_MEM_REGION_FLASH_MASK,
     AM_HAL_PWRCTRL_MEM_REGION_FLASH_MASK,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_FLASH_512K},
    {AM_HAL_PWRCTRL_MEMEN_FLASH_1M,
     AM_HAL_PWRCTRL_PWRONSTATUS_FLASH_1M,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_FLASH_1M,
     AM_HAL_PWRCTRL_MEM_REGION_FLASH_MASK,
     AM_HAL_PWRCTRL_MEM_REGION_FLASH_MASK,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_FLASH_1M},
    {AM_HAL_PWRCTRL_MEMEN_CACHE,
     0,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_CACHE,
     AM_HAL_PWRCTRL_MEM_REGION_CACHE_MASK,
     0,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_CACHE},
    {AM_HAL_PWRCTRL_MEMEN_ALL,
     AM_HAL_PWRCTRL_PWRONSTATUS_ALL,
     AM_HAL_PWRCTRL_MEMPWREVENTEN_ALL,
     AM_HAL_PWRCTRL_MEM_REGION_ALL_MASK,
     AM_HAL_PWRCTRL_MEM_REGION_ALT_ALL_MASK,
     AM_HAL_PWRCTRL_MEMPWDINSLEEP_ALL}
};

// ****************************************************************************
//
//  am_hal_pwrctrl_periph_enable()
//  Enable power for a peripheral.
//
// ****************************************************************************
uint32_t
am_hal_pwrctrl_periph_enable(am_hal_pwrctrl_periph_e ePeripheral)
{


    //
    // Enable power control for the given device.
    //
    AM_CRITICAL_BEGIN
    PWRCTRL->DEVPWREN |= am_hal_pwrctrl_peripheral_control[ePeripheral].ui32PeriphEnable;
    AM_CRITICAL_END




    for (uint32_t wait_usecs = 0; wait_usecs < AM_HAL_PWRCTRL_MAX_WFE; wait_usecs += 10)
    {
        am_hal_flash_delay(FLASH_CYCLES_US(10));

        if ( (PWRCTRL->DEVPWRSTATUS & am_hal_pwrctrl_peripheral_control[ePeripheral].ui32PeriphStatus) > 0)
        {
            break;
        }
    }

    //
    // Check the device status.
    //
    if ( (PWRCTRL->DEVPWRSTATUS & am_hal_pwrctrl_peripheral_control[ePeripheral].ui32PeriphStatus) > 0 )
    {
        return AM_HAL_STATUS_SUCCESS;
    }
    else
    {
        return AM_HAL_STATUS_FAIL;
    }

}

// ****************************************************************************
//
//  am_hal_pwrctrl_periph_disable()
//  Disable power for a peripheral.
//
// ****************************************************************************
uint32_t
am_hal_pwrctrl_periph_disable(am_hal_pwrctrl_periph_e ePeripheral)
{

    //
    // Disable power domain for the given device.
    //
    AM_CRITICAL_BEGIN
    PWRCTRL->DEVPWREN &= ~am_hal_pwrctrl_peripheral_control[ePeripheral].ui32PeriphEnable;
    AM_CRITICAL_END


    for (uint32_t wait_usecs = 0; wait_usecs < AM_HAL_PWRCTRL_MAX_WFE; wait_usecs += 10)
    {
        am_hal_flash_delay(FLASH_CYCLES_US(10));

        if ( (PWRCTRL->DEVPWRSTATUS & am_hal_pwrctrl_peripheral_control[ePeripheral].ui32PeriphStatus) == 0 )
        {
            break;
        }
    }

    //
    // Check the device status.
    //
    if ( ( PWRCTRL->DEVPWRSTATUS & am_hal_pwrctrl_peripheral_control[ePeripheral].ui32PeriphStatus) == 0 )
    {
        return AM_HAL_STATUS_SUCCESS;
    }
    else
    {
        return AM_HAL_STATUS_FAIL;
    }
}

//*****************************************************************************
//
//! @brief Determine whether a peripheral is currently enabled.
//!
//! @param ePeripheral - The peripheral to enable.
//! @param pui32Enabled - Pointer to a ui32 that will return as 1 or 0.
//!
//! This function determines to the caller whether a given peripheral is
//! currently enabled or disabled.
//!
//! @return status - generic or interface specific status.
//
//*****************************************************************************
uint32_t
am_hal_pwrctrl_periph_enabled(am_hal_pwrctrl_periph_e ePeripheral,
                              uint32_t *pui32Enabled)
{
    uint32_t ui32Mask = 0;
    uint32_t ui32Enabled = 0;

    if ( pui32Enabled == NULL )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

    switch ( ePeripheral )
    {
        case AM_HAL_PWRCTRL_PERIPH_NONE:
        case AM_HAL_PWRCTRL_PERIPH_SCARD:
            break;
        case AM_HAL_PWRCTRL_PERIPH_IOS:
        case AM_HAL_PWRCTRL_PERIPH_UART0:
        case AM_HAL_PWRCTRL_PERIPH_UART1:
            ui32Mask = PWRCTRL_DEVPWRSTATUS_HCPA_Msk;
            break;
        case AM_HAL_PWRCTRL_PERIPH_IOM0:
        case AM_HAL_PWRCTRL_PERIPH_IOM1:
        case AM_HAL_PWRCTRL_PERIPH_IOM2:
            ui32Mask = PWRCTRL_DEVPWRSTATUS_HCPB_Msk;
            break;
        case AM_HAL_PWRCTRL_PERIPH_IOM3:
        case AM_HAL_PWRCTRL_PERIPH_IOM4:
        case AM_HAL_PWRCTRL_PERIPH_IOM5:
            ui32Mask = PWRCTRL_DEVPWRSTATUS_HCPC_Msk;
            break;
        case AM_HAL_PWRCTRL_PERIPH_ADC:
            ui32Mask = PWRCTRL_DEVPWRSTATUS_PWRADC_Msk;
            break;
        case AM_HAL_PWRCTRL_PERIPH_MSPI:
            ui32Mask = PWRCTRL_DEVPWRSTATUS_PWRMSPI_Msk;
            break;
        case AM_HAL_PWRCTRL_PERIPH_PDM:
            ui32Mask = PWRCTRL_DEVPWRSTATUS_PWRPDM_Msk;
            break;
        case AM_HAL_PWRCTRL_PERIPH_BLEL:
            ui32Mask = PWRCTRL_DEVPWRSTATUS_BLEL_Msk;
            break;
        default:
            return AM_HAL_STATUS_FAIL;
    }

    if ( ui32Mask != 0 )
    {
        ui32Enabled = PWRCTRL->DEVPWRSTATUS & ui32Mask ? 1 : 0;
    }

    *pui32Enabled = ui32Enabled;

    return AM_HAL_STATUS_SUCCESS;
}



// ****************************************************************************
//
//  am_hal_pwrctrl_memory_enable()
//  Enable a configuration of memory.
//
// ****************************************************************************
uint32_t
am_hal_pwrctrl_memory_enable(am_hal_pwrctrl_mem_e eMemConfig)
{
    uint32_t ui32MemEnMask, ui32MemDisMask, ui32MemRegionMask, ui32MemStatusMask;

    ui32MemEnMask     =  am_hal_pwrctrl_memory_control[eMemConfig].ui32MemoryEnable;
    ui32MemDisMask    = ~am_hal_pwrctrl_memory_control[eMemConfig].ui32MemoryEnable;
    ui32MemRegionMask = am_hal_pwrctrl_memory_control[eMemConfig].ui32MemoryMask;
    ui32MemStatusMask = am_hal_pwrctrl_memory_control[eMemConfig].ui32StatusMask;

    //
    // Disable unneeded memory. If nothing to be disabled, skip to save time.
    //
    // Note that a deliberate disable step using a disable mask is taken here
    // for 2 reasons: 1) To only affect the specified type of memory, and 2)
    // To avoid inadvertently disabling any memory currently being depended on.
    //
    if ( ui32MemDisMask != 0 )
    {
        PWRCTRL->MEMPWREN &=
            ~(ui32MemDisMask & ui32MemRegionMask)                                   |
             (_VAL2FLD(PWRCTRL_MEMPWREN_DTCM, PWRCTRL_MEMPWREN_DTCM_GROUP0DTCM0)    |
              _VAL2FLD(PWRCTRL_MEMPWREN_FLASH0, PWRCTRL_MEMPWREN_FLASH0_EN));
        am_hal_flash_delay(FLASH_CYCLES_US(1));
    }


    //
    // Enable the required memory.
    //
    if ( ui32MemEnMask != 0 )
    {
        PWRCTRL->MEMPWREN |= ui32MemEnMask;

        for (uint32_t wait_usecs = 0; wait_usecs < AM_HAL_PWRCTRL_MAX_WFE; wait_usecs += 10)
        {
            am_hal_flash_delay(FLASH_CYCLES_US(10));

            if ( (PWRCTRL->MEMPWRSTATUS & ui32MemStatusMask) ==
                  am_hal_pwrctrl_memory_control[eMemConfig].ui32MemoryStatus )
            {
                break;
            }
        }
    }

    //
    // Return status based on whether the power control memory status has reached the desired state.
    //
    if ( ( PWRCTRL->MEMPWRSTATUS & ui32MemStatusMask) ==
           am_hal_pwrctrl_memory_control[eMemConfig].ui32MemoryStatus )
    {
        return AM_HAL_STATUS_SUCCESS;
    }
    else
    {
        return AM_HAL_STATUS_FAIL;
    }
}

// ****************************************************************************
//
//  am_hal_pwrctrl_memory_deepsleep_powerdown()
//  Power down respective memory.
//
// ****************************************************************************
uint32_t
am_hal_pwrctrl_memory_deepsleep_powerdown(am_hal_pwrctrl_mem_e eMemConfig)
{
    if ( eMemConfig >= AM_HAL_PWRCTRL_MEM_MAX )
    {
        return AM_HAL_STATUS_FAIL;
    }

    //
    // Power down the required memory.
    //
    PWRCTRL->MEMPWDINSLEEP |= am_hal_pwrctrl_memory_control[eMemConfig].ui32PwdSlpEnable;

    return AM_HAL_STATUS_SUCCESS;
}

// ****************************************************************************
//
//  am_hal_pwrctrl_memory_deepsleep_retain()
//  Apply retention voltage to respective memory.
//
// ****************************************************************************
uint32_t
am_hal_pwrctrl_memory_deepsleep_retain(am_hal_pwrctrl_mem_e eMemConfig)
{
    if ( eMemConfig >= AM_HAL_PWRCTRL_MEM_MAX )
    {
        return AM_HAL_STATUS_FAIL;
    }

    //
    // Retain the required memory.
    //
    PWRCTRL->MEMPWDINSLEEP &= ~am_hal_pwrctrl_memory_control[eMemConfig].ui32PwdSlpEnable;

    return AM_HAL_STATUS_SUCCESS;
}

// ****************************************************************************
//
//  am_hal_pwrctrl_low_power_init()
//  Initialize system for low power configuration.
//
// ****************************************************************************
uint32_t
am_hal_pwrctrl_low_power_init(void)
{
    uint32_t      ui32Status;

    //
    // Take a snapshot of the reset status, if not done already
    //
    if (!gAmHalResetStatus)
    {
        gAmHalResetStatus = RSTGEN->STAT;
    }

    //
    // Software workaround for Errata ERR019.
    //
    if ((APOLLO3_A1) && (1 == PWRCTRL->SUPPLYSTATUS_b.SIMOBUCKON))
    {
      ui32Status = am_hal_pwrctrl_periph_enable(AM_HAL_PWRCTRL_PERIPH_PDM);
      if (AM_HAL_STATUS_SUCCESS != ui32Status)
      {
        return ui32Status;
      }
    }

    //
    // Adjust the SIMOBUCK LP settings.
    //
    if (APOLLO3_GE_B0)
    {
        MCUCTRL->SIMOBUCK2_b.SIMOBUCKCORELPHIGHTONTRIM  = 2;
        MCUCTRL->SIMOBUCK2_b.SIMOBUCKCORELPLOWTONTRIM   = 3;
        MCUCTRL->SIMOBUCK3_b.SIMOBUCKCORELPHIGHTOFFTRIM = 5;
        MCUCTRL->SIMOBUCK3_b.SIMOBUCKCORELPLOWTOFFTRIM  = 2;
        MCUCTRL->SIMOBUCK3_b.SIMOBUCKMEMLPHIGHTOFFTRIM  = 6;
        MCUCTRL->SIMOBUCK3_b.SIMOBUCKMEMLPLOWTOFFTRIM   = 1;
        MCUCTRL->SIMOBUCK3_b.SIMOBUCKMEMLPHIGHTONTRIM   = 3;
        MCUCTRL->SIMOBUCK4_b.SIMOBUCKMEMLPLOWTONTRIM    = 3;
    }

    //
    // Adjust the SIMOBUCK Timeout settings.
    //
    if (APOLLO3_GE_A1)
    {
        MCUCTRL->SIMOBUCK4_b.SIMOBUCKCOMP2TIMEOUTEN = 0;
    }

    //
    // Configure cache for low power and performance.
    //
    am_hal_cachectrl_control(AM_HAL_CACHECTRL_CONTROL_LPMMODE_RECOMMENDED, 0);

    //
    // Check if the BLE is already enabled.
    //
    if ( PWRCTRL->DEVPWRSTATUS_b.BLEL == 0)
    {
        //
        // First request the BLE feature and check that it was available and acknowledged.
        //
        MCUCTRL->FEATUREENABLE_b.BLEREQ = 1;
        ui32Status = am_hal_flash_delay_status_check(10000,
                        (uint32_t)&MCUCTRL->FEATUREENABLE,
                        (MCUCTRL_FEATUREENABLE_BLEAVAIL_Msk |
                          MCUCTRL_FEATUREENABLE_BLEACK_Msk  |
                          MCUCTRL_FEATUREENABLE_BLEREQ_Msk),
                        (MCUCTRL_FEATUREENABLE_BLEAVAIL_Msk |
                         MCUCTRL_FEATUREENABLE_BLEACK_Msk   |
                         MCUCTRL_FEATUREENABLE_BLEREQ_Msk),
                         true);

        if (AM_HAL_STATUS_SUCCESS != ui32Status)
        {
            return AM_HAL_STATUS_TIMEOUT;
        }

        //
        // Next, enable the BLE Buck.
        //
        PWRCTRL->SUPPLYSRC |= _VAL2FLD(PWRCTRL_SUPPLYSRC_BLEBUCKEN,
                                       PWRCTRL_SUPPLYSRC_BLEBUCKEN_EN);

        //
        // Allow the buck to go to low power mode in BLE sleep.
        //
        PWRCTRL->MISC |= _VAL2FLD(PWRCTRL_MISC_MEMVRLPBLE,
                                  PWRCTRL_MISC_MEMVRLPBLE_EN);

        //
        // Check for Apollo3 A0 Silicon.
        //
        if ( APOLLO3_A0 )
        {
            // Disable SIMO Buck clkdiv because if ble is out of reset then the same bit divides the simobuck clk too aggressively.
            MCUCTRL->SIMOBUCK4_b.SIMOBUCKCLKDIVSEL = 0x0;
            MCUCTRL->BLEBUCK2_b.BLEBUCKTONHITRIM   = 0xF;
            MCUCTRL->BLEBUCK2_b.BLEBUCKTONLOWTRIM  = 0xF;
        }
    }

    return AM_HAL_STATUS_SUCCESS;
}

void am_hal_pwrctrl_blebuck_trim(void)
{
  //
  // Enable the BLE buck trim values
  //
  if ( APOLLO3_GE_A1 )
  {
    AM_CRITICAL_BEGIN
    MCUCTRL->BLEBUCK2_b.BLEBUCKTONHITRIM = 0x19;
    MCUCTRL->BLEBUCK2_b.BLEBUCKTONLOWTRIM = 0xC;
    CLKGEN->BLEBUCKTONADJ_b.TONADJUSTEN = CLKGEN_BLEBUCKTONADJ_TONADJUSTEN_DIS;
    AM_CRITICAL_END
  }

}

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
