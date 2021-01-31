/***************************************************************************//**
* \file CyReservedRegionBlockDevice.h
*
* \brief
* Block device for working via an underlying block device without altering
* a reserved region.
*
********************************************************************************
* \copyright
* Copyright 2020 Cypress Semiconductor Corporation
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
*******************************************************************************/

#ifndef CY_RESERVED_REGION_BLOCK_DEVICE_H
#define CY_RESERVED_REGION_BLOCK_DEVICE_H

#include "mbed.h"

/**
 * Block device for working via an underlying block device without altering
* a reserved region.
 */
class CyReservedRegionBlockDevice : public mbed::BlockDevice {
public:
    /**
     * Create a CyReservedRegionBlockDevice.  The reserved region will be created at the beginning of the external storage.
     *
     * @param[in] underlying_bd        The underlying block device to use
     * @param[in] reserved_region_size Size of the reserved region (will be rounded up to the nearest erase size).
     */
    CyReservedRegionBlockDevice(BlockDevice *underlying_bd, mbed::bd_size_t reserved_region_size);

    /**
     * Initialize the block device.
     *
     * @return 0 on success. A nonzero error code from the underlying block device on failure.
     */
    virtual int init() override;

    /**
     * Deinitialize the block device.
     *
     * @return 0 on success. A nonzero error code from the underlying block device on failure.
     */
    virtual int deinit() override;

    /**
     * Destruct the block device.
     *
     * @return 0 on success. A nonzero error code from the underlying block device on failure.
     */
    virtual ~CyReservedRegionBlockDevice() override
    {
        deinit();
    }

    /**
     * Read blocks from the block device.
     *
     * @param[out] buffer   Buffer to write blocks to
     * @param[in]  addr     Address of block to begin reading from
     * @param[in]  size     Size to read in bytes, must be a multiple of read block size
     * @return 0 on success. A nonzero error code from the underlying block device on failure.
     */
    virtual int read(void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size) override;

    /**
     * Program blocks to the block device.
     *
     * The blocks must have been erased prior to being programmed.
     *
     * @param[in] buffer   Buffer of data to write to blocks
     * @param[in] addr     Address of block to begin writing to
     * @param[in] size     Size to write in bytes, must be a multiple of program block size
     * @return 0 on success.  A nonzero error code from the underlying block device on failure.
     */
    virtual int program(const void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size) override;

    /**
     * Erase blocks on a block device.
     *
     * The state of an erased block is undefined until it has been programmed.
     *
     * @param[in] addr     Address of block to begin erasing
     * @param[in] size     Size to erase in bytes, must be a multiple of erase block size
     * @return 0 on success.  A nonzero error code from the underlying block device on failure.
     */
    virtual int erase(mbed::bd_addr_t addr, mbed::bd_size_t size) override;

    /**
     * Get the size of a readable block.
     *
     * @return Size of a readable block in bytes
     */
    virtual mbed::bd_size_t get_read_size() const override;

    /**
     * Get the size of a programmable block.
     *
     * @return Size of a program block size in bytes
     * @note Must be a multiple of the read size.
     */
    virtual mbed::bd_size_t get_program_size() const override;

    /**
     * Get the size of an erasable block.
     *
     * @return Size of a minimal erase block, common to all regions, in bytes
     * @note Must be a multiple of the program size.
     */
    virtual mbed::bd_size_t get_erase_size() const override;

    /**
     * Get the minimal erasable sector size of given address.
     *
     * @param[in] addr Any address within block queried for erase sector size (can be any address within flash size offset)
     * @return Size of minimal erase sector size, in given address region, in bytes
     * @note Must be a multiple of the program size.
     */
    virtual mbed::bd_size_t get_erase_size(mbed::bd_addr_t addr) const override;

    /**
     * Get the value of storage byte after it was erased.
     *
     * If get_erase_value returns a non-negative byte value, the underlying
     * storage is set to that value when erased, and storage containing
     * that value can be programmed without another erase.
     *
     * @return The value of storage when erased, or -1 if you can't
     *         rely on the value of erased storage
     */
    virtual int get_erase_value() const override;

    /**
     * Get the total size of the region of the underlying device available for use.
     *
     * @return Size of usable region in bytes
     */

    virtual mbed::bd_size_t size() const override;

    /**
     * Get the BlockDevice class type.
     *
     *  @return A string represent the BlockDevice class type.
     */
    virtual const char *get_type() const override;

    /**
     * Read from the reserved region.
     *
     * @param[out] buffer Buffer to read data into
     * @param[in]  addr   Address to start read from
     * @param[in]  size   Size to read in bytes, must be a multiple of the read block size
     * @return 0 on success.  A nonzero error code from the underlying block device on failure.
     */
    int reserved_read(void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /**
     * Get the end address of the reserved region.
     *
     * @return The end address of the reserved region
     */
    mbed::bd_addr_t reserved_region_end() const;

private:
    BlockDevice *_underlying_bd;
    mbed::bd_size_t _reserved_region_size; // Initial size passed in; used in init() to compute _reserved_region_end
    mbed::bd_addr_t _reserved_region_end; // Actual used reserved region end; rounded up to nearest erase size
};

#endif /* CY_RESERVED_REGION_BLOCK_DEVICE_H */
