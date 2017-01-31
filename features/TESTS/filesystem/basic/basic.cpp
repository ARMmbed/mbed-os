/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
 *
 */

/* The following copyright notice is reproduced from the glibc project
 * REF_LICENCE_GLIBC
 *
 * Copyright (C) 1991, 1992 Free Software Foundation, Inc.
 * This file is part of the GNU C Library.
 *
 * The GNU C Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * The GNU C Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the GNU C Library; see the file COPYING.LIB.  If
 * not, write to the Free Software Foundation, Inc., 675 Mass Ave,
 * Cambridge, MA 02139, USA.
 */


/** @file basic.cpp POSIX File API (stdio) test cases
 *
 * Consult the documentation under the test-case functions for
 * a description of the individual test case.
 */

#include "mbed.h"
#include "SDFileSystem.h"
#include "test_env.h"
#include "fsfat_debug.h"
#include "fsfat_test.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

#include <stdio.h>
/* FIXME: unistd.h needed for fsfat_basic_test_04 but this error is generated:
 * [Error] unistd.h@185,10: conflicting declaration of C function 'unsigned int sleep(unsigned int)'
 * #include <unistd.h>     // STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO
 */
#include <stdlib.h>
#include <string.h>

using namespace utest::v1;

/* DEVICE_SPI
 *  This symbol is defined in targets.json if the target has a SPI interface, which is required for SDCard support.
 * FSFAT_SDCARD_INSTALLTED
 *  For testing purposes, an SDCard must be installed on the target for the test cases in this file to succeed.
 *  If the target has an SD card installed then uncomment the #define FSFAT_SDCARD_INSTALLED directive for the target.
 *
 * Notes
 *   The following 2 lines should be instated to perform the tests in this file:
 *      #define FSFAT_SDCARD_INSTALLED
 *      #define DEVICE_SPI
 */
#if defined(DEVICE_SPI) && defined(FSFAT_SDCARD_INSTALLED)

#if defined(TARGET_KL25Z)
SDFileSystem sd(PTD2, PTD3, PTD1, PTD0, "sd");

#elif defined(TARGET_KL46Z) || defined(TARGET_KL43Z)
SDFileSystem sd(PTD6, PTD7, PTD5, PTD4, "sd");

#elif defined(TARGET_K64F) || defined(TARGET_K66F)
SDFileSystem sd(PTE3, PTE1, PTE2, PTE4, "sd");

#elif defined(TARGET_K22F)
SDFileSystem sd(PTD6, PTD7, PTD5, PTD4, "sd");

#elif defined(TARGET_K20D50M)
SDFileSystem sd(PTD2, PTD3, PTD1, PTC2, "sd");

#elif defined(TARGET_nRF51822)
SDFileSystem sd(p12, p13, p15, p14, "sd");

#elif defined(TARGET_NUCLEO_F030R8) || \
      defined(TARGET_NUCLEO_F070RB) || \
      defined(TARGET_NUCLEO_F072RB) || \
      defined(TARGET_NUCLEO_F091RC) || \
      defined(TARGET_NUCLEO_F103RB) || \
      defined(TARGET_NUCLEO_F302R8) || \
      defined(TARGET_NUCLEO_F303RE) || \
      defined(TARGET_NUCLEO_F334R8) || \
      defined(TARGET_NUCLEO_F401RE) || \
      defined(TARGET_NUCLEO_F410RB) || \
      defined(TARGET_NUCLEO_F411RE) || \
      defined(TARGET_NUCLEO_L053R8) || \
      defined(TARGET_NUCLEO_L073RZ) || \
      defined(TARGET_NUCLEO_L152RE)
SDFileSystem sd(D11, D12, D13, D10, "sd");

#elif defined(TARGET_DISCO_F051R8) || \
      defined(TARGET_NUCLEO_L031K6)
SDFileSystem sd(SPI_MOSI, SPI_MISO, SPI_SCK, SPI_CS, "sd");

#elif defined(TARGET_LPC2368)
SDFileSystem sd(p11, p12, p13, p14, "sd");

#elif defined(TARGET_LPC11U68)
SDFileSystem sd(D11, D12, D13, D10, "sd");

