/*
 * Copyright (c) 2014-2016, Arm Limited and affiliates.
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

/**
* \file \test_libTrace\Test.c
*
* \brief Unit tests for libTrace
*/
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "unity.h"
#include "thread_dhcpv6_client.h"
#include "dhcp_service_api.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"

#include "unity_improved.h"

#include "sim_socket_api.h"
#include "sim_address.h"

/*
 * Unity test code starts
 *
 */
void setUp(void)
{
    trace_init();
    set_trace_config(TRACE_ACTIVE_LEVEL_ALL);

    printf("\nUNITY testcase Begin\n");
}

void tearDown(void)
{
    unity_impr_teststep_clean();
    printf("UNITY testcase End\n");
    trace_free();
}


uint8_t test_router_id;
uint8_t test_router_mask_ptr[8];
void dhcp_router_id_cb(int32_t tr_id, uint8_t router_id,  const uint8_t router_mask_ptr[static 8])
{
    TEST_METHOD_CALLED("dhcp_router_id_cb", NULL);
    printf("  id = %x\n", router_id);
    test_router_id = router_id;
    if (router_mask_ptr != NULL) {
        memcpy(test_router_mask_ptr, router_mask_ptr, 8);
    } else {
        memset(test_router_mask_ptr, 0, 8);
    }
}
void lt_test_dhcp_router_id_cb(uint8_t router_id,  const uint8_t router_mask_ptr[static 8])
{
    TEST_METHOD_CHECK("dhcp_router_id_cb");
    TEST_ASSERT_EQUAL_INT_MESSAGE(router_id, test_router_id, "Wrong router id given");
    if (router_mask_ptr != NULL) {
        TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(test_router_mask_ptr, router_mask_ptr, 8, "Incorrect router mask");
    }
}
typedef struct {
    uint16_t instance_id;
    void *ptr;
    uint8_t msg_name;
    uint8_t *msg_ptr;
    uint16_t msg_len;
} test_service_recv_resp_t;

int test_dhcp_service_receive_resp_cb_a(uint16_t instance_id, void *ptr, uint8_t msg_name,  uint8_t *msg_ptr, uint16_t msg_len)
{
    test_service_recv_resp_t *msg_data_ptr = malloc(sizeof(test_service_recv_resp_t));
    msg_data_ptr->instance_id = instance_id;
    msg_data_ptr->ptr = ptr;
    msg_data_ptr->msg_name = msg_name;
    msg_data_ptr->msg_ptr = malloc(msg_len);
    memcpy(msg_data_ptr->msg_ptr, msg_ptr, msg_len);
    msg_data_ptr->msg_len = msg_len;
    TEST_METHOD_CALLED("dhcp_service_receive_resp_cb", msg_data_ptr);
    return RET_MSG_ACCEPTED;
}
int test_dhcp_service_receive_resp_cb(uint16_t instance_id, void *ptr, uint8_t msg_name,  uint8_t *msg_ptr, uint16_t msg_len)
{
    test_service_recv_resp_t *msg_data_ptr;
    msg_data_ptr = TEST_METHOD_CHECK("dhcp_service_receive_resp_cb");
    printf("Data :\n");
    test_buf(msg_ptr, msg_len, msg_data_ptr->msg_ptr, msg_data_ptr->msg_len);
    TEST_ASSERT_EQUAL_INT_MESSAGE(instance_id, msg_data_ptr->instance_id, "Wrong instance ID");
    TEST_ASSERT_EQUAL_INT_MESSAGE(ptr, msg_data_ptr->ptr, "Wrong pointer received");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(msg_ptr, msg_data_ptr->msg_ptr, msg_data_ptr->msg_len, "MESSAGE_DATA_FAILED");
    TEST_ASSERT_EQUAL_INT_MESSAGE(msg_len, msg_data_ptr->msg_len, "Message length wrong");
    free(msg_data_ptr->msg_ptr);
    free(msg_data_ptr);
}

/*
 * Testcases start here
 *
 *
 *
 * */
#define EID_SILICON_LABS   0x00 ,0x00 ,0x9b ,0xc1

#define MESH_PREFIX             0xfd ,0x00 ,0xdb ,0x08 ,0x00 ,0x00 ,0x00 ,0x00
#define GLOBAL_PREFIX           0x20 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00



