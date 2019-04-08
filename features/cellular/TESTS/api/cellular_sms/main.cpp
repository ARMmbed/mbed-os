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

#if defined(TARGET_ADV_WISE_1570) || defined(TARGET_MTB_ADV_WISE_1570)
#error [NOT_SUPPORTED] target MTB_ADV_WISE_1570 does not have SMS functionality
#endif

#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "mbed.h"

#include "AT_CellularSMS.h"
#include "CellularContext.h"
#include "CellularDevice.h"
#include "../../cellular_tests_common.h"

#define NETWORK_TIMEOUT (600*1000)
#define SIM_BUSY 314

static CellularContext *ctx;
static CellularDevice *device;
static CellularSMS *sms;
static char service_center_address[SMS_MAX_PHONE_NUMBER_SIZE];
static int service_address_type;

static void create_context()
{
    device = CellularDevice::get_default_instance();
    TEST_ASSERT(device != NULL);
    device->set_timeout(9000);
    ctx = CellularContext::get_default_instance();
    TEST_ASSERT(ctx != NULL);
    ctx->set_sim_pin(MBED_CONF_APP_CELLULAR_SIM_PIN);
#ifdef MBED_CONF_APP_APN
    ctx->set_credentials(MBED_CONF_APP_APN);
#endif
}

static void store_service_center_address()
{
    // First we need to go SIM_PIN state to make sure that we can get service address and device ready to accept AT commands
    create_context();
    TEST_ASSERT(ctx->set_sim_ready() == NSAPI_ERROR_OK);

    wait(5); // some modems needs more time access sim

    ATHandler *at_init = device->get_at_handler();
    at_init->lock();
    at_init->set_at_timeout(30 * 1000);
    at_init->cmd_start("AT+CSCA?");
    at_init->cmd_stop();

    TEST_ASSERT(at_init->get_last_error() == NSAPI_ERROR_OK);

    at_init->resp_start("+CSCA:");
    at_init->read_string(service_center_address, sizeof(service_center_address));
    service_address_type = at_init->read_int();
    at_init->resp_stop();
    TEST_ASSERT(at_init->get_last_error() == NSAPI_ERROR_OK);

    at_init->unlock();
    device->release_at_handler(at_init);
}

static void init()
{
    // First store current service address
    store_service_center_address();
    create_context();

    TEST_ASSERT(ctx->register_to_network() == NSAPI_ERROR_OK);
    sms = device->open_sms();
    TEST_ASSERT(sms != NULL);
    wait(3); // some modems needs more time access sim
}

static void test_sms_initialize_text_mode()
{
    nsapi_error_t err = sms->initialize(CellularSMS::CellularSMSMmodeText);
    TEST_ASSERT(err == NSAPI_ERROR_OK || (err == NSAPI_ERROR_DEVICE_ERROR &&
                                          ((AT_CellularSMS *)sms)->get_device_error().errCode == SIM_BUSY));

}

static void test_sms_initialize_pdu_mode()
{
    nsapi_error_t err = sms->initialize(CellularSMS::CellularSMSMmodePDU);
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_UNSUPPORTED || (err == NSAPI_ERROR_DEVICE_ERROR &&
                                                                            ((AT_CellularSMS *)sms)->get_device_error().errCode == SIM_BUSY));
}

static void test_set_cscs()
{
    nsapi_error_t err = sms->set_cscs("IRA");
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_UNSUPPORTED || (err == NSAPI_ERROR_DEVICE_ERROR &&
                                                                            ((AT_CellularSMS *)sms)->get_device_error().errCode == SIM_BUSY));
    err = sms->set_cscs("UCS2");
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_UNSUPPORTED || (err == NSAPI_ERROR_DEVICE_ERROR &&
                                                                            ((AT_CellularSMS *)sms)->get_device_error().errCode == SIM_BUSY));
    err = sms->set_cscs("GSM");
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_UNSUPPORTED || (err == NSAPI_ERROR_DEVICE_ERROR &&
                                                                            ((AT_CellularSMS *)sms)->get_device_error().errCode == SIM_BUSY));
}

static void test_set_csca()
{
    nsapi_error_t err = sms->set_csca("55555", 129);
    TEST_ASSERT(err == NSAPI_ERROR_OK || (err == NSAPI_ERROR_DEVICE_ERROR &&
                                          ((AT_CellularSMS *)sms)->get_device_error().errCode == SIM_BUSY));
    err = sms->set_csca("+35855555", 145);
    TEST_ASSERT(err == NSAPI_ERROR_OK || (err == NSAPI_ERROR_DEVICE_ERROR &&
                                          ((AT_CellularSMS *)sms)->get_device_error().errCode == SIM_BUSY));
    err = sms->set_csca(service_center_address, service_address_type);
    TEST_ASSERT(err == NSAPI_ERROR_OK || (err == NSAPI_ERROR_DEVICE_ERROR &&
                                          ((AT_CellularSMS *)sms)->get_device_error().errCode == SIM_BUSY));
}

