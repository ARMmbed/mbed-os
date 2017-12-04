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

#ifndef DIR_H
#define DIR_H

#include "filesystem/FileSystem.h"
#include "platform/DirHandle.h"

namespace mbed {
/** \addtogroup filesystem */
/** @{*/


/** Dir class
 */
class Dir : public DirHandle {
public:
    /** Create an uninitialized directory
     *
     *  Must call open to initialize the directory on a file system
     */
    Dir();

    /** Open a directory on a filesystem 
     *
     *  @param fs       Filesystem as target for a directory
     *  @param path     Name of the directory to open
     */
    Dir(FileSystem *fs, const char *path);

    /** Destroy a file
     *
     *  Closes file if the file is still open
     */
    virtual ~Dir();

    /** Open a directory on the filesystem
     *
     *  @param fs       Filesystem as target for a directory
     *  @param path     Name of the directory to open
     *  @return         0 on success, negative error code on failure
     */
    virtual int open(FileSystem *fs, const char *path);

    /** Close a directory
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual int close();

    /** Read the next directory entry
     *
     *  @param ent      The directory entry to fill out
     *  @return         1 on reading a filename, 0 at end of directory, negative error on failure
     */
    virtual ssize_t read(struct dirent *ent);

    /** Set the current position of the directory
     *
     *  @param offset   Offset of the location to seek to,
     *                  must be a value returned from tell
     */
    virtual void seek(off_t offset);

    /** Get the current position of the directory
     *
     *  @return         Position of the directory that can be passed to rewind
     */
    virtual off_t tell();

    /** Rewind the current position to the beginning of the directory
     */
    virtual void rewind();

    /** Get the sizeof the directory 
     *
     *  @return         Number of files in the directory
     */
    virtual size_t size();

private:
    FileSystem *_fs;
    fs_dir_t _dir;
};


/** @}*/
} // namespace mbed

#endif
