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

#include "AT_CellularNetwork.h"
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
static CELLULAR_DEVICE *device;
static CellularNetwork* nw;


static bool fsm_callback(int state, int next_state)
{
    if (next_state == cellular_target_state) {
        TEST_ASSERT(network_semaphore.release() == osOK);
        return false;
    }
    return true;
}

// test methods that are already run in state machine (CellularConnectionFSM) but as it might change
// we wan't to run these 'manually' also
static void test_network_interface_fsm()
{
#if defined (MDMRTS) && defined (MDMCTS)
    cellular_serial.set_flow_control(SerialBase::RTSCTS, MDMRTS, MDMCTS);
#endif
    device = new CELLULAR_DEVICE(queue);
    TEST_ASSERT(device != NULL);

    CellularNetwork* nw = device->open_network(&cellular_serial);
    TEST_ASSERT(nw != NULL);
    TEST_ASSERT(nw->init() == NSAPI_ERROR_OK);

    delete device;
    device = NULL;
}

static void init_network_interface()
{
    cellular.set_serial(&cellular_serial);
    TEST_ASSERT(cellular.init() == NSAPI_ERROR_OK);
    cellular.set_callback(&fsm_callback);
    TEST_ASSERT(cellular.start_dispatch() == NSAPI_ERROR_OK);
    cellular.set_sim_pin(MBED_CONF_APP_CELLULAR_SIM_PIN);
#ifdef MBED_CONF_APP_APN
    CellularNetwork *network = cellular.get_network();
    TEST_ASSERT(network->set_credentials(MBED_CONF_APP_APN) == NSAPI_ERROR_OK);
#endif
    cellular_target_state = CellularConnectionFSM::STATE_REGISTERING_NETWORK;
    TEST_ASSERT(cellular.continue_to_state(cellular_target_state) == NSAPI_ERROR_OK);
    TEST_ASSERT(network_semaphore.wait(NETWORK_TIMEOUT) == 1);
}


static bool get_network_registration(CellularNetwork::RegistrationType type,
        CellularNetwork::RegistrationStatus &status, bool &is_registered)
{
    is_registered = false;
    nsapi_error_t err = nw->get_registration_status(type, status);
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_UNSUPPORTED);

    switch (status) {
        case CellularNetwork::RegisteredRoaming:
        case CellularNetwork::RegisteredHomeNetwork:
            is_registered = true;
            break;
        case CellularNetwork::RegisteredSMSOnlyRoaming:
        case CellularNetwork::RegisteredSMSOnlyHome:
        case CellularNetwork::RegisteredCSFBNotPreferredRoaming:
        case CellularNetwork::RegisteredCSFBNotPreferredHome:
        case CellularNetwork::AttachedEmergencyOnly:
        case CellularNetwork::RegistrationDenied:
        case CellularNetwork::NotRegistered:
        case CellularNetwork::Unknown:
        case CellularNetwork::SearchingNetwork:
        default:
            break;
    }
    return true;
}

static bool is_registered()
{
    CellularNetwork::RegistrationStatus status;
    bool is_registered = false;

    for (int type = 0; type < CellularNetwork::C_MAX; type++) {
        if (get_network_registration((CellularNetwork::RegistrationType) type, status, is_registered)) {
            if (is_registered) {
                break;
            }
        }
    }

    return is_registered;
}

static void nw_callback(nsapi_event_t ev, intptr_t intptr)
{
}

