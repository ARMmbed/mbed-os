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

#ifndef MBED_CONF_APP_CELLULAR_SIM_PIN
#error [NOT_SUPPORTED] SIM pin code is needed. Skipping this build.
#endif

#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "mbed.h"

#include "CellularDevice.h"
#include "../../cellular_tests_common.h"
#include CELLULAR_STRINGIFY(CELLULAR_DEVICE.h)

#define NETWORK_TIMEOUT (180*1000)

static UARTSerial cellular_serial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
static CELLULAR_DEVICE* cellular_device;
static EventQueue queue(2 * EVENTS_EVENT_SIZE);

static void urc_callback()
{
}

static void wait_for_power(CellularPower* pwr)
{
    nsapi_error_t err = pwr->set_device_ready_urc_cb(&urc_callback);
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_UNSUPPORTED);

    int sanity_count = 0;
    while (pwr->is_device_ready() != NSAPI_ERROR_OK) {
        sanity_count++;
        wait(1);
        TEST_ASSERT(sanity_count < 20);
    }

    err = pwr->set_at_mode();
    TEST_ASSERT(err == NSAPI_ERROR_OK);

    pwr->remove_device_ready_urc_cb(&urc_callback);
}

static void test_power_interface()
{
    cellular_device = new CELLULAR_DEVICE(queue);
    CellularPower* pwr = cellular_device->open_power(&cellular_serial);

    nsapi_error_t err = pwr->on();
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_UNSUPPORTED);
    wait_for_power(pwr);

    TEST_ASSERT(pwr->set_power_level(1,0) == NSAPI_ERROR_OK);

    err = pwr->reset();
    TEST_ASSERT(err == NSAPI_ERROR_OK);
    wait_for_power(pwr);

    err = pwr->off();
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_UNSUPPORTED);
}

using namespace utest::v1;

static utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_ABORT;
}

static Case cases[] = {
    Case("CellularPower test interface", test_power_interface, greentea_failure_handler)
};

static utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10*60, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

static Specification specification(test_setup, cases);

int main()
{
#if MBED_CONF_MBED_TRACE_ENABLE
    trace_open();
#endif
    int ret = Harness::run(specification);
#if MBED_CONF_MBED_TRACE_ENABLE
    trace_close();
#endif
    return ret;
}
