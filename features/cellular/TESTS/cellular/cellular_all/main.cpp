/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#include "CellularTests.h"

#if !defined(MBED_CONF_NSAPI_PRESENT)
#error [NOT_SUPPORTED] A json configuration file is needed. Skipping this build.
#endif

#ifndef CELLULAR_DEVICE
#error [NOT_SUPPORTED] CELLULAR_DEVICE must be defined for this test
#endif

EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;
CELLULAR_DEVICE cellularDevice(queue);

UARTSerial serial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
CellularNetwork *network = NULL;
CellularPower *pwr = NULL;
CellularSIM *sim = NULL;
CellularSMS *sms = NULL;

using namespace utest::v1;

// using namespace mbed;
utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    // power test
    Case("Create power", test_create_power, greentea_failure_handler),
#ifdef MBED_CONF_APP_CELLULAR_SIM_PIN
    // sim test
    Case("test get SIM state", test_get_sim_state, greentea_failure_handler),
    Case("SIM set pin", test_set_pin, greentea_failure_handler),
    Case("SIM change pin", test_change_pin, greentea_failure_handler),
#endif
    // network tests
    Case("attach", test_attach, greentea_failure_handler),
    // SMS tests
    Case("SMS init", test_sms_init, greentea_failure_handler),
    // network tests
    Case("connect", test_connect, greentea_failure_handler),
    Case("get_ip_address", test_get_ip_address, greentea_failure_handler),
    // stack tests
    Case("open", test_socket_open, greentea_failure_handler),
    Case("bind", test_socket_bind, greentea_failure_handler),
//    Case("set socket blocking", test_socket_set_blocking, greentea_failure_handler),
//    Case("socket send receive in blocking mode", test_socket_send_receive_blocking, greentea_failure_handler),
    Case("set socket non blocking", test_socket_set_non_blocking, greentea_failure_handler),
    Case("socket send receive in non blocking mode", test_socket_send_receive_non_blocking, greentea_failure_handler),
    Case("close", test_socket_close, greentea_failure_handler),
    // network tests
    Case("disconnect", test_disconnect, greentea_failure_handler),

    // test closing of all interface, must be the last test case
    Case("Close all Interfaces", test_close_interfaces, greentea_failure_handler)
};

utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(300, "default_auto");

    return verbose_test_setup_handler(number_of_cases);
}

Specification specification(test_setup, cases);

void test_close_interfaces()
{
    // SMS is already closed in it's test
    cellularDevice.close_network();
    cellularDevice.close_sim();
    cellularDevice.close_power();
}

int main()
{
#if defined (MDMRTS) && defined (MDMCTS)
    serial.set_flow_control(SerialBase::RTSCTS, MDMRTS, MDMCTS);
#endif
    pwr = cellularDevice.open_power(&serial);
    sim = cellularDevice.open_sim(&serial);
    sms = cellularDevice.open_sms(&serial);
    network = cellularDevice.open_network(&serial);

    t.start(callback(&queue, &EventQueue::dispatch_forever));
    return Harness::run(specification);
}
