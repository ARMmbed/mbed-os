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


#if !defined(MBED_CONF_NSAPI_PRESENT) || !defined(MBED_CONF_APP_CELLULAR_SIM_PIN)
#error [NOT_SUPPORTED] A json configuration file is needed & SIM pin code is needed. Skipping this build.
#else

#include "CellularUtil.h" // for CELLULAR_ helper macros

#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "mbed.h"


#include "AT_CellularInformation.h"
#include "CellularContext.h"
#include "CellularDevice.h"
#include "../../cellular_tests_common.h"

static CellularContext *ctx;

static void init_to_sim_state()
{
    ctx = CellularContext::get_default_instance();
    TEST_ASSERT(ctx != NULL);
    ctx->set_sim_pin(MBED_CONF_APP_CELLULAR_SIM_PIN);
#ifdef MBED_CONF_APP_APN
    ctx->set_credentials(MBED_CONF_APP_APN);
#endif
    TEST_ASSERT(ctx->set_sim_ready() == NSAPI_ERROR_OK);
}

static void test_information_interface()
{
    CellularDevice *dev = CellularDevice::get_default_instance();

    CellularInformation *info = dev->open_information();
    const int kbuf_size = 100;
    char *buf = new char[kbuf_size];

    TEST_ASSERT(info->get_manufacturer(buf, kbuf_size) == NSAPI_ERROR_OK);
    TEST_ASSERT(info->get_model(buf, kbuf_size) == NSAPI_ERROR_OK);
    TEST_ASSERT(info->get_revision(buf, kbuf_size) == NSAPI_ERROR_OK);
    TEST_ASSERT((info->get_serial_number(buf, kbuf_size, CellularInformation::SN) == NSAPI_ERROR_OK) ||
                ((((AT_CellularInformation *)info)->get_device_error().errType == 3) &&    // 3 == CME error from the modem
                 (((AT_CellularInformation *)info)->get_device_error().errCode == 4)));     // 4 == "operation not supported"

    nsapi_error_t err = info->get_serial_number(buf, kbuf_size, CellularInformation::IMEI);
    TEST_ASSERT(err == NSAPI_ERROR_UNSUPPORTED || err == NSAPI_ERROR_OK);

    err = info->get_serial_number(buf, kbuf_size, CellularInformation::IMEISV);
    TEST_ASSERT(err == NSAPI_ERROR_UNSUPPORTED || err == NSAPI_ERROR_OK);

    err = info->get_serial_number(buf, kbuf_size, CellularInformation::SVN);
    TEST_ASSERT(err == NSAPI_ERROR_UNSUPPORTED || err == NSAPI_ERROR_OK);

    err = info->get_imsi(buf, kbuf_size);
    TEST_ASSERT(err == NSAPI_ERROR_UNSUPPORTED || err == NSAPI_ERROR_OK);

    err = info->get_iccid(buf, kbuf_size);
    TEST_ASSERT(err == NSAPI_ERROR_UNSUPPORTED || err == NSAPI_ERROR_OK);

    dev->close_information();

    delete [] buf;
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
#endif // !defined(MBED_CONF_NSAPI_PRESENT) || !defined(MBED_CONF_APP_CELLULAR_SIM_PIN)
