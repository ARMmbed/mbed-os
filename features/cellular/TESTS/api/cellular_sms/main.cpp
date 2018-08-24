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

#include "AT_CellularSMS.h"
#include "CellularConnectionFSM.h"
#include "CellularDevice.h"
#include "../../cellular_tests_common.h"
#include CELLULAR_STRINGIFY(CELLULAR_DEVICE.h)

#define NETWORK_TIMEOUT (600*1000)

static UARTSerial cellular_serial(MDMTXD, MDMRXD, MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE);
static EventQueue queue(8 * EVENTS_EVENT_SIZE);
static rtos::Semaphore network_semaphore(0);
static CellularConnectionFSM *cellularConnectionFSM;
static CellularConnectionFSM::CellularState cellular_target_state;
static CellularSMS* sms;
static char service_center_address[SMS_MAX_PHONE_NUMBER_SIZE];
static int service_address_type;

static bool cellular_status(int state, int next_state)
{
    if (cellular_target_state == state) {
        (void)network_semaphore.release();
        return false; // return false -> state machine is halted
    }
    return true;
}

static void createFSM()
{
#if defined (MDMRTS) && defined (MDMCTS)
    cellular_serial.set_flow_control(SerialBase::RTSCTS, MDMRTS, MDMCTS);
#endif
    cellularConnectionFSM = new CellularConnectionFSM();
    cellularConnectionFSM->set_serial(&cellular_serial);
    cellularConnectionFSM->set_callback(&cellular_status);

    TEST_ASSERT(cellularConnectionFSM->init() == NSAPI_ERROR_OK);
    TEST_ASSERT(cellularConnectionFSM->start_dispatch() == NSAPI_ERROR_OK);
    cellularConnectionFSM->set_sim_pin(MBED_CONF_APP_CELLULAR_SIM_PIN);

    CellularDevice *device = cellularConnectionFSM->get_device();
    TEST_ASSERT(device != NULL);
    device->set_timeout(30000);

}
static void store_service_center_address()
{
    // Frist we need to go SIM_PIN state to make sure that we can get service address and device ready to accept AT commands
    createFSM();
    cellular_target_state = CellularConnectionFSM::STATE_SIM_PIN;
    TEST_ASSERT(cellularConnectionFSM->continue_to_state(cellular_target_state) == NSAPI_ERROR_OK);
    TEST_ASSERT(network_semaphore.wait(NETWORK_TIMEOUT) == 1); // cellular network searching may take several minutes

    delete cellularConnectionFSM;
    cellularConnectionFSM = NULL;

    ATHandler *at_init = new ATHandler(&cellular_serial, queue, 30000, "\r");
    at_init->cmd_start("AT+CSCA?");
    at_init->cmd_stop();

    TEST_ASSERT(at_init->get_last_error() == NSAPI_ERROR_OK);

    at_init->resp_start("+CSCA:");
    at_init->read_string(service_center_address, sizeof(service_center_address));
    service_address_type = at_init->read_int();
    at_init->resp_stop();

    TEST_ASSERT(at_init->get_last_error() == NSAPI_ERROR_OK);

    delete at_init;
}

static void init()
{
    // First store current service address
    store_service_center_address();

    createFSM();
    CellularNetwork *network = cellularConnectionFSM->get_network();

    TEST_ASSERT(network != NULL);
    TEST_ASSERT(network->set_credentials(MBED_CONF_APP_APN, NULL, NULL) == NSAPI_ERROR_OK);

    cellular_target_state = CellularConnectionFSM::STATE_REGISTERING_NETWORK;
    TEST_ASSERT(cellularConnectionFSM->continue_to_state(cellular_target_state) == NSAPI_ERROR_OK);
    TEST_ASSERT(network_semaphore.wait(NETWORK_TIMEOUT) == 1); // cellular network searching may take several minutes

    sms = cellularConnectionFSM->get_device()->open_sms(&cellular_serial);
    TEST_ASSERT(sms != NULL);

    wait(3);
}

static void test_sms_initialize_text_mode()
{
    TEST_ASSERT(sms->initialize(CellularSMS::CellularSMSMmodeText) == NSAPI_ERROR_OK);
}

static void test_sms_initialize_pdu_mode()
{
    TEST_ASSERT(sms->initialize(CellularSMS::CellularSMSMmodePDU) == NSAPI_ERROR_OK);
}

static void test_set_cscs()
{
    TEST_ASSERT(sms->set_cscs("IRA") == NSAPI_ERROR_OK);
    TEST_ASSERT(sms->set_cscs("UCS2") == NSAPI_ERROR_OK);
    TEST_ASSERT(sms->set_cscs("GSM") == NSAPI_ERROR_OK);
}

static void test_set_csca()
{
    TEST_ASSERT(sms->set_csca("55555", 129) == NSAPI_ERROR_OK);
    TEST_ASSERT(sms->set_csca("+35855555", 145) == NSAPI_ERROR_OK);
    TEST_ASSERT(sms->set_csca(service_center_address, service_address_type) == NSAPI_ERROR_OK);
}

static void test_set_cpms_me()
{
    TEST_ASSERT(sms->set_cpms("ME", "ME", "ME") == NSAPI_ERROR_OK);
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

    TEST_ASSERT(sms->get_sms(buf, buf_len, phone_num, SMS_MAX_PHONE_NUMBER_SIZE, time_stamp, SMS_MAX_TIME_STAMP_SIZE, &buf_size) == buf_len-1);
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
