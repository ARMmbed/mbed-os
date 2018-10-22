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

#ifndef IPC_RPC_H
#define IPC_RPC_H

#include <stdint.h>

#if defined(__MBED__)
#define IPCPIPE_ASSERT   MBED_ASSERT
#include "mbed_assert.h"
#else
#include "project.h"
#define IPCPIPE_ASSERT   CY_ASSERT
#endif

#define IPCRPC_MAX_ARGUMENTS        8

/* IPC Pipe message data structure */
typedef struct
{
  uint32_t  client_id;
  uint32_t  result;
  uint32_t  args_num;
  uint32_t  args[IPCRPC_MAX_ARGUMENTS];
} IpcRpcMessage;


typedef struct
{
    volatile uint8_t    busy_flag;
    IpcRpcMessage       message;
} IpcRpcBuffer;




#if defined(__cplusplus)
extern "C"  {
#endif

void        ipcrpc_init(void);
uint32_t    ipcrpc_call(uint32_t call_id, uint32_t args_num, ...);

#if defined(__cplusplus)
}
#endif

#endif /* IPC_RPC_H */
/* [] END OF FILE */
