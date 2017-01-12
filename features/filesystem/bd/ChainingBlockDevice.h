/* mbed Microcontroller Library
 * Copyright (c) 2006-2012 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef MBED_CHAINING_BLOCK_DEVICE_H
#define MBED_CHAINING_BLOCK_DEVICE_H

#include "BlockDevice.h"
#include "mbed.h"


/** Block device for chaining multiple block devices
 *  with the same block size at sequential addresses
 *
 *  @code
 *  #include "mbed.h"
 *  #include "HeapBlockDevice.h"
 *  #include "ChainingBlockDevice.h"
 *
 *  // Create two smaller block devices with
 *  // 64 and 32 blocks of size 512 bytes
 *  HeapBlockDevice mem1(512, 64);
 *  HeapBlockDevice mem2(512, 32);
 *
 *  // Create a block device backed by mem1 and mem2
 *  // contains 96 blocks of size 512 bytes
 *  BlockDevice *bds[] = {&mem1, &mem2};
 *  ChainingBlockDevice chainmem(bds);
 */
class ChainingBlockDevice : public BlockDevice {
public:
    /** Lifetime of the memory block device
     *
     *  @param bds      Array of block devices to chain with sequential block addresses
     *  @param count    Number of block devices to chain
     *  @note All block devices must have the same block size
     */
    ChainingBlockDevice(BlockDevice **bds, size_t bd_count);

    /** Lifetime of the memory block device
     *
     *  @param bds      Array of block devices to chain with sequential block addresses
     *  @note All block devices must have the same block size
     */
    template <size_t SIZE>
    ChainingBlockDevice(BlockDevice *(&bds)[SIZE])
        : _bds(bds), _bd_count(sizeof(bds) / sizeof(bds[0]))
        , _block_size(0), _block_count(0) {
    }

    /** Initialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual bd_error_t init();

    /** Deinitialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual bd_error_t deinit();

    /** Read blocks from a block device
     *
     *  @param buffer   Buffer to write block to
     *  @param block    Index of block to begin reading from
     *  @param count    Number of blocks to read (defaults to 1)
     *  @return         Either the number of blocks read or a negative error code on failure
     */
    virtual bd_count_or_error_t read(void *buffer, bd_block_t block, bd_count_t count = 1);

    /** Write blocks to a block device
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param block    Index of block to begin writing to
     *  @param count    Number of blocks to write (defaults to 1)
     *  @return         Either the number of blocks written or a negative error code on failure
     */
    virtual bd_count_or_error_t write(const void *buffer, bd_block_t block, bd_count_t count = 1);

    /** Sync an underlying block device
     *
     *  @return         0 on success or a negative error code on failure
     */
    virtual bd_error_t sync();

    /** Queries the status of the underlying block device
     *
     *  @return         Status of the underlying device, this is 0 for
     *                  a ready device, or a negative error code if a read
     *                  or write could fail.
     */
    virtual bd_error_t status();

    /** Get the block size of the underlying device
     *
     *  @return         The number of 8-bit bytes in a block
     */
    virtual bd_size_t get_block_size();

    /** Get the block count of the underlying device
     *
     *  @return         The number of blocks on the device
     */
    virtual bd_count_t get_block_count();

protected:
    BlockDevice **_bds;
    size_t _bd_count;
    bd_size_t _block_size;
    bd_count_t _block_count;
};


#endif
