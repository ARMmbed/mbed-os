/*
 * Copyright (c) 2017, Arm Limited and affiliates.
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

#include "AT_CellularDevice.h"

AT_CellularDevice::AT_CellularDevice(EventQueue &queue) :
    _atHandlers(0), _network(0), _sms(0), _sim(0), _power(0), _multiplexer(0), _information(0), _queue(queue)
{
}

AT_CellularDevice::~AT_CellularDevice()
{
}

ATHandler* AT_CellularDevice::get_at_handler(FileHandle *fileHandle)
{
    return NULL;
}

void AT_CellularDevice::release_at_handler(ATHandler* at_handler)
{
    
}

CellularNetwork *AT_CellularDevice::open_network(FileHandle *fh)
{
    return NULL;
}

CellularSMS *AT_CellularDevice::open_sms(FileHandle *fh)
{
    return NULL;
}

CellularSIM *AT_CellularDevice::open_sim(FileHandle *fh)
{
    return NULL;
}

CellularPower *AT_CellularDevice::open_power(FileHandle *fh)
{
    return NULL;
}

CellularMultiplexer *AT_CellularDevice::open_multiplexer(FileHandle *fh)
{
    return NULL;
}

CellularInformation *AT_CellularDevice::open_information(FileHandle *fh)
{
    return NULL;
}

void AT_CellularDevice::close_network()
{
}

void AT_CellularDevice::close_sms()
{
}

void AT_CellularDevice::close_power()
{
}

void AT_CellularDevice::close_sim()
{
}

void AT_CellularDevice::close_multiplexer()
{
}

void AT_CellularDevice::close_information()
{
}