static void test_network_registration()
{
    cellular.get_device()->set_timeout(10*1000);
    nw = cellular.get_network();
    TEST_ASSERT(nw != NULL);

    nw->attach(&nw_callback);

    bool success = false;
    for (int type = 0; type < CellularNetwork::C_MAX; type++) {
        if (!nw->set_registration_urc((CellularNetwork::RegistrationType)type, true)) {
            success = true;
        }
    }
    // each modem should support at least one registration type
    TEST_ASSERT(success);

    int sanity_count = 0;
    while (is_registered() == false) {
        if (sanity_count == 0) {
            TEST_ASSERT(nw->set_registration() == NSAPI_ERROR_OK);
        }
        sanity_count++;
        wait(2);
        TEST_ASSERT(sanity_count < 60);
    }

    // device was registered right away, test set_registration call
    if (sanity_count == 0) {
        TEST_ASSERT(nw->set_registration() == NSAPI_ERROR_OK);
    }

    CellularNetwork::NWRegisteringMode reg_mode = CellularNetwork::NWModeDeRegister;
    TEST_ASSERT(nw->get_network_registering_mode(reg_mode) == NSAPI_ERROR_OK);
    TEST_ASSERT(reg_mode == CellularNetwork::NWModeAutomatic);
}

static void test_attach()
{
    TEST_ASSERT(nw->set_attach() == NSAPI_ERROR_OK);

    CellularNetwork::AttachStatus status;
    TEST_ASSERT(nw->get_attach(status) == NSAPI_ERROR_OK);
    TEST_ASSERT(status == CellularNetwork::Attached);
}

static void test_activate_context()
{
    TEST_ASSERT(nw->activate_context() == NSAPI_ERROR_OK);
}

static void test_connect()
{
    TEST_ASSERT(nw->connect() == NSAPI_ERROR_OK);
    nsapi_connection_status_t st =  nw->get_connection_status();
    TEST_ASSERT(st == NSAPI_STATUS_GLOBAL_UP);
}

static void test_credentials()
{
    TEST_ASSERT(nw->set_credentials(NULL, "username", "pass") == NSAPI_ERROR_OK);
    TEST_ASSERT(nw->set_credentials("internet", NULL, "pass") == NSAPI_ERROR_OK);
    TEST_ASSERT(nw->set_credentials("internet", "user", NULL) == NSAPI_ERROR_OK);
    TEST_ASSERT(nw->set_credentials("internet", CellularNetwork::NOAUTH, "user", "pass") == NSAPI_ERROR_OK);
}

static void test_other()
{
    TEST_ASSERT(nw->get_3gpp_error() == 0);

    CellularNetwork::RateControlExceptionReports reports;
    CellularNetwork::RateControlUplinkTimeUnit timeUnit;
    int uplinkRate;
    // can't test values as they are optional
    nsapi_error_t err = nw->get_rate_control(reports, timeUnit, uplinkRate);
    tr_error("get_rate_control: %d", err);
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_DEVICE_ERROR);
#if CELLULAR_DEVICE != QUECTEL_BG96 // QUECTEL_BG96 does not give any specific reason for device error
    if (err == NSAPI_ERROR_DEVICE_ERROR) {
        TEST_ASSERT(((AT_CellularNetwork*)nw)->get_device_error().errCode == 100 && // 100 == unknown command for modem
                ((AT_CellularNetwork*)nw)->get_device_error().errType == 3); // 3 == CME error from the modem
    }
