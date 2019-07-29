/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2017 All rights reserved
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
#include "mbed_critical.h"
#include "tmpm46b_fc.h"

#define PROGRAM_WIRTE_MAX             16U      // Page program could be written 16 bytes/4 words once
#define SECTOR_SIZE                   0x8000   // (512 * 8) sectors
#define PAGE_SIZE                     16U      // Page program size is 16 bytes

#if defined ( __ICCARM__ )    // IAR Compiler
#define FLASH_API_ROM           ((uint32_t *)__section_begin("FLASH_CODE_ROM"))
#define SIZE_FLASH_API          ((uint32_t)__section_size("FLASH_CODE_ROM"))
#define FLASH_API_RAM           ((uint32_t *)__section_begin("FLASH_CODE_RAM"))
#pragma section = "FLASH_CODE_RAM"
#pragma section = "FLASH_CODE_ROM"
#endif

#if defined ( __ICCARM__ )    // IAR Compiler
static void Copy_Routine(uint32_t *dest, uint32_t *source, uint32_t size)
{
    uint32_t *dest_addr, *source_addr, tmpsize;
    uint32_t i, tmps, tmpd, mask;

    dest_addr   = dest;
    source_addr = source;

    tmpsize = size >> 2U;
    for (i = 0U; i < tmpsize; i++) {    // 32bits copy
        *dest_addr = *source_addr;
        dest_addr++;
        source_addr++;
    }
    if (size & 0x00000003U) {   // if the last data size is not 0(maybe 1,2 or 3), copy the last data
        mask = 0xFFFFFF00U;
        i = size & 0x00000003U;
        tmps = *source_addr;
        tmpd = *dest_addr;
        while (i - 1U) {
            mask = mask << 8U;
            i--;
        }
        tmps = tmps & (~mask);
        tmpd = tmpd & (mask);
        *dest_addr = tmps + tmpd;       // 32bits copy, but only change the bytes need to be changed
    } else {
        // Do nothing
    }
}
#endif

int32_t flash_init(flash_t *obj)
{
    TSB_FC->WCLKCR  = 0x00000004U;
    TSB_FC->PROGCR  = 0x00000001U;
    TSB_FC->ERASECR = 0x00000007U;
    TSB_FC->AREASEL = 0x00000000U;
#if defined ( __ICCARM__ )    // IAR Compiler
    Copy_Routine(FLASH_API_RAM, FLASH_API_ROM, SIZE_FLASH_API);
#endif
    return 0;
}

int32_t flash_free(flash_t *obj)
{
    return 0;
}

#if defined ( __ICCARM__ )    // IAR Compiler
#pragma location = "FLASH_ROM"
#endif
int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    int status = FAIL;
    // We need to prevent flash accesses during erase operation
    core_util_critical_section_enter();

    if (FC_SUCCESS == FC_EraseBlock(address)) {
        status = SUCCESS;
    } else {
        // Do nothing
    }
    core_util_critical_section_exit();
    return status;
}

#if defined ( __ICCARM__ )    // IAR Compiler
#pragma location = "FLASH_ROM"
#endif
int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    int status = SUCCESS;

    // We need to prevent flash accesses during program operation
    core_util_critical_section_enter();
    while (size > PROGRAM_WIRTE_MAX) {
        if (FC_SUCCESS == FC_WritePage((uint32_t)address, (uint32_t *)data)) {      // write one page every time
            // Do nothing
        } else {
            status = FAIL;
            break;
        }
        size = size - PROGRAM_WIRTE_MAX;
        address = address + PROGRAM_WIRTE_MAX;
        data = data + PROGRAM_WIRTE_MAX;
    }
    if (FC_SUCCESS == FC_WritePage((uint32_t)address, (uint32_t *)data)) {  // write the last data, no more than one page
        // Do nothing
    } else {
        status = FAIL;
    }

    core_util_critical_section_exit();
    return status;
}

#if defined ( __ICCARM__ )    // IAR Compiler
#pragma location = "FLASH_ROM"
#endif
uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    if (address >= FLASH_CHIP_SIZE)
        return MBED_FLASH_INVALID_SIZE;

    return SECTOR_SIZE;
}

#if defined ( __ICCARM__ )    // IAR Compiler
#pragma location = "FLASH_ROM"
#endif
uint32_t flash_get_page_size(const flash_t *obj)
{
    return PAGE_SIZE;
}

#if defined ( __ICCARM__ )    // IAR Compiler
#pragma location = "FLASH_ROM"
#endif
uint32_t flash_get_start_address(const flash_t *obj)
{
    return FLASH_START_ADDR;
}

#if defined ( __ICCARM__ )    // IAR Compiler
#pragma location = "FLASH_ROM"
#endif
uint32_t flash_get_size(const flash_t *obj)
{
    return FLASH_CHIP_SIZE;
}

#if defined ( __ICCARM__ )    // IAR Compiler
#pragma location = "FLASH_ROM"
#endif
uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}
