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
#include "nsconfig.h"
#include "test_nwk_nvm.h"
#include <string.h>
#include "ns_types.h"
#include "eventOS_event.h"
#include "ns_trace.h"
#include "string.h"
#include "randLIB.h"
#include "nsdynmemLIB.h"
#include "net_nvm_api.h"
#include "Core/include/socket.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/NVM/nwk_nvm.h"
#include "Service_Libs/mle_service/mle_service_api.h"

#include "address_stub.h"
#include "protocol_core_stub.h"
#include "mac_helper_stub.h"
#include "mesh_stub.h"
#include "nsdynmemLIB_stub.h"
#include "buffer_dyn_stub.h"
#include "common_functions_stub.h"

#define C_CHECK(condition) \
  do { if (!(condition)) { printf("ERROR: %s at file %s on line %d\n", #condition, __FILE__, __LINE__); return false; } } while(0)

//Stubbed test data
wpan_nvm_params_t test_params;


static void test_wpan_params_updated(wpan_nvm_params_t *parameters)
{
    test_params = *parameters;
}

static bool test_wpan_params_get(wpan_nvm_params_t *parameters)
{
    if (parameters->pan_id == test_params.pan_id) {
        *parameters = test_params;
        return true;
    }
    return false;
}

static void test_params_set(uint16_t pan_id, uint32_t mac_counter, uint32_t mle_counter)
{
    test_params.pan_id = pan_id;
    test_params.mac_security_frame_counter = mac_counter;
    test_params.mle_securit_counter = mle_counter;
}

bool test_net_nvm_wpan_params_storage_enable(void)
{
    protocol_interface_info_entry_t interface;
    memset(&interface, 0, sizeof(protocol_interface_info_entry_t));
    protocol_core_stub.entry_ptr = &interface;
    nsdynmemlib_stub.returnCounter = 0;
    int8_t ret = net_nvm_wpan_params_storage_enable(0, NULL, NULL);
    C_CHECK( ret == -1 );

    protocol_core_stub.entry_ptr = NULL;
    ret = net_nvm_wpan_params_storage_enable(0, test_wpan_params_updated, test_wpan_params_get);
    C_CHECK( ret == -1 );
    protocol_core_stub.entry_ptr = &interface;
    ret = net_nvm_wpan_params_storage_enable(0, test_wpan_params_updated, test_wpan_params_get);
    C_CHECK( ret == -2 );
    nsdynmemlib_stub.returnCounter = 1;
    ret = net_nvm_wpan_params_storage_enable(0, test_wpan_params_updated, test_wpan_params_get);
    C_CHECK( ret == 0 );

    ret = net_nvm_wpan_params_storage_enable(0, test_wpan_params_updated, test_wpan_params_get);
    C_CHECK( ret == 0 );

    net_nvm_wpan_params_storage_disable(0);
    return true;
}

bool test_net_nvm_wpan_params_storage_disable(void)
{
    protocol_interface_info_entry_t interface;
    memset(&interface, 0, sizeof(protocol_interface_info_entry_t));
    protocol_core_stub.entry_ptr = NULL;
    nsdynmemlib_stub.returnCounter = 1;
    int8_t ret = net_nvm_wpan_params_storage_disable(0);

    protocol_core_stub.entry_ptr = &interface;
    ret = net_nvm_wpan_params_storage_disable(0);
    C_CHECK( ret == -1 );
    net_nvm_wpan_params_storage_enable(0, test_wpan_params_updated, test_wpan_params_get);
    ret = net_nvm_wpan_params_storage_disable(0);
    C_CHECK( ret == 0 );
    return true;
}

bool test_net_nvm_wpan_params_storage_reset(void)
{
    protocol_interface_info_entry_t interface;
    memset(&interface, 0, sizeof(protocol_interface_info_entry_t));
    protocol_core_stub.entry_ptr = NULL;
    nsdynmemlib_stub.returnCounter = 1;
    int8_t ret = net_nvm_wpan_params_storage_reset(0);
    C_CHECK( ret == -1 );
    protocol_core_stub.entry_ptr = &interface;
    ret = net_nvm_wpan_params_storage_reset(0);
    C_CHECK( ret == -1 );

    net_nvm_wpan_params_storage_enable(0, test_wpan_params_updated, test_wpan_params_get);
    ret = net_nvm_wpan_params_storage_reset(0);
    C_CHECK( ret == 0 );

    net_nvm_wpan_params_storage_disable(0);

    return true;
}

bool test_net_nvm_usage(void)
{
    protocol_interface_info_entry_t interface;
    memset(&interface, 0, sizeof(protocol_interface_info_entry_t));
    nsdynmemlib_stub.returnCounter = 1;
    protocol_core_stub.entry_ptr = &interface;
    net_nvm_wpan_params_storage_enable(0, test_wpan_params_updated, test_wpan_params_get);
    C_CHECK( interface.nwk_wpan_nvm_api);
    //Now lets start using and testing from stack and APP function callbacks
    //Test GET all possible
    test_params_set(0xface, 100, 101);

    interface.nwk_wpan_nvm_api->nvm_params_update_cb(interface.nwk_wpan_nvm_api, false);

    //Test that we load from NVM and start using updated frame counters
    wpan_nvm_params_t *result = interface.nwk_wpan_nvm_api->nvm_params_get_cb(interface.nwk_wpan_nvm_api,0xface);
    C_CHECK( interface.nwk_wpan_nvm_api->params.mac_security_frame_counter == (100+1000));
    C_CHECK( interface.nwk_wpan_nvm_api->params.mle_securit_counter == (101+1000));
    C_CHECK( test_params.mac_security_frame_counter == (100+1000));
    C_CHECK( test_params.mle_securit_counter == (101+1000));

    result = interface.nwk_wpan_nvm_api->nvm_params_get_cb(interface.nwk_wpan_nvm_api, 0xbabe);
    C_CHECK( interface.nwk_wpan_nvm_api->params.mac_security_frame_counter == (0));
    C_CHECK( interface.nwk_wpan_nvm_api->params.mle_securit_counter == (0));
    C_CHECK( test_params.mac_security_frame_counter == (0));
    C_CHECK( test_params.mle_securit_counter == (0));


    mac_helper_link_frame_counter_set(0,900);

    interface.nwk_wpan_nvm_api->nvm_params_update_cb(interface.nwk_wpan_nvm_api, false);
    result = interface.nwk_wpan_nvm_api->nvm_params_get_cb(interface.nwk_wpan_nvm_api, 0xbabe);
    C_CHECK( interface.nwk_wpan_nvm_api->params.mac_security_frame_counter == (0));
    C_CHECK( interface.nwk_wpan_nvm_api->params.mle_securit_counter == (0));
    C_CHECK( test_params.mac_security_frame_counter == (0));
    C_CHECK( test_params.mle_securit_counter == (0));
    mac_helper_link_frame_counter_set(0, 951);
    interface.nwk_wpan_nvm_api->nvm_params_update_cb(interface.nwk_wpan_nvm_api, false);
    result = interface.nwk_wpan_nvm_api->nvm_params_get_cb(interface.nwk_wpan_nvm_api, 0xbabe);
    C_CHECK( interface.nwk_wpan_nvm_api->params.mac_security_frame_counter == (951));
    C_CHECK( interface.nwk_wpan_nvm_api->params.mle_securit_counter == (0));
    C_CHECK( test_params.mac_security_frame_counter == (951));
    C_CHECK( test_params.mle_securit_counter == (0));
    mac_helper_link_frame_counter_set(0, 1030);
    interface.nwk_wpan_nvm_api->nvm_params_update_cb(interface.nwk_wpan_nvm_api, true);
    result = interface.nwk_wpan_nvm_api->nvm_params_get_cb(interface.nwk_wpan_nvm_api, 0xbabe);
    C_CHECK( interface.nwk_wpan_nvm_api->params.mac_security_frame_counter == (1030));
    C_CHECK( interface.nwk_wpan_nvm_api->params.mle_securit_counter == (0));
    C_CHECK( test_params.mac_security_frame_counter == (951));
    C_CHECK( test_params.mle_securit_counter == (0));
    net_nvm_wpan_params_storage_reset(0);
    result = interface.nwk_wpan_nvm_api->nvm_params_get_cb(interface.nwk_wpan_nvm_api, 0xbabe);
    C_CHECK( interface.nwk_wpan_nvm_api->params.mac_security_frame_counter == (1951));
    C_CHECK( interface.nwk_wpan_nvm_api->params.mle_securit_counter == (1000));
    C_CHECK( test_params.mac_security_frame_counter == (1951));
    C_CHECK( test_params.mle_securit_counter == (1000));

    net_nvm_wpan_params_storage_disable(0);
    return true;
}
