/** @file fslittle_test.h
 *
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
 * Header file for test support data structures and function API.
 */
#ifndef __FSLITTLE_TEST_H
#define __FSLITTLE_TEST_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Defines */
//#define FSLITTLE_INIT_1_TABLE_HEAD                 { "a", ""}
#define FSLITTLE_INIT_1_TABLE_MID_NODE             { "/sd/01234567.txt", "abcdefghijklmnopqrstuvwxyz"}
//#define FSLITTLE_INIT_1_TABLE_TAIL                 { "/sd/fopentst/hello/world/animal/wobbly/dog/foot/backrght.txt", "present"}
#define FSLITTLE_TEST_RW_TABLE_SENTINEL            0xffffffff
#define FSLITTLE_TEST_BYTE_DATA_TABLE_SIZE         256
#define FSLITTLE_UTEST_MSG_BUF_SIZE                256
#define FSLITTLE_UTEST_DEFAULT_TIMEOUT_MS          10000
#define FSLITTLE_MBED_HOSTTEST_TIMEOUT             60
#define FSLITTLE_MAX_FILE_BASENAME                 8
#define FSLITTLE_MAX_FILE_EXTNAME                  3
#define FSLITTLE_BUF_MAX_LENGTH                    64
#define FSLITTLE_FILENAME_MAX_LENGTH               255


/* support macro for make string for utest _MESSAGE macros, which dont support formatted output */
#define FSLITTLE_TEST_UTEST_MESSAGE(_buf, _max_len, _fmt, ...)   \
  do                                                            \
  {                                                             \
      snprintf((_buf), (_max_len), (_fmt), __VA_ARGS__);        \
  }while(0);


/*
 * Structures
 */

/* kv data for test */
typedef struct fslittle_kv_data_t {
    const char *filename;
    const char *value;
} fslittle_kv_data_t;


extern const uint8_t fslittle_test_byte_data_table[FSLITTLE_TEST_BYTE_DATA_TABLE_SIZE];

int32_t fslittle_test_create(const char *filename, const char *data, size_t len);
int32_t fslittle_test_delete(const char *key_name);
int32_t fslittle_test_filename_gen(char *name, const size_t len);
#ifdef __cplusplus
}
#endif

#endif /* __FSLITTLE_TEST_H */
