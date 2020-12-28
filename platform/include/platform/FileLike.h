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
#ifndef MBED_FILELIKE_H
#define MBED_FILELIKE_H

#include "platform/FileBase.h"
#include "platform/FileHandle.h"
#include "platform/NonCopyable.h"

namespace mbed {
/**
 * \defgroup platform_FileLike FileLike class
 * \ingroup platform-public-api-file
 * @{
 */

/** Class FileLike
 *
 *  A file-like object is one that can be opened with fopen by
 *  fopen("/name", mode).
 *
 *  @note Synchronization level: Set by subclass
 */
class FileLike : public FileHandle, public FileBase, private NonCopyable<FileLike> {
public:
    /** Constructor FileLike
     *
     *  @param name     The name to use to open the file.
     */
    FileLike(const char *name = NULL) : FileBase(name, FilePathType) {}
    virtual ~FileLike() {}
};

/**@}*/

} // namespace mbed

#endif
