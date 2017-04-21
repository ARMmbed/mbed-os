/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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
#include <stdio.h>
#include "TestHarness.h"
#include "mbed.h"

/* EEPROM 24LC256 Test Unit, to test I2C asynchronous communication.
 */

#if !DEVICE_I2C || !DEVICE_I2C_ASYNCH
#error i2c_master_eeprom_asynch requires asynch I2C
#endif

#if defined(TARGET_K64F)
#define TEST_SDA_PIN PTE25
#define TEST_SCL_PIN PTE24

#elif defined(TARGET_K66F)
#define TEST_SDA_PIN PTD9
#define TEST_SCL_PIN PTD8

#elif defined(TARGET_EFM32LG_STK3600) || defined(TARGET_EFM32GG_STK3700) || defined(TARGET_EFM32WG_STK3800)
#define TEST_SDA_PIN PD6
#define TEST_SCL_PIN PD7

#elif defined(TARGET_EFM32ZG_STK3200)
#define TEST_SDA_PIN PE12
#define TEST_SCL_PIN PE13

#elif defined(TARGET_EFM32HG_STK3400)
#define TEST_SDA_PIN PD6
#define TEST_SCL_PIN PD7

#elif defined(TARGET_RZ_A1H)
#define TEST_SDA_PIN P1_3
#define TEST_SCL_PIN P1_2
void sleep()
{
}

#else
#error Target not supported
#endif

#define PATTERN_MASK 0x66, ~0x66, 0x00, 0xFF, 0xA5, 0x5A, 0xF0, 0x0F

volatile int why;
volatile bool complete;
void cbdone(int event) {
    complete = true;
    why = event;
}

const unsigned char pattern[] = { PATTERN_MASK };

TEST_GROUP(I2C_Master_EEPROM_Asynchronous)
{
    I2C *obj;
    const int eeprom_address = 0xA0;
    event_callback_t callback;

    void setup() {
        obj = new I2C(TEST_SDA_PIN, TEST_SCL_PIN);
        obj->frequency(400000);
        complete = false;
        why = 0;
        callback.attach(cbdone);
    }

    void teardown() {
        delete obj;
        obj = NULL;
    }

};

TEST(I2C_Master_EEPROM_Asynchronous, tx_rx_one_byte_separate_transactions)
{
    int rc;
    char data[] = { 0, 0, 0x66};

    rc = obj->transfer(eeprom_address, data, sizeof(data), NULL, 0, callback, I2C_EVENT_ALL, false);
    CHECK_EQUAL(0, rc);
    while (!complete) {
        sleep();
    }

    CHECK_EQUAL(why, I2C_EVENT_TRANSFER_COMPLETE);

    // wait until slave is ready
    do {
        complete = 0;
        why = 0;
        obj->transfer(eeprom_address, NULL, 0, NULL, 0, callback, I2C_EVENT_ALL, false);
        while (!complete) {
            sleep();
        }
    } while (why != I2C_EVENT_TRANSFER_COMPLETE);


    // write the address for reading (0,0) then start reading data
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    why = 0;
    complete = 0;
    obj->transfer(eeprom_address, data, 2, NULL, 0, callback, I2C_EVENT_ALL, true);
    while (!complete) {
        sleep();
    }
    CHECK_EQUAL(why, I2C_EVENT_TRANSFER_COMPLETE);

    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    why = 0;
    complete = 0;
    rc = obj->transfer(eeprom_address, NULL, 0, data, 1, callback, I2C_EVENT_ALL, false);
    CHECK_EQUAL(0, rc);
    while (!complete) {
        sleep();
    }
    CHECK_EQUAL(why, I2C_EVENT_TRANSFER_COMPLETE);
    CHECK_EQUAL(data[0], 0x66);
}

TEST(I2C_Master_EEPROM_Asynchronous, tx_rx_one_byte_one_transactions)
{
    int rc;
    char send_data[] = { 0, 0, 0x66};
    rc = obj->transfer(eeprom_address, send_data, sizeof(send_data), NULL, 0, callback, I2C_EVENT_ALL, false);
    CHECK_EQUAL(0, rc)

    while (!complete) {
        sleep();
    }

    CHECK_EQUAL(why, I2C_EVENT_TRANSFER_COMPLETE);

    // wait until slave is ready
    do {
        complete = 0;
        why = 0;
        obj->transfer(eeprom_address, NULL, 0, NULL, 0, callback, I2C_EVENT_ALL, false);
        while (!complete) {
            sleep();
        }
    } while (why != I2C_EVENT_TRANSFER_COMPLETE);


    send_data[0] = 0;
    send_data[1] = 0;
    send_data[2] = 0;
    char receive_data[1] = {0};
    why = 0;
    complete = 0;
    rc = obj->transfer(eeprom_address, send_data, 2, receive_data, 1, callback, I2C_EVENT_ALL, false);
    CHECK_EQUAL(0, rc);
    while (!complete) {
        sleep();
    }

    CHECK_EQUAL(why, I2C_EVENT_TRANSFER_COMPLETE);
    CHECK_EQUAL(receive_data[0], 0x66);
}

TEST(I2C_Master_EEPROM_Asynchronous, tx_rx_pattern)
{
    int rc;
    char data[] = { 0, 0, PATTERN_MASK};
    // write 8 bytes to 0x0, then read them
    rc = obj->transfer(eeprom_address, data, sizeof(data), NULL, 0, callback, I2C_EVENT_ALL, false);
    CHECK_EQUAL(0, rc);

    while (!complete) {
        sleep();
    }
    CHECK_EQUAL(why, I2C_EVENT_TRANSFER_COMPLETE);

    // wait until slave is ready
    do {
        complete = 0;
        why = 0;
        obj->transfer(eeprom_address, NULL, 0, NULL, 0, callback, I2C_EVENT_ALL, false);
        while (!complete) {
            sleep();
        }
    } while (why != I2C_EVENT_TRANSFER_COMPLETE);

    complete = 0;
    why = 0;
    char rec_data[8] = {0};
    rc = obj->transfer(eeprom_address, rec_data, 2, NULL, 0, callback, I2C_EVENT_ALL, true);
    CHECK_EQUAL(0, rc);
    while (!complete) {
        sleep();
    }
    CHECK_EQUAL(why, I2C_EVENT_TRANSFER_COMPLETE);

    complete = 0;
    why = 0;
    rc = obj->transfer(eeprom_address, NULL, 0, rec_data, 8, callback, I2C_EVENT_ALL, false);
    CHECK_EQUAL(0, rc);
    while (!complete) {
        sleep();
    }
    CHECK_EQUAL(why, I2C_EVENT_TRANSFER_COMPLETE);

    // received buffer match with pattern
    rc = memcmp(pattern, rec_data, sizeof(rec_data));
    CHECK_EQUAL(0, rc);
}