#endif

    uplinkRate = -1;
    err = nw->get_apn_backoff_timer(uplinkRate);
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_DEVICE_ERROR);
    if (err == NSAPI_ERROR_DEVICE_ERROR) {
#if CELLULAR_DEVICE != QUECTEL_BG96 // QUECTEL_BG96 does not give any specific reason for device error
            TEST_ASSERT(((AT_CellularNetwork*)nw)->get_device_error().errCode == 100 && // 100 == unknown command for modem
                    ((AT_CellularNetwork*)nw)->get_device_error().errType == 3); // 3 == CME error from the modem
#endif
    } else {
        TEST_ASSERT(uplinkRate >= 0);
    }

    err = nw->set_access_technology(CellularNetwork::RAT_GSM);
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_UNSUPPORTED);

    // scanning of operators might take a long time
    cellular.get_device()->set_timeout(240*1000);
    CellularNetwork::operList_t operators;
    TEST_ASSERT(nw->scan_plmn(operators, uplinkRate) == NSAPI_ERROR_OK);
    cellular.get_device()->set_timeout(10*1000);


    // all current targets support IPV4
    nsapi_ip_stack_t stack_type = IPV4_STACK;
    TEST_ASSERT(nw->set_stack_type(stack_type) == NSAPI_ERROR_OK);
    TEST_ASSERT(nw->get_stack_type() == stack_type);

    CellularNetwork::pdpContextList_t params_list;
    err = nw->get_pdpcontext_params(params_list);

    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_DEVICE_ERROR);
    if (err == NSAPI_ERROR_DEVICE_ERROR) {
#if CELLULAR_DEVICE != TELIT_HE910 // TELIT_HE910 just gives an error and no specific error number so we can't know is this real error or that modem/network does not support the command
        TEST_ASSERT(((AT_CellularNetwork*)nw)->get_device_error().errCode == 100 && // 100 == unknown command for modem
                ((AT_CellularNetwork*)nw)->get_device_error().errType == 3); // 3 == CME error from the modem
#endif
    } else {
        // should have some values, only not optional are apn and bearer id
        CellularNetwork::pdpcontext_params_t* params = params_list.get_head();
        TEST_ASSERT(strlen(params->apn) > 0);
        TEST_ASSERT(params->bearer_id >= 0)
    }

    int rxlev = -1, ber = -1, rscp = -1, ecno = -1, rsrq = -1, rsrp = -1;
    err = nw->get_extended_signal_quality(rxlev, ber, rscp, ecno, rsrq, rsrp);
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_DEVICE_ERROR);
    if (err == NSAPI_ERROR_DEVICE_ERROR) {
#if CELLULAR_DEVICE != QUECTEL_BG96 // QUECTEL_BG96 does not give any specific reason for device error
        TEST_ASSERT(((AT_CellularNetwork*)nw)->get_device_error().errCode == 100 && // 100 == unknown command for modem
                ((AT_CellularNetwork*)nw)->get_device_error().errType == 3); // 3 == CME error from the modem
#endif
    } else {
        // we should have some values which are not optional
        TEST_ASSERT(rxlev >= 0 && ber >= 0 && rscp >= 0 && ecno >= 0 && rsrq >= 0 && rsrp >= 0);
    }

    int rssi = -1;
    ber = -1;
    err = nw->get_signal_quality(rssi, ber);
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_DEVICE_ERROR);
    if (err == NSAPI_ERROR_DEVICE_ERROR) {
        TEST_ASSERT(((AT_CellularNetwork*)nw)->get_device_error().errCode == 100 && // 100 == unknown command for modem
                ((AT_CellularNetwork*)nw)->get_device_error().errType == 3); // 3 == CME error from the modem
    } else {
        // test for values
        TEST_ASSERT(rssi >= 0);
        TEST_ASSERT(ber >= 0);
    }

    int cell_id = -5;
    TEST_ASSERT(nw->get_cell_id(cell_id) == NSAPI_ERROR_OK);
    TEST_ASSERT(cell_id != -5);

    int format = -1;
    CellularNetwork::operator_t operator_params;
    // all params are optional so can't test operator_params
    TEST_ASSERT(nw->get_operator_params(format, operator_params) == NSAPI_ERROR_OK);

    nsapi_connection_status_t st =  nw->get_connection_status();
    TEST_ASSERT(st == NSAPI_STATUS_DISCONNECTED);

    TEST_ASSERT(nw->set_blocking(true) == NSAPI_ERROR_OK);

#if CELLULAR_DEVICE != QUECTEL_BG96
    // QUECTEL_BG96 timeouts with this one, tested with 3 minute timeout
    CellularNetwork::operator_names_list op_names;
    err = nw->get_operator_names(op_names);
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_DEVICE_ERROR);
    if (err == NSAPI_ERROR_DEVICE_ERROR) {
        // if device error then we must check was that really device error or that modem/network does not support the commands
        TEST_ASSERT(((AT_CellularNetwork*)nw)->get_device_error().errCode == 4// 4 == NOT SUPPORTED BY THE MODEM
                && ((AT_CellularNetwork*)nw)->get_device_error().errType == 3); // 3 == CME error from the modem
    } else {
        CellularNetwork::operator_names_t* opn = op_names.get_head();
        TEST_ASSERT(strlen(opn->numeric) > 0);
        TEST_ASSERT(strlen(opn->alpha > 0));
    }
