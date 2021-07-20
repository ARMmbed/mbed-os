/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef __NFTL_H
#define __NFTL_H

#include "filesystem/mbed_filesystem.h"
#include "errno.h"
#include "MbedCRC.h"

#include "nftl_conf.h"
#include "nftl_util.h"

/**
 * Short description:
 *
 * NFTL (i.e. Nand Flash Translation Layer), is used to provide
 * logical-to-physical block mapping on Nand Flash.
 *
 * Detailed description:
 *
 * NFTL stores mapping info (16 bytes, see later chapter for detailed date format description)
 * in the spare area on each flash page.
 *
 * During boot, NFTL read back the mapping info, and restore the mapping table in RAM.
 *
 * NFTL uses the last page on each block to store the backup info of all pages' spare info,
 * to accelerate the scan process to setup the mapping table in RAM.
 *
 * Hereafter is detailed description of the NFTL data format on flash:
 *
 * 1. data+spare on each page:
 *
 *    --------------------------------
 *    | data(4KB) | spare(16B) + ECC |    -> page 0
 *    --------------------------------
 *    | data(4KB) | spare(16B) + ECC |    -> page 1
 *    --------------------------------
 *                .
 *                .
 *                .
 *
 * 2. data format in spare area:
 *
 *    |----------|----------|---------------------|----------------------------|
 *    0    LB    3   Rev    7     reserved       15  used by vendor (variable) |
 *
 *    - totally 16 bytes used by NFTL.
 *    - Logical Block index: 0 - 3 byte, this is the logical block number.
 *    - Revision number: 4 - 7 byte, this is the revision of the logical block.
 *                       RFC1982 serial bumber arithmetic used to avoid overflow..
 *    - Reserved: 8 - 16 byte, reserved for future use.
 *    - Other: used by vendor (ECC, bad block flag, etc.).
 *
 *    Note: 1. vendor area may be stored before or after nftl bytes, depending on
 *             how the vendor hal APIs (hal_flash_xxx) implemented.
 *          2. LB and Rev data is stored in little-endian format on flash.
 *             Please ensure endian conversion when read from to write
 *             to the flash area.
 *
 * 3. page format on each block:
 *
 *    |---------|-------|
 *    | data    | spare |    -> page 0
 *    |---------|-------|
 *    | data    | spare |    -> page 1
 *    |---------|-------|
 *    | data    | spare |    -> page 2
 *    |---------|-------|
 *      ...        ...       -> ...
 *    |---------|-------|
 *    |accl data|       |    -> last page (i.e. ACCELERATE page)
 *    |---------|-------|
 *
 *    The last page on each block is used to store backup/accelerate data
 *    of each page on this block. So the last page on each block is somehow
 *    wasted (no valid data stored). It's used to decrease the scan time to setup
 *    the logical-to-physical mapping table (LB and Rev data).
 *
 *    The data format on the ACCELERATE page:
 *
 *    0               7              15
 *    |---------------|---------------|
 *    | page0's spare | page1's spare |
 *    |---------------|---------------|
 *    | page2's spare | page3's spare |
 *    |---------------|---------------|
 *    | page4's spare | page5's spare |
 *    |---------------|---------------|
 *    |  accl page's  |               |
 *    |---------------|---------------|
 *    |    ...        |    ...        |
 *    |---------------|---------------|
 *
 *    Note: All data store on the accl page is in little-endian format.
 */

/* FTL error codes */
typedef enum {
    NFTL_ERR_OK,
    NFTL_ERR_INVALID_ARGUMENT,
    NFTL_ERR_NAND_BAD,
    NFTL_ERR_NAND_IO,
    NFTL_ERR_NAND_NO_PAGE,
    NFTL_ERR_NAND_ERASED_PENDING_WRITE,
    NFTL_ERR_NAND_UNKNOWN,
    NFTL_ERR_NAND_READONLY,
} nftl_err_t;

/* FTL logical parition index, support 3 for now, flexible to add more later */
typedef enum {
    NFTL_PARTITION0,
    NFTL_PARTITION1,
    NFTL_PARTITION2,
    NFTL_PARTITION_MAX
} nftl_partition_t;

/* FTL logical partition table information */
typedef struct {
    const char  *desc; /* description */
    uint32_t     blkoff; /* block start offset */
    uint32_t     blkcnt; /* block count */
} nftl_partition_info_t;

#define NFTL_BLKS_BITMAP_LEN     ((NFTL_WORK_BLOCK_NUMS + 7) / 8)

#define NFTL_PAGES_PER_BLK       64
#define NFTL_PAGE_BITMAP_LEN     ((NFTL_PAGES_PER_BLK + 7) / 8)

#define NFTL_TAG_PAGE_IN_BLOCK   (NFTL_PAGES_PER_BLK - 1)

#define NFTL_INVALID_BLOCK       0xFFFFFFFF

#define NFTL_SPARE_AREA_LEN      16

typedef uint32_t nftl_block_t;
typedef uint32_t nftl_offset_t;
typedef uint32_t nftl_size_t;

typedef struct {
    uint8_t                 page_bitmap[NFTL_PAGE_BITMAP_LEN];
} nftl_phy_block_info_t;

typedef struct {
    uint32_t                page_id;
    uint32_t                reversion;
} nftl_logic_info_t;

typedef struct {
    uint32_t                logic_block;
    uint32_t                reversion;
} nftl_block_tag;

