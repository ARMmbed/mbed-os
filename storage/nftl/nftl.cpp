/* mbed Microcontroller Library
 * Copyright (c) 2021 ARM Limited
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

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nftl.h"
#include "nftl_conf.h"

#include "BlockDevice.h"
#include "SPINANDBlockDevice.h"

#define NFTL_START_GC_MAX_THRESHOLD      8
#define NFTL_START_GC_MID_THRESHOLD      4
#define NFTL_START_GC_MIN_THRESHOLD      2
#define NFTL_GC_MAX_BLOCKS               8
#define NFTL_GC_MID_BLOCKS               4
#define NFTL_GC_MIN_BLOCKS               1

PlatformMutex lock;
uint8_t buf[2112];
static nftl_block_mgr_t    g_block_mgr[3];

static uint32_t nftl_alloc_new_block(nftl_block_mgr_t *blk_mgr);
static nftl_err_t nftl_immigrate_block(nftl_block_mgr_t *blk_mgr, uint32_t old_blk);
static nftl_err_t nftl_alloc_new_page(nftl_block_mgr_t *blk_mgr, uint32_t *page_id);

static SPINANDBlockDevice bd(
    MBED_CONF_SPINAND_SPINAND_IO0,
    MBED_CONF_SPINAND_SPINAND_IO1,
    MBED_CONF_SPINAND_SPINAND_IO2,
    MBED_CONF_SPINAND_SPINAND_IO3,
    MBED_CONF_SPINAND_SPINAND_SCK,
    MBED_CONF_SPINAND_SPINAND_CSN,
    MBED_CONF_SPINAND_SPINAND_POLARITY_MODE,
    MBED_CONF_SPINAND_SPINAND_FREQ
);



#ifdef NFTL_YES_TRACE
static void dump_blk_mgr(nftl_block_mgr_t *mgr)
{
    uint32_t     i;

    if (mgr) {
        printf("partition: %d\r\n", mgr->partition);
        printf("logic_block_nums: %ld\r\n", mgr->logic_block_nums);
        printf("phy_block_nums: %ld\r\n", mgr->phy_block_nums);
        printf("initialized: %ld\r\n", mgr->initialized);

        for (i = 0; i < 20; i++)
            printf("logic: %ld, phy: %ld, rever: %ld\r\n", i,
                   mgr->logic_blocks[i].page_id,
                   mgr->logic_blocks[i].reversion);
    }
}
#endif

int32_t hal_flash_read_with_spare(hal_partition_t in_partition, uint32_t *off_set,
                                  void *data_buf, uint32_t data_buf_len,
                                  void *spare_buf, uint32_t spare_buf_len)
{
    int ret;

    ret = bd.read_spare(buf, *off_set, data_buf_len + spare_buf_len + TAG_OFFSET);
    if (ret != 0) {
        NFTL_ERROR("[%s] read spare error, off_set: %lx, spare_buf_len: %lx", __func__, *off_set, spare_buf_len);
        return ret;
    }

    memcpy(data_buf, buf, data_buf_len);
    memcpy(spare_buf, buf + NFTL_PAGE_SIZE + TAG_OFFSET, spare_buf_len);

    return ret;
}

int32_t hal_flash_write_with_spare(hal_partition_t in_partition, uint32_t *off_set,
                                   void *data_buf, uint32_t data_buf_len,
                                   void *spare_buf, uint32_t spare_buf_len)
{
    int ret;

    memcpy(buf, data_buf, data_buf_len);
    buf[NFTL_PAGE_SIZE] = 0xFF;
    memcpy(buf + NFTL_PAGE_SIZE + TAG_OFFSET, spare_buf, spare_buf_len);

    ret = bd.program_spare(buf, *off_set, data_buf_len + spare_buf_len + TAG_OFFSET);
    if (ret != 0) {
        NFTL_ERROR("[%s] program spare error, off_set: %lx, spare_buf_len: %lx", __func__, *off_set, spare_buf_len);
        return ret;
    }

    return ret;
}

int hal_flash_isbad(hal_partition_t in_partition, uint32_t off_set)
{
    int ret;
    uint8_t bad_flag0;
    uint8_t bad_flag1;
    uint32_t addr;

    addr = (off_set / NFTL_BLOCK_OFFSET * NFTL_BLOCK_OFFSET) + NFTL_PAGE_SIZE + BAD_FLAG_OFFSET;
    ret = bd.read_spare(&bad_flag0, addr, 1);
    if (ret != 0) {
        NFTL_ERROR("[%s] read error, address: %lx, len: 1", __func__, addr);
        return ret;
    }
    addr = (off_set / NFTL_BLOCK_OFFSET * NFTL_BLOCK_OFFSET) + NFTL_PAGE_SIZE * 2 + BAD_FLAG_OFFSET;
    ret = bd.read_spare(&bad_flag1, addr, 1);
    if (ret != 0) {
        NFTL_ERROR("[%s] read error, address: %lx, len: 1", __func__, addr);
        return ret;
    }

    if ((bad_flag0 == 0x00) && (bad_flag1 == 0x00)) {
        return true;
    } else {
        return false;
    }
}

int hal_flash_markbad(hal_partition_t in_partition, uint32_t off_set)
{
    int ret;
    uint8_t bad_flag0 = 0;
    uint8_t bad_flag1 = 0;
    uint32_t addr;

    addr = (off_set / NFTL_BLOCK_OFFSET * NFTL_BLOCK_OFFSET) + NFTL_PAGE_SIZE + BAD_FLAG_OFFSET;
    ret = bd.program_spare(&bad_flag0, addr, 1);
    if (ret != 0) {
        NFTL_ERROR("[%s] program error, address: %lx, len: 1", __func__, addr);
        return ret;
    }

    addr = (off_set / NFTL_BLOCK_OFFSET * NFTL_BLOCK_OFFSET) + NFTL_PAGE_SIZE * 2 + BAD_FLAG_OFFSET;
    ret = bd.program_spare(&bad_flag1, addr, 1);
    if (ret != 0) {
        NFTL_ERROR("[%s] program error, address: %lx, len: 1", __func__, addr);
        return ret;
    }

    return ret;
}

static uint8_t nftl_isvalid_bitmap(uint8_t *bitmap, uint32_t pos)
{
    uint32_t    index;
    uint32_t    off;

    index = pos >> 3;
    off   = pos & 0x7;

    return bitmap[index] & (1 << off);
}

static void nftl_set_bitmap(uint8_t *bitmap, uint32_t pos)
{
    uint32_t    index;
    uint32_t    off;

    index = pos >> 3;
    off   = pos & 0x7;

    bitmap[index] |= 1 << off;
}

static void nftl_clear_bitmap(uint8_t *bitmap, uint32_t pos)
{
    uint32_t    index;
    uint32_t    off;

    index = pos >> 3;
    off   = pos & 0x7;

    bitmap[index] &= ~(1 << off);
}

static uint32_t count_valid_page(nftl_block_mgr_t *mgr, uint32_t blk_off)
{
    uint32_t cnt = 0;

    for (int i = 0; i < NFTL_TAG_PAGE_IN_BLOCK; i++) {
        if (nftl_isvalid_bitmap(mgr->phy_blocks[blk_off].page_bitmap, i)) {
            cnt++;
        }
    }

    return cnt;
}

static uint32_t count_empty_block(nftl_block_mgr_t *mgr)
{
    uint32_t cnt = 0;
    uint32_t i;
    uint32_t j;

    for (i = 0; i < mgr->phy_block_nums; i++) {
        if (!hal_flash_isbad(mgr->partition, (NFTL_BLOCK_OFFSET * i)) &&
                (i != mgr->allocating_blk)) {
            for (j = 0; j < NFTL_TAG_PAGE_IN_BLOCK; j++) {
                if (nftl_isvalid_bitmap(mgr->phy_blocks[i].page_bitmap, j)) {
                    break;
                }
            }
            if (j >= NFTL_TAG_PAGE_IN_BLOCK) {
                cnt++;
            }
        }
    }

    return cnt;
}

static void nftl_proc_bad_block(nftl_block_mgr_t *blk_mgr, uint32_t blk)
{
    uint32_t              offset;

    offset = blk * NFTL_BLOCK_OFFSET;
    hal_flash_markbad(blk_mgr->partition, offset);
    blk_mgr->bad_block_nums++;
    NFTL_ERROR("[nftl] %s, process bad block: %ld, total bad blocks: %ld\r\n",
               __func__, blk, blk_mgr->bad_block_nums);

    return;
}

static void nftl_remove_logic_mapping(nftl_block_mgr_t *blk_mgr, uint32_t logic_blk,
                                      uint32_t blk, uint32_t page)
{
    uint32_t              i;
    uint32_t              page_id;

    nftl_clear_bitmap(blk_mgr->phy_blocks[blk].page_bitmap, page);

    if (NFTL_INVALID_BLOCK != logic_blk) {
//        blk_mgr->logic_blocks[logic_blk].page_id   = NFTL_INVALID_BLOCK;
    } else {
        page_id = blk * NFTL_PAGES_PER_BLK + page;
        for (i = 0; i < blk_mgr->logic_block_nums; i++) {
            if (page_id == blk_mgr->logic_blocks[i].page_id) {
                blk_mgr->logic_blocks[i].page_id = NFTL_INVALID_BLOCK;
                NFTL_ERROR("[nftl] read error, so clear the mapping: lblk: %ld, ppg: %ld", logic_blk, page_id);
                break;
            }
        }
    }
}

static void nftl_add_logic_mapping(nftl_block_mgr_t *blk_mgr, uint32_t logic_blk,
                                   uint32_t blk, uint32_t page, uint32_t reversion)
{
    NFTL_ASSERT(nftl_isvalid_bitmap(blk_mgr->phy_blocks[blk].page_bitmap, page) == 0);

    nftl_set_bitmap(blk_mgr->phy_blocks[blk].page_bitmap, page);

    blk_mgr->logic_blocks[logic_blk].page_id   = blk * NFTL_PAGES_PER_BLK + page;
    blk_mgr->logic_blocks[logic_blk].reversion = reversion;
}

static void nftl_create_mapping_tables(nftl_block_mgr_t *blk_mgr, nftl_block_tag *tag,
                                       uint32_t blk, uint32_t pg)
{
    uint32_t                blk_off;
    uint32_t                page_off;
    nftl_logic_info_t      *logic_blk;

    logic_blk = &blk_mgr->logic_blocks[tag->logic_block];

    if ((logic_blk->page_id == NFTL_INVALID_BLOCK) ||
            nftl_scmp(tag->reversion, logic_blk->reversion) > 0) {
        /*  clear old page bitmap */
        if (logic_blk->page_id != NFTL_INVALID_BLOCK) {
            blk_off  = logic_blk->page_id / NFTL_PAGES_PER_BLK;
            page_off = logic_blk->page_id % NFTL_PAGES_PER_BLK;

            if (nftl_isvalid_bitmap(blk_mgr->phy_blocks[blk_off].page_bitmap, page_off)) {
                nftl_remove_logic_mapping(blk_mgr, tag->logic_block, blk_off, page_off);
            }
        }

        /*  update new page mapping and page bitmap */
        nftl_add_logic_mapping(blk_mgr, tag->logic_block, blk, pg, tag->reversion);
    }
}

