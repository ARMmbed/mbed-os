#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "HeapBlockDevice.h"
#include "SlicingBlockDevice.h"
#include "ChainingBlockDevice.h"
#include "ClusteringBlockDevice.h"
#include "DividingBlockDevice.h"
#include <stdlib.h>

using namespace utest::v1;

#define BLOCK_SIZE 512
uint8_t write_block[BLOCK_SIZE];
uint8_t read_block[BLOCK_SIZE];


void test_slicing() {
    HeapBlockDevice bd(BLOCK_SIZE, 16);

    // Test with first slice of block device
    SlicingBlockDevice slice1(&bd, 0, 8);

    int err = slice1.init();
    TEST_ASSERT_EQUAL(0, err);

    TEST_ASSERT_EQUAL(BLOCK_SIZE, slice1.get_block_size());
    TEST_ASSERT_EQUAL(8, slice1.get_block_count());

    // Fill with random sequence
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        write_block[i] = 0xff & rand();
    }

    // Write, sync, and read the block
    err = slice1.write(write_block, 0, 1);
    TEST_ASSERT_EQUAL(1, err);

    err = slice1.sync();
    TEST_ASSERT_EQUAL(0, err);

    err = slice1.read(read_block, 0, 1);
    TEST_ASSERT_EQUAL(1, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    // Check with original block device
    err = bd.read(read_block, 0, 1);
    TEST_ASSERT_EQUAL(1, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    err = slice1.deinit();
    TEST_ASSERT_EQUAL(0, err);


    // Test with second slice of block device
    SlicingBlockDevice slice2(&bd, 8, 16);

    err = slice2.init();
    TEST_ASSERT_EQUAL(0, err);

    TEST_ASSERT_EQUAL(BLOCK_SIZE, slice2.get_block_size());
    TEST_ASSERT_EQUAL(8, slice2.get_block_count());

    // Fill with random sequence
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        write_block[i] = 0xff & rand();
    }

    // Write, sync, and read the block
    err = slice2.write(write_block, 0, 1);
    TEST_ASSERT_EQUAL(1, err);

    err = slice2.sync();
    TEST_ASSERT_EQUAL(0, err);

    err = slice2.read(read_block, 0, 1);
    TEST_ASSERT_EQUAL(1, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    // Check with original block device
    err = bd.read(read_block, 8, 1);
    TEST_ASSERT_EQUAL(1, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    err = slice2.deinit();
    TEST_ASSERT_EQUAL(0, err);
}

void test_chaining() {
    HeapBlockDevice bd1(BLOCK_SIZE, 8);
    HeapBlockDevice bd2(BLOCK_SIZE, 8);

    // Test with chain of block device
    BlockDevice *bds[] = {&bd1, &bd2};
    ChainingBlockDevice chain(bds);

    int err = chain.init();
    TEST_ASSERT_EQUAL(0, err);

    TEST_ASSERT_EQUAL(BLOCK_SIZE, chain.get_block_size());
    TEST_ASSERT_EQUAL(16, chain.get_block_count());

    // Fill with random sequence
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        write_block[i] = 0xff & rand();
    }

    // Write, sync, and read the block
    err = chain.write(write_block, 0, 1);
    TEST_ASSERT_EQUAL(1, err);

    err = chain.sync();
    TEST_ASSERT_EQUAL(0, err);

    err = chain.read(read_block, 0, 1);
    TEST_ASSERT_EQUAL(1, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    // Write, sync, and read the block
    err = chain.write(write_block, 8, 1);
    TEST_ASSERT_EQUAL(1, err);

    err = chain.sync();
    TEST_ASSERT_EQUAL(0, err);

    err = chain.read(read_block, 8, 1);
    TEST_ASSERT_EQUAL(1, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    err = chain.deinit();
    TEST_ASSERT_EQUAL(0, err);
}

void test_clustering() {
    HeapBlockDevice bd(BLOCK_SIZE/2, 32);

    // Test with clustering block device
    ClusteringBlockDevice cluster(&bd, BLOCK_SIZE);

    int err = cluster.init();
    TEST_ASSERT_EQUAL(0, err);

    TEST_ASSERT_EQUAL(BLOCK_SIZE, cluster.get_block_size());
    TEST_ASSERT_EQUAL(16, cluster.get_block_count());

    // Fill with random sequence
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        write_block[i] = 0xff & rand();
    }

    // Write, sync, and read the block
    err = cluster.write(write_block, 0, 1);
    TEST_ASSERT_EQUAL(1, err);

    err = cluster.sync();
    TEST_ASSERT_EQUAL(0, err);

    err = cluster.read(read_block, 0, 1);
    TEST_ASSERT_EQUAL(1, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    err = cluster.deinit();
    TEST_ASSERT_EQUAL(0, err);
}

void test_dividing() {
    HeapBlockDevice bd(BLOCK_SIZE*2, 8);

    // Test with dividing block device
    DividingBlockDevice divide(&bd, BLOCK_SIZE);

    int err = divide.init();
    TEST_ASSERT_EQUAL(0, err);

    TEST_ASSERT_EQUAL(BLOCK_SIZE, divide.get_block_size());
    TEST_ASSERT_EQUAL(16, divide.get_block_count());

    // Fill with random sequence
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        write_block[i] = 0xff & rand();
    }

    // Write, sync, and read the block
    err = divide.write(write_block, 0, 1);
    TEST_ASSERT_EQUAL(1, err);

    err = divide.sync();
    TEST_ASSERT_EQUAL(0, err);

    err = divide.read(read_block, 0, 1);
    TEST_ASSERT_EQUAL(1, err);

    // Check that the data was unmodified
    srand(1);
    for (int i = 0; i < BLOCK_SIZE; i++) {
        TEST_ASSERT_EQUAL(0xff & rand(), read_block[i]);
    }

    err = divide.deinit();
    TEST_ASSERT_EQUAL(0, err);
}


// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(10, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Testing slicing of a block device", test_slicing),
    Case("Testing chaining of block devices", test_chaining),
    Case("Testing clustering a block device", test_clustering),
    Case("Testing dividing a block device",   test_dividing),
};

Specification specification(test_setup, cases);

int main() {
    return !Harness::run(specification);
}
