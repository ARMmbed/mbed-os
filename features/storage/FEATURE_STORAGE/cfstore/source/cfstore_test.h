/** @file cfstore_test.h
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
#ifndef __CFSTORE_TEST_H
#define __CFSTORE_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "configuration_store.h"

/* Defines */
#define CFSTORE_INIT_1_TABLE_HEAD               { "a", ""}
#define CFSTORE_INIT_1_TABLE_MID_NODE           { "0123456789abcdef0123456", "abcdefghijklmnopqrstuvwxyz"}
#define CFSTORE_INIT_1_TABLE_TAIL               { "yotta.hello-world.animal{wobbly-dog}{foot}backRight", "present"}
#define CFSTORE_TEST_RW_TABLE_SENTINEL          0xffffffff
#define CFSTORE_TEST_BYTE_DATA_TABLE_SIZE       256
#define CFSTORE_UTEST_MSG_BUF_SIZE              256
#define CFSTORE_UTEST_DEFAULT_TIMEOUT_MS        10000
#define CFSTORE_MBED_HOSTTEST_TIMEOUT 			60

/* support macro for make string for utest _MESSAGE macros, which dont support formatted output */
#define CFSTORE_TEST_UTEST_MESSAGE(_buf, _max_len, _fmt, ...)   \
  do                                                            \
  {                                                             \
      snprintf((_buf), (_max_len), (_fmt), __VA_ARGS__);        \
  }while(0);


/*
 * Structures
 */

/* kv data for test */
typedef struct cfstore_kv_data_t {
    const char* key_name;
    const char* value;
} cfstore_kv_data_t;

typedef struct cfstore_test_rw_data_entry_t
{
    uint32_t offset;
    char rw_char;
} cfstore_test_rw_data_entry_t;


extern cfstore_kv_data_t cfstore_test_init_1_data[];
extern cfstore_test_rw_data_entry_t cfstore_test_rw_data_table[];
extern const char* cfstore_test_opcode_str[];
extern const uint8_t cfstore_test_byte_data_table[CFSTORE_TEST_BYTE_DATA_TABLE_SIZE];

int32_t cfstore_test_check_node_correct(const cfstore_kv_data_t* node);
int32_t cfstore_test_create(const char* key_name, const char* data, size_t* len, ARM_CFSTORE_KEYDESC* kdesc);
int32_t cfstore_test_create_table(const cfstore_kv_data_t* table);
int32_t cfstore_test_delete(const char* key_name);
int32_t cfstore_test_delete_all(void);
int32_t cfstore_test_dump(void);
int32_t cfstore_test_init_1(void);
int32_t cfstore_test_kv_is_found(const char* key_name, bool* bfound);
int32_t cfstore_test_kv_name_gen(char* name, const size_t len);
int32_t cfstore_test_read(const char* key_name, char* data, size_t* len);
int32_t cfstore_test_startup(void);
int32_t cfstore_test_write(const char* key_name, const char* data, size_t* len);

#ifdef __cplusplus
}
#endif

#endif /* __CFSTORE_TEST_H */
