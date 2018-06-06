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
#include MBED_CONF_APP_HEADER_FILE
#include "udp_tests.h"
#include "UDPSocket.h"
#include "unity/unity.h"
#include "utest.h"

using namespace utest::v1;

namespace
{
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
            ret = sock->open(get_interface());
            if (ret == NSAPI_ERROR_NO_MEMORY || ret == NSAPI_ERROR_NO_SOCKET) {
                printf("[round#%02d] unable to open new socket, error: %d\n", i, ret);
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

        UDPSocketItem *tmp;
        for(UDPSocketItem *it = socket_list_head; it;) {
            ++open_sockets[i];
            tmp = it;
            it = it->next;
            socket_list_head = it;
            delete tmp->sock;
            delete tmp;
        }
        printf("[round#%02d] %d sockets opened\n", i, open_sockets[i]);
    }
    TEST_ASSERT_EQUAL(open_sockets[0], open_sockets[1]);
    // In case of lwIP one is taken by DHCP -> reduction by one to three
    TEST_ASSERT(open_sockets[0] >= 3);
}
