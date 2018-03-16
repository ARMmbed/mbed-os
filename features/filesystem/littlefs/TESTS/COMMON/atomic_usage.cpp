/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


/**
 * This file contains code which performs various atomic operations using
 * littlefs. It is intended for use in tests and test applications to
 * validate that the defined behavior below is being met.
 *
 * # Defined behavior
 * - A file rename is atomic (Note - rename can be used to replace a file)
 *      - Atomic file rename tested by setup/perform/check_file_rename
 *      - Atomic file replace tested by setup/perform/check_file_rename_replace
 * - A directory rename is atomic (Note - rename can be used to replace an empty directory)
 *      - Tested by setup/perform/check_directory_rename
 * - Directory create is atomic
 * - Directory delete is atomic
 * - File create is atomic
 * - File delete is atomic
 * - File contents are atomically written on close
 *      - Tested by setup/perform/check_file_change_contents
 */


#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include <stdlib.h>
#include <errno.h>

#include "ObservingBlockDevice.h"
#include "ExhaustibleBlockDevice.h"
#include "FileSystem.h"

#include "atomic_usage.h"

// test configuration
#ifndef MBED_TEST_FILESYSTEM
#define MBED_TEST_FILESYSTEM LittleFileSystem
#endif

#ifndef MBED_TEST_FILESYSTEM_DECL
#define MBED_TEST_FILESYSTEM_DECL MBED_TEST_FILESYSTEM fs("fs")
#endif


// declarations
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define INCLUDE(x) STRINGIZE(x.h)

#include INCLUDE(MBED_TEST_FILESYSTEM)


#define DEBUG(...)
#define DEBUG_CHECK(...)
#define BUFFER_SIZE             64
// Version is written to a file and is used
// to determine if a reformat is required
#define ATOMIC_USAGE_VERSION    1

#define ARRAY_LENGTH(array)     (sizeof(array) / sizeof(array[0]))

#define TEST_ASSERT_OR_EXIT(condition)              \
        TEST_ASSERT(condition); if (!(condition)) {error("Assert failed");}

#define TEST_ASSERT_EQUAL_OR_EXIT(expected, actual) \
        TEST_ASSERT_EQUAL(expected, actual); if ((int64_t)(expected) != (int64_t)(actual)) {error("Assert failed");}

using namespace utest::v1;

typedef void (*test_function_t)(FileSystem *fs);
typedef bool (*test_function_bool_t)(FileSystem *fs);

struct TestEntry {
    const char *name;
    test_function_t setup;
    test_function_bool_t perform;
    test_function_t check;
};

/**
 * Write data to the file while checking for error conditions
 *
 * @param file File to write to
 * @param data Data to write
 * @param size Size of data to write
 * @return true if flash has been exhausted, false otherwise
 */
static bool file_write(File *file, uint8_t *data, uint32_t size)
{
    int res = file->write(data, size);
    if (-ENOSPC  == res) {
        return true;
    }
    TEST_ASSERT_EQUAL_OR_EXIT(size, res);
    return false;
}

/**
 * Write padding data of the given size
 *
 * @param file Pointer to the file to write to
 * @param padding Value to pad
 * @param size Size to pad
 * @return true if flash has been exhausted, false otherwise
 */
static bool file_pad(File *file, char padding, uint32_t size)
{
    uint8_t buf[BUFFER_SIZE];
    memset(buf, padding, sizeof(buf));

    while (size > 0) {
        uint32_t write_size = sizeof(buf) <= size ? sizeof(buf) : size;
        if (file_write(file, buf, write_size)) {
            return true;
        }
        size -= write_size;
    }

    return false;
}

/*
 * Similar to fscanf but uses and mbed file
 *
 * @param file File to scan from
 * @param format Format string of values to read
 * @return the number of arguments read
 */
static int file_scanf(File *file, const char *format, ...)
{
    uint8_t buf[BUFFER_SIZE];
    va_list args;
    memset(buf, 0, sizeof(buf));

    int res = file->read(buf, sizeof(buf) - 1);
    TEST_ASSERT_OR_EXIT(res >= 0);

    va_start (args, format);
    int count = vsscanf((char*)buf, format, args);
    va_end (args);
    TEST_ASSERT_OR_EXIT(count >= 0);

    return count;
}

/*
 * Similar to fprintf but uses and mbed file
 *
 * @param file File to print to
 * @param format Format string of values to write
 * @return size written to file or -1 on out of space
 */
