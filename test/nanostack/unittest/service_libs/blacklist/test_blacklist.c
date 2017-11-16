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
#include "test_blacklist.h"
#include <string.h>
#include <inttypes.h>

#include "address.h"
#include "mle.h"
#include "blacklist.h"

#include "mle_stub.h"
#include "address_stub.h"
#include "nsdynmemLIB_stub.h"

bool test_BLACKLIST_init()
{
    nsdynmemlib_stub.returnCounter = 0;
    if (0 == blacklist_init()) {
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    if (0 != blacklist_init()) {
        return false;
    }

    if (0 != blacklist_init()) {
        return false;
    }

    blacklist_params_set(1, 1, 1, 1, 1, 1);
    blacklist_free();

    return true;
}

bool test_BLACKLIST_reject()
{
    nsdynmemlib_stub.returnCounter = 2;

    if (blacklist_reject(NULL)) {
        return false;
    }

    if (0 != blacklist_init()) {
        return false;
    }

    blacklist_params_set(60, 60, 10, 1, 3, 600);

    uint8_t addr[16] = {0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x6e, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x01};

    blacklist_update(addr, false);
    blacklist_ttl_update(5);

    if (!blacklist_reject(addr)) {
        return false;
    }

    blacklist_ttl_update(15);

    if (blacklist_reject(addr)) {
        return false;
    }

    uint8_t addr2[16] = {0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x6e, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x02};

    if (!blacklist_reject(addr2)) {
        return false;
    }

    blacklist_update(addr, true);

    if (blacklist_reject(addr2)) {
        return false;
    }

    blacklist_clear();
    blacklist_free();
    return true;
}

bool test_BLACKLIST_update()
{
    nsdynmemlib_stub.returnCounter = 3;
    uint8_t addr[16] = {0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x6e, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x01};

    blacklist_update(addr, false);

    if (0 != blacklist_init()) {
        return false;
    }

    blacklist_params_set(60, 60, 10, 1, 3, 600);

    blacklist_update(NULL, false);

    blacklist_update(addr, false);
    blacklist_update(addr, false);
    blacklist_update(addr, false);
    blacklist_update(addr, false);

    blacklist_ttl_update(200);

    blacklist_update(addr, false);

    blacklist_clear();
    blacklist_free();
    return true;
}

bool test_BLACKLIST_purge_list()
{
    nsdynmemlib_stub.returnCounter = 3;

    if (0 != blacklist_init()) {
        return false;
    }

    blacklist_params_set(60, 60, 10, 2, 2, 15);

    uint8_t addr[16] = {0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x6e, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x01};
    uint8_t addr2[16] = {0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x6e, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x02};

    blacklist_update(addr, false);
    blacklist_ttl_update(5);

    blacklist_update(addr2, false);
    blacklist_ttl_update(20);

    blacklist_clear();
    blacklist_free();
    return true;
}
