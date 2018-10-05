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
#ifndef MBED_FILESYSTEMHANDLE_H
#define MBED_FILESYSTEMHANDLE_H

#include "platform/platform.h"

#include "platform/FileBase.h"
#include "platform/FileHandle.h"
#include "platform/DirHandle.h"
#include "platform/NonCopyable.h"

namespace mbed {
/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_FileSystemHandle FileSystemHandle functions
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
class FileSystemHandle : private NonCopyable<FileSystemHandle> {
public:
    /** FileSystemHandle lifetime
     */
    virtual ~FileSystemHandle() {}

    /** Open a file on the filesystem
     *
     *  @param file     Destination for the handle to a newly created file
     *  @param filename The name of the file to open
     *  @param flags    The flags to open the file in, one of O_RDONLY, O_WRONLY, O_RDWR,
     *                  bitwise or'd with one of O_CREAT, O_TRUNC, O_APPEND
     *  @return         0 on success, negative error code on failure
     */
    virtual int open(FileHandle **file, const char *filename, int flags) = 0;

    /** Open a directory on the filesystem
     *
     *  @param dir      Destination for the handle to the directory
     *  @param path     Name of the directory to open
     *  @return         0 on success, negative error code on failure
     */
    virtual int open(DirHandle **dir, const char *path);

    /** Remove a file from the filesystem.
     *
     *  @param path     The name of the file to remove.
     *  @return         0 on success, negative error code on failure
     */
    virtual int remove(const char *path);

    /** Rename a file in the filesystem.
     *
     *  @param path     The name of the file to rename.
     *  @param newpath  The name to rename it to
     *  @return         0 on success, negative error code on failure
     */
    virtual int rename(const char *path, const char *newpath);

    /** Store information about the file in a stat structure
     *
     *  @param path     The name of the file to find information about
     *  @param st       The stat buffer to write to
     *  @return         0 on success, negative error code on failure
     */
    virtual int stat(const char *path, struct stat *st);

    /** Create a directory in the filesystem.
     *
     *  @param path     The name of the directory to create.
     *  @param mode     The permissions with which to create the directory
     *  @return         0 on success, negative error code on failure
     */
    virtual int mkdir(const char *path, mode_t mode);

    /** Store information about the mounted filesystem in a statvfs structure
     *
     *  @param path     The name of the file to find information about
     *  @param buf      The stat buffer to write to
     *  @return         0 on success, negative error code on failure
     */
    virtual int statvfs(const char *path, struct statvfs *buf);
};
/**@}*/

/**@}*/

} // namespace mbed

#endif

/** @}*/
