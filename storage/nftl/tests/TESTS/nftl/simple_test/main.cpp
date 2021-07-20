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

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include <stdlib.h>

#include "nftl_conf.h"
#include "nftl_util.h"
#include "nftl.h"

using namespace utest::v1;

uint8_t     write_buf[2048];
uint8_t     read_buf[2048];
uint8_t     write_spare_buf[16];
uint8_t     read_spare_buf[16];

// Simple test for all APIs
void functionality_test()
{
	uint32_t    block;
	uint32_t    i;
	uint32_t    j;
	uint32_t    w_off;
	uint32_t    r_off;
	uint32_t    total_blk;
	uint8_t     data_record[200];

	nftl_init();

	total_blk = 200;

	//for (i = 0; i < 3; i++)
	{

		for (j = 0; j < 1; j++) {

			data_record[j] = i*64 + j;

			memset(write_buf, data_record[j], 2048);

			nftl_flash_write(NFTL_PARTITION0, j, 0, write_buf, 2048);
			nftl_flash_write(NFTL_PARTITION0, j, 0, write_buf, 2048);
			nftl_flash_write(NFTL_PARTITION0, j, 0, write_buf, 2048);
		}
	}

	printf("write finished\r\n");

	for (i = 0; i < 1; i++) {

		nftl_flash_read(NFTL_PARTITION0, i, 0, read_buf, 2048);

		if (read_buf[10] != data_record[i]) {
			printf("failed to check data\r\n");
		} else {
			printf("match: %d, %d\r\n", read_buf[10], data_record[i]);
		}
	}

	printf("test pass\r\n");

 /*   uint8_t *dummy = new (std::nothrow) uint8_t[num_blocks * erase_size];
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory for test");
    delete[] dummy;

    HeapBlockDevice heap_bd(num_blocks * erase_size, read_size, prog_size, erase_size);
    FlashSimBlockDevice bd(&heap_bd, blank);

    int err = bd.init();
    TEST_ASSERT_EQUAL(0, err);

    uint8_t read_buf[test_buf_size], write_buf[test_buf_size];

    TEST_ASSERT_EQUAL(num_blocks * erase_size, bd.size());
    TEST_ASSERT_EQUAL(read_size, bd.get_read_size());
    TEST_ASSERT_EQUAL(prog_size, bd.get_program_size());
    TEST_ASSERT_EQUAL(erase_size, bd.get_erase_size());
    TEST_ASSERT_EQUAL(blank, bd.get_erase_value());

    srand(1);
    for (bd_size_t i = 0; i < test_buf_size; i++) {
        write_buf[i] = 0xff & rand();
    }

    // Make sure we can't program if not erased (even after init)
    err = bd.program(write_buf, 0, test_buf_size);
    TEST_ASSERT_EQUAL(BD_ERROR_NOT_ERASED, err);

    err = bd.erase(0, erase_size * 2);
    TEST_ASSERT_EQUAL(0, err);

    err = bd.program(write_buf, 0, test_buf_size);
    TEST_ASSERT_EQUAL(0, err);

    // Allow programming same data
    err = bd.program(write_buf, 0, test_buf_size);
    TEST_ASSERT_EQUAL(0, err);

    srand(2);
    for (bd_size_t i = 0; i < test_buf_size; i++) {
        write_buf[i] = 0xff & rand();
    }

    err = bd.program(write_buf, 0, test_buf_size);
    TEST_ASSERT_EQUAL(BD_ERROR_NOT_ERASED, err);

    err = bd.program(write_buf, 2 * erase_size - test_buf_size, test_buf_size);
    TEST_ASSERT_EQUAL(0, err);

    memset(write_buf, blank, test_buf_size / 2);
    err = bd.read(read_buf, test_buf_size * 2, test_buf_size / 2);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(write_buf, read_buf, test_buf_size / 2);

    srand(1);
    for (bd_size_t i = 0; i < test_buf_size; i++) {
        write_buf[i] = 0xff & rand();
    }

    err = bd.read(read_buf, 0, test_buf_size);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(write_buf, read_buf, test_buf_size);

    srand(2);
    for (bd_size_t i = 0; i < test_buf_size; i++) {
        write_buf[i] = 0xff & rand();
    }

    err = bd.read(read_buf, 2 * erase_size - test_buf_size, test_buf_size);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(write_buf, read_buf, test_buf_size);

    err = bd.deinit();
    TEST_ASSERT_EQUAL(0, err);

    err = bd.init();
    TEST_ASSERT_EQUAL(0, err);

    // Make sure data lives across inits
    err = bd.read(read_buf, 2 * erase_size - test_buf_size, test_buf_size);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(write_buf, read_buf, test_buf_size);

    err = bd.erase(0, erase_size);
    TEST_ASSERT_EQUAL(0, err);

    // Make sure erase returns the erase value
    memset(write_buf, blank, test_buf_size);
    err = bd.read(read_buf, 0, test_buf_size);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(write_buf, read_buf, test_buf_size);*/
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(3000, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("NFTL functionality test", functionality_test),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
