/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#ifndef MBED_PROFILING_BLOCK_DEVICE_H
#define MBED_PROFILING_BLOCK_DEVICE_H

#include "BlockDevice.h"
#include "mbed.h"


/** Block device for measuring storage operations of another block device
 *
 *  @code
 *  #include "mbed.h"
 *  #include "HeapBlockDevice.h"
 *  #include "ProfilingBlockDevice.h"
 *
 *  // Create a heap block device and profiling block device
 *  HeapBlockDevice mem(64*512, 512);
 *  ProfilingBlockDevice profiler(&mem);
 *
 *  // do block device work....
 *
 *  printf("read count: %lld\n", profiler.get_read_count());
 *  printf("program count: %lld\n", profiler.get_program_count());
 *  printf("erase count: %lld\n", profiler.get_erase_count());
 */
class ProfilingBlockDevice : public BlockDevice
{
public:
    /** Lifetime of the memory block device
     *
     *  @param bd       Block device to back the ProfilingBlockDevice
     */
    ProfilingBlockDevice(BlockDevice *bd);

    /** Lifetime of a block device
     */
    virtual ~ProfilingBlockDevice() {};

    /** Initialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     *  @note The init and deinit functions do not effect profile counts
     */
    virtual int init();

    /** Deinitialize a block device
     *
     *  @return         0 on success or a negative error code on failure
     *  @note The init and deinit functions do not effect profile counts
     */
    virtual int deinit();

    /** Read blocks from a block device
     *
     *  @param buffer   Buffer to read blocks into
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes, must be a multiple of read block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int read(void *buffer, bd_addr_t addr, bd_size_t size);

    /** Program blocks to a block device
     *
     *  The blocks must have been erased prior to being programmed
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes, must be a multiple of program block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size);

    /** Erase blocks on a block device
     *
     *  The state of an erased block is undefined until it has been programmed
     *
     *  @param addr     Address of block to begin erasing
     *  @param size     Size to erase in bytes, must be a multiple of erase block size
     *  @return         0 on success, negative error code on failure
     */
    virtual int erase(bd_addr_t addr, bd_size_t size);

    /** Get the size of a readable block
     *
     *  @return         Size of a readable block in bytes
     */
    virtual bd_size_t get_read_size() const;

    /** Get the size of a programable block
     *
     *  @return         Size of a programable block in bytes
     *  @note Must be a multiple of the read size
     */
    virtual bd_size_t get_program_size() const;

    /** Get the size of a eraseable block
     *
     *  @return         Size of a eraseable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual bd_size_t get_erase_size() const;

    /** Get the total size of the underlying device
     *
     *  @return         Size of the underlying device in bytes
     */
    virtual bd_size_t size() const;

    /** Reset the current profile counts to zero
     */
    void reset();

    /** Get number of bytes that have been read from the block device
     *
     *  @return The number of bytes that have been read from the block device
     */
    bd_size_t get_read_count() const;

    /** Get number of bytes that have been programed to the block device
     *
     *  @return The number of bytes that have been programed to the block device
     */
    bd_size_t get_program_count() const;

    /** Get number of bytes that have been erased from the block device
     *
     *  @return The number of bytes that have been erased from the block device
     */
    bd_size_t get_erase_count() const;

private:
    BlockDevice *_bd;
    bd_size_t _read_count;
    bd_size_t _program_count;
    bd_size_t _erase_count;
};


#endif
