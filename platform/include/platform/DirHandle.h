/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef MBED_DIRHANDLE_H
#define MBED_DIRHANDLE_H

#include <stdint.h>
#include "platform/mbed_toolchain.h"
#include "platform/NonCopyable.h"

namespace mbed {
/** \addtogroup platform-public-api */
/** @{*/
/**
 * \defgroup platform_DirHandle DirHandle functions
 * @{
 */


/** Represents a directory stream. An opendir function returns
 *  objects of this type. The core functions are read and seek,
 *  but only a subset needs to be provided.
 *
 *  If a FileSystemLike class defines the opendir method, then you
 *  can access the directories of an object of that type by either:
 *  @code
 *  DIR *d  = opendir("/example/directory");
 *  @endcode
 *  or
 *  @code
 *  DIR *d = opendir("/example");
 *  @endcode
 *  to open the root of the file system.
 *
 *  The root directory is considered to contain all FileHandle and
 *  FileSystem objects, so the DIR pointer returned by opendir("/")
 *  reflects this.
 *
 *  @note to create a directory, @see Dir
 *  @note Synchronization level: Set by subclass
 */
class DirHandle : private NonCopyable<DirHandle> {
public:
    virtual ~DirHandle() {}

    /** Read the next directory entry
     *
     *  @param ent      The directory entry to fill out
     *  @return         1 on reading a filename, 0 at end of directory, negative error on failure
     */
    virtual ssize_t read(struct dirent *ent) = 0;

    /** Close a directory
     *
     *  @return          0 on success, negative error code on failure
     */
    virtual int close() = 0;

    /** Set the current position of the directory
     *
     *  @param offset   Offset of the location to seek to,
     *                  must be a value returned from tell
     */
    virtual void seek(off_t offset) = 0;

    /** Get the current position of the directory
     *
     *  @return         Position of the directory that can be passed to rewind
     */
    virtual off_t tell() = 0;

    /** Rewind the current position to the beginning of the directory
     */
    virtual void rewind() = 0;

    /** Get the sizeof the directory
     *
     *  @return         Number of files in the directory
     */
    virtual size_t size()
    {
        off_t off = tell();
        size_t size = 0;
        struct dirent *ent = new struct dirent;

        rewind();
        while (read(ent) > 0) {
            size += 1;
        }
        seek(off);

        delete ent;
        return size;
    }
};

/**@}*/

/**@}*/
} // namespace mbed

#endif /* MBED_DIRHANDLE_H */
