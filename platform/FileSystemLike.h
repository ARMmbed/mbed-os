/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_FILESYSTEMLIKE_H
#define MBED_FILESYSTEMLIKE_H

#include "platform/platform.h"

#include "platform/FileSystemHandle.h"
#include "platform/FileHandle.h"
#include "platform/DirHandle.h"
#include "platform/NonCopyable.h"

namespace mbed {
/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_FileSystemLike FileSystemLike functions
 * @{
 */


/** A filesystem-like object is one that can be used to open file-like
 *  objects though it by fopen("/name/filename", mode)
 *
 *  Implementations must define at least open (the default definitions
 *  of the rest of the functions just return error values).
 *
 * @note Synchronization level: Set by subclass
 */
class FileSystemLike : public FileSystemHandle, public FileBase, private NonCopyable<FileSystemLike> {
public:
    /** FileSystemLike lifetime
     */
    FileSystemLike(const char *name = NULL) : FileBase(name, FileSystemPathType) {}
    virtual ~FileSystemLike() {}

    // Inherited functions with name conflicts
    using FileSystemHandle::open;

    /** Open a file on the filesystem
     *
     *  @param path     The name of the file to open
     *  @param flags    The flags to open the file in, one of O_RDONLY, O_WRONLY, O_RDWR,
     *                  bitwise or'd with one of O_CREAT, O_TRUNC, O_APPEND
     *  @return         A file handle on success, NULL on failure
     *  @deprecated Replaced by `int open(FileHandle **, ...)` for propagating error codes
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.5",
                          "Replaced by `int open(FileHandle **, ...)` for propagating error codes")
    FileHandle *open(const char *path, int flags)
    {
        FileHandle *file;
        int err = open(&file, path, flags);
        return err ? NULL : file;
    }

    /** Open a directory on the filesystem
     *
     *  @param path     Name of the directory to open
     *  @return         A directory handle on success, NULL on failure
     *  @deprecated Replaced by `int open(DirHandle **, ...)` for propagating error codes
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.5",
                          "Replaced by `int open(DirHandle **, ...)` for propagating error codes")
    DirHandle *opendir(const char *path)
    {
        DirHandle *dir;
        int err = open(&dir, path);
        return err ? NULL : dir;
    }
};

/**@}*/

/**@}*/

} // namespace mbed

#endif
