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
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "SPIFBlockDevice.h"
#include "mbed_trace.h"
#include "rtos/Thread.h"
#include <stdlib.h>

using namespace utest::v1;

#define TEST_BLOCK_COUNT 10
#define TEST_ERROR_MASK 16
#define SPIF_TEST_NUM_OF_THREADS 5

const struct {
    const char *name;
    bd_size_t (BlockDevice::*method)() const;
} ATTRS[] = {
    {"read size",    &BlockDevice::get_read_size},
    {"program size", &BlockDevice::get_program_size},
    {"erase size",   &BlockDevice::get_erase_size},
    {"total size",   &BlockDevice::size},
};

static SingletonPtr<PlatformMutex> _mutex;

// Mutex is protecting rand() per srand for buffer writing and verification.
// Mutex is also protecting printouts for clear logs.
// Mutex is NOT protecting Block Device actions: erase/program/read - which is the purpose of the multithreaded test!
void basic_erase_program_read_test(SPIFBlockDevice &block_device, bd_size_t block_size, uint8_t *write_block,
                                   uint8_t *read_block, unsigned addrwidth)
{
    int err = 0;
    _mutex->lock();
    // Find a random block
    bd_addr_t block = (rand() * block_size) % block_device.size();

    // Use next random number as temporary seed to keep
    // the address progressing in the pseudorandom sequence
    unsigned seed = rand();

    // Fill with random sequence
    srand(seed);
    for (bd_size_t i_ind = 0; i_ind < block_size; i_ind++) {
        write_block[i_ind] = 0xff & rand();
    }
    // Write, sync, and read the block
    utest_printf("\ntest  %0*llx:%llu...", addrwidth, block, block_size);
    _mutex->unlock();

    err = block_device.erase(block, block_size);
    TEST_ASSERT_EQUAL(0, err);

    err = block_device.program(write_block, block, block_size);
    TEST_ASSERT_EQUAL(0, err);

    err = block_device.read(read_block, block, block_size);
    TEST_ASSERT_EQUAL(0, err);

    _mutex->lock();
    // Check that the data was unmodified
    srand(seed);
    int val_rand;
    for (bd_size_t i_ind = 0; i_ind < block_size; i_ind++) {
        val_rand = rand();
        if ((0xff & val_rand) != read_block[i_ind]) {
            utest_printf("\n Assert Failed Buf Read - block:size: %llx:%llu \n", block, block_size);
            utest_printf("\n pos: %llu, exp: %02x, act: %02x, wrt: %02x \n", i_ind, (0xff & val_rand), read_block[i_ind],
                         write_block[i_ind]);
        }
        TEST_ASSERT_EQUAL(0xff & val_rand, read_block[i_ind]);
    }
    _mutex->unlock();
}

void test_spif_random_program_read_erase()
{
    utest_printf("\nTest Random Program Read Erase Starts..\n");

    SPIFBlockDevice block_device(MBED_CONF_SPIF_DRIVER_SPI_MOSI, MBED_CONF_SPIF_DRIVER_SPI_MISO,
                                 MBED_CONF_SPIF_DRIVER_SPI_CLK,
                                 MBED_CONF_SPIF_DRIVER_SPI_CS);

    int err = block_device.init();
    TEST_ASSERT_EQUAL(0, err);

    for (unsigned atr = 0; atr < sizeof(ATTRS) / sizeof(ATTRS[0]); atr++) {
        static const char *prefixes[] = {"", "k", "M", "G"};
        for (int i_ind = 3; i_ind >= 0; i_ind--) {
            bd_size_t size = (block_device.*ATTRS[atr].method)();
            if (size >= (1ULL << 10 * i_ind)) {
                utest_printf("%s: %llu%sbytes (%llubytes)\n",
                             ATTRS[atr].name, size >> 10 * i_ind, prefixes[i_ind], size);
                break;
            }
        }
    }

    bd_size_t block_size = block_device.get_erase_size();
    unsigned addrwidth = ceil(log(float(block_device.size() - 1)) / log(float(16))) + 1;

    uint8_t *write_block = new (std::nothrow) uint8_t[block_size];
    uint8_t *read_block = new (std::nothrow) uint8_t[block_size];
    if (!write_block || !read_block) {
        utest_printf("\n Not enough memory for test");
        goto end;
    }

    for (int b = 0; b < TEST_BLOCK_COUNT; b++) {
        basic_erase_program_read_test(block_device, block_size, write_block, read_block, addrwidth);
    }

    err = block_device.deinit();
    TEST_ASSERT_EQUAL(0, err);

end:
    delete[] write_block;
    delete[] read_block;
}

