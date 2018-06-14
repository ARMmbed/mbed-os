/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#include "driver/CordioHCITransportDriver.h"
#include "driver/CordioHCIDriver.h"
#include "hci_defs.h"
#include "rtos/Semaphore.h"

#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"

using namespace utest::v1;

using ble::vendor::cordio::CordioHCIDriver;
using ble::vendor::cordio::CordioHCITransportDriver;

extern ble::vendor::cordio::CordioHCIDriver& ble_cordio_get_hci_driver();

namespace ble {
namespace vendor {
namespace cordio {

struct CordioHCIHook {
    static CordioHCIDriver& get_driver() {
        return ble_cordio_get_hci_driver();
    }

    static CordioHCITransportDriver& get_transport_driver() {
        return get_driver()._transport_driver;
    }

    static void set_data_received_handler(void (*handler)(uint8_t*, uint8_t)) {
        get_transport_driver().set_data_received_handler(handler);
    }
};

} // namespace cordio
} // namespace vendor
} // namespace ble

using ble::vendor::cordio::CordioHCIHook;

#define RESET_COMMAND_TIMEOUT (10 * 1000)
#define RESET_PARAMETER_LENGTH  4
#define RESET_EXPECTED_STATUS  0
#define HCI_OPCODE_RESET_LSB (HCI_OPCODE_RESET & 0xFF)
#define HCI_OPCODE_RESET_MSB (HCI_OPCODE_RESET >> 8)

enum test_result_t {
    TEST_RESULT_TIMEOUT_FAILURE,
    TEST_RESULT_FAILURE,
    TEST_RESULT_SUCCESS
};

enum state_t {
    WAITING_EVENT_PACKET,
    WAITING_EVENT_CODE_COMPLETE,
    WAITING_PARAMETER_LENGTH,
    WAITING_STATUS,
    WAITING_NUM_HCI_EVT_PACKET,
    WAITING_OPCODE_LSB,
    WAITING_OPCODE_MSB,
    DONE
};

static state_t state = WAITING_EVENT_PACKET;
static test_result_t test_result = TEST_RESULT_TIMEOUT_FAILURE;

static rtos::Semaphore sem;

static uint8_t reset_cmd[] = {
    HCI_OPCODE_RESET_LSB, HCI_OPCODE_RESET_MSB,  // reset opcode
    0 // parameter length
};

static void hci_driver_rx_dummy_handler(uint8_t* data, uint8_t len) { }

static void hci_driver_rx_reset_handler(uint8_t* data, uint8_t len) {
    for (size_t i = 0; i < len; ++i) {
        switch (state) {
            case WAITING_EVENT_PACKET:
                if (data[i] == HCI_EVT_TYPE) {
                    state = WAITING_EVENT_CODE_COMPLETE;
                } else {
                    test_result = TEST_RESULT_FAILURE;
                }
                break;

            case WAITING_EVENT_CODE_COMPLETE:
                if (data[i] == HCI_CMD_CMPL_EVT) {
                    state = WAITING_PARAMETER_LENGTH;
                } else {
                    test_result = TEST_RESULT_FAILURE;
                }
                break;

            case WAITING_PARAMETER_LENGTH:
                if (data[i] == RESET_PARAMETER_LENGTH) {
                    state = WAITING_NUM_HCI_EVT_PACKET;
                } else {
                    test_result = TEST_RESULT_FAILURE;
                }
                break;

            case WAITING_NUM_HCI_EVT_PACKET:
                // controler dependent; can be any value, pass on to the next token
                state = WAITING_OPCODE_LSB;
                break;

            case WAITING_OPCODE_LSB:
                if (data[i] == HCI_OPCODE_RESET_LSB) {
                    state = WAITING_OPCODE_MSB;
                } else {
                    test_result = TEST_RESULT_FAILURE;
                }
                break;

            case WAITING_OPCODE_MSB:
                if (data[i] == HCI_OPCODE_RESET_MSB) {
                    state = WAITING_STATUS;
                } else {
                    test_result = TEST_RESULT_FAILURE;
                }
                break;

            case WAITING_STATUS:
                if (data[i] == RESET_EXPECTED_STATUS) {
                    test_result = TEST_RESULT_SUCCESS;
                    state = DONE;
                } else {
                    test_result = TEST_RESULT_FAILURE;
                }
                break;
        }

        if (test_result != TEST_RESULT_TIMEOUT_FAILURE) {
            CordioHCIHook::set_data_received_handler(hci_driver_rx_dummy_handler);
            sem.release();
            return;
        }
    }
}

void test_reset_command() {
    CordioHCIDriver& driver = CordioHCIHook::get_driver();
    CordioHCITransportDriver& transport_driver = CordioHCIHook::get_transport_driver();

    driver.initialize();

    CordioHCIHook::set_data_received_handler(hci_driver_rx_reset_handler);
    transport_driver.write(HCI_CMD_TYPE, sizeof(reset_cmd), reset_cmd);
    sem.wait(RESET_COMMAND_TIMEOUT);
    CordioHCIHook::set_data_received_handler(hci_driver_rx_dummy_handler);

    driver.terminate();

    TEST_ASSERT_EQUAL(TEST_RESULT_SUCCESS, test_result);
    TEST_ASSERT_EQUAL(DONE, state);
}

Case cases[] = {
    Case("Test reset command", test_reset_command),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(15, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    return !Harness::run(specification);
}
