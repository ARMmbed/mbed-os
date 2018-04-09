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
#ifdef CELLULAR_DEVICE

using namespace mbed;

static bool wait_register()
{
    tr_info("Try registering to network...");
    if (network->set_registration() != NSAPI_ERROR_OK) {
        tr_error("Network registration request failed.");
        return false;
    }

    CellularNetwork::RegistrationStatus status;
    for (int i=0; i<180; i++) {
        tr_info("Register to network %d...", i);
        for (int type = 0; type < CellularNetwork::C_MAX; type++) {
            if (network->get_registration_status((CellularNetwork::RegistrationType)type, status) ==  NSAPI_ERROR_OK) {
                tr_info("status %d...", status);
                switch (status) {
                    case CellularNetwork::RegisteredRoaming:
                    // fall-through
                    case CellularNetwork::RegisteredHomeNetwork:
                        tr_info("Registered to network.");
                        return true;
                    case CellularNetwork::RegisteredSMSOnlyRoaming:
                    // fall-through
                    case CellularNetwork::RegisteredSMSOnlyHome:
                        tr_warn("SMS only network registration!");
                        return true;
                    case CellularNetwork::RegisteredCSFBNotPreferredRoaming:
                    // fall-through
                    case CellularNetwork::RegisteredCSFBNotPreferredHome:
                        tr_warn("Not preferred network registration!");
                        return true;
                    case CellularNetwork::AttachedEmergencyOnly:
                        tr_warn("Emergency only network registration!");
                        return true;
                    case CellularNetwork::RegistrationDenied:
                        tr_warn("Network registration denied!");
                        wait(i);
                        break;
                    case CellularNetwork::NotRegistered:
                    case CellularNetwork::Unknown:
                    case CellularNetwork::SearchingNetwork:
                    default:
                        break;
                }
            }
        }
        wait(1);
    }
    return false;
}

void test_attach()
{
    cellularDevice.set_timeout(120*1000); // 120 second timeout for at commands after power is up. It might take time to register, attach and connect
    tr_info("Register to network.");
    TEST_ASSERT(wait_register());
    tr_info("Attach to network.");
    nsapi_error_t err = network->set_attach();
    TEST_ASSERT(!err);
    CellularNetwork::AttachStatus status;
    err = network->get_attach(status);
    TEST_ASSERT(!err);
}

void test_connect()
{
    nsapi_error_t err = network->connect();
    TEST_ASSERT(!err);
}

void test_get_ip_address()
{
    const char *ip = network->get_ip_address();
    TEST_ASSERT(ip && ip[0]);
    tr_info("IP: %s\r\n", ip);
}

void test_disconnect()
{
    nsapi_error_t err = network->disconnect();
    TEST_ASSERT(!err);
}

#endif // CELLULAR_DEVICE
