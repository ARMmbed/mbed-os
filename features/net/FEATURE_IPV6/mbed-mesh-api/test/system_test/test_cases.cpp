/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "mbed-mesh-api/Mesh6LoWPAN_ND.h"
#include "mbed-mesh-api/MeshThread.h"
#include "mbed-mesh-api/MeshInterfaceFactory.h"
#include "mbed-mesh-api/mesh_interface_types.h"
#include "sal/test/ctest_env.h"
#include "atmel-rf-driver/driverRFPhy.h"    // rf_device_register
#include "test_cases.h"
#include "mbed-drivers/test_env.h"
#define HAVE_DEBUG 1
#include "ns_trace.h"

#define TRACE_GROUP  "main"     // for traces

#define TEST_RESULT_PRINT()\
    do {\
        TEST_PRINT("{{summary %s [%s] (%d/%d FAILED)}}\r\n", __func__, (TEST_RESULT()?"PASS":"FAIL"), tests_failed_global, tests_total_global);\
    }while (0)

static uint8_t mesh_network_state = MESH_DISCONNECTED;
AbstractMesh *mesh_api = NULL;

extern void test_result_notify(int result, AbstractMesh *meshAPI);

// Thread device type, currently defined statically, change also
// YOTTA_CFG_MBED_MESH_API_THREAD_DEVICE_TYPE from static_config.h to match selection
#define TEST_THREAD_DEVICE_TYPE_SED false

/*
 * Callback from mesh network. Called when network state changes.
 */
void test_callback_connect_disconnect(mesh_connection_status_t mesh_state)
{
    tr_info("test_callback_connect_disconnect() %d", mesh_state);
    mesh_network_state = mesh_state;
    if (mesh_network_state == MESH_CONNECTED) {
        tr_info("Connected to mesh network!");
        mesh_error_t err = mesh_api->disconnect();
        TEST_EQ(err, MESH_ERROR_NONE);
    } else if (mesh_network_state == MESH_DISCONNECTED) {
        tr_info("Disconnected from mesh network!");
        TEST_RESULT_PRINT();
        test_result_notify(test_pass_global, mesh_api);
    } else {
        // untested branch, catch erros by bad state checking...
        TEST_EQ(mesh_network_state, MESH_CONNECTED);
        tr_error("Networking error!");
        TEST_RESULT_PRINT();
        test_result_notify(test_pass_global, mesh_api);
    }
}

void test_mesh_api_connect_disconnect(int8_t rf_device_id, mesh_network_type_t type)
{
    mesh_error_t err;
    TEST_PRINT("\r\nBegin %s, type=%d\r\n", __func__, type);

    mesh_api = MeshInterfaceFactory::createInterface(type);
    // initialize interface
    if (type == MESH_TYPE_THREAD) {
        uint8_t eui64[8];
        char *pskd;
        rf_read_mac_address(eui64);
        pskd = (char *)"Secret password";
        err = ((MeshThread *)mesh_api)->init(rf_device_id, test_callback_connect_disconnect, eui64, pskd);
    } else {
        err = mesh_api->init(rf_device_id, test_callback_connect_disconnect);
    }

    if (!TEST_EQ(err, MESH_ERROR_NONE)) {
        TEST_RESULT_PRINT();
        test_result_notify(test_pass_global, mesh_api);
        return;
    }

    err = mesh_api->connect();
    if (!TEST_EQ(err, MESH_ERROR_NONE)) {
        TEST_RESULT_PRINT();
        test_result_notify(test_pass_global, mesh_api);
        return;
    }
    // control goes to test callback
}

void test_callback_init(mesh_connection_status_t mesh_state)
{
    tr_info("test_callback_init() %d", mesh_state);
}

