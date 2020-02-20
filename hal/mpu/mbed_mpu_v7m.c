/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#include "hal/mpu_api.h"
#include "platform/mbed_assert.h"
#include "cmsis.h"

#if ((__ARM_ARCH_7M__ == 1U) || (__ARM_ARCH_7EM__ == 1U) || (__ARM_ARCH_6M__ == 1U)) && \
    defined (__MPU_PRESENT) && (__MPU_PRESENT == 1U) && \
    !defined(MBED_MPU_CUSTOM)

#if !DEVICE_MPU
#error "Device has v7m MPU but it is not enabled. Add 'MPU' to device_has in targets.json"
#endif

#ifdef MBED_CONF_TARGET_MPU_ROM_END
#define MBED_MPU_ROM_END             MBED_CONF_TARGET_MPU_ROM_END
#else
#define MBED_MPU_ROM_END             (0x10000000 - 1)
#endif
#define MBED_MPU_RAM_START           (MBED_MPU_ROM_END + 1)

MBED_STATIC_ASSERT(
    MBED_MPU_ROM_END == 0x04000000 - 1 ||
    MBED_MPU_ROM_END == 0x08000000 - 1 ||
    MBED_MPU_ROM_END == 0x0C000000 - 1 ||
    MBED_MPU_ROM_END == 0x10000000 - 1 ||
    MBED_MPU_ROM_END == 0x14000000 - 1 ||
    MBED_MPU_ROM_END == 0x18000000 - 1 ||
    MBED_MPU_ROM_END == 0x1C000000 - 1 ||
    MBED_MPU_ROM_END == 0x20000000 - 1,
    "Unsupported value for MBED_MPU_ROM_END");

