/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "platform/Transaction.h"

using utest::v1::Case;

static void dummy_callback(int)
{
    /* do nothing. */
}

/** Test Transaction class - creation with initialisation
 *
 *  Given is Transaction class.
 *  When object of the Transaction class is created and initialised.
 *  Then get_object() returns object's instance for the transaction and get_transaction() returns the transaction.
 *
 */
template<typename Type>
void test_Transaction_init()
{
    Type object;
    const size_t tx_buffer_size = 24;
    const size_t rx_buffer_size = 16;
    const uint32_t event_id = 123;
    const uint8_t word_width = 8;
    unsigned char tx_buffer[tx_buffer_size];
    unsigned char rx_buffer[rx_buffer_size];
    const event_callback_t &callback = dummy_callback;
    transaction_t transaction_data =
    { tx_buffer, tx_buffer_size, rx_buffer, rx_buffer_size, event_id, callback, word_width };

    Transaction<Type> test_transaction(&object, transaction_data);

    TEST_ASSERT_EQUAL(&object, test_transaction.get_object());

    TEST_ASSERT_EQUAL((void *)tx_buffer, test_transaction.get_transaction()->tx_buffer);
    TEST_ASSERT_EQUAL((void *)rx_buffer, test_transaction.get_transaction()->rx_buffer);
    TEST_ASSERT_EQUAL(tx_buffer_size,   test_transaction.get_transaction()->tx_length);
    TEST_ASSERT_EQUAL(rx_buffer_size,   test_transaction.get_transaction()->rx_length);
    TEST_ASSERT_EQUAL(event_id,         test_transaction.get_transaction()->event);
    TEST_ASSERT_EQUAL(word_width,       test_transaction.get_transaction()->width);
#if MBED_CONF_PLATFORM_CALLBACK_COMPARABLE
    TEST_ASSERT_TRUE(callback == test_transaction.get_transaction()->callback);
#else
    TEST_ASSERT_FALSE(nullptr == test_transaction.get_transaction()->callback)
#endif
}

/** Test Transaction class - creation without initialisation
 *
 *  Given is Transaction class.
 *  When object of the Transaction class is created.
 *  Then this operation is successful.
 *
 */
template<typename Type>
void test_Transaction_empty()
{
    Type object;

    Transaction<Type> test_transaction;

    /* Just indicate successful execution of the test case. */
    TEST_ASSERT_TRUE(true);
}

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return utest::v1::verbose_test_setup_handler(number_of_cases);
}

Case cases[] = {
    Case("Test Transaction - init <Timer>", test_Transaction_init<Timer>),
    Case("Test Transaction - init <Ticker>", test_Transaction_init<Ticker>),
    Case("Test Transaction - no init <Timer>", test_Transaction_empty<Timer>),
    Case("Test Transaction - no init <Ticker>", test_Transaction_empty<Ticker>),
};

utest::v1::Specification specification(test_setup, cases);

int main()
{
    return !utest::v1::Harness::run(specification);
}