static void test_set_cpms_me()
{
    nsapi_error_t err = sms->set_cpms("ME", "ME", "ME");
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_UNSUPPORTED || (err == NSAPI_ERROR_DEVICE_ERROR &&
                                                                            ((AT_CellularSMS *)sms)->get_device_error().errCode == SIM_BUSY));
}

#ifdef MBED_CONF_APP_CELLULAR_PHONE_NUMBER
static const char TEST_MESSAGE[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static int callbacks_received = 0;

static void sms_callback()
{
    callbacks_received++;
}

static void test_set_sms_callback()
{
    sms->set_sms_callback(sms_callback);
}


static void test_set_cpms_sm()
{
    TEST_ASSERT(sms->set_cpms("SM", "SM", "SM") == NSAPI_ERROR_OK);
}

static void test_sms_send()
{
    const int msg_len = strlen(TEST_MESSAGE);
    TEST_ASSERT(sms->send_sms(MBED_CONF_APP_CELLULAR_PHONE_NUMBER, TEST_MESSAGE, msg_len) == msg_len);
}

static void test_get_sms()
{
    uint16_t buf_len = sizeof(TEST_MESSAGE);
    char buf[buf_len];

    char phone_num[SMS_MAX_PHONE_NUMBER_SIZE];

    char time_stamp[SMS_MAX_TIME_STAMP_SIZE];

    int buf_size = 0;

    wait(7);

    TEST_ASSERT(sms->get_sms(buf, buf_len, phone_num, SMS_MAX_PHONE_NUMBER_SIZE, time_stamp, SMS_MAX_TIME_STAMP_SIZE, &buf_size) == buf_len - 1);
    TEST_ASSERT(strcmp(phone_num, MBED_CONF_APP_CELLULAR_PHONE_NUMBER) == 0);
    TEST_ASSERT(strcmp(buf, TEST_MESSAGE) == 0);
    TEST_ASSERT(buf_size == 0);
    TEST_ASSERT(callbacks_received > 0);
    callbacks_received = 0;

}

static void test_delete_all_messages()
{
    //send a message so that there is something to delete
    test_sms_send();
    wait(7);
    TEST_ASSERT(sms->delete_all_messages() == NSAPI_ERROR_OK);
    callbacks_received = 0;
}

static void test_set_extra_sim_wait_time_1000()
{
    sms->set_extra_sim_wait_time(1000);
}

#endif

using namespace utest::v1;

static utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_ABORT;
}

static Case cases[] = {
    Case("CellularSMS init", init, greentea_failure_handler),
    Case("CellularSMS test ME for storage", test_set_cpms_me, greentea_failure_handler),
    Case("CellularSMS test initialize to PDU mode", test_sms_initialize_pdu_mode, greentea_failure_handler),
    Case("CellularSMS test character sets", test_set_cscs, greentea_failure_handler),
    Case("CellularSMS test service center address", test_set_csca, greentea_failure_handler)
#ifdef MBED_CONF_APP_CELLULAR_PHONE_NUMBER
    ,
    Case("CellularSMS test delete all messages", test_delete_all_messages, greentea_failure_handler),
    Case("CellularSMS test sms callback", test_set_sms_callback, greentea_failure_handler),
    Case("CellularSMS test sms send", test_sms_send, greentea_failure_handler),
    Case("CellularSMS test get sms", test_get_sms, greentea_failure_handler),
    Case("CellularSMS test set sim wait time 1s", test_set_extra_sim_wait_time_1000, greentea_failure_handler),
    Case("CellularSMS test SM for storage", test_set_cpms_sm, greentea_failure_handler),
    Case("CellularSMS test initialize to text mode", test_sms_initialize_text_mode, greentea_failure_handler),
    Case("CellularSMS test delete all messages", test_delete_all_messages, greentea_failure_handler),
    Case("CellularSMS test sms send", test_sms_send, greentea_failure_handler),
    Case("CellularSMS test get sms", test_get_sms, greentea_failure_handler),
    Case("CellularSMS test delete all messages", test_delete_all_messages, greentea_failure_handler)
#endif
};

static utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(600, "default_auto");
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
