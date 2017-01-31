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
#ifndef MBED_SLICING_BLOCK_DEVICE_H
#define MBED_SLICING_BLOCK_DEVICE_H

#include "BlockDevice.h"
#include "mbed.h"


/** Block device for mapping to a slice of another block device
 *
 *  @code
 *  #include "mbed.h"
 *  #include "HeapBlockDevice.h"
 *  #include "SlicingBlockDevice.h"
 *
 *  // Create a block device with 64 blocks of size 512
 *  HeapBlockDevice mem(512, 64);
 *
 *  // Create a block device that maps to the first 32 blocks
 *  SlicingBlockDevice slice1(&mem, 0, 32);
 *
 *  // Create a block device that maps to the last 32 blocks
 *  SlicingBlockDevice slice2(&mem, 32, 64);
 *
 *  // Create a block device that maps to the middle 32 blocks
 *  SlicingBlockDevice slice3(&mem, 16, 48);
 */
class SlicingBlockDevice : public BlockDevice {
public:
    /** Lifetime of the memory block device
     *
     *  @param bd       Block device to back the SlicingBlockDevice
     *  @param start    Start block index to map to block 0
     *  @param stop     End block index to mark the end of the block device,
     *                  this block is not mapped
     */
    SlicingBlockDevice(BlockDevice *bd, bd_block_t start, bd_block_t end);

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
    BlockDevice *_bd;
    bd_block_t _start;
    bd_block_t _stop;
};


#endif
