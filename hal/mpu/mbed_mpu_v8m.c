/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#include "hal/mpu_api.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_error.h"
#include "cmsis.h"

#if ((__ARM_ARCH_8M_BASE__ == 1U) || (__ARM_ARCH_8M_MAIN__ == 1U)) && \
    defined (__MPU_PRESENT) && (__MPU_PRESENT == 1U) && \
    !defined(MBED_MPU_CUSTOM)

#if !DEVICE_MPU
#error "Device has v8m MPU but it is not enabled. Add 'MPU' to device_has in targets.json"
#endif

#if !defined(MBED_MPU_ROM_END)
#define MBED_MPU_ROM_END             (0x20000000 - 1)
#endif

MBED_STATIC_ASSERT(MBED_MPU_ROM_END == 0x1fffffff, "Changing MBED_MPU_ROM_END for ARMv8-M is not supported.");

void mbed_mpu_init()
{
    // Flush memory writes before configuring the MPU.
    __DSB();

    const uint32_t regions = (MPU->TYPE & MPU_TYPE_DREGION_Msk) >> MPU_TYPE_DREGION_Pos;
    if (regions < 4) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_HAL, MBED_ERROR_CODE_EINVAL), "Device is not capable of supporting an MPU - remove DEVICE_MPU for device_has.");
    }

    // Disable the MCU
    MPU->CTRL = 0;

    // Reset all mapping
    for (uint32_t i = 0; i < regions; i++) {
        ARM_MPU_ClrRegionEx(MPU, i);
    }

    /*
     * ARMv8-M memory map:
     *
     * Start        End            Name            Executable by default    Default cache       Mbed MPU protection
     * 0x00000000 - 0x1FFFFFFF     Code            Yes                      WT, WA              Write disabled
     * 0x20000000 - 0x3FFFFFFF     SRAM            Yes                      WB, WA, RA          Execute disabled
     * 0x40000000 - 0x5FFFFFFF     Peripheral      No
     * 0x60000000 - 0x7FFFFFFF     RAM             Yes                      WB, WA, RA          Execute disabled
     * 0x80000000 - 0x9FFFFFFF     RAM             Yes                      WT, RA              Execute disabled
     * 0xA0000000 - 0xBFFFFFFF     Device          No
     * 0xC0000000 - 0xDFFFFFFF     Device          No
     * 0xE0000000 - 0xFFFFFFFF     System          No
     */

    uint32_t region;
    uint8_t outer;
    uint8_t inner;

    region = 0;
    MPU->RNR = region;
    outer = 0xA;    // Write-Through, Non-transient, Read-allocate
    inner = 0xA;    // Write-Through, Non-transient, Read-allocate
    ARM_MPU_SetMemAttrEx(MPU, region, (outer << 4) | (inner << 0));
    MPU->RBAR = (0x00000000 & MPU_RBAR_BASE_Msk) |      // Start address is 0x00000000
                (0 << MPU_RBAR_SH_Pos) |             // Not shareable
                (3 << MPU_RBAR_AP_Pos) |             // RO allowed by all privilege levels
                (0 << MPU_RBAR_XN_Pos);              // Execute Never disabled
    MPU->RLAR = (0x1FFFFFFF & MPU_RLAR_LIMIT_Msk) |     // Last address is 0x1FFFFFFF
                (region << MPU_RLAR_AttrIndx_Pos) |  // Attribute index - configured to be the same as the region number
                (1 << MPU_RLAR_EN_Pos);              // Region enabled

    region = 1;
    MPU->RNR = region;
    outer = 0xF;    // Write-Back, Non-transient, Read-allocate, Write-allocate
    outer = 0xF;    // Write-Back, Non-transient, Read-allocate, Write-allocate
    ARM_MPU_SetMemAttrEx(MPU, region, (outer << 4) | (inner << 0));
    MPU->RBAR = (0x20000000 & MPU_RBAR_BASE_Msk) |      // Start address is 0x20000000
                (0 << MPU_RBAR_SH_Pos) |             // Not shareable
                (1 << MPU_RBAR_AP_Pos) |             // RW allowed by all privilege levels
                (1 << MPU_RBAR_XN_Pos);              // Execute Never enabled
    MPU->RLAR = (0x3FFFFFFF & MPU_RLAR_LIMIT_Msk) |     // Last address is 0x3FFFFFFF
                (region << MPU_RLAR_AttrIndx_Pos) |  // Attribute index - configured to be the same as the region number
                (1 << MPU_RLAR_EN_Pos);              // Region enabled

    region = 2;
    MPU->RNR = region;
    outer = 0xF;    // Write-Back, Non-transient, Read-allocate, Write-allocate
    outer = 0xF;    // Write-Back, Non-transient, Read-allocate, Write-allocate
    ARM_MPU_SetMemAttrEx(MPU, region, (outer << 4) | (inner << 0));
    MPU->RBAR = (0x60000000 & MPU_RBAR_BASE_Msk) |      // Start address is 0x60000000
                (0 << MPU_RBAR_SH_Pos) |             // Not shareable
                (1 << MPU_RBAR_AP_Pos) |             // RW allowed by all privilege levels
                (1 << MPU_RBAR_XN_Pos);              // Execute Never enabled
    MPU->RLAR = (0x7FFFFFFF & MPU_RLAR_LIMIT_Msk) |     // Last address is 0x7FFFFFFF
                (region << MPU_RLAR_AttrIndx_Pos) |  // Attribute index - configured to be the same as the region number
                (1 << MPU_RLAR_EN_Pos);              // Region enabled

    region = 3;
    MPU->RNR = region;
    outer = 0xA;    // Write-Through, Non-transient, Read-allocate
    inner = 0xA;    // Write-Through, Non-transient, Read-allocate
    ARM_MPU_SetMemAttrEx(MPU, region, (outer << 4) | (inner << 0));
    MPU->RBAR = (0x80000000 & MPU_RBAR_BASE_Msk) |      // Start address is 0x80000000
                (0 << MPU_RBAR_SH_Pos) |             // Not shareable
                (1 << MPU_RBAR_AP_Pos) |             // RW allowed by all privilege levels
                (1 << MPU_RBAR_XN_Pos);              // Execute Never enabled
    MPU->RLAR = (0x9FFFFFFF & MPU_RLAR_LIMIT_Msk) |     // Last address is 0x9FFFFFFF
                (region << MPU_RLAR_AttrIndx_Pos) |  // Attribute index - configured to be the same as the region number
                (1 << MPU_RLAR_EN_Pos);              // Region enabled

    // Enable the MPU
    MPU->CTRL =
        (1 << MPU_CTRL_PRIVDEFENA_Pos) |            // Use the default memory map for unmapped addresses
        (1 << MPU_CTRL_HFNMIENA_Pos) |              // Keep MPU turned on for faults
        (1 << MPU_CTRL_ENABLE_Pos);                 // Enable MPU

    // Ensure changes take effect
    __ISB();
    __DSB();
}

