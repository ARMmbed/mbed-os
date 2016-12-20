/** @file fsfat_test.h
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
#ifndef __FSFAT_TEST_H
#define __FSFAT_TEST_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

//#include "configuration_store.h"

/* Defines */
#define FSFAT_INIT_1_TABLE_HEAD               { "a", ""}
#define FSFAT_INIT_1_TABLE_MID_NODE           { "0123456789abcdef0123456", "abcdefghijklmnopqrstuvwxyz"}
#define FSFAT_INIT_1_TABLE_TAIL               { "yotta.hello-world.animal{wobbly-dog}{foot}backRight", "present"}
#define FSFAT_TEST_RW_TABLE_SENTINEL          0xffffffff
#define FSFAT_TEST_BYTE_DATA_TABLE_SIZE       256
#define FSFAT_UTEST_MSG_BUF_SIZE              256
#define FSFAT_UTEST_DEFAULT_TIMEOUT_MS        10000
#define FSFAT_MBED_HOSTTEST_TIMEOUT 			60

/* support macro for make string for utest _MESSAGE macros, which dont support formatted output */
#define FSFAT_TEST_UTEST_MESSAGE(_buf, _max_len, _fmt, ...)   \
  do                                                            \
  {                                                             \
      snprintf((_buf), (_max_len), (_fmt), __VA_ARGS__);        \
  }while(0);


/*
 * Structures
 */

/* kv data for test */
typedef struct fsfat_kv_data_t {
    const char* key_name;
    const char* value;
} fsfat_kv_data_t;

typedef struct fsfat_test_rw_data_entry_t
{
    uint32_t offset;
    char rw_char;
} fsfat_test_rw_data_entry_t;


extern fsfat_kv_data_t fsfat_test_init_1_data[];
extern fsfat_test_rw_data_entry_t fsfat_test_rw_data_table[];
extern const char* fsfat_test_opcode_str[];
extern const uint8_t fsfat_test_byte_data_table[FSFAT_TEST_BYTE_DATA_TABLE_SIZE];

int32_t fsfat_test_check_node_correct(const fsfat_kv_data_t* node);
//int32_t fsfat_test_create(const char* key_name, const char* data, size_t* len, ARM_FSFAT_KEYDESC* kdesc);
int32_t fsfat_test_create_table(const fsfat_kv_data_t* table);
int32_t fsfat_test_delete(const char* key_name);
int32_t fsfat_test_delete_all(void);
int32_t fsfat_test_dump(void);
int32_t fsfat_test_init_1(void);
int32_t fsfat_test_kv_is_found(const char* key_name, bool* bfound);
int32_t fsfat_test_kv_name_gen(char* name, const size_t len);
int32_t fsfat_test_read(const char* key_name, char* data, size_t* len);
int32_t fsfat_test_startup(void);
int32_t fsfat_test_write(const char* key_name, const char* data, size_t* len);

#ifdef __cplusplus
}
#endif

#endif /* __FSFAT_TEST_H */
