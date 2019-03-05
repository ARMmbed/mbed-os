/* Copyright (c) 2019 Unisoc Communications Inc.
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

#include "wland_ota.h"
#include "wland_flash.h"
#include "rda5981_ota.h"
#include "wland_dbg.h"
#include "wland_types.h"
#include "critical.h"
#include <stdbool.h>
#include <string.h>

#define IMAGE_MAGIC         0xAEAE

u32 wland_ota_partition_addr = 0;
u32 wland_ota_partition_len = 0;
u32 wland_crc_result = ~0UL;

static u32 crc32(const u8 *p, u32 len, u32 crc)
{
    const u32 *crc32_tab = (const u32 *)CRC32_TABLE_ADDR;

    if (rda_ccfg_hwver() >= 4) {
        crc32_tab = (const u32 *)CRC32_TABLE_ADDR_4;
    }
    /* Calculate CRC */
    while(len--) {
         crc = crc32_tab[((crc & 0xFF) ^ *p++)] ^ (crc >> 8);
    }

    return crc;
}

static int rda5981_ota_erase_flash(u32 addr, u32 len)
{
    addr &= (flash_size -1);
    rda5981_spi_erase_partition((void *)addr, len);
    return 0;
}
//write without erase
static int rda5981_ota_write_flash(u32 addr, char *buf, u32 len)
{
    int ret = 0;
    u8 *temp_buf = NULL, *temp_buf_aligned;

    addr &= (flash_size -1);
    if ((u32)buf % 4) {
        temp_buf = malloc(len + 3);
        if (temp_buf == NULL) {
            goto out;
        }
        if ((u32)temp_buf % 4) {
            temp_buf_aligned = temp_buf + (4-(u32)temp_buf%4);
        } else {
            temp_buf_aligned = temp_buf;
        }
        memcpy(temp_buf_aligned, (unsigned char *)buf, len);
    } else {
        temp_buf_aligned = (u8 *)buf;
    }
    core_util_critical_section_enter();
    RDA5991H_WRITE_FLASH(addr, temp_buf_aligned, len);
    core_util_critical_section_exit();

out:
    if (temp_buf) {
        free(temp_buf);
    }
    return ret;
}

int rda5981_write_partition_start(u32 addr, u32 img_len)
{
    if (addr < 0x18001000 || addr+img_len>0x18000000+flash_size) {
        WLAND_DBG(ERROR,"write partition start addr error. (0x%08x, %u)\r\n", addr, img_len);
        return -1;
    }
    if (addr%0x1000 || img_len%0x1000) {
        WLAND_DBG(ERROR,"write partition start length error.(mast be 4k alignment) (0x%08x, %u)\r\n", addr, img_len);
        return -1;
    }

    WLAND_DBG(INFO, "rda5981_write_partition_start:0x%08x, %u\r\n", addr, img_len);
    wland_ota_partition_addr = addr;
    wland_ota_partition_len = img_len;
    wland_crc_result = ~0U;

    rda5981_ota_erase_flash(addr, img_len);
    return 0;
}
int rda5981_write_partition(u32 offset, const u8 *buf, u32 len)
{
    if (wland_ota_partition_addr==0 || offset+len>wland_ota_partition_len) {
        WLAND_DBG(ERROR,"write partition error. out of start addr(0x%08x, %u). (0x%08x, %u)\r\n",
            wland_ota_partition_addr, wland_ota_partition_len, offset, len);
        return -1;
    }
    if (len%0x400) {
        WLAND_DBG(ERROR,"write partition length error.(mast be 1k alignment) (0x%08x, %u)\r\n", offset, len);
        return -1;
    }
    WLAND_DBG(DEBUG, "rda5981_write_partition:0x%08x, %u.(%02x)\r\n",
        wland_ota_partition_addr + offset, len, buf[0]);
    wland_crc_result = crc32(buf, len, wland_crc_result);

    WLAND_DBG(DEBUG, "rda5981_write_partition: wland_crc_result 0x%08x\r\n",
        wland_crc_result);
    return rda5981_ota_write_flash(wland_ota_partition_addr + offset, (char *)buf, len);
    //return rda5981_write_flash(wland_ota_partition_addr + offset, buf, len);
}
int rda5981_write_partition_end(void)
{
    WLAND_DBG(INFO, "check crc32:0x%08x, %u\r\n", wland_ota_partition_addr, wland_ota_partition_len);
    if (wland_ota_partition_addr == 0) {
        WLAND_DBG(ERROR,"OTA is not started\r\n");
        return -1;
    }
    core_util_critical_section_enter();
    spi_flash_flush_cache();
    //u32 crc32_check = crc32(wland_ota_partition_addr, wland_ota_partition_len, ~0U);
    u32 crc32_check = bootrom_crc32((unsigned char *)wland_ota_partition_addr, wland_ota_partition_len);
    core_util_critical_section_exit();
    WLAND_DBG(INFO, "rda5981_write_partition_end:0x%08x:0x%08x\r\n", wland_crc_result, crc32_check);
    wland_ota_partition_addr = 0UL;

    if (crc32_check == wland_crc_result) {
        return 0;
    } else {
        WLAND_DBG(ERROR,"check crc32 error\r\n");
        return -1;
    }
}