static void nftl_recover_mapping_from_spare(uint32_t blk, nftl_block_mgr_t *blk_mgr)
{
    uint32_t                i;
    uint32_t                r_off;
    int                     ret;
    nftl_block_tag         *tag;

    r_off = blk * NFTL_BLOCK_OFFSET;

    /*  read start from the first page */
    for (i = 0; i < NFTL_TAG_PAGE_IN_BLOCK; i++) {
        /* TODO: deal with endian */
        ret = hal_flash_read_with_spare(blk_mgr->partition, &r_off, blk_mgr->page_buff,
                                        NFTL_PAGE_SIZE, blk_mgr->spare_buff, NFTL_SPARE_AREA_LEN);

        r_off += NFTL_PAGE_OFFSET;

        if (ret == 0) {
            tag = (nftl_block_tag *)blk_mgr->spare_buff;

            if (tag->logic_block < blk_mgr->logic_block_nums) {
                nftl_create_mapping_tables(blk_mgr, tag, blk, i);
            } else {
                if (tag->logic_block != NFTL_INVALID_BLOCK) {
                    NFTL_ERROR("[nftl] %s, invalid logic block, p: %d, lblk: %ld, rev: %ld, "
                               "nums: %ld, pblk: %ld, ppage: %ld, roff: %ld\r\n", __func__,
                               blk_mgr->partition, tag->logic_block, tag->reversion,
                               blk_mgr->logic_block_nums, blk, i, r_off);
                    NFTL_ASSERT(tag->logic_block < blk_mgr->logic_block_nums);
                }

                return;
            }
        } else {
            NFTL_ERROR("[nftl] %s read err: %d\r\n", __func__, ret);
            return;
        }
    }
}

static void nftl_recover_mapping_from_tagpage(nftl_block_tag *tag, nftl_block_mgr_t *blk_mgr, uint32_t blk)
{
    uint32_t                i;

    for (i = 0; i < NFTL_TAG_PAGE_IN_BLOCK; i++) {
        if (tag->logic_block != NFTL_INVALID_BLOCK) {
            NFTL_ASSERT(tag->logic_block < blk_mgr->logic_block_nums);
            nftl_create_mapping_tables(blk_mgr, tag, blk, i);
        }

        tag++;
    }
}

static void nftl_blk_release(nftl_block_mgr_t *blk_mgr, uint32_t phy_blk)
{
    int             ret;

    if (0 == count_valid_page(blk_mgr, phy_blk)) {
        ret = bd.erase(phy_blk * NFTL_BLOCK_OFFSET, NFTL_BLOCK_SIZE);

        if (ret != 0) {
            NFTL_ERROR("[nftl] %s failed to erase block %ld part %d", __func__, phy_blk, blk_mgr->partition);
            hal_flash_markbad(blk_mgr->partition, phy_blk * NFTL_BLOCK_OFFSET);
        }
    }

    return;
}

