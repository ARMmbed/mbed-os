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
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS //Required for PRIu64
#endif

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "mbed_trace.h"
#include <inttypes.h>
#include <stdlib.h>
#include "BufferedBlockDevice.h"

using namespace utest::v1;

#define TEST_BLOCK_COUNT 10
#define TEST_ERROR_MASK 16
#define TEST_NUM_OF_THREADS 5

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
void basic_erase_program_read_test(BlockDevice *block_device, bd_size_t block_size, uint8_t *write_block,
                                   uint8_t *read_block, unsigned addrwidth)
{
    int err = 0;
    _mutex->lock();
    // Find a random block
    bd_addr_t block = (rand() * block_size) % (block_device->size());

    // Use next random number as temporary seed to keep
    // the address progressing in the pseudorandom sequence
    unsigned seed = rand();

    // Fill with random sequence
    srand(seed);
    for (bd_size_t i_ind = 0; i_ind < block_size; i_ind++) {
        write_block[i_ind] = 0xff & rand();
    }
    // Write, sync, and read the block
    utest_printf("test  %0*llx:%llu...\n", addrwidth, block, block_size);
    _mutex->unlock();

    err = block_device->erase(block, block_size);
    TEST_ASSERT_EQUAL(0, err);

    err = block_device->program(write_block, block, block_size);
    TEST_ASSERT_EQUAL(0, err);

    err = block_device->read(read_block, block, block_size);
    TEST_ASSERT_EQUAL(0, err);

    _mutex->lock();
    // Check that the data was unmodified
    srand(seed);
    int val_rand;
    for (bd_size_t i_ind = 0; i_ind < block_size; i_ind++) {
        val_rand = rand();
        if ((0xff & val_rand) != read_block[i_ind]) {
            utest_printf("\n Assert Failed Buf Read - block:size: %llx:%llu \n", block, block_size);
            utest_printf("\n pos: %llu, exp: %02x, act: %02x, wrt: %02x \n", i_ind, (0xff & val_rand),
                         read_block[i_ind],
                         write_block[i_ind]);
        }
        TEST_ASSERT_EQUAL(0xff & val_rand, read_block[i_ind]);
    }
    _mutex->unlock();
}

void test_random_program_read_erase()
{
    utest_printf("\nTest Random Program Read Erase Starts..\n");

    BlockDevice *block_device = BlockDevice::get_default_instance();

    TEST_SKIP_UNLESS_MESSAGE(block_device != NULL, "no block device found.");

    int err = block_device->init();
    TEST_ASSERT_EQUAL(0, err);

    for (unsigned atr = 0; atr < sizeof(ATTRS) / sizeof(ATTRS[0]); atr++) {
        static const char *prefixes[] = {"", "k", "M", "G"};
        for (int i_ind = 3; i_ind >= 0; i_ind--) {
            bd_size_t size = (block_device->*ATTRS[atr].method)();
            if (size >= (1ULL << 10 * i_ind)) {
                utest_printf("%s: %llu%sbytes (%llubytes)\n",
                             ATTRS[atr].name, size >> 10 * i_ind, prefixes[i_ind], size);
                break;
            }
        }
    }

    bd_size_t block_size = block_device->get_erase_size();
    unsigned addrwidth = ceil(log(float(block_device->size() - 1)) / log(float(16))) + 1;

    uint8_t *write_block = new (std::nothrow) uint8_t[block_size];
    uint8_t *read_block = new (std::nothrow) uint8_t[block_size];
    if (!write_block || !read_block) {
        utest_printf("Not enough memory for test\n");
        goto end;
    }

    for (int b = 0; b < TEST_BLOCK_COUNT; b++) {
        basic_erase_program_read_test(block_device, block_size, write_block, read_block, addrwidth);
    }

    err = block_device->deinit();
    TEST_ASSERT_EQUAL(0, err);

end:
    delete[] write_block;
    delete[] read_block;
}

