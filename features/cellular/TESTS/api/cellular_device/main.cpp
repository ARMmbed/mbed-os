/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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


#if !defined(MBED_CONF_NSAPI_PRESENT)
#error [NOT_SUPPORTED] A json configuration file is needed. Skipping this build.
#endif

#include "CellularUtil.h" // for CELLULAR_ helper macros
#include "CellularTargets.h"

#ifndef CELLULAR_DEVICE
#error [NOT_SUPPORTED] CELLULAR_DEVICE must be defined
#endif

#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "mbed.h"

#include "CellularLog.h"
#include "CellularDevice.h"
#include CELLULAR_STRINGIFY(CELLULAR_DEVICE.h)

static UARTSerial cellular_serial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
static CellularDevice *device;
static EventQueue queue(8 * EVENTS_EVENT_SIZE);

static void create_device()
{
    device = new CELLULAR_DEVICE(queue);
    TEST_ASSERT(device != NULL);
}

static void open_close_interfaces()
{
    CellularNetwork *nw = device->open_network(&cellular_serial);
    TEST_ASSERT(nw != NULL);
    device->close_network();
    nw = device->open_network(NULL);
    TEST_ASSERT(nw == NULL);

    CellularSIM* sim = device->open_sim(&cellular_serial);
    TEST_ASSERT(sim != NULL);
    device->close_sim();
    sim = device->open_sim(NULL);
    TEST_ASSERT(sim == NULL);

    CellularInformation* info = device->open_information(&cellular_serial);
    TEST_ASSERT(info != NULL);
    device->close_information();
    info = device->open_information(NULL);
    TEST_ASSERT(info == NULL);

    CellularPower* power = device->open_power(&cellular_serial);
    TEST_ASSERT(power != NULL);
    device->close_power();
    power = device->open_power(NULL);
    TEST_ASSERT(power == NULL);

    CellularSMS* sms = device->open_sms(&cellular_serial);
    TEST_ASSERT(sms != NULL);
    device->close_sms();
    sms = device->open_sms(NULL);
    TEST_ASSERT(sms == NULL);
}

static void other_methods()
{
    // test first without any open interfaces
    device->set_timeout(5000);
    device->modem_debug_on(true);
    device->modem_debug_on(false);
    NetworkStack* stack = device->get_stack();
    TEST_ASSERT(stack == NULL);

    CellularNetwork *nw = device->open_network(&cellular_serial);
    TEST_ASSERT(nw != NULL);

    // then test witj open interface which is called
    device->set_timeout(5000);
    device->modem_debug_on(true);
    device->modem_debug_on(false);
    stack = device->get_stack();
    TEST_ASSERT(stack != NULL);
}

static void delete_device()
{
    // delete will close all opened interfaces
    delete device;
    device = NULL;
}

using namespace utest::v1;

static utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_ABORT;
}

static Case cases[] = {
    Case("CellularDevice create device", create_device, greentea_failure_handler),
    Case("CellularDevice Open and close interfaces", open_close_interfaces, greentea_failure_handler),
    Case("CellularDevice other methods", other_methods, greentea_failure_handler),
    Case("CellularDevice delete device", delete_device, greentea_failure_handler)
};

static utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10*60, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

static Specification specification(test_setup, cases);

int main()
{
    mbed_trace_init();

    return Harness::run(specification);
}