static nftl_err_t nftl_immigrate_block(nftl_block_mgr_t *blk_mgr, uint32_t old_blk)
{
    int                     ret = 0;
    uint32_t                r_off;
    uint32_t                w_off;
    uint32_t                i;
    nftl_block_tag         *tag;
    uint32_t                page_id;

#ifdef CONFIG_LFS_SYSTEM_READONLY
    if (blk_mgr->partition == NFTL_PHYSICAL_PARTITION0 || blk_mgr->partition == NFTL_PHYSICAL_PARTITION0) {
        NFTL_WARN("[nftl] %s partition %d readonly\r\n", __func__, blk_mgr->partition);
        return NFTL_ERR_NAND_READONLY;
    }
#endif

    do {
        NFTL_TRACE("[nftl] %s, immigrating block: %ld to %ld\r\n", __func__, old_blk, blk_mgr->allocating_blk);

        NFTL_ASSERT(old_blk != blk_mgr->allocating_blk);

        r_off = old_blk * NFTL_BLOCK_OFFSET;
        ret = 0;

        for (i = 0; i < NFTL_TAG_PAGE_IN_BLOCK; i++) {
            if (nftl_isvalid_bitmap(blk_mgr->phy_blocks[old_blk].page_bitmap, i)) {
                NFTL_ASSERT(r_off == old_blk * NFTL_BLOCK_OFFSET + i * NFTL_PAGE_OFFSET);

                /* read the old block pages */
                /* TODO: deal with endian */
                ret = hal_flash_read_with_spare(blk_mgr->partition, &r_off, blk_mgr->page_buff,
                                                NFTL_PAGE_SIZE, blk_mgr->spare_buff, NFTL_SPARE_AREA_LEN);

                if (0 != ret) {
                    NFTL_ERROR("[nftl] %s, read error in gc: %d, p: %d, blk: %ld, page: %ld",
                               __func__, ret, blk_mgr->partition, old_blk, i);

                    /* remove the old logic mapping */
                    nftl_remove_logic_mapping(blk_mgr, NFTL_INVALID_BLOCK, old_blk, i);
                    nftl_blk_release(blk_mgr, old_blk);

                    continue;
                }

                r_off += NFTL_PAGE_OFFSET;

                /* update tag info and write to the new block page */
                tag = (nftl_block_tag *)&blk_mgr->spare_buff[0];
                tag->reversion++;

                NFTL_ASSERT(tag->logic_block != NFTL_INVALID_BLOCK);
                NFTL_ASSERT(tag->logic_block < blk_mgr->logic_block_nums);

                if (NFTL_ERR_OK != nftl_alloc_new_page(blk_mgr, &page_id)) {
                    NFTL_ERROR("[nftl] %s: no pages for immigrate: p: %d", __func__, blk_mgr->partition);
                    return NFTL_ERR_NAND_NO_PAGE;
                }

                w_off = page_id * NFTL_PAGE_OFFSET;

                if (tag->logic_block >= blk_mgr->logic_block_nums) {
                    NFTL_ERROR("[nftl] %s, logic block error: p: %d, blk: %ld, ver: %ld, nums: %ld",
                               __func__, blk_mgr->partition, tag->logic_block, tag->reversion, blk_mgr->logic_block_nums);
                    NFTL_ERROR("[nftl] %s, rblk: %ld, rpage: %ld, wpageid: %ld",
                               __func__, old_blk, i, page_id);
                    NFTL_ASSERT(tag->logic_block < blk_mgr->logic_block_nums);
                }

                /* write the valid pages into the allocating block */
                /* TODO: deal with endian */
                ret = hal_flash_write_with_spare(blk_mgr->partition, &w_off, blk_mgr->page_buff,
                                                 NFTL_PAGE_SIZE, blk_mgr->spare_buff, NFTL_SPARE_AREA_LEN);

                if (0 != ret) {
                    NFTL_TRACE("[%s]bad block found, w_off: %ld", __func__, w_off);
                    nftl_proc_bad_block(blk_mgr, page_id / NFTL_PAGES_PER_BLK);

                    if (nftl_alloc_new_block(blk_mgr) == NFTL_INVALID_BLOCK) {
                        NFTL_ERROR("[nftl] %s: no pages for alloc, p: %d", __func__, blk_mgr->partition);
                        return NFTL_ERR_NAND_NO_PAGE;
                    }

                    break;
                }

                NFTL_ASSERT(tag->logic_block < blk_mgr->logic_block_nums);

                /* update allocating block info */
                blk_mgr->allocating_blk_tags[page_id % NFTL_PAGES_PER_BLK].logic_block = tag->logic_block;
                blk_mgr->allocating_blk_tags[page_id % NFTL_PAGES_PER_BLK].reversion = tag->reversion;

                /* add the new logic mapping */
                nftl_add_logic_mapping(blk_mgr, tag->logic_block, page_id / NFTL_PAGES_PER_BLK,
                                       page_id % NFTL_PAGES_PER_BLK, tag->reversion);

                blk_mgr->logic_blocks[tag->logic_block].page_id = page_id;
                blk_mgr->logic_blocks[tag->logic_block].reversion = tag->reversion;
            } else {
                r_off += NFTL_PAGE_OFFSET;
            }
        }
    } while (ret);

    for (i = 0; i < NFTL_TAG_PAGE_IN_BLOCK; i++) {
        if (nftl_isvalid_bitmap(blk_mgr->phy_blocks[old_blk].page_bitmap, i)) {
            nftl_clear_bitmap(blk_mgr->phy_blocks[old_blk].page_bitmap, i);
        }
    }

    ret = bd.erase(old_blk * NFTL_BLOCK_OFFSET, NFTL_BLOCK_SIZE);
    if (ret != 0) {
        NFTL_ERROR("failed to erase block %ld part %d", old_blk, blk_mgr->partition);
        hal_flash_markbad(blk_mgr->partition, old_blk * NFTL_BLOCK_OFFSET);
    }

    return NFTL_ERR_OK;
}

static void nftl_update_tagpage(nftl_block_mgr_t *blk_mgr, uint32_t blk)
{
    int              ret;
    uint32_t         w_off;
    nftl_block_tag  *blk_tags;

    do {
        if (blk_mgr->allocating_page != NFTL_TAG_PAGE_IN_BLOCK) {
            NFTL_TRACE("[nftl] %s, allocating_page: %ld\r\n", __func__, blk_mgr->allocating_page);
        }

        w_off = blk * NFTL_BLOCK_OFFSET + NFTL_TAG_PAGE_IN_BLOCK * NFTL_PAGE_OFFSET;

        blk_tags = blk_mgr->allocating_blk_tags;

#ifdef NFTL_TAGPAGE_CHECK
        uint32_t         r_off;
        uint32_t         i;
        nftl_block_tag  *blk_tags_read;

        r_off = blk * NFTL_BLOCK_OFFSET;
        for (i = 0; i < NFTL_TAG_PAGE_IN_BLOCK; i++) {
            NFTL_ASSERT(blk_mgr->allocating_blk_tags[i].logic_block < blk_mgr->logic_block_nums);

            /* TODO: deal with endian */
            hal_flash_read_with_spare(blk_mgr->partition, &r_off, blk_mgr->page_buff,
                                      NFTL_PAGE_SIZE, blk_mgr->spare_buff, NFTL_SPARE_AREA_LEN);

            blk_tags_read = (nftl_block_tag *)blk_mgr->spare_buff;

            NFTL_ASSERT(blk_tags_read->logic_block == blk_tags[i].logic_block);
            NFTL_ASSERT(blk_tags_read->reversion == blk_tags[i].reversion);
        }
#endif

        /* no need to process error, because all mapping info is in spare area */
        /* TODO: deal with endian */
        ret = bd.program(blk_tags, w_off, sizeof(nftl_block_tag) * NFTL_TAG_PAGE_IN_BLOCK);

        w_off += sizeof(nftl_block_tag) * NFTL_TAG_PAGE_IN_BLOCK;

        if (NFTL_ERR_OK == ret) {
            break;
        } else {
            if (nftl_alloc_new_block(blk_mgr) == NFTL_INVALID_BLOCK) {
                NFTL_ERROR("[nftl] %s: no block left for bad block alloc", __func__);
                return;
            }

            if (NFTL_ERR_OK != nftl_immigrate_block(blk_mgr, blk)) {
                NFTL_ERROR("[nftl] %s: immigrate block fail", __func__);
                return;
            }

            nftl_proc_bad_block(blk_mgr, blk);

            blk = blk_mgr->allocating_blk;
        }
    } while (ret);
}