#define MAC_ADDR           0x14 ,0x6e ,0x0a ,0x00 ,0x00 ,0x00 ,0x00 ,0x03
#define SERVER_MAC_ADDR    0x14 ,0x6e ,0x0a ,0x02 ,0x00 ,0x01 ,0x04 ,0x00

#define SHORT_ADDR         0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
#define ASSIGNEDSHORT_ADDR 0x00 ,0x00 ,0x00 ,0xff ,0xfe ,0x00 ,0x04 ,0x00

#define IAID      0x00 ,0x00 ,0x00 ,0x01
#define T1        0x00 ,0x00 ,0x00 ,0x00
#define T2        0x00 ,0x00 ,0x00 ,0x00
#define PREF_TIME 0x00 ,0x00 ,0x00 ,0x00
#define REQ_TIME  0x00 ,0x00 ,0x00 ,0x00
#define IA_ADDRESS                 0x00 ,0x05 ,0x00 ,0x18, MESH_PREFIX, SHORT_ADDR, PREF_TIME, REQ_TIME
#define ASSIGNER_ROUTER_IA_ADDRESS 0x00 ,0x05 ,0x00 ,0x18, MESH_PREFIX, ASSIGNEDSHORT_ADDR, PREF_TIME, REQ_TIME


#define ROUTER_ID         0x01
#define ROUTER_MASK       0xC0 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00

#define CLIENT_IDENTIFIER 0x00 ,0x01 ,0x00 ,0x0c ,0x00 ,0x03 ,0x00 ,0x1b ,MAC_ADDR
#define SERVER_IDENTIFIER 0x00 ,0x02 ,0x00 ,0x0c ,0x00 ,0x03 ,0x00 ,0x1b ,SERVER_MAC_ADDR
#define RAPID_COMMIT      0x00 ,0x0e ,0x00 ,0x00
#define ELAPSED_TIME      0x00 ,0x08 ,0x00 ,0x02 ,0x00 ,0x00
#define OPTION_REQUEST    0x00 ,0x06 ,0x00 ,0x02 ,0x00 ,0x11
#define IAFNTA            0x00 ,0x03 ,0x00 ,0x28 ,IAID ,T1 ,T2 ,IA_ADDRESS
#define ASSIGNED_IAFNTA   0x00 ,0x03 ,0x00 ,0x28 ,IAID ,T1 ,T2 ,ASSIGNER_ROUTER_IA_ADDRESS
#define VENDOR_OPTION     0x00 ,0x00 ,0x00 ,0x09 ,ROUTER_ID, ROUTER_MASK
#define VENDOR_SPECIFIC   0x00 ,0x11 ,0x00 ,0x11 ,EID_SILICON_LABS, VENDOR_OPTION

uint8_t router_solicit_req[] = {
    0x01 , 0x00 , 0x00 , 0x01
    , ELAPSED_TIME
    , CLIENT_IDENTIFIER
    , RAPID_COMMIT
    , OPTION_REQUEST
    , IAFNTA
};
uint8_t router_solicit_resp[] = {
    0x07 , 0x00 , 0x00 , 0x01
    , ELAPSED_TIME
    , SERVER_IDENTIFIER
    , CLIENT_IDENTIFIER
    , VENDOR_SPECIFIC
    , RAPID_COMMIT
    , ASSIGNED_IAFNTA
};

#undef PREF_TIME
#undef REQ_TIME

#define PREF_TIME 0x00 ,0x00 ,0x0e ,0x10
#define REQ_TIME  0x00 ,0x00 ,0x1c ,0x20

uint8_t gua_dhcp_addr[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2};
uint8_t gua_prefix[16] = {GLOBAL_PREFIX};
uint8_t gua_assigned_addr[16] = {GLOBAL_PREFIX, ASSIGNEDSHORT_ADDR};