static int file_printf(File *file, const char *format, ...)
{
    uint8_t buf[BUFFER_SIZE];
    va_list args;
    va_start (args, format);
    int size = vsprintf((char*)buf, format, args);
    va_end (args);
    TEST_ASSERT_OR_EXIT((size >= 0) && (size <= (int)sizeof(buf)));

    if (file_write(file, buf, size)) {
        return -1;
    }

    return size;
}


static const char FILE_RENAME_A[] = "file_to_rename_a.txt";
static const char FILE_RENAME_B[] = "file_to_rename_b.txt";
static const char FILE_RENAME_CONTENTS[] = "Test contents for the file to be renamed";
static const int FILE_RENAME_LEN = strlen(FILE_RENAME_CONTENTS);

/**
 * Setup for the file rename test
 *
 * Create file FILE_RENAME_A with contents FILE_RENAME_CONTENTS.
 */
static void setup_file_rename(FileSystem *fs)
{
    DEBUG("setup_file_rename()\n");

    File file;

    int res = file.open(fs, FILE_RENAME_A, O_WRONLY | O_CREAT);
    DEBUG("  open result %i\n", res);
    TEST_ASSERT_EQUAL_OR_EXIT(0, res);

    res = file.write(FILE_RENAME_CONTENTS, FILE_RENAME_LEN);
    DEBUG("  write result %i\n", res);
    TEST_ASSERT_EQUAL_OR_EXIT(FILE_RENAME_LEN, res);
}

/**
 * Change the file name to either FILE_RENAME_A or FILE_RENAME_B
 */
static bool perform_file_rename(FileSystem *fs)
{
    DEBUG("perform_file_rename()\n");

    struct stat st;
    int res = fs->stat(FILE_RENAME_A, &st);
    const char *src = (res == 0) ? FILE_RENAME_A : FILE_RENAME_B;
    const char *dst = (res == 0) ? FILE_RENAME_B : FILE_RENAME_A;

    DEBUG("  stat result  %i\n", res);
    TEST_ASSERT_OR_EXIT((res == -ENOENT) || (res == 0));

    DEBUG("  Renaming %s to %s\n", src, dst);
    res = fs->rename(src, dst);
    if (-ENOSPC  == res) {
        return true;
    }
    TEST_ASSERT_EQUAL_OR_EXIT(0, res);
    return false;
}

/**
 * Check that the file rename is in a good state
 *
 * Check that there is only one file and that file contains the correct
 * contents.
 *
 * Allowed states:
 * - File FILE_RENAME_A exists with contents and FILE_RENAME_B does not
 * - File FILE_RENAME_B exists with contents and FILE_RENAME_A does not
 *
 */
static void check_file_rename(FileSystem *fs)
{

    int files = 0;
    int valids = 0;
    const char * const filenames[] = {FILE_RENAME_A, FILE_RENAME_B};

    for (int i = 0; i < 2; i++) {
        File file;
        if (file.open(fs, filenames[i], O_RDONLY) == 0) {
            uint8_t buf[BUFFER_SIZE];
            files++;
            memset(buf, 0, sizeof(buf));
            int res = file.read(buf, FILE_RENAME_LEN);
            if (res != FILE_RENAME_LEN) {
                break;
            }
            if (memcmp(buf, FILE_RENAME_CONTENTS, FILE_RENAME_LEN) != 0) {
                break;
            }
            valids++;
        }
    }

    TEST_ASSERT_EQUAL_OR_EXIT(1, files);
    TEST_ASSERT_EQUAL_OR_EXIT(1, valids);
}


static const char  FILE_RENAME_REPLACE[] = "rename_replace_file.txt";
static const char  FILE_RENAME_REPLACE_NEW[] = "new_rename_replace_file.txt";
static const char  FILE_RENAME_REPLACE_FMT[] = "file replace count: %lu\n";

/**
 * Create the file FILE_RENAME_REPLACE with initial contents
 *
 * Create an write an initial count of 0 to the file.
 */
static void setup_file_rename_replace(FileSystem *fs)
{
    DEBUG("setup_file_rename_replace()\n");
    File file;

    // Write out initial count

    int res = file.open(fs, FILE_RENAME_REPLACE, O_WRONLY | O_CREAT);
    TEST_ASSERT_EQUAL_OR_EXIT(0, res);

    uint32_t count = 0;
    uint8_t buf[BUFFER_SIZE];
    memset(buf, 0, sizeof(buf));
    const int length = sprintf((char*)buf, FILE_RENAME_REPLACE_FMT, count);
    TEST_ASSERT_OR_EXIT(length > 0);

    res = file.write(buf, length);
    DEBUG("  write result %i\n", res);
    TEST_ASSERT_EQUAL_OR_EXIT(length, res);
}