static void nftl_complete_gc(nftl_block_mgr_t *blk_mgr, bool completely)
{
    uint32_t            empty_blocks = 0;
    uint32_t            valid_pages;
    uint32_t            i;
    uint32_t            j;
    uint32_t            r_off;
    uint32_t            w_off;
    uint32_t            gc_blk;
    int                 ret;
    nftl_block_tag     *tag;
    uint32_t            page_id;

#ifdef CONFIG_LFS_SYSTEM_READONLY
    if (blk_mgr->partition == NFTL_PHYSICAL_PARTITION0 || blk_mgr->partition == NFTL_PHYSICAL_PARTITION0) {
        NFTL_WARN("[nftl] %s partition %d readonly\r\n", __func__, blk_mgr->partition);
        return;
    }
#endif

    gc_blk = blk_mgr->allocating_blk + 1;

    for (i = 0; i < blk_mgr->phy_block_nums - 1; i++) {
        if (true != completely) {
            empty_blocks = count_empty_block(blk_mgr);

            if (empty_blocks >= 8) {
                NFTL_WARN("%s end complete gc, part: %d, empty blocks: %ld\r\n", __func__, blk_mgr->partition, empty_blocks);
                break;
            }
        }

        if (gc_blk >= blk_mgr->phy_block_nums) {
            gc_blk = 0;
        }

        valid_pages = count_valid_page(blk_mgr, gc_blk);

        if ((gc_blk != blk_mgr->allocating_blk) &&
                (valid_pages > 0 && valid_pages < NFTL_TAG_PAGE_IN_BLOCK)) {
            NFTL_WARN("%s complete gc, part: %d, blk: %ld, empty blocks: %ld, gc pages: %ld", __func__,
                      blk_mgr->partition, gc_blk, empty_blocks, valid_pages);

            for (j = 0; j < NFTL_TAG_PAGE_IN_BLOCK; j++) {
                if (nftl_isvalid_bitmap(blk_mgr->phy_blocks[gc_blk].page_bitmap, j)) {
                    r_off = gc_blk * NFTL_BLOCK_OFFSET + j * NFTL_PAGE_OFFSET;

                    /*  read the old block pages */
                    /* TODO: deal with endian */
                    ret = hal_flash_read_with_spare(blk_mgr->partition, &r_off, blk_mgr->page_buff,
                                                    NFTL_PAGE_SIZE, blk_mgr->spare_buff, NFTL_SPARE_AREA_LEN);

                    if (0 != ret) {
                        NFTL_ERROR("[fntl] %s, read error in complete gc: %d, p: %d, blk: %ld, "
                                   "page: %ld", __func__, ret, blk_mgr->partition, gc_blk, j);

                        /* remove the old logic mapping */
                        nftl_remove_logic_mapping(blk_mgr, NFTL_INVALID_BLOCK, gc_blk, j);

                        nftl_blk_release(blk_mgr, gc_blk);

                        continue;
                    }

                    /* update tag info and write to the new block page */
                    tag = (nftl_block_tag *)&blk_mgr->spare_buff[0];
                    tag->reversion++;

                    NFTL_ASSERT(tag->logic_block < blk_mgr->logic_block_nums);

                    if (NFTL_ERR_OK != nftl_alloc_new_page(blk_mgr, &page_id)) {
                        NFTL_ERROR("[fntl] %s, no page left for complete gc", __func__);
                        break;
                    }

                    if (tag->logic_block >= blk_mgr->logic_block_nums) {
                        NFTL_ERROR("[nftl] %s, logic block error: p: %d, blk: %ld, ver: %ld, nums: %ld",
                                   __func__, blk_mgr->partition, tag->logic_block, tag->reversion, blk_mgr->logic_block_nums);

                        NFTL_ERROR("[nftl] %s, rblk: %ld, rpage: %ld, wpageid: %ld",
                                   __func__, gc_blk, j, page_id);

                        NFTL_ASSERT(tag->logic_block < blk_mgr->logic_block_nums);
                    }

                    /* write the valid pages into the allocating block */

                    w_off = page_id * NFTL_PAGE_OFFSET;
                    /* TODO: deal with endian */
                    ret = hal_flash_write_with_spare(blk_mgr->partition, &w_off, blk_mgr->page_buff,
                                                     NFTL_PAGE_SIZE, blk_mgr->spare_buff, NFTL_SPARE_AREA_LEN);

                    if (0 != ret) {
                        NFTL_TRACE("[%s]bad block found, w_off: %ld", __func__, w_off);

                        if (nftl_alloc_new_block(blk_mgr) == NFTL_INVALID_BLOCK) {
                            NFTL_ERROR("[nftl] %s: no pages for alloc", __func__);

                            return;
                        }

                        if (NFTL_ERR_OK != nftl_immigrate_block(blk_mgr, page_id / NFTL_PAGES_PER_BLK)) {
                            NFTL_ERROR("[nftl]: %s: immigrate block fail", __func__);
                            return;
                        }

                        nftl_proc_bad_block(blk_mgr, page_id / NFTL_PAGES_PER_BLK);

                        break;
                    }

                    /*  remove the old logic mapping */
                    nftl_remove_logic_mapping(blk_mgr, tag->logic_block, gc_blk, j);

                    nftl_blk_release(blk_mgr, gc_blk);

                    /*  add the new logic mapping */
                    nftl_add_logic_mapping(blk_mgr, tag->logic_block, page_id / NFTL_PAGES_PER_BLK,
                                           page_id % NFTL_PAGES_PER_BLK, tag->reversion);

                    blk_mgr->allocating_blk_tags[page_id % NFTL_PAGES_PER_BLK].logic_block = tag->logic_block;
                    blk_mgr->allocating_blk_tags[page_id % NFTL_PAGES_PER_BLK].reversion   = tag->reversion;
                }
            }
        }

        gc_blk++;
    }

    return;
}

static void nftl_gc_start(nftl_block_mgr_t *blk_mgr)
{
    uint32_t            i;
    uint32_t            j;
    uint32_t            dirtiest_block = NFTL_INVALID_BLOCK;
    int32_t             min_pages = 0x7FFFFFFF;
    uint32_t            blk_idx;
    uint32_t            valid_pages;
    uint32_t            blk_off;
    uint32_t            page_off;
    uint32_t            page_id;
    uint32_t            r_off;
    uint32_t            w_off;
    int                 ret;
    nftl_block_tag     *tag = NULL;
    uint32_t            gc_pages = 0;
    uint32_t            empty_blocks = 0;

#ifdef CONFIG_LFS_SYSTEM_READONLY
    if (blk_mgr->partition == NFTL_PHYSICAL_PARTITION0 || blk_mgr->partition == NFTL_PHYSICAL_PARTITION0) {
        NFTL_WARN("[nftl] %s partition %d readonly\r\n", __func__, blk_mgr->partition);
        return;
    }
#endif

    empty_blocks = count_empty_block(blk_mgr);

    if (empty_blocks < 8) {
        gc_pages = NFTL_TAG_PAGE_IN_BLOCK;
    } else if (empty_blocks < blk_mgr->phy_block_nums / 8) {
        gc_pages = 32;
    } else if (empty_blocks < blk_mgr->phy_block_nums / 4) {
        gc_pages = 8;
    } else if (empty_blocks < blk_mgr->phy_block_nums / 3) {
        gc_pages = 4;
    } else if (empty_blocks < blk_mgr->phy_block_nums / 2) {
        gc_pages = 2;
    } else {
        return;
    }

    blk_idx = blk_mgr->allocating_blk + 1;

    if (blk_idx >= blk_mgr->phy_block_nums) {
        blk_idx = 0;
    }

    dirtiest_block = blk_idx;
    min_pages      = count_valid_page(blk_mgr, blk_idx);

    /* find the dirtiest block */
    for (i = 0; i < (blk_mgr->phy_block_nums - 1); i++) {
        if (blk_idx >= blk_mgr->phy_block_nums) {
            blk_idx = 0;
        }

        valid_pages = count_valid_page(blk_mgr, blk_idx);

        if (valid_pages > 0) {
            if (min_pages == 0 || (uint32_t)min_pages > valid_pages) {
                min_pages = valid_pages;
                dirtiest_block = blk_idx;

                /* to accelerate the gc find, when find a block's valid page less than 10, so we can collect it */
                if (min_pages < 10) {
                    break;
                }
            }
        }

        blk_idx++;
    }

    if (min_pages >= NFTL_TAG_PAGE_IN_BLOCK) {
        return;
    }

    gc_pages = nftl_min(gc_pages, min_pages);

    blk_mgr->gc_times++;

    if (blk_mgr->gc_times % 50 == 0) {
        NFTL_WARN("[%s] patition: %d, dirtiest block: %ld, min pages: %ld, empty blocks: %ld, gc times: %ld",
                  __func__, blk_mgr->partition, dirtiest_block, min_pages, empty_blocks, blk_mgr->gc_times);
    }

    for (i = 0; i < NFTL_TAG_PAGE_IN_BLOCK; i++) {
        if (nftl_isvalid_bitmap(blk_mgr->phy_blocks[dirtiest_block].page_bitmap, i)) {
            r_off = dirtiest_block * NFTL_BLOCK_OFFSET + i * NFTL_PAGE_OFFSET;

            /*  read the old block pages */
            /* TODO: deal with endian */
            ret = hal_flash_read_with_spare(blk_mgr->partition, &r_off, blk_mgr->page_buff,
                                            NFTL_PAGE_SIZE, blk_mgr->spare_buff, NFTL_SPARE_AREA_LEN);

            if (0 != ret) {
                NFTL_ERROR("[fntl] %s, read error in immigrate: %d, p: %d, blk: %ld, page: %ld",
                           __func__, ret, blk_mgr->partition, dirtiest_block, i);

                /* remove the old logic mapping */
                nftl_remove_logic_mapping(blk_mgr, NFTL_INVALID_BLOCK, dirtiest_block, i);

                nftl_blk_release(blk_mgr, dirtiest_block);

                continue;
            }

            /* update tag info and write to the new block page */
            tag = (nftl_block_tag *)&blk_mgr->spare_buff[0];
            tag->reversion++;

            if (NFTL_ERR_OK != nftl_alloc_new_page(blk_mgr, &page_id)) {
                NFTL_ERROR("[fntl] %s, no page left for gc", __func__);
                break;
            }

            if (tag->logic_block >= blk_mgr->logic_block_nums) {
                NFTL_ERROR("[nftl] %s, logic block error: p: %d, blk: %ld, ver: %ld nums: %ld",
                           __func__, blk_mgr->partition, tag->logic_block, tag->reversion, blk_mgr->logic_block_nums);

                NFTL_ERROR("[nftl] %s, rblk: %ld, rpage: %ld, wpageid: %ld",
                           __func__, dirtiest_block, i, page_id);

                NFTL_ASSERT(tag->logic_block < blk_mgr->logic_block_nums);
            }

            /* write the valid pages into the allocating block */
            w_off = page_id * NFTL_PAGE_OFFSET;
            /* TODO: deal with endian */
            ret = hal_flash_write_with_spare(blk_mgr->partition, &w_off, blk_mgr->page_buff,
                                             NFTL_PAGE_SIZE, blk_mgr->spare_buff, NFTL_SPARE_AREA_LEN);

            if (0 != ret) {
                NFTL_TRACE("[%s]bad block found, w_off: %ld", __func__, w_off);

                if (nftl_alloc_new_block(blk_mgr) == NFTL_INVALID_BLOCK) {
                    NFTL_ERROR("[nftl] %s: no pages for alloc", __func__);
                    return;
                }

                if (NFTL_ERR_OK != nftl_immigrate_block(blk_mgr, page_id / NFTL_PAGES_PER_BLK)) {
                    NFTL_ERROR("[nftl]: %s: immigrate block fail", __func__);
                    return;
                }

                nftl_proc_bad_block(blk_mgr, page_id / NFTL_PAGES_PER_BLK);

                break;
            }

            /*  remove the old logic mapping */
            nftl_remove_logic_mapping(blk_mgr, tag->logic_block, dirtiest_block, i);

            nftl_blk_release(blk_mgr, dirtiest_block);

            /*  add the new logic mapping */
            nftl_add_logic_mapping(blk_mgr, tag->logic_block, page_id / NFTL_PAGES_PER_BLK, page_id % NFTL_PAGES_PER_BLK, tag->reversion);

            blk_mgr->allocating_blk_tags[page_id % NFTL_PAGES_PER_BLK].logic_block = tag->logic_block;
            blk_mgr->allocating_blk_tags[page_id % NFTL_PAGES_PER_BLK].reversion   = tag->reversion;

            gc_pages--;

            if (gc_pages <= 0) {
                break;
            }
        }
    }

    for (i = 0; i < blk_mgr->phy_block_nums; i++) {
        for (j = 0; j < NFTL_TAG_PAGE_IN_BLOCK; j++) {
            nftl_clear_bitmap(blk_mgr->phy_blocks[i].page_bitmap, j);
        }
    }

    for (i = 0; i < blk_mgr->logic_block_nums; i++) {
        if (blk_mgr->logic_blocks[i].page_id != NFTL_INVALID_BLOCK) {

            blk_off = blk_mgr->logic_blocks[i].page_id / NFTL_PAGES_PER_BLK;
            page_off = blk_mgr->logic_blocks[i].page_id % NFTL_PAGES_PER_BLK;

            nftl_set_bitmap(blk_mgr->phy_blocks[blk_off].page_bitmap, page_off);
        }
    }
}

