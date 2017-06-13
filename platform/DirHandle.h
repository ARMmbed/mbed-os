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
#ifndef MBED_DIRHANDLE_H
#define MBED_DIRHANDLE_H

#include <stdint.h>
#include "platform/platform.h"
#include "platform/FileHandle.h"

namespace mbed {
/** \addtogroup platform */


/** Represents a directory stream. Objects of this type are returned
 *  by an opendir function. The core functions are read and seek,
 *  but only a subset needs to be provided.
 *
 *  If a FileSystemLike class defines the opendir method, then the
 *  directories of an object of that type can be accessed by
 *  DIR *d = opendir("/example/directory") (or opendir("/example")
 *  to open the root of the filesystem), and then using readdir(d) etc.
 *
 *  The root directory is considered to contain all FileHandle and
 *  FileSystem objects, so the DIR* returned by opendir("/") will
 *  reflect this.
 *
 *  @note to create a directory, @see Dir
 *  @note Synchronization level: Set by subclass
 *  @ingroup platform
 */
class DirHandle {
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

    /** Closes the directory.
     *
     *  @returns
     *    0 on success,
     *   -1 on error.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.4", "Replaced by DirHandle::close")
    virtual int closedir() { return close(); };

    /** Return the directory entry at the current position, and
     *  advances the position to the next entry.
     *
     * @returns
     *  A pointer to a dirent structure representing the
     *  directory entry at the current position, or NULL on reaching
     *  end of directory or error.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.4", "Replaced by DirHandle::read")
    virtual struct dirent *readdir()
    {
        static struct dirent ent;
        return (read(&ent) > 0) ? &ent : NULL;
    }

    /** Resets the position to the beginning of the directory.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.4", "Replaced by DirHandle::rewind")
    virtual void rewinddir() { rewind(); }

    /** Returns the current position of the DirHandle.
     *
     * @returns
     *   the current position,
     *  -1 on error.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.4", "Replaced by DirHandle::tell")
    virtual off_t telldir() { return tell(); }

    /** Sets the position of the DirHandle.
     *
     *  @param location The location to seek to. Must be a value returned by telldir.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.4", "Replaced by DirHandle::seek")
    virtual void seekdir(off_t location) { seek(location); }
};


} // namespace mbed

#endif /* MBED_DIRHANDLE_H */