void test_mesh_api_init(int8_t rf_device_id)
{
    mesh_error_t err;
    TEST_PRINT("\r\nBegin %s\r\n", __func__);
    mesh_api = (Mesh6LoWPAN_ND *)MeshInterfaceFactory::createInterface(MESH_TYPE_6LOWPAN_ND);

    do {
        // no callback set
        err = mesh_api->init(rf_device_id, NULL);
        if (!TEST_EQ(err, MESH_ERROR_PARAM)) {
            break;
        }

        // bad rf-device_id
        err = mesh_api->init(rf_device_id + 1, test_callback_init);
        if (!TEST_EQ(err, MESH_ERROR_MEMORY)) {
            break;
        }

        // successful re-initialization
        err = mesh_api->init(rf_device_id, test_callback_init);
        if (!TEST_EQ(err, MESH_ERROR_NONE)) {
            break;
        }

        /* Thread can't be instantiated if ND is initialized */
        MeshThread *meshThread = (MeshThread *)MeshInterfaceFactory::createInterface(MESH_TYPE_THREAD);
        if (!TEST_EQ(meshThread, NULL)) {
            break;
        }
        break;
    } while (1);

    TEST_RESULT_PRINT();
    test_result_notify(test_pass_global, mesh_api);
}

void test_mesh_api_init_thread(int8_t rf_device_id)
{
    mesh_error_t err;
    uint8_t eui64[8];
    char *pskd;

    TEST_PRINT("\r\nBegin %s\r\n", __func__);
    rf_read_mac_address(eui64);
    pskd = (char *)"Secret password";

    mesh_api = MeshInterfaceFactory::createInterface(MESH_TYPE_THREAD);

    do {
        // no callback set
        err = ((MeshThread *)mesh_api)->init(rf_device_id, NULL);
        if (!TEST_EQ(err, MESH_ERROR_PARAM)) {
            break;
        }

        // No address in eui64
        err = ((MeshThread *)mesh_api)->init(rf_device_id, test_callback_init, NULL, pskd);
        if (!TEST_EQ(err, MESH_ERROR_PARAM)) {
            break;
        }

        // bad rf-device_id
        err = ((MeshThread *)mesh_api)->init(rf_device_id + 1, test_callback_init, eui64, pskd);
        if (!TEST_EQ(err, MESH_ERROR_MEMORY)) {
            break;
        }

        // successful re-initialization
        err = ((MeshThread *)mesh_api)->init(rf_device_id, test_callback_init, eui64, pskd);
        if (!TEST_EQ(err, MESH_ERROR_NONE)) {
            break;
        }

        /* Thread can't be instantiated if it has been initialized already */
        MeshThread *apiThread = (MeshThread *)MeshInterfaceFactory::createInterface(MESH_TYPE_THREAD);
        if (!TEST_EQ(apiThread, NULL)) {
            break;
        }

        /* 6LoWPAN ND can't be instantiated if Thread has been initialized */
        Mesh6LoWPAN_ND *apiND = (Mesh6LoWPAN_ND *)MeshInterfaceFactory::createInterface(MESH_TYPE_6LOWPAN_ND);
        if (!TEST_EQ(apiND, NULL)) {
            break;
        }

        break;
    } while (1);

    TEST_RESULT_PRINT();
    test_result_notify(test_pass_global, mesh_api);
}

/*
 * Callback from mesh network for mesh_api_connect test
 */
void test_callback_connect(mesh_connection_status_t mesh_state)
{
    mesh_error_t err;
    tr_info("test_callback_connect() %d", mesh_state);
    mesh_network_state = mesh_state;

    if (mesh_network_state == MESH_CONNECTED) {
        tr_info("Connected to mesh network!");
        // try to connect again, should fail
        err = mesh_api->connect();
        if (!TEST_EQ(err, MESH_ERROR_STATE)) {
            TEST_RESULT_PRINT();
            test_result_notify(test_pass_global, mesh_api);
        } else {
            // disconnect
            err = mesh_api->disconnect();
            if (!TEST_EQ(err, MESH_ERROR_NONE)) {
                TEST_RESULT_PRINT();
                test_result_notify(test_pass_global, mesh_api);
            }
        }
    } else if (mesh_network_state == MESH_DISCONNECTED) {
        tr_info("Disconnected from mesh network!");
        TEST_RESULT_PRINT();
        test_result_notify(test_pass_global, mesh_api);
    } else {
        // untested branch, catch errors by bad state checking...
        TEST_EQ(true, false);
        tr_error("Networking error!");
        TEST_RESULT_PRINT();
        test_result_notify(test_pass_global, mesh_api);
    }
}

