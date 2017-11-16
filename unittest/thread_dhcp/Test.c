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

typedef struct {
    int8_t interface;
    uint8_t dhcp_addr[16];
    uint8_t mesh_link_prefix[16];
    uint8_t mac64[8];
} test_ctx_t;

test_ctx_t ctx;

uint8_t test_router_id;
uint8_t test_router_mask_ptr[8];
void dhcp_router_id_cb(int8_t interface_id, uint8_t *ml16_address, uint8_t router_id,  const uint8_t router_mask_ptr[static 8])
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

/*
 * Testcases start here
 *
 *
 *
 * */



#define EID_SILICON_LABS     0x00 ,0x00 ,0x9b ,0xc1

#define MAC_ADDR             0x14 ,0x6e ,0x0a ,0x00 ,0x00 ,0x00 ,0x00 ,0x03
#define SERVER_MAC_ADDR      0x14 ,0x6e ,0x0a ,0x02 ,0x00 ,0x01 ,0x04 ,0x00

#define MESH_PREFIX          0xfd ,0x00 ,0xdb ,0x08 ,0x00 ,0x00 ,0x00 ,0x00
#define GLOBAL_PREFIX        0x20 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00

#define EMPTY_SHORT_ADDR     0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
#define ASSIGNED_ROUTER_ADDR 0x00 ,0x00 ,0x00 ,0xff ,0xfe ,0x00 ,0x04 ,0x00

#define IAID      0x00 ,0x00 ,0x00 ,0x01
#define T1        0x00 ,0x00 ,0x00 ,0x00
#define T2        0x00 ,0x00 ,0x00 ,0x00
#define PREF_TIME 0x00 ,0x00 ,0x00 ,0x00
#define REQ_TIME  0x00 ,0x00 ,0x00 ,0x00
#define EMPTY_ROUTER_IA_ADDRESS    0x00 ,0x05 ,0x00 ,0x18, MESH_PREFIX, EMPTY_SHORT_ADDR, PREF_TIME, REQ_TIME
#define ASSIGNER_ROUTER_IA_ADDRESS 0x00 ,0x05 ,0x00 ,0x18, MESH_PREFIX, ASSIGNED_ROUTER_ADDR, PREF_TIME, REQ_TIME


#define ROUTER_ID         0x09
#define ROUTER_MASK       0xC0 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
#define VENDOR_OPTION           0x00 ,0x00 ,0x00 ,0x09 ,ROUTER_ID, ROUTER_MASK
#define VENDOR_SPECIFIC         0x00 ,0x11 ,0x00 ,0x11 ,EID_SILICON_LABS, VENDOR_OPTION

#define CLIENT_IDENTIFIER       0x00 ,0x01 ,0x00 ,0x0c ,0x00 ,0x03 ,0x00 ,0x1b ,MAC_ADDR
#define SERVER_IDENTIFIER       0x00 ,0x02 ,0x00 ,0x0c ,0x00 ,0x03 ,0x00 ,0x1b ,SERVER_MAC_ADDR
#define RAPID_COMMIT            0x00 ,0x0e ,0x00 ,0x00
#define ELAPSED_TIME            0x00 ,0x08 ,0x00 ,0x02 ,0x00 ,0x00
#define OPTION_REQUEST_VENDOR   0x00 ,0x06 ,0x00 ,0x02 ,0x00 ,0x11
#define OPTION_REQUEST_EMPTY    0x00 ,0x06 ,0x00 ,0x00
#define EMPTY_ROUTER_IAFNTA     0x00 ,0x03 ,0x00 ,0x28 ,IAID ,T1 ,T2 ,EMPTY_ROUTER_IA_ADDRESS
#define ASSIGNED_ROUTER_IAFNTA  0x00 ,0x03 ,0x00 ,0x28 ,IAID ,T1 ,T2 ,ASSIGNER_ROUTER_IA_ADDRESS

/*Global address defines*/
#define EMPTY_GUA_IA_ADDRESS           0x00 ,0x05 ,0x00 ,0x18, GLOBAL_PREFIX, EMPTY_SHORT_ADDR, PREF_TIME, REQ_TIME
#define EMPTY_GUA_IAFNTA               0x00 ,0x03 ,0x00 ,0x28 ,IAID ,T1 ,T2 ,EMPTY_GUA_IA_ADDRESS
#define GUA_ASSIGNED_ADDR              0x00 ,0x00 ,0x00 ,0x00 ,0x04 ,0x03 ,0x02 ,0x01
#define ASSIGNED_GUA_IA_ADDRESS        0x00 ,0x05 ,0x00 ,0x18, GLOBAL_PREFIX, GUA_ASSIGNED_ADDR, PREF_TIME, REQ_TIME
#define ASSIGNED_GUA_IAFNTA            0x00 ,0x03 ,0x00 ,0x28 ,IAID ,T1 ,T2 ,ASSIGNED_GUA_IA_ADDRESS


