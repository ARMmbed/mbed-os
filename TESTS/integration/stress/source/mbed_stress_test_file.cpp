/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "mbed.h"

#include "unity/unity.h"

#if COMPONENT_SPIF || COMPONENT_QSPIF || COMPONENT_DATAFLASH
#define MOUNT_POINT "flash"
#elif COMPONENT_SD
#define MOUNT_POINT "sd"
#else
#error [NOT_SUPPORTED] Target does not support filesystem test
#endif

void mbed_stress_test_format_file(void)
{
    BlockDevice* bd = BlockDevice::get_default_instance();
    FileSystem* fs = FileSystem::get_default_instance();

    int result = fs->reformat(bd);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "could not format block device");
}

void mbed_stress_test_write_file(const char* file, size_t offset, const unsigned char* data, size_t data_length, size_t block_size)
{
    char filename[255] = { 0 };
    snprintf(filename, 255, "/" MOUNT_POINT "/%s", file);

    FILE* output = fopen(filename, "w+");
    TEST_ASSERT_NOT_NULL_MESSAGE(output, "could not open file");

    int result = fseek(output, offset, SEEK_SET);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "could not seek to location");

    size_t index = 0;

    while (index < data_length)
    {
        size_t write_length = data_length - index;

        if (write_length > block_size)
        {
            write_length = block_size;
        }

        size_t written = fwrite(&data[index], sizeof(unsigned char), write_length, output);
        TEST_ASSERT_EQUAL_UINT_MESSAGE(write_length, written, "failed to write");

        index += write_length;
    }
    TEST_ASSERT_EQUAL_UINT_MESSAGE(index, data_length, "wrong length");

    result = fclose(output);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "could not close file");
}

void mbed_stress_test_compare_file(const char* file, size_t offset, const unsigned char* data, size_t data_length, size_t block_size)
{
    char filename[255] = { 0 };
    snprintf(filename, 255, "/" MOUNT_POINT "/%s", file);

    FILE* output = fopen(filename, "r");
    TEST_ASSERT_NOT_NULL_MESSAGE(output, "could not open file");

    int result = fseek(output, offset, SEEK_SET);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "could not seek to location");

    char* buffer = (char*) malloc(block_size);
    TEST_ASSERT_NOT_NULL_MESSAGE(buffer, "could not allocate buffer");

    size_t index = 0;
    while (index < data_length)
    {
        uint32_t read_length = data_length - index;

        if (read_length > block_size)
        {
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
}
