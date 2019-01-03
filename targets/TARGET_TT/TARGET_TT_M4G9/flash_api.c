/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
 * SPDX-License-Identifier: Apache-2.0
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
#include "tmpm4g9_fc.h"

#define PROGRAM_WIRTE_MAX                       (16U)           // Page program could be written 16 bytes/4 words once
#define SECTOR_SIZE                             (0x8000)        // 32KB each sectors or block
#define FLASH_CHIP_SIZE                         (0x00180000)    // Flash chip size is 1536KByte
#define MASK_CHIP_ID_FROM_ADD                   (0x00FFFFFFUL)

#define SUCCESS                                 (0U)
#define FAIL                                    (-1)
// IHOSC1EN
#define CGOSCCR_IHOSC1EN_MASK                   ((uint32_t)0x00000001)    // IHOSC1EN  :Mask
#define CGOSCCR_IHOSC1EN_RW_DISABLE             ((uint32_t)0x00000000)    // IHOSC1EN  :[R/W] :Disable
#define CGOSCCR_IHOSC1EN_RW_ENABLE              ((uint32_t)0x00000001)    // IHOSC1EN  :[R/W] :Enable

static void internal_hosc_enable(void);

int32_t flash_init(flash_t *obj)
{
    obj->flash_inited = 0;
    obj->flash_inited = 1;
    internal_hosc_enable();             // Internal HOSC enable
    return 0;
}

int32_t flash_free(flash_t *obj)
{
    obj->flash_inited = 0;

    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    int status = FAIL;

    if (obj->flash_inited == 0) {
        flash_init(obj);
    }

    // We need to prevent flash accesses during erase operation
    core_util_critical_section_enter();

    if (TXZ_SUCCESS == fc_erase_block_code_flash((uint32_t*)FC_CODE_FLASH_ADDRESS_TOP, (uint32_t*)address)) {
        status = SUCCESS;
    } else {
        // Do nothing
    }

    core_util_critical_section_exit();

    return status;
}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    int status = SUCCESS;

    address &= MASK_CHIP_ID_FROM_ADD;

    // We need to prevent flash accesses during program operation
    core_util_critical_section_enter();

    if (TXZ_SUCCESS == fc_write_code_flash((uint32_t*)data, (uint32_t*)address, size)) {
        // Do nothing
    } else {
        status = FAIL;
    }

    core_util_critical_section_exit();

    return status;
}

uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    if ((address >= FC_CODE_FLASH_ADDRESS_TOP) && (address < (FC_CODE_FLASH_ADDRESS_TOP + FLASH_CHIP_SIZE))) {
        return SECTOR_SIZE;
    } else {
        // Do nothing
    }

    return MBED_FLASH_INVALID_SIZE;
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    return PROGRAM_WIRTE_MAX;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    return FC_CODE_FLASH_ADDRESS_TOP;
}

uint32_t flash_get_size(const flash_t *obj)
{
    return FLASH_CHIP_SIZE;
}

static void internal_hosc_enable(void)
{
    uint32_t work;
    work = (uint32_t)(TSB_CG->OSCCR & ~CGOSCCR_IHOSC1EN_MASK);
    TSB_CG->OSCCR = (uint32_t)(work | CGOSCCR_IHOSC1EN_RW_ENABLE);
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    (void)obj;

    return 0xFF;
}
