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
#ifndef MBED_ATOMIC_USAGE_H
#define MBED_ATOMIC_USAGE_H

#include "BlockDevice.h"

/**
 * Setup the given block device to test littlefs atomic operations
 *
 * Format the blockdevice with a littlefs filesystem and create
 * the files and directories required to test atomic operations.
 *
 * @param bd Block device format and setup
 * @param force_rebuild Force a reformat even if the device is already setup
 * @return true if the block device was formatted, false otherwise
 * @note utest asserts are used to detect fatal errors so utest must be
 * initialized before calling this function.
 */
bool setup_atomic_operations(BlockDevice *bd, bool force_rebuild);

/**
 * Perform a set of atomic littlefs operations on the block device
 *
 * Mount the block device as a littlefs filesystem and a series of
 * atomic operations on it. Since the operations performed are atomic
 * the file system will always be in a well defined state. The block
 * device must have been setup by calling setup_atomic_operations.
 *
 * @param bd Block device to perform the operations on
 * @return -1 if flash is exhausted, otherwise the cycle count on the fs
 * @note utest asserts are used to detect fatal errors so utest must be
 * initialized before calling this function.
 */
int64_t perform_atomic_operations(BlockDevice *bd);

/**
 * Check that the littlefs image on the block device is in a good state
 *
 * Mount the block device as a littlefs filesystem and check the files
 * and directories to ensure they are valid. Since all the operations
 * performed are atomic the filesystem should always be in a good
 * state.
 *
 * @param bd Block device to check
 * @note This function does not change the contents of the block device
 * @note utest asserts are used to detect fatal errors so utest must be
 * initialized before calling this function.
 */
void check_atomic_operations(BlockDevice *bd);

#endif
