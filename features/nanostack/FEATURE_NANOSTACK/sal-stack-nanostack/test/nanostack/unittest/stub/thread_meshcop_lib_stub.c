/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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
#include "nsconfig.h"
#include "common_functions.h"
#include <string.h>
#include "thread_meshcop_lib_stub.h"
thread_meshcop_lib_stub_def thread_meshcop_lib_stub;

uint8_t *thread_meshcop_tlv_data_write(uint8_t *ptr, uint8_t type, uint16_t length, const uint8_t *data)
{
    return NULL;
}

uint8_t *thread_meshcop_tlv_data_write_header(uint8_t *ptr, uint8_t type, uint16_t length)
{
    return NULL;
}

uint8_t *thread_meshcop_tlv_data_write_uint8(uint8_t *ptr, uint8_t type, uint8_t data)
{
    return NULL;
}

uint8_t *thread_meshcop_tlv_data_write_uint16(uint8_t *ptr, uint8_t type, uint16_t data)
{
    return NULL;
}
uint8_t *thread_meshcop_tlv_data_write_uint32(uint8_t *ptr, uint8_t type, uint32_t data)
{
    return NULL;
}
uint8_t *thread_meshcop_tlv_data_write_uint64(uint8_t *ptr, uint8_t type, uint64_t data)
{
    return NULL;
}

bool thread_meshcop_tlv_exist(const uint8_t *ptr, const uint16_t length, const uint8_t type)
{
    return thread_meshcop_lib_stub.expectedBool;
}
uint16_t thread_meshcop_tlv_list_generate(const uint8_t *ptr, uint16_t length,uint8_t *result_ptr, uint16_t *result_len)
{
	return 0;
}
uint16_t thread_meshcop_tlv_list_remove(uint8_t *tlv_ptr, uint16_t tlv_len, uint8_t tlv_type)
{
	return 0;
}

bool thread_meshcop_tlv_list_type_available(const uint8_t *list_ptr, uint16_t list_len, uint8_t tlv_type)
{
    return thread_meshcop_lib_stub.expectedBool;
}

uint16_t thread_meshcop_tlv_find(const uint8_t *ptr, uint16_t length, uint8_t type, uint8_t **result_ptr)
{
    uint8_t *data_ptr;
    *data_ptr = thread_meshcop_lib_stub.expectedUnit8ResultValue;
    *result_ptr = (uint8_t *) data_ptr;
    return thread_meshcop_lib_stub.expectedUint16;

}

uint8_t thread_meshcop_tlv_data_get_uint8(const uint8_t *ptr, uint16_t length, uint8_t type, uint8_t *data_ptr)
{
    return thread_meshcop_lib_stub.expectedInt8;
}

uint8_t thread_meshcop_tlv_data_get_uint16(const uint8_t *ptr, uint16_t length, uint8_t type, uint16_t *data_ptr)
{
    return thread_meshcop_lib_stub.expectedInt8;
}

uint8_t thread_meshcop_tlv_data_get_uint32(const uint8_t *ptr, uint16_t length, uint8_t type, uint32_t *data_ptr)
{
    return thread_meshcop_lib_stub.expectedInt8;
}
uint8_t thread_meshcop_tlv_data_get_uint64(const uint8_t *ptr, uint16_t length, uint8_t type, uint64_t *data_ptr)
{
    return thread_meshcop_lib_stub.expectedInt8;
}
uint16_t thread_meshcop_tlv_find_next(uint8_t* tlv_ba, uint16_t tlv_ba_length, uint8_t tlv_id, uint8_t** found_tlv)
{
    return 0;
}

