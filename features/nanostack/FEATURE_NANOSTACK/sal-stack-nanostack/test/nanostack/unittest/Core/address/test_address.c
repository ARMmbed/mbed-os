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
#include "test_address.h"
#include <string.h>
#include "NWK_INTERFACE/Include/protocol.h"


bool test_addr_len_from_type()
{
    addr_len_from_type(0);
    return true;
}

bool test_addr_max_slaac_entries_set()
{
    protocol_interface_info_entry_t cur;
    addr_max_slaac_entries_set(&cur, 5);

    if (cur.ip_addresses_max_slaac_entries == 5) {
        return true;
    } else {
        return false;
    }
}
