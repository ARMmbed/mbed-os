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

#if !MBED_CONF_BLE_PRESENT
#error [NOT_SUPPORTED] BLE cordio test cases require the BLE library.
#else

#include <stdio.h>
#include <algorithm>

#include "driver/CordioHCITransportDriver.h"
#include "driver/CordioHCIDriver.h"
#include "hci_defs.h"
#include "rtos/EventFlags.h"

#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"

using namespace utest::v1;

using ble::vendor::cordio::CordioHCIDriver;
using ble::vendor::cordio::CordioHCITransportDriver;

extern ble::vendor::cordio::CordioHCIDriver &ble_cordio_get_hci_driver();

#if CORDIO_ZERO_COPY_HCI
#error [NOT_SUPPORTED] Test not relevant for zero copy hci.
#else

namespace ble {
namespace vendor {
namespace cordio {

struct CordioHCIHook {
    static CordioHCIDriver &get_driver()
    {
        return ble_cordio_get_hci_driver();
    }

    static CordioHCITransportDriver &get_transport_driver()
    {
        return get_driver()._transport_driver;
    }

    static void set_data_received_handler(void (*handler)(uint8_t *, uint8_t))
    {
        get_transport_driver().set_data_received_handler(handler);
    }
};

} // namespace cordio
} // namespace vendor
} // namespace ble

using ble::vendor::cordio::CordioHCIHook;

//
// Handle signal mechanism
//
#define RESET_COMMAND_TIMEOUT (10 * 1000)

static const uint32_t RESET_RECEIVED_FLAG = 1 << 0;
static const uint32_t RECEPTION_ERROR_FLAG = 1 << 1;
static const uint32_t RESET_STATUS_ERROR_FLAG = 1 << 2;

static const uint32_t WAITING_FLAGS =
    RESET_RECEIVED_FLAG | RECEPTION_ERROR_FLAG | RESET_STATUS_ERROR_FLAG;

static rtos::EventFlags event_channel;

static void signal_flag(uint32_t flag)
{
    if (!(event_channel.get() & flag)) {
        event_channel.set(flag);
    }
}

uint32_t wait_for_event()
{
    // clear reception flags
    uint32_t flags = event_channel.get();
    event_channel.clear(flags & ~RESET_RECEIVED_FLAG);

    return event_channel.wait_any(
               WAITING_FLAGS,
               /* timeout */ RESET_COMMAND_TIMEOUT,
               /* clear */ false
           );
}

//
// Handle reset command reception
//

#define RESET_PARAMETER_LENGTH  4
#define RESET_EXPECTED_STATUS  0
#define HCI_OPCODE_RESET_LSB (HCI_OPCODE_RESET & 0xFF)
#define HCI_OPCODE_RESET_MSB (HCI_OPCODE_RESET >> 8)
#define RESET_PACKET_LENGTH (1 + HCI_EVT_HDR_LEN + RESET_PARAMETER_LENGTH)
#define RESET_STATUS_INDEX 6

static bool is_reset_event(const uint8_t *data, uint16_t len)
{
    if (len != RESET_PACKET_LENGTH) {
        return false;
    }

    if (*data++ != HCI_EVT_TYPE) {
        return false;
    }

    if (*data++ != HCI_CMD_CMPL_EVT) {
        return false;
    }

    if (*data++ != RESET_PARAMETER_LENGTH) {
        return false;
    }

    // Note skip num of HCI packet as this is controller dependent
    data++;

    if (*data++ != HCI_OPCODE_RESET_LSB) {
        return false;
    }

    if (*data++ != HCI_OPCODE_RESET_MSB) {
        return false;
    }

    return true;
}

static void hci_driver_rx_reset_handler(uint8_t *data, uint8_t len)
{
    enum packet_state_t {
        WAITING_FOR_PACKET_TYPE,
        WAITING_FOR_HEADER_COMPLETE,
        WAITING_FOR_DATA_COMPLETE,
        SYNCHRONIZATION_ERROR,
        STATUS_ERROR
    };

    static uint8_t packet[256] = { 0 };
    static uint16_t position = 0;
    static uint16_t packet_length;
    static packet_state_t reception_state = WAITING_FOR_PACKET_TYPE;

    while (len) {
        switch (reception_state) {
            case WAITING_FOR_PACKET_TYPE:
                if (*data != HCI_EVT_TYPE) {
                    reception_state = SYNCHRONIZATION_ERROR;
                    signal_flag(RECEPTION_ERROR_FLAG);
                    return;
                }

                packet[position++] = *data++;
                --len;
                packet_length = 1 + HCI_EVT_HDR_LEN;
                reception_state = WAITING_FOR_HEADER_COMPLETE;
                break;

            case WAITING_FOR_HEADER_COMPLETE:
            case WAITING_FOR_DATA_COMPLETE: {
                uint16_t step = std::min((uint16_t) len, (uint16_t)(packet_length - position));
                memcpy(packet + position, data, step);
                position += step;
                data += step;
                len -= step;

                if (reception_state == WAITING_FOR_HEADER_COMPLETE &&
                        position == packet_length
                   ) {
                    reception_state = WAITING_FOR_DATA_COMPLETE;
                    packet_length += packet[HCI_EVT_HDR_LEN];
                }
            }
            break;


            // dead end; we never exit from the error state; just asignal it again.
            case SYNCHRONIZATION_ERROR:
                signal_flag(RECEPTION_ERROR_FLAG);
                return;

            case STATUS_ERROR:
                signal_flag(RESET_STATUS_ERROR_FLAG);
                return;
        }

        bool packet_complete = (reception_state == WAITING_FOR_DATA_COMPLETE) &&
                               (position == packet_length);

        if (packet_complete) {
            if (is_reset_event(packet, packet_length)) {
                if (packet[RESET_STATUS_INDEX] != RESET_EXPECTED_STATUS) {
                    reception_state = STATUS_ERROR;
                    signal_flag(RESET_STATUS_ERROR_FLAG);
                    return;
                } else {
                    signal_flag(RESET_RECEIVED_FLAG);
                }
            }

            reception_state = WAITING_FOR_PACKET_TYPE;
            position = 0;
            packet_length = 1;
        }
    }
}

static uint8_t reset_cmd[] = {
    HCI_OPCODE_RESET_LSB, HCI_OPCODE_RESET_MSB,  // reset opcode
    0 // parameter length
};

#define EXPECTED_CONSECUTIVE_RESET   10

void test_multiple_reset_command()
{
    CordioHCIDriver &driver = CordioHCIHook::get_driver();
    CordioHCITransportDriver &transport_driver = CordioHCIHook::get_transport_driver();

    driver.initialize();

    CordioHCIHook::set_data_received_handler(hci_driver_rx_reset_handler);

    for (size_t i = 0; i < EXPECTED_CONSECUTIVE_RESET; ++i) {
        transport_driver.write(HCI_CMD_TYPE, sizeof(reset_cmd), reset_cmd);
        uint32_t events = wait_for_event();
        TEST_ASSERT_EQUAL(RESET_RECEIVED_FLAG, events);
        if (events != RESET_RECEIVED_FLAG) {
            break;
        }
    }

    driver.terminate();
}

Case cases[] = {
    Case("Test multiple reset commands", test_multiple_reset_command)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main()
{
    return !Harness::run(specification);
}
#endif // CORDIO_ZERO_COPY_HCI

#endif //!MBED_CONF_BLE_PRESENT
