/*
 * Copyright (c) 2019-2020, Arm Limited and affiliates.
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

#if !USB_DEVICE_TESTS
#error [NOT_SUPPORTED] usb device tests not enabled
#else

#if !defined(MBED_CONF_RTOS_PRESENT)
#error [NOT_SUPPORTED] USB stack and test cases require RTOS to run.
#else

#include <stdio.h>
#include <string.h>
#include <stdlib.h>     /* srand, rand */
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"

#include "mbed.h"
#include "USBMSD.h"
#include "TestUSBMSD.h"

#include "HeapBlockDevice.h"
#include "FATFileSystem.h"

// TARGET_NANO100 SRAM 16KB can't afford mass-storage-disk test, so skip usb_msd_test.
#if !defined(DEVICE_USBDEVICE) || !DEVICE_USBDEVICE || TARGET_NANO100
#error [NOT_SUPPORTED] USB Device not supported for this target
#else


#define OS_WINDOWS  1
#define OS_LINUX    2
#define OS_MAC      3

// Host side unmount was disabled for windows machines.
// PowerShell execution policies/restrictions cause that
// on some windows machines unmount is failing
// To re-enable it comment out below line.
#define DISABLE_HOST_SIDE_UMOUNT

#ifdef MIN
#undef MIN
#endif
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

#define DEFAULT_BLOCK_SIZE  512
#define HEAP_BLOCK_DEVICE_SIZE (128 * DEFAULT_BLOCK_SIZE)
#define MIN_HEAP_SIZE (HEAP_BLOCK_DEVICE_SIZE + 6144)


/* TODO:
 *
 * Test if slave(DUT) can force host to refresh mounted fs. (not supported in USBMSD yet)
 *
 */

#define TEST_DIR    "usb_msd_test_data"
#define TEST_FILE   "usb_msd_test_file"
#define TEST_STRING "usb_msd_test_string"

using namespace utest::v1;

uint32_t prev_read_counter = 0;
uint32_t prev_program_counter = 0;
extern uint32_t mbed_heap_size;

static char _key[256] = { 0 };
static char _value[128] = { 0 };

static volatile bool msd_process_done = false;


FATFileSystem heap_fs("heap_fs");

Semaphore media_remove_event(0, 1);


/** Creates heap block device
 *
 */
BlockDevice *get_heap_block_device()
{
    // create 64kB heap block device
    if (mbed_heap_size >= MIN_HEAP_SIZE) {
        static HeapBlockDevice bd(128 * DEFAULT_BLOCK_SIZE, DEFAULT_BLOCK_SIZE);
        bd.init();
        return &bd;
    } else {
        return NULL;
    }
}

uint64_t get_fs_mount_size(FileSystem *fs)
{
    struct statvfs stat;
    fs->statvfs(fs->getName(), &stat);
    uint64_t size = stat.f_bsize * stat.f_blocks;
    return size;
}

/**
 * Create test data
 *
 * @param fs_root filesystem path
 */
static bool test_files_create(const char *fs_root, const char *test_file = TEST_FILE, const char *test_string = TEST_STRING)
{
    char path[128];
    sprintf(path, "/%s/%s", fs_root, TEST_DIR);
    int ret = mkdir(path, 0777);
    if (ret != 0 && errno != EEXIST) {
        utest_printf("mkdir failed!!! errno: %d\n", errno);
        return false;
    }
    sprintf(path, "/%s/%s/%s", fs_root, TEST_DIR, test_file);
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        utest_printf("fopen failed!!! errno: %d\n", errno);
        return false;
    }
    fprintf(f, test_string);
    fflush(f);
    fclose(f);
    return true;
}

/**
 * Remove test data
 *
 * @param fs_root filesystem path
 */
static void test_files_remove(const char *fs_root)
{
    DIR *dir;
    struct dirent *dp;
    char path[512];

    sprintf(path, "/%s/%s", fs_root, TEST_DIR);
    dir = opendir(path);
    if (dir == NULL) {
        return;
    }
    while ((dp = readdir(dir)) != NULL) {
        sprintf(path, "/%s/%s/%s", fs_root, TEST_DIR, dp->d_name);
        remove(path);
    }
    sprintf(path, "/%s/%s", fs_root, TEST_DIR);
    remove(path);
}

/**
 * Check if test data exist
 *
 * @param fs_root filesystem path
 * @return true if data exist
 */