static void test_thread_job(void *block_device_ptr)
{
    static int thread_num = 0;
    thread_num++;
    BlockDevice *block_device = (BlockDevice *)block_device_ptr;

    bd_size_t block_size = block_device->get_erase_size();
    unsigned addrwidth = ceil(log(float(block_device->size() - 1)) / log(float(16))) + 1;

    uint8_t *write_block = new (std::nothrow) uint8_t[block_size];
    uint8_t *read_block = new (std::nothrow) uint8_t[block_size];

    if (!write_block || !read_block) {
        utest_printf("Not enough memory for test\n");
        goto end;
    }

    for (int b = 0; b < TEST_BLOCK_COUNT; b++) {
        basic_erase_program_read_test(block_device, block_size, write_block, read_block, addrwidth);
    }

end:
    delete[] write_block;
    delete[] read_block;
}

void test_multi_threads()
{
    utest_printf("\nTest Multi Threaded Erase/Program/Read Starts..\n");

    BlockDevice *block_device = BlockDevice::get_default_instance();

    TEST_SKIP_UNLESS_MESSAGE(block_device != NULL, "\nno block device found.\n");

    char *dummy = new (std::nothrow) char[TEST_NUM_OF_THREADS * OS_STACK_SIZE];
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory for test.\n");
    delete[] dummy;

    int err = block_device->init();
    TEST_ASSERT_EQUAL(0, err);

    for (unsigned atr = 0; atr < sizeof(ATTRS) / sizeof(ATTRS[0]); atr++) {
        static const char *prefixes[] = {"", "k", "M", "G"};
        for (int i_ind = 3; i_ind >= 0; i_ind--) {
            bd_size_t size = (block_device->*ATTRS[atr].method)();
            if (size >= (1ULL << 10 * i_ind)) {
                utest_printf("%s: %llu%sbytes (%llubytes)\n",
                             ATTRS[atr].name, size >> 10 * i_ind, prefixes[i_ind], size);
                break;
            }
        }
    }

    rtos::Thread bd_thread[TEST_NUM_OF_THREADS];

    osStatus threadStatus;
    int i_ind;

    for (i_ind = 0; i_ind < TEST_NUM_OF_THREADS; i_ind++) {
        threadStatus = bd_thread[i_ind].start(callback(test_thread_job, (void *)block_device));
        if (threadStatus != 0) {
            utest_printf("Thread %d Start Failed!\n", i_ind + 1);
        }
    }

    for (i_ind = 0; i_ind < TEST_NUM_OF_THREADS; i_ind++) {
        bd_thread[i_ind].join();
    }

    err = block_device->deinit();
    TEST_ASSERT_EQUAL(0, err);
}

