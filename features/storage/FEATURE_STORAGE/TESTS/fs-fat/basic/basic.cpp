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

/* The following copyright notice is reproduced from the glibc project */

/* Copyright (C) 1991, 1992 Free Software Foundation, Inc.
This file is part of the GNU C Library.

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with the GNU C Library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.  */


/** @file basic.cpp POSIX File API (stdio) test cases
 *
 * Consult the documentation under the test-case functions for
 * a description of the individual test case.
 */

#include "mbed.h"
#include "SDFileSystem.h"
#include "test_env.h"

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"


using namespace utest::v1;


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
SDFileSystem sd(p11, p12, p13, p14, "sd");
#endif

#define FSFAT_BASIC_MSG_BUF_SIZE              256

const char *sd_file_path = "/sd/out.txt";
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
static control_t sdcard_test_00() {

    uint8_t data_written[FSFAT_BASIC_DATA_SIZE] = { 0 };
    bool result = false;

    // Fill data_written buffer with random data
    // Write these data into the file
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
    bool read_result = false;
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

/** @brief  test-fseek.c test ported from glibc project
 *
 * @return on success returns CaseNext to continue to next test case, otherwise will assert on errors.
 */
static control_t sdcard_test_01() {
    FILE *fp, *fp1;
    int i, j;
    int ret = 0;

    fp = fopen (sd_file_path, "w+");
    if (fp == NULL)	{
        printf("errno=%d\n", errno);
        TEST_ASSERT_MESSAGE(false, "error");
        return CaseNext;
    }

    for (i = 0; i < 256; i++) {
        putc (i, fp);
    }
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
    return CaseNext;
}

utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(300, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
           /*          1         2         3         4         5         6        7  */
           /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
        Case("FSFAT_test_00: fopen()/fgetc()/fprintf()/fclose() test.", sdcard_test_00),
        Case("FSFAT_test_01: fopen()/fseek()/fclose() test.", sdcard_test_01)
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

int main()
{
    return !Harness::run(specification);
}