static bool test_files_exist(const char *fs_root, const char *test_file = TEST_FILE, const char *test_string = TEST_STRING)
{
    char path[128];
    char str[512] = { 0 };
    sprintf(path, "/%s/%s/%s", fs_root, TEST_DIR, test_file);
    FILE *f = fopen(path, "r");
    if (f != NULL) {
        fscanf(f, "%s", str);
        if (strcmp(test_string, str) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * Mounts a filesystem to a block device
 *
 * @param bd block device
 * @param fs filesystem
 * @return true if success, false otherwise
 */
static bool prepare_storage(BlockDevice *bd, FileSystem *fs)
{
    const char *fs_root = fs->getName();
    int err = fs->mount(bd);
    if (err) {
        utest_printf("%s filesystem mount failed\ntry to reformat device... ", fs->getName());
        err = fs->reformat(bd);
        if (err) {
            utest_printf("failed !!!\n");
            return false;
        } else {
            utest_printf("succeed\n");
        }
    }
    // remove old test data
    test_files_remove(fs_root);

    return true;
}

void run_processing(Semaphore *sem)
{
    sem->release();
}

void msd_process(USBMSD *msd)
{
    Semaphore proc;
    msd->attach(callback(run_processing, &proc));
    while (!msd_process_done) {
        proc.try_acquire_for(100);
        msd->process();
        if (msd->media_removed()) {
            media_remove_event.release();
        }
    }
    msd->attach(NULL);
}


// wait until msd negotiation is done (no r/w disk operation for at least 1s)
// max wait time is 15s
#define WAIT_MSD_COMMUNICATION_DONE() \
    for (int x = 0; x < 15; x++) { \
        prev_read_counter = usb.get_read_counter();\
        prev_program_counter = usb.get_program_counter();\
        ThisThread::sleep_for(1000);\
        if ((usb.get_read_counter() == prev_read_counter) && \
            (usb.get_program_counter() == prev_program_counter)) {\
            break;\
        }\
    }


#define TEST_ASSERT_EQUAL_STRING_LOOP(expected, actual, loop_index)                             \
        if (strcmp(expected, actual) != 0) {                                                    \
            char str[128];                                                                      \
            sprintf(str, "expected %s was %s (loop index: %lu)", expected, actual, loop_index); \
            TEST_ASSERT_MESSAGE(false, str);                                                    \
        }

#define TEST_ASSERT_EQUAL_LOOP(expected, actual, loop_index)                                    \
        if (expected != actual) {                                                               \
            char str[128];                                                                      \
            sprintf(str, "expected %d was %d (loop index: %lu)", expected, actual, loop_index); \
            TEST_ASSERT_MESSAGE(false, str);                                                    \
        }


/** Initialize storages
 *
 * Given the DUT USB mass storage device
 * When DUT has enought heap memory
 * Then initialize heap block device for tests
 * When DUT has any falsh block device
 * Then initialize it for tests
 */
void storage_init()
{
    TEST_ASSERT_MESSAGE(mbed_heap_size >= MIN_HEAP_SIZE, "Not enough heap memory for HeapBlockDevice creation");
    FATFileSystem::format(get_heap_block_device());
    bool result = prepare_storage(get_heap_block_device(), &heap_fs);
    TEST_ASSERT_MESSAGE(result, "heap storage initialisation failed");
}

/** Test mass storage device mount and unmount
 *
 * Given the DUT USB mass storage device connected to the host
 * When DUT call @USBMSD::connect
 * Then host detects mass storage device is mounted as removable disk drive and it reports valid filesystem size
 * When DUT call @USBMSD::disconnect
 * Then host detects mass storage device is unmounted (ejected)
 *
 * Given the DUT USB mass storage device connected to the host and mounted
 * When host unmounts (ejects) mass storage device
 * Then DUT detects media remove event
 */
template <uint32_t N>
void mount_unmount_test(BlockDevice *bd, FileSystem *fs)
{
    Thread msd_thread(osPriorityHigh);
    TestUSBMSD usb(bd, false);
    msd_process_done = false;
    msd_thread.start(callback(msd_process, &usb));

    for (uint32_t i = 1; i <= N; i++) {
        // mount
        usb.connect();
        WAIT_MSD_COMMUNICATION_DONE();
        // check if device is mounted on host side
        greentea_send_kv("check_if_mounted", 0);
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));

        TEST_ASSERT_EQUAL_STRING_LOOP("passed", _key, i);

        greentea_send_kv("get_mounted_fs_size", 0);
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        uint64_t ret_size = atoll(_key);
        TEST_ASSERT_EQUAL_UINT64(get_fs_mount_size(fs), ret_size);

        // unmount
        usb.disconnect();
        // check if device is detached on host side
        greentea_send_kv("check_if_not_mounted", 0);
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING_LOOP("passed", _key, i);
    }

    for (uint32_t i = 1; i <= N; i++) {
        // mount
        usb.connect();
        WAIT_MSD_COMMUNICATION_DONE();
        // check if device is mounted on host side
        greentea_send_kv("check_if_mounted", 0);
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING_LOOP("passed", _key, i);

        greentea_send_kv("get_mounted_fs_size", 0);
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        uint64_t ret_size = atoll(_key);
        TEST_ASSERT_EQUAL_UINT64(get_fs_mount_size(fs), ret_size);

#ifdef DISABLE_HOST_SIDE_UMOUNT
        greentea_send_kv("get_os_type", 0);
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        int32_t os_type = atoi(_value);
        if (os_type != OS_WINDOWS) {
#endif
            // unmount msd device on host side
            greentea_send_kv("unmount", 0);
            greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
            TEST_ASSERT_EQUAL_STRING_LOOP("passed", _key, i);

            // wait for unmount event (set 10s timeout)
            media_remove_event.try_acquire_for(10000);
            if (!usb.media_removed()) {
                TEST_ASSERT_EQUAL_LOOP(true, usb.media_removed(), i);
            }

            // unmount since media_removed doesn't disconnects device side
            usb.disconnect();
#ifdef DISABLE_HOST_SIDE_UMOUNT
        } else {
            // unmount
            usb.disconnect();
        }
#endif
        // check if device is detached on host side
        greentea_send_kv("check_if_not_mounted", 0);
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
        TEST_ASSERT_EQUAL_STRING_LOOP("passed", _key, i);
    }

    // mount
    usb.connect();
    WAIT_MSD_COMMUNICATION_DONE();
    // check if device is mounted on host side
    greentea_send_kv("check_if_mounted", 0);
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    TEST_ASSERT_EQUAL_STRING("passed", _key);

    // unmount
    usb.disconnect();
    // check if device is detached on host side
    greentea_send_kv("check_if_not_mounted", 0);
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    TEST_ASSERT_EQUAL_STRING("passed", _key);

    msd_process_done = true; // terminate msd_thread
    msd_thread.join();
}

/** Test mass storage device mount and unmount together with underlying file system operations
 *
 * Given the DUT USB mass storage device connected to the host
 * When DUT call @USBMSD::connect
 * Then host detects that mass storage device is mounted as removable disk drive and test files are present
 * When DUT call @USBMSD::disconnect
 * Then host detects mass storage device is unmounted (ejected)
 *
 * Given the DUT USB mass storage device connected to the host and already mounted
 * When host unmounts (ejects) mass storage device
 * Then DUT detects media remove event
 */
void mount_unmount_and_data_test(BlockDevice *bd, FileSystem *fs)
{
    const char *fs_root = fs->getName();
    Thread msd_thread(osPriorityHigh);
    TestUSBMSD usb(bd, false);
    msd_process_done = false;
    msd_thread.start(callback(msd_process, &usb));

    // mount
    usb.connect();
    WAIT_MSD_COMMUNICATION_DONE();
    // check if device is mounted on host side
    greentea_send_kv("check_if_mounted", 0);
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    TEST_ASSERT_EQUAL_STRING("passed", _key);

    greentea_send_kv("check_file_exist", TEST_DIR " " TEST_FILE " " TEST_STRING);
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    TEST_ASSERT_EQUAL_STRING("non-exist", _key);

    usb.disconnect();
    // check if device is detached on host side
    greentea_send_kv("check_if_not_mounted", 0);
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    TEST_ASSERT_EQUAL_STRING("passed", _key);

    test_files_create(fs_root);
    TEST_ASSERT(test_files_exist(fs_root));

    usb.connect();
    WAIT_MSD_COMMUNICATION_DONE();
    // check if device is mounted on host side
    greentea_send_kv("check_if_mounted", 0);
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    TEST_ASSERT_EQUAL_STRING("passed", _key);

    greentea_send_kv("check_file_exist", TEST_DIR " " TEST_FILE " " TEST_STRING);
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    TEST_ASSERT_EQUAL_STRING("exist", _key);

    greentea_send_kv("delete_files", TEST_DIR " " TEST_FILE);
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    TEST_ASSERT_EQUAL_STRING("passed", _key);

    do {
        ThisThread::sleep_for(1);
    } while (test_files_exist(fs_root));
    TEST_ASSERT_EQUAL(false, test_files_exist(fs_root));

    usb.disconnect();
    // check if device is detached on host side
    greentea_send_kv("check_if_not_mounted", 0);
    greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));
    TEST_ASSERT_EQUAL_STRING("passed", _key);

    msd_process_done = true;    // terminate msd_thread
    msd_thread.join();
    test_files_remove(fs_root);
}

void heap_block_device_mount_unmount_test()
{
    TEST_ASSERT_MESSAGE(mbed_heap_size >= MIN_HEAP_SIZE, "Not enough heap memory for HeapBlockDevice creation");
    mount_unmount_test<3>(get_heap_block_device(), &heap_fs);
}

void heap_block_device_mount_unmount_and_data_test()
{
    TEST_ASSERT_MESSAGE(mbed_heap_size >= MIN_HEAP_SIZE, "Not enough heap memory for HeapBlockDevice creation");
    mount_unmount_and_data_test(get_heap_block_device(), &heap_fs);
}


Case cases[] = {
    Case("storage initialization", storage_init),

    Case("mount/unmount test - Heap block device", heap_block_device_mount_unmount_test),
    Case("mount/unmount and data test - Heap block device", heap_block_device_mount_unmount_and_data_test),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(300, "pyusb_msd");
    utest::v1::status_t status = greentea_test_setup_handler(number_of_cases);
    TestUSBMSD::setup_serial_number();
    return status;
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}

#endif // !defined(DEVICE_USBDEVICE) || !DEVICE_USBDEVICE
#endif // !defined(MBED_CONF_RTOS_PRESENT)
#endif // !defined(USB_DEVICE_TESTS)