#endif

#if CELLULAR_DEVICE != TELIT_HE910
    // TELIT_HE910 just gives an error and no specific error number so we can't know is this real error or that modem/network does not support the command
    CellularNetwork::Supported_UE_Opt supported_opt = SUPPORTED_UE_OPT_MAX;
    CellularNetwork::Preferred_UE_Opt preferred_opt = PREFERRED_UE_OPT_MAX;
    err = nw->get_ciot_optimization_config(supported_opt, preferred_opt);
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_DEVICE_ERROR);
    if (err == NSAPI_ERROR_DEVICE_ERROR) {
        // if device error then we must check was that really device error or that modem/network does not support the commands
        TEST_ASSERT(((AT_CellularNetwork*)nw)->get_device_error().errCode == 100 && // 100 == unknown command for modem
                ((AT_CellularNetwork*)nw)->get_device_error().errType == 3); // 3 == CME error from the modem
    } else {
        TEST_ASSERT(supported_opt != SUPPORTED_UE_OPT_MAX);
        TEST_ASSERT(preferred_opt != PREFERRED_UE_OPT_MAX);
    }

    err = nw->set_ciot_optimization_config(supported_opt, preferred_opt);
    TEST_ASSERT(err == NSAPI_ERROR_OK || err == NSAPI_ERROR_DEVICE_ERROR);
    if (err == NSAPI_ERROR_DEVICE_ERROR) {
        // if device error then we must check was that really device error or that modem/network does not support the commands
        TEST_ASSERT(((AT_CellularNetwork*)nw)->get_device_error().errCode == 100 && // 100 == unknown command for modem
                ((AT_CellularNetwork*)nw)->get_device_error().errType == 3); // 3 == CME error from the modem
    }
#endif

}

static void test_disconnect()
{
    nsapi_connection_status_t st =  nw->get_connection_status();
    TEST_ASSERT(st == NSAPI_STATUS_GLOBAL_UP);
    TEST_ASSERT(nw->disconnect() == NSAPI_ERROR_OK);
}

static void test_detach()
{
    // in PPP mode there is NO CARRIER waiting so flush it out
    rtos::Thread::wait(6*1000);
    ((AT_CellularNetwork*)nw)->get_at_handler().flush();

    nsapi_connection_status_t st =  nw->get_connection_status();
    TEST_ASSERT(st == NSAPI_STATUS_DISCONNECTED);

    TEST_ASSERT(nw->detach() == NSAPI_ERROR_OK);
    st =  nw->get_connection_status();
    TEST_ASSERT(st == NSAPI_STATUS_DISCONNECTED);
}

using namespace utest::v1;

static utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason)
{
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_ABORT;
}

static Case cases[] = {
    Case("CellularNetwork state machine methods", test_network_interface_fsm, greentea_failure_handler),
    Case("CellularNetwork init", init_network_interface, greentea_failure_handler),
    Case("CellularNetwork test registering", test_network_registration, greentea_failure_handler),
    Case("CellularNetwork test attach", test_attach, greentea_failure_handler),
    Case("CellularNetwork test activate pdp context", test_activate_context, greentea_failure_handler),
    Case("CellularNetwork test other functions", test_other, greentea_failure_handler),
    Case("CellularNetwork test connect", test_connect, greentea_failure_handler),
    Case("CellularNetwork test credentials", test_credentials, greentea_failure_handler),
    Case("CellularNetwork test disconnect", test_disconnect, greentea_failure_handler),
    Case("CellularNetwork test detach", test_detach, greentea_failure_handler)

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
