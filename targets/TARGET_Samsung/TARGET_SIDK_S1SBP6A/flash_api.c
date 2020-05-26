/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#if DEVICE_FLASH
#include "flash_api.h"
#include "mbed_critical.h"
#include "cmsis.h"

#define BP6A_FLASH_WRITE_SIZE        4
#define BP6A_FLASH_PAGE_SIZE         2048
#define BP6A_FLASH_MEM_BASE          0
#define BP6A_FLASH_FULL_SIZE         (2*1024*1024)


int32_t flash_init(flash_t *obj)
{
    modifyreg32(BP_FLASH_CTRL_BASE + UFC_DCYCRDCON_OFFSET, 0xFFFF, 0x9F4); // flash wait setting
    return 0;
}


int32_t flash_free(flash_t *obj)
{
    return 0;
}

int32_t flash_erase_sector(flash_t *obj, uint32_t address)
{
    modifyreg32(BP_FLASH_CTRL_BASE + UFC_FRWCON0_OFFSET,
                UFC_FRWCON_WRMD_MASK |
                UFC_FRWCON_ERSMD_MASK,
                UFC_FRWCON_WRMD(ERASE_MODE) |
                UFC_FRWCON_ERSMD(SECTOR_ERASE_MODE));

    putreg32(address, 0xFFFFFFFF); //load dummy

    modifyreg32(BP_FLASH_CTRL_BASE + UFC_FRWCON0_OFFSET,
                UFC_FRWCON_HVEN_MASK,
                UFC_FRWCON_HVEN(1));

    while (getreg32(BP_FLASH_CTRL_BASE + UFC_FRWCON0_OFFSET) & (0x01 << UFC_FRWCON_HVEN_SHIFT));

    return 0;


}

int32_t flash_program_page(flash_t *obj, uint32_t address, const uint8_t *data, uint32_t size)
{
    uint32_t i;

    for (i = 0; i < size; i += 4) {
        modifyreg32(BP_FLASH_CTRL_BASE + UFC_FRWCON0_OFFSET,
                    UFC_FRWCON_WRMD_MASK,
                    UFC_FRWCON_WRMD(WRITE_MODE));
        *((uint32_t *)(address + i)) = (uint32_t)(data[i] | data[i + 1] << 8 |
                                                  data[i + 3] << 24 | data[i + 2] << 16);
        modifyreg32(BP_FLASH_CTRL_BASE + UFC_FRWCON0_OFFSET,
                    UFC_FRWCON_HVEN_MASK,
                    UFC_FRWCON_HVEN(1));
        while (getreg32(BP_FLASH_CTRL_BASE + UFC_FRWCON0_OFFSET) & (0x01 << UFC_FRWCON_HVEN_SHIFT));
    }
    return 0;
}


uint32_t flash_get_sector_size(const flash_t *obj, uint32_t address)
{
    /*  1 sector = 1 page */
    if (address >= (BP6A_FLASH_MEM_BASE + BP6A_FLASH_FULL_SIZE)) {
        return MBED_FLASH_INVALID_SIZE;
    } else {
        return BP6A_FLASH_PAGE_SIZE;
    }
}

uint32_t flash_get_page_size(const flash_t *obj)
{
    return BP6A_FLASH_WRITE_SIZE;
}

uint32_t flash_get_start_address(const flash_t *obj)
{
    return BP6A_FLASH_MEM_BASE;
}

uint32_t flash_get_size(const flash_t *obj)
{
    return BP6A_FLASH_FULL_SIZE;
}

uint8_t flash_get_erase_value(const flash_t *obj)
{
    UNUSED(obj);

    return 0xFF;
}

#endif