void test_mesh_api_connect(int8_t rf_device_id, mesh_network_type_t type)
{
    mesh_error_t err;

    TEST_PRINT("\r\nBegin %s, type=%d\r\n", __func__, type);
    mesh_api = MeshInterfaceFactory::createInterface(type);

    do {
        // connect uninitialized
        err = mesh_api->connect();
        if (!TEST_EQ(err, MESH_ERROR_PARAM)) {
            TEST_RESULT_PRINT();
            test_result_notify(test_pass_global, mesh_api);
            break;
        }

        // initialize interface
        if (type == MESH_TYPE_THREAD) {
            uint8_t eui64[8];
            char *pskd;
            rf_read_mac_address(eui64);
            pskd = (char *)"Secret password";
            err = ((MeshThread *)mesh_api)->init(rf_device_id, test_callback_connect, eui64, pskd);
        } else {
            err = mesh_api->init(rf_device_id, test_callback_connect);
        }
        if (!TEST_EQ(err, MESH_ERROR_NONE)) {
            TEST_RESULT_PRINT();
            test_result_notify(test_pass_global, mesh_api);
            break;
        }

        // successful connect
        err = mesh_api->connect();
        if (!TEST_EQ(err, MESH_ERROR_NONE)) {
            TEST_RESULT_PRINT();
            test_result_notify(test_pass_global, mesh_api);
            break;
        }
        //test continues in callback
        break;
    } while (1);
}

/*
 * Callback from mesh network for mesh_api_connect test
 */
void test_callback_disconnect(mesh_connection_status_t mesh_state)
{
    tr_info("test_callback_disconnect() %d", mesh_state);
    TEST_EQ(true, false);
    tr_error("Networking error, callback received!");
    TEST_RESULT_PRINT();
    test_result_notify(test_pass_global, mesh_api);
}

void test_mesh_api_disconnect(int8_t rf_device_id, mesh_network_type_t type)
{
    mesh_error_t err;

    TEST_PRINT("\r\nBegin %s\r\n", __func__);
    mesh_api = MeshInterfaceFactory::createInterface(type);

    do {
        // disconnect not initialized, callback not called
        err = mesh_api->disconnect();
        if (!TEST_EQ(err, MESH_ERROR_UNKNOWN)) {
            break;
        }

        // initialize interface
        if (type == MESH_TYPE_THREAD) {
            uint8_t eui64[8];
            char *pskd;
            rf_read_mac_address(eui64);
            pskd = (char *)"Secret password";
            err = ((MeshThread *)mesh_api)->init(rf_device_id, test_callback_disconnect, eui64, pskd);
        } else {
            err = mesh_api->init(rf_device_id, test_callback_disconnect);
        }
        if (!TEST_EQ(err, MESH_ERROR_NONE)) {
            break;
        }

        // disconnect not connected
        err = mesh_api->disconnect();
        if (!TEST_EQ(err, MESH_ERROR_UNKNOWN)) {
            break;
        }
        break;
    } while (1);

    TEST_RESULT_PRINT();
    test_result_notify(test_pass_global, mesh_api);
}

/*
 * Callback from mesh network for test_mesh_api_delete_connected test
 */
void test_callback_delete_connected(mesh_connection_status_t mesh_state)
{
    tr_info("test_callback_delete() %d", mesh_state);
    if (mesh_state == MESH_CONNECTED) {
        delete mesh_api;
        mesh_api = NULL;
        TEST_RESULT_PRINT();
        test_result_notify(test_pass_global, mesh_api);
    } else {
        tr_error("Networking error!");
        TEST_EQ(true, false);
        TEST_RESULT_PRINT();
        test_result_notify(test_pass_global, mesh_api);
    }
}

void test_mesh_api_delete_connected(int8_t rf_device_id, mesh_network_type_t type)
{
    mesh_error_t err;

    TEST_PRINT("\r\nBegin %s, type=%d\r\n", __func__, type);
    mesh_api = MeshInterfaceFactory::createInterface(type);

    do {
        // initialize interface
        if (type == MESH_TYPE_THREAD) {
            uint8_t eui64[8];
            char *pskd;
            rf_read_mac_address(eui64);
            pskd = (char *)"Secret password";
            err = ((MeshThread *)mesh_api)->init(rf_device_id, test_callback_delete_connected, eui64, pskd);
        } else {
            err = mesh_api->init(rf_device_id, test_callback_delete_connected);
        }
        if (!TEST_EQ(err, MESH_ERROR_NONE)) {
            TEST_RESULT_PRINT();
            test_result_notify(test_pass_global, mesh_api);
            break;
        }

        // successful connect
        err = mesh_api->connect();
        if (!TEST_EQ(err, MESH_ERROR_NONE)) {
            TEST_RESULT_PRINT();
            test_result_notify(test_pass_global, mesh_api);
            break;
        }
        //test continues in callback
        break;
    } while (1);
}


