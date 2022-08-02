/* mbed Microcontroller Library
 * Copyright(C) TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2021
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
#include "flash_api.h"
#include "mbed_critical.h"
#include "flash.h"

#define PROGRAM_WRITE_MAX               (16U)           // Page program could be written 16 bytes/4 words once
#define SECTOR_SIZE                     (0x8000)        // 32KB each sectors or block
#define FLASH_CHIP_SIZE                 (0x00040000)    // Flash chip size is 2048 KByte
#define MASK_CHIP_ID_FROM_ADD           (0x00FFFFFFUL)
#define FC_MAX_BLOCKS           (uint8_t)(0x16)             /*!< Maxmum blocks */

#define SUCCESS                         (0U)
#define FAIL                            (-1)
// IHOSC1EN
#define CGOSCCR_IHOSC1EN_MASK           ((uint32_t)0x00000001)  // IHOSC1EN  :Mask
#define CGOSCCR_IHOSC1EN_RW_DISABLE     ((uint32_t)0x00000000)  // IHOSC1EN  :[R/W] :Disable
#define CGOSCCR_IHOSC1EN_RW_ENABLE      ((uint32_t)0x00000001)  // IHOSC1EN  :[R/W] :Enable

static void internal_hosc_enable(void);
static int16_t flash_get_block_no(const flash_t *obj, uint32_t address);

static uint32_t flash_block_address[FC_MAX_BLOCKS] = {
    (0x5E000000UL),   /*!< CODE FLASH Block0  */
    (0x5E008000UL),   /*!< CODE FLASH Block1  */
    (0x5E010000UL),   /*!< CODE FLASH Block2  */
    (0x5E018000UL),   /*!< CODE FLASH Block3  */
    (0x5E020000UL),   /*!< CODE FLASH Block4  */
    (0x5E028000UL),   /*!< CODE FLASH Block5  */
    (0x5E030000UL),   /*!< CODE FLASH Block6  */
    (0x5E038000UL),   /*!< CODE FLASH Block7  */
    (0x5E040000UL),   /*!< CODE FLASH Block8  */
    (0x5E048000UL),   /*!< CODE FLASH Block9  */
    (0x5E050000UL),   /*!< CODE FLASH Block10 */
    (0x5E058000UL),   /*!< CODE FLASH Block11 */
    (0x5E060000UL),   /*!< CODE FLASH Block12 */
    (0x5E068000UL),   /*!< CODE FLASH Block13 */
    (0x5E070000UL),   /*!< CODE FLASH Block14 */
    (0x5E078000UL)    /*!< CODE FLASH Block15 */
};


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
    int16_t block_no;

    if (obj->flash_inited == 0) {
        flash_init(obj);
    }

    // We need to prevent flash accesses during erase operation
    core_util_critical_section_enter();

    block_no = flash_get_block_no(obj, address);
    if (block_no == FAIL) {
        return status;
    } else {
        //Continue
    }

    if (TXZ_SUCCESS == fc_erase_block_code_flash(block_no, 1)) {
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

    if (TXZ_SUCCESS == fc_write_code_flash((uint32_t *)data, (uint32_t *)address, size)) {
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
    return PROGRAM_WRITE_MAX;
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

static int16_t flash_get_block_no(const flash_t *obj, uint32_t address)
{
    (void)obj;
    for (int i = 0 ; i < FC_MAX_BLOCKS; i++) {
        if (flash_block_address[i] == address) {
            return i;
        }
    }
    return FAIL;

}