void test_get_erase_value()
{
    utest_printf("\nTest BlockDevice::get_erase_value()..\n");

    // Test flow:
    //  1. Write data to selected region
    //    - Known starting point
    //  2. Erase selected region
    //  3. Read erased region and compare with get_erase_value()

    BlockDevice *block_device = BlockDevice::get_default_instance();
    TEST_SKIP_UNLESS_MESSAGE(block_device != NULL, "no block device found.");

    int err = block_device->init();
    TEST_ASSERT_EQUAL(0, err);

    // Check erase value
    int erase_value_int = block_device->get_erase_value();
    utest_printf("block_device->get_erase_value()=%d\n", erase_value_int);
    TEST_SKIP_UNLESS_MESSAGE(erase_value_int >= 0, "Erase not supported in this block device. Test skipped.");

    // Assuming that get_erase_value() returns byte value as documentation mentions
    // "If get_erase_value() returns a non-negative byte value" for unknown case.
    TEST_ASSERT(erase_value_int <= 255);
    uint8_t erase_value = (uint8_t)erase_value_int;

    // Determine data_buf_size
    bd_size_t erase_size = block_device->get_erase_size();
    TEST_ASSERT(erase_size > 0);
    bd_size_t data_buf_size = erase_size;

    // Determine start_address
    bd_addr_t start_address = rand();            // low 32 bytes
    start_address += (uint64_t)rand() << 32;     // high 32 bytes
    start_address %= block_device->size() - data_buf_size - erase_size; //  fit all data + alignment reserve
    start_address += erase_size;                 // add alignment reserve
    start_address -= start_address % erase_size; // align with erase_block
    utest_printf("start_address=0x%016" PRIx64 "\n", start_address);

    // Allocate buffer for read test data
    uint8_t *data_buf = (uint8_t *)malloc(data_buf_size);
    TEST_SKIP_UNLESS_MESSAGE(data_buf, "Not enough memory for test.\n");

    // Write random data to selected region to make sure data is not accidentally set to "erased" value.
    // With this pre-write, the test case will fail even if block_device->erase() is broken.
    for (bd_size_t i = 0; i < data_buf_size; i++) {
        data_buf[i] = (uint8_t) rand();
    }
    utest_printf("writing given memory region\n");
    err = block_device->program((const void *)data_buf, start_address, data_buf_size);
    TEST_ASSERT_EQUAL(0, err);

    // Erase given memory region
    utest_printf("erasing given memory region\n");
    err = block_device->erase(start_address, data_buf_size);
    TEST_ASSERT_EQUAL(0, err);

    // Read erased memory region
    utest_printf("reading erased memory region\n");
    err = block_device->read((void *)data_buf, start_address, data_buf_size);
    TEST_ASSERT_EQUAL(0, err);

    // Verify erased memory region
    utest_printf("verifying erased memory region\n");
    for (bd_size_t i = 0; i < data_buf_size; i++) {
        TEST_ASSERT_EQUAL(erase_value, data_buf[i]);
    }

    free(data_buf);

    // BlockDevice deinitialization
    err = block_device->deinit();
    TEST_ASSERT_EQUAL(0, err);
}