uint8_t router_solicit_req[] = {
    0x01 , 0x00 , 0x00 , 0x01
    , ELAPSED_TIME
    , CLIENT_IDENTIFIER
    , RAPID_COMMIT
    , OPTION_REQUEST_VENDOR
    , EMPTY_ROUTER_IAFNTA
};
uint8_t router_solicit_resp[] = {
    0x07 , 0x00 , 0x00 , 0x01
    , ELAPSED_TIME
    , SERVER_IDENTIFIER
    , CLIENT_IDENTIFIER
    , VENDOR_SPECIFIC
    , RAPID_COMMIT
    , ASSIGNED_ROUTER_IAFNTA
};

uint8_t gua_dhcp_addr[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2};
uint8_t gua_prefix[16] = {GLOBAL_PREFIX};
uint8_t gua_assigned_addr[16] = {GLOBAL_PREFIX, GUA_ASSIGNED_ADDR};

uint8_t leader_addr[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
uint8_t router_addr[16] = {MESH_PREFIX, ASSIGNED_ROUTER_ADDR};
uint8_t mesh_link_prefix[16] = {MESH_PREFIX};
uint8_t mac[8] = {MAC_ADDR};
uint8_t server_mac[8] = {SERVER_MAC_ADDR};
uint8_t router_mask[8] = {ROUTER_MASK};

void test_client_init(void)
{
    TEST_DESCRIPTION("1 test service create and delete\n");
    TEST_ASSERT_EQUAL_INT(0, lt_test_sockets_open());
    thread_dhcp_client_init(1);
    TEST_ASSERT_EQUAL_INT(2, lt_test_sockets_open());
    thread_dhcp_client_delete(1);
    TEST_ASSERT_EQUAL_INT(0, lt_test_sockets_open());
    thread_dhcp_client_init(2);
    TEST_ASSERT_EQUAL_INT(2, lt_test_sockets_open());
    thread_dhcp_client_delete(2);
    TEST_ASSERT_EQUAL_INT(0, lt_test_sockets_open());
}



uint8_t gua_solicit_req[] = {
    0x01 , 0x00 , 0x00 , 0x01
    , ELAPSED_TIME
    , CLIENT_IDENTIFIER
    , RAPID_COMMIT
    , OPTION_REQUEST_EMPTY
    , EMPTY_GUA_IAFNTA
};
uint8_t gua_solicit_resp[] = {
    0x07 , 0x00 , 0x00 , 0x01
    , ELAPSED_TIME
    , SERVER_IDENTIFIER
    , CLIENT_IDENTIFIER
    , VENDOR_SPECIFIC
    , RAPID_COMMIT
    , ASSIGNED_GUA_IAFNTA
};

void test_client_get_global_address(void)
{
    TEST_DESCRIPTION("1 get Global address\n"
                     "2 delete Global address manually\n"
                     "3 delete service\n");
    thread_dhcp_client_init(1);
    thread_dhcp_client_get_global_address(1, gua_dhcp_addr, gua_prefix, mac, NULL);
    lt_test_socket_sendto(2, gua_dhcp_addr, gua_solicit_req, sizeof(gua_solicit_req));
    lt_test_socket_recv_data(1, 2, gua_dhcp_addr, gua_solicit_resp, sizeof(gua_solicit_resp));
    lt_test_addr_add(gua_assigned_addr);

    thread_dhcp_client_global_address_delete(1, gua_dhcp_addr, gua_prefix); // Delete GUA by hand
    lt_test_addr_delete(gua_assigned_addr);
    thread_dhcp_client_delete(1);
}

void test_client_get_global_address_retry(void)
{
    TEST_DESCRIPTION("1 get Global address\n"
                     "2 no response, make retransmission\n"
                     "3 receive reply \n"
                     "4 delete service\n"
                     "5 service deletes allocated address\n");
    thread_dhcp_client_init(1);
    thread_dhcp_client_get_global_address(1, gua_dhcp_addr, gua_prefix, mac, NULL);
    lt_test_socket_sendto(2, gua_dhcp_addr, gua_solicit_req, sizeof(gua_solicit_req));

    dhcp_service_timer_tick(10);
    dhcp_service_timer_tick(10);
    lt_test_socket_sendto(2, gua_dhcp_addr, gua_solicit_req, sizeof(gua_solicit_req));

    lt_test_socket_recv_data(1, 2, gua_dhcp_addr, gua_solicit_resp, sizeof(gua_solicit_resp));
    lt_test_addr_add(gua_assigned_addr);

    thread_dhcp_client_delete(1);
    lt_test_addr_delete(gua_assigned_addr);
}

uint8_t non_assigned_gua[16] = {GLOBAL_PREFIX, 0};
void test_client_get_global_address_retry_no_response(void)
{
    TEST_DESCRIPTION("1 get Global address\n"
                     "2 no response, make retransmissions\n"
                     "3 delete service\n");
    thread_dhcp_client_init(1);
    thread_dhcp_client_get_global_address(1, gua_dhcp_addr, gua_prefix, mac, NULL);
    lt_test_socket_sendto(2, gua_dhcp_addr, gua_solicit_req, sizeof(gua_solicit_req));

    dhcp_service_timer_tick(11); // Allow for max 1.1 initial randomisation
    lt_test_socket_sendto(2, gua_dhcp_addr, gua_solicit_req, sizeof(gua_solicit_req));
    dhcp_service_timer_tick(24); // Allow for max 2.1 randomisation each retry
    lt_test_socket_sendto(2, gua_dhcp_addr, gua_solicit_req, sizeof(gua_solicit_req));
    dhcp_service_timer_tick(49);
    lt_test_socket_sendto(2, gua_dhcp_addr, gua_solicit_req, sizeof(gua_solicit_req));
    dhcp_service_timer_tick(102);
    lt_test_socket_sendto(2, gua_dhcp_addr, gua_solicit_req, sizeof(gua_solicit_req));
    dhcp_service_timer_tick(214);
    lt_test_socket_sendto(2, gua_dhcp_addr, gua_solicit_req, sizeof(gua_solicit_req));
    dhcp_service_timer_tick(450);
    lt_test_socket_sendto(2, gua_dhcp_addr, gua_solicit_req, sizeof(gua_solicit_req));
    dhcp_service_timer_tick(944);
    lt_test_socket_sendto(2, gua_dhcp_addr, gua_solicit_req, sizeof(gua_solicit_req));
    dhcp_service_timer_tick(1982);
    lt_test_socket_sendto(2, gua_dhcp_addr, gua_solicit_req, sizeof(gua_solicit_req));

    thread_dhcp_client_delete(1);
    lt_test_addr_delete(non_assigned_gua);// TODO This is bug should fix add boolean when got address.
}

#undef T1
#undef T2
#define T1                         0x00 ,0x00 ,0x00 ,0x32
#define T2                         0x00 ,0x00 ,0x00 ,0x4b
#define ASSIGNED_GUA_IAFNTA        0x00 ,0x03 ,0x00 ,0x28 ,IAID ,T1 ,T2 ,ASSIGNED_GUA_IA_ADDRESS
uint8_t gua_renew_req[] = {
    0x05 , 0x00 , 0x00 , 0x01
    , ELAPSED_TIME
    , CLIENT_IDENTIFIER
    , SERVER_IDENTIFIER
    , RAPID_COMMIT
    , OPTION_REQUEST_EMPTY
    , ASSIGNED_GUA_IAFNTA
};
uint8_t gua_renew_resp[] = {
    0x07 , 0x00 , 0x00 , 0x01
    , ELAPSED_TIME
    , SERVER_IDENTIFIER
    , CLIENT_IDENTIFIER
    , VENDOR_SPECIFIC
    , RAPID_COMMIT
    , ASSIGNED_GUA_IAFNTA
};

void test_client_global_address_renew(void)
{
    TEST_DESCRIPTION("1 get Global address\n"
                     "2 receive reply \n"
                     "3 receive timeout for global address\n"
                     "4 send renew\n"
                     "5 recv renew\n"
                     "6 delete address\n"
                     "7 service delete\n");
    thread_dhcp_client_init(1);
    thread_dhcp_client_get_global_address(1, gua_dhcp_addr, gua_prefix, mac, NULL);
    lt_test_socket_sendto(2, gua_dhcp_addr, gua_solicit_req, sizeof(gua_solicit_req));
    lt_test_socket_recv_data(1, 2, gua_dhcp_addr, gua_solicit_resp, sizeof(gua_solicit_resp));
    lt_test_addr_add(gua_assigned_addr);
    lt_timeout_address(1, gua_assigned_addr);
    lt_test_socket_sendto(2, gua_dhcp_addr, gua_renew_req, sizeof(gua_renew_req));
    lt_test_socket_recv_data(1, 2, gua_dhcp_addr, gua_renew_resp, sizeof(gua_renew_resp));
    thread_dhcp_client_global_address_delete(1, gua_dhcp_addr, gua_prefix); // Delete GUA by hand
    lt_test_addr_delete(gua_assigned_addr);
    thread_dhcp_client_delete(1);
}

#undef T1
#undef T2
#define T1                         0x00 ,0x00 ,0x00 ,0x32
#define T2                         0x00 ,0x00 ,0x00 ,0x4b
#define ASSIGNED_ROUTER_IAFNTA  0x00 ,0x03 ,0x00 ,0x28 ,IAID ,T1 ,T2 ,ASSIGNER_ROUTER_IA_ADDRESS

uint8_t router_renew_req[] = {
    0x05 , 0x00 , 0x00 , 0x01
    , ELAPSED_TIME
    , CLIENT_IDENTIFIER
    , SERVER_IDENTIFIER
    , RAPID_COMMIT
    , OPTION_REQUEST_VENDOR
    , ASSIGNED_ROUTER_IAFNTA
};
uint8_t router_renew_resp[] = {
    0x07 , 0x00 , 0x00 , 0x01
    , ELAPSED_TIME
    , SERVER_IDENTIFIER
    , CLIENT_IDENTIFIER
    , VENDOR_SPECIFIC
    , RAPID_COMMIT
    , ASSIGNED_ROUTER_IAFNTA
};

