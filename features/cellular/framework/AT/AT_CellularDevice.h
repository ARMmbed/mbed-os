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

#ifndef AT_CELLULAR_DEVICE_H_
#define AT_CELLULAR_DEVICE_H_

#include "CellularDevice.h"

#include "AT_CellularNetwork.h"
#include "AT_CellularSIM.h"
#include "AT_CellularSMS.h"
#include "AT_CellularPower.h"
#include "AT_CellularInformation.h"

#include "ATHandler.h"

namespace mbed
{

/**
 *  Class AT_CellularDevice
 *
 *  A class defines opening and closing of cellular interfaces.
 *  Deleting/Closing of opened interfaces can be done only through this class.
 */
class AT_CellularDevice : public CellularDevice
{
public:
    AT_CellularDevice(events::EventQueue &queue);
    virtual ~AT_CellularDevice();

protected:
    ATHandler *_atHandlers;

    ATHandler *get_at_handler(FileHandle *fh);

    /** Releases the given at_handler. If last reference to at_hander then it's deleted.
     *
     *  @param at_handler
     */
    void release_at_handler(ATHandler* at_handler);

public: // CellularDevice
    virtual CellularNetwork *open_network(FileHandle *fh);

    virtual CellularSMS *open_sms(FileHandle *fh);

    virtual CellularPower *open_power(FileHandle *fh);

    virtual CellularSIM *open_sim(FileHandle *fh);

    virtual CellularInformation *open_information(FileHandle *fh);

    virtual void close_network();

    virtual void close_sms();

    virtual void close_power();

    virtual void close_sim();

    virtual void close_information();

    virtual void set_timeout(int timeout);

    virtual uint16_t get_send_delay();

    virtual void modem_debug_on(bool on);

    virtual NetworkStack *get_stack();

protected:
    AT_CellularNetwork *_network;
    AT_CellularSMS *_sms;
    AT_CellularSIM *_sim;
    AT_CellularPower* _power;
    AT_CellularInformation* _information;

protected:
    events::EventQueue &_queue;
    int _default_timeout;
    bool _modem_debug_on;
};

} // namespace mbed
#endif // AT_CELLULAR_DEVICE_H_