void test_spif_unaligned_erase()
{
    utest_printf("\nTest Unaligned Erase Starts..\n");

    SPIFBlockDevice block_device(MBED_CONF_SPIF_DRIVER_SPI_MOSI, MBED_CONF_SPIF_DRIVER_SPI_MISO,
                                 MBED_CONF_SPIF_DRIVER_SPI_CLK,
                                 MBED_CONF_SPIF_DRIVER_SPI_CS);

    int err = block_device.init();
    TEST_ASSERT_EQUAL(0, err);

    for (unsigned atr = 0; atr < sizeof(ATTRS) / sizeof(ATTRS[0]); atr++) {
        static const char *prefixes[] = {"", "k", "M", "G"};
        for (int i_ind = 3; i_ind >= 0; i_ind--) {
            bd_size_t size = (block_device.*ATTRS[atr].method)();
            if (size >= (1ULL << 10 * i_ind)) {
                utest_printf("%s: %llu%sbytes (%llubytes)\n",
                             ATTRS[atr].name, size >> 10 * i_ind, prefixes[i_ind], size);
                break;
            }
        }
    }

    bd_addr_t addr = 0;
    bd_size_t sector_erase_size = block_device.get_erase_size(addr);
    unsigned addrwidth = ceil(log(float(block_device.size() - 1)) / log(float(16))) + 1;

    utest_printf("\ntest  %0*llx:%llu...", addrwidth, addr, sector_erase_size);

    //unaligned start address
    addr += 1;
    err = block_device.erase(addr, sector_erase_size - 1);
    TEST_ASSERT_EQUAL(SPIF_BD_ERROR_INVALID_ERASE_PARAMS, err);

    err = block_device.erase(addr, sector_erase_size);
    TEST_ASSERT_EQUAL(SPIF_BD_ERROR_INVALID_ERASE_PARAMS, err);

    err = block_device.erase(addr, 1);
    TEST_ASSERT_EQUAL(SPIF_BD_ERROR_INVALID_ERASE_PARAMS, err);

    //unaligned end address
    addr = 0;

    err = block_device.erase(addr, 1);
    TEST_ASSERT_EQUAL(SPIF_BD_ERROR_INVALID_ERASE_PARAMS, err);

    err = block_device.erase(addr, sector_erase_size + 1);
    TEST_ASSERT_EQUAL(SPIF_BD_ERROR_INVALID_ERASE_PARAMS, err);

    //erase size exceeds flash device size
    err = block_device.erase(addr, block_device.size() + 1);
    TEST_ASSERT_EQUAL(SPIF_BD_ERROR_INVALID_ERASE_PARAMS, err);

    // Valid erase
    err = block_device.erase(addr, sector_erase_size);
    TEST_ASSERT_EQUAL(SPIF_BD_ERROR_OK, err);

    err = block_device.deinit();
    TEST_ASSERT_EQUAL(0, err);
}

static void test_spif_thread_job(void *block_device_ptr/*, int thread_num*/)
{
    static int thread_num = 0;
    thread_num++;
    SPIFBlockDevice *block_device = (SPIFBlockDevice *)block_device_ptr;
    utest_printf("\n Thread %d Started \n", thread_num);

    bd_size_t block_size = block_device->get_erase_size();
    unsigned addrwidth = ceil(log(float(block_device->size() - 1)) / log(float(16))) + 1;

    uint8_t *write_block = new (std::nothrow) uint8_t[block_size];
    uint8_t *read_block = new (std::nothrow) uint8_t[block_size];
    if (!write_block || !read_block) {
        utest_printf("\n Not enough memory for test");
        goto end;
    }

    for (int b = 0; b < TEST_BLOCK_COUNT; b++) {
        basic_erase_program_read_test((*block_device), block_size, write_block, read_block, addrwidth);
    }

end:
    delete[] write_block;
    delete[] read_block;
}

void test_spif_multi_threads()
{
    utest_printf("\nTest Multi Threaded Erase/Program/Read Starts..\n");

    SPIFBlockDevice block_device(MBED_CONF_SPIF_DRIVER_SPI_MOSI, MBED_CONF_SPIF_DRIVER_SPI_MISO,
                                 MBED_CONF_SPIF_DRIVER_SPI_CLK,
                                 MBED_CONF_SPIF_DRIVER_SPI_CS);

    int err = block_device.init();
    TEST_ASSERT_EQUAL(0, err);

    for (unsigned atr = 0; atr < sizeof(ATTRS) / sizeof(ATTRS[0]); atr++) {
        static const char *prefixes[] = {"", "k", "M", "G"};
        for (int i_ind = 3; i_ind >= 0; i_ind--) {
            bd_size_t size = (block_device.*ATTRS[atr].method)();
            if (size >= (1ULL << 10 * i_ind)) {
                utest_printf("%s: %llu%sbytes (%llubytes)\n",
                             ATTRS[atr].name, size >> 10 * i_ind, prefixes[i_ind], size);
                break;
            }
        }
    }

    rtos::Thread spif_bd_thread[SPIF_TEST_NUM_OF_THREADS];

    osStatus threadStatus;
    int i_ind;

    for (i_ind = 0; i_ind < SPIF_TEST_NUM_OF_THREADS; i_ind++) {
        threadStatus = spif_bd_thread[i_ind].start(test_spif_thread_job, (void *)&block_device);
        if (threadStatus != 0) {
            utest_printf("\n Thread %d Start Failed!", i_ind + 1);
        }
    }

    for (i_ind = 0; i_ind < SPIF_TEST_NUM_OF_THREADS; i_ind++) {
        spif_bd_thread[i_ind].join();
    }

    err = block_device.deinit();
    TEST_ASSERT_EQUAL(0, err);
}

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing unaligned erase blocks", test_spif_unaligned_erase),
    Case("Testing read write random blocks", test_spif_random_program_read_erase),
    Case("Testing Multi Threads Erase Program Read", test_spif_multi_threads)
};

Specification specification(test_setup, cases);

int main()
{
    mbed_trace_init();
    utest_printf("MAIN STARTS\n");
    return !Harness::run(specification);
}
