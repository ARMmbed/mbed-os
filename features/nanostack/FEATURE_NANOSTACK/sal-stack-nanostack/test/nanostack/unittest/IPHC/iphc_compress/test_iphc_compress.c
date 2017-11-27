/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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
#include "test_iphc_compress.h"
#include <string.h>
#include "Core/include/ns_buffer.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "nsdynmemLIB_stub.h"

bool test_iphc_compress()
{
    protocol_interface_info_entry_t info;
    nsdynmemlib_stub.returnCounter = 1;
    buffer_t buf;
    memset(&buf, 0, sizeof(buffer_t));
    iphc_compress(&info, &buf, 0);
    return true;
}

