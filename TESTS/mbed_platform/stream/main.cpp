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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "FileStream.h"


using utest::v1::Case;

#define STRINGS_SIZE 5
const char *test_strings[STRINGS_SIZE] = { "one", "two", "three", "four", "five" };


/** Test Stream interface: printf/scanf

    Given a FileStream object created
    When put some strings to file stream via printf
    Then it can be read from file stream via scanf
 */
void test_printf_scanf()
{
    char str[16];
    const uint32_t file_max_size = 128;
    FileStream<file_max_size> file("printf_scanf_test_file");

    for(int i = 0; i < STRINGS_SIZE; i++) {
        int ret = file.printf("%s ", test_strings[i]);
    }

    fseek((std::FILE*)file, SEEK_SET, 0);

    for(int i = 0; i < STRINGS_SIZE; i++) {
        file.scanf("%s", str);
        TEST_ASSERT_EQUAL_STRING(test_strings[i], str);
    }
}

/** Test Stream interface: puts/gets

    Given a FileStream object created
    When put some strings to file stream via puts
    Then it can be read from file stream via gets
 */
void test_puts_gets()
{
    char str[16];
    const uint32_t file_max_size = 128;
    FileStream<file_max_size> file("_puts_gets_test_file");

    for(int i = 0; i < STRINGS_SIZE; i++) {
        file.puts(test_strings[i]);
    }

    fseek((std::FILE*)file, 0, SEEK_SET);

    for(int i = 0; i < STRINGS_SIZE; i++) {
        TEST_ASSERT_EQUAL_STRING(test_strings[i], file.gets(str, strlen(test_strings[i]) + 1));
    }
}

/** Test Stream interface: putc/getc

    Given a FileStream object created
    When put some characters to file stream via putc
    Then it can be read from file stream via getc
 */
void test_putc_getc()
{
    const uint32_t file_max_size = 32;
    char chars[] = { 'a', 'b', 'c' };
    const uint32_t count = sizeof(chars);
    FileStream<file_max_size> file("test_file");

    for(int i = 0; i < count; i++) {
        file.putc(chars[i]);
    }

    fseek((std::FILE*)file, 0, SEEK_SET);

    for(int i = 0; i < count; i++) {
        TEST_ASSERT_EQUAL_INT((int)chars[i], file.getc());
    }
}

/** Test FileBase interface: file list

    Given a FileStream class inherited from FileBase
    When before any FileStream object creation
    Then the underlying file list is empty
    When the FileStream object is created
    Then the file is on the underlying file list and is accessible via get and lookup
    When the FileStream object is destroyed
    Then it is not present on the underlying file list
 */
void test_FileBase_list()
{
    const char *name = "test_file";
    const uint32_t file_max_size = 1;

    FileBase *file = FileStream<file_max_size>::get(0);
    TEST_ASSERT_EQUAL_PTR(NULL, file);

    {
        FileStream<file_max_size> stream(name);

        FileBase *file1 = FileStream<file_max_size>::get(0);
        TEST_ASSERT_NOT_NULL(file1);

        FileBase *file2 = FileStream<file_max_size>::lookup(name, strlen(name));
        TEST_ASSERT_EQUAL_PTR(file1, file2);
    }

    file = FileStream<file_max_size>::lookup(name, strlen(name));
    TEST_ASSERT_EQUAL_PTR(NULL, file);
}


/** Test FileBase interface: file name and file type

    Given a FileStream object inherited from FileBase
    When check its name
    Then the name is same as set during construction
    When check its path type
    Then the path type is FilePathType
 */
void test_FileBase_name_and_type()
{
    const char *name = "test_file";

    FileStream<1> stream(name);

    TEST_ASSERT_EQUAL_STRING(name, stream.getName());

    TEST_ASSERT_EQUAL_MESSAGE(FilePathType, stream.getPathType(), "Wrong file type!!!");
}


utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return utest::v1::verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test files list empty", test_FileBase_list),
    Case("Test file name and type", test_FileBase_name_and_type),
    Case("Test putc/getc", test_putc_getc),
    Case("Test puts/gets", test_puts_gets),
    Case("Test printf/scanf", test_printf_scanf),
};

utest::v1::Specification specification(test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}