#elif defined(TARGET_LPC1549)
SDFileSystem sd(D11, D12, D13, D10, "sd");

#elif defined(TARGET_RZ_A1H)
SDFileSystem sd(P8_5, P8_6, P8_3, P8_4, "sd");

#elif defined(TARGET_LPC11U37H_401)
SDFileSystem sd(SDMOSI, SDMISO, SDSCLK, SDSSEL, "sd");

#else
#error "[NOT SUPPORTED] Instantiate SDFileSystem sd(p11, p12, p13, p14, "sd") with the correct pin specification for target"
#endif


#define FSFAT_BASIC_TEST_00      fsfat_basic_test_00
#define FSFAT_BASIC_TEST_01      fsfat_basic_test_01
#define FSFAT_BASIC_TEST_02      fsfat_basic_test_02
#define FSFAT_BASIC_TEST_03      fsfat_basic_test_03
#define FSFAT_BASIC_TEST_04      fsfat_basic_test_04


#define FSFAT_BASIC_MSG_BUF_SIZE              256

static const char *sd_file_path = "/sd/out.txt";
static const char *sd_mount_pt = "sd";
const int FSFAT_BASIC_DATA_SIZE = 256;
static char fsfat_basic_msg_g[FSFAT_BASIC_MSG_BUF_SIZE];

#define FSFAT_BASIC_MSG(_buf, _max_len, _fmt, ...)   \
  do                                                            \
  {                                                             \
      snprintf((_buf), (_max_len), (_fmt), __VA_ARGS__);        \
  }while(0);

