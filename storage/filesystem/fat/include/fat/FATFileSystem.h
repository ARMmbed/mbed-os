/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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

#ifndef MBED_FATFILESYSTEM_H
#define MBED_FATFILESYSTEM_H

#include "filesystem/FileSystem.h"
#include "blockdevice/BlockDevice.h"
#include "FileHandle.h"
#include <stdint.h>
#include "PlatformMutex.h"
#include "storage/filesystem/fat/ChaN/ff.h"

namespace mbed {

/**
 * FAT file system based on ChaN's FAT file system library v0.8
 *
 * Synchronization level: Thread safe
 */
class FATFileSystem : public FileSystem {
public:
    /** Lifetime of the FAT file system.
     *
     *  @param name     Name of the file system in the tree.
     *  @param bd       Block device to mount. Mounted immediately if not NULL.
     */
    FATFileSystem(const char *name = NULL, BlockDevice *bd = NULL);
    virtual ~FATFileSystem();

    /** Format a logical drive, FDISK partitioning rule.
     *
     *  The block device to format should be mounted when this function is called.
     *
     *  @param bd
     *    This is the block device that will be formatted.
     *
     *  @param cluster_size
     *    This is the number of bytes per cluster. A larger cluster size decreases
     *    the overhead of the FAT table, but also increases the minimum file size. The
     *    cluster size must be a multiple of the underlying device's allocation unit
     *    and is currently limited to a max of 32,768 bytes. If the cluster size is set to zero, a cluster size
     *    is determined from the device's allocation unit. Defaults to zero.
     *
     *  @return         0 on success, negative error code on failure.
     */
    static int format(BlockDevice *bd, bd_size_t cluster_size = 0);

    /** Mount a file system to a block device.
     *
     *  @param bd       Block device to mount to.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int mount(BlockDevice *bd);

    /** Unmount a file system from the underlying block device.
     *
     *  @return         0 on success, negative error code on failure.
     */
    virtual int unmount();

    /** Reformat a file system, results in an empty and mounted file system.
     *
     *  @param bd
     *      Block device to reformat and mount. If NULL, the mounted
     *      Block device is used.
     *      Note: If mount fails, bd must be provided.
     *      Default: NULL
     *
     *  @param allocation_unit
     *      This is the number of bytes per cluster size. The valid value is N
     *      times the sector size. N is a power of 2, from 1 to 128, for the FAT
     *      volume, and up to 16MiB for the exFAT volume. If zero is given,
     *      the default allocation unit size is selected by the underlying
     *      file system, which depends on the volume size.
     *
     *  @return         0 on success, negative error code on failure.
     */
    virtual int reformat(BlockDevice *bd, int allocation_unit);

    /** Reformat a file system, results in an empty and mounted file system.
     *
     *  @param bd       Block device to reformat and mount. If NULL, the mounted
     *                  Block device is used.
     *                  Note: If mount fails, bd must be provided.
     *                  Default: NULL
     *  @return         0 on success, negative error code on failure.
     */
    virtual int reformat(BlockDevice *bd = NULL)
    {
        // Required for virtual inheritance shenanigans.
        return reformat(bd, 0);
    }

    /** Remove a file from the file system.
     *
     *  @param path     The name of the file to remove.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int remove(const char *path);

    /** Rename a file in the file system.
     *
     *  @param path     The current name of the file to rename.
     *  @param newpath  The new name of the file.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int rename(const char *path, const char *newpath);

    /** Store information about the file in a stat structure.
     *
     *  @param path     The name of the file to store information about.
     *  @param st       The stat buffer to write to.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int stat(const char *path, struct stat *st);

    /** Create a directory in the file system.
     *
     *  @param path     The name of the directory to create.
     *  @param mode     The permissions with which to create the directory.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int mkdir(const char *path, mode_t mode);

    /** Store information about the mounted file system in a statvfs structure.
     *
     *  @param path     The name of the file to store information about.
     *  @param buf      The stat buffer to write to.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int statvfs(const char *path, struct statvfs *buf);

protected:
#if !(DOXYGEN_ONLY)
    /** Open a file on the file system.
     *
     *  @param file     Destination for the handle to a newly created file.
     *  @param path     The name of the file to open.
     *  @param flags    The flags that trigger opening of the file. These flags are O_RDONLY, O_WRONLY, and O_RDWR,
     *                  with an O_CREAT, O_TRUNC, or O_APPEND bitwise OR operator.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int file_open(fs_file_t *file, const char *path, int flags);

    /** Close a file
     *
     *  @param file     File handle.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int file_close(fs_file_t file);

    /** Read the contents of a file into a buffer.
     *
     *  @param file     File handle.
     *  @param buffer   The buffer to read into.
     *  @param len      The number of bytes to read.
     *  @return         The number of bytes read; 0 at the end of the file, negative error on failure.
     */
    virtual ssize_t file_read(fs_file_t file, void *buffer, size_t len);

