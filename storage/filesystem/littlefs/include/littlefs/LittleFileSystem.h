/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#ifndef MBED_LFSFILESYSTEM_H
#define MBED_LFSFILESYSTEM_H

#include "filesystem/FileSystem.h"
#include "blockdevice/BlockDevice.h"
#include "platform/PlatformMutex.h"
#include "littlefs/lfs.h"

namespace mbed {

/**
 * LittleFileSystem, a little file system
 *
 * Synchronization level: Thread safe
 */
class LittleFileSystem : public mbed::FileSystem {
public:
    /** Lifetime of the LittleFileSystem
     *
     *  @param name     Name of the file system in the tree.
     *  @param bd       Block device to mount. Mounted immediately if not NULL.
     *  @param read_size
     *      Minimum size of a block read. This determines the size of read buffers.
     *      This may be larger than the physical read size to improve performance
     *      by caching more of the block device.
     *  @param prog_size
     *      Minimum size of a block program. This determines the size of program
     *      buffers. This may be larger than the physical program size to improve
     *      performance by caching more of the block device.
     *  @param block_size
     *      Size of an erasable block. This does not impact ram consumption and
     *      may be larger than the physical erase size. However, this should be
     *      kept small as each file currently takes up an entire block.
     *  @param lookahead
     *      Number of blocks to lookahead during block allocation. A larger
     *      lookahead reduces the number of passes required to allocate a block.
     *      The lookahead buffer requires only 1 bit per block so it can be quite
     *      large with little ram impact. Should be a multiple of 32.
     */
    LittleFileSystem(const char *name = NULL, mbed::BlockDevice *bd = NULL,
                     lfs_size_t read_size = MBED_LFS_READ_SIZE,
                     lfs_size_t prog_size = MBED_LFS_PROG_SIZE,
                     lfs_size_t block_size = MBED_LFS_BLOCK_SIZE,
                     lfs_size_t lookahead = MBED_LFS_LOOKAHEAD);

    virtual ~LittleFileSystem();

    /** Format a block device with the LittleFileSystem.
     *
     *  The block device to format should be mounted when this function is called.
     *
     *  @param bd       This is the block device that will be formatted.
     *  @param read_size
     *      Minimum size of a block read. This determines the size of read buffers.
     *      This may be larger than the physical read size to improve performance
     *      by caching more of the block device.
     *  @param prog_size
     *      Minimum size of a block program. This determines the size of program
     *      buffers. This may be larger than the physical program size to improve
     *      performance by caching more of the block device.
     *  @param block_size
     *      Size of an erasable block. This does not impact ram consumption and
     *      may be larger than the physical erase size. However, this should be
     *      kept small as each file currently takes up an entire block.
     *  @param lookahead
     *      Number of blocks to lookahead during block allocation. A larger
     *      lookahead reduces the number of passes required to allocate a block.
     *      The lookahead buffer requires only 1 bit per block so it can be quite
     *      large with little ram impact. Should be a multiple of 32.
     */
    static int format(mbed::BlockDevice *bd,
                      lfs_size_t read_size = MBED_LFS_READ_SIZE,
                      lfs_size_t prog_size = MBED_LFS_PROG_SIZE,
                      lfs_size_t block_size = MBED_LFS_BLOCK_SIZE,
                      lfs_size_t lookahead = MBED_LFS_LOOKAHEAD);

    /** Mount a file system to a block device.
     *
     *  @param bd       Block device to mount to.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int mount(mbed::BlockDevice *bd);

    /** Unmount a file system from the underlying block device.
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual int unmount();

    /** Reformat a file system. Results in an empty and mounted file system.
     *
     *  @param bd
     *      Block device to reformat and mount. If NULL, the mounted
     *      Block device is used.
     *      Note: If mount fails, bd must be provided.
     *      Default: NULL
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual int reformat(mbed::BlockDevice *bd);

    /** Remove a file from the file system.
     *
     *  @param path     The name of the file to remove.
     *  @return         0 on success, negative error code on failure
     */
    virtual int remove(const char *path);

    /** Rename a file in the file system.
     *
     *  @param path     The name of the file to rename.
     *  @param newpath  The name to rename it to.
     *  @return         0 on success, negative error code on failure
     */
    virtual int rename(const char *path, const char *newpath);

    /** Store information about the file in a stat structure
     *
     *  @param path     The name of the file to find information about.
     *  @param st       The stat buffer to write to.
     *  @return         0 on success, negative error code on failure
     */
    virtual int stat(const char *path, struct stat *st);

    /** Create a directory in the file system.
     *
     *  @param path     The name of the directory to create.
     *  @param mode     The permissions with which to create the directory.
     *  @return         0 on success, negative error code on failure
     */
    virtual int mkdir(const char *path, mode_t mode);

