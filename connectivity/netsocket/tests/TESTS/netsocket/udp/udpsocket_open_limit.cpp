/*
 * Copyright (c) 2018, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "greentea-client/test_env.h"
#include "mbed.h"
#include "udp_tests.h"
#include "UDPSocket.h"
#include "unity/unity.h"
#include "utest.h"
#include "SocketStats.h"

using namespace utest::v1;

namespace {
typedef struct UDPSocketItem {
    UDPSocket *sock;
    UDPSocketItem *next;
} SocketItem;
}

void UDPSOCKET_OPEN_LIMIT()
{
    int open_sockets[2] = {0};

    for (int i = 0; i < 2; i++) {
        UDPSocketItem *socket_list_head = NULL;
        UDPSocketItem *it;

        UDPSocket *sock;
        int ret;
        while (true) {
            sock = new UDPSocket;
            if (!sock) {
                break;
            }
            ret = sock->open(NetworkInterface::get_default_instance());
            if (ret == NSAPI_ERROR_NO_MEMORY || ret == NSAPI_ERROR_NO_SOCKET) {
                tr_info("[round#%02d] unable to open new socket, error: %d", i, ret);
                delete sock;
                break;
            }

            // Hopefully this doesn't interfere when trying to allocate more sockets
            it = new UDPSocketItem;
            if (!it) {
                delete sock;
                break;
            }

            it->sock = sock;
            // Order of items in the list doesn't matter
            it->next = socket_list_head;
            socket_list_head = it;
        }

        if (!socket_list_head) {
            break;
        }
#if MBED_CONF_NSAPI_SOCKET_STATS_ENABLED
        int count = fetch_stats();
        int open_count = 0;
        for (int j = 0; j < count; j++) {
            if ((udp_stats[j].state == SOCK_OPEN) && (udp_stats[j].proto == NSAPI_UDP)) {
                open_count++;
            }
        }
        TEST_ASSERT(open_count >= 3);
#endif
        UDPSocketItem *tmp;
        for (UDPSocketItem *it = socket_list_head; it;) {
            ++open_sockets[i];
            tmp = it;
            it = it->next;
            socket_list_head = it;
            delete tmp->sock;
            delete tmp;
        }
        tr_info("[round#%02d] %d sockets opened", i, open_sockets[i]);
    }
    TEST_ASSERT_EQUAL(open_sockets[0], open_sockets[1]);
    // In case of lwIP one is taken by DHCP -> reduction by one to three
    TEST_ASSERT(open_sockets[0] >= 3);
}
