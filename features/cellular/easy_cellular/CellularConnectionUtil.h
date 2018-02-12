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

#ifndef _CELLULAR_CONNECTION_UTIL_H
#define _CELLULAR_CONNECTION_UTIL_H

#include <UARTSerial.h>
#include <NetworkInterface.h>
#include <EventQueue.h>

#include "CellularNetwork.h"
#include "CellularPower.h"

// modem type is defined as CELLULAR_DEVICE macro
#define _CELLULAR_STRINGIFY(a) #a
#define CELLULAR_STRINGIFY(a) _CELLULAR_STRINGIFY(a)
#include "CellularTargets.h"
#include CELLULAR_STRINGIFY(CELLULAR_DEVICE.h)

class CellularConnectionUtil
{
public:
    enum CellularState {
        STATE_POWER_ON = 0,
        STATE_DEVICE_READY = 1,
        STATE_START_CELLULAR = 2,
        STATE_SIM_PIN = 3,
        STATE_REGISTER_NETWORK = 4,
        STATE_REGISTERING_NETWORK = 5,
        STATE_ATTACH_NETWORK = 7,
        STATE_ATTACHING_NETWORK = 8,
        STATE_CONNECT_NETWORK = 9,
        STATE_READY = 10,
    };

public:
    CellularConnectionUtil();
    virtual ~CellularConnectionUtil();
    void set_serial(UARTSerial *serial);
    void set_callback(mbed::Callback<int(int, int)> status_callback);
    EventQueue* get_queue();
    bool start(bool start_dispatch = true);
    void stop();
    CellularNetwork* get_network();
    CellularDevice* get_device();
    bool continue_with_state(CellularState state);

protected:
    bool open_power(FileHandle *fh);
    bool open_sim();
    bool open_network();
    bool get_network_registration(CellularNetwork::RegistrationType type, CellularNetwork::RegistrationStatus &status, bool &is_registered);
    bool set_network_registration(char *plmn = 0);
    bool get_attach_network(CellularNetwork::AttachStatus &status);
    bool set_attach_network();

private:
    void device_ready();
    void report_failure(const char* msg);
    void event();

    UARTSerial *_serial;
    CellularState _state;
    CellularState _next_state;

    mbed::Callback<int(int, int)> _status_callback;

    CellularNetwork *_network;
    CellularPower *_power;
    EventQueue _queue;
    Thread *_queue_thread;
    CellularDevice *_cellularDevice;
};


#endif /* _CELLULAR_CONNECTION_UTIL_H */
