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
    __DMB();

    const uint32_t regions = (MPU->TYPE & MPU_TYPE_DREGION_Msk) >> MPU_TYPE_DREGION_Pos;
    if (regions < 4) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_HAL, MBED_ERROR_CODE_EINVAL), "Device is not capable of supporting an MPU - remove DEVICE_MPU for device_has.");
    }

    // Disable the MCU
    MPU->CTRL = 0;

    // Reset all mapping
    for (uint32_t i = 0; i < regions; i++) {
        ARM_MPU_ClrRegion(i);
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

    const uint8_t WTRA = ARM_MPU_ATTR_MEMORY_(1, 0, 1, 0);      // Non-transient, Write-Through, Read-allocate, Not Write-allocate
    const uint8_t WBWARA = ARM_MPU_ATTR_MEMORY_(1, 1, 1, 1);    // Non-transient, Write-Back, Read-allocate, Write-allocate
    enum {
        AttrIndex_WTRA,
        AttrIndex_WBWARA,
    };

    ARM_MPU_SetMemAttr(AttrIndex_WTRA, ARM_MPU_ATTR(WTRA, WTRA));
    ARM_MPU_SetMemAttr(AttrIndex_WBWARA, ARM_MPU_ATTR(WBWARA, WBWARA));

    ARM_MPU_SetRegion(
        0,                          // Region
        ARM_MPU_RBAR(
            0x00000000,             // Base
            ARM_MPU_SH_NON,         // Non-shareable
            1,                      // Read-Only
            1,                      // Non-Privileged
            0),                     // Execute Never disabled
        ARM_MPU_RLAR(
            0x1FFFFFFF,             // Limit
            AttrIndex_WTRA)         // Attribute index - Write-Through, Read-allocate
    );

    ARM_MPU_SetRegion(
        1,                          // Region
        ARM_MPU_RBAR(
            0x20000000,             // Base
            ARM_MPU_SH_NON,         // Non-shareable
            0,                      // Read-Write
            1,                      // Non-Privileged
            1),                     // Execute Never enabled
        ARM_MPU_RLAR(
            0x3FFFFFFF,             // Limit
            AttrIndex_WBWARA)       // Attribute index - Write-Back, Write-allocate
    );

    ARM_MPU_SetRegion(
        2,                          // Region
        ARM_MPU_RBAR(
            0x60000000,             // Base
            ARM_MPU_SH_NON,         // Non-shareable
            0,                      // Read-Write
            1,                      // Non-Privileged
            1),                     // Execute Never enabled
        ARM_MPU_RLAR(
            0x7FFFFFFF,             // Limit
            AttrIndex_WBWARA)       // Attribute index - Write-Back, Write-allocate
    );

    ARM_MPU_SetRegion(
        3,                          // Region
        ARM_MPU_RBAR(
            0x80000000,             // Base
            ARM_MPU_SH_NON,         // Non-shareable
            0,                      // Read-Write
            1,                      // Non-Privileged
            1),                     // Execute Never enabled
        ARM_MPU_RLAR(
            0x9FFFFFFF,             // Limit
            AttrIndex_WTRA)         // Attribute index - Write-Through, Read-allocate
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

    // Disable the MCU
    MPU->CTRL = 0;

    // Ensure changes take effect
    __DSB();
    __ISB();
}

static void enable_region(bool enable, uint32_t region)
{
    MPU->RNR = region;
    MPU->RLAR = (MPU->RLAR & ~MPU_RLAR_EN_Msk) | (enable << MPU_RLAR_EN_Pos);
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

    for (uint32_t region = 1; region <= 3; region++) {
        enable_region(enable, region);
    }

    // Ensure changes take effect
    __DSB();
    __ISB();
}

#endif