/* hal flash partition define */
typedef enum {
    HAL_PARTITION_ERROR = -1,
    HAL_PARTITION_BOOTLOADER,   /**< Bootloader partition index */
    HAL_PARTITION_APPLICATION,  /**< App partition index; Or OTA A partition */
    HAL_PARTITION_ATE,          /**< For ATE */
    HAL_PARTITION_OTA_TEMP,     /**< For OTA upgrade */
    HAL_PARTITION_RF_FIRMWARE,  /**< For RF firmware */
    HAL_PARTITION_PARAMETER_1,  /**< For OTA args */
    HAL_PARTITION_PARAMETER_2,  /**< For kv storage */
    HAL_PARTITION_PARAMETER_3,  /**< For User defined */
    HAL_PARTITION_PARAMETER_4,  /**< Used by security */
    HAL_PARTITION_BT_FIRMWARE,  /**< For bt firmware */
    HAL_PARTITION_SPIFFS,       /**< For spiffs file system */
    HAL_PARTITION_LITTLEFS,
    HAL_PARTITION_LITTLEFS2,
    HAL_PARTITION_LITTLEFS3,
    HAL_PARTITION_CUSTOM_1,     /**< For User defined */
    HAL_PARTITION_CUSTOM_2,     /**< For User defined */
    HAL_PARTITION_2ND_BOOT,     /**< For 2nd boot */
    HAL_PARTITION_MBINS_APP,    /**< For app bin when muti bins */
    HAL_PARTITION_MBINS_KERNEL, /**< For kernel bin when muti bins */
    HAL_PARTITION_OTA_SUB,      /**< For Sub device OTA */
    HAL_PARTITION_GPT,
    HAL_PARTITION_ENV,
    HAL_PARTITION_ENV_REDUND,
    HAL_PARTITION_RTOSA,
    HAL_PARTITION_RTOSB,
    HAL_PARTITION_BOOT1,
    HAL_PARTITION_BOOT1_REDUND,
    HAL_PARTITION_MAX,
    HAL_PARTITION_NONE,
} hal_partition_t;

typedef struct {
    uint8_t                 page_buff[NFTL_PAGE_SIZE];
    uint8_t                 spare_buff[NFTL_SPARE_AREA_LEN];
    uint32_t                phy_block_nums;
    nftl_phy_block_info_t  *phy_blocks;
    uint32_t                logic_block_nums;
    nftl_logic_info_t      *logic_blocks;
    uint32_t                allocating_blk;
    uint32_t                allocating_page;
    nftl_block_tag          allocating_blk_tags[NFTL_TAG_PAGE_IN_BLOCK];
    hal_partition_t         partition;
    nftl_lock_t             par_lock;
    uint8_t                 initialized;
    uint32_t                bad_block_nums;
    uint32_t                gc_times;
} nftl_block_mgr_t;

/**
 * @brief NFTL initialization.
 *
 */
void nftl_init(void);

/**
 * @brief NFTL deinitialization.
 */
void nftl_deinit(void);

/**
 * @brief Read flash data from logical block.
 *
 * @param[in]  p       the partition number.
 * @param[in]  blk     the logical block index.
 * @param[in]  off     the offset to read.
 * @param[in]  outbuf  the buffer to store the read data.
 * @param[in]  len     the length of the outbuf.
 *
 * @return 0 on success, otherwise failed
 *
 */
int nftl_flash_read(nftl_partition_t p, nftl_block_t blk, nftl_offset_t off,
                    void *outbuf, nftl_size_t len);

/**
 * @brief Write data onto flash.
 *
 * @param[in]  p       the partition number.
 * @param[in]  blk     the logical block index.
 * @param[in]  off     the offset to read.
 * @param[in]  inbuf   the buffer storing the data to write.
 * @param[in]  len     the length of the inbuf.
 *
 * @return 0 on success, otherwise failed
 *
 */
int nftl_flash_write(nftl_partition_t p, nftl_block_t blk, nftl_size_t off,
                     const void *inbuf, nftl_size_t len);

/**
 * @brief Erase a logical block.
 *
 * @param[in]  p       the partition number.
 * @param[in]  blk     the logical block index.
 *
 * @return 0 on success, otherwise failed
 *
 */
int nftl_flash_erase(nftl_partition_t p, nftl_block_t blk);

/**
 * @brief Sync all buffered data onto flash.
 *
 * @param[in]  p       the partition number.
 *
 * @return 0 on success, otherwise failed
 *
 */
int nftl_flash_sync(nftl_partition_t p);

/**
 * @brief Get the occupation information of a NFTL partition.
 *
 * @param[in]  p       the partition number.
 *
 * @return 0 on success, otherwise failed
 *
 */
uint32_t nftl_flash_occupation(nftl_partition_t p);

/**
 * @brief Cleanup a NFTL partition (i.e. gabage collection).
 *
 * @param[in]  p       the partition number.
 *
 * @return none.
 *
 */
void nftl_flash_cleanup(nftl_partition_t p);

/**
 * @brief Dump the management information of a NFTL partition.
 *
 * @param[in]  mgr  the pointer of the partition manager.
 *
 * @return none.
 *
 */
void nftl_dump(nftl_block_mgr_t *mgr);

#ifdef NFTL_GC_NOTIFY
/**
 * @brief Notify a block as gabage to NFTL.
 *
 * @param[in]  p       the partition number.
 * @param[in]  blk     the logical block index.
 *
 * @return NFTL_ERR_OK on success, otherwise failed
 *
 */
nftl_err_t nftl_flash_notify_gabage(nftl_partition_t p, nftl_block_t blk);
#endif /* NFTL_GC_NOTIFY */

/**
 * @brief Erase all blocks in a NFTL parition.
 *
 * @param[in]  p       the partition number.
 *
 * @return 0 on success, otherwise failed
 *
 */
int nftl_flash_eraseall(nftl_partition_t p);

#endif

