/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#ifndef MBED_TESTFILEHANDLE_H
#define MBED_TESTFILEHANDLE_H

#include "platform/FileHandle.h"

#define POS_IS_VALID(pos) (pos >= 0 && pos < _end)
#define NEW_POS_IS_VALID(pos) (pos >= 0 && pos < (int32_t)FILE_SIZE)
#define SEEK_POS_IS_VALID(pos) (pos >= 0 && pos <= _end)
#define INVALID_POS (-1)

template<uint32_t FILE_SIZE>
class TestFile : public mbed::FileHandle {
public:
    TestFile(): _pos(0), _end(0) {}
    ~TestFile() {}

    enum FunctionName {
        fnNone, fnRead, fnWrite, fnSeek, fnClose, fnIsatty, fnTruncate
    };

    virtual ssize_t read(void *buffer, size_t size)
    {
        ssize_t read;
        _fnCalled = fnRead;

        for (read = 0; (size_t)read < size; read++) {
            if (POS_IS_VALID(_pos)) {
                ((uint8_t *)buffer)[read] = _data[_pos++];
            } else {
                break;
            }
        } // for
        return read;
    }

    virtual ssize_t write(const void *buffer, size_t size)
    {
        ssize_t written;
        _fnCalled = fnWrite;

        for (written = 0; (size_t)written < size; written++) {
            if (NEW_POS_IS_VALID(_pos)) {
                _data[_pos++] = ((uint8_t *)buffer)[written];
            } else {
                if (0 == written) {
                    return -ENOSPC;
                }
                break;
            }
            if (_end < _pos) {
                _end++;
            }
        } // for
        return written;
    }

    virtual off_t seek(off_t offset, int whence)
    {
        _fnCalled = fnSeek;
        int32_t new_pos = INVALID_POS;

        switch (whence) {
            case SEEK_SET:
                new_pos = offset;
                break;

            case SEEK_CUR:
                new_pos = _pos + offset;
                break;

            case SEEK_END:
                new_pos = _end - offset;
                break;

            default:
                // nothing todo
                break;
        }

        if (SEEK_POS_IS_VALID(new_pos)) {
            _pos = new_pos;
        } else {
            return -EINVAL;
        }

        return _pos;
    }

    virtual int close()
    {
        _fnCalled = fnClose;
        return 0;
    }

    virtual off_t size()
    {
        return _end;
    }

    virtual int truncate(off_t length)
    {
        _fnCalled = fnTruncate;
        if (!NEW_POS_IS_VALID(length)) {
            return -EINVAL;
        }
        while (_end < length) {
            _data[_end++] = 0;
        }
        _end = length;
        return 0;
    }


    static void resetFunctionCallHistory()
    {
        _fnCalled = fnNone;
    }

    static bool functionCalled(FunctionName name)
    {
        return (name == _fnCalled);
    }

    static FunctionName getFunctionCalled()
    {
        return _fnCalled;
    }

private:

    // stores last function call name
    static FunctionName _fnCalled;

    // file storage
    uint8_t _data[FILE_SIZE];

    int32_t _pos, _end;
};

template<uint32_t FILE_SIZE>
typename TestFile<FILE_SIZE>::FunctionName TestFile<FILE_SIZE>::_fnCalled;


#endif // MBED_TESTFILEHANDLE_H