void test_contiguous_erase_write_read()
{
    utest_printf("\nTest Contiguous Erase/Program/Read Starts..\n");

    // Test flow:
    //  1. Erase whole test area
    //    - Tests contiguous erase
    //  2. Write smaller memory area
    //    - Tests contiguous sector writes
    //  3. Return step 2 for whole erase region

    BlockDevice *block_device = BlockDevice::get_default_instance();
    TEST_SKIP_UNLESS_MESSAGE(block_device != NULL, "no block device found.");

    // Initialize BlockDevice
    int err = block_device->init();
    TEST_ASSERT_EQUAL(0, err);

    // Test parameters
    bd_size_t erase_size = block_device->get_erase_size();
    TEST_ASSERT(erase_size > 0);
    bd_size_t program_size = block_device->get_program_size();
    TEST_ASSERT(program_size > 0);
    utest_printf("erase_size=%" PRId64 "\n", erase_size);
    utest_printf("program_size=%" PRId64 "\n", program_size);
    utest_printf("block_device->size()=%" PRId64 "\n", block_device->size());

    // Determine write/read buffer size
    //  start write_read_buf_size from 1% block_device->size()
    bd_size_t write_read_buf_size = block_device->size() / 100; // 1%, 10k=100, 100k=1k, 1MB=10k, 32MB=32k
    //  try to limit write_read_buf_size to 10k. If program_size*2 is larger than 10k, that will be used instead.
    if (write_read_buf_size > 10000) {
        write_read_buf_size = 10000;
    }
    //  2 program_size blocks is minimum for contiguous write/read test
    if (write_read_buf_size < program_size * 2) {
        write_read_buf_size = program_size * 2; // going over 10k
    }
    bd_size_t contiguous_write_read_blocks_per_region = write_read_buf_size /
                                                        program_size; // 2 is minimum to test contiguous write
    write_read_buf_size = contiguous_write_read_blocks_per_region * program_size;
    utest_printf("contiguous_write_read_blocks_per_region=%" PRIu64 "\n", contiguous_write_read_blocks_per_region);
    utest_printf("write_read_buf_size=%" PRIu64 "\n", write_read_buf_size);

    // Determine test region count
    int contiguous_write_read_regions = TEST_BLOCK_COUNT;
    utest_printf("contiguous_write_read_regions=%d\n", contiguous_write_read_regions);

    // Determine whole erase size
    bd_size_t contiguous_erase_size = write_read_buf_size * contiguous_write_read_regions;
    contiguous_erase_size -= contiguous_erase_size % erase_size; // aligned to erase_size
    contiguous_erase_size += erase_size; // but larger than write/read size * regions
    utest_printf("contiguous_erase_size=%" PRIu64 "\n", contiguous_erase_size);

    // Determine starting address
    bd_addr_t start_address = rand();            // low 32 bytes
    start_address += (uint64_t)rand() << 32;     // high 32 bytes
    start_address %= block_device->size() - contiguous_erase_size - erase_size; //  fit all data + alignment reserve
    start_address += erase_size;                 // add alignment reserve
    start_address -= start_address % erase_size; // align with erase_block
    bd_addr_t stop_address = start_address + write_read_buf_size * contiguous_write_read_regions;
    utest_printf("start_address=0x%016" PRIx64 "\n", start_address);
    utest_printf("stop_address=0x%016" PRIx64 "\n", stop_address);

    // Allocate write/read buffer
    uint8_t *write_read_buf = (uint8_t *)malloc(write_read_buf_size);
    if (write_read_buf == NULL) {
        block_device->deinit();
        TEST_SKIP_MESSAGE("not enough memory for test");
    }
    utest_printf("write_read_buf_size=%" PRIu64 "\n", (uint64_t)write_read_buf_size);

    // Pre-fill the to-be-erased region. By pre-filling the region,
    // we can be sure the test will not pass if the erase doesn't work.
    for (bd_size_t offset = 0; start_address + offset < stop_address; offset += write_read_buf_size) {
        for (size_t i = 0; i < write_read_buf_size; i++) {
            write_read_buf[i] = (uint8_t)rand();
        }
        utest_printf("pre-filling memory, from 0x%" PRIx64 " of size 0x%" PRIx64 "\n", start_address + offset,
                     write_read_buf_size);
        err = block_device->program((const void *)write_read_buf, start_address + offset, write_read_buf_size);
        TEST_ASSERT_EQUAL(0, err);
    }

    // Erase the whole region first
    utest_printf("erasing memory, from 0x%" PRIx64 " of size 0x%" PRIx64 "\n", start_address, contiguous_erase_size);
    err = block_device->erase(start_address, contiguous_erase_size);
    TEST_ASSERT_EQUAL(0, err);

    // Loop through all write/read regions
    int region = 0;
    for (; start_address < stop_address; start_address += write_read_buf_size) {
        utest_printf("\nregion #%d start_address=0x%016" PRIx64 "\n", region++, start_address);

        // Generate test data
        unsigned int seed = rand();
        utest_printf("generating test data, seed=%u\n", seed);
        srand(seed);
        for (size_t i = 0; i < write_read_buf_size; i++) {
            write_read_buf[i] = (uint8_t)rand();
        }

        // Write test data
        utest_printf("writing test data\n");
        err = block_device->program((const void *)write_read_buf, start_address, write_read_buf_size);
        TEST_ASSERT_EQUAL(0, err);

        // Read test data
        memset(write_read_buf, 0, (size_t)write_read_buf_size);
        utest_printf("reading test data\n");
        err = block_device->read(write_read_buf, start_address, write_read_buf_size);
        TEST_ASSERT_EQUAL(0, err);

        // Verify read data
        utest_printf("verifying test data\n");
        srand(seed);
        for (size_t i = 0; i < write_read_buf_size; i++) {
            uint8_t expected_value = (uint8_t)rand();
            if (write_read_buf[i] != expected_value) {
                utest_printf("data verify failed, write_read_buf[%d]=%" PRIu8 " and not %" PRIu8 "\n",
                             i, write_read_buf[i], expected_value);
            }
            TEST_ASSERT_EQUAL(write_read_buf[i], expected_value);
        }
        utest_printf("verify OK\n");
    }

    free(write_read_buf);

    // BlockDevice deinitialization
    err = block_device->deinit();
    TEST_ASSERT_EQUAL(0, err);
}

