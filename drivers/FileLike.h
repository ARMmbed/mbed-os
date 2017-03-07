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
#ifndef MBED_FILELIKE_H
#define MBED_FILELIKE_H

#include "platform/mbed_toolchain.h"
#include "drivers/FileBase.h"

namespace mbed {
/** \addtogroup drivers */
/** @{*/


/* Class FileLike
 *  A file-like object is one that can be opened with fopen by
 *  fopen("/name", mode).
 *
 *  @Note Synchronization level: Set by subclass
 */
class FileLike : public FileBase {
public:
    /** Constructor FileLike
     *
     *  @param name     The name to use to open the file.
     */
    FileLike(const char *name = NULL) : FileBase(name, FilePathType) {}
    virtual ~FileLike() {}

    /** Read the contents of a file into a buffer
     *
     *  @param buffer   The buffer to read in to
     *  @param size     The number of bytes to read
     *  @return         The number of bytes read, 0 at end of file, negative error on failure
     */
    virtual ssize_t read(void *buffer, size_t len) = 0;

    /** Write the contents of a buffer to a file
     *
     *  @param buffer   The buffer to write from
     *  @param size     The number of bytes to write 
     *  @return         The number of bytes written, negative error on failure
     */
    virtual ssize_t write(const void *buffer, size_t len) = 0;

    /** Close a file
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual int close() = 0;

    /** Flush any buffers associated with the file
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual int sync() = 0;

    /** Check if the file in an interactive terminal device
     *
     *  @return         True if the file is a terminal
     */
    virtual int isatty() = 0;

    /** Move the file position to a given offset from from a given location
     *
     *  @param offset   The offset from whence to move to
     *  @param whence   The start of where to seek
     *      SEEK_SET to start from beginning of file,
     *      SEEK_CUR to start from current position in file,
     *      SEEK_END to start from end of file
     *  @return         The new offset of the file
     */
    virtual off_t seek(off_t offset, int whence = SEEK_SET) = 0;

    /** Get the file position of the file
     *
     *  @return         The current offset in the file
     */
    virtual off_t tell() = 0;

    /** Rewind the file position to the beginning of the file
     *
     *  @note This is equivalent to file_seek(file, 0, FS_SEEK_SET)
     */
    virtual void rewind() = 0;

    /** Get the size of the file
     *
     *  @return         Size of the file in bytes
     */
    virtual size_t size() = 0;

    /** Move the file position to a given offset from a given location.
     *
     *  @param offset The offset from whence to move to
     *  @param whence SEEK_SET for the start of the file, SEEK_CUR for the
     *   current file position, or SEEK_END for the end of the file.
     *
     *  @returns
     *    new file position on success,
     *    -1 on failure or unsupported
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.4", "Replaced by FileLike::seek")
    virtual off_t lseek(off_t offset, int whence) { return seek(offset, whence); }

    /** Flush any buffers associated with the FileHandle, ensuring it
     *  is up to date on disk
     *
     *  @returns
     *    0 on success or un-needed,
     *   -1 on error
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.4", "Replaced by FileLike::sync")
    virtual int fsync() { return sync(); }

    /** Find the length of the file
     *
     *  @returns
     *   Length of the file
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.4", "Replaced by FileLike::size")
    virtual off_t flen() { return size(); }

protected:
    /** Acquire exclusive access to this object.
     */
    virtual void lock() {
        // Stub
    }

    /** Release exclusive access to this object.
     */
    virtual void unlock() {
        // Stub
    }
};


/** @}*/
} // namespace mbed

#endif
