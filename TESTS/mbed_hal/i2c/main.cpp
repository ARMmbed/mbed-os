/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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
    i2c_t obj;

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
    i2c_t obj;
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);

    i2c_capabilities_t cap;
    i2c_get_capabilities(&cap);

    i2c_frequency(&obj, cap.minimum_frequency);

    i2c_frequency(&obj, (cap.minimum_frequency + cap.maximum_frequency) / 2);

    i2c_frequency(&obj, cap.maximum_frequency);

    i2c_free(&obj);
}

// Test that i2c_timeout successfully sets given timeout
void test_i2c_timeout()
{
    i2c_t obj;
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);

    i2c_timeout(&obj, 0);
    i2c_timeout(&obj, 1);
    i2c_timeout(&obj, 10);

    i2c_free(&obj);
}

// Test that i2c_start/i2c_stop successfully sends start/stop signal to the bus
void test_i2c_start_stop()
{
    i2c_t obj;
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);


    bool ret = i2c_start(&obj);
    TEST_ASSERT_FALSE(ret);

    ret = i2c_stop(&obj);
    TEST_ASSERT_FALSE(ret);

    i2c_free(&obj);
}

// Test i2c_read/i2c_write calls for non-existent slave address
void test_i2c_read_write()
{
    int ret;
    uint16_t test_address = 123; // choose high address to be sure there is no device with such address
    const uint32_t size = 3;
    uint8_t data[size];

    i2c_t obj;
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);

    ret = i2c_write(&obj, test_address, (void*)data, size, false);
    TEST_ASSERT_EQUAL(I2C_ERROR_NO_SLAVE, ret);

    ret = i2c_read(&obj, test_address, (void*)data, size, true);
    TEST_ASSERT_EQUAL(I2C_ERROR_NO_SLAVE, ret);

    i2c_free(&obj);
}

#if DEVICE_I2CSLAVE
// Test that i2c_slave_address successfully sets given address
void test_i2c_slave_address()
{
    i2c_t obj;
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_SLAVE);

    i2c_slave_address(&obj, 123);
    i2c_slave_address(&obj, 124);
    i2c_slave_address(&obj, 125);

    i2c_free(&obj);
}

// Test that i2c_slave_address successfully sets given address
void test_i2c_slave_receive()
{
    i2c_t obj;
    uint16_t test_address = 123; // choose high address to be sure there is no device with such address
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_SLAVE);

    i2c_slave_address(&obj, test_address);

    i2c_slave_status status = i2c_slave_receive(&obj);
    TEST_ASSERT_EQUAL(NOT_ADDRESSED, status);

    i2c_free(&obj);
}
#endif

#if DEVICE_I2C_ASYNCH
void i2c_async_handler(i2c_t *obj, void *ctx, i2c_async_event_t event)
{

}

// Test i2c_transfer_async call for non-existent slave address
void test_i2c_transfer_async()
{
    int ret;
    uint16_t test_address = 123; // choose high address to be sure there is no device with such address
    const uint32_t size = 3;
    uint8_t tx_data[size];
    uint8_t rx_data[size];

    i2c_t obj;
    i2c_init(&obj, I2C_SDA, I2C_SCL, DEVICE_AS_MASTER);

    i2c_transfer_async(&obj, tx_data, size, tx_data, uint8_t tx_data,
                       test_address, true, i2c_async_handler, DMA_USAGE_NEVER);

    i2c_abort_async(&obj);

    i2c_free(&obj);
}
#endif

Case cases[] = {
    Case("i2c - get capabilities test", test_i2c_capabilities),
    Case("i2c - init/free test", test_i2c_init_free),
    Case("i2c - set frequency test", test_i2c_frequency),
    Case("i2c - set timeout test", test_i2c_timeout),
    Case("i2c - start/stop signal test", test_i2c_start_stop),
    Case("i2c - master read/write test", test_i2c_read_write),
#if DEVICE_I2CSLAVE
    Case("i2c - setting slave address test", test_i2c_slave_address),
    Case("i2c - slave state test", test_i2c_slave_receive),
#endif
#if DEVICE_I2C_ASYNCH
    Case("i2c - transfer async test", test_i2c_transfer_async),
#endif
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    Harness::run(specification);
}
