/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "flash_api.h"
#include "flash_data.h"
#include "mbed_critical.h"
#include "mbed_toolchain.h"

#ifndef DOMAIN_NS

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#include <arm_cmse.h>
#endif

#define MBED_FLASH_ALGO_ERASE   1UL
#define MBED_FLASH_ALGO_PROGRAM 2UL

extern uint32_t SystemCoreClock;

/*
    This binary blob (thumb code) sets r9 (static base) as the code we are jumping to
    is PIC (position independent code).

    These are the instructions (r0 is a pointer to arg_t):
        push {r5, lr, r4}
        mov r5, r9
        push {r5}
        ldr r5, [r0, #20]
        ldr r3, [r0, #16]
        mov r9, r3
        ldr r3, [r0, #12]
        ldr r2, [r0, #8]
        ldr r1, [r0, #4]
        ldr r0, [r0, #0]
        blx r5
        pop {r5}
        mov r9, r5
        pop {r4-r5, pc}
        bx r14
*/
static uint32_t jump_to_flash_algo[] = {
    0x464DB530,
    0x6945B420,
    0x46996903,
    0x688268C3,
    0x68006841,
    0xBC2047A8,
    0xBD3046A9
};

// should be called within critical section
static int32_t flash_algo_init(flash_t *obj, uint32_t address, uint32_t function)
{
    args_t arguments = {
        .r0 = address,
        .r1 = SystemCoreClock,
        .r2 = function,
        .r3 = 0,
        .r9 = (uint32_t)obj->flash_algo->algo_blob + obj->flash_algo->static_base,
        .pc = (uint32_t)obj->flash_algo->algo_blob + obj->flash_algo->init
    };
    return ((flash_algo_jump_t)(((uint32_t)&jump_to_flash_algo) | 1))(&arguments);
}

// should be called within critical section
static int32_t flash_algo_uninit(flash_t *obj, uint32_t address, uint32_t function)
{
    args_t arguments = {
        .r0 = address,
        .r1 = SystemCoreClock,
        .r2 = function,
        .r3 = 0,
        .r9 = (uint32_t)obj->flash_algo->algo_blob + obj->flash_algo->static_base,
        .pc = (uint32_t)obj->flash_algo->algo_blob + obj->flash_algo->uninit
    };
    return ((flash_algo_jump_t)(((uint32_t)&jump_to_flash_algo) | 1))(&arguments);
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/* Check if address range [start_addr, end_addr] is in non-secure flash
 *
 *  @param obj          The flash object
 *  @param start_addr   Start address to check
 *  @param end_addr     End address to check. Could be the same as start_addr to just check start_addr
 *                      for e.g. flash_erase_sector.
 *  @return             0 for success, -1 for error
 */ 
static int32_t flash_check_nonsecure(flash_t *obj, uint32_t start_addr, uint32_t end_addr)
{
    /* Check if end address wraps around */
    if (end_addr < start_addr) {
        return -1;
    }

    /* Check if start address is in non-secure flash */
    if ((start_addr < obj->target_config_ns->flash_start) ||
        (start_addr >= (obj->target_config_ns->flash_start + obj->target_config_ns->flash_size))) {
        return -1;
    }

    /* Check if end address is in non-secure flash */
    if (end_addr != start_addr) {
        if ((end_addr < obj->target_config_ns->flash_start) ||
            (end_addr >= (obj->target_config_ns->flash_start + obj->target_config_ns->flash_size))) {
            return -1;
        }
    }

    return 0;
}
#endif

MBED_NONSECURE_ENTRY int32_t flash_init(flash_t *obj)
{
    flash_set_target_config(obj);
    return 0;
}

MBED_NONSECURE_ENTRY int32_t flash_free(flash_t *obj)
{
    return 0;
}

MBED_NONSECURE_ENTRY int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    if (cmse_nonsecure_caller()) {
        // Confine non-secure access to non-secure flash
        if (flash_check_nonsecure(obj, address, address)) {
            return -1;
        }
    }
#endif

    core_util_critical_section_enter();
    flash_algo_init(obj, address, MBED_FLASH_ALGO_ERASE);

    args_t arguments = {
        .r0 = address,
        .r1 = 0,
        .r2 = 0,
        .r3 = 0,
        .r9 = (uint32_t)obj->flash_algo->algo_blob + obj->flash_algo->static_base,
        .pc = (uint32_t)obj->flash_algo->algo_blob + obj->flash_algo->erase_sector
    };
    int32_t ret = ((flash_algo_jump_t)(((uint32_t)&jump_to_flash_algo) | 1))(&arguments);

    flash_algo_uninit(obj, address, MBED_FLASH_ALGO_ERASE);
    core_util_critical_section_exit();
    return ret ? -1 : 0;
}

MBED_NONSECURE_ENTRY int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    if (cmse_nonsecure_caller()) {
        // Confine non-secure access to non-secure flash
        if (flash_check_nonsecure(obj, address, address + size - 1)) {
            return -1;
        }
    }
#endif

    core_util_critical_section_enter();
    flash_algo_init(obj, address, MBED_FLASH_ALGO_PROGRAM);

    args_t arguments = {
        .r0 = address,
        .r1 = size,
        .r2 = (uint32_t)data,
        .r3 = 0,
        .r9 = (uint32_t)obj->flash_algo->algo_blob + obj->flash_algo->static_base,
        .pc = (uint32_t)obj->flash_algo->algo_blob + obj->flash_algo->program_page
    };
    int32_t ret = ((flash_algo_jump_t)(((uint32_t)&jump_to_flash_algo) | 1))(&arguments);

    flash_algo_uninit(obj, address, MBED_FLASH_ALGO_PROGRAM);
    core_util_critical_section_exit();
    return ret ? -1 : 0;
}

MBED_NONSECURE_ENTRY uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    if (cmse_nonsecure_caller()) {
        const sector_info_t *sectors = obj->target_config_ns->sectors;

        if (address >= obj->target_config_ns->flash_start + obj->target_config_ns->flash_size) {
            return MBED_FLASH_INVALID_SIZE;
        }

        int sector_index = obj->target_config_ns->sector_info_count - 1;
        for (; sector_index >= 0; sector_index--) {
            if (address >= sectors[sector_index].start) {
                return sectors[sector_index].size;
            }
        }
        return MBED_FLASH_INVALID_SIZE;
    }
#endif

    const sector_info_t *sectors = obj->target_config->sectors;

    if (address >= obj->target_config->flash_start + obj->target_config->flash_size) {
        return MBED_FLASH_INVALID_SIZE;
    }

    int sector_index = obj->target_config->sector_info_count - 1;
    for (; sector_index >= 0; sector_index--) {
        if (address >= sectors[sector_index].start) {
            return sectors[sector_index].size;
        }
    }
    return MBED_FLASH_INVALID_SIZE;
}

MBED_NONSECURE_ENTRY uint32_t flash_get_page_size(const flash_t *obj)
{
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    if (cmse_nonsecure_caller()) {
        return obj->target_config_ns->page_size;
    }
#endif

    return obj->target_config->page_size;
}

MBED_NONSECURE_ENTRY uint32_t flash_get_start_address(const flash_t *obj)
{
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    if (cmse_nonsecure_caller()) {
        return obj->target_config_ns->flash_start;
    }
#endif

    return obj->target_config->flash_start;
}

MBED_NONSECURE_ENTRY uint32_t flash_get_size(const flash_t *obj)
{
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    if (cmse_nonsecure_caller()) {
        return obj->target_config_ns->flash_size;
    }
#endif

    return obj->target_config->flash_size;
}

#endif  // #ifndef DOMAIN_NS
