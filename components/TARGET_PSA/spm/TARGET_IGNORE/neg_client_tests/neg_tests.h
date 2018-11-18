/* Copyright (c) 2017-2018 ARM Limited
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

#define PSA_NEG_TEST_NAME(test_name) psa_neg_test_ ## test_name

#define PSA_NEG_TEST(test_name)       \
void PSA_NEG_TEST_NAME(test_name)()   \
{                                     \
    osStatus status = osOK;           \
    Thread T1;                        \
                                      \
    status = T1.start(test_name);     \
    TEST_ASSERT_TRUE(status == osOK); \
    test_sem.wait(osWaitForever);     \
    TEST_ASSERT_TRUE(error_thrown);   \
                                      \
    status = T1.terminate();          \
    TEST_ASSERT_TRUE(status == osOK); \
}                                     \

