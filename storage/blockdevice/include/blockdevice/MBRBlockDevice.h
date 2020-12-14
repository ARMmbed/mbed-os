/* mbed Microcontroller Library
 * Copyright (c) 2017-2020 ARM Limited
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

/** \addtogroup storage */
/** @{*/

#ifndef MBED_MBR_BLOCK_DEVICE_H
#define MBED_MBR_BLOCK_DEVICE_H

#include "BlockDevice.h"

namespace mbed {

/** Additional error codes used for MBR records
 */
enum {
    BD_ERROR_INVALID_MBR       = -3101,
    BD_ERROR_INVALID_PARTITION = -3102,
};


/** Block device for managing a Master Boot Record
 *  https://en.wikipedia.org/wiki/Master_boot_record
 *
 *  @note
 *  The MBR partition table is relatively limited:
 *  - At most 4 partitions are supported
 *  - Extended partitions are currently not supported and will error during init
 */
class MBRBlockDevice : public BlockDevice {
public:
    /** Format the MBR to contain the following partition
     *
     *  @param bd       Block device to partition
     *  @param part     Partition to use, 1-4
     *  @param type     8-bit partition type to specify the filesystem to use in this partition.
     *                  It can be found in the filesystem documentation or refer to
     *                  https://en.wikipedia.org/wiki/Partition_type#List_of_partition_IDs
     *  @param start    Start block address to map to block 0 of partition.
     *                  Negative addresses are calculated from the end of the
     *                  underlying block devices. Block 0 is implicitly ignored
     *                  from the range to store the MBR.
     *  @return         0 on success or a negative error code on failure.
     *  @note This is the same as partition(bd, part, type, start, bd->size())
     */
    static int partition(BlockDevice *bd, int part, uint8_t type, bd_addr_t start);

    /** Format the MBR to contain the following partition
     *
     *  @param bd       Block device to partition
     *  @param part     Partition to use, 1-4
     *  @param type     8-bit partition type to specify the filesystem to use in this partition.
     *                  It can be found in the filesystem documentation or refer to
     *                  https://en.wikipedia.org/wiki/Partition_type#List_of_partition_IDs
     *  @param start    Start block address to map to block 0 of partition,
     *                  negative addresses are calculated from the end of the
     *                  underlying block devices. Block 0 is implicitly ignored
     *                  from the range to store the MBR.
     *  @param stop     End block address to mark the end of the partition.
     *                  This block is not mapped: negative addresses are calculated
     *                  from the end of the underlying block device.
     *  @return         0 on success or a negative error code on failure.
     */
    static int partition(BlockDevice *bd, int part, uint8_t type, bd_addr_t start, bd_addr_t stop);

    /** Lifetime of the block device
     *
     *  @param bd       Block device to back the MBRBlockDevice
     *  @param part     Partition to use, 1-4
     */
    MBRBlockDevice(BlockDevice *bd, int part);

    /** Lifetime of the block device
     */
    virtual ~MBRBlockDevice() {};

    /** Initialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual int init();

    /** Deinitialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual int deinit();

    /** Ensure data on storage is in sync with the driver
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual int sync();

    /** Read blocks from a block device
     *
     *  @param buffer   Buffer to read blocks into
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes, must be a multiple of read block size
     *  @return         0 on success or a negative error code on failure
     */
    virtual int read(void *buffer, bd_addr_t addr, bd_size_t size);

    /** Program blocks to a block device
     *
     *  The blocks must have been erased prior to being programmed
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes, must be a multiple of program block size
     *  @return         0 on success or a negative error code on failure
     */
    virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size);

    /** Erase blocks on a block device
     *
     *  The state of an erased block is undefined until it has been programmed,
     *  unless get_erase_value returns a non-negative byte value
     *
     *  @param addr     Address of block to begin erasing
     *  @param size     Size to erase in bytes, must be a multiple of erase block size
     *  @return         0 on success or a negative error code on failure
     */
    virtual int erase(bd_addr_t addr, bd_size_t size);

    /** Get the size of a readable block
     *
     *  @return         Size of a readable block in bytes
     */
    virtual bd_size_t get_read_size() const;

    /** Get the size of a programmable block
     *
     *  @return         Size of a programmable block in bytes
     *  @note Must be a multiple of the read size
     */
    virtual bd_size_t get_program_size() const;

    /** Get the size of an erasable block
     *
     *  @return         Size of an erasable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual bd_size_t get_erase_size() const;

    /** Get the size of an erasable block given address
     *
     *  @param addr     Address within the erasable block
     *  @return         Size of an erasable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual bd_size_t get_erase_size(bd_addr_t addr) const;

    /** Get the value of storage when erased
     *
     *  If get_erase_value returns a non-negative byte value, the underlying
     *  storage is set to that value when erased, and storage containing
     *  that value can be programmed without another erase.
     *
     *  @return         The value of storage when erased, or -1 if you can't
     *                  rely on the value of erased storage
     */
    virtual int get_erase_value() const;

    /** Get the total size of the underlying device
     *
     *  @return         Size of the underlying device in bytes
     */
    virtual bd_size_t size() const;

    /** Get the offset of the partition on the underlying block device
     *  @return         Offset of the partition on the underlying device
     */
    virtual bd_addr_t get_partition_start() const;

    /** Get size of partition on underlying block device
     *  @return         Size of the partition on the underlying device
     */
    virtual bd_addr_t get_partition_stop() const;

    /** Get 8-bit type of the partition
     *  @return         8-bit type of partition assigned during format
     */
    virtual uint8_t get_partition_type() const;

    /** Get the partition number
     *  @return         The partition number, 1-4
     */
    virtual int get_partition_number() const;

    /** Get the BlockDevice class type.
     *
     *  @return         A string represent the BlockDevice class type.
     */
    virtual const char *get_type() const;

protected:
    BlockDevice *_bd;
    bd_size_t _offset;
    bd_size_t _size;
    uint8_t _type;
    uint8_t _part;
    uint32_t _init_ref_count;
    bool _is_initialized;
};

} // namespace mbed

// Added "using" for backwards compatibility
#ifndef MBED_NO_GLOBAL_USING_DIRECTIVE
using mbed::MBRBlockDevice;
#endif

#endif

/** @}*/