/**
 * Atomically increment the count in FILE_RENAME_REPLACE using a rename
 */
bool perform_file_rename_replace(FileSystem *fs)
{
    DEBUG("perform_file_rename_replace()\n");
    File file;

    // Read in previous count

    int res = file.open(fs, FILE_RENAME_REPLACE, O_RDONLY);
    TEST_ASSERT_EQUAL_OR_EXIT(0, res);

    uint64_t count;
    int args_read = file_scanf(&file, FILE_RENAME_REPLACE_FMT, &count);
    TEST_ASSERT_EQUAL_OR_EXIT(1, args_read);

    res = file.close();
    if (-ENOSPC  == res) {
        return true;
    }
    TEST_ASSERT_EQUAL(0, res);

    // Write out new count

    count++;

    res = file.open(fs, FILE_RENAME_REPLACE_NEW, O_WRONLY | O_CREAT);
    if (-ENOSPC  == res) {
        return true;
    }
    TEST_ASSERT_EQUAL_OR_EXIT(0, res);

    if (file_printf(&file, FILE_RENAME_REPLACE_FMT, count) <= 0) {
        return true;
    }

    res = file.close();
    if (-ENOSPC  == res) {
        return true;
    }
    TEST_ASSERT_EQUAL(0, res);

    // Rename file

    res = fs->rename(FILE_RENAME_REPLACE_NEW, FILE_RENAME_REPLACE);
    if (-ENOSPC  == res) {
        return true;
    }
    TEST_ASSERT_EQUAL_OR_EXIT(0, res);
    DEBUG("  count %llu -> %llu\n", count - 1, count);

    return false;
}

/**
 * Check that FILE_RENAME_REPLACE always has a valid count
 *
 * Allowed states:
 * - FILE_RENAME_REPLACE exists with valid contents
 */
static void check_file_rename_replace(FileSystem *fs)
{
    DEBUG_CHECK("check_file_rename_replace()\n");
    File file;

    // Read in previous count

    int res = file.open(fs, FILE_RENAME_REPLACE, O_RDONLY);
    TEST_ASSERT_EQUAL_OR_EXIT(0, res);

    uint64_t count;
    int args_read = file_scanf(&file, FILE_RENAME_REPLACE_FMT, &count);
    TEST_ASSERT_EQUAL_OR_EXIT(1, args_read);
    DEBUG_CHECK("  count %llu\n", count);
}


static const char DIRECTORY_RENAME_A[] = "dir_a";
static const char DIRECTORY_RENAME_B[] = "dir_b";

/**
 * Create DIRECTORY_RENAME_A with initial contents
 */
static void setup_directory_rename(FileSystem *fs)
{
    DEBUG("setup_directory_rename()\n");

    int res = fs->mkdir(DIRECTORY_RENAME_A, 0777);
    TEST_ASSERT_EQUAL_OR_EXIT(0, res);
}

/*
 * Change the directory name from either DIRECTORY_RENAME_A or DIRECTORY_RENAME_B to the other
 */
static bool perform_directory_rename(FileSystem *fs)
{
    DEBUG("perform_directory_rename()\n");

    struct stat st;
    int res = fs->stat(DIRECTORY_RENAME_A, &st);
    const char *src = (res == 0) ? DIRECTORY_RENAME_A : DIRECTORY_RENAME_B;
    const char *dst = (res == 0) ? DIRECTORY_RENAME_B : DIRECTORY_RENAME_A;

    DEBUG("  stat result  %i\n", res);
    TEST_ASSERT_OR_EXIT((res == -ENOENT) || (res == 0));

    DEBUG("  Renaming %s to %s\n", src, dst);
    res = fs->rename(src, dst);
    if (-ENOSPC  == res) {
        return true;
    }
    TEST_ASSERT_EQUAL_OR_EXIT(0, res);
    return false;
}

/*
 * Change the directory name from either DIRECTORY_RENAME_A or DIRECTORY_RENAME_B to the other
 *
 * Allowed states:
 * - DIRECTORY_RENAME_A exists with valid contents and DIRECTORY_RENAME_B does not exist
 * - DIRECTORY_RENAME_B exists with valid contents and DIRECTORY_RENAME_A does not exist
 */
