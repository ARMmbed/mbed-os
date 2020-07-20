/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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

#ifndef FILE_H
#define FILE_H

#include "filesystem/FileSystem.h"
#include "platform/FileHandle.h"

namespace mbed {
/** \addtogroup filesystem */
/** @{*/


/** File class
 */
class File : public FileHandle {
public:
    /** Create an uninitialized file
     *
     *  Must call open to initialize the file on a file system
     */
    File();

    /** Create a file on a filesystem
     *
     *  Creates and opens a file on a filesystem
     *
     *  @param fs       Filesystem as target for the file
     *  @param path     The name of the file to open
     *  @param flags    The flags to open the file in, one of O_RDONLY, O_WRONLY, O_RDWR,
     *                  bitwise or'd with one of O_CREAT, O_TRUNC, O_APPEND
     */
    File(FileSystem *fs, const char *path, int flags = O_RDONLY);

    /** Destroy a file
     *
     *  Closes file if the file is still open
     */
    virtual ~File();

    /** Open a file on the filesystem
     *
     *  @param fs       Filesystem as target for the file
     *  @param path     The name of the file to open
     *  @param flags    The flags to open the file in, one of O_RDONLY, O_WRONLY, O_RDWR,
     *                  bitwise or'd with one of O_CREAT, O_TRUNC, O_APPEND
     *  @return         0 on success, negative error code on failure
     */
    virtual int open(FileSystem *fs, const char *path, int flags = O_RDONLY);

    /** Close a file
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual int close();

    /** Read the contents of a file into a buffer
     *
     *  @param buffer   The buffer to read in to
     *  @param size     The number of bytes to read
     *  @return         The number of bytes read, 0 at end of file, negative error on failure
     */

    virtual ssize_t read(void *buffer, size_t size);

    /** Write the contents of a buffer to a file
     *
     *  @param buffer   The buffer to write from
     *  @param size     The number of bytes to write
     *  @return         The number of bytes written, negative error on failure
     */
    virtual ssize_t write(const void *buffer, size_t size);

    /** Flush any buffers associated with the file
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual int sync();

    /** Check if the file in an interactive terminal device
     *
     *  @return         True if the file is a terminal
     */
    virtual int isatty();

    /** Move the file position to a given offset from from a given location
     *
     *  @param offset   The offset from whence to move to
     *  @param whence   The start of where to seek
     *      SEEK_SET to start from beginning of file,
     *      SEEK_CUR to start from current position in file,
     *      SEEK_END to start from end of file
     *  @return         The new offset of the file
     */
    virtual off_t seek(off_t offset, int whence = SEEK_SET);

    /** Get the file position of the file
     *
     *  @return         The current offset in the file
     */
    virtual off_t tell();

    /** Rewind the file position to the beginning of the file
     *
     *  @note This is equivalent to file_seek(file, 0, FS_SEEK_SET)
     */
    virtual void rewind();

    /** Get the size of the file
     *
     *  @return         Size of the file in bytes
     */
    virtual off_t size();

    /** Truncate or extend a file.
     *
     * The file's length is set to the specified value. The seek pointer is
     * not changed. If the file is extended, the extended area appears as if
     * it were zero-filled.
     *
     *  @param length   The requested new length for the file
     *
     *  @return         Zero on success, negative error code on failure
     */
    virtual int truncate(off_t length);

private:
    FileSystem *_fs;
    fs_file_t _file;
};


/** @}*/
} // namespace mbed

#endif
