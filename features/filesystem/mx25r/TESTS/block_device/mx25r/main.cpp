#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "MX25RBlockDevice.h"
#include <stdlib.h>

using namespace utest::v1;

#ifndef MX25R_INSTALLED
#define MX25R_INSTALLED defined(TARGET_K82F)
#endif

#if !MX25R_INSTALLED
#error [NOT_SUPPORTED] MX25R Required
#endif

#define BLOCK_SIZE 4096
uint8_t write_block[BLOCK_SIZE];
uint8_t read_block[BLOCK_SIZE];


void test_read_write() {
    MX25RBlockDevice bd(PTE2, PTE4, PTE1, PTE5);

    int err = bd.init();
    TEST_ASSERT_EQUAL(0, err);

    printf("read size: %llu\r\n", bd.read_size());
    printf("program size: %llu\r\n", bd.program_size());
    printf("erase size: %llu\r\n", bd.erase_size());

    // Fill with random sequence
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        write_block[i] = 0xff & rand();
    }

    // Write, sync, and read the block
    err = bd.write(write_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    err = bd.read(read_block, 0, BLOCK_SIZE);
    TEST_ASSERT_EQUAL(0, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }
    
    err = bd.deinit();
    TEST_ASSERT_EQUAL(0, err);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing read write of a block", test_read_write),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
