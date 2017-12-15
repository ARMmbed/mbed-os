/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#ifndef MBED_FILESTREAM_H
#define MBED_FILESTREAM_H

#include "platform/Stream.h"
#include "platform/SingletonPtr.h"
#include "platform/PlatformMutex.h"


#define POS_IS_VALID(pos) (pos >= 0 && pos < _end)
#define NEW_POS_IS_VALID(new_pos) (new_pos >= 0 && new_pos < FILE_SIZE)

template<uint32_t FILE_SIZE>
class FileStream : public Stream {
public:
    FileStream(const char *name = NULL) : Stream(name), _pos(-1), _end(0) {}

protected:
    int _getc();
    int _putc(int c);

    off_t seek(off_t offset, int whence);

    void lock();
    void unlock();

private:
    uint8_t _data[FILE_SIZE];
    int32_t _pos, _end;

    static SingletonPtr<PlatformMutex> _mutex;

};

template<uint32_t FILE_SIZE> SingletonPtr<PlatformMutex> FileStream<FILE_SIZE>::_mutex;

template<uint32_t FILE_SIZE>
int FileStream<FILE_SIZE>::_getc()
{
    if(POS_IS_VALID(_pos)) {
        return _data[_pos++];
    } else {
        return EOF;
    }
}

template<uint32_t FILE_SIZE>
int FileStream<FILE_SIZE>::_putc(int c)
{
    if(NEW_POS_IS_VALID(_pos + 1)) {
        _data[++_pos] = (uint8_t)(0XFF & c);
        if(_pos == _end) {
            _end++;
        }
        return c;
    }
    return EOF;
}

template<uint32_t FILE_SIZE>
off_t FileStream<FILE_SIZE>::seek(off_t offset, int whence)
{
    switch(whence)
    {
        case SEEK_SET:
            if(POS_IS_VALID(offset)) {
                _pos = offset;
            } else {
                return -1;
            }
            break;
        case SEEK_CUR:
            if(POS_IS_VALID(_pos + offset)) {
                _pos += offset;
            } else {
                return -1;
            }
            break;
        case SEEK_END:
            if(POS_IS_VALID(_end - offset)) {
                _pos = _end - offset;
            } else {
                return -1;
            }
            break;
        default:
            return -2;
    }
    return _pos;
}


template<uint32_t FILE_SIZE>
void FileStream<FILE_SIZE>::lock() {
    _mutex->lock();
}

template<uint32_t FILE_SIZE>
void FileStream<FILE_SIZE>::unlock() {
    _mutex->unlock();
}

#endif // MBED_FILESTREAM_H
