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

#include "CellularTargets.h"
#if defined(CELLULAR_DEVICE) || defined(DOXYGEN_ONLY)

#include "UARTSerial.h"
#include "NetworkInterface.h"
#include "EventQueue.h"
#include "Thread.h"

#include "CellularNetwork.h"
#include "CellularPower.h"
#include "CellularSIM.h"

// modem type is defined as CELLULAR_DEVICE macro
#define _CELLULAR_STRINGIFY(a) #a
#define CELLULAR_STRINGIFY(a) _CELLULAR_STRINGIFY(a)
#include CELLULAR_STRINGIFY(CELLULAR_DEVICE.h)

namespace mbed {

const int PIN_SIZE = 8;
const int MAX_RETRY_ARRAY_SIZE = 10;

/** CellularConnectionFSM class
 *
 *  Finite State Machine for connecting to cellular network
 */
class CellularConnectionFSM
{
public:
    CellularConnectionFSM();
    virtual ~CellularConnectionFSM();

public:
    /** Cellular connection states
     */
    enum CellularState {
        STATE_INIT = 0,
        STATE_POWER_ON,
        STATE_DEVICE_READY,
        STATE_SIM_PIN,
        STATE_REGISTER_NETWORK,
        STATE_REGISTERING_NETWORK,
        STATE_ATTACH_NETWORK,
        STATE_ATTACHING_NETWORK,
        STATE_CONNECT_NETWORK,
        STATE_CONNECTED,
    };

public:
    /** Initialize cellular device
     *  @remark Must be called before any other methods
     *  @return see nsapi_error_t, 0 on success
     */
    nsapi_error_t init();

    /** Set serial connection for cellular device
     *  @param serial UART driver
     */
    void set_serial(UARTSerial *serial);

    /** Set callback for state update
     *  @param status_callback function to call on state changes
     */
    void set_callback(mbed::Callback<bool(int, int)> status_callback);

    /** Get event queue that can be chained to main event queue (or use start_dispatch)
     *  @return event queue
     */
    events::EventQueue* get_queue();

    /** Start event queue dispatching
     *  @return see nsapi_error_t, 0 on success
     */
    nsapi_error_t start_dispatch();

    /** Stop event queue dispatching and close cellular interfaces
     */
    void stop();

    /** Get cellular network interface
     *  @return network interface, NULL on failure
     */
    CellularNetwork* get_network();

    /** Get cellular device interface
     *  @return device interface, NULL on failure
     */
    CellularDevice* get_device();

    /** Get cellular sim interface
     *  @return sim interface, NULL on failure
     */
    CellularSIM* get_sim();

    /** Change cellular connection to the target state
     *  @param state to continue
     *  @return see nsapi_error_t, 0 on success
     */
    nsapi_error_t continue_to_state(CellularState state);

    /** Set cellular device SIM PIN code
     *  @param sim_pin PIN code
     */
    void set_sim_pin(const char *sim_pin);

    /** Sets the timeout array for network rejects. After reject next item is tried and after all items are waited and
     *  still fails then current network event will fail.
     *
     *  @param timeout      timeout array using seconds
     *  @param array_len    length of the array
     */
    void set_retry_timeout_array(uint16_t timeout[], int array_len);

private:
    bool open_power(FileHandle *fh);
    bool open_sim();
    bool get_network_registration(CellularNetwork::RegistrationType type, CellularNetwork::RegistrationStatus &status, bool &is_registered);
    bool set_network_registration(char *plmn = 0);
    bool get_attach_network(CellularNetwork::AttachStatus &status);
    bool set_attach_network();

private:
    friend class EasyCellularConnection;
    NetworkStack *get_stack();

private:
    void device_ready();
    void report_failure(const char* msg);
    void event();

    UARTSerial *_serial;
    CellularState _state;
    CellularState _next_state;

    Callback<bool(int, int)> _status_callback;

    CellularNetwork *_network;
    CellularPower *_power;
    CellularSIM *_sim;
    events::EventQueue _queue;
    rtos::Thread *_queue_thread;
    CellularDevice *_cellularDevice;
    char _sim_pin[PIN_SIZE+1];
    int _retry_count;
    int _state_retry_count;
    int _start_time;
    uint16_t _retry_timeout_array[MAX_RETRY_ARRAY_SIZE];
    int _retry_array_length;
    events::EventQueue _at_queue;
};

} // namespace

#endif // CELLULAR_DEVICE || DOXYGEN

#endif /* _CELLULAR_CONNECTION_UTIL_H */
