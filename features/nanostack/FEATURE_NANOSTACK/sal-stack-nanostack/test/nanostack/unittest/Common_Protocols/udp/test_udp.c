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
#include "test_udp.h"
#include <string.h>
#include "nsdynmemLIB.h"
#include "Core/include/address.h"
#include "Core/include/ns_buffer.h"

bool test_udp_down()
{
    buffer_t *buf = ns_dyn_mem_alloc(sizeof(buffer_t) + 100);
    buf->src_sa.addr_type = ADDR_NONE;
    udp_down(buf);
    return true;
}