/*
 * Callback from mesh network for test_mesh_api_data_poll_rate_set_thread test
 */
void test_callback_data_poll_rate_set_thread(mesh_connection_status_t mesh_state)
{
    tr_info("test_callback_data_poll_rate_set() %d", mesh_state);
    mesh_network_state = mesh_state;

    if (mesh_network_state == MESH_CONNECTED) {
        tr_info("Connected to mesh network!");

#if (TEST_THREAD_DEVICE_TYPE_SED == true)
        mesh_error_t err;
        // set data polling rate to 0 (enables fast polling mode)
        err = ((MeshThread*)mesh_api)->data_poll_rate_set(0);
        if (!TEST_EQ(err, MESH_ERROR_NONE)) {
            TEST_RESULT_PRINT();
            test_result_notify(test_pass_global, mesh_api);
            return;
        }

        // try to set data polling rate to 100000
        err = ((MeshThread*)mesh_api)->data_poll_rate_set(864001+1);
        if (!TEST_EQ(err, MESH_ERROR_PARAM)) {
            TEST_RESULT_PRINT();
            test_result_notify(test_pass_global, mesh_api);
            return;
        }

        // OK case, set data polling rate to 5
        err = ((MeshThread*)mesh_api)->data_poll_rate_set(5);
        if (!TEST_EQ(err, MESH_ERROR_NONE)) {
            TEST_RESULT_PRINT();
            test_result_notify(test_pass_global, mesh_api);
            return;
        }
#endif
        TEST_RESULT_PRINT();
        test_result_notify(test_pass_global, mesh_api);
    } else if (mesh_network_state == MESH_DISCONNECTED) {
        tr_info("Disconnected from mesh network!");
        TEST_RESULT_PRINT();
        test_result_notify(test_pass_global, mesh_api);
    } else {
        // untested branch, catch errors by bad state checking...
        TEST_EQ(true, false);
        tr_error("Networking error!");
        TEST_RESULT_PRINT();
        test_result_notify(test_pass_global, mesh_api);
    }
}

void test_mesh_api_data_poll_rate_set_thread(int8_t rf_device_id)
{
    mesh_error_t err;

    TEST_PRINT("\r\nBegin %s\r\n", __func__);

    mesh_api = MeshInterfaceFactory::createInterface(MESH_TYPE_THREAD);

    do {
        // try to set data polling rate when not initialized
        err = ((MeshThread*)mesh_api)->data_poll_rate_set(1);
        if (!TEST_EQ(err, MESH_ERROR_UNKNOWN)) {
            TEST_RESULT_PRINT();
            test_result_notify(test_pass_global, mesh_api);
            break;
        }

        uint8_t eui64[8];
        rf_read_mac_address(eui64);
        err = ((MeshThread *)mesh_api)->init(rf_device_id, test_callback_data_poll_rate_set_thread, eui64, NULL);

        if (!TEST_EQ(err, MESH_ERROR_NONE)) {
            TEST_RESULT_PRINT();
            test_result_notify(test_pass_global, mesh_api);
            break;
        }

        // try to set data polling rate when not connected
        err = ((MeshThread*)mesh_api)->data_poll_rate_set(1);
        if (!TEST_EQ(err, MESH_ERROR_UNKNOWN)) {
            TEST_RESULT_PRINT();
            test_result_notify(test_pass_global, mesh_api);
            break;
        }

        // successful connect
        err = mesh_api->connect();
        if (!TEST_EQ(err, MESH_ERROR_NONE)) {
            TEST_RESULT_PRINT();
            test_result_notify(test_pass_global, mesh_api);
            break;
        }
        //test continues in callback
        break;
    } while (1);
}

