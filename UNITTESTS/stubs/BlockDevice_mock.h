/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#ifndef BLOCKDEVICEMOCK_H
#define BLOCKDEVICEMOCK_H

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "features/storage/blockdevice/BlockDevice.h"

class BlockDeviceMock : public BlockDevice {
public:
    MOCK_METHOD0(init, int());
    MOCK_METHOD0(deinit, int());
    MOCK_METHOD0(sync, int());
    MOCK_METHOD3(read, int(void *, bd_addr_t, bd_size_t));
    MOCK_METHOD3(program, int(const void *, bd_addr_t, bd_size_t));
    MOCK_METHOD2(erase, int(bd_addr_t, bd_size_t));
    MOCK_METHOD2(trim, int(bd_addr_t, bd_size_t));
    MOCK_CONST_METHOD0(get_read_size, bd_size_t());
    MOCK_CONST_METHOD0(get_program_size, bd_size_t());
    MOCK_CONST_METHOD0(get_erase_size, bd_size_t());
    MOCK_CONST_METHOD1(get_erase_size, bd_size_t(bd_addr_t));
    MOCK_CONST_METHOD0(get_erase_value, int());
    MOCK_CONST_METHOD0(size, bd_size_t());
    MOCK_CONST_METHOD2(is_valid_read, bool(bd_addr_t, bd_size_t));
    MOCK_CONST_METHOD2(is_valid_program, bool(bd_addr_t, bd_size_t));
    MOCK_CONST_METHOD2(is_valid_erase, bool(bd_addr_t, bd_size_t));
    MOCK_CONST_METHOD0(get_type, const char *());
};

// Can be used to compare if the data programmed by mock are correct.
MATCHER_P2(ByteBufferMatcher, buffer, length, "") {
    const uint8_t *argp = static_cast<const uint8_t*>(arg);
    FILE* fd = fopen("testlog.txt", "w");
    for (int i = 0; i < length; i++) {
        fprintf(fd, "%03d %02X %02X\n", i, argp[i], buffer[i]);
    }
    fclose(fd);
    return 0 == memcmp(arg, buffer, length);
}

ACTION_P2(SetArg0ToCharPtr, value, size)
{
    for (int i = 0; i < size; i++) {
        static_cast<char *>(arg0)[i] = reinterpret_cast<const char *>(value)[i];
    }
}

#endif // BLOCKDEVICEMOCK_H
