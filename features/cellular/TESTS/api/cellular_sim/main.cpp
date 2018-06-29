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

#include "CellularConnectionFSM.h"
#include "CellularDevice.h"
#include "../../cellular_tests_common.h"
#include CELLULAR_STRINGIFY(CELLULAR_DEVICE.h)

#define NETWORK_TIMEOUT (180*1000)

static UARTSerial cellular_serial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
static EventQueue queue(8 * EVENTS_EVENT_SIZE);
static rtos::Semaphore network_semaphore(0);
static CellularConnectionFSM cellular;
static CellularConnectionFSM::CellularState cellular_target_state;

static char *get_rand_string(char *str, size_t size)
{
    const char charset[] = "0123456789";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

static bool fsm_callback(int state, int next_state)
{
    if (next_state == CellularConnectionFSM::STATE_SIM_PIN) {
        TEST_ASSERT(network_semaphore.release() == osOK);
        return false;
    }
    return true;
}

static void init_to_sim_state()
{
    cellular.set_serial(&cellular_serial);
    TEST_ASSERT(cellular.init() == NSAPI_ERROR_OK);
#if defined (MDMRTS) && defined (MDMCTS)
    cellular_serial.set_flow_control(SerialBase::RTSCTS, MDMRTS, MDMCTS);
#endif
    cellular.set_callback(&fsm_callback);
    TEST_ASSERT(cellular.start_dispatch() == NSAPI_ERROR_OK);
    cellular_target_state = CellularConnectionFSM::STATE_SIM_PIN;
    TEST_ASSERT(cellular.continue_to_state(cellular_target_state) == NSAPI_ERROR_OK);
    TEST_ASSERT(network_semaphore.wait(NETWORK_TIMEOUT) == 1);
}

static void test_sim_interface()
{
    CellularSIM *sim = cellular.get_sim();
    TEST_ASSERT(sim != NULL);

    // 1. test set_pin
    nsapi_error_t err = sim->set_pin(MBED_CONF_APP_CELLULAR_SIM_PIN);
    MBED_ASSERT(err == NSAPI_ERROR_OK);

    // 2. test change pin
    char pin[5];
    int sanity_count = 0;
    while (strcmp(get_rand_string(pin, 5), MBED_CONF_APP_CELLULAR_SIM_PIN) == 0) {
        sanity_count++;
        TEST_ASSERT(sanity_count < 50);
    };

    // change pin and change it back
    err = sim->change_pin(MBED_CONF_APP_CELLULAR_SIM_PIN, pin);
    TEST_ASSERT(err == NSAPI_ERROR_OK);
    err = sim->change_pin(pin, MBED_CONF_APP_CELLULAR_SIM_PIN);
    TEST_ASSERT(err == NSAPI_ERROR_OK);

    // 3. test set_pin_query
    err = sim->set_pin_query(MBED_CONF_APP_CELLULAR_SIM_PIN, false);
    TEST_ASSERT(err == NSAPI_ERROR_OK);
    err = sim->set_pin_query(MBED_CONF_APP_CELLULAR_SIM_PIN, true);
    TEST_ASSERT(err == NSAPI_ERROR_OK);

    // 4. test get_sim_state
    CellularSIM::SimState state;
    err = sim->get_sim_state(state);
    TEST_ASSERT(err == NSAPI_ERROR_OK);
    TEST_ASSERT(state == CellularSIM::SimStateReady);

    // 5. test get_imsi
    char imsi[16] = {0};
    err = sim->get_imsi(imsi);
    TEST_ASSERT(err == NSAPI_ERROR_OK);
    TEST_ASSERT(strlen(imsi) > 0);
}

using namespace utest::v1;

static utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_ABORT;
}

static Case cases[] = {
    Case("CellularSIM init", init_to_sim_state, greentea_failure_handler),
    Case("CellularSIM test interface", test_sim_interface, greentea_failure_handler)
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
