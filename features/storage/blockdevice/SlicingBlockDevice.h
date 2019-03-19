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

/** \addtogroup storage */
/** @{*/

#ifndef MBED_SLICING_BLOCK_DEVICE_H
#define MBED_SLICING_BLOCK_DEVICE_H

#include "BlockDevice.h"
#include "platform/mbed_assert.h"

namespace mbed {

/** Block device for mapping to a slice of another block device
 */
class SlicingBlockDevice : public BlockDevice {
public:
    /** Lifetime of the memory block device
     *
     *  @param bd       Block device to back the SlicingBlockDevice
     *  @param start    Start block address to map to block 0, negative addresses
     *                  are calculated from the end of the underlying block device
     *  @param end      End block address to mark the end of the block device,
     *                  this block is not mapped, negative addresses are
     *                  calculated from the end of the underlying block device.
     *                  The default stops at end of the underlying block device.
     */
    SlicingBlockDevice(BlockDevice *bd, bd_addr_t start, bd_addr_t end = 0);

    /** Lifetime of a block device
     */
    virtual ~SlicingBlockDevice() {};

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

    /** Get the BlockDevice class type.
     *
     *  @return         A string represent the BlockDevice class type.
     */
    virtual const char *get_type() const;

    /** Convenience function for checking block program validity
     *
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes
     *  @return         True if program is valid for underlying block device
     */
    virtual bool is_valid_program(bd_addr_t addr, bd_size_t size) const;

    /** Convenience function for checking block read validity
     *
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes
     *  @return         True if read is valid for underlying block device
     */
    virtual bool is_valid_read(bd_addr_t addr, bd_size_t size) const;

    /** Convenience function for checking block erase validity
     *
     *  @param addr     Address of block to begin erasing
     *  @param size     Size to erase in bytes
     *  @return         True if erase is valid for underlying block device
     */
    virtual bool is_valid_erase(bd_addr_t addr, bd_size_t size) const;


protected:
    BlockDevice *_bd;
    bool _start_from_end;
    bd_size_t _start;
    bool _stop_from_end;
    bd_size_t _stop;
};

} // namespace mbed

// Added "using" for backwards compatibility
#ifndef MBED_NO_GLOBAL_USING_DIRECTIVE
using mbed::SlicingBlockDevice;
#endif

#endif

/** @}*/
