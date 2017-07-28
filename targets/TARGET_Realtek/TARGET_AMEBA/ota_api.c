/* mbed Microcontroller Library
 * Copyright (c) 2013-2017 Realtek Semiconductor Corp.
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
#include <stdio.h>
#include <string.h>

#include "mbed_wait_api.h"

#include "rtl8195a.h"
#include "flash_ext.h"

#define FLASH_TOP           0x200000
#define FLASH_SECTOR_SIZE   0x1000
#define FLASH_SECTOR_MASK   ~(FLASH_SECTOR_SIZE - 1)
#define OTA_REGION1         0x0b000
#define OTA_REGION2         0xc0000
#define TAG_OFS             0xc
#define VER_OFS             0x10

#define TAG_DOWNLOAD        0x81950001
#define TAG_VERIFIED        0x81950003

static flash_t flash_obj;

typedef struct imginfo_s {
    uint32_t base;
    uint32_t tag;
    uint64_t ver;
} imginfo_t;


void OTA_GetImageInfo(imginfo_t *info)
{
    uint32_t ver_hi, ver_lo;

    flash_ext_read_word(&flash_obj, info->base + TAG_OFS, &info->tag);
    flash_ext_read_word(&flash_obj, info->base + VER_OFS, &ver_lo);
    flash_ext_read_word(&flash_obj, info->base + VER_OFS + 4, &ver_hi);

    if (info->tag == TAG_DOWNLOAD) {
        info->ver = ((uint64_t)ver_hi << 32) | (uint64_t) ver_lo;
    } else {
        info->ver = 0;
    }
}

uint32_t OTA_GetBase(void)
{
    static uint32_t ota_base = 0;
    imginfo_t region1, region2;

    if (ota_base == OTA_REGION1 || ota_base == OTA_REGION2) {
        return ota_base;
    }

    region1.base = OTA_REGION1;
    region2.base = OTA_REGION2;

    OTA_GetImageInfo(&region1);
    OTA_GetImageInfo(&region2);

    if (region1.ver >= region2.ver) {
        ota_base = region2.base;
    } else {
        ota_base = region1.base;
    }
    return ota_base;
}

uint32_t OTA_MarkUpdateDone(void)
{
    uint32_t addr = OTA_GetBase() + TAG_OFS;

    return flash_ext_write_word(&flash_obj, addr, TAG_DOWNLOAD);
}

uint32_t OTA_UpdateImage(uint32_t offset, uint32_t len, uint8_t *data)
{
    uint32_t addr, start, end, count, shift;
    uint8_t *pdata = data;
    uint8_t buf[FLASH_SECTOR_SIZE];

    start = OTA_GetBase() + offset;
    end = start + len;

    if (data == NULL || start > FLASH_TOP || end > FLASH_TOP) {
        return 0;
    }

    addr = start & FLASH_SECTOR_MASK;
    if (addr != start) {
        shift = start - addr;
        count = MIN(FLASH_SECTOR_SIZE - shift, len);
        flash_ext_stream_read(&flash_obj, addr, shift, buf);
        memcpy((void *)(buf + shift), (void *)pdata, count);

        flash_ext_erase_sector(&flash_obj, addr);
        flash_ext_stream_write(&flash_obj, addr, FLASH_SECTOR_SIZE, buf);
        addr += FLASH_SECTOR_SIZE;
        pdata += count;
    }

    while (addr < end) {
        printf("OTA: update addr=0x%lx, len=%ld\r\n", addr, len);
        count = MIN(FLASH_SECTOR_SIZE, end - addr);
        flash_ext_erase_sector(&flash_obj, addr);
        flash_ext_stream_write(&flash_obj, addr, count, pdata);
        addr += FLASH_SECTOR_SIZE;
        pdata += count;
    }
    return len;
}

uint32_t OTA_ReadImage(uint32_t offset, uint32_t len, uint8_t *data)
{
    uint32_t addr, endaddr;

    addr = OTA_GetBase() + offset;
    endaddr = addr + len;

    if (data == NULL || addr > FLASH_TOP || endaddr > FLASH_TOP) {
        return 0;
    }

    printf("OTA: read addr=0x%lx\r\n", addr);
    return flash_ext_stream_read(&flash_obj, addr, len, data);
}

void OTA_ResetTarget(void)
{
    __RTK_CTRL_WRITE32(0x14, 0x00000021);
    wait(1);

    // write SCB->AIRCR
    HAL_WRITE32(0xE000ED00, 0x0C,
                (0x5FA << 16) |                               // VECTKEY
                (HAL_READ32(0xE000ED00, 0x0C) & (7 << 8)) | // PRIGROUP
                (1 << 2));                                    // SYSRESETREQ

    // not reached
    while (1);
}
