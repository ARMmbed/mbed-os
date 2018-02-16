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

#ifdef MBED_CONF_APP_CELLULAR_SIM_PIN

using namespace mbed;

void test_get_sim_state()
{
    wait(1);
    CellularSIM::SimState state = CellularSIM::SimStateUnknown;
    log_info("Wait SIM for 180 seconds...");
    for (int i = 0; i < 180; i++) {
        CellularSIM::SimState tmp_state;
        if ((sim->get_sim_state(tmp_state) == NSAPI_ERROR_OK) && tmp_state != CellularSIM::SimStateUnknown) {
            state = tmp_state;
            break;
        }
    }
    TEST_ASSERT_MESSAGE(state == CellularSIM::SimStateReady || state == CellularSIM::SimStatePinNeeded ||
                        state == CellularSIM::SimStatePukNeeded, "Invalid SIM state");
}

// creates PIN which is different than one defined in MBED_CONF_APP_CELLULAR_SIM_PIN
static void create_random_pin(char* random_pin)
{

    char s[11];
    do {
        sprintf(s,"%d", rand());

    } while (strncmp(s, MBED_CONF_APP_CELLULAR_SIM_PIN, 4) == 0);

    strncpy(random_pin, s, 4);
    random_pin[4] = '\0';
}

void test_set_pin()
{
    // run test only if sim is not in ready state as then sim interface will return NSAPI_ERROR_OK
    nsapi_error_t err;
    CellularSIM::SimState state = CellularSIM::SimStateUnknown;
    if ((sim->get_sim_state(state) == NSAPI_ERROR_OK) && (state != CellularSIM::SimStateReady)) {
        char random_pin[5];
        create_random_pin(random_pin);
        err = sim->set_pin(random_pin);
        TEST_ASSERT_MESSAGE(err != 0, "Setting random pin should fail");
    }

    err = sim->set_pin(MBED_CONF_APP_CELLULAR_SIM_PIN);
    char err_msg[60];
    sprintf(err_msg, "Setting correct pin: %s failed with: %d", MBED_CONF_APP_CELLULAR_SIM_PIN, err);
    TEST_ASSERT_MESSAGE(err == 0, err_msg);
}

void test_change_pin()
{
    char random_pin[5];
    create_random_pin(random_pin);
    nsapi_error_t err = sim->change_pin(MBED_CONF_APP_CELLULAR_SIM_PIN, random_pin);

    char err_msg[60];
    sprintf(err_msg, "Change from original pin failed with: %d", err);
    TEST_ASSERT_MESSAGE(err == NSAPI_ERROR_OK, err_msg);

    err = sim->change_pin(random_pin, MBED_CONF_APP_CELLULAR_SIM_PIN);
    sprintf(err_msg, "Change back original pin failed with: %d", err);
    TEST_ASSERT_MESSAGE(err == NSAPI_ERROR_OK, err_msg);
}

#endif

#endif // CELLULAR_DEVICE
