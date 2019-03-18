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

#include "ipc_rpc.h"
#include "Mutex.h"
#include "Semaphore.h"
#include "mbed_assert.h"
#include "cy_ipc_config.h"
#include "cy_ipc_pipe.h"
#include <stdarg.h>
#include "platform/SingletonPtr.h"

using namespace rtos;


static SingletonPtr<Mutex>      msg_mutex;
static SingletonPtr<Semaphore>  msg_semaphore;


#define RPC_GEN     RPC_GEN_INTERFACE_IDS
#include "rpc_api.h"
#undef RPC_GEN

// This function uses a "C" linkage as it is a callback called from Cypress library
// which is C-only.
extern "C" void ipcrpc_release(void);
void ipcrpc_release(void)
{
    // Just signal on semaphore that we are done with a call.
    msg_semaphore->release();
}

// Encapsulate call arguments and send a message over IPC pipe to the
// other core for execution.
uint32_t ipcrpc_call(uint32_t call_id, uint32_t args_num, ...)
{
    va_list ap;
    static IpcRpcMessage    message;
    cy_en_ipc_pipe_status_t status;
    ScopedMutexLock         lock(*msg_mutex.get());

    // Make sure semaphore is initialized.
    (void)msg_semaphore.get();

    // Copy data to the buffer.
    message.client_id = call_id;
    message.args_num = args_num;
    message.result = 0; // default result

    va_start(ap, args_num);
    for (uint32_t i = 0; i < args_num; ++i) {
        message.args[i] = va_arg (ap, uint32_t);
    }
    va_end (ap);

    // send message
    status = Cy_IPC_Pipe_SendMessage(CY_IPC_EP_RPCPIPE_DEST,
                                     CY_IPC_EP_RPCPIPE_ADDR,
                                     &message,
                                     ipcrpc_release);
    // We are using dedicated IPC channel here and have locked global mutex
    // so this had to succeed.
    MBED_ASSERT(status == CY_IPC_PIPE_SUCCESS);

    // Now wait for the response;
    msg_semaphore->acquire();

    return message.result;
}

extern "C" {

    void ipcrpc_init(void)
    {
        uint32_t rpc_counter = 0;
#define RPC_GEN     RPC_GEN_INTERFACE_IDS_INIT
#include "rpc_api.h"
#undef RPC_GEN
    }


#define RPC_GEN     RPC_GEN_INTERFACE
#include "rpc_api.h"
#undef RPC_GEN


#define RPC_GEN     RPC_GEN_IMPLEMENTATION
#include "rpc_api.h"
#undef RPC_GEN


} /* extern "C" */

/* [] END OF FILE */
