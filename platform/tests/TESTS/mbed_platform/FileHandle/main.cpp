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

#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "TestFile.h"
#include "mbed.h"

using utest::v1::Case;


/** Test fopen and fclose
 *
 *  Given a file to be opened
 *
 *  When the file is open
 *  Then returned file descriptor is valid
 *
 *  When the file is closed
 *  Then underneath retargeting layer function is called
 *       and the fclose function return with succeed
 *
 */
void test_fopen_fclose()
{
    std::FILE *file;
    const uint32_t FS = 5;
    TestFile<FS> fh;

    file = fdopen(&fh, "w+");
    TEST_ASSERT_NOT_NULL(file);

    TestFile<FS>::resetFunctionCallHistory();
    int ret = std::fclose(file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnClose));
    TEST_ASSERT_EQUAL_INT(0, ret);
}


/** Test fwrite and fread
 *
 *  Given already opened file
 *
 *  When write some data to file
 *  Then underneath retargeting layer write function is called
 *       fwrite return number of successfully written elements
 *       when not all elements were written stream error is set
 *
 *  When read previously written data from file
 *  Then underneath retargeting layer read function is called
 *       fread return number of successfully read elements
 *       read data match previously written
 *       when read less then expected stream eof is set
 *
 */
void test_fwrite_fread()
{
    std::FILE *file;
    const uint32_t FS = 5;
    TestFile<FS> fh;
    char read_buf[16];
    const char *str1 = "abc";
    const char *str2 = "def";
    const uint32_t str1_size = strlen(str1);
    const uint32_t str2_size = strlen(str2);
    std::size_t write_ret;
    std::size_t read_ret;

    file = fdopen(&fh, "w+");
    TEST_ASSERT_NOT_NULL(file);
    std::setbuf(file, NULL);

    // write 3; expected written 3
    TestFile<FS>::resetFunctionCallHistory();
    write_ret = std::fwrite(str1, 1, str1_size, file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnWrite));
    TEST_ASSERT_EQUAL_INT(str1_size, write_ret);

    // write 3; expected written 2
    TestFile<FS>::resetFunctionCallHistory();
    write_ret = std::fwrite(str2, 1, str2_size, file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnWrite));
#if !defined(__MICROLIB)
    TEST_ASSERT_TRUE(std::ferror(file) != 0);
#endif
    std::clearerr(file);

    // ARMCC/IAR returns 0 here instead of number of elements successfully written !!!
    TEST_ASSERT_TRUE(write_ret >= 0 && write_ret <= (str2_size - 1));

    // write 3; expected written 0
    TestFile<FS>::resetFunctionCallHistory();
    write_ret = std::fwrite(str1, 1, str1_size, file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnWrite));
#if !defined(__MICROLIB)
    TEST_ASSERT_TRUE(std::ferror(file) != 0);
#endif
    TEST_ASSERT_EQUAL_INT(0, write_ret);

    std::rewind(file);

    // read 3; expected read 3
    TestFile<FS>::resetFunctionCallHistory();
    read_ret = std::fread(read_buf, 1, str1_size, file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnRead));
    TEST_ASSERT_EQUAL_INT(str1_size, read_ret);
    TEST_ASSERT_EQUAL_INT(0, strncmp(str1, read_buf, str1_size));

    // read 3; expected read 2
    TestFile<FS>::resetFunctionCallHistory();
    read_ret = std::fread(read_buf, 1, str2_size, file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnRead));
#if !defined(__MICROLIB)
    TEST_ASSERT_TRUE(std::feof(file) != 0);
#endif
    std::clearerr(file);
    TEST_ASSERT_EQUAL_INT(str2_size - 1, read_ret);
    TEST_ASSERT_EQUAL_INT(0, strncmp(str2, read_buf, str2_size - 1));

    // read 3; expected read 0
    TestFile<FS>::resetFunctionCallHistory();
    read_ret = std::fread(read_buf, 1, str2_size, file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnRead));
#if !defined(__MICROLIB)
    TEST_ASSERT_TRUE(std::feof(file) != 0);
#endif
    TEST_ASSERT_EQUAL_INT(0, read_ret);

    std::fclose(file);
}



/** Test fputc and fgetc
 *
 *  Given already opened file
 *
 *  When write some data to file
 *  Then underneath retargeting layer write function is called
 *       fputc return written element
 *       on failure, returns EOF and stream error is sets
 *
 *  When read previously written data from file
 *  Then underneath retargeting layer read function is called
 *       fgetc return read element
 *       read data match previously written
 *       on failure, returns EOF and stream error/eof is sets respectively
 *
 */
