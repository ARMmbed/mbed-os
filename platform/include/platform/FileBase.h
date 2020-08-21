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
#ifndef MBED_FILEBASE_H
#define MBED_FILEBASE_H

typedef int FILEHANDLE;

#include "platform/platform.h"
#include "platform/SingletonPtr.h"
#include "platform/PlatformMutex.h"
#include "platform/NonCopyable.h"

namespace mbed {

typedef enum {
    FilePathType,
    FileSystemPathType
} PathType;

/** \defgroup platform-public-api-file File
 * \ingroup platform-public-api
 */

/**
 * \defgroup platform_FileBase FileBase class
 * \ingroup platform-public-api-file
 * @{
 */

/** Class FileBase
 *
 */

class FileBase : private NonCopyable<FileBase> {
public:
    FileBase(const char *name, PathType t);
    virtual ~FileBase();

    const char *getName(void);
    PathType    getPathType(void);

    static FileBase *lookup(const char *name, unsigned int len);

    static FileBase *get(int n);

    void set_as_default();

private:
    static FileBase *_head;
    static FileBase *_default;
    static SingletonPtr<PlatformMutex> _mutex;

    FileBase   *_next;
    const char *const _name;
    const PathType _path_type;
};

/**@}*/

} // namespace mbed

#endif