static uint32_t nftl_find_empty_block(nftl_block_mgr_t *blk_mgr)
{
    uint32_t                i;
    uint32_t                new_blk;
    uint32_t                offset;

    NFTL_ASSERT(blk_mgr->allocating_blk != NFTL_INVALID_BLOCK);

    /* TODO: use a random start offset to achive wear leveing */
    new_blk = blk_mgr->allocating_blk + 1;
    if (new_blk >= blk_mgr->phy_block_nums) {
        new_blk = 0;
    }

    for (i = 0; i < blk_mgr->phy_block_nums - 1; i++) {
        offset = new_blk * NFTL_BLOCK_OFFSET;

        if (!hal_flash_isbad(blk_mgr->partition, offset)) {
            if (count_valid_page(blk_mgr, new_blk) == 0) {
                return new_blk;
            }
        }

        new_blk++;

        if (new_blk >= blk_mgr->phy_block_nums) {
            new_blk = 0;
        }
    }

    return NFTL_INVALID_BLOCK;
}

static uint32_t nftl_alloc_new_block(nftl_block_mgr_t *blk_mgr)
{
    uint32_t                new_blk;
    int                     ret;
    uint32_t                offset;

#ifdef CONFIG_LFS_SYSTEM_READONLY
    if (blk_mgr->partition == NFTL_PHYSICAL_PARTITION0 || blk_mgr->partition == NFTL_PHYSICAL_PARTITION0) {
        NFTL_WARN("[nftl] %s partition %d readonly\r\n", __func__, blk_mgr->partition);
        return NFTL_INVALID_BLOCK;
    }
#endif

    do {
        new_blk = nftl_find_empty_block(blk_mgr);

        /* no empty block found */
        if (new_blk == NFTL_INVALID_BLOCK) {
            blk_mgr->allocating_blk  = NFTL_INVALID_BLOCK;
            blk_mgr->allocating_page = 0;

            return NFTL_INVALID_BLOCK;
        }

        blk_mgr->allocating_blk  = new_blk;
        blk_mgr->allocating_page = 0;

        offset = new_blk * NFTL_BLOCK_OFFSET;

        NFTL_TRACE("[%s] erasing offset %ld in part %d, blk: %ld", __func__,
                   offset, blk_mgr->partition, blk_mgr->allocating_blk);

        ret = bd.erase(offset, NFTL_BLOCK_SIZE);

        if (ret) {
            NFTL_ERROR("[%s] bad block %ld found in part %d, ret: %d", __func__,
                       new_blk, blk_mgr->partition, ret);
            nftl_proc_bad_block(blk_mgr, new_blk);
        }

    } while (ret);

    return new_blk;
}

static nftl_err_t nftl_alloc_new_page(nftl_block_mgr_t *blk_mgr, uint32_t *page_id)
{
    uint32_t                blk_off;
    uint32_t                page_off;

    if (blk_mgr->allocating_blk == NFTL_INVALID_BLOCK) {
        NFTL_ERROR("[nftl] %s allocating blk is invalid, empty blocks: %ld", __func__, count_empty_block(blk_mgr));

        /* alloc new block */
        if (NFTL_INVALID_BLOCK == nftl_alloc_new_block(blk_mgr)) {
            NFTL_ERROR("[nftl] %s alloc new block failed", __func__);

            return NFTL_ERR_NAND_NO_PAGE;
        }
    }

    if (blk_mgr->allocating_page >= NFTL_TAG_PAGE_IN_BLOCK) {

        nftl_update_tagpage(blk_mgr, blk_mgr->allocating_blk);

        /* alloc new block */
        if (NFTL_INVALID_BLOCK == nftl_alloc_new_block(blk_mgr)) {
            NFTL_ERROR("[nftl] %s alloc new block failed", __func__);

            return NFTL_ERR_NAND_NO_PAGE;
        }
    }

    blk_off  = blk_mgr->allocating_blk;
    page_off = blk_mgr->allocating_page;

    *page_id = blk_off * NFTL_PAGES_PER_BLK + page_off;

    blk_mgr->allocating_page++;

    return NFTL_ERR_OK;
}

