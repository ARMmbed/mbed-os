/*
 * Copyright (c) , Arm Limited and affiliates.
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

#ifndef __FILE_HANDLE_STUB_H__
#define __FILE_HANDLE_STUB_H__

#include "FileHandle.h"

namespace mbed {

static uint8_t filehandle_stub_short_value_counter = 0;
static char *filehandle_stub_table = NULL;
static uint8_t filehandle_stub_table_pos = 0;

class FileHandle_stub : public FileHandle {
public:
    ssize_t size_value;

    FileHandle_stub()
    {
        size_value = 0;
    }

    virtual ssize_t read(void *buffer, size_t size)
    {
        if (filehandle_stub_table) {
            ssize_t ret = strlen(filehandle_stub_table) - filehandle_stub_table_pos;
            if (size < ret) {
                ret = size;
            }
            memcpy(buffer, filehandle_stub_table + filehandle_stub_table_pos, ret);
            filehandle_stub_table_pos += ret;
            return ret;
        }
        return 0;
    }

    virtual ssize_t write(const void *buffer, size_t size)
    {
        if (size_value > 0) {
            size_value--;
            return size;
        } else if (size_value < 0) {
            return -1;
        }
        return 0;
    }

    virtual off_t seek(off_t offset, int whence = SEEK_SET)
    {
        return 0;
    }

    virtual int close()
    {
    }

    virtual short poll(short events) const
    {
        if (filehandle_stub_short_value_counter) {
            filehandle_stub_short_value_counter--;
            return short_value;
        }
        return 0;
    }

    virtual void sigio(Callback<void()> func)
    {
        if (func) {
            func();
        }
    }

    short short_value;
};

} // namespace mbed

#endif
