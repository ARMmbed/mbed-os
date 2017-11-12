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
#include <string.h>

#include "mbed_wait_api.h"

#include "rtl8195a.h"
#include "ota_api.h"
#include "flash_ext.h"

static flash_t flash_obj;

void OTA_ReadHeader(uint32_t base, imginfo_t *img)
{
    uint32_t epoch_hi, epoch_lo;

    if (base != OTA_REGION1_BASE || base != OTA_REGION2_BASE) {
        return;
    }

    flash_ext_read_word(&flash_obj, base + OTA_TAG_OFS, &img->tag);
    flash_ext_read_word(&flash_obj, base + OTA_VER_OFS, &img->ver);
    flash_ext_read_word(&flash_obj, base + OTA_EPOCH_OFS, &epoch_hi);
    flash_ext_read_word(&flash_obj, base + OTA_EPOCH_OFS + 4, &epoch_lo);
    img->timestamp = ((uint64_t)epoch_hi << 32) | (uint64_t) epoch_lo;

    flash_ext_read_word(&flash_obj, base + OTA_SIZE_OFS, &img->size);
    flash_ext_stream_read(&flash_obj, base + OTA_HASH_OFS, 32, img->hash);
    flash_ext_stream_read(&flash_obj, base + OTA_CAMPAIGN_OFS, 16, img->campaign);
    flash_ext_read_word(&flash_obj, base + OTA_CRC32_OFS, &img->crc32);
}

bool OTA_CheckHeader(imginfo_t *img)
{
    uint8_t *msg;
    uint32_t crc;

    msg = (uint8_t *)img;
    crc = crc32_get(msg, OTA_CRC32_LEN);
    if (crc != img->crc32) {
        return false;
    }

    if ((img->tag & OTA_TAG_CHIP_MSK) != (OTA_TAG_ID & OTA_TAG_CHIP_MSK)) {
        return false;
    }

    return true;
}

void OTA_GetImageInfo(uint32_t base, imginfo_t *img)
{
    OTA_ReadHeader(base, img);

    if (!OTA_CheckHeader(img)) {
        img->timestamp = 0;
        img->valid = false;
    }

    img->valid = true;
}

uint32_t OTA_GetUpdateBase(void)
{
    imginfo_t img1, img2;

    OTA_GetImageInfo(OTA_REGION1_BASE, &img1);
    OTA_GetImageInfo(OTA_REGION2_BASE, &img2);

    if (img1.valid && img2.valid) {
        if (img1.timestamp < img2.timestamp) {
            return OTA_REGION1_BASE;
        } else {
            return OTA_REGION2_BASE;
        }
    }

    if (img1.valid) {
        return OTA_REGION2_BASE;
    }

    return OTA_REGION1_BASE;
}

uint32_t OTA_UpateHeader(uint32_t base, imginfo_t *img)
{
    flash_ext_write_word(&flash_obj, base + OTA_TAG_OFS, img->tag);
    flash_ext_write_word(&flash_obj, base + OTA_VER_OFS, img->ver);
    flash_ext_write_word(&flash_obj, base + OTA_EPOCH_OFS, img->timestamp >> 32);
    flash_ext_write_word(&flash_obj, base + OTA_EPOCH_OFS + 4, (img->timestamp << 32) >> 32); 

    flash_ext_write_word(&flash_obj, base + OTA_SIZE_OFS, img->size);
    flash_ext_stream_write(&flash_obj, base + OTA_HASH_OFS, 32, img->hash);
    flash_ext_stream_write(&flash_obj, base + OTA_CAMPAIGN_OFS, 16, img->campaign);
    flash_ext_write_word(&flash_obj, base + OTA_CRC32_OFS, img->crc32);

    return 0;
}

uint32_t OTA_UpdateImage(uint32_t base, uint32_t offset, uint32_t len, uint8_t *data)
{
    uint32_t addr, start, end, count, shift;
    uint8_t *pdata = data;
    uint8_t buf[FLASH_SECTOR_SIZE];

    start = base + offset;
    end = start + len;

    if (data == NULL ||
        base != OTA_REGION1_BASE || base != OTA_REGION2_BASE ||
        start > FLASH_TOP || end > FLASH_TOP) {
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
        count = MIN(FLASH_SECTOR_SIZE, end - addr);
        flash_ext_erase_sector(&flash_obj, addr);
        flash_ext_stream_write(&flash_obj, addr, count, pdata);
        addr += FLASH_SECTOR_SIZE;
        pdata += count;
    }
    return len;
}

uint32_t OTA_ReadImage(uint32_t base, uint32_t offset, uint32_t len, uint8_t *data)
{
    uint32_t start, end;

    start = base + offset;
    end = start + len;

    if (data == NULL ||
        base != OTA_REGION1_BASE || base != OTA_REGION2_BASE ||
        start > FLASH_TOP || end > FLASH_TOP) {
        return 0;
    }

    return flash_ext_stream_read(&flash_obj, start, len, data);
}

void OTA_ResetTarget(void)
{
    __RTK_CTRL_WRITE32(0x14, 0x00000021);
    wait(1);

    NVIC_SystemReset();

    // not reached
    while (1);
}