    /** Store information about the mounted file system in a statvfs structure.
     *
     *  @param path     The name of the file to find information about.
     *  @param buf      The stat buffer to write to.
     *  @return         0 on success, negative error code on failure
     */
    virtual int statvfs(const char *path, struct statvfs *buf);

protected:
#if !(DOXYGEN_ONLY)
    /** Open a file on the file system.
     *
     *  @param file     Destination of the newly created handle to the referenced file.
     *  @param path     The name of the file to open.
     *  @param flags    The flags that trigger opening of the file. These flags are O_RDONLY, O_WRONLY, and O_RDWR,
     *                  with an O_CREAT, O_TRUNC, or O_APPEND bitwise OR operator.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int file_open(mbed::fs_file_t *file, const char *path, int flags);

    /** Close a file
     *
     *  @param file     File handle.
     *  return          0 on success, negative error code on failure
     */
    virtual int file_close(mbed::fs_file_t file);

    /** Read the contents of a file into a buffer
     *
     *  @param file     File handle.
     *  @param buffer   The buffer to read in to.
     *  @param size     The number of bytes to read.
     *  @return         The number of bytes read, 0 at end of file, negative error on failure
     */
    virtual ssize_t file_read(mbed::fs_file_t file, void *buffer, size_t size);

    /** Write the contents of a buffer to a file
     *
     *  @param file     File handle.
     *  @param buffer   The buffer to write from.
     *  @param size     The number of bytes to write.
     *  @return         The number of bytes written, negative error on failure
     */
    virtual ssize_t file_write(mbed::fs_file_t file, const void *buffer, size_t size);

    /** Flush any buffers associated with the file
     *
     *  @param file     File handle.
     *  @return         0 on success, negative error code on failure
     */
    virtual int file_sync(mbed::fs_file_t file);

    /** Move the file position to a given offset from a given location
     *
     *  @param file     File handle.
     *  @param offset   The offset from whence to move to.
     *  @param whence   The start of where to seek.
     *      SEEK_SET to start from beginning of file,
     *      SEEK_CUR to start from current position in file,
     *      SEEK_END to start from end of file.
     *  @return         The new offset of the file
     */
    virtual off_t file_seek(mbed::fs_file_t file, off_t offset, int whence);

    /** Get the file position of the file
     *
     *  @param file     File handle.
     *  @return         The current offset in the file
     */
    virtual off_t file_tell(mbed::fs_file_t file);

    /** Get the size of the file
     *
     *  @param file     File handle.
     *  @return         Size of the file in bytes
     */
    virtual off_t file_size(mbed::fs_file_t file);

    /** Truncate or extend a file.
     *
     * The file's length is set to the specified value. The seek pointer is
     * not changed. If the file is extended, the extended area appears as if
     * it were zero-filled.
     *
     *  @param file     File handle.
     *  @param length   The requested new length for the file
     *
     *  @return         Zero on success, negative error code on failure
     */
    virtual int file_truncate(mbed::fs_file_t file, off_t length);

    /** Open a directory on the file system.
     *
     *  @param dir      Destination for the handle to the directory.
     *  @param path     Name of the directory to open.
     *  @return         0 on success, negative error code on failure
     */
    virtual int dir_open(mbed::fs_dir_t *dir, const char *path);

    /** Close a directory
     *
     *  @param dir      Dir handle.
     *  return          0 on success, negative error code on failure
     */
    virtual int dir_close(mbed::fs_dir_t dir);

    /** Read the next directory entry
     *
     *  @param dir      Dir handle.
     *  @param ent      The directory entry to fill out.
     *  @return         1 on reading a filename, 0 at end of directory, negative error on failure
     */
    virtual ssize_t dir_read(mbed::fs_dir_t dir, struct dirent *ent);

    /** Set the current position of the directory
     *
     *  @param dir      Dir handle.
     *  @param offset   Offset of the location to seek to,
     *                  must be a value returned from dir_tell
     */
    virtual void dir_seek(mbed::fs_dir_t dir, off_t offset);

    /** Get the current position of the directory
     *
     *  @param dir      Dir handle.
     *  @return         Position of the directory that can be passed to dir_rewind
     */
    virtual off_t dir_tell(mbed::fs_dir_t dir);

    /** Rewind the current position to the beginning of the directory
     *
     *  @param dir      Dir handle
     */
    virtual void dir_rewind(mbed::fs_dir_t dir);
#endif //!(DOXYGEN_ONLY)

private:
    lfs_t _lfs; // The actual file system
    struct lfs_config _config;
    mbed::BlockDevice *_bd; // The block device

    // default parameters
    const lfs_size_t _read_size;
    const lfs_size_t _prog_size;
    const lfs_size_t _block_size;
    const lfs_size_t _lookahead;

    // thread-safe locking
    PlatformMutex _mutex;
};

} // namespace mbed

// Added "using" for backwards compatibility
#ifndef MBED_NO_GLOBAL_USING_DIRECTIVE
using mbed::LittleFileSystem;
#endif

#endif

/** @}*/
