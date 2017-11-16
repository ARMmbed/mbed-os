/*
 * Copyright (c) 2015-2016, Arm Limited and affiliates.
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
#include "test_thread_bootstrap.h"
#include <string.h>
#include "ns_types.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "event_stub.h"

bool test_thread_bootstrap_tasklet_init()
{
    protocol_interface_info_entry_t et;
    et.bootStrapId = 0;
    if (0 != thread_bootstrap_tasklet_init(&et)) {
        return false;
    }

    et.bootStrapId = -3;
    event_stub.int8_value = -3;
    if (-1 != thread_bootstrap_tasklet_init(&et)) {
        return false;
    }

    return true;
}

