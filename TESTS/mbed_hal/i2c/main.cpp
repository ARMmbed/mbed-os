/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#if !DEVICE_I2C
#error [NOT_SUPPORTED] I2C not supported for this target
#endif

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "i2c_test.h"

#include "mbed.h"
#include "i2c_api.h"


using namespace utest::v1;

#define DEVICE_AS_MASTER   false
#define DEVICE_AS_SLAVE    true

#define I2C_ADDRESS_MASK_7BIT   (0x7F)
#define I2C_ADDRESS_MASK_10BIT  (0x3FF)
// 7bit addressing, allowed values from 0x8 to 0x77
#define I2C_7BIT_ADDRESS_MIN    (0x08)
#define I2C_7BIT_ADDRESS_MAX    (0x77)
#define I2C_7BIT_ADDRESS        (0x55)
#define MAKE_7BIT_SLAVE_ADDRESS(addr)   ((I2C_ADDRESS_MASK_7BIT & addr) << 1)
#define MAKE_7BIT_READ_ADDRESS(addr)        (((I2C_ADDRESS_MASK_7BIT & addr) << 1) | 1)
#define MAKE_7BIT_WRITE_ADDRESS(addr)   ((I2C_ADDRESS_MASK_7BIT & addr) << 1)


// TODO:
//    - add 10bit addressing support
//    - add test for concurrent master and slave initialization using different I2C interfaces
//        (requires pins standardisation for more than one interfaces : I2C0_SDA, I2C0_SCL; I2C1_SDA, I2C1_SCL)


// Test that i2c_get_capabilities fills given i2c_capabilities_t instance with capabilities
void test_i2c_capabilities()
{
    i2c_capabilities_t cap;

    i2c_get_capabilities(&cap);

    TEST_ASSERT_TRUE(cap.minimum_frequency > 0);
    TEST_ASSERT_TRUE(cap.minimum_frequency <= cap.maximum_frequency);

#if DEVICE_I2CSLAVE
    TEST_ASSERT_TRUE(cap.supports_slave_mode);
#else
    TEST_ASSERT_FALSE(cap.supports_slave_mode);
#endif
}

// Test that i2c_init successfully initializes and i2c_free successfully frees i2c interface
void test_i2c_init_free()
{
    i2c_t obj = {};

    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);
    i2c_free(&obj);

    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);
    i2c_free(&obj);

    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);
    i2c_free(&obj);

#if DEVICE_I2CSLAVE
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_SLAVE);
    i2c_free(&obj);
#endif

    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);
    i2c_free(&obj);

#if DEVICE_I2CSLAVE
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_SLAVE);
    i2c_free(&obj);
#endif

    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);
    i2c_free(&obj);

#if DEVICE_I2CSLAVE
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_SLAVE);
    i2c_free(&obj);
#endif

    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);
    i2c_free(&obj);

#if DEVICE_I2CSLAVE
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_SLAVE);
    i2c_free(&obj);

    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_SLAVE);
    i2c_free(&obj);

    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_SLAVE);
    i2c_free(&obj);
#endif
}

//  Test that i2c_frequency successfully sets given frequency
void test_i2c_frequency()
{
    i2c_t obj = {};
    uint32_t set_frequency;
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);

    i2c_capabilities_t cap;
    i2c_get_capabilities(&cap);

    set_frequency = i2c_frequency(&obj, cap.minimum_frequency);
    TEST_ASSERT_EQUAL(cap.minimum_frequency, set_frequency);

    set_frequency = i2c_frequency(&obj, (cap.minimum_frequency + cap.maximum_frequency) / 2);
    TEST_ASSERT(set_frequency >= cap.minimum_frequency && set_frequency <= cap.maximum_frequency);

    set_frequency = i2c_frequency(&obj, cap.maximum_frequency);
    TEST_ASSERT_EQUAL(cap.maximum_frequency, set_frequency);
    i2c_free(&obj);


    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_SLAVE);
    i2c_get_capabilities(&cap);
    set_frequency = i2c_frequency(&obj, cap.minimum_frequency);

    TEST_ASSERT_EQUAL(cap.minimum_frequency, set_frequency);
    set_frequency = i2c_frequency(&obj, (cap.minimum_frequency + cap.maximum_frequency) / 2);

    TEST_ASSERT(set_frequency >= cap.minimum_frequency && set_frequency <= cap.maximum_frequency);
    set_frequency = i2c_frequency(&obj, cap.maximum_frequency);

    TEST_ASSERT_EQUAL(cap.maximum_frequency, set_frequency);
    i2c_free(&obj);
}

// Test that i2c_start successfully sends start signal to the bus
void test_i2c_start()
{
    i2c_t obj = {};
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);

    i2c_start(&obj);

    i2c_free(&obj);
}

// Test that i2c_stop successfully sends stop signal to the bus
void test_i2c_stop()
{
    i2c_t obj = {};
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);

    i2c_stop(&obj);

    i2c_free(&obj);
}

