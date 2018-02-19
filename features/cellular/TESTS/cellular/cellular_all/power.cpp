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

static bool start_cellular_at(CellularPower *pwr)
{
    nsapi_error_t err = pwr->off();
    if (err != NSAPI_ERROR_OK && err != NSAPI_ERROR_UNSUPPORTED) {
        return false;
    }
    err = pwr->on();
    if (err != NSAPI_ERROR_OK && err != NSAPI_ERROR_UNSUPPORTED) {
        return false;
    }
    tr_info("Wait for cellular device 180 seconds...");
    for (int i = 0; i < 180; i++) {
        if (pwr->set_at_mode() == NSAPI_ERROR_OK) {
            return true;
        }
        wait(1);
    }
    return false;
}

void test_create_power()
{
    TEST_ASSERT(start_cellular_at(pwr));

    tr_info("Cellular device is ready!");
}

// TODO: tests still missing for off, sleep, opt_power_save_mode, opt_receive_period

#endif // CELLULAR_DEVICE