void test_program_read_small_data_sizes()
{
    utest_printf("\nTest program-read small data sizes, from 1 to 7 bytes..\n");

    BlockDevice *bd = BlockDevice::get_default_instance();

    TEST_SKIP_UNLESS_MESSAGE(bd != NULL, "no block device found.");

    int err = bd->init();
    TEST_ASSERT_EQUAL(0, err);

    bd_size_t erase_size = bd->get_erase_size();
    bd_size_t program_size = bd->get_program_size();
    bd_size_t read_size = bd->get_read_size();
    TEST_ASSERT(program_size > 0);

    err = bd->deinit();
    TEST_ASSERT_EQUAL(0, err);

    // See that we have enough memory for buffered block device
    char *dummy = new (std::nothrow) char[program_size + read_size];
    TEST_SKIP_UNLESS_MESSAGE(dummy, "Not enough memory for test.\n");
    delete[] dummy;

    // use BufferedBlockDevice for better handling of block devices program and read
    BufferedBlockDevice *block_device = new BufferedBlockDevice(bd);

    // BlockDevice initialization
    err = block_device->init();
    TEST_ASSERT_EQUAL(0, err);

    const char write_buffer[] = "1234567";
    char read_buffer[7] = {};

    // Determine starting address
    bd_addr_t start_address = 0;

    for (int i = 1; i <= 7; i++) {
        err = block_device->erase(start_address, erase_size);
        TEST_ASSERT_EQUAL(0, err);

        err = block_device->program((const void *)write_buffer, start_address, i);
        TEST_ASSERT_EQUAL(0, err);

        err = block_device->sync();
        TEST_ASSERT_EQUAL(0, err);

        err = block_device->read(read_buffer, start_address, i);
        TEST_ASSERT_EQUAL(0, err);

        err = memcmp(write_buffer, read_buffer, i);
        TEST_ASSERT_EQUAL(0, err);
    }

    // BlockDevice deinitialization
    err = block_device->deinit();
    TEST_ASSERT_EQUAL(0, err);

    delete block_device;
}

void test_get_type_functionality()
{
    BlockDevice *block_device = BlockDevice::get_default_instance();
    TEST_SKIP_UNLESS_MESSAGE(block_device, "No block device component is defined for this target");
    const char *bd_type = block_device->get_type();
    TEST_ASSERT_NOT_EQUAL(0, bd_type);

#if COMPONENT_QSPIF
    TEST_ASSERT_EQUAL(0, strcmp(bd_type, "QSPIF"));
#elif COMPONENT_SPIF
    TEST_ASSERT_EQUAL(0, strcmp(bd_type, "SPIF"));
#elif COMPONENT_DATAFLASH
    TEST_ASSERT_EQUAL(0, strcmp(bd_type, "DATAFLASH"));
#elif COMPONENT_SD
    TEST_ASSERT_EQUAL(0, strcmp(bd_type, "SD"));
#elif COMPONET_FLASHIAP
    TEST_ASSERT_EQUAL(0, strcmp(bd_type, "FLASHIAP"));
#endif

}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(60, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing read write random blocks", test_random_program_read_erase, greentea_failure_handler),
    Case("Testing multi threads erase program read", test_multi_threads, greentea_failure_handler),
    Case("Testing contiguous erase, write and read", test_contiguous_erase_write_read, greentea_failure_handler),
    Case("Testing BlockDevice::get_erase_value()", test_get_erase_value, greentea_failure_handler),
    Case("Testing program read small data sizes", test_program_read_small_data_sizes, greentea_failure_handler),
    Case("Testing get type functionality", test_get_type_functionality, greentea_failure_handler)
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
