/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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
#include "nsconfig.h"
#include "test_mac_timer.h"
#include <string.h>
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/rf_driver_storage.h"
#include "sw_mac_stub.h"

bool test_timer_mac_start()
{
    protocol_interface_rf_mac_setup_s mac;
    mac.mac_timer_id = 1;
    mac_event_t ev;
    timer_mac_start(&mac, ev, 0);
    return true;
}

bool test_timer_mac_stop()
{
    protocol_interface_rf_mac_setup_s mac;
    mac.mac_timer_id = 1;
    timer_mac_stop(&mac);
    return true;
}

bool test_timer_mac_interrupt()
{
    timer_mac_interrupt(0, 0);
    protocol_interface_rf_mac_setup_s rf_mac_setup;
    sw_mac_stub.rf_mac_ptr = &rf_mac_setup;
    timer_mac_interrupt(0, 0);

    return true;
}
