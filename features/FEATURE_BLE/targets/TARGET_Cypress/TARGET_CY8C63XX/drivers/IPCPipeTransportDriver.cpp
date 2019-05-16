/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
 * Copyright (c) 2017-2018 Future Electronics
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

#include "IPCPipeTransportDriver.h"
#include "ipcpipe_transport.h"
#include "mbed_assert.h"
#include "mbed_error.h"

namespace ble {
namespace vendor {
namespace cypress {


void dump_buffer(uint8_t *buffer, uint32_t len)
{

    while (len > 0) {
        printf(" %02x", *buffer++);
        --len;
    }
    printf("\n");
}

void ipc_h4_receive(uint32_t *ptr)
{
    IpcPipeMessage *message = (IpcPipeMessage *)ptr;

    // We don't expect header to be received from M0+ core.
    MBED_ASSERT(message->header_length == 0);

//    printf("BLE received: ");
//    h4_dump_buffer(buffer->message.data, buffer->message.length);
    cordio::CordioHCITransportDriver::on_data_received(message->data, message->data_length);
}

IPCPipeTransportDriver::IPCPipeTransportDriver()
{ }

void IPCPipeTransportDriver::initialize()
{
//    printf("H4 Transport driver initialization.\n");
    ipcpipe_transport_start(IPCPIPE_CLIENT_H4, ipc_h4_receive, NULL);
}

void IPCPipeTransportDriver::terminate()
{
    ipcpipe_transport_stop(IPCPIPE_CLIENT_H4);
}

uint16_t IPCPipeTransportDriver::write(uint8_t type, uint16_t len, uint8_t *pData)
{
//    printf("BLE sending T<%02x>:", type);
//    dump_buffer(pData, len);
    ipcpipe_write_data(IPCPIPE_CLIENT_H4, &type, 1, pData, len);
    return len;
}

} // namespace cypress
} // namespace vendor
} // namespace ble
