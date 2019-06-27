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
#include "platform/Stream.h"
#include "platform/mbed_error.h"
#include <errno.h>

namespace mbed {

Stream::Stream(const char *name) : FileLike(name), _file(NULL)
{
    // No lock needed in constructor
    /* open ourselves */
    _file = fdopen(this, "w+");
    // fdopen() will make us buffered because Stream::isatty()
    // wrongly returns zero which is not being changed for
    // backward compatibility
    if (_file) {
        mbed_set_unbuffered_stream(_file);
    } else {
        MBED_ERROR1(MBED_MAKE_ERROR(MBED_MODULE_PLATFORM, MBED_ERROR_CODE_OPEN_FAILED), "Stream obj failure", _file);
    }
}

Stream::~Stream()
{
    // No lock can be used in destructor
    fclose(_file);
}

int Stream::putc(int c)
{
    lock();
    std::fseek(_file, 0, SEEK_CUR);
    int ret = std::fputc(c, _file);
    unlock();
    return ret;
}
int Stream::puts(const char *s)
{
    lock();
    std::fseek(_file, 0, SEEK_CUR);
    int ret = std::fputs(s, _file);
    unlock();
    return ret;
}
int Stream::getc()
{
    lock();
    fflush(_file);
    int ret = std::fgetc(_file);
    unlock();
    return ret;
}
char *Stream::gets(char *s, int size)
{
    lock();
    fflush(_file);
    char *ret = std::fgets(s, size, _file);
    unlock();
    return ret;
}

int Stream::close()
{
    return 0;
}

ssize_t Stream::write(const void *buffer, size_t length)
{
    const char *ptr = (const char *)buffer;
    const char *end = ptr + length;

    lock();
    while (ptr != end) {
        if (_putc(*ptr++) == EOF) {
            break;
        }
    }
    unlock();

    return ptr - (const char *)buffer;
}

ssize_t Stream::read(void *buffer, size_t length)
{
    char *ptr = (char *)buffer;
    char *end = ptr + length;

    lock();
    while (ptr != end) {
        int c = _getc();
        if (c == EOF) {
            break;
        }
        *ptr++ = c;
    }
    unlock();

    return ptr - (const char *)buffer;
}

off_t Stream::seek(off_t offset, int whence)
{
    return 0;
}

off_t Stream::tell()
{
    return 0;
}

void Stream::rewind()
{
}

int Stream::isatty()
{
    return 0;
}

int Stream::sync()
{
    return 0;
}

off_t Stream::size()
{
    return 0;
}

int Stream::printf(const char *format, ...)
{
    lock();
    std::va_list arg;
    va_start(arg, format);
    std::fseek(_file, 0, SEEK_CUR);
    int r = vfprintf(_file, format, arg);
    va_end(arg);
    unlock();
    return r;
}

int Stream::scanf(const char *format, ...)
{
    lock();
    std::va_list arg;
    va_start(arg, format);
    fflush(_file);
    int r = vfscanf(_file, format, arg);
    va_end(arg);
    unlock();
    return r;
}

int Stream::vprintf(const char *format, std::va_list args)
{
    lock();
    std::fseek(_file, 0, SEEK_CUR);
    int r = vfprintf(_file, format, args);
    unlock();
    return r;
}

int Stream::vscanf(const char *format, std::va_list args)
{
    lock();
    fflush(_file);
    int r = vfscanf(_file, format, args);
    unlock();
    return r;
}

} // namespace mbed
