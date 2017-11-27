/*
 * Copyright (c) 2015, Arm Limited and affiliates.
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

/*
This file improves the unity to allow module tests be more flexible.

When you make simulated function add
  void my_test_method(char *data_ptr)
  {
    UNITY_METHOD_CALLED("my_test_method",data_ptr);
  }

When testing the method values
  void test_my_test_method(char *data_ptr)
  {
   params_ptr = UNITY_METHOD_CHECK("my_test_method");
   TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(data_ptr, params_ptr,3,"invalid string");
  }
In testcase
  do_stuff("foo","bar");
  test_my_test_method("foo");
  test_my_test_method("bar");

that will allow correct order of methods to be check in testcase and multiple
different parameter values to be checkked if two methods are called

*/

#define TEST_METHOD_CALLED(name,ptr) unity_impr_teststep_set(name,ptr)
#define TEST_METHOD_CHECK(name) unity_impr_teststep_get(name)

#define TEST_DESCRIPTION(...) printf(__VA_ARGS__)

void unity_impr_teststep_set(char *function_name_ptr, void *params_ptr);
void *unity_impr_teststep_get(char *function_name_ptr);
void unity_impr_teststep_clean(void);

void print_buf(const uint8_t *buf, uint16_t len);
void test_buf(const uint8_t *buf, uint16_t len, const uint8_t *buf1, uint16_t len1);


#define put_uint32(x, z)    do {\
                                (x)[0] = (uint8_t) ((z) >> 24);\
                                (x)[1] = (uint8_t) ((z) >> 16);\
                                (x)[2] = (uint8_t) ((z) >> 8);\
                                (x)[3] = (uint8_t) ((z));\
                                } while(0)

#define get_uint32(x) \
   (((uint32_t)(x)[0] << 24 ) |\
    ((uint32_t)(x)[1] << 16 ) |\
    ((uint32_t)(x)[2] << 8 ) |\
     (uint32_t)(x)[3])
