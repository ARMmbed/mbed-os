/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
 *
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

/*
 * Based on mbed-stress-test by Marcus Chang @ Arm Mbed - http://github.com/ARMmbed/mbed-stress-test
*/
#if INTEGRATION_TESTS && MBED_CONF_RTOS_PRESENT

#include "mbed.h"
#include "unity/unity.h"
#include "file_test.h"

void file_test_write(const char *file, size_t offset, const unsigned char *data, size_t data_length, size_t block_size)
{
    char filename[255] = { 0 };
    snprintf(filename, 255, "/sd/%s", file);

    FILE *output = fopen(filename, "w+");
    TEST_ASSERT_NOT_NULL_MESSAGE(output, "could not open file");

    int result = fseek(output, offset, SEEK_SET);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "could not seek to location");

    Timer timer;
    timer.start();

    size_t index = 0;
    while (index < data_length) {
        size_t write_length = data_length - index;

        if (write_length > block_size) {
            write_length = block_size;
        }

        size_t written = fwrite(&data[index], sizeof(unsigned char), write_length, output);
        TEST_ASSERT_EQUAL_UINT_MESSAGE(write_length, written, "failed to write");

        index += write_length;
    }
    TEST_ASSERT_EQUAL_UINT_MESSAGE(index, data_length, "wrong length");

    result = fclose(output);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "could not close file");

    timer.stop();
    tr_info("[FS] Wrote: \"%s\" %.2fKB (%.2fKB/s, %.2f secs)", file,
            float(data_length) / 1024, float(data_length) / timer.read() / 1024, timer.read());
}

void file_test_read(const char *file, size_t offset, const unsigned char *data, size_t data_length, size_t block_size)
{
    char filename[255] = { 0 };
    snprintf(filename, 255, "/sd/%s", file);

    FILE *output = fopen(filename, "r");
    TEST_ASSERT_NOT_NULL_MESSAGE(output, "could not open file");

    int result = fseek(output, offset, SEEK_SET);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "could not seek to location");

    char *buffer = (char *) malloc(block_size);
    TEST_ASSERT_NOT_NULL_MESSAGE(buffer, "could not allocate buffer");

    Timer timer;
    timer.start();

    size_t index = 0;
    while (index < data_length) {
        uint32_t read_length = data_length - index;

        if (read_length > block_size) {
            read_length = block_size;
        }

        size_t read = fread(buffer, sizeof(char), read_length, output);
        TEST_ASSERT_EQUAL_MESSAGE(read, read_length, "failed to read");
        TEST_ASSERT_EQUAL_STRING_LEN_MESSAGE(buffer, &data[index], read_length, "character mismatch");

        index += read_length;
    }
    TEST_ASSERT_EQUAL_UINT_MESSAGE(index, data_length, "wrong length");

    result = fclose(output);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "could not close file");

    free(buffer);

    tr_info("[FS] Read : \"%s\" %.2fKB (%.2fKB/s, %.2f secs)", file,
            float(data_length) / 1024, float(data_length) / timer.read() / 1024, timer.read());
}

#endif //#if INTEGRATION_TESTS
