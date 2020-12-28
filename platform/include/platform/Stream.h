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
#ifndef MBED_STREAM_H
#define MBED_STREAM_H

#include "platform/platform.h"
#include "platform/FileLike.h"
#include "platform/NonCopyable.h"
#include "platform/mbed_toolchain.h"
#include <cstdio>
#include <cstdarg>

namespace mbed {
/** \addtogroup platform-public-api */
/** @{*/

/**
 * \defgroup platform_Stream Stream class
 * @{
 */

extern void mbed_set_unbuffered_stream(std::FILE *_file);

/** File stream
 *
 * @note Synchronization level: Set by subclass
 */
class Stream : public FileLike, private NonCopyable<Stream> {

public:
    Stream(const char *name = NULL);
    virtual ~Stream();

#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    int putc(int c);
    int puts(const char *s);
    int getc();
    char *gets(char *s, int size);
    int printf(const char *format, ...) MBED_PRINTF_METHOD(1, 2);
    int scanf(const char *format, ...) MBED_SCANF_METHOD(1, 2);
    int vprintf(const char *format, std::va_list args) MBED_PRINTF_METHOD(1, 0);
    int vscanf(const char *format, std::va_list args) MBED_SCANF_METHOD(1, 0);

    operator std::FILE *()
    {
        return _file;
    }

#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    virtual int close();
    virtual ssize_t write(const void *buffer, size_t length);
    virtual ssize_t read(void *buffer, size_t length);
    virtual off_t seek(off_t offset, int whence);
    virtual off_t tell();
    virtual void rewind();
    virtual int isatty();
    virtual int sync();
    virtual off_t size();

protected:
    virtual int _putc(int c) = 0;
    virtual int _getc() = 0;

#if !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY
    std::FILE *_file;
#endif // !MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY

    /** Acquire exclusive access to this object.
     */
    virtual void lock()
    {
        // Stub
    }

    /** Release exclusive access to this object.
     */
    virtual void unlock()
    {
        // Stub
    }
};
/**@}*/

/**@}*/
} // namespace mbed

#endif