void test_fputc_fgetc()
{
    std::FILE *file;
    const uint32_t FS = 3;
    TestFile<FS> fh;
    char char_buf[3] = {'a', 'b', 'c' };
    int ret;

    file = fdopen(&fh, "w+");
    TEST_ASSERT_NOT_NULL(file);
    std::setbuf(file, NULL);

    // write 1; expected written 1
    TestFile<FS>::resetFunctionCallHistory();
    ret = std::fputc(char_buf[0], file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnWrite));
    TEST_ASSERT_EQUAL_INT(char_buf[0], ret);

    // write 1; expected written 1
    TestFile<FS>::resetFunctionCallHistory();
    ret = std::fputc(char_buf[1], file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnWrite));
    TEST_ASSERT_EQUAL_INT(char_buf[1], ret);

    // write 1; expected written 1
    TestFile<FS>::resetFunctionCallHistory();
    ret = std::fputc(char_buf[2], file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnWrite));
    TEST_ASSERT_EQUAL_INT(char_buf[2], ret);

    // write 1; expected written 0
    TestFile<FS>::resetFunctionCallHistory();
    ret = std::fputc(char_buf[0], file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnWrite));
#if !defined(__MICROLIB)
    TEST_ASSERT_TRUE(std::ferror(file) != 0);
#endif
    TEST_ASSERT_EQUAL_INT(EOF, ret);

    std::rewind(file);

    // read 1; expected read 1
    TestFile<FS>::resetFunctionCallHistory();
    ret = std::fgetc(file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnRead));
    TEST_ASSERT_EQUAL_INT(char_buf[0], ret);

    // read 1; expected read 1
    TestFile<FS>::resetFunctionCallHistory();
    ret = std::fgetc(file);
    TEST_ASSERT_EQUAL_INT(char_buf[1], ret);

    // read 1; expected read 1
    TestFile<FS>::resetFunctionCallHistory();
    ret = std::fgetc(file);
    TEST_ASSERT_EQUAL_INT(char_buf[2], ret);

    // read 1; expected read 0
    TestFile<FS>::resetFunctionCallHistory();
    ret = std::fgetc(file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnRead));
#if !defined(__MICROLIB)
    TEST_ASSERT_TRUE(std::feof(file) != 0);
#endif
    TEST_ASSERT_EQUAL_INT(EOF, ret);

    std::fclose(file);
}

/** Test fputs and fgets
 *
 *  Given already opened file
 *
 *  When write some data to file
 *  Then underneath retargeting layer write function is called
 *       on success, returns a non-negative value
 *       on failure, returns EOF and set stream error
 *
 *  When read previously written data from file
 *  Then underneath retargeting layer read function is called
 *       fgets return valid buffer, and read data match previously written
 *       when read less then expected stream EOF is set
 *       on failure, stream error is sets
 *
 */
void test_fputs_fgets()
{
    std::FILE *file;
    const uint32_t FS = 5;
    TestFile<FS> fh;
    const char *str1 = "abc";
    const char *str2 = "def";
    const uint32_t str1_size = strlen(str1);
    const uint32_t str2_size = strlen(str2);
    char read_buf[16];
    int fputs_ret;
    char *fgets_ret;

    file = fdopen(&fh, "w+");
    TEST_ASSERT_NOT_NULL(file);
    std::setbuf(file, NULL);

    // write 3; expected written 3
    TestFile<FS>::resetFunctionCallHistory();
    fputs_ret = std::fputs(str1, file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnWrite));
    TEST_ASSERT_TRUE(fputs_ret >= 0);

    // write 3; expected written 2
    TestFile<FS>::resetFunctionCallHistory();
    fputs_ret = std::fputs(str2, file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnWrite));
#if !defined(__MICROLIB)
    TEST_ASSERT_TRUE(std::ferror(file) != 0);
#endif
    std::clearerr(file);
    TEST_ASSERT_EQUAL_INT(EOF, fputs_ret);

    // write 3; expected written 0
    TestFile<FS>::resetFunctionCallHistory();
    fputs_ret = std::fputs(str1, file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnWrite));
#if !defined(__MICROLIB)
    TEST_ASSERT_TRUE(std::ferror(file) != 0);
#endif
    TEST_ASSERT_EQUAL_INT(EOF, fputs_ret);

    std::rewind(file);

    // read 3; expected read 3
    TestFile<FS>::resetFunctionCallHistory();
    fgets_ret = std::fgets(read_buf, str1_size + 1, file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnRead));
    TEST_ASSERT_EQUAL_INT(read_buf, fgets_ret);
    TEST_ASSERT_EQUAL_INT(0, strncmp(read_buf, str1, str1_size));

    // read 3; expected read 2
    TestFile<FS>::resetFunctionCallHistory();
    fgets_ret = std::fgets(read_buf, str2_size + 1, file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnRead));