/** @brief  fopen test case
 *
 * - open a file
 * - generate random data items, write the item to the file and store a coy in a buffer for later use.
 * - close the file.
 * - open the file.
 * - read the data items from the file and check they are the same as write.
 * - close the file.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
static control_t fsfat_basic_test_00()
{

    uint8_t data_written[FSFAT_BASIC_DATA_SIZE] = { 0 };
    bool result = false;
    bool read_result = false;

    // Fill data_written buffer with random data
    // Write these data into the file
    FSFAT_FENTRYLOG("%s:entered\n", __func__);
    bool write_result = false;
    {
        printf("SD: Writing ... ");
        FILE *f = fopen(sd_file_path, "w");
        if (f) {
            for (int i = 0; i < FSFAT_BASIC_DATA_SIZE; i++) {
                data_written[i] = rand() % 0XFF;
                fprintf(f, "%c", data_written[i]);
            }
            write_result = true;
            fclose(f);
        }
        printf("[%s]\r\n", write_result ? "OK" : "FAIL");
    }

    // Read back the data from the file and store them in data_read
    {
        printf("SD: Reading data ... ");
        FILE *f = fopen(sd_file_path, "r");
        if (f) {
              read_result = true;
            for (int i = 0; i < FSFAT_BASIC_DATA_SIZE; i++) {
                uint8_t data = fgetc(f);
                if (data != data_written[i]) {
                    read_result = false;
                    break;
                }
            }
            fclose(f);
        }
        printf("[%s]\r\n", read_result ? "OK" : "FAIL");
    }
    result = write_result && read_result;
    return CaseNext;
}


extern int errno;

/** @brief  test-fseek.c test ported from glibc project. See the licence at REF_LICENCE_GLIBC.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
static control_t fsfat_basic_test_01()
{
    FILE *fp, *fp1;
    int i, j;
    int ret = 0;

    FSFAT_FENTRYLOG("%s:entered\n", __func__);
    fp = fopen (sd_file_path, "w+");
    if (fp == NULL)	{
        printf("errno=%d\n", errno);
        TEST_ASSERT_MESSAGE(false, "error");
        return CaseNext;
    }

    for (i = 0; i < 256; i++) {
        putc (i, fp);
    }
    /* FIXME: freopen() should open the specified file closing the first stream. As can be seen from the
     * code below, the old file descriptor fp can still be used, and this should not happen.
     */
    fp1 = freopen (sd_file_path, "r", fp);
    TEST_ASSERT_MESSAGE(fp1 == fp, "Error: cannot open file for reading");

    for (i = 1; i <= 255; i++)	{
        ret = fseek (fp, (long) -i, SEEK_END);
        FSFAT_BASIC_MSG(fsfat_basic_msg_g, FSFAT_BASIC_MSG_BUF_SIZE, "%s:Error: fseek() failed (ret=%d).\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(ret == 0, fsfat_basic_msg_g);

        if ((j = getc (fp)) != 256 - i) {
            FSFAT_BASIC_MSG(fsfat_basic_msg_g, FSFAT_BASIC_MSG_BUF_SIZE, "%s: Error: SEEK_END failed (j=%d)\n",  __func__, j);
            TEST_ASSERT_MESSAGE(false, fsfat_basic_msg_g);
        }
        ret = fseek (fp, (long) i, SEEK_SET);
        FSFAT_BASIC_MSG(fsfat_basic_msg_g, FSFAT_BASIC_MSG_BUF_SIZE, "%s: Error: Cannot SEEK_SET (ret=%d).\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(ret == 0, fsfat_basic_msg_g);

        if ((j = getc (fp)) != i) {
            FSFAT_BASIC_MSG(fsfat_basic_msg_g, FSFAT_BASIC_MSG_BUF_SIZE, "%s: Error: Cannot SEEK_SET (j=%d).\n", __func__, j);
            TEST_ASSERT_MESSAGE(ret == 0, fsfat_basic_msg_g);
        }
        if ((ret = fseek (fp, (long) i, SEEK_SET))) {
            FSFAT_BASIC_MSG(fsfat_basic_msg_g, FSFAT_BASIC_MSG_BUF_SIZE, "%s: Error: Cannot SEEK_SET (ret=%d).\n", __func__, (int) ret);
            TEST_ASSERT_MESSAGE(ret == 0, fsfat_basic_msg_g);
        }
        if ((ret = fseek (fp, (long) (i >= 128 ? -128 : 128), SEEK_CUR))) {
            FSFAT_BASIC_MSG(fsfat_basic_msg_g, FSFAT_BASIC_MSG_BUF_SIZE, "%s: Error: Cannot SEEK_CUR (ret=%d).\n", __func__, (int) ret);
            TEST_ASSERT_MESSAGE(ret == 0, fsfat_basic_msg_g);
        }
        if ((j = getc (fp)) != (i >= 128 ? i - 128 : i + 128)) {
            FSFAT_BASIC_MSG(fsfat_basic_msg_g, FSFAT_BASIC_MSG_BUF_SIZE, "%s: Error: Cannot SEEK_CUR (j=%d).\n", __func__, j);
            TEST_ASSERT_MESSAGE(ret == 0, fsfat_basic_msg_g);
        }
    }
    fclose (fp);
    remove(sd_file_path);
    return CaseNext;
}


/** @brief  test_rdwr.c test ported from glibc project. See the licence at REF_LICENCE_GLIBC.
 *
 * WARNING: this test does not currently work. See WARNING comments below.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
static control_t fsfat_basic_test_02()
{
    static const char hello[] = "Hello, world.\n";
    static const char replace[] = "Hewwo, world.\n";
    static const size_t replace_from = 2, replace_to = 4;
    char* filename = (char*) sd_file_path; //todo: remove
    char buf[BUFSIZ];
    FILE *f;
    int lose = 0;
    int32_t ret = 0;
    char *rets = NULL;

    FSFAT_FENTRYLOG("%s:entered\n", __func__);
    f = fopen(filename, "w+");
    if (f == NULL) {
        FSFAT_BASIC_MSG(fsfat_basic_msg_g, FSFAT_BASIC_MSG_BUF_SIZE, "%s: Error: Cannot open file for writing (filename=%s).\n", __func__, filename);
        TEST_ASSERT_MESSAGE(false, fsfat_basic_msg_g);
    }


    ret = fputs(hello, f);
    if (ret == EOF) {
        FSFAT_BASIC_MSG(fsfat_basic_msg_g, FSFAT_BASIC_MSG_BUF_SIZE, "%s: Error: fputs() failed to write string to file (filename=%s, string=%s).\n", __func__, filename, hello);
        TEST_ASSERT_MESSAGE(false, fsfat_basic_msg_g);
    }

    rewind(f);
    rets = fgets(buf, sizeof(buf), f);
    if (rets == NULL) {
        FSFAT_BASIC_MSG(fsfat_basic_msg_g, FSFAT_BASIC_MSG_BUF_SIZE, "%s: Error: fgets() failed to get string from file (filename=%s).\n", __func__, filename);
        TEST_ASSERT_MESSAGE(false, fsfat_basic_msg_g);
    }
    rets = NULL;

    rewind(f);
    ret = fputs(buf, f);
    if (ret == EOF) {
        FSFAT_BASIC_MSG(fsfat_basic_msg_g, FSFAT_BASIC_MSG_BUF_SIZE, "%s: Error: fputs() failed to write string to file (filename=%s, string=%s).\n", __func__, filename, buf);
        TEST_ASSERT_MESSAGE(false, fsfat_basic_msg_g);
    }

    rewind(f);
    {
        register size_t i;
        for (i = 0; i < replace_from; ++i)
        {
            int c = getc(f);
            if (c == EOF)
            {
                printf("EOF at %u.\n", i);
                lose = 1;
                break;
            }
            else if (c != hello[i])
            {
                printf("Got '%c' instead of '%c' at %u.\n",
                (unsigned char) c, hello[i], i);
                lose = 1;
                break;
            }
        }
    }
    /* WARNING: printf("%s: here1. (lose = %d)\n", __func__, lose); */
    {
        long int where = ftell(f);
        if (where == replace_from)
        {
            register size_t i;
            for (i = replace_from; i < replace_to; ++i) {
                if (putc(replace[i], f) == EOF) {
                    printf("putc('%c') got %s at %u.\n",
                    replace[i], strerror(errno), i);
                    lose = 1;
                    break;
                }
                /* WARNING: The problem seems to be that putc() is not writing the 'w' chars into the file
                 * printf("%s: here1.5. (char = %c, char as int=%d, ret=%d) \n", __func__, replace[i], (int) replace[i], ret);
                 */
            }
        }
        else if (where == -1L)
        {
            printf("ftell got %s (should be at %u).\n",
            strerror(errno), replace_from);
            lose = 1;
        }
        else
        {
            printf("ftell returns %u; should be %u.\n", where, replace_from);
            lose = 1;
        }
    }

    if (!lose)
    {
        rewind(f);
        memset(buf, 0, BUFSIZ);
        if (fgets(buf, sizeof(buf), f) == NULL)
        {
            printf("fgets got %s.\n", strerror(errno));
            lose = 1;
        }
        else if (strcmp(buf, replace))
        {
            printf("Read \"%s\" instead of \"%s\".\n", buf, replace);
            lose = 1;
        }
    }

    if (lose) {
        FSFAT_BASIC_MSG(fsfat_basic_msg_g, FSFAT_BASIC_MSG_BUF_SIZE, "%s: Error: Test Failed. Losing file (filename=%s).\n", __func__, filename);
        TEST_ASSERT_MESSAGE(false, fsfat_basic_msg_g);
    }
    remove(filename);
    return CaseNext;
}