static void check_directory_rename(FileSystem *fs)
{
    DEBUG_CHECK("check_directory_rename()\n");

    static const char *directory_names[] = {
        DIRECTORY_RENAME_A,
        DIRECTORY_RENAME_B
    };

    size_t directories = 0;
    for (size_t i = 0; i < ARRAY_LENGTH(directory_names); i++) {
        Dir dir;
        int res = dir.open(fs, directory_names[i]);
        TEST_ASSERT_OR_EXIT((res == -ENOENT) || (res == 0));
        if (res == 0) {
            directories++;
        }
    }
    TEST_ASSERT_EQUAL_OR_EXIT(1, directories);
}


static const char CHANGE_CONTENTS_NAME[] = "file_changing_contents.txt";
static const char CHANGE_CONTENTS_FILL = ' ';
static const uint32_t BLOCK_SIZE = 512;

/**
 * Create file CHANGE_CONTENTS_NAME with initial contents
 *
 * File contains three blocks of data each which start
 * with a count.
 */
static void setup_file_change_contents(FileSystem *fs)
{
    DEBUG("setup_file_change_contents()\n");

    File file;
    int res = file.open(fs, CHANGE_CONTENTS_NAME, O_WRONLY | O_CREAT);
    TEST_ASSERT_EQUAL_OR_EXIT(0, res);

    for (int count = 1; count <= 3; count++) {
        int size = file_printf(&file, "%lu\n", count);
        TEST_ASSERT_OR_EXIT(size >= 0);

        bool dead = file_pad(&file, CHANGE_CONTENTS_FILL, BLOCK_SIZE - size);
        TEST_ASSERT_EQUAL_OR_EXIT(false, dead);
    }
}

/**
 * Atomically increment the counts in the file CHANGE_CONTENTS_NAME
 *
 * Read in the current counts, increment them and then write them
 * back in non-sequential order.
 */
static bool perform_file_change_contents(FileSystem *fs)
{
    DEBUG("perform_file_change_contents()\n");
    File file;

    int res = file.open(fs, CHANGE_CONTENTS_NAME,  O_RDWR);
    TEST_ASSERT_EQUAL_OR_EXIT(0, res);

    // Read in values
    uint32_t values[3];
    for (int i = 0; i < 3; i++) {
        file.seek(i * BLOCK_SIZE);
        int args_read = file_scanf(&file, "%lu\n", &values[i]);
        TEST_ASSERT_EQUAL_OR_EXIT(1, args_read);
    }

    // Increment values
    for (int i = 0; i < 3; i++) {
        values[i]++;
    }

    // Write values out of order
    int i;
    i = 0;
    file.seek(i * BLOCK_SIZE);
    if (file_printf(&file, "%lu\n", values[i]) <= 0) {
        return true;
    }
    DEBUG("    value[%i]: %lu -> %lu\n", i, values[i] - 1, values[i]);

    i = 2;
    file.seek(i * BLOCK_SIZE);
    if (file_printf(&file, "%lu\n", values[i]) <= 0) {
        return true;
    }
    DEBUG("    value[%i]: %lu -> %lu\n", i, values[i] - 1, values[i]);

    i = 1;
    file.seek(i * BLOCK_SIZE);
    if (file_printf(&file, "%lu\n", values[i]) <= 0) {
        return true;
    }
    DEBUG("    value[%i]: %lu -> %lu\n", i, values[i] - 1, values[i]);

    res = file.close();
    if (-ENOSPC  == res) {
        return true;
    }
    TEST_ASSERT_EQUAL(0, res);

    return false;
}

/*
 * Change the directory name from either DIRECTORY_RENAME_A or DIRECTORY_RENAME_B to the other
 *
 * Allowed states:
 * - CHANGE_CONTENTS_NAME exists and contains 3 counts which are in order
 */
static void check_file_change_contents(FileSystem *fs)
{
    DEBUG_CHECK("check_file_change_contents()\n");
    File file;

    int res = file.open(fs, CHANGE_CONTENTS_NAME,  O_RDONLY);
    TEST_ASSERT_EQUAL_OR_EXIT(0, res);

    // Read in values
    uint32_t values[3];
    for (int i = 0; i < 3; i++) {
        file.seek(i * BLOCK_SIZE);
        int args_read = file_scanf(&file, "%lu\n", &values[i]);
        TEST_ASSERT_EQUAL_OR_EXIT(1, args_read);
        DEBUG_CHECK("    value[%i]: %lu\n", i, values[i]);
    }

    TEST_ASSERT_EQUAL_OR_EXIT(values[0] + 1, values[1]);
    TEST_ASSERT_EQUAL_OR_EXIT(values[1] + 1, values[2]);
}


