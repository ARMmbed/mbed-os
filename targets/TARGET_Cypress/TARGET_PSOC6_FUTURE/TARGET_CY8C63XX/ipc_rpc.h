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

/** IPC RPC message data structure
 * Used to pass RPC call arguments to M0 core for execution
 */
typedef struct {
    uint32_t  client_id;                    ///< Client ID of the RPC client
    uint32_t  result;                       ///< Function execution result returned from callee to caller
    uint32_t  args_num;                     ///< Number of arguments to RPC function call
    uint32_t  args[IPCRPC_MAX_ARGUMENTS];   ///< Arguments of RPC function call
} IpcRpcMessage;


/** IPC RPC message buffer
 * Used to hold and transfer RPC message
 */
typedef struct {
    volatile uint8_t    busy_flag;          ///< Indicates whether the RPC call using this buffer is in progress
    IpcRpcMessage       message;            ///< RPC message associated with a call
} IpcRpcBuffer;


/** Function handling the RPC call
 * It packs its arguments into the RPC message buffer, initializes transfer
 * and waits for completion.
 *
 * @param call_id     unique identifier of the RPC API function to be executed
 * @param args_num    number of call arguments
 * @param ...         call arguments
 *
 * @return call result (as returned by executed function)
 */
uint32_t    ipcrpc_call(uint32_t call_id, uint32_t args_num, ...);

#if defined(__cplusplus)
extern "C"  {
#endif
/** Initialization function for RPC mechanism.
 * Generated automatically during wrapper generation; needs to be called from startup code.
 */
void        ipcrpc_init(void);
#if defined(__cplusplus)
}
#endif

#endif /* IPC_RPC_H */
/* [] END OF FILE */
