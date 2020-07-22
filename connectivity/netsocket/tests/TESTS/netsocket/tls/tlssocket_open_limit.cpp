/*
 * Copyright (c) 2019, ARM Limited, All Rights Reserved
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
#include "TLSSocket.h"
#include "unity/unity.h"
#include "utest.h"
#include "tls_tests.h"

using namespace utest::v1;

#if defined(MBEDTLS_SSL_CLI_C)

namespace {
typedef struct TLSSocketItem {
    TLSSocket *sock;
    TLSSocketItem *next;
} SocketItem;
}

void TLSSOCKET_OPEN_LIMIT()
{
    SKIP_IF_TCP_UNSUPPORTED();
    int open_sockets[2] = {0};

    for (int i = 0; i < 2; i++) {
        TLSSocketItem *socket_list_head = NULL;
        TLSSocketItem *it;

        TLSSocket *sock;
        int ret;
        while (true) {
            sock = new TLSSocket;
            if (!sock) {
                break;
            }
            ret = sock->open(NetworkInterface::get_default_instance());
            if (ret == NSAPI_ERROR_NO_MEMORY || ret == NSAPI_ERROR_NO_SOCKET) {
                tr_error("[round#%02d] unable to open new socket, error: %d\n", i, ret);
                delete sock;
                break;
            }

            // Hopefully this doesn't interfere when trying to allocate more sockets
            it = new TLSSocketItem;
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
            if ((tls_stats[j].state == SOCK_OPEN) && (tls_stats[j].proto == NSAPI_TCP)) {
                open_count++;
            }
        }
        TEST_ASSERT(open_count >= 4);
#endif

        TLSSocketItem *tmp;
        for (TLSSocketItem *it = socket_list_head; it;) {
            ++open_sockets[i];
            tmp = it;
            it = it->next;
            socket_list_head = it;
            delete tmp->sock;
            delete tmp;
        }
        tr_info("[round#%02d] %d sockets opened\n", i, open_sockets[i]);
    }
    TEST_ASSERT_EQUAL(open_sockets[0], open_sockets[1]);
    TEST_ASSERT(open_sockets[0] >= 4);
}

#endif // defined(MBEDTLS_SSL_CLI_C)
