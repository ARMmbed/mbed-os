/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#ifndef MBED_STORAGELITEFS_H
#define MBED_STORAGELITEFS_H

#include "StorageLite.h"

#if (STORAGELITE_ENABLED) || defined(DOXYGEN_ONLY)

#include "FileSystem.h"
#include "BlockDevice.h"

/**
 * StorageLiteFS, FileSystem interface for the StorageLite feature
 */
class StorageLiteFS : public mbed::FileSystem {
public:

    /**
     * @brief StorageLiteFS constructor.
     *
     * @param[in]  name                     Name to add filesystem to tree as.
     * @param[in]  stlite                   StorageLite instance.
     * @param[in]  flags                    StorageLite flags given when creating files.
     *
     * @return                              none.
     */
    StorageLiteFS(const char *name = 0, StorageLite *stlite = 0,
                  uint32_t flags = StorageLite::encrypt_flag);

    virtual ~StorageLiteFS();

    /**
     * @brief Mounts a filesystem to a block device
     *
     * @param[in]  bd                       Underlying block device.
     *
     * @return                              0 for success, nonzero for failure.
     */
    virtual int mount(BlockDevice *bd);

    /**
     * @brief Unmounts a filesystem from the underlying block device
     *
     * @return                              0 for success, nonzero for failure.
     */
    virtual int unmount();

    /**
     * @brief Reformats a filesystem, results in an empty and mounted filesystem
     *
     * @param[in]  bd                       Underlying block device.
     *      BlockDevice to reformat and mount. If NULL, the mounted
     *      block device will be used.
     *      Note: if mount fails, bd must be provided.
     *      Default: NULL
     *
     * @return                              0 for success, nonzero for failure.
     */
    virtual int reformat(BlockDevice *bd);

    /**
     * @brief Remove a file from the filesystem.
     *
     * @param[in]  path     The name of the file to remove.
     *
     * @return                              0 for success, nonzero for failure.
     */
    virtual int remove(const char *path);

    /**
     * @brief Store information about the file in a stat structure
     *
     * @param[in]  path                     The name of the file to find information about
     * @param[in]  st                       The stat buffer to write to
     *
     * @return                              0 for success, nonzero for failure.
     */
    virtual int stat(const char *path, struct stat *st);

    /**
     * @brief Store information about the mounted filesystem in a statvfs structure
     *
     * @param[in]  path     The name of the file to find information about
     * @param[in]  buf      The stat buffer to write to
     *
     * @return                              0 for success, nonzero for failure.
     */
    virtual int statvfs(const char *path, struct statvfs *buf);

protected:
    /**
     * @brief Open a file on the filesystem
     *
     * @param[in]  file     Destination for the handle to a newly created file
     * @param[in]  path     The name of the file to open
     * @param[in]  flags    The flags to open the file in, one of O_RDONLY, O_WRONLY, O_RDWR,
     *                  bitwise or'd with one of O_CREAT, O_TRUNC, O_APPEND
     *
     * @return                              0 for success, nonzero for failure.
     */
    virtual int file_open(mbed::fs_file_t *file, const char *path, int flags);

    /**
     * @brief Close a file
     *
     * @param[in]  file     File handle
     *
     * @return                              0 for success, nonzero for failure.
     */
    virtual int file_close(mbed::fs_file_t file);

    /**
     * @brief Read the contents of a file into a buffer
     *
     * @param[in]  file     File handle
     * @param[in]  buffer   The buffer to read in to
     * @param[in]  size     The number of bytes to read
     *
     * @return                              The number of bytes read, negative error on failure
     */
    virtual ssize_t file_read(mbed::fs_file_t file, void *buffer, size_t size);

    /**
     * @brief Write the contents of a buffer to a file
     *
     * @param[in]  file     File handle
     * @param[in]  buffer   The buffer to write from
     * @param[in]  size     The number of bytes to write
     *
     * @return                              The number of bytes written, negative error on failure
     */
    virtual ssize_t file_write(mbed::fs_file_t file, const void *buffer, size_t size);

    /**
     * @brief Move the file position to a given offset from from a given location
     *
     * @param[in]  file     File handle
     * @param[in]  offset   The offset from whence to move to
     * @param[in]  whence   The start of where to seek
     *      SEEK_SET to start from beginning of file,
     *      SEEK_CUR to start from current position in file,
     *      SEEK_END to start from end of file
     *
     * @return                              Always 0 (unsupported)
     */
    virtual off_t file_seek(mbed::fs_file_t file, off_t offset, int whence);

    /**
     * @brief Get the file position of the file
     *
     * @param[in]  file     File handle
     *
     * @return                              Always 0 (unsupported)
     */
    virtual off_t file_tell(mbed::fs_file_t file);

    /**
     * @brief Get the size of the file
     *
     * @param[in]  file     File handle
     *
     * @return                              Always 0 (unsupported)
     */
    virtual off_t file_size(mbed::fs_file_t file);

private:
    StorageLite *_stlite;
    uint32_t _flags;
    BlockDevice *_bd;
};

/** @}*/

#endif // STORAGELITE_ENABLED

#endif
