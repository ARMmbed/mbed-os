/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "mac_api.h"
#include "fhss_api.h"
#include "fhss_config.h"
#include "net_fhss.h"
#include "Service_Libs/fhss/fhss.h"
#include "test_fhss_config.h"
#include "nsdynmemLIB_stub.h"
#include "fhss_mac_interface_stub.h"
#include "fhss_stub.h"

bool test_ns_fhss_create()
{
    fhss_api_t *api;
    fhss_timer_t fhss_timer;
    fhss_configuration_t fhss_configuration;

    // Test creating FHSS with no memory
    nsdynmemlib_stub.returnCounter = 0;
    api = ns_fhss_create(&fhss_configuration, &fhss_timer, NULL);
    if (api) {
        return false;
    }
    // fhss_enable returns fail
    fhss_stub.int8_value = -1;
    // Able to allocate memory
    nsdynmemlib_stub.returnCounter = 1;
    api = ns_fhss_create(&fhss_configuration, &fhss_timer, NULL);
    if (api) {
        return false;
    }
    // fhss_enable returns success
    fhss_stub.int8_value = 0;
    // Able to allocate memory
    nsdynmemlib_stub.returnCounter = 1;
    api = ns_fhss_create(&fhss_configuration, &fhss_timer, NULL);
    if (!api) {
        return false;
    }
    // Test calling all functions
    fhss_mac_interface_stub.int8_value = 0;
    api->receive_frame(api, 0, NULL, 0, NULL, FHSS_SYNCH_FRAME);
    if (fhss_mac_interface_stub.int8_value != 1) {
        return false;
    }
    api->check_tx_conditions(api, true, 0, FHSS_DATA_FRAME, 100, 0, 0);
    if (fhss_mac_interface_stub.int8_value != 2) {
        return false;
    }
    api->data_tx_done(api, true, true, 0);
    if (fhss_mac_interface_stub.int8_value != 3) {
        return false;
    }
    api->data_tx_fail(api, 0, FHSS_DATA_FRAME);
    if (fhss_mac_interface_stub.int8_value != 4) {
        return false;
    }
    api->is_broadcast_channel(api);
    if (fhss_mac_interface_stub.int8_value != 5) {
        return false;
    }
    api->read_timestamp(api);
    if (fhss_mac_interface_stub.int8_value != 6) {
        return false;
    }
    api->synch_state_set(api, FHSS_UNSYNCHRONIZED, 0);
    if (fhss_mac_interface_stub.int8_value != 7) {
        return false;
    }
    api->tx_handle(api, true, NULL, FHSS_DATA_FRAME, NULL, 0, 0, 0);
    if (fhss_mac_interface_stub.int8_value != 8) {
        return false;
    }

    free(api);
    return true;
}

bool test_ns_fhss_delete()
{
    fhss_api_t *fhss_api = malloc(sizeof(fhss_api));

    // By setting bool value false, fhss_struct can not be found
    fhss_stub.bool_value = false;
    if (ns_fhss_delete(NULL) != -1) {
        return false;
    }
    fhss_stub.int8_value = -1;
    // By setting bool value true, fhss_struct can be found
    fhss_stub.bool_value = true;
    // Test without API
    if (ns_fhss_delete(NULL) != -1) {
        return false;
    }
    fhss_stub.int8_value = 0;
    // Test with API
    if (ns_fhss_delete(fhss_api) != 0) {
        return false;
    }
    return true;
}

bool test_ns_fhss_configuration_set()
{
    // By setting bool value false, fhss_struct can not be found
    fhss_stub.bool_value = false;
    ns_fhss_configuration_set(NULL, NULL);
    // By setting bool value true, fhss_struct can be found
    fhss_stub.bool_value = true;
    ns_fhss_configuration_set(NULL, NULL);
    return true;
}
