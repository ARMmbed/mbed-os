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

#define SIM_TIMEOUT (180*1000)

static UARTSerial cellular_serial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
static EventQueue queue(2 * EVENTS_EVENT_SIZE);
static CellularConnectionFSM cellular;
static CellularConnectionFSM::CellularState cellular_target_state;
static rtos::Semaphore fsm_semaphore(0);

static bool fsm_callback(int state, int next_state)
{
    if (next_state == CellularConnectionFSM::STATE_SIM_PIN) {
        TEST_ASSERT(fsm_semaphore.release() == osOK);
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
    TEST_ASSERT(fsm_semaphore.wait(SIM_TIMEOUT) == 1);
}

static void test_information_interface()
{
    CellularInformation *info = cellular.get_device()->open_information(&cellular_serial);
    const int kbuf_size = 100;
    char *buf = (char*)malloc(sizeof(char) * kbuf_size);

    TEST_ASSERT(info->get_manufacturer(buf, kbuf_size) == NSAPI_ERROR_OK);
    TEST_ASSERT(info->get_model(buf, kbuf_size) == NSAPI_ERROR_OK);
    TEST_ASSERT(info->get_revision(buf, kbuf_size) == NSAPI_ERROR_OK);
    TEST_ASSERT(info->get_serial_number(buf, kbuf_size, CellularInformation::SN) == NSAPI_ERROR_OK);

    nsapi_error_t err = info->get_serial_number(buf, kbuf_size, CellularInformation::IMEI);
    TEST_ASSERT(err == NSAPI_ERROR_UNSUPPORTED || err == NSAPI_ERROR_OK);

    err = info->get_serial_number(buf, kbuf_size, CellularInformation::IMEISV);
    TEST_ASSERT(err == NSAPI_ERROR_UNSUPPORTED || err == NSAPI_ERROR_OK);

    err = info->get_serial_number(buf, kbuf_size, CellularInformation::SVN);
    TEST_ASSERT(err == NSAPI_ERROR_UNSUPPORTED || err == NSAPI_ERROR_OK);

    cellular.get_device()->close_information();

    free(buf);
}

using namespace utest::v1;

static utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_ABORT;
}

static Case cases[] = {
    Case("CellularInformation init", init_to_sim_state, greentea_failure_handler),
    Case("CellularInformation test interface", test_information_interface, greentea_failure_handler)
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