void mbed_mpu_free()
{
    // Flush memory writes before configuring the MPU.
    __DSB();

    // Disable the MCU
    MPU->CTRL = 0;

    // Ensure changes take effect
    __ISB();
    __DSB();
}

void mbed_mpu_enable_rom_wn(bool enable)
{
    // Flush memory writes before configuring the MPU.
    __DSB();

    MPU->RNR = 0;
    MPU->RLAR = (MPU->RLAR & ~MPU_RLAR_EN_Msk) | (enable ? MPU_RLAR_EN_Msk : 0);

    // Ensure changes take effect
    __ISB();
    __DSB();
}

void mbed_mpu_enable_ram_xn(bool enable)
{
    // Flush memory writes before configuring the MPU.
    __DSB();

    MPU->RNR = 1;
    MPU->RLAR = (MPU->RLAR & ~MPU_RLAR_EN_Msk) | (enable ? MPU_RLAR_EN_Msk : 0);

    MPU->RNR = 2;
    MPU->RLAR = (MPU->RLAR & ~MPU_RLAR_EN_Msk) | (enable ? MPU_RLAR_EN_Msk : 0);

    MPU->RNR = 3;
    MPU->RLAR = (MPU->RLAR & ~MPU_RLAR_EN_Msk) | (enable ? MPU_RLAR_EN_Msk : 0);

    // Ensure changes take effect
    __ISB();
    __DSB();
}

#endif
