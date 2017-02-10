#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "SPIFBlockDevice.h"
#include <stdlib.h>

using namespace utest::v1;

#ifndef SPIF_INSTALLED
#define SPIF_INSTALLED defined(TARGET_K82F)
#endif

//#if !SPIF_INSTALLED
//#error [NOT_SUPPORTED] SPIF Required
//#endif

#define BLOCK_SIZE 4096
uint8_t write_block[BLOCK_SIZE];
uint8_t read_block[BLOCK_SIZE];


void test_read_write() {
    //SPIFBlockDevice bd(PTE2, PTE4, PTE1, PTE5);
    SPIFBlockDevice bd(D11, D12, D13, D10);

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
    printf("w (0x%02x%02x%02x%02x)\n",
        write_block[0], write_block[1], write_block[2], write_block[3],
        write_block[4], write_block[5], write_block[6], write_block[7]);
    printf("r (0x%02x%02x%02x%02x)\n",
        read_block[0], read_block[1], read_block[2], read_block[3],
        read_block[4], read_block[5], read_block[6], read_block[7]);

    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }
    
    err = bd.deinit();
    TEST_ASSERT_EQUAL(0, err);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(10*1000*1000, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing read write of a block", test_read_write),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