#if !defined(__MICROLIB)
    TEST_ASSERT_TRUE(std::feof(file) != 0);
#endif
    std::clearerr(file);
    TEST_ASSERT_EQUAL_INT(read_buf, fgets_ret);
    TEST_ASSERT_EQUAL_INT(0, strncmp(read_buf, str2, str2_size - 2));

    // read 3; expected read 0
    TestFile<FS>::resetFunctionCallHistory();
    fgets_ret = std::fgets(read_buf, str2_size + 1, file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnRead));
#if !defined(__MICROLIB)
    TEST_ASSERT_TRUE(std::feof(file) != 0);
#endif
    TEST_ASSERT_EQUAL_INT(NULL, fgets_ret);

    std::fclose(file);
}

/** Test fprintf and fscanf
 *
 *  Given already opened file
 *
 *  When write some data to file
 *  Then underneath retargeting layer write function is called
 *       fprintf return number of written components
 *       fprintf return negative value if an error occurred and set stream error
 *
 *  When read previously written data from file
 *  Then underneath retargeting layer read function is called
 *       fscanf return number of read components, and read data match previously written
 *       when read less then expected stream EOF is set
 *       on failure, stream error is sets
 *
 */
void test_fprintf_fscanf()
{
    std::FILE *file;
    const uint32_t FS = 5;
    TestFile<FS> fh;
    const char *str1 = "abc";
    const char *str2 = "def";
    const uint32_t str1_size = strlen(str1);
    const uint32_t str2_size = strlen(str2);
    char read_buf[16];
    int fprintf_ret;
    int fscanf_ret;

    file = fdopen(&fh, "w+");
    TEST_ASSERT_NOT_NULL(file);
    std::setbuf(file, NULL);

    // write 3; expected written 3
    TestFile<FS>::resetFunctionCallHistory();
    fprintf_ret = fprintf(file, "%s", str1);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnWrite));
    TEST_ASSERT_EQUAL_INT(str1_size, fprintf_ret);

#if !defined(__MICROLIB)
    // feof() and ferror() functions are not supported in Microlib.
    // write 3; expected written 2
    TestFile<FS>::resetFunctionCallHistory();
    fprintf_ret = fprintf(file, "%s", str2);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnWrite));
    TEST_ASSERT_TRUE(std::ferror(file) != 0);
    std::clearerr(file);
    TEST_ASSERT_TRUE(fprintf_ret < 0);

    // write 3; expected written 0
    TestFile<FS>::resetFunctionCallHistory();
    fprintf_ret = fprintf(file, "%s", str2);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnWrite));
    TEST_ASSERT_TRUE(std::ferror(file) != 0);
    TEST_ASSERT_TRUE(fprintf_ret < 0);
#else
    // Writing remaining available file space of 2 characters
    // to make further fscanf() test to pass.
    // write 2; expected written 2
    TestFile<FS>::resetFunctionCallHistory();
    fprintf_ret = 0;
    fprintf_ret += fprintf(file, "%c", str2[0]);
    fprintf_ret += fprintf(file, "%c", str2[1]);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnWrite));
    TEST_ASSERT_EQUAL_INT(2, fprintf_ret);
#endif
    std::rewind(file);

    // read 3; expected read 3
    TestFile<FS>::resetFunctionCallHistory();
    fscanf_ret = fscanf(file, "%3s", read_buf);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnRead));
    TEST_ASSERT_EQUAL_INT(1, fscanf_ret);
    TEST_ASSERT_EQUAL_INT(0, strncmp(read_buf, str1, str1_size));

    // read 3; expected read 2
    TestFile<FS>::resetFunctionCallHistory();
    fscanf_ret = fscanf(file, "%3s", read_buf);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnRead));
#if !defined(__MICROLIB)
    TEST_ASSERT_TRUE(std::feof(file) != 0);
    std::clearerr(file);
#endif
    TEST_ASSERT_EQUAL_INT(1, fscanf_ret);
    TEST_ASSERT_EQUAL_INT(0, strncmp(read_buf, str2, str2_size - 1));

    // read 3; expected read 0
    TestFile<FS>::resetFunctionCallHistory();
    fscanf_ret = fscanf(file, "%3s", read_buf);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnRead));
#if !defined(__MICROLIB)
    TEST_ASSERT_TRUE(std::feof(file) != 0);
#endif
    TEST_ASSERT_EQUAL_INT(EOF, fscanf_ret);

    std::fclose(file);
}

/** Test fseek and ftell
 *
 *  ARM library is quite good at optimising out unnecessary calls to underlying
 *  seek, so only test real non empty files.
 *
 *  Given already opened file is not empty
 *
 *  When set the file position indicator via fseek
 *  Then underneath retargeting layer seek function is called
 *       fseek return with succeed and ftell return already set position
 *  Check actual character read or written.
 *
 */
