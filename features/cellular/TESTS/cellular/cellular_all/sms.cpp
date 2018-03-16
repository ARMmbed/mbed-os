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

void test_sms_init()
{
    // for some weird reason if we don't wait for few seconds we get SIM Busy error in initialize even is SIM is ready...
    wait(3);

    // check SIM state as we have tested sim and it might not be ready
    for (int i = 0; i < MAX_SIM_READY_WAITING_TIME; i++) {
        CellularSIM::SimState state;
        if (sim->get_sim_state(state) == NSAPI_ERROR_OK && state == CellularSIM::SimStateReady) {
            break;
        }
        wait(1);
    }

    nsapi_error_t err = sms->initialize(CellularSMS::CellularSMSMmodeText);
    char err_msg[60];
    sprintf(err_msg, "SMS initialize failed with: %d", err);

    cellularDevice.close_sms();
    sms = NULL;
    TEST_ASSERT_MESSAGE(!err, err_msg);
}

#endif // CELLULAR_DEVICE