void mbed_mpu_init()
{
    // Flush memory writes before configuring the MPU.
    __DMB();

    const uint32_t regions = (MPU->TYPE & MPU_TYPE_DREGION_Msk) >> MPU_TYPE_DREGION_Pos;

    // Our MPU setup requires 3 or 4 regions - if this assert is hit, remove
    // a region by setting MPU_ROM_END to 0x1fffffff, or remove MPU from device_has
#if MBED_MPU_RAM_START == 0x20000000
    MBED_ASSERT(regions >= 3);
#else
    MBED_ASSERT(regions >= 4);
#endif

    // Disable the MCU
    MPU->CTRL = 0;

    // Reset all mapping
    for (uint32_t i = 0; i < regions; i++) {
        ARM_MPU_ClrRegion(i);
    }

    /*
     * ARMv6m and ARMv7-M memory map:
     *
     * Start        End            Name            Executable by default        Mbed MPU protection
     * 0x00000000 - 0x1FFFFFFF     Code            Yes                          Write disabled for first portion and execute disabled for the rest
     * 0x20000000 - 0x3FFFFFFF     SRAM            Yes                          Execute disabled
     * 0x40000000 - 0x5FFFFFFF     Peripheral      No
     * 0x60000000 - 0x7FFFFFFF     RAM             Yes                          Execute disabled
     * 0x80000000 - 0x9FFFFFFF     RAM             Yes                          Execute disabled
     * 0xA0000000 - 0xBFFFFFFF     Device          No
     * 0xC0000000 - 0xDFFFFFFF     Device          No
     * 0xE0000000 - 0xFFFFFFFF     System          No
     */

    // Select region 0 and use it for the WT read-only rom region
    // - Code 0x00000000 to MBED_MPU_ROM_END
    ARM_MPU_SetRegion(
        ARM_MPU_RBAR(
            0,                          // Region
            0x00000000),                // Base
        ARM_MPU_RASR(
            0,                          // DisableExec
            ARM_MPU_AP_RO,              // AccessPermission
            0,                          // TypeExtField
            0,                          // IsShareable
            1,                          // IsCacheable
            0,                          // IsBufferable
            // SubRegionDisable - based on where ROM ends
            ((MBED_MPU_ROM_END >= 0x00000000) ? 0 : (1 << 0)) |    // 0 to enable, 1 << n to disable
            ((MBED_MPU_ROM_END >= 0x04000000) ? 0 : (1 << 1)) |
            ((MBED_MPU_ROM_END >= 0x08000000) ? 0 : (1 << 2)) |
            ((MBED_MPU_ROM_END >= 0x0C000000) ? 0 : (1 << 3)) |
            ((MBED_MPU_ROM_END >= 0x10000000) ? 0 : (1 << 4)) |
            ((MBED_MPU_ROM_END >= 0x14000000) ? 0 : (1 << 5)) |
            ((MBED_MPU_ROM_END >= 0x18000000) ? 0 : (1 << 6)) |
            ((MBED_MPU_ROM_END >= 0x1C000000) ? 0 : (1 << 7)),
            ARM_MPU_REGION_SIZE_512MB)  // Size
    );

#if MBED_MPU_RAM_START < 0x20000000
    // Select region 3 and use it for a WT ram region in the Code area
    // - Code MBED_MPU_ROM_END + 1 to 0x1FFFFFFF
    ARM_MPU_SetRegion(
        ARM_MPU_RBAR(
            3,                          // Region
            0x00000000),                // Base
        ARM_MPU_RASR(
            1,                          // DisableExec
            ARM_MPU_AP_FULL,            // AccessPermission
            0,                          // TypeExtField
            0,                          // IsShareable
            1,                          // IsCacheable
            0,                          // IsBufferable
            // SubRegionDisable - based on where RAM starts
            ((MBED_MPU_RAM_START <= 0x04000000) ? 0 : (1 << 0)) |    // 0 to enable, 1 << n to disable
            ((MBED_MPU_RAM_START <= 0x08000000) ? 0 : (1 << 1)) |
            ((MBED_MPU_RAM_START <= 0x0C000000) ? 0 : (1 << 2)) |
            ((MBED_MPU_RAM_START <= 0x10000000) ? 0 : (1 << 3)) |
            ((MBED_MPU_RAM_START <= 0x14000000) ? 0 : (1 << 4)) |
            ((MBED_MPU_RAM_START <= 0x18000000) ? 0 : (1 << 5)) |
            ((MBED_MPU_RAM_START <= 0x1C000000) ? 0 : (1 << 6)) |
            ((MBED_MPU_RAM_START <= 0x20000000) ? 0 : (1 << 7)),
            ARM_MPU_REGION_SIZE_512MB)  // Size
    );
#define LAST_RAM_REGION 3
#else
#define LAST_RAM_REGION 2
#endif

    // Select region 1 and use it for WBWA ram regions
    // - SRAM 0x20000000 to 0x3FFFFFFF
    // - RAM  0x60000000 to 0x7FFFFFFF
    ARM_MPU_SetRegion(
        ARM_MPU_RBAR(
            1,                          // Region
            0x00000000),                // Base
        ARM_MPU_RASR(
            1,                          // DisableExec
            ARM_MPU_AP_FULL,            // AccessPermission
            1,                          // TypeExtField
            0,                          // IsShareable
            1,                          // IsCacheable
            1,                          // IsBufferable
            // SubRegionDisable
            (1 << 0) |     // Disable Sub-region
            (0 << 1) |     // Enable Sub-region SRAM 0x20000000 - 0x3FFFFFFF
            (1 << 2) |     // Disable Sub-region
            (0 << 3) |     // Enable Sub-region RAM 0x60000000 - 0x7FFFFFFF
            (1 << 4) |     // Disable Sub-region
            (1 << 5) |     // Disable Sub-region
            (1 << 6) |     // Disable Sub-region
            (1 << 7),      // Disable Sub-region
            ARM_MPU_REGION_SIZE_4GB)    // Size
    );

    // Select region 2 and use it for the WT ram region
    // - RAM 0x80000000 to 0x9FFFFFFF
    ARM_MPU_SetRegion(
        ARM_MPU_RBAR(
            2,                          // Region
            0x80000000),                // Base
        ARM_MPU_RASR(
            1,                          // DisableExec
            ARM_MPU_AP_FULL,            // AccessPermission
            0,                          // TypeExtField
            0,                          // IsShareable
            1,                          // IsCacheable
            0,                          // IsBufferable
            0U,                         // SubRegionDisable
            ARM_MPU_REGION_SIZE_512MB)  // Size
    );

    // Enable the MPU
    MPU->CTRL =
        (1 << MPU_CTRL_PRIVDEFENA_Pos) |            // Use the default memory map for unmapped addresses
        (1 << MPU_CTRL_HFNMIENA_Pos) |              // Keep MPU turned on for faults
        (1 << MPU_CTRL_ENABLE_Pos);                 // Enable MPU

    // Ensure changes take effect
    __DSB();
    __ISB();
}

void mbed_mpu_free()
{
    // Flush memory writes before configuring the MPU.
    __DMB();

    // Disable the MPU
    MPU->CTRL = 0;

    // Ensure changes take effect
    __DSB();
    __ISB();
}

static void enable_region(bool enable, uint32_t region)
{
    MPU->RNR = region;
    MPU->RASR = (MPU->RASR & ~MPU_RASR_ENABLE_Msk) | (enable << MPU_RASR_ENABLE_Pos);
}

void mbed_mpu_enable_rom_wn(bool enable)
{
    // Flush memory writes before configuring the MPU.
    __DMB();

    enable_region(enable, 0);

    // Ensure changes take effect
    __DSB();
    __ISB();
}

void mbed_mpu_enable_ram_xn(bool enable)
{
    // Flush memory writes before configuring the MPU.
    __DMB();

    for (uint32_t region = 1; region <= LAST_RAM_REGION; region++) {
        enable_region(enable, region);
    }

    // Ensure changes take effect
    __DSB();
    __ISB();
}

#endif