uint8_t leader_addr[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
uint8_t router_addr[16] = {MESH_PREFIX, ASSIGNEDSHORT_ADDR};
uint8_t mesh_link_prefix[16] = {MESH_PREFIX};
uint8_t mac[8] = {MAC_ADDR};
uint8_t server_mac[8] = {SERVER_MAC_ADDR};
uint8_t router_mask[8] = {ROUTER_MASK};


uint8_t arm_router_solicit_resp[] = {
    0x07 , 0x00 , 0x00 , 0x01
    , SERVER_IDENTIFIER
    , CLIENT_IDENTIFIER
    , ASSIGNED_IAFNTA
    , VENDOR_SPECIFIC
    , RAPID_COMMIT
};

/*

Simulated test functions
*/

typedef struct {
    uint16_t instance_id;
    uint32_t msg_tr_id;
    uint8_t msg_name;
    uint8_t *msg_ptr;
    uint16_t msg_len;
} test_recv_req_cb_t;

int sim_dhcp_service_receive_req_cb(uint16_t instance_id, uint32_t msg_tr_id, uint8_t msg_name, uint8_t *msg_ptr, uint16_t msg_len)
{
    test_recv_req_cb_t *msg_data_ptr;
    msg_data_ptr = malloc(sizeof(test_recv_req_cb_t));
    msg_data_ptr->instance_id = instance_id;
    msg_data_ptr->msg_name = msg_name;
    msg_data_ptr->msg_tr_id = msg_tr_id;
    TEST_METHOD_CALLED("dhcp_service_receive_req_cb", msg_data_ptr);
    printf("  instance id = %d\n", instance_id);
    printf("        tr id = %d\n", msg_tr_id);

    return RET_MSG_ACCEPTED;
}
void lt_dhcp_service_receive_req_cb(uint16_t instance_id, uint32_t msg_tr_id, uint8_t msg_name, uint8_t *msg_ptr, uint16_t msg_len)
{
    test_recv_req_cb_t *msg_data_ptr = TEST_METHOD_CHECK("dhcp_service_receive_req_cb");
    TEST_ASSERT_EQUAL_INT_MESSAGE(instance_id, msg_data_ptr->instance_id, "Wrong Instance ID");
    TEST_ASSERT_EQUAL_INT_MESSAGE(msg_name, msg_data_ptr->msg_name, "Wrong msg name ID");
    TEST_ASSERT_EQUAL_INT_MESSAGE(msg_tr_id, msg_data_ptr->msg_tr_id, "Wrong transaction ID");
    free(msg_data_ptr);
}
int ut_dhcp_service_send_resp(uint32_t msg_tr_id, uint8_t options, uint8_t *msg_ptr, uint16_t msg_len)
{
    uint8_t *ptr;
    dhcp_service_send_resp(msg_tr_id, options, msg_ptr, msg_len);
}

uint8_t     routerMask[8] = {ROUTER_MASK};
uint8_t     routerID = ROUTER_ID;

/*

  Testcase definitions

*/


void test_service_server_init(void)
{
    uint16_t instance1, instance2;
    TEST_DESCRIPTION("1 Init multible service\n"
                     "2 receive message for service \n"
                     "3 answer message\n"
                     "6 service delete\n");
    instance1 = dhcp_service_init(1, sim_dhcp_service_receive_req_cb);
    instance2 = dhcp_service_init(1, sim_dhcp_service_receive_req_cb);
    lt_test_socket_recv_data(1, 1, gua_dhcp_addr, router_solicit_req, sizeof(router_solicit_req));
    lt_dhcp_service_receive_req_cb(2, 1/*msg_tr_id*/, 0x01, router_solicit_req, sizeof(router_solicit_req));
    dhcp_service_timer_tick(49);
    dhcp_service_timer_tick(49);
    ut_dhcp_service_send_resp(1/*msg_tr_id*/, 0, router_solicit_resp, sizeof(router_solicit_resp));
    lt_test_socket_sendto(1, gua_dhcp_addr, router_solicit_resp, sizeof(router_solicit_resp));

    dhcp_service_delete(instance1);
    dhcp_service_delete(instance2);
}
void test_service_server_init_wrong_interface(void)
{
    uint16_t instance1, instance2;
    TEST_DESCRIPTION("1 Init multible service\n"
                     "2 receive message for client socket \n"
                     "6 service delete\n");
    instance1 = dhcp_service_init(1, sim_dhcp_service_receive_req_cb);
    instance2 = dhcp_service_init(1, sim_dhcp_service_receive_req_cb);
    lt_test_socket_recv_data(2, 1, gua_dhcp_addr, router_solicit_resp, sizeof(router_solicit_resp));
    dhcp_service_delete(instance1);
    dhcp_service_delete(instance2);
}

void test_router_id_server_init(void)
{
    TEST_DESCRIPTION("1 init router id server\n"
                     "2 receive message for service \n"
                     "3 answer message\n"
                     "6 service delete\n");
    thread_routerid_server_init(1, mesh_link_prefix, server_mac);
    lt_test_socket_recv_data(1, 1, gua_dhcp_addr, router_solicit_req, sizeof(router_solicit_req));
    lt_test_socket_sendto(1, gua_dhcp_addr, arm_router_solicit_resp, sizeof(arm_router_solicit_resp));
    thread_routerid_server_delete(1);
}
uint8_t clt_addr1[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
uint8_t clt_addr2[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2};
uint8_t clt_addr3[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3};
uint8_t clt_addr4[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4};
uint8_t clt_addr5[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5};
uint8_t clt_addr6[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6};
uint8_t clt_addr7[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7};
uint8_t clt_addr8[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8};

void test_service_server_multi_client(void)
{
    uint16_t instance1, instance2;
    TEST_DESCRIPTION("1 Init multible service\n"
                     "2 receive Multiple messages for service from different clients all use same message tr id\n"
                     "3 answer messages\n"
                     "6 service delete\n");
    instance1 = dhcp_service_init(1, sim_dhcp_service_receive_req_cb);
    instance2 = dhcp_service_init(1, sim_dhcp_service_receive_req_cb);
    lt_test_socket_recv_data(1, 1, clt_addr1, router_solicit_req, sizeof(router_solicit_req));
    lt_dhcp_service_receive_req_cb(2, 1/*msg_tr_id*/, 0x01, router_solicit_req, sizeof(router_solicit_req));
    lt_test_socket_recv_data(1, 1, clt_addr2, router_solicit_req, sizeof(router_solicit_req));
    lt_dhcp_service_receive_req_cb(2, 2/*msg_tr_id*/, 0x01, router_solicit_req, sizeof(router_solicit_req));
    lt_test_socket_recv_data(1, 1, clt_addr3, router_solicit_req, sizeof(router_solicit_req));
    lt_dhcp_service_receive_req_cb(2, 3/*msg_tr_id*/, 0x01, router_solicit_req, sizeof(router_solicit_req));
    lt_test_socket_recv_data(1, 1, clt_addr4, router_solicit_req, sizeof(router_solicit_req));
    lt_dhcp_service_receive_req_cb(2, 4/*msg_tr_id*/, 0x01, router_solicit_req, sizeof(router_solicit_req));
    lt_test_socket_recv_data(1, 1, clt_addr5, router_solicit_req, sizeof(router_solicit_req));
    lt_dhcp_service_receive_req_cb(2, 5/*msg_tr_id*/, 0x01, router_solicit_req, sizeof(router_solicit_req));
    lt_test_socket_recv_data(1, 1, clt_addr6, router_solicit_req, sizeof(router_solicit_req));
    lt_dhcp_service_receive_req_cb(2, 6/*msg_tr_id*/, 0x01, router_solicit_req, sizeof(router_solicit_req));
    lt_test_socket_recv_data(1, 1, clt_addr7, router_solicit_req, sizeof(router_solicit_req));
    lt_dhcp_service_receive_req_cb(2, 7/*msg_tr_id*/, 0x01, router_solicit_req, sizeof(router_solicit_req));
    lt_test_socket_recv_data(1, 1, clt_addr8, router_solicit_req, sizeof(router_solicit_req));
    lt_dhcp_service_receive_req_cb(2, 8/*msg_tr_id*/, 0x01, router_solicit_req, sizeof(router_solicit_req));

    ut_dhcp_service_send_resp(1/*msg_tr_id*/, 0, router_solicit_resp, sizeof(router_solicit_resp));
    lt_test_socket_sendto(1, clt_addr1, router_solicit_resp, sizeof(router_solicit_resp));
    ut_dhcp_service_send_resp(1/*msg_tr_id*/, 0, router_solicit_resp, sizeof(router_solicit_resp)); // tests so that no duplicates

    ut_dhcp_service_send_resp(8/*msg_tr_id*/, 0, router_solicit_resp, sizeof(router_solicit_resp));
    lt_test_socket_sendto(1, clt_addr8, router_solicit_resp, sizeof(router_solicit_resp));
    ut_dhcp_service_send_resp(2/*msg_tr_id*/, 0, router_solicit_resp, sizeof(router_solicit_resp));
    lt_test_socket_sendto(1, clt_addr2, router_solicit_resp, sizeof(router_solicit_resp));
    ut_dhcp_service_send_resp(7/*msg_tr_id*/, 0, router_solicit_resp, sizeof(router_solicit_resp));
    lt_test_socket_sendto(1, clt_addr7, router_solicit_resp, sizeof(router_solicit_resp));
    ut_dhcp_service_send_resp(6/*msg_tr_id*/, 0, router_solicit_resp, sizeof(router_solicit_resp));
    lt_test_socket_sendto(1, clt_addr6, router_solicit_resp, sizeof(router_solicit_resp));
    ut_dhcp_service_send_resp(3/*msg_tr_id*/, 0, router_solicit_resp, sizeof(router_solicit_resp));
    lt_test_socket_sendto(1, clt_addr3, router_solicit_resp, sizeof(router_solicit_resp));
    ut_dhcp_service_send_resp(4/*msg_tr_id*/, 0, router_solicit_resp, sizeof(router_solicit_resp));
    lt_test_socket_sendto(1, clt_addr4, router_solicit_resp, sizeof(router_solicit_resp));
// transaction 5 not answered check that no leaks left
    dhcp_service_delete(instance1);
    dhcp_service_delete(instance2);
}
void test_service_server_multi_client_test_2(void)
{
    uint16_t instance1, instance2;
    TEST_DESCRIPTION("1 Init multible service\n"
                     "2 send one client transaction\n"
                     "3 receive Multiple messages for service from different clients all use same message tr id\n"
                     "4 receive response to client transaction\n"
                     "5 answer messages\n"
                     "6 service delete\n");
    instance1 = dhcp_service_init(1, sim_dhcp_service_receive_req_cb);
    instance2 = dhcp_service_init(1, sim_dhcp_service_receive_req_cb);

    dhcp_service_send_req(1, 0, (void *)3333, gua_dhcp_addr, router_solicit_req, sizeof(router_solicit_req), test_dhcp_service_receive_resp_cb_a);
    lt_test_socket_sendto(2, gua_dhcp_addr, router_solicit_req, sizeof(router_solicit_req));

    lt_test_socket_recv_data(1, 1, clt_addr1, router_solicit_req, sizeof(router_solicit_req));
    lt_dhcp_service_receive_req_cb(2, 2/*msg_tr_id*/, 0x01, router_solicit_req, sizeof(router_solicit_req));
    lt_test_socket_recv_data(1, 1, clt_addr2, router_solicit_req, sizeof(router_solicit_req));
    lt_dhcp_service_receive_req_cb(2, 3/*msg_tr_id*/, 0x01, router_solicit_req, sizeof(router_solicit_req));
    lt_test_socket_recv_data(1, 1, clt_addr3, router_solicit_req, sizeof(router_solicit_req));
    lt_dhcp_service_receive_req_cb(2, 4/*msg_tr_id*/, 0x01, router_solicit_req, sizeof(router_solicit_req));

    lt_test_socket_recv_data(1, 2, gua_dhcp_addr, router_solicit_resp, sizeof(router_solicit_resp));
    test_dhcp_service_receive_resp_cb(1, (void *)3333, 0x07,  &router_solicit_resp[4], sizeof(router_solicit_resp) - 4);

    ut_dhcp_service_send_resp(2/*msg_tr_id*/, 0, router_solicit_resp, sizeof(router_solicit_resp));
    lt_test_socket_sendto(1, clt_addr1, router_solicit_resp, sizeof(router_solicit_resp));
    ut_dhcp_service_send_resp(3/*msg_tr_id*/, 0, router_solicit_resp, sizeof(router_solicit_resp));
    lt_test_socket_sendto(1, clt_addr2, router_solicit_resp, sizeof(router_solicit_resp));
    ut_dhcp_service_send_resp(4/*msg_tr_id*/, 0, router_solicit_resp, sizeof(router_solicit_resp));
    lt_test_socket_sendto(1, clt_addr3, router_solicit_resp, sizeof(router_solicit_resp));
    dhcp_service_delete(instance1);
    dhcp_service_delete(instance2);
}


