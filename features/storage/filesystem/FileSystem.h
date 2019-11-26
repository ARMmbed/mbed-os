
/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_FILESYSTEM_H
#define MBED_FILESYSTEM_H

#include "platform/platform.h"

#include "platform/FileBase.h"
#include "platform/FileHandle.h"
#include "platform/DirHandle.h"
#include "platform/FileSystemLike.h"
#include "features/storage/blockdevice/BlockDevice.h"

namespace mbed {
/** \addtogroup file system */
/** @{*/


// Opaque pointer representing files and directories.
typedef void *fs_file_t;
typedef void *fs_dir_t;

// Predeclared classes.
class Dir;
class File;

/** A file system object. Provides file system operations and file operations
 *  for the File and Dir classes on a block device.
 *
 *  Implementations must provide at minimum file operations and mount
 *  operations for block devices.
 *
 * @note Synchronization level: Set by subclass
 */
class FileSystem : public FileSystemLike {
public:

    /** File system lifetime.
     *
     *  @param name       Name to add file system to tree as.
     */
    FileSystem(const char *name = NULL);
    virtual ~FileSystem() {}

    /** Return the default file system.
     *
     * Returns the default file system based on the default block device configuration.
     * Use the components in target.json or application config to change
     * the default block device and affect the default filesystem.
     * SD block device => FAT filesystem
     * QSPIF, SPIF, DATAFLASH or FLAHIAP block device => LITTLE filesystem
     *
     * An application can override all target settings by implementing
     * FileSystem::get_default_instance() - the default
     * definition is weak, and calls get_target_default_instance().
     */
    static FileSystem *get_default_instance();

    /** Mount a file system to a block device.
     *
     *  @param bd       Block device to mount to.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int mount(BlockDevice *bd) = 0;

    /** Unmount a file system from the underlying block device.
     *
     *  @return         0 on success, negative error code on failure.
     */
    virtual int unmount() = 0;

    /** Reformat a file system. Results in an empty and mounted file system.
     *
     *  @param bd       Block device to reformat and mount. If NULL, the mounted
     *                  Block device is used.
     *                  Note: If mount fails, bd must be provided.
     *                  Default: NULL
     *  @return         0 on success, negative error code on failure.
     */
    virtual int reformat(BlockDevice *bd = NULL);

    /** Remove a file from the file system.
     *
     *  @param path     The name of the file to remove.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int remove(const char *path);

    /** Rename a file in the file system.
     *
     *  @param path     The existing name of the file to rename.
     *  @param newpath  The new name of the file.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int rename(const char *path, const char *newpath);

    /** Store information about the file in a stat structure.
     *
     *  @param path     The name of the file to find information about.
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
     *  @param path     The name of the file to find information about.
     *  @param buf      The stat buffer to write to.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int statvfs(const char *path, struct statvfs *buf);

protected:
    friend class File;
    friend class Dir;

#if !(DOXYGEN_ONLY)
    /** Open a file on the file system.
     *
     *  @param file     Destination of the newly created handle to the referenced file.
     *  @param path     The name of the file to open.
     *  @param flags    The flags that trigger opening of the file. These flags are O_RDONLY, O_WRONLY, and O_RDWR,
     *                  with an O_CREAT, O_TRUNC, or O_APPEND bitwise OR operator.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int file_open(fs_file_t *file, const char *path, int flags) = 0;

    /** Close a file.
     *
     *  @param file     File handle.
     *  return          0 on success, negative error code on failure.
     */
    virtual int file_close(fs_file_t file) = 0;

    /** Read the contents of a file into a buffer.
     *
     *  @param file     File handle.
     *  @param buffer   The buffer to read in to.
     *  @param size     The number of bytes to read.
     *  @return         The number of bytes read, 0 at the end of the file, negative error on failure.
     */
    virtual ssize_t file_read(fs_file_t file, void *buffer, size_t size) = 0;

    /** Write the contents of a buffer to a file.
     *
     *  @param file     File handle.
     *  @param buffer   The buffer to write from.
     *  @param size     The number of bytes to write.
     *  @return         The number of bytes written, negative error on failure.
     */
    virtual ssize_t file_write(fs_file_t file, const void *buffer, size_t size) = 0;

    /** Flush any buffers associated with the file.
     *
     *  @param file     File handle.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int file_sync(fs_file_t file);

    /** Check whether the file is an interactive terminal device.
     *  If so, line buffered behavior is used by default.
     *
     *  @param file     File handle.
     *  @return         True if the file is a terminal.
     */
    virtual int file_isatty(fs_file_t file);

    /** Move the file position to a given offset from a given location.
     *
     *  @param file     File handle.
     *  @param offset   The offset from whence to move to.
     *  @param whence   The start of where to seek.
     *      SEEK_SET to start from the beginning of the file,
     *      SEEK_CUR to start from the current position in the file,
     *      SEEK_END to start from the end of the file.
     *  @return         The new offset of the file
     */
    virtual off_t file_seek(fs_file_t file, off_t offset, int whence) = 0;

    /** Get the file position of the file.
     *
     *  @param file     File handle.
     *  @return         The current offset in the file.
     */
    virtual off_t file_tell(fs_file_t file);

    /** Rewind the file position to the beginning of the file.
     *
     *  @param file     File handle.
     *  @note This is equivalent to file_seek(file, 0, FS_SEEK_SET)
     */
    virtual void file_rewind(fs_file_t file);

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
    virtual int file_truncate(fs_file_t file, off_t length);

    /** Open a directory on the file system.
     *
     *  @param dir      Destination for the handle to the directory.
     *  @param path     Name of the directory to open.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int dir_open(fs_dir_t *dir, const char *path);

    /** Close a directory.
     *
     *  @param dir      Dir handle.
     *  @return         0 on success, negative error code on failure.
     */
    virtual int dir_close(fs_dir_t dir);

    /** Read the next directory entry.
     *
     *  @param dir      Dir handle.
     *  @param ent      The directory entry to fill out.
     *  @return         1 on reading a filename, 0 at the end of the directory, negative error on failure.
     */
    virtual ssize_t dir_read(fs_dir_t dir, struct dirent *ent);

    /** Set the current position of the directory.
     *
     *  @param dir      Dir handle.
     *  @param offset   Offset of the location to seek to,
     *                  must be a value returned from dir_tell.
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

    /** Get the size of the directory.
     *
     *  @param dir      Dir handle.
     *  @return         Number of files in the directory.
     */
    virtual size_t dir_size(fs_dir_t dir);
#endif //!defined(DOXYGEN_ONLY)

protected:
    // Hooks for file systemHandle
    virtual int open(FileHandle **file, const char *path, int flags);
    virtual int open(DirHandle **dir, const char *path);
};


/** @}*/
} // namespace mbed

#endif
