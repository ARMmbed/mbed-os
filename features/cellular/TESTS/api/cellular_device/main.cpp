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

#ifndef MBED_CONF_APP_CELLULAR_SIM_PIN
#error [NOT_SUPPORTED] SIM pin code is needed. Skipping this build.
#endif

#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "mbed.h"

#include "CellularLog.h"
#include "CellularDevice.h"
#include "Semaphore.h"
#include "../../cellular_tests_common.h"

static CellularDevice *device;
static rtos::Semaphore semaphore;


const int TIME_OUT_DEVICE_READY = 5 * 60 * 1000; // 5 minutes
const int TIME_OUT_REGISTER = 10 * 60 * 1000; // 10 minutes

enum CurrentOp {
    OP_DEVICE_READY,
    OP_SIM_READY,
    OP_REGISTER,
    OP_ATTACH
};
static CurrentOp op;

static void create_device()
{
    device = CellularDevice::get_default_instance();
    TEST_ASSERT(device != NULL);
}

static void open_close_interfaces()
{
    CellularNetwork *nw = device->open_network();
    TEST_ASSERT(nw != NULL);
    device->close_network();

    CellularInformation *info = device->open_information();
    TEST_ASSERT(info != NULL);
    device->close_information();

    CellularSMS *sms = device->open_sms();
    TEST_ASSERT(sms != NULL);
    device->close_sms();

    CellularContext *ctx = device->create_context();
    TEST_ASSERT(ctx != NULL);
    TEST_ASSERT(device->get_context_list() == ctx);
    device->delete_context(ctx);
}

static void other_methods()
{
    // test first without any open interfaces
    device->set_timeout(5000);
    device->modem_debug_on(true);
    device->modem_debug_on(false);

    CellularNetwork *nw = device->open_network();
    TEST_ASSERT(nw != NULL);

    // then test with open interface which is called
    device->set_timeout(5000);
    device->modem_debug_on(true);
    device->modem_debug_on(false);

    TEST_ASSERT(device->get_queue() != NULL);
    TEST_ASSERT(device->hard_power_on() == NSAPI_ERROR_OK);
    TEST_ASSERT(device->soft_power_on() == NSAPI_ERROR_OK);
    wait(5);
    TEST_ASSERT_EQUAL_INT(device->init(), NSAPI_ERROR_OK);
}

static void shutdown()
{
    TEST_ASSERT(device->shutdown() == NSAPI_ERROR_OK);
    TEST_ASSERT(device->soft_power_off() == NSAPI_ERROR_OK);
    TEST_ASSERT(device->hard_power_off() == NSAPI_ERROR_OK);
}

static void callback_func(nsapi_event_t ev, intptr_t ptr)
{
    if (ev >= NSAPI_EVENT_CELLULAR_STATUS_BASE && ev <= NSAPI_EVENT_CELLULAR_STATUS_END) {
        cell_callback_data_t *ptr_data = (cell_callback_data_t *)ptr;
        cellular_connection_status_t cell_ev = (cellular_connection_status_t)ev;
        if (cell_ev == CellularDeviceReady && ptr_data->error == NSAPI_ERROR_OK && op == OP_DEVICE_READY) {
            TEST_ASSERT_EQUAL_INT(semaphore.release(), osOK);
        } else if (cell_ev == CellularSIMStatusChanged && ptr_data->error == NSAPI_ERROR_OK &&
                   ptr_data->status_data == CellularDevice::SimStateReady && op == OP_SIM_READY) {
            TEST_ASSERT_EQUAL_INT(semaphore.release(), osOK);
        } else if (cell_ev == CellularRegistrationStatusChanged &&
                   (ptr_data->status_data == CellularNetwork::RegisteredHomeNetwork ||
                    ptr_data->status_data == CellularNetwork::RegisteredRoaming ||
                    ptr_data->status_data == CellularNetwork::AlreadyRegistered) &&
                   ptr_data->error == NSAPI_ERROR_OK &&
                   op == OP_REGISTER) {
            TEST_ASSERT_EQUAL_INT(semaphore.release(), osOK);
        } else if (cell_ev == CellularAttachNetwork  && ptr_data->status_data == CellularNetwork::Attached &&
                   ptr_data->error == NSAPI_ERROR_OK && op == OP_ATTACH) {
            TEST_ASSERT_EQUAL_INT(semaphore.release(), osOK);
        }
    }
}

static void init_to_device_ready_state()
{
    device = CellularDevice::get_default_instance();
    TEST_ASSERT(device != NULL);

#ifdef MBED_CONF_APP_CELLULAR_SIM_PIN
    device->set_sim_pin(MBED_CONF_APP_CELLULAR_SIM_PIN);
#endif
#ifdef MBED_CONF_APP_CELLULAR_PLMN
    device->set_plmn(MBED_CONF_APP_CELLULAR_PLMN);
#endif
    device->attach(&callback_func);

    op = OP_DEVICE_READY;
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, device->hard_power_on());
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, device->soft_power_on());
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, device->init());
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, device->set_device_ready());

    int sema_err = semaphore.wait(TIME_OUT_DEVICE_READY);
    TEST_ASSERT_EQUAL_INT(1, sema_err);
}

static void continue_to_sim_ready_state()
{
    op = OP_SIM_READY;
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, device->set_sim_ready());

    int sema_err = semaphore.wait(TIME_OUT_DEVICE_READY);
    TEST_ASSERT_EQUAL_INT(1, sema_err);
}

static void continue_to_register_state()
{
    op = OP_REGISTER;
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, device->register_to_network());

    int sema_err = semaphore.wait(TIME_OUT_REGISTER); // cellular network searching may take several minutes
    TEST_ASSERT_EQUAL_INT(1, sema_err);
}

static void continue_to_attach_state()
{
    op = OP_ATTACH;
    TEST_ASSERT_EQUAL_INT(NSAPI_ERROR_OK, device->attach_to_network());

    int sema_err = semaphore.wait(TIME_OUT_REGISTER); // cellular network attach may take several minutes
    TEST_ASSERT_EQUAL_INT(1, sema_err);
}

using namespace utest::v1;

static utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
#if MBED_CONF_MBED_TRACE_ENABLE
    trace_close();
#endif
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_ABORT;
}

static Case cases[] = {
    Case("CellularDevice create device", create_device, greentea_failure_handler),
    Case("CellularDevice Open and close interfaces", open_close_interfaces, greentea_failure_handler),
    Case("CellularDevice other methods", other_methods, greentea_failure_handler),
    Case("CellularDevice init to device ready", init_to_device_ready_state, greentea_failure_handler),
    Case("CellularDevice sim ready", continue_to_sim_ready_state, greentea_failure_handler),
    Case("CellularDevice register", continue_to_register_state, greentea_failure_handler),
    Case("CellularDevice attach", continue_to_attach_state, greentea_failure_handler),
    Case("CellularDevice shutdown", shutdown, greentea_failure_handler),
};

static utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(10 * 60, "default_auto");
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

