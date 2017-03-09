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
#ifndef MBED_FILESYSTEMLIKE_H
#define MBED_FILESYSTEMLIKE_H

#include "platform/platform.h"

#include "drivers/FileBase.h"
#include "drivers/FileHandle.h"
#include "drivers/DirHandle.h"

namespace mbed {
/** \addtogroup drivers */
/** @{*/

/** A filesystem-like object is one that can be used to open files
 *  though it by fopen("/name/filename", mode)
 *
 *  Implementations must define at least open (the default definitions
 *  of the rest of the functions just return error values).
 *
 * @Note Synchronization level: Set by subclass
 */
class FileSystemLike : public FileBase {

public:
    /** FileSystemLike constructor
     *
     *  @param name The name to use for the filesystem.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "The mbed 2 filesystem classes have been superseeded by the FileSystem api, "
        "Replaced by FileSystem")
    FileSystemLike(const char *name);

    virtual ~FileSystemLike();

    MBED_DEPRECATED_SINCE("mbed-os-5.4",
        "The mbed 2 filesystem classes have been superseeded by the FileSystem api, "
        "Replaced by FileSystem")
    static DirHandle *opendir();
    friend class BaseDirHandle;

    /** Opens a file from the filesystem
     *
     *  @param filename The name of the file to open.
     *  @param flags One of O_RDONLY, O_WRONLY, or O_RDWR, OR'd with
     *    zero or more of O_CREAT, O_TRUNC, or O_APPEND.
     *
     *  @returns
     *    A pointer to a FileHandle object representing the
     *   file on success, or NULL on failure.
     */
    virtual FileHandle *open(const char *filename, int flags) = 0;

    /** Remove a file from the filesystem.
     *
     *  @param filename the name of the file to remove.
     *  @param returns 0 on success, -1 on failure.
     */
    virtual int remove(const char *filename) { (void) filename; return -1; };

    /** Rename a file in the filesystem.
     *
     *  @param oldname the name of the file to rename.
     *  @param newname the name to rename it to.
     *
     *  @returns
     *    0 on success,
     *   -1 on failure.
     */
    virtual int rename(const char *oldname, const char *newname) { (void) oldname, (void) newname; return -1; };

    /** Opens a directory in the filesystem and returns a DirHandle
     *   representing the directory stream.
     *
     *  @param name The name of the directory to open.
     *
     *  @returns
     *    A DirHandle representing the directory stream, or
     *   NULL on failure.
     */
    virtual DirHandle *opendir(const char *name) { (void) name; return NULL; };

    /** Creates a directory in the filesystem.
     *
     *  @param name The name of the directory to create.
     *  @param mode The permissions to create the directory with.
     *
     *  @returns
     *    0 on success,
     *   -1 on failure.
     */
    virtual int mkdir(const char *name, mode_t mode) { (void) name, (void) mode; return -1; }

    /** Store information about file in stat structure
     *
     *  @param name The name of the file to find information about
     *  @param st The stat buffer to write to
     *  @returns
     *    0 on success or un-needed,
     *   -1 on error
     */
    virtual int stat(const char *name, struct stat *st) { return -1; };
};

} // namespace mbed

#endif

/** @}*/
