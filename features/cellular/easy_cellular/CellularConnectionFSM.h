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
#include "CellularUtil.h"

// modem type is defined as CELLULAR_DEVICE macro
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
        STATE_REGISTERING_NETWORK,
        STATE_MANUAL_REGISTERING_NETWORK,
        STATE_ATTACHING_NETWORK,
        STATE_ACTIVATING_PDP_CONTEXT,
        STATE_CONNECTING_NETWORK,
        STATE_CONNECTED
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

    /** Register callback for status reporting
     *
     *  The specified status callback function will be called on status changes
     *  on the network. The parameters on the callback are the event type and
     *  event-type dependent reason parameter.
     *
     *  @param status_cb The callback for status changes
     */
    virtual void attach(mbed::Callback<void(nsapi_event_t, intptr_t)> status_cb);

    /** Get event queue that can be chained to main event queue (or use start_dispatch)
     *  @return event queue
     */
    events::EventQueue *get_queue();

    /** Start event queue dispatching
     *  @return see nsapi_error_t, 0 on success
     */
    nsapi_error_t start_dispatch();

    /** Stop event queue dispatching and close cellular interfaces. After calling stop(), init() must be called
     *  before any other methods.
     */
    void stop();

    /** Get cellular network interface
     *  @return network interface, NULL on failure
     */
    CellularNetwork *get_network();

    /** Get cellular device interface
     *  @return device interface, NULL on failure
     */
    CellularDevice *get_device();

    /** Get cellular sim interface. SIM interface is released when moving from STATE_ATTACHING_NETWORK to STATE_ACTIVATING_PDP_CONTEXT.
     *  After SIM interface is closed, this method returns NULL, and any instances fetched using this method are invalid.
     *  SIM interface can be created again using CellularDevice, which you can get with the method get_device().
     *  @return sim interface, NULL on failure
     */
    CellularSIM *get_sim();

    /** Change cellular connection to the target state
     *  @param state to continue. Default is to connect.
     *  @return see nsapi_error_t, 0 on success
     */
    nsapi_error_t continue_to_state(CellularState state = STATE_CONNECTED);

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

    /** Sets the operator plmn which is used when registering to a network specified by plmn. If plmn is not set then automatic
     *  registering is used when registering to a cellular network. Does not start any operations.
     *
     *  @param plmn operator in numeric format. See more from 3GPP TS 27.007 chapter 7.3.
     */
    void set_plmn(const char* plmn);

    /** returns readable format of the given state. Used for printing states while debugging.
     *
     *  @param state state which is returned in string format
     *  @return      string format of the given state
     */
    const char *get_state_string(CellularState state);

private:
    bool power_on();
    bool open_sim();
    bool get_network_registration(CellularNetwork::RegistrationType type, CellularNetwork::RegistrationStatus &status, bool &is_registered);
    bool is_registered();
    void device_ready();

    // state functions to keep state machine simple
    void state_init();
    void state_power_on();
    void state_device_ready();
    void state_sim_pin();
    void state_registering();
    void state_manual_registering_network();
    void state_attaching();
    void state_activating_pdp_context();
    void state_connect_to_network();
    void state_connected();
    void enter_to_state(CellularState state);
    void retry_state_or_fail();
    void network_callback(nsapi_event_t ev, intptr_t ptr);
    nsapi_error_t continue_from_state(CellularState state);
    bool is_registered_to_plmn();

private:
    friend class EasyCellularConnection;
    NetworkStack *get_stack();

private:
    void report_failure(const char* msg);
    void event();
    void ready_urc_cb();

    UARTSerial *_serial;
    CellularState _state;
    CellularState _next_state;

    Callback<bool(int, int)> _status_callback;
    Callback<void(nsapi_event_t, intptr_t)> _event_status_cb;

    CellularNetwork *_network;
    CellularPower *_power;
    CellularSIM *_sim;
    events::EventQueue _queue;
    rtos::Thread *_queue_thread;
    CellularDevice *_cellularDevice;
    char _sim_pin[PIN_SIZE+1];
    int _retry_count;
    int _start_time;
    int _event_timeout;

    uint16_t _retry_timeout_array[MAX_RETRY_ARRAY_SIZE];
    int _retry_array_length;
    events::EventQueue _at_queue;
    char _st_string[20];
    int _event_id;
    const char *_plmn;
    bool _command_success;
    bool _plmn_network_found;
};

} // namespace

#endif // CELLULAR_DEVICE || DOXYGEN

#endif /* _CELLULAR_CONNECTION_UTIL_H */
