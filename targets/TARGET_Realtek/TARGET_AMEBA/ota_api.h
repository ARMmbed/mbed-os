/* mbed Microcontroller Library
 * Copyright (c) 2013-2017 Realtek Semiconductor Corp.
 *
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
#ifndef MBED_OTA_API_H
#define MBED_OTA_API_H

#define FLASH_TOP            0x200000
#define FLASH_SECTOR_SIZE    0x1000
#define FLASH_SECTOR_MASK    ~(FLASH_SECTOR_SIZE - 1)

#define OTA_REGION1_BASE    0x40000
#define OTA_REGION2_BASE    0x120000
#define OTA_REGION1_SIZE    0xe0000
#define OTA_REGION2_SIZE    0xe0000
#define OTA_REGION_SIZE     0xe0000
#define OTA_MBED_FS_BASE    0xb000

#define OTA_CRC32_LEN       0x44
#define OTA_HEADER_LEN      0x48

#define OTA_HEADER_OFS      0x0
#define OTA_TAG_OFS         0x0
#define OTA_VER_OFS         0x4
#define OTA_EPOCH_OFS       0x8
#define OTA_SIZE_OFS        0x10
#define OTA_HASH_OFS        0x14
#define OTA_CAMPAIGN_OFS    0x34
#define OTA_CRC32_OFS       0x44
#define OTA_IMAGE_OFS       0x48

#define OTA_TAG_ID          0x81950001
#define OTA_VER_ID          0x81950001

#define OTA_TAG_CHIP_MSK    0xFFFF0000
#define OTA_TAG_INFO_MSK    0x0000FFFF

typedef struct imginfo_s {
    uint32_t tag;
    uint32_t ver;
    uint64_t timestamp;
    uint32_t size;
    uint8_t hash[32];
    uint8_t campaign[16];
    uint32_t crc32;
    bool valid;
} imginfo_t;

#ifdef __cplusplus
extern "C" {
#endif

extern void OTA_GetImageInfo(uint32_t base, imginfo_t *info);
extern uint32_t OTA_GetUpdateBase(void);

extern uint32_t OTA_UpdateHeader(uint32_t base, imginfo_t *img);
extern uint32_t OTA_UpdateImage(uint32_t base, uint32_t offset, uint32_t len, uint8_t *data);
extern void OTA_ReadHeader(uint32_t base, imginfo_t *img);
extern uint32_t OTA_ReadImage(uint32_t base, uint32_t offset, uint32_t len, uint8_t *data);
extern void OTA_ResetTarget(void);

#ifdef __cplusplus
}
#endif

#endif /* MBED_OTA_API_H */