static const TestEntry atomic_test_entries[] = {
    {"File rename", setup_file_rename, perform_file_rename, check_file_rename},
    {"File rename replace", setup_file_rename_replace, perform_file_rename_replace, check_file_rename_replace},
    {"Directory rename", setup_directory_rename, perform_directory_rename, check_directory_rename},
    {"File change contents", setup_file_change_contents, perform_file_change_contents, check_file_change_contents},
};

static const char  FILE_SETUP_COMPLETE[] = "setup_complete.txt";
static const char  FILE_SETUP_COMPLETE_FMT[] = "Test version: %lu\n";

static bool format_required(BlockDevice *bd)
{
    MBED_TEST_FILESYSTEM_DECL;

    if (fs.mount(bd) != 0) {
        return true;
    }

    // Check if setup complete file exists
    File file;
    int res = file.open(&fs, FILE_SETUP_COMPLETE,  O_RDONLY);
    if (res != 0) {
        return true;
    }

    // Read contents of setup complete file
    uint8_t buf[BUFFER_SIZE];
    memset(buf, 0, sizeof(buf));
    int size_read = file.read(buf, sizeof(buf) - 1);
    if (size_read <= 0) {
        return true;
    }

    // Get the test version
    uint32_t version = 0;
    res = sscanf((char*)buf, FILE_SETUP_COMPLETE_FMT, &version);
    if (res != 1) {
        return true;
    }

    if (ATOMIC_USAGE_VERSION != version) {
        return true;
    }

    // Setup file exists and is the correct version
    return false;
}

static void format(BlockDevice *bd)
{
    MBED_TEST_FILESYSTEM_DECL;

    int res = fs.format(bd);
    TEST_ASSERT_EQUAL_OR_EXIT(0, res);

    res = fs.mount(bd);
    TEST_ASSERT_EQUAL_OR_EXIT(0, res);

    for (size_t i = 0; i < ARRAY_LENGTH(atomic_test_entries); i++) {
        atomic_test_entries[i].setup(&fs);
    }

    File file;
    res = file.open(&fs, FILE_SETUP_COMPLETE, O_CREAT | O_WRONLY);
    TEST_ASSERT_EQUAL_OR_EXIT(0, res);

    int size = file_printf(&file, FILE_SETUP_COMPLETE_FMT, (uint32_t)ATOMIC_USAGE_VERSION);
    TEST_ASSERT_OR_EXIT(size >= 0);
}

static int64_t get_cycle_count(FileSystem *fs)
{
    File file;

    int res = file.open(fs, FILE_RENAME_REPLACE, O_RDONLY);
    TEST_ASSERT_EQUAL_OR_EXIT(0, res);

    uint64_t count = 0;
    int args_read = file_scanf(&file, FILE_RENAME_REPLACE_FMT, &count);
    TEST_ASSERT_EQUAL_OR_EXIT(1, args_read);

    file.close();

    return (int64_t)count;
}

bool setup_atomic_operations(BlockDevice *bd, bool force_rebuild)
{
    if (force_rebuild || format_required(bd)) {
        format(bd);
        TEST_ASSERT_EQUAL_OR_EXIT(false, format_required(bd));
        return true;
    }
    return false;
}

int64_t perform_atomic_operations(BlockDevice *bd)
{
    MBED_TEST_FILESYSTEM_DECL;
    bool out_of_space = false;

    fs.mount(bd);

    for (size_t i = 0; i < ARRAY_LENGTH(atomic_test_entries); i++) {
        out_of_space |= atomic_test_entries[i].perform(&fs);
    }

    int64_t cycle_count = get_cycle_count(&fs);

    fs.unmount();

    if (out_of_space) {
        return -1;
    } else  {
        return cycle_count;
    }
}

void check_atomic_operations(BlockDevice *bd)
{
    MBED_TEST_FILESYSTEM_DECL;
    fs.mount(bd);

    for (size_t i = 0; i < ARRAY_LENGTH(atomic_test_entries); i++) {
        atomic_test_entries[i].check(&fs);
    }

    fs.unmount();
}