/** @brief  temptest.c test ported from glibc project. See the licence at REF_LICENCE_GLIBC.
 *
 * WARNING: this test does not currently work. See WARNING comments below.
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
static control_t fsfat_basic_test_03()
{
    char *fn;
    FILE *fp;
    char *files[500];
    int i;

    FSFAT_FENTRYLOG("%s:entered\n", __func__);
    memset(files, 0, 500*sizeof(char*));
    for (i = 0; i < 500; i++) {
        fn = tmpnam((char *) NULL);

        /* FIXME: tmpnam() doesnt currently generate a temporary filename
         * re-instate the code below when it does.
        FSFAT_BASIC_MSG(fsfat_basic_msg_g, FSFAT_BASIC_MSG_BUF_SIZE, "%s: Error: failed to generate a temporary filename.\n", __func__);
        TEST_ASSERT_MESSAGE(fn != NULL, fsfat_basic_msg_g);

        files[i] = strdup(fn);
        FSFAT_DBGLOG("%s:filename=%s\n", __func__, fn);
        fp = fopen (fn, "w");
        fclose(fp);
         */
    }

    for (i = 0; i < 500; i++) {
        if(files[i] != NULL) {
            remove(files[i]);
            free(files[i]);
        }
    }
    return CaseNext;
}