static nftl_err_t nftl_init_partition(nftl_block_mgr_t *blk_mgr)
{
    uint32_t                i;
    uint32_t                j;
    nftl_block_tag         *tag = NULL;
    int32_t                 ret;
    uint32_t                r_off;

    blk_mgr->logic_blocks     = (nftl_logic_info_t *)malloc(blk_mgr->logic_block_nums * sizeof(nftl_logic_info_t));

    if (NULL == blk_mgr->logic_blocks) {
        NFTL_ERROR("[%s] no memory left for logic blocks", __func__);
        return NFTL_ERR_NAND_IO;
    }

    blk_mgr->phy_blocks = (nftl_phy_block_info_t *)malloc(blk_mgr->phy_block_nums * sizeof(nftl_phy_block_info_t));

    if (NULL == blk_mgr->phy_blocks) {
        free(blk_mgr->logic_blocks);
        NFTL_ERROR("[%s] no memory left for phy blocks", __func__);
        return NFTL_ERR_NAND_IO;
    }

    for (i = 0; i < blk_mgr->logic_block_nums; i++) {
        blk_mgr->logic_blocks[i].page_id   = NFTL_INVALID_BLOCK;
        blk_mgr->logic_blocks[i].reversion = 0;
    }

    blk_mgr->bad_block_nums   = 0;

    for (i = 0; i < blk_mgr->phy_block_nums; i++) {
        for (j = 0; j < NFTL_PAGE_BITMAP_LEN; j++) {
            blk_mgr->phy_blocks[i].page_bitmap[j] = 0;
        }

        r_off = i * NFTL_BLOCK_OFFSET + NFTL_TAG_PAGE_IN_BLOCK * NFTL_PAGE_OFFSET;

        if (hal_flash_isbad(blk_mgr->partition, r_off)) {
            blk_mgr->bad_block_nums++;
            NFTL_TRACE("[%s] bad block found: %ld, total bad blocks: %ld", __func__, i, blk_mgr->bad_block_nums);
            continue;
        }

        /* TODO: deal with endian */
        ret = bd.read(blk_mgr->page_buff, r_off, NFTL_PAGE_SIZE);
        r_off += NFTL_PAGE_OFFSET;

        if (0 == ret) {
            tag = (nftl_block_tag *)blk_mgr->page_buff;

            if (NFTL_INVALID_BLOCK != tag->logic_block) {
                nftl_recover_mapping_from_tagpage(tag, blk_mgr, i);
            } else {
                nftl_recover_mapping_from_spare(i, blk_mgr);
            }
        } else {
            nftl_recover_mapping_from_spare(i, blk_mgr);
        }
    }

    /*  alloc a new block for writing new data */
    if (NFTL_INVALID_BLOCK == nftl_alloc_new_block(blk_mgr)) {
        NFTL_ERROR("[NFTL] %s no block left in init\r\n", __func__);
    }

    nftl_lock_create(&blk_mgr->par_lock);
    nftl_complete_gc(blk_mgr, false);
    blk_mgr->initialized = 1;

    return NFTL_ERR_OK;
}

int nftl_flash_read(nftl_partition_t p, nftl_block_t blk, nftl_offset_t off,
                    void *outbuf, nftl_size_t len)
{
    uint32_t                r_off;
    uint32_t                r_len;
    int                     ret = NFTL_ERR_OK;
    uint32_t                phy_blk;
    nftl_block_mgr_t       *blk_mgr;
    uint8_t                *p_buf;

    NFTL_ASSERT(off < NFTL_PAGE_SIZE);

    if (p >= NFTL_PARTITION_MAX) {
        NFTL_ERROR("[%s] no partition found", __func__);
        return NFTL_ERR_NAND_UNKNOWN;
    }

    blk_mgr = &g_block_mgr[p];

    if (!blk_mgr->initialized) {
        NFTL_ERROR("[%s] partition %d not initialized", __func__, p);
        return NFTL_ERR_NAND_UNKNOWN;
    }

    p_buf = (uint8_t *)outbuf;

    nftl_lock(&blk_mgr->par_lock);

    while (len > 0) {
        if (blk >= blk_mgr->logic_block_nums) {
            NFTL_ERROR("[%s] block exceed the max index", __func__);
            nftl_unlock(&blk_mgr->par_lock);
            return NFTL_ERR_NAND_UNKNOWN;
        }

        phy_blk = blk_mgr->logic_blocks[blk].page_id;

        if (phy_blk == NFTL_INVALID_BLOCK) { /*  erased pending write state */
            NFTL_DEBUG("[%s] logic block %ld not be mapped in part %d, part_in: %d",
                       __func__, blk, blk_mgr->partition, p);
            memset(outbuf, NFTL_ERASED_VALUE, len);
            nftl_unlock(&blk_mgr->par_lock);
            return NFTL_ERR_OK;
        }

        r_off = phy_blk * NFTL_PAGE_OFFSET;
        r_off += off;

        r_len = nftl_min(NFTL_PAGE_SIZE - off, len);

        /*  only the first block maybe not read from the start, so clear off now */
        off = 0;

        /* TODO: deal with endian */
        ret = bd.read(p_buf, r_off, r_len);

        r_off += r_len;

        if (ret != 0) {
            NFTL_ERROR("[%s] read error, blk: %ld, readlen: %ld, p: %d, off: %ld", __func__, blk, r_len, blk_mgr->partition, r_off);
            nftl_unlock(&blk_mgr->par_lock);
            return ret;
        }

        p_buf += r_len;
        len   -= r_len;

        blk++;
    }

    nftl_unlock(&blk_mgr->par_lock);

    return ret;
}

int nftl_flash_write(nftl_partition_t p, nftl_block_t blk, nftl_size_t off,
                     const void *inbuf, nftl_size_t len)
{
    uint32_t                old_blk_off = NFTL_INVALID_BLOCK;
    uint32_t                old_page_off = NFTL_INVALID_BLOCK;
    uint32_t                new_blk_off = NFTL_INVALID_BLOCK;
    uint32_t                new_page_off = NFTL_INVALID_BLOCK;
    nftl_block_mgr_t       *blk_mgr;
    uint32_t                page_id;
    uint32_t                reversion;
    uint32_t                w_off;
    uint32_t                w_len;
    nftl_block_tag         *tag;
    int                     ret;
    uint8_t                *p_buf;

    NFTL_ASSERT(off % NFTL_PAGE_SIZE == 0);
    NFTL_ASSERT(len % NFTL_PAGE_SIZE == 0);

#ifdef CONFIG_LFS_SYSTEM_READONLY
    if (p == NFTL_PARTITION0 || p == NFTL_PARTITION1) {
        NFTL_WARN("[nftl] %s partition %d readonly\r\n", __func__, p);
        return NFTL_ERR_NAND_READONLY;
    }
#endif

    if (p >= NFTL_PARTITION_MAX) {
        NFTL_ERROR("[%s] no partition found", __func__);
        return NFTL_ERR_NAND_UNKNOWN;
    }

    blk_mgr = &g_block_mgr[p];

    if (!blk_mgr->initialized) {
        NFTL_ERROR("[%s] partition %d not initialized", __func__, p);
        return NFTL_ERR_NAND_UNKNOWN;
    }

    p_buf = (uint8_t *)inbuf;

    nftl_lock(&blk_mgr->par_lock);

    while (len > 0) {
        if (blk >= blk_mgr->logic_block_nums) {
            NFTL_ERROR("[%s] block exceed the max index", __func__);
            nftl_unlock(&blk_mgr->par_lock);
            return NFTL_ERR_NAND_UNKNOWN;
        }

        reversion = blk_mgr->logic_blocks[blk].reversion;
        page_id  = blk_mgr->logic_blocks[blk].page_id;
        reversion++;

        if (page_id != NFTL_INVALID_BLOCK) {

            old_blk_off  = page_id / NFTL_PAGES_PER_BLK;
            old_page_off = page_id % NFTL_PAGES_PER_BLK;
        }

        do {
            if (NFTL_ERR_OK != nftl_alloc_new_page(blk_mgr, &page_id)) {
                NFTL_ERROR("[%s] no page left for write", __func__);
                nftl_unlock(&blk_mgr->par_lock);
                return NFTL_ERR_NAND_NO_PAGE;
            }
            new_blk_off  = page_id / NFTL_PAGES_PER_BLK;
            new_page_off = page_id % NFTL_PAGES_PER_BLK;

            w_off = page_id * NFTL_PAGE_OFFSET;
            w_off += off;

            NFTL_ASSERT(new_page_off != NFTL_TAG_PAGE_IN_BLOCK);
            NFTL_ASSERT(blk < blk_mgr->logic_block_nums);

            blk_mgr->allocating_blk_tags[new_page_off].logic_block = blk;
            blk_mgr->allocating_blk_tags[new_page_off].reversion   = reversion;

            tag = (nftl_block_tag *)&blk_mgr->spare_buff[0];
            tag->logic_block = blk;
            tag->reversion = reversion;

            NFTL_ASSERT(blk != NFTL_INVALID_BLOCK);

            w_len = nftl_min(NFTL_PAGE_SIZE - off, len);

            NFTL_ASSERT(tag->logic_block != NFTL_INVALID_BLOCK);

            if (tag->logic_block >= blk_mgr->logic_block_nums) {

                NFTL_ERROR("[nftl] %s, logic block error: p: %d, blk: %ld, ver: %ld nums: %ld",
                           __func__, blk_mgr->partition, tag->logic_block, tag->reversion, blk_mgr->logic_block_nums);

                NFTL_ASSERT(tag->logic_block < blk_mgr->logic_block_nums);
            }

            /* TODO: deal with endian */
            ret = hal_flash_write_with_spare(blk_mgr->partition, &w_off, p_buf, w_len, blk_mgr->spare_buff, NFTL_SPARE_AREA_LEN);

            if (ret) {
                if (NFTL_INVALID_BLOCK == nftl_alloc_new_block(blk_mgr)) {
                    NFTL_ERROR("[nftl]: %s: bad block process fail\r\n", __func__);
                    nftl_unlock(&blk_mgr->par_lock);
                    return NFTL_ERR_NAND_NO_PAGE;
                }

                if (new_page_off > 0) {
                    if (NFTL_ERR_OK != nftl_immigrate_block(blk_mgr, new_blk_off)) {
                        NFTL_ERROR("[nftl]: %s: immigrate block fail\r\n", __func__);
                        nftl_unlock(&blk_mgr->par_lock);
                        return NFTL_ERR_NAND_IO;
                    }
                }
                nftl_proc_bad_block(blk_mgr, new_blk_off);
            } else {
                off    = 0;
                len   -= w_len;
                p_buf += w_len;

                /* update new mapping info */
                nftl_add_logic_mapping(blk_mgr, blk, new_blk_off, new_page_off, reversion);

                if (NFTL_INVALID_BLOCK != old_blk_off && NFTL_INVALID_BLOCK != old_page_off) {
                    /* clear old mapping info */
                    if (nftl_isvalid_bitmap(blk_mgr->phy_blocks[old_blk_off].page_bitmap, old_page_off)) {
                        nftl_remove_logic_mapping(blk_mgr, blk, old_blk_off, old_page_off);

                        nftl_blk_release(blk_mgr, old_blk_off);
                    }

                    old_blk_off = NFTL_INVALID_BLOCK;
                    old_page_off = NFTL_INVALID_BLOCK;
                }
                if (new_page_off == 0) {
                    nftl_gc_start(blk_mgr);
                }
            }
        } while (ret);

        blk++;
    }

    nftl_unlock(&blk_mgr->par_lock);

    return NFTL_ERR_OK;
}

