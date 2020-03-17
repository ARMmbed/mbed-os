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
#ifndef MBED_FILESYSTEMLIKE_H
#define MBED_FILESYSTEMLIKE_H

#include "platform/platform.h"

#include "platform/FileBase.h"
#include "platform/FileSystemHandle.h"
#include "platform/FileHandle.h"
#include "platform/DirHandle.h"
#include "platform/NonCopyable.h"

namespace mbed {
/**
 * \defgroup platform_FileSystemLike FileSystemLike functions
 * \ingroup platform-public-api-file
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

};

/**@}*/

} // namespace mbed

#endif
