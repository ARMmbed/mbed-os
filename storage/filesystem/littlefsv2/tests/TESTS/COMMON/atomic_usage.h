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