    /** Write the contents of a buffer to a file.
     *
     *  @param file     File handle.
     *  @param buffer   The buffer to write from.
     *  @param len      The number of bytes to write.
     *  @return         The number of bytes written, negative error on failure.
     */
    virtual ssize_t file_write(fs_file_t file, const void *buffer, size_t len);

    /** Flush any buffers associated with the file.
     *
     *  @param file     File handle.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int file_sync(fs_file_t file);

    /** Move the file position to a given offset from a given location
     *
     *  @param file     File handle.
     *  @param offset   The offset from whence to move to.
     *  @param whence   The start of where to seek.
     *      SEEK_SET to start from the beginning of the file,
     *      SEEK_CUR to start from the current position in the file,
     *      SEEK_END to start from the end of the file.
     *  @return         The new offset of the file.
     */
    virtual off_t file_seek(fs_file_t file, off_t offset, int whence);

    /** Get the file position of the file.
     *
     *  @param file     File handle.
     *  @return         The current offset in the file.
     */
    virtual off_t file_tell(fs_file_t file);

    /** Get the size of the file.
     *
     *  @param file     File handle.
     *  @return         Size of the file in bytes.
     */
    virtual off_t file_size(fs_file_t file);

    /** Truncate or extend a file.
     *
     * The file's length is set to the specified value. The seek pointer is
     * not changed. If the file is extended, the extended area appears as if
     * it were zero-filled.
     *
     *  @param file     File handle.
     *  @param length   The requested new length for the file.
     *
     *  @return         0 on success, negative error code on failure.
     */
    virtual int file_truncate(mbed::fs_file_t file, off_t length);

    /** Open a directory on the file system.
     *
     *  @param dir      Destination for the handle to the directory.
     *  @param path     Name of the directory to open.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int dir_open(fs_dir_t *dir, const char *path);

    /** Close a directory
     *
     *  @param dir      Dir handle.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int dir_close(fs_dir_t dir);

    /** Read the next directory entry
     *
     *  @param dir      Dir handle.
     *  @param ent      The directory entry to fill out.
     *  @return         1 on reading a filename, 0 at the end of the directory, negative error on failure.
     */
    virtual ssize_t dir_read(fs_dir_t dir, struct dirent *ent);

    /** Set the current position of the directory.
     *
     *  @param dir      Dir handle.
     *  @param offset   Offset of the location to seek to.
     *                  Must be a value returned by dir_tell.
     */
    virtual void dir_seek(fs_dir_t dir, off_t offset);

    /** Get the current position of the directory.
     *
     *  @param dir      Dir handle.
     *  @return         Directory position, which can be passed to dir_rewind.
     */
    virtual off_t dir_tell(fs_dir_t dir);

    /** Rewind the current position to the beginning of the directory.
     *
     *  @param dir      Dir handle.
     */
    virtual void dir_rewind(fs_dir_t dir);
#endif //!(DOXYGEN_ONLY)

private:
    FATFS _fs; // Work area (file system object) for logical drive.
    char _fsid[sizeof("0:")];
    int _id;

protected:
    virtual void lock();
    virtual void unlock();
    virtual int mount(BlockDevice *bd, bool mount);
};

} // namespace mbed

// Added "using" for backwards compatibility.
#ifndef MBED_NO_GLOBAL_USING_DIRECTIVE
using mbed::FATFileSystem;
#endif

#endif

/** @}*/