// Test i2c_read/i2c_write calls for non-existent slave address
void test_i2c_read_write()
{
    int ret;
    const uint32_t size = 3;
    uint8_t data[size];

    i2c_t obj = {};
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);

    ret = i2c_write(&obj, MAKE_7BIT_WRITE_ADDRESS(I2C_7BIT_ADDRESS), (void *)data, size, false);
    TEST_ASSERT_NOT_EQUAL(size, ret);

    ret = i2c_write(&obj, MAKE_7BIT_WRITE_ADDRESS(I2C_7BIT_ADDRESS), (void *)data, size, true);
    TEST_ASSERT_NOT_EQUAL(size, ret);

    ret = i2c_read(&obj, MAKE_7BIT_READ_ADDRESS(I2C_7BIT_ADDRESS), (void *)data, size, false);
    TEST_ASSERT_NOT_EQUAL(size, ret);

    ret = i2c_read(&obj, MAKE_7BIT_READ_ADDRESS(I2C_7BIT_ADDRESS), (void *)data, size, true);
    TEST_ASSERT_NOT_EQUAL(size, ret);

    i2c_free(&obj);
}

#if DEVICE_I2CSLAVE
// Test that i2c_slave_address successfully sets given address
void test_i2c_slave_address()
{
    i2c_t obj = {};
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_SLAVE);

    i2c_slave_address(&obj, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS_MIN));
    i2c_slave_address(&obj, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS));
    i2c_slave_address(&obj, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS_MAX));

    i2c_free(&obj);
}

// Test that i2c_slave_address successfully sets given address
void test_i2c_slave_status()
{
    i2c_t obj = {};
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_SLAVE);

    i2c_slave_address(&obj, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS));

    i2c_slave_status_t status = i2c_slave_status(&obj);
    TEST_ASSERT_EQUAL(NoData, status);

    i2c_free(&obj);
}

// Test that i2c_set_clock_stretching successfully enable/disable clock stretching feature
void test_i2c_set_clock_stretching()
{
    i2c_t obj = {};
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);

    i2c_set_clock_stretching(&obj, true);
    i2c_set_clock_stretching(&obj, false);

    i2c_free(&obj);
}
#endif

#if DEVICE_I2C_ASYNCH
volatile bool async_handler_called;
void i2c_async_handler(i2c_t *obj, i2c_async_event_t *event, void *ctx)
{
    i2c_async_event_t *e = (i2c_async_event_t *)ctx;
    e->sent_bytes = event->sent_bytes;
    e->received_bytes = event->received_bytes;
    e->error = event->error;
    async_handler_called = true;
}

// Test i2c_transfer_async call for non-existent slave address
void test_i2c_transfer_async()
{
    int ret;
    const uint32_t size = 3;
    uint8_t tx_data[size];
    uint8_t rx_data[size];

    i2c_t obj = {};
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);

    {
        i2c_async_event_t event = {};
        async_handler_called = false;
        i2c_transfer_async(&obj, tx_data, size, NULL, 0, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS), true, i2c_async_handler, &event);
        wait(1);
        TEST_ASSERT_EQUAL(true, async_handler_called);
        TEST_ASSERT_EQUAL(true, event.error);
    }

    {
        i2c_async_event_t event = {};
        async_handler_called = false;
        i2c_transfer_async(&obj, NULL, 0, rx_data, size, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS), true, i2c_async_handler, &event);
        wait(1);
        TEST_ASSERT_EQUAL(true, async_handler_called);
        TEST_ASSERT_EQUAL(true, event.error);
    }

    {
        i2c_async_event_t event = {};
        async_handler_called = false;
        i2c_transfer_async(&obj, tx_data, size, rx_data, size, MAKE_7BIT_SLAVE_ADDRESS(I2C_7BIT_ADDRESS), true, i2c_async_handler, &event);
        wait(1);
        TEST_ASSERT_EQUAL(true, async_handler_called);
        TEST_ASSERT_EQUAL(true, event.error);
    }

    i2c_free(&obj);
}
#endif

Case cases[] = {
    Case("i2c - get capabilities test", test_i2c_capabilities),
    Case("i2c - init/free test", test_i2c_init_free),
    Case("i2c - set frequency test", test_i2c_frequency),
    Case("i2c - start signal test", test_i2c_start),
    Case("i2c - stop signal test", test_i2c_stop),
    Case("i2c - master read/write test", test_i2c_read_write),
#if DEVICE_I2CSLAVE
    Case("i2c - setting slave address test", test_i2c_slave_address),
    Case("i2c - slave state test", test_i2c_slave_status),
    Case("i2c - enable/disable clock stretching test", test_i2c_set_clock_stretching),
#endif
#if DEVICE_I2C_ASYNCH
    Case("i2c - transfer async test", test_i2c_transfer_async),
#endif
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(115, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