static bool fsfat_basic_fileno_check(const char *name, FILE *stream, int fd)
{
    int sfd = fileno (stream);
    FSFAT_DBGLOG("(fileno (%s) = %d) %c= %d\n", name, sfd, sfd == fd ? '=' : '!', fd);

    if (sfd == fd) {
        return true;
    } else {
        return false;
    }
}

/** @brief  tst-fileno.c test ported from glibc project. See the licence at REF_LICENCE_GLIBC.
 *
 * WARNING: this test does not currently work. See WARNING comments below.
 *
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
static control_t fsfat_basic_test_04()
{
    /* FIXME: unistd.h needed for STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO but this error is generated:
     * [Error] unistd.h@185,10: conflicting declaration of C function 'unsigned int sleep(unsigned int)'
    int ret = -1;
    ret = fsfat_basic_fileno_check("stdin", stdin, STDIN_FILENO);
    FSFAT_BASIC_MSG(fsfat_basic_msg_g, FSFAT_BASIC_MSG_BUF_SIZE, "%s: Error: stdin does not have expected file number (expected=%d, fileno=%d.\n", __func__, stdin, fileno(stdin));
    TEST_ASSERT_MESSAGE(ret == true, fsfat_basic_msg_g);

    ret = fsfat_basic_fileno_check("stdout", stdout, STDOUT_FILENO);
    FSFAT_BASIC_MSG(fsfat_basic_msg_g, FSFAT_BASIC_MSG_BUF_SIZE, "%s: Error: stdout does not have expected file number (expected=%d, fileno=%d.\n", __func__, stdout, fileno(stdout));
    TEST_ASSERT_MESSAGE(ret == true, fsfat_basic_msg_g);

    ret = fsfat_basic_fileno_check("stderr", stderr, STDERR_FILENO);
    FSFAT_BASIC_MSG(fsfat_basic_msg_g, FSFAT_BASIC_MSG_BUF_SIZE, "%s: Error: stderr does not have expected file number (expected=%d, fileno=%d.\n", __func__, stderr, fileno(stderr));
    TEST_ASSERT_MESSAGE(ret == true, fsfat_basic_msg_g);
    */
    return CaseNext;
}

#else

#define FSFAT_BASIC_TEST_00      fsfat_basic_test_dummy
#define FSFAT_BASIC_TEST_01      fsfat_basic_test_dummy
#define FSFAT_BASIC_TEST_02      fsfat_basic_test_dummy
#define FSFAT_BASIC_TEST_03      fsfat_basic_test_dummy
#define FSFAT_BASIC_TEST_04      fsfat_basic_test_dummy

/** @brief  fsfat_basic_test_dummy    Dummy test case for testing when platform doesnt have an SDCard installed.
 *
 * @return success always
 */
static control_t fsfat_basic_test_dummy()
{
    printf("Null test\n");
    return CaseNext;
}

#endif  /* defined(DEVICE_SPI) && defined(FSFAT_SDCARD_INSTALLED) */

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(300, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}


Case cases[] = {
           /*          1         2         3         4         5         6        7  */
           /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        Case("FSFAT_BASIC_TEST_00: fopen()/fgetc()/fprintf()/fclose() test.", FSFAT_BASIC_TEST_00),
        Case("FSFAT_BASIC_TEST_01: fopen()/fseek()/fclose() test.", FSFAT_BASIC_TEST_01)
        /* WARNING: Test case not working but currently not required for PAL support
         * Case("FSFAT_BASIC_TEST_02: fopen()/fgets()/fputs()/ftell()/rewind()/remove() test.", FSFAT_BASIC_TEST_02)
         * Case("FSFAT_BASIC_TEST_03: tmpnam() test.", FSFAT_BASIC_TEST_03)
         * Case("FSFAT_BASIC_TEST_04: fileno() test.", FSFAT_BASIC_TEST_04)
         */
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}
