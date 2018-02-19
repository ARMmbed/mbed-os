/*
 * Copyright (c) , Arm Limited and affiliates.
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

#include "cellular_mux.h"
#include "cellular_mux_stub.h"

using namespace mbed;

ssize_t cellular_mux_stub::size_value = 0;
Mux::MuxReturnStatus cellular_mux_stub::status_value = Mux::MUX_STATUS_SUCCESS;


void Mux::module_init()
{

}

Mux::MuxReturnStatus Mux::mux_start(MuxEstablishStatus &status)
{
    return cellular_mux_stub::status_value;
}

Mux::MuxReturnStatus Mux::dlci_establish(uint8_t dlci_id, MuxEstablishStatus &status, FileHandle **obj)
{
    return cellular_mux_stub::status_value;
}

void Mux::serial_attach(FileHandle *serial)
{

}

void Mux::eventqueue_attach(events::EventQueue *event_queue)
{

}

//Private functions are needed because those are static
ssize_t Mux::user_data_tx(uint8_t dlci_id, const void* buffer, size_t size)
{
    return cellular_mux_stub::size_value;
}

ssize_t Mux::user_data_rx(void* buffer, size_t size)
{
    return cellular_mux_stub::size_value;
}

