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

#include "File.h"
#include <errno.h>

namespace mbed {

File::File()
    : _fs(0), _file(0)
{
}

File::File(FileSystem *fs, const char *path, int flags)
    : _fs(0), _file(0)
{
    open(fs, path, flags);
}

File::~File()
{
    if (_fs) {
        close();
    }
}

int File::open(FileSystem *fs, const char *path, int flags)
{
    if (_fs) {
        return -EINVAL;
    }

    int err = fs->file_open(&_file, path, flags);
    if (!err) {
        _fs = fs;
    }

    return err;
}

int File::close()
{
    if (!_fs) {
        return -EINVAL;
    }

    int err = _fs->file_close(_file);
    _fs = 0;
    return err;
}

ssize_t File::read(void *buffer, size_t len)
{
    MBED_ASSERT(_fs);
    return _fs->file_read(_file, buffer, len);
}

ssize_t File::write(const void *buffer, size_t len)
{
    MBED_ASSERT(_fs);
    return _fs->file_write(_file, buffer, len);
}

int File::sync()
{
    MBED_ASSERT(_fs);
    return _fs->file_sync(_file);
}

int File::isatty()
{
    MBED_ASSERT(_fs);
    return _fs->file_isatty(_file);
}

off_t File::seek(off_t offset, int whence)
{
    MBED_ASSERT(_fs);
    return _fs->file_seek(_file, offset, whence);
}

off_t File::tell()
{
    MBED_ASSERT(_fs);
    return _fs->file_tell(_file);
}

void File::rewind()
{
    MBED_ASSERT(_fs);
    return _fs->file_rewind(_file);
}

off_t File::size()
{
    MBED_ASSERT(_fs);
    return _fs->file_size(_file);
}

int File::truncate(off_t length)
{
    MBED_ASSERT(_fs);
    return _fs->file_truncate(_file, length);
}

} // namespace mbed