int nftl_flash_erase(nftl_partition_t p, nftl_block_t blk)
{
    uint32_t                blk_off;
    uint32_t                page_off;
    nftl_block_mgr_t       *blk_mgr;
    uint32_t                page_id;

#ifdef CONFIG_LFS_SYSTEM_READONLY
    if (p == NFTL_PARTITION0 || p == NFTL_PARTITION1) {
        NFTL_WARN("[nftl] %s partition %d readonly\r\n", __func__, p);
        return NFTL_ERR_NAND_READONLY;
    }
#endif

    if (p >= NFTL_PARTITION_MAX) {
        NFTL_ERROR("[%s] no partition found", __func__);
        return NFTL_ERR_NAND_UNKNOWN;
    }

    blk_mgr = &g_block_mgr[p];

    if (!blk_mgr->initialized) {
        NFTL_ERROR("[%s] partition %d not initialized", __func__, p);
        return NFTL_ERR_NAND_UNKNOWN;
    }

    if (blk >= blk_mgr->logic_block_nums) {
        NFTL_ERROR("[%s] block exceed the max index", __func__);
        return NFTL_ERR_NAND_UNKNOWN;
    }

    nftl_lock(&blk_mgr->par_lock);

    page_id  = blk_mgr->logic_blocks[blk].page_id;

    if (page_id == NFTL_INVALID_BLOCK) {
        nftl_unlock(&blk_mgr->par_lock);
        return NFTL_ERR_OK;
    }

    blk_off  = page_id / NFTL_PAGES_PER_BLK;
    page_off = page_id % NFTL_PAGES_PER_BLK;

    if (nftl_isvalid_bitmap(blk_mgr->phy_blocks[blk_off].page_bitmap, page_off)) {

        nftl_remove_logic_mapping(blk_mgr, blk, blk_off, page_off);

        nftl_blk_release(blk_mgr, blk_off);
    }

#ifdef NFTL_GC_IN_THREAD
    nftl_gc_start(blk_mgr);
#endif

    nftl_unlock(&blk_mgr->par_lock);

    return NFTL_ERR_OK;
}

int nftl_flash_sync(nftl_partition_t p)
{
    return NFTL_ERR_OK;
}

uint32_t nftl_flash_occupation(nftl_partition_t p)
{
    uint32_t                i;
    uint32_t                used_blks = 0;
    nftl_block_mgr_t       *blk_mgr;

    if (p >= NFTL_PARTITION_MAX) {
        NFTL_ERROR("[%s] no partition found", __func__);
        return 0;
    }

    blk_mgr = &g_block_mgr[p];

    for (i = 0; i < blk_mgr->phy_block_nums; i++) {
        if (0 != count_valid_page(blk_mgr, i)) {
            used_blks++;
        }
    }

    return used_blks * 100 / blk_mgr->phy_block_nums;
}

void nftl_flash_cleanup(nftl_partition_t p)
{
    nftl_block_mgr_t       *blk_mgr;

#ifdef CONFIG_LFS_SYSTEM_READONLY
    if (p == NFTL_PARTITION0 || p == NFTL_PARTITION1) {
        NFTL_WARN("[nftl] %s partition %d readonly\r\n", __func__, p);
        return;
    }
#endif

    if (p >= NFTL_PARTITION_MAX) {
        NFTL_ERROR("[%s] no partition found", __func__);
        return;
    }

    blk_mgr = &g_block_mgr[p];

    nftl_lock(&blk_mgr->par_lock);
    nftl_complete_gc(blk_mgr, true);
    nftl_unlock(&blk_mgr->par_lock);
}

#ifdef NFTL_YES_TRACE
static void nftl_dump_partition(nftl_block_mgr_t *blk_mgr)
{
    uint32_t               i;

    printf("==== NFTL management info for logical partition %d =====\r\n", blk_mgr->partition);

    printf("allocating block: %ld\r\n", blk_mgr->allocating_blk);
    printf("allocating page: %ld\r\n", blk_mgr->allocating_page);
    printf("allocating block tags\r\n");
    printf("page_id\treversion\r\n");
    for (i = 0; i < NFTL_TAG_PAGE_IN_BLOCK; i++) {
        printf("%u\t%ld\r\n",
               blk_mgr->allocating_blk_tags[i].logic_block,
               blk_mgr->allocating_blk_tags[i].reversion);
    }

    printf("\r\n");

    printf("blkidx\tbitmap\r\n");
    for (i = 0; i < blk_mgr->phy_block_nums; i++) {
        if (
            (blk_mgr->phy_blocks[i].page_bitmap[0] == 0) &&
            (blk_mgr->phy_blocks[i].page_bitmap[1] == 0) &&
            (blk_mgr->phy_blocks[i].page_bitmap[2] == 0) &&
            (blk_mgr->phy_blocks[i].page_bitmap[3] == 0) &&
            (blk_mgr->phy_blocks[i].page_bitmap[4] == 0) &&
            (blk_mgr->phy_blocks[i].page_bitmap[5] == 0) &&
            (blk_mgr->phy_blocks[i].page_bitmap[6] == 0) &&
            (blk_mgr->phy_blocks[i].page_bitmap[7] == 0)
        ) {
            continue;
        }

        printf("%u\t%02x%02x%02x%02x%02x%02x%02x%02x\r\n", i,
               blk_mgr->phy_blocks[i].page_bitmap[0],
               blk_mgr->phy_blocks[i].page_bitmap[1],
               blk_mgr->phy_blocks[i].page_bitmap[2],
               blk_mgr->phy_blocks[i].page_bitmap[3],
               blk_mgr->phy_blocks[i].page_bitmap[4],
               blk_mgr->phy_blocks[i].page_bitmap[5],
               blk_mgr->phy_blocks[i].page_bitmap[6],
               blk_mgr->phy_blocks[i].page_bitmap[7]);
    }

    printf("logic block\tpage_id\treversion\r\n");
    for (i = 0; i < blk_mgr->logic_block_nums; i++) {
        if (blk_mgr->logic_blocks[i].page_id != -1) {
            printf("%ld\t\t%ld\t%ld\r\n", i, blk_mgr->logic_blocks[i].page_id,
                   blk_mgr->logic_blocks[i].reversion);
        }
    }

}

