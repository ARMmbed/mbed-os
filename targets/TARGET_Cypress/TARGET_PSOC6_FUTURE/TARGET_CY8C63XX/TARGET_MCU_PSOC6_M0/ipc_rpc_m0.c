/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#include "psoc6_utils.h"
#include "ipc_rpc.h"
#include "rpc_defs.h"
#include "cy_ipc_config.h"
#include "cy_ipc_pipe.h"

#define RPC_GEN     RPC_GEN_IMPLEMENTATION
#include "rpc_api.h"
#undef RPC_GEN


void ipcrpc_init(void)
{
    uint32_t rpc_counter = 0;

#define RPC_GEN     RPC_GEN_INITIALIZATION
#include "rpc_api.h"
#undef RPC_GEN
}

/* [] END OF FILE */