void test_fseek_ftell()
{
    std::FILE *file;
    long ftell_ret;
    int fssek_ret;
    const uint32_t FS = 128;
    TestFile<FS> fh;

    file = fdopen(&fh, "w+");
    TEST_ASSERT_NOT_NULL(file);
    std::setbuf(file, NULL);

    TestFile<FS>::resetFunctionCallHistory();
    ftell_ret = std::ftell(file);
    TEST_ASSERT_EQUAL(0, ftell_ret);

    const char *str = "Hello world";
    const std::size_t size = std::strlen(str);

    std::fwrite(str, 1, size, file);

    TestFile<FS>::resetFunctionCallHistory();
    ftell_ret = std::ftell(file);
    TEST_ASSERT_EQUAL(size, ftell_ret);

    TestFile<FS>::resetFunctionCallHistory();
    fssek_ret = std::fseek(file, 5, SEEK_SET);
    TEST_ASSERT_EQUAL(0, fssek_ret);
    ftell_ret = std::ftell(file);
    TEST_ASSERT_EQUAL(5, ftell_ret);
    int c = std::fgetc(file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnRead));
    TEST_ASSERT_EQUAL(c, str[5]);

    TestFile<FS>::resetFunctionCallHistory();
    fssek_ret = std::fseek(file, -6, SEEK_CUR);
    TEST_ASSERT_EQUAL(0, fssek_ret);
    ftell_ret = std::ftell(file);
    TEST_ASSERT_EQUAL(0, ftell_ret);
    c = std::fgetc(file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnRead));
    TEST_ASSERT_EQUAL(c, str[0]);

    TestFile<FS>::resetFunctionCallHistory();
    fssek_ret = std::fseek(file, 0, SEEK_END);
    TEST_ASSERT_EQUAL(0, fssek_ret);
    ftell_ret = std::ftell(file);
    TEST_ASSERT_EQUAL(size, ftell_ret);
    c = std::fputc('!', file);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnWrite));
    TEST_ASSERT_EQUAL(c, '!');
    TEST_ASSERT_EQUAL(fh.size(), size + 1);

    std::fclose(file);
}

/** Test ftruncate and fstat (st_size)
 *
 *  Check we get EBADF for illegal handles
 *
 *  Given already opened file is empty
 *
 *  Check initial size is returned as 0
 *  Call ftruncate with negative value - check our EINVAL is passed back
 *  Call ftruncate with positive value to increase size - check no error return
 *  Check fstat st_size now reads back as the value we set.
 *  Call ftruncate with smaller positive value to decrease size - check no error return
 *  Check fstat st_size now reads back as the value we set.
 */
void test_ftruncate_fstat()
{
    int fildes;
    int ret;
    struct stat st;
    const uint32_t FS = 128;
    TestFile<FS> fh;

    ret = ftruncate(12345678, 24);
    TEST_ASSERT_EQUAL(-1, ret);
    TEST_ASSERT_EQUAL(EBADF, errno);

    ret = fstat(12345678, &st);
    TEST_ASSERT_EQUAL(-1, ret);
    TEST_ASSERT_EQUAL(EBADF, errno);

    fildes = bind_to_fd(&fh);
    TEST_ASSERT_TRUE(fildes >= 0);

    ret = fstat(fildes, &st);
    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_EQUAL(0, st.st_size);

    TestFile<FS>::resetFunctionCallHistory();
    ret = ftruncate(fildes, -3);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnTruncate));
    TEST_ASSERT_EQUAL(-1, ret);
    TEST_ASSERT_EQUAL(EINVAL, errno);

    TestFile<FS>::resetFunctionCallHistory();
    ret = ftruncate(fildes, 24);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnTruncate));
    TEST_ASSERT_EQUAL(0, ret);

    ret = fstat(fildes, &st);
    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_EQUAL(24, st.st_size);

    ret = ftruncate(fildes, 12);
    TEST_ASSERT_TRUE(TestFile<FS>::functionCalled(TestFile<FS>::fnTruncate));
    TEST_ASSERT_EQUAL(0, ret);

    ret = fstat(fildes, &st);
    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_EQUAL(12, st.st_size);

    close(fildes);
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return utest::v1::verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test fopen/fclose", test_fopen_fclose),
    Case("Test fwrite/fread", test_fwrite_fread),
    Case("Test fputc/fgetc", test_fputc_fgetc),
    Case("Test fputs/fgets", test_fputs_fgets),
    Case("Test fprintf/fscanf", test_fprintf_fscanf),
    Case("Test fseek/ftell", test_fseek_ftell),
    Case("Test ftruncate/fstat", test_ftruncate_fstat)
};

utest::v1::Specification specification(test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}
