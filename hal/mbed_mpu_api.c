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
#include "cmsis.h"

void mbed_mpu_init()
{
    mbed_mpu_enable_ram_xn(false);
}

void mbed_mpu_free()
{
    mbed_mpu_enable_ram_xn(false);
}

void mbed_mpu_enable_ram_xn(bool enable)
{
#if MBED_MPU_ENABLED
    // Flush memory writes before configuring the MPU.
    __DSB();

    const uint32_t regions = (MPU->TYPE & MPU_TYPE_DREGION_Msk) >> MPU_TYPE_DREGION_Pos;

    // Disable the MCU
    MPU->CTRL = 0;

    // Reset all mapping
    for (uint32_t i = 0; i < regions; i++) {
        ARM_MPU_ClrRegion(i);
    }

    if (!enable) {
        return;
    }

    /*
     * ARMv6m and ARMv7m memory map:
     *
     * Start        End            Name            Executable by default        Mbed MPU protection
     * 0x00000000 - 0x1FFFFFFF     Code            Yes                          Execute disabled for second half
     * 0x20000000 - 0x3FFFFFFF     SRAM            Yes                          Execute disabled
     * 0x40000000 - 0x5FFFFFFF     Peripheral      No
     * 0x60000000 - 0x7FFFFFFF     RAM             Yes                          Execute disabled
     * 0x80000000 - 0x9FFFFFFF     RAM             Yes                          Execute disabled
     * 0xA0000000 - 0xBFFFFFFF     Device          No
     * 0xC0000000 - 0xDFFFFFFF     Device          No
     * 0xE0000000 - 0xFFFFFFFF     System          No
     */

    if (regions >= 3) {
        // Select region 0 and used it for the WT rom region
        // - RAM 0x10000000 - 0x1FFFFFFF
        MPU->RNR = 0;
        // Set address to 0
        MPU->RBAR = 0;
        // Configure and enable region
        MPU->RASR =
            MPU_RASR_ENABLE_Msk |
            ARM_MPU_RASR(
                1,                          // DisableExec
                ARM_MPU_AP_FULL,            // AccessPermission
                0,                          // TypeExtField
                0,                          // IsShareable
                1,                          // IsCacheable
                0,                          // IsBufferable
                                            // SubRegionDisable
                (1 << 0) |     // Disable Sub-region
                (1 << 1) |     // Disable Sub-region
                (1 << 2) |     // Disable Sub-region
                (1 << 3) |     // Disable Sub-region
                (0 << 4) |     // Enable Sub-region RAM 0x10000000 - 0x1FFFFFFF
                (0 << 5) |
                (0 << 6) |
                (0 << 7),
                ARM_MPU_REGION_SIZE_512MB   // Size
            );

        // Select region 1 and used it for WBWA ram regions
        // - SRAM 0x20000000 - 0x3FFFFFFF
        // - RAM  0x60000000 - 0x7FFFFFFF
        MPU->RNR = 1;
        // Set address to 0
        MPU->RBAR = 0;
        // Configure and enable region
        MPU->RASR =
            MPU_RASR_ENABLE_Msk |
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
                ARM_MPU_REGION_SIZE_4GB     // Size
            );

        // Select region 2 and used it for the WT ram region
        // - RAM RAM 0x80000000 - 0x9FFFFFFF
        MPU->RNR = 2;
        // Set address
        MPU->RBAR = 0x80000000;
        // Configure and enable region
        MPU->RASR =
            MPU_RASR_ENABLE_Msk |
            ARM_MPU_RASR(
                1,                          // DisableExec
                ARM_MPU_AP_FULL,            // AccessPermission
                0,                          // TypeExtField
                0,                          // IsShareable
                1,                          // IsCacheable
                0,                          // IsBufferable
                ~0U,                        // SubRegionDisable
                ARM_MPU_REGION_SIZE_512MB   // Size
            );
    }

    // Enable the MPU
    MPU->CTRL =
        (1 << MPU_CTRL_PRIVDEFENA_Pos) |            // Use the default memory map for unmapped addresses
        (1 << MPU_CTRL_HFNMIENA_Pos) |              // Keep MPU turned on for faults
        (1 << MPU_CTRL_ENABLE_Pos);                 // Enable MPU

    // Ensure changes take effect
    __ISB();
    __DSB();
#endif
}