void nftl_dump(nftl_block_mgr_t *blk_mgr)
{
    uint32_t               partition;

    if (NULL == blk_mgr) {
        for (partition = 0; partition < 3; partition++) {
            blk_mgr = &g_block_mgr[partition];
            nftl_dump_partition(blk_mgr);
        }
    } else {
        nftl_dump_partition(blk_mgr);
    }
}
#endif

#ifdef NFTL_GC_NOTIFY
nftl_err_t nftl_flash_notify_gabage(nftl_partition_t p, nftl_block_t blk)
{
#ifdef CONFIG_LFS_SYSTEM_READONLY
    if (p == NFTL_PARTITION0 || p == NFTL_PARTITION1) {
        NFTL_WARN("[nftl] %s partition %d readonly\r\n", __func__, p);
        return NFTL_ERR_NAND_READONLY;
    }
#endif

    NFTL_TRACE("flash notify %s part - %d block - %ld", __func__, p, blk);

    return nftl_flash_erase(p, blk);
}
#endif

void nftl_statistics(void)
{
    uint32_t               partition;
    uint32_t               i;
    nftl_block_mgr_t      *blk_mgr;
    uint32_t               total_blks;
    uint32_t               used_blks;
    uint32_t               used_pages;
    uint32_t               valid_pages;
    printf("%15s%15s%15s%15s%15s%15s\n", "partition", "total blks", "used blks", "bad blks", "total pages", "used pages");

    for (partition = 0; partition < 3; partition++) {
        blk_mgr = &g_block_mgr[partition];
        total_blks = blk_mgr->phy_block_nums;
        used_blks  = 0;
        used_pages = 0;

        for (i = 0; i < total_blks; i++) {
            valid_pages = count_valid_page(blk_mgr, i);
            used_pages += valid_pages;

            if (0 != valid_pages) {
                used_blks++;
            }
        }

        printf("%15u%15lu%9lu(%3lu%%)%9lu(%3lu%%)%15lu%9lu(%3lu%%)%15lu\n",
               blk_mgr->partition,
               blk_mgr->phy_block_nums,
               used_blks,
               used_blks * 100 / blk_mgr->phy_block_nums,
               blk_mgr->bad_block_nums,
               blk_mgr->bad_block_nums * 100 / blk_mgr->phy_block_nums,
               blk_mgr->phy_block_nums * NFTL_TAG_PAGE_IN_BLOCK,
               used_pages,
               used_pages * 100 / (blk_mgr->phy_block_nums * NFTL_TAG_PAGE_IN_BLOCK),
               blk_mgr->gc_times);
    }
    return;
}

#ifdef NFTL_GC_IN_THREAD
static ktask_t              *g_nftl_gc_bg_task;

static void nftl_gc_bg_task(void *argv)
{
    uint32_t            i;
    nftl_block_mgr_t   *blk_mgr;

    while (1) {
        for (i = 0; i < NFTL_PARTITION_MAX; i++) {
            blk_mgr = &g_block_mgr[i];
            nftl_lock(&blk_mgr->par_lock);
            nftl_gc_start(blk_mgr);
            nftl_unlock(&blk_mgr->par_lock);
        }

        nftl_msleep(2000);
    }
}
#endif

void nftl_init(void)
{
    nftl_partition_t      partition;
    nftl_block_mgr_t     *blk_mgr;
    nftl_err_t            ret;


    if (!g_block_mgr[NFTL_PARTITION0].initialized) {
        bd.init();

        g_block_mgr[NFTL_PARTITION0].partition        = NFTL_PHYSICAL_PARTITION0;
        g_block_mgr[NFTL_PARTITION0].logic_block_nums = NFTL_TAG_PAGE_IN_BLOCK * NFTL_PART0_BLOCK_NUMS;
        g_block_mgr[NFTL_PARTITION0].phy_block_nums   = NFTL_PART0_BLOCK_NUMS + NFTL_PART0_BUFFER_BLOCK_NUMS;
        g_block_mgr[NFTL_PARTITION0].initialized      = 0;

#if NFTL_LOGICAL_PARTITION_NUM > 1
        g_block_mgr[NFTL_PARTITION1].partition        = NFTL_PHYSICAL_PARTITION1;
        g_block_mgr[NFTL_PARTITION1].logic_block_nums = NFTL_TAG_PAGE_IN_BLOCK * NFTL_PART1_BLOCK_NUMS;
        g_block_mgr[NFTL_PARTITION1].phy_block_nums   = NFTL_PART1_BLOCK_NUMS + NFTL_PART1_BUFFER_BLOCK_NUMS;
        g_block_mgr[NFTL_PARTITION1].initialized      = 0;
#if NFTL_LOGICAL_PARTITION_NUM > 2
        g_block_mgr[NFTL_PARTITION2].partition        = NFTL_PHYSICAL_PARTITION2;
        g_block_mgr[NFTL_PARTITION2].logic_block_nums = NFTL_TAG_PAGE_IN_BLOCK * NFTL_PART2_BLOCK_NUMS;
        g_block_mgr[NFTL_PARTITION2].phy_block_nums   = NFTL_PART2_BLOCK_NUMS + NFTL_PART2_BUFFER_BLOCK_NUMS;
        g_block_mgr[NFTL_PARTITION2].initialized      = 0;
#endif /* NFTL_LOGICAL_PARTITION_NUM > 1 */
#endif /* NFTL_LOGICAL_PARTITION_NUM > 2 */

        for (partition = NFTL_PARTITION0; partition < 1; partition = (nftl_partition_t)(partition + 1)) {
            blk_mgr = &g_block_mgr[partition];

            ret = nftl_init_partition(blk_mgr);

            if (ret != NFTL_ERR_OK) {
                NFTL_ERROR("[%s] init partition %d failed: %d", __func__, partition, ret);
                return;
            }
        }
    }

#ifdef NFTL_GC_IN_THREAD
//    krhino_task_dyn_create(&g_nftl_gc_bg_task, "gctask", NULL, 31, 0, 4096, nftl_gc_bg_task, 1);

    aos_task_new("gctask", nftl_gc_bg_task, NULL, 4096);
#endif
}

void nftl_deinit(void)
{
    nftl_partition_t      partition;
    nftl_block_mgr_t     *blk_mgr;

    g_block_mgr[NFTL_PARTITION0].initialized      = 0;
#if NFTL_LOGICAL_PARTITION_NUM > 1
    g_block_mgr[NFTL_PARTITION1].initialized      = 0;
#if NFTL_LOGICAL_PARTITION_NUM > 2
    g_block_mgr[NFTL_PARTITION2].initialized      = 0,
#endif /* NFTL_LOGICAL_PARTITION_NUM > 1 */
#endif /* NFTL_LOGICAL_PARTITION_NUM > 2 */


    for (partition = NFTL_PARTITION0; partition < 1; partition = (nftl_partition_t)(partition + 1)) {
        blk_mgr = &g_block_mgr[partition];

        free(blk_mgr->logic_blocks);
        free(blk_mgr->phy_blocks);

    }
}

int nftl_flash_eraseall(nftl_partition_t p)
{
    int ret;
    nftl_block_mgr_t *blk_mgr;

#ifdef CONFIG_LFS_SYSTEM_READONLY
    if (p == NFTL_PARTITION0 || p == NFTL_PARTITION1) {
        NFTL_WARN("[nftl] %s partition %d readonly\r\n", __func__, p);
        return NFTL_ERR_NAND_READONLY;
    }
#endif

    if (p >= NFTL_PARTITION_MAX) {
        NFTL_ERROR("partition %d not found", p);
        return NFTL_ERR_NAND_UNKNOWN;
    }

    blk_mgr = &g_block_mgr[p];

    for (uint32_t blk = 0; blk < blk_mgr->phy_block_nums; blk++) {
        ret = bd.erase(blk * NFTL_BLOCK_OFFSET, NFTL_BLOCK_SIZE);
        if (ret != 0) {
            NFTL_ERROR("failed to erase block %ld part %d", blk, blk_mgr->partition);
            hal_flash_markbad(blk_mgr->partition, blk * NFTL_BLOCK_OFFSET);
        }
    }

    return NFTL_ERR_OK;
}
